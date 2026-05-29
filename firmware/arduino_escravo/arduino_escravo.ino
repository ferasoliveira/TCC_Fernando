/**
 * ============================================================================
 *               EB-15 Robotic Arm — Escravo (Arduino Uno)
 *                        FIRMWARE EXECUTOR
 * ============================================================================
 * Fase 5: Controle de Malha Fechada (PID + Tangente Hiperbólica)
 * Responsabilidade: Execução do laço PD discreto local a 200 Hz com atenuação
 *                   não linear Tanh, controlando dinamicamente OCR1A e OCR2A.
 */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "config.h"

// Instancia a SoftwareSerial (A4 = RX, A5 = TX)
SoftwareSerial SerialMestre(UART_RX_PIN, UART_TX_PIN);

// Estrutura do Frame de 10 Bytes
struct __attribute__((__packed__)) UnoFrame {
  uint8_t  preamble;      // 0xAA
  int16_t  steps_j4;      // Passos J4 (Bytes 1-2)
  int16_t  steps_j5;      // Passos J5 (Bytes 3-4)
  int16_t  steps_j6;      // Passos J6 (Bytes 5-6)
  uint8_t  velocity;      // Velocidade (Byte 7)
  uint8_t  acceleration;  // Aceleração (Byte 8)
  uint8_t  checksum;      // XOR modular (Byte 9)
};

// Estrutura de Rastreamento de Encoder Multi-voltas
struct EncoderTracker {
  uint16_t last_raw;
  int16_t  turns;
  bool     initialized;
  int32_t  accumulated_ticks;
  float    current_deg;
};

EncoderTracker g_encoders[3];

// Parser Serial
enum ParserState : uint8_t {
  WAIT_PREAMBLE = 0,
  READ_DATA     = 1,
  READ_CHECKSUM = 2
};

ParserState g_parser_state = WAIT_PREAMBLE;
uint8_t g_rx_buffer[10];
uint8_t g_rx_index = 0;
uint8_t g_computed_xor = 0;

// Variáveis voláteis usadas nas ISRs para controle dos motores
volatile int16_t g_target_steps_j4 = 0;
volatile int16_t g_target_steps_j5 = 0;
volatile int16_t g_target_steps_j6 = 0;

volatile int16_t isr_steps_remaining_j4 = 0;
volatile int16_t isr_steps_remaining_j5 = 0;
volatile int16_t isr_steps_remaining_j6 = 0;

volatile bool g_dir_j4 = true;
volatile bool g_dir_j5 = true;
volatile bool g_dir_j6 = true;

// Posicionamento estimado em passos acumulados
volatile int32_t g_pos_j4 = 0;
volatile int32_t g_pos_j5 = 0;
volatile int32_t g_pos_j6 = 0;

// Parâmetros recebidos
uint8_t g_velocity = 0;
uint8_t g_acceleration = 0;

// Ganhos PD + Tanh locais para as juntas J4, J5, J6 do punho
const float PID_KP[3] = { 0.8, 0.8, 0.8 }; // Proporcional
const float PID_KD[3] = { 2.0, 2.0, 2.0 }; // Derivativo (Amortecimento)
const float PID_GAMMA[3] = { 8.0, 8.0, 8.0 }; // Coeficiente Tanh
float g_pid_last_err[3] = { 0.0, 0.0, 0.0 };

// Protótipos das funções
void processSerialMestre();
void handleReceivedFrame(const UnoFrame& frame);

void soft_i2c_init();
void soft_i2c_delay();
void soft_i2c_start();
void soft_i2c_stop();
bool soft_i2c_write(uint8_t byte);
uint8_t soft_i2c_read(bool ack);
void select_tca_channel(uint8_t channel);
uint16_t read_as5600_raw(uint8_t channel);
void update_encoder(uint8_t idx, uint8_t channel);
void update_closed_loop_pid();

void setup() {
  Serial.begin(115200);
  Serial.println(F("\n[Escravo - Arduino Uno] Inicializando Sistema (Fase 5)..."));

  SerialMestre.begin(SERIAL_BAUD);

  // Configuração de pinagem CNC Shield
  pinMode(PUL_J4, OUTPUT); pinMode(DIR_J4, OUTPUT);
  pinMode(PUL_J5, OUTPUT); pinMode(DIR_J5, OUTPUT);
  pinMode(PUL_J6, OUTPUT); pinMode(DIR_J6, OUTPUT);
  pinMode(MOTORS_ENABLE, OUTPUT);

  digitalWrite(MOTORS_ENABLE, HIGH);
  digitalWrite(PUL_J4, LOW); digitalWrite(DIR_J4, LOW);
  digitalWrite(PUL_J5, LOW); digitalWrite(DIR_J5, LOW);
  digitalWrite(PUL_J6, LOW); digitalWrite(DIR_J6, LOW);

  soft_i2c_init();

  for (int i = 0; i < 3; i++) {
    g_encoders[i].last_raw = 0;
    g_encoders[i].turns = 0;
    g_encoders[i].initialized = false;
    g_encoders[i].accumulated_ticks = 0;
    g_encoders[i].current_deg = 0.0;
  }

  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  // --- CONFIGURAÇÃO DE REGISTRADORES DO TIMER 1 ---
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10); // CTC prescaler = 64
  OCR1A = 6249;
  
  // --- CONFIGURAÇÃO DE REGISTRADORES DO TIMER 2 ---
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;
  TCCR2A |= (1 << WGM21); // CTC
  TCCR2B |= (1 << CS22) | (1 << CS21); // CTC prescaler = 256
  OCR2A = 250;
  sei();

  Serial.println(F("[Escravo] Timers de Hardware e malha fechada local ativados."));
}

void loop() {
  processSerialMestre();

  // Executa a varredura local I2C e a Malha Fechada a 200 Hz
  static unsigned long last_sensor_read = 0;
  if (micros() - last_sensor_read >= (1000000UL / CONTROL_HZ)) {
    last_sensor_read = micros();
    
    // Varre os encoders J4-J6 nos canais 3, 4, 5 do TCA9548A #2
    update_encoder(0, 3);
    update_encoder(1, 4);
    update_encoder(2, 5);

    // Fecha a malha local das juntas superiores ajustando os timers em tempo real
    update_closed_loop_pid();
  }
}

/**
 * @brief Executa o algoritmo PD + Tanh local a 200 Hz para J4-J6
 *        Modifica dinamicamente os valores de OCR1A e OCR2A nos timers AVR
 */
void update_closed_loop_pid() {
  const float dt = 0.005; // 200 Hz
  const float max_speed_deg_s[3] = { 90.0, 90.0, 90.0 }; // Velocidades limite de J4-J6

  for (int i = 0; i < 3; i++) {
    // Alvo desejado lido nos passos UART
    float desired_deg = 0.0;
    if (i == 0) desired_deg = g_target_steps_j4 / STEPS_PER_DEG[0];
    else if (i == 1) desired_deg = g_target_steps_j5 / STEPS_PER_DEG[1];
    else if (i == 2) desired_deg = g_target_steps_j6 / STEPS_PER_DEG[2];

    float real_deg = g_encoders[i].current_deg;
    float error = desired_deg - real_deg;

    // Termo derivativo
    float err_diff = (error - g_pid_last_err[i]) / dt;
    g_pid_last_err[i] = error;

    // PD não linear Proporcional com atenuação Tanh
    float tanh_factor = tanh(PID_GAMMA[i] * abs(error));
    float u_speed = PID_KP[i] * error * tanh_factor + PID_KD[i] * err_diff;

    // Satura a velocidade
    u_speed = constrain(u_speed, -max_speed_deg_s[i], max_speed_deg_s[i]);
    float speed_steps_s = abs(u_speed) * STEPS_PER_DEG[i];

    if (speed_steps_s > 1.0 && abs(error) > 0.05) {
      cli();
      if (i == 0 || i == 1) {
        // Timer 1 (prescaler = 64, tickrate = 250.000 Hz)
        uint16_t ocr_val = (uint16_t)(250000UL / speed_steps_s) - 1;
        if (ocr_val < 10) ocr_val = 10; // Limite de velocidade física superior
        OCR1A = ocr_val;
      } else if (i == 2) {
        // Timer 2 (prescaler = 256, tickrate = 62.500 Hz)
        uint8_t ocr_val = (uint8_t)(62500UL / speed_steps_s) - 1;
        if (ocr_val < 5) ocr_val = 5;
        OCR2A = ocr_val;
      }
      sei();
    } else {
      if (abs(error) <= 0.05) {
        // Zera passos restantes para cessar movimento e evitar jitter
        cli();
        if (i == 0) isr_steps_remaining_j4 = 0;
        else if (i == 1) isr_steps_remaining_j5 = 0;
        else if (i == 2) isr_steps_remaining_j6 = 0;
        sei();
      }
    }
  }
}

// ISRs da Fase 4
ISR(TIMER1_COMPA_vect) {
  bool j4_active = (isr_steps_remaining_j4 > 0);
  bool j5_active = (isr_steps_remaining_j5 > 0);
  if (j4_active) digitalWrite(PUL_J4, HIGH);
  if (j5_active) digitalWrite(PUL_J5, HIGH);
  delayMicroseconds(2);
  if (j4_active) { digitalWrite(PUL_J4, LOW); isr_steps_remaining_j4--; g_pos_j4 += (g_dir_j4 ? 1 : -1); }
  if (j5_active) { digitalWrite(PUL_J5, LOW); isr_steps_remaining_j5--; g_pos_j5 += (g_dir_j5 ? 1 : -1); }
  if (isr_steps_remaining_j4 == 0 && isr_steps_remaining_j5 == 0) TIMSK1 &= ~(1 << OCIE1A);
}

ISR(TIMER2_COMPA_vect) {
  if (isr_steps_remaining_j6 > 0) {
    digitalWrite(PUL_J6, HIGH); delayMicroseconds(2); digitalWrite(PUL_J6, LOW);
    isr_steps_remaining_j6--; g_pos_j6 += (g_dir_j6 ? 1 : -1);
  } else TIMSK2 &= ~(1 << OCIE2A);
}

void handleReceivedFrame(const UnoFrame& frame) {
  g_target_steps_j4 = frame.steps_j4;
  g_target_steps_j5 = frame.steps_j5;
  g_target_steps_j6 = frame.steps_j6;
  g_velocity = frame.velocity;
  g_acceleration = frame.acceleration;

  g_dir_j4 = (g_target_steps_j4 >= 0);
  g_dir_j5 = (g_target_steps_j5 >= 0);
  g_dir_j6 = (g_target_steps_j6 >= 0);

  digitalWrite(DIR_J4, g_dir_j4 ? HIGH : LOW);
  digitalWrite(DIR_J5, g_dir_j5 ? HIGH : LOW);
  digitalWrite(DIR_J6, g_dir_j6 ? HIGH : LOW);

  cli();
  isr_steps_remaining_j4 = abs(g_target_steps_j4);
  isr_steps_remaining_j5 = abs(g_target_steps_j5);
  isr_steps_remaining_j6 = abs(g_target_steps_j6);
  
  uint16_t vel_factor = (g_velocity > 0) ? g_velocity : 50;
  uint32_t freq_1 = vel_factor * 10UL;
  if (freq_1 < 40) freq_1 = 40;
  OCR1A = (250000UL / freq_1) - 1;

  uint32_t freq_2 = vel_factor * 10UL;
  if (freq_2 < 40) freq_2 = 40;
  OCR2A = (62500UL / freq_2) - 1;
  sei();

  while (digitalRead(TRIGGER_PIN) == HIGH);

  cli();
  TIMSK1 |= (1 << OCIE1A);
  TIMSK2 |= (1 << OCIE2A);
  digitalWrite(MOTORS_ENABLE, LOW);
  sei();
}

// Recepção serial e Software I2C (inalterados)
void processSerialMestre() {
  while (SerialMestre.available()) {
    uint8_t byte_in = SerialMestre.read();
    switch (g_parser_state) {
      case WAIT_PREAMBLE:
        if (byte_in == 0xAA) {
          g_rx_index = 0; g_rx_buffer[g_rx_index++] = byte_in; g_computed_xor = byte_in; g_parser_state = READ_DATA;
        }
        break;
      case READ_DATA:
        g_rx_buffer[g_rx_index++] = byte_in; g_computed_xor ^= byte_in;
        if (g_rx_index >= 9) g_parser_state = READ_CHECKSUM;
        break;
      case READ_CHECKSUM:
        g_rx_buffer[g_rx_index] = byte_in; g_parser_state = WAIT_PREAMBLE;
        if (byte_in == g_computed_xor) {
          UnoFrame received_frame; memcpy(&received_frame, g_rx_buffer, sizeof(UnoFrame));
          SerialMestre.write(0x06); handleReceivedFrame(received_frame);
        } else SerialMestre.write(0x15);
        break;
    }
  }
}

// Software I2C
#define SCL_HIGH() pinMode(I2C_SCL_PIN, INPUT)
#define SCL_LOW()  { pinMode(I2C_SCL_PIN, OUTPUT); digitalWrite(I2C_SCL_PIN, LOW); }
#define SDA_HIGH() pinMode(I2C_SDA_PIN, INPUT)
#define SDA_LOW()  { pinMode(I2C_SDA_PIN, OUTPUT); digitalWrite(I2C_SDA_PIN, LOW); }
#define SDA_READ() digitalRead(I2C_SDA_PIN)

void soft_i2c_init() {
  pinMode(I2C_SCL_PIN, INPUT); pinMode(I2C_SDA_PIN, INPUT);
  digitalWrite(I2C_SCL_PIN, LOW); digitalWrite(I2C_SDA_PIN, LOW);
}
void soft_i2c_delay() { delayMicroseconds(2); }
void soft_i2c_start() { SDA_LOW(); soft_i2c_delay(); SCL_LOW(); soft_i2c_delay(); }
void soft_i2c_stop() { SDA_LOW(); soft_i2c_delay(); SCL_HIGH(); soft_i2c_delay(); SDA_HIGH(); soft_i2c_delay(); }
bool soft_i2c_write(uint8_t byte) {
  for (uint8_t i = 0; i < 8; i++) {
    if (byte & 0x80) SDA_HIGH(); else SDA_LOW();
    soft_i2c_delay(); SCL_HIGH(); soft_i2c_delay(); SCL_LOW(); byte <<= 1;
  }
  SDA_HIGH(); soft_i2c_delay(); SCL_HIGH(); soft_i2c_delay(); bool ack = (SDA_READ() == 0); SCL_LOW(); soft_i2c_delay(); return ack;
}
uint8_t soft_i2c_read(bool ack) {
  uint8_t byte = 0; SDA_HIGH();
  for (uint8_t i = 0; i < 8; i++) {
    soft_i2c_delay(); SCL_HIGH(); soft_i2c_delay();
    byte <<= 1; if (SDA_READ()) byte |= 1; SCL_LOW();
  }
  if (ack) { SDA_LOW(); } else { SDA_HIGH(); }
  soft_i2c_delay(); SCL_HIGH(); soft_i2c_delay(); SCL_LOW(); soft_i2c_delay(); SDA_HIGH(); return byte;
}
void select_tca_channel(uint8_t channel) {
  soft_i2c_start(); soft_i2c_write(TCA9548A_ADDR << 1); soft_i2c_write(1 << channel); soft_i2c_stop();
}
uint16_t read_as5600_raw(uint8_t channel) {
  select_tca_channel(channel);
  soft_i2c_start();
  if (!soft_i2c_write(0x36 << 1)) { soft_i2c_stop(); return 0xFFFF; }
  soft_i2c_write(0x0C); soft_i2c_stop();
  soft_i2c_start();
  if (!soft_i2c_write((0x36 << 1) | 1)) { soft_i2c_stop(); return 0xFFFF; }
  uint8_t high = soft_i2c_read(true); uint8_t low = soft_i2c_read(false); soft_i2c_stop();
  return ((uint16_t)high << 8) | low;
}
void update_encoder(uint8_t idx, uint8_t channel) {
  uint16_t raw = read_as5600_raw(channel);
  if (raw == 0xFFFF) return;
  EncoderTracker& tracker = g_encoders[idx];
  if (!tracker.initialized) {
    tracker.last_raw = raw; tracker.turns = 0; tracker.initialized = true; tracker.accumulated_ticks = raw;
  } else {
    int16_t diff = raw - tracker.last_raw;
    if (diff < -2048) tracker.turns++; else if (diff > 2048) tracker.turns--;
    tracker.last_raw = raw; tracker.accumulated_ticks = ((int32_t)tracker.turns * 4096) + raw;
  }
  tracker.current_deg = tracker.accumulated_ticks * (360.0 / 4096.0);
}
