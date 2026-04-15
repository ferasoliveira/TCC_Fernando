/*
 * ============================================================================
 *  EB-15 Robotic Arm — Arduino Uno Executor Firmware
 * ============================================================================
 *  Responsabilidade: Executar segmentos de movimento para J4, J5, J6
 *  Comunicação: Serial UART com ESP32-S3 (protocolo binário)
 *  Este firmware NÃO planeja trajetórias. Apenas executa segmentos recebidos.
 *
 *  Seções deste arquivo:
 *    1. Configuração e Constantes
 *    2. Enums e Structs
 *    3. Variáveis Globais
 *    4. CRC-8
 *    5. Protocolo Serial (parser, envio)
 *    6. Fila de Segmentos
 *    7. Controle de Motores (J4–J6)
 *    8. Homing
 *    9. Watchdog de Comunicação
 *   10. Status e Reporting
 *   11. Setup e Loop
 * ============================================================================
 */

// ============================================================================
//  SEÇÃO 1: CONFIGURAÇÃO E CONSTANTES
// ============================================================================

// --- Pinagem dos motores (TB6600) ---
#define PUL_J4  2
#define DIR_J4  3
#define PUL_J5  4
#define DIR_J5  5
#define PUL_J6  6
#define DIR_J6  7

// --- Chaves de fim de curso (PLACEHOLDER — definir quando disponíveis) ---
// #define LIMIT_J4  8
// #define LIMIT_J5  9
// #define LIMIT_J6  10

// --- Comunicação Serial ---
#define SERIAL_BAUD       115200
#define FRAME_START       0xAA
#define FRAME_END         0x55
#define MAX_FRAME_SIZE    64
#define MAX_PAYLOAD_SIZE  59

// --- Fila de segmentos ---
#define SEGMENT_QUEUE_SIZE  8

// --- Timing ---
#define HEARTBEAT_TIMEOUT_MS   2000
#define STATUS_INTERVAL_MS     100
#define MIN_STEP_INTERVAL_US   50

// --- Motor ---
#define PULSE_WIDTH_US  5

// ============================================================================
//  SEÇÃO 2: ENUMS E STRUCTS
// ============================================================================

// --- Estados do executor ---
enum ExecutorState : uint8_t {
  EXEC_IDLE       = 0,
  EXEC_RUNNING    = 1,
  EXEC_HOMING     = 2,
  EXEC_FAULT      = 3,
  EXEC_STOPPED    = 4
};

// --- Códigos de falha ---
enum FaultCode : uint8_t {
  FAULT_NONE            = 0,
  FAULT_SERIAL_TIMEOUT  = 1,
  FAULT_LIMIT_HIT       = 2,
  FAULT_HOMING_FAILED   = 3,
  FAULT_QUEUE_OVERRUN   = 4
};

// --- Comandos do protocolo ---
enum SerialCmd : uint8_t {
  CMD_HEARTBEAT   = 0x01,
  CMD_ACK         = 0x02,
  CMD_NACK        = 0x03,
  CMD_SEGMENT     = 0x10,
  CMD_STOP        = 0x11,
  CMD_HOME        = 0x12,
  CMD_STATUS_REQ  = 0x13,
  CMD_SET_POS     = 0x14,
  CMD_STATUS_RSP  = 0x20,
  CMD_FAULT_NOTIFY = 0x21,
  CMD_HOME_DONE   = 0x22,
  CMD_SEG_DONE    = 0x23
};

// --- Códigos de erro NACK ---
enum NackError : uint8_t {
  ERR_CRC           = 0x01,
  ERR_UNKNOWN_CMD   = 0x02,
  ERR_QUEUE_FULL    = 0x03,
  ERR_INVALID_STATE = 0x04,
  ERR_PAYLOAD_SIZE  = 0x05
};

// --- Segmento de movimento ---
struct Segment {
  uint8_t  seq;
  int16_t  steps_j4;
  int16_t  steps_j5;
  int16_t  steps_j6;
  uint16_t duration_ms;
  uint8_t  flags;       // bit 0: último segmento, bit 1: sincronizar
};

// --- Frame serial recebido ---
struct SerialFrame {
  uint8_t cmd;
  uint8_t payload[MAX_PAYLOAD_SIZE];
  uint8_t payload_len;
  bool    valid;
};

// ============================================================================
//  SEÇÃO 3: VARIÁVEIS GLOBAIS
// ============================================================================

// --- Estado ---
volatile ExecutorState g_state = EXEC_IDLE;
volatile FaultCode     g_fault = FAULT_NONE;

// --- Posição estimada (passos) ---
volatile int32_t g_pos_j4 = 0;
volatile int32_t g_pos_j5 = 0;
volatile int32_t g_pos_j6 = 0;

// --- Fila de segmentos ---
Segment g_queue[SEGMENT_QUEUE_SIZE];
volatile uint8_t g_queue_head = 0;
volatile uint8_t g_queue_tail = 0;
volatile uint8_t g_queue_count = 0;

// --- Segmento em execução ---
volatile uint8_t g_current_seq = 0;
volatile bool    g_segment_active = false;

// --- Watchdog ---
volatile unsigned long g_last_heartbeat = 0;

// --- Buffer de recepção serial ---
uint8_t  g_rx_buf[MAX_FRAME_SIZE];
uint8_t  g_rx_idx = 0;
bool     g_rx_in_frame = false;

// ============================================================================
//  SEÇÃO 4: CRC-8
// ============================================================================

uint8_t crc8(const uint8_t* data, uint8_t len) {
  uint8_t crc = 0x00;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x80)
        crc = (crc << 1) ^ 0x07;
      else
        crc = crc << 1;
    }
  }
  return crc;
}

// ============================================================================
//  SEÇÃO 5: PROTOCOLO SERIAL
// ============================================================================

// --- Enviar frame ---
void sendFrame(uint8_t cmd, const uint8_t* payload, uint8_t payload_len) {
  uint8_t frame[MAX_FRAME_SIZE];
  uint8_t idx = 0;

  uint8_t len = 1 + payload_len; // CMD + PAYLOAD

  frame[idx++] = FRAME_START;
  frame[idx++] = len;
  frame[idx++] = cmd;

  for (uint8_t i = 0; i < payload_len; i++) {
    frame[idx++] = payload[i];
  }

  // CRC sobre LEN + CMD + PAYLOAD
  uint8_t crc = crc8(&frame[1], 1 + len); // LEN byte + CMD + PAYLOAD
  frame[idx++] = crc;
  frame[idx++] = FRAME_END;

  Serial.write(frame, idx);
}

// --- Enviar ACK ---
void sendAck(uint8_t acknowledged_cmd) {
  uint8_t payload[1] = { acknowledged_cmd };
  sendFrame(CMD_ACK, payload, 1);
}

// --- Enviar NACK ---
void sendNack(uint8_t failed_cmd, uint8_t error_code) {
  uint8_t payload[2] = { failed_cmd, error_code };
  sendFrame(CMD_NACK, payload, 2);
}

// --- Enviar heartbeat ---
void sendHeartbeat() {
  sendFrame(CMD_HEARTBEAT, NULL, 0);
}

// --- Enviar status ---
void sendStatusResponse() {
  uint8_t payload[16];
  uint8_t idx = 0;

  payload[idx++] = (uint8_t)g_state;

  // pos_j4 (int32_t, little-endian)
  int32_t p4 = g_pos_j4;
  payload[idx++] = (uint8_t)(p4 & 0xFF);
  payload[idx++] = (uint8_t)((p4 >> 8) & 0xFF);
  payload[idx++] = (uint8_t)((p4 >> 16) & 0xFF);
  payload[idx++] = (uint8_t)((p4 >> 24) & 0xFF);

  // pos_j5
  int32_t p5 = g_pos_j5;
  payload[idx++] = (uint8_t)(p5 & 0xFF);
  payload[idx++] = (uint8_t)((p5 >> 8) & 0xFF);
  payload[idx++] = (uint8_t)((p5 >> 16) & 0xFF);
  payload[idx++] = (uint8_t)((p5 >> 24) & 0xFF);

  // pos_j6
  int32_t p6 = g_pos_j6;
  payload[idx++] = (uint8_t)(p6 & 0xFF);
  payload[idx++] = (uint8_t)((p6 >> 8) & 0xFF);
  payload[idx++] = (uint8_t)((p6 >> 16) & 0xFF);
  payload[idx++] = (uint8_t)((p6 >> 24) & 0xFF);

  payload[idx++] = SEGMENT_QUEUE_SIZE - g_queue_count; // queue_free
  payload[idx++] = (uint8_t)g_fault;
  payload[idx++] = g_current_seq;

  sendFrame(CMD_STATUS_RSP, payload, idx);
}

// --- Enviar fault notify ---
void sendFaultNotify(uint8_t fault_code, uint8_t detail) {
  uint8_t payload[2] = { fault_code, detail };
  sendFrame(CMD_FAULT_NOTIFY, payload, 2);
}

// --- Enviar home done ---
void sendHomeDone(uint8_t result) {
  uint8_t payload[1] = { result };
  sendFrame(CMD_HOME_DONE, payload, 1);
}

// --- Enviar segment done ---
void sendSegmentDone(uint8_t seq) {
  uint8_t payload[1] = { seq };
  sendFrame(CMD_SEG_DONE, payload, 1);
}

// --- Parser serial (byte a byte) ---
SerialFrame parseSerialByte(uint8_t byte_in) {
  SerialFrame result;
  result.valid = false;

  if (!g_rx_in_frame) {
    if (byte_in == FRAME_START) {
      g_rx_in_frame = true;
      g_rx_idx = 0;
      g_rx_buf[g_rx_idx++] = byte_in;
    }
    return result;
  }

  g_rx_buf[g_rx_idx++] = byte_in;

  // Overflow protection
  if (g_rx_idx >= MAX_FRAME_SIZE) {
    g_rx_in_frame = false;
    g_rx_idx = 0;
    return result;
  }

  // Check if we have enough data: START + LEN + at least 1 (CMD) + CRC + END
  if (g_rx_idx < 4) return result;

  uint8_t len = g_rx_buf[1]; // LEN = CMD + PAYLOAD length

  // Expected total frame size: START(1) + LEN(1) + CMD+PAYLOAD(len) + CRC(1) + END(1)
  uint8_t expected_size = 1 + 1 + len + 1 + 1;

  if (g_rx_idx < expected_size) return result;

  // We have a complete frame
  g_rx_in_frame = false;

  // Validate END byte
  if (g_rx_buf[expected_size - 1] != FRAME_END) {
    g_rx_idx = 0;
    return result;
  }

  // Validate CRC: CRC is over LEN + CMD + PAYLOAD
  uint8_t received_crc = g_rx_buf[expected_size - 2];
  uint8_t calculated_crc = crc8(&g_rx_buf[1], 1 + len); // LEN byte + (CMD + PAYLOAD)

  if (received_crc != calculated_crc) {
    // CRC mismatch
    uint8_t cmd_byte = g_rx_buf[2];
    sendNack(cmd_byte, ERR_CRC);
    g_rx_idx = 0;
    return result;
  }

  // Extract command and payload
  result.cmd = g_rx_buf[2];
  result.payload_len = len - 1; // len includes CMD byte
  for (uint8_t i = 0; i < result.payload_len; i++) {
    result.payload[i] = g_rx_buf[3 + i];
  }
  result.valid = true;

  g_rx_idx = 0;
  return result;
}

// --- Despacho de comando ---
void dispatchCommand(const SerialFrame& frame) {
  switch (frame.cmd) {

    case CMD_HEARTBEAT:
      g_last_heartbeat = millis();
      sendHeartbeat(); // Respond with heartbeat
      break;

    case CMD_SEGMENT:
      handleSegment(frame);
      break;

    case CMD_STOP:
      handleStop();
      sendAck(CMD_STOP);
      break;

    case CMD_HOME:
      handleHome(frame);
      break;

    case CMD_STATUS_REQ:
      sendStatusResponse();
      break;

    case CMD_SET_POS:
      handleSetPosition(frame);
      break;

    default:
      sendNack(frame.cmd, ERR_UNKNOWN_CMD);
      break;
  }
}

// ============================================================================
//  SEÇÃO 6: FILA DE SEGMENTOS
// ============================================================================

bool queueIsFull() {
  return g_queue_count >= SEGMENT_QUEUE_SIZE;
}

bool queueIsEmpty() {
  return g_queue_count == 0;
}

bool queuePush(const Segment& seg) {
  if (queueIsFull()) return false;
  g_queue[g_queue_tail] = seg;
  g_queue_tail = (g_queue_tail + 1) % SEGMENT_QUEUE_SIZE;
  g_queue_count++;
  return true;
}

bool queuePop(Segment& seg) {
  if (queueIsEmpty()) return false;
  seg = g_queue[g_queue_head];
  g_queue_head = (g_queue_head + 1) % SEGMENT_QUEUE_SIZE;
  g_queue_count--;
  return true;
}

void queueClear() {
  g_queue_head = 0;
  g_queue_tail = 0;
  g_queue_count = 0;
}

// ============================================================================
//  SEÇÃO 7: CONTROLE DE MOTORES (J4–J6)
// ============================================================================

// --- Mover um passo em um eixo ---
inline void stepMotor(uint8_t pul_pin, uint8_t dir_pin, bool direction) {
  digitalWrite(dir_pin, direction ? HIGH : LOW);
  digitalWrite(pul_pin, HIGH);
  delayMicroseconds(PULSE_WIDTH_US);
  digitalWrite(pul_pin, LOW);
}

// --- Executar um segmento (blocking) ---
void executeSegment(const Segment& seg) {
  g_segment_active = true;
  g_current_seq = seg.seq;

  int16_t steps[3] = { seg.steps_j4, seg.steps_j5, seg.steps_j6 };
  uint8_t pul_pins[3] = { PUL_J4, PUL_J5, PUL_J6 };
  uint8_t dir_pins[3] = { DIR_J4, DIR_J5, DIR_J6 };
  volatile int32_t* positions[3] = { &g_pos_j4, &g_pos_j5, &g_pos_j6 };

  // Absolute steps for each axis
  int16_t abs_steps[3];
  bool directions[3];
  int16_t max_steps = 0;

  for (uint8_t i = 0; i < 3; i++) {
    directions[i] = (steps[i] >= 0);
    abs_steps[i] = abs(steps[i]);
    if (abs_steps[i] > max_steps) max_steps = abs_steps[i];
  }

  if (max_steps == 0) {
    // No movement needed, just wait the duration
    if (seg.duration_ms > 0) delay(seg.duration_ms);
    g_segment_active = false;
    sendSegmentDone(seg.seq);
    return;
  }

  // Calculate step interval for coordinated movement
  // All axes must finish in duration_ms
  unsigned long interval_us;
  if (seg.duration_ms > 0 && max_steps > 0) {
    interval_us = ((unsigned long)seg.duration_ms * 1000UL) / (unsigned long)max_steps;
  } else {
    interval_us = MIN_STEP_INTERVAL_US * 2;
  }
  if (interval_us < MIN_STEP_INTERVAL_US) interval_us = MIN_STEP_INTERVAL_US;

  // Bresenham-style coordinated stepping
  int16_t counters[3] = { 0, 0, 0 };
  int16_t done_steps[3] = { 0, 0, 0 };

  // Set directions
  for (uint8_t i = 0; i < 3; i++) {
    digitalWrite(dir_pins[i], directions[i] ? HIGH : LOW);
  }

  for (int16_t step = 0; step < max_steps; step++) {
    // Check for stop condition
    if (g_state == EXEC_STOPPED || g_state == EXEC_FAULT) {
      g_segment_active = false;
      return;
    }

    unsigned long step_start = micros();

    for (uint8_t i = 0; i < 3; i++) {
      counters[i] += abs_steps[i];
      if (counters[i] >= max_steps) {
        counters[i] -= max_steps;
        // Generate pulse
        digitalWrite(pul_pins[i], HIGH);
        delayMicroseconds(PULSE_WIDTH_US);
        digitalWrite(pul_pins[i], LOW);
        done_steps[i]++;
        // Update position
        *positions[i] += directions[i] ? 1 : -1;
      }
    }

    // Wait for remaining interval
    unsigned long elapsed = micros() - step_start;
    if (elapsed < interval_us) {
      // Use delayMicroseconds for short waits
      unsigned long remaining = interval_us - elapsed;
      if (remaining > 16383) {
        // delayMicroseconds max is 16383 on Arduino
        delay(remaining / 1000);
        remaining = remaining % 1000;
      }
      if (remaining > 0) delayMicroseconds(remaining);
    }

    // Check serial periodically (every 100 steps)
    if (step % 100 == 0) {
      processSerial();
    }
  }

  g_segment_active = false;
  sendSegmentDone(seg.seq);
}

// ============================================================================
//  SEÇÃO 8: COMMAND HANDLERS
// ============================================================================

void handleSegment(const SerialFrame& frame) {
  // Validate payload size
  if (frame.payload_len != 10) {
    sendNack(CMD_SEGMENT, ERR_PAYLOAD_SIZE);
    return;
  }

  // Check state — only accept segments in IDLE or RUNNING
  if (g_state != EXEC_IDLE && g_state != EXEC_RUNNING) {
    sendNack(CMD_SEGMENT, ERR_INVALID_STATE);
    return;
  }

  // Check queue space
  if (queueIsFull()) {
    sendNack(CMD_SEGMENT, ERR_QUEUE_FULL);
    return;
  }

  // Parse segment from payload
  Segment seg;
  const uint8_t* p = frame.payload;
  seg.seq = p[0];
  seg.steps_j4  = (int16_t)(p[1] | (p[2] << 8));
  seg.steps_j5  = (int16_t)(p[3] | (p[4] << 8));
  seg.steps_j6  = (int16_t)(p[5] | (p[6] << 8));
  seg.duration_ms = (uint16_t)(p[7] | (p[8] << 8));
  seg.flags = p[9];

  // Enqueue
  queuePush(seg);
  sendAck(CMD_SEGMENT);

  // Start running if idle
  if (g_state == EXEC_IDLE) {
    g_state = EXEC_RUNNING;
  }
}

void handleStop() {
  // Immediate stop
  queueClear();
  g_segment_active = false;
  g_state = EXEC_STOPPED;
}

void handleHome(const SerialFrame& frame) {
  if (g_state == EXEC_RUNNING) {
    sendNack(CMD_HOME, ERR_INVALID_STATE);
    return;
  }

  uint8_t axis_mask = 0x07; // default: all axes
  if (frame.payload_len >= 1) {
    axis_mask = frame.payload[0];
  }

  sendAck(CMD_HOME);
  g_state = EXEC_HOMING;

  // =========================================================================
  //  STUB: Homing real requer chaves de fim de curso
  //  Por enquanto, simplesmente zera as posições
  // =========================================================================
  if (axis_mask & 0x01) g_pos_j4 = 0;
  if (axis_mask & 0x02) g_pos_j5 = 0;
  if (axis_mask & 0x04) g_pos_j6 = 0;

  g_state = EXEC_IDLE;
  sendHomeDone(0); // 0 = sucesso
}

void handleSetPosition(const SerialFrame& frame) {
  if (frame.payload_len != 12) {
    sendNack(CMD_SET_POS, ERR_PAYLOAD_SIZE);
    return;
  }

  const uint8_t* p = frame.payload;
  g_pos_j4 = (int32_t)(p[0] | (p[1] << 8) | ((int32_t)p[2] << 16) | ((int32_t)p[3] << 24));
  g_pos_j5 = (int32_t)(p[4] | (p[5] << 8) | ((int32_t)p[6] << 16) | ((int32_t)p[7] << 24));
  g_pos_j6 = (int32_t)(p[8] | (p[9] << 8) | ((int32_t)p[10] << 16) | ((int32_t)p[11] << 24));

  sendAck(CMD_SET_POS);
}

// ============================================================================
//  SEÇÃO 9: WATCHDOG DE COMUNICAÇÃO
// ============================================================================

void checkWatchdog() {
  if (g_last_heartbeat == 0) return; // Não ativado ainda

  unsigned long now = millis();
  if (now - g_last_heartbeat > HEARTBEAT_TIMEOUT_MS) {
    // Timeout de comunicação
    if (g_state != EXEC_FAULT) {
      handleStop(); // Para tudo
      g_state = EXEC_FAULT;
      g_fault = FAULT_SERIAL_TIMEOUT;
      sendFaultNotify(FAULT_SERIAL_TIMEOUT, 0);
    }
  }
}

// ============================================================================
//  SEÇÃO 10: PROCESSAMENTO SERIAL
// ============================================================================

void processSerial() {
  while (Serial.available()) {
    uint8_t byte_in = Serial.read();
    SerialFrame frame = parseSerialByte(byte_in);
    if (frame.valid) {
      dispatchCommand(frame);
    }
  }
}

// ============================================================================
//  SEÇÃO 11: SETUP E LOOP PRINCIPAL
// ============================================================================

void setup() {
  // Inicializa serial
  Serial.begin(SERIAL_BAUD);

  // Configura pinos dos motores
  pinMode(PUL_J4, OUTPUT);
  pinMode(DIR_J4, OUTPUT);
  pinMode(PUL_J5, OUTPUT);
  pinMode(DIR_J5, OUTPUT);
  pinMode(PUL_J6, OUTPUT);
  pinMode(DIR_J6, OUTPUT);

  // Estado inicial
  digitalWrite(PUL_J4, LOW);
  digitalWrite(PUL_J5, LOW);
  digitalWrite(PUL_J6, LOW);
  digitalWrite(DIR_J4, LOW);
  digitalWrite(DIR_J5, LOW);
  digitalWrite(DIR_J6, LOW);

  // Chaves de fim de curso (quando disponíveis)
  // pinMode(LIMIT_J4, INPUT_PULLUP);
  // pinMode(LIMIT_J5, INPUT_PULLUP);
  // pinMode(LIMIT_J6, INPUT_PULLUP);

  g_state = EXEC_IDLE;
  g_fault = FAULT_NONE;
  g_last_heartbeat = 0;
}

void loop() {
  // 1. Processar dados seriais
  processSerial();

  // 2. Verificar watchdog
  checkWatchdog();

  // 3. Executar segmentos da fila
  if (g_state == EXEC_RUNNING && !g_segment_active) {
    Segment seg;
    if (queuePop(seg)) {
      executeSegment(seg);

      // Verificar se era o último segmento
      if ((seg.flags & 0x01) && queueIsEmpty()) {
        g_state = EXEC_IDLE;
      }
    } else {
      // Fila vazia, mas ainda em RUNNING
      // Aguardar mais segmentos ou timeout
    }
  }

  // 4. Recuperação de STOPPED
  if (g_state == EXEC_STOPPED) {
    // Manter em STOPPED até receber novo comando ou heartbeat
    // O ESP pode enviar novo SEGMENT ou HOME para retomar
  }

  // 5. Recuperação de FAULT (apenas por reset ou heartbeat)
  if (g_state == EXEC_FAULT) {
    // Verificar se comunicação restaurou (novo heartbeat)
    if (g_last_heartbeat > 0 && (millis() - g_last_heartbeat < HEARTBEAT_TIMEOUT_MS)) {
      g_state = EXEC_IDLE;
      g_fault = FAULT_NONE;
    }
  }
}
