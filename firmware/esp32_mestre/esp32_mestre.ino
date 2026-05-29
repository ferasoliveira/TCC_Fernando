/**
 * ============================================================================
 *               EB-15 Robotic Arm — Mestre (ESP32-S3)
 *                        FIRMWARE SUPERVISOR
 * ============================================================================
 * Fase 6: WiFi, WebServer e Soquete RTDE
 * Responsabilidade: Inicialização de WiFi, WebServer HTTP e soquetes RTDE TCP
 *                   a 50 Hz isolados no Core 0 via FreeRTOS.
 */

#include <Arduino.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <math.h>
#include "config.h"

HardwareSerial SerialUno(2);
WebServer server(80);
WiFiServer rtde_server(5000); // RTDE TCP Port 5000

// Página HTML/CSS/JS Embutida (PROGMEM)
const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>EB-15 Dashboard</title>
    <style>
        :root{--bg:#121212;--panel:#1e1e1e;--text:#e0e0e0;--muted:#888;--acc:#bb86fc;--acc-h:#9c4dcc;--dang:#cf6679;--dang-h:#b00020;--bord:#333;--succ:#03dac6;}
        *{box-sizing:border-box;margin:0;padding:0;font-family:sans-serif;}
        body{background:var(--bg);color:var(--text);padding:1rem;}
        header{display:flex;justify-content:space-between;border-bottom:2px solid var(--bord);padding-bottom:1rem;margin-bottom:1rem;}
        .badge{padding:0.4rem 0.8rem;border-radius:4px;font-weight:bold;margin-left:0.5rem;background:#555;}
        #conn-state.connected{background:var(--succ);color:#000;} #conn-state.error{background:var(--dang);color:#fff;}
        .dashboard{display:grid;grid-template-columns:repeat(auto-fit,minmax(300px,1fr));gap:1.5rem;}
        .panel{background:var(--panel);border-radius:8px;padding:1.5rem;box-shadow:0 4px 6px rgba(0,0,0,0.3); display:flex; flex-direction:column;}
        h2{border-bottom:1px solid var(--bord);padding-bottom:0.5rem;margin-bottom:1rem;} h3{color:var(--muted);margin:1rem 0 0.5rem;font-size:0.9rem;}
        button{background:var(--acc);color:#fff;border:none;padding:0.6rem 1rem;border-radius:4px;cursor:pointer;font-weight:bold;}
        button:hover{background:var(--acc-h);} button.danger{background:var(--dang);} button.danger:hover{background:var(--dang-h);}
        .quick-actions{display:grid;grid-template-columns:1fr 1fr;gap:0.5rem;}
        .jog-row{display:flex;align-items:center;background:#2a2a2a;padding:0.5rem;border-radius:4px;margin-bottom:0.5rem;}
        .jog-row span{width:30px;font-weight:bold;} .jog-row button{flex:1;margin:0 0.2rem;}
        table{width:100%;border-collapse:collapse;margin-top:0.5rem;} th,td{padding:0.4rem;text-align:left;border-bottom:1px solid var(--bord);}
        th{color:var(--muted);} td{font-family:monospace;font-size:1.1rem;}
        .api-form{display:flex;flex-direction:column;gap:0.5rem;} select,textarea{background:#2a2a2a;color:var(--text);border:1px solid var(--bord);padding:0.5rem;}
        pre{background:#000;padding:0.5rem;overflow-x:auto;font-size:0.9rem;min-height:100px;flex:1;}
        .speed-control{display:flex;gap:0.5rem;margin-top:1rem;align-items:center;} .speed-control input{flex:1;}
    </style>
</head>
<body>
    <header>
        <h1>EB-15 Robotic System</h1>
        <div class="status-indicator">
            <span id="conn-state" class="badge">DISCONNECTED</span>
            <span id="robot-state" class="badge">UNKNOWN</span>
        </div>
    </header>
    <main class="dashboard">
        <section class="panel controls-panel">
            <h2>Command Core</h2>
            <div class="quick-actions">
                <button id="btn-home" class="action-btn">Homing Seq</button>
                <button id="btn-stop" class="action-btn danger">E-STOP</button>
                <button id="btn-pause" class="action-btn">Pause</button>
                <button id="btn-resume" class="action-btn">Resume</button>
            </div>
            <button id="btn-ack" class="action-btn" style="margin-top:0.5rem;background:#555;width:100%;">Acknowledge Fault</button>
            <h3>Joint Jogging</h3>
            <div id="jog-controls" class="jog-grid"></div>
            <div class="speed-control">
                <label>Speed:</label>
                <input type="range" id="jog-speed" min="1" max="100" value="50">
                <span id="speed-val">50%</span>
            </div>
        </section>
        <section class="panel telemetry-panel">
            <h2>Telemetry</h2>
            <div class="telemetry-block">
                <h3>Joints (deg)</h3>
                <table id="joint-table">
                    <tr><th>J1</th><td id="t-j1">0.0</td><th>J4</th><td id="t-j4">0.0</td></tr>
                    <tr><th>J2</th><td id="t-j2">0.0</td><th>J5</th><td id="t-j5">0.0</td></tr>
                    <tr><th>J3</th><td id="t-j3">0.0</td><th>J6</th><td id="t-j6">0.0</td></tr>
                </table>
            </div>
            <div class="telemetry-block">
                <h3>Pose TCP (mm/deg)</h3>
                <table id="pose-table">
                    <tr><th>X</th><td id="t-x">0.0</td><th>Rx</th><td id="t-rx">0.0</td></tr>
                    <tr><th>Y</th><td id="t-y">0.0</td><th>Ry</th><td id="t-ry">0.0</td></tr>
                    <tr><th>Z</th><td id="t-z">0.0</td><th>Rz</th><td id="t-rz">0.0</td></tr>
                </table>
            </div>
        </section>
    </main>
    <script>
        const $=(id)=>document.getElementById(id);
        async function api(ep,m='GET',b=null){
          try{
            const opt={method:m,headers:{}};
            if(b){opt.headers['Content-Type']='application/json';opt.body=JSON.stringify(b);}
            const r=await fetch(ep,opt); const d=await r.json();
            $('conn-state').className='badge connected'; $('conn-state').innerText='CONNECTED'; return d;
          }catch(e){
            $('conn-state').className='badge error'; $('conn-state').innerText='DISCONNECTED'; return null;
          }
        }
        function setup(){
          let jg=$('jog-controls');
          for(let i=1;i<=6;i++){
            jg.innerHTML+=`<div class="jog-row"><span>J${i}</span><button onmousedown="api('/api/jog','POST',{joint:${i},dir:-1,spd:$('jog-speed').value})" onmouseup="api('/api/jog/stop','POST')">-</button><button onmousedown="api('/api/jog','POST',{joint:${i},dir:1,spd:$('jog-speed').value})" onmouseup="api('/api/jog/stop','POST')">+</button></div>`;
          }
          $('btn-home').onclick=()=>api('/api/home','POST'); $('btn-stop').onclick=()=>api('/api/stop','POST');
          $('btn-pause').onclick=()=>api('/api/pause','POST'); $('btn-resume').onclick=()=>api('/api/resume','POST');
          $('btn-ack').onclick=()=>api('/api/fault/ack','POST');
          $('jog-speed').oninput=(e)=>$('speed-val').innerText=e.target.value+'%';
        }
        setup();
        setInterval(async()=>{
          let s=await api('/api/status'); 
          if(s) {
              $('robot-state').innerText=s.state||'READY';
              $('robot-state').className='badge connected';
          }
          let j=await api('/api/joints'); if(j&&j.joints) for(let i=1;i<=6;i++) if($(`t-j${i}`)) $(`t-j${i}`).innerText=j.joints[`j${i}`].deg.toFixed(2);
          let p=await api('/api/pose'); if(p&&p.pose) ['x','y','z','rx','ry','rz'].forEach(a=>$('t-'+a).innerText=p.pose[a].toFixed(1));
        }, 800);
    </script>
</body>
</html>
)=====";

// Estruturas de Dados
struct __attribute__((__packed__)) UnoFrame {
  uint8_t  preamble;
  int16_t  steps_j4;
  int16_t  steps_j5;
  int16_t  steps_j6;
  uint8_t  velocity;
  uint8_t  acceleration;
  uint8_t  checksum;
};

struct EncoderTracker {
  uint16_t last_raw;
  int16_t  turns;
  bool     initialized;
  int32_t  accumulated_ticks;
  float    current_deg;
};

struct CartesianPose {
  float x; float y; float z;
  float rx; float ry; float rz;
};

struct TrajectorySegment {
  int32_t  steps_target[6];
  uint32_t duration_ms;
  bool     active;
};

// Formato de Telemetria de alta performance do Soquete RTDE (52 Bytes)
struct __attribute__((__packed__)) RtdeFrame {
  float joints[6];     // J1-J6 Posições reais em graus
  float errors[6];     // J1-J6 Erros instantâneos em graus
  float temperature;  // Temperatura interna do chip do ESP32 S3 (°C)
};

// Variáveis Globais
EncoderTracker g_encoders[3];
CartesianPose g_current_pose;
float g_joint_angles[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

#define MAX_SEGMENTS 30
TrajectorySegment g_segments[MAX_SEGMENTS];
volatile int g_seg_head = 0;
volatile int g_seg_tail = 0;

// Geração de Passos (ISR)
volatile int32_t isr_steps_target[3]  = {0, 0, 0};
volatile int32_t isr_steps_current[3] = {0, 0, 0};
volatile uint32_t isr_step_delay[3]   = {1, 1, 1};
volatile uint32_t isr_step_accum[3]   = {0, 0, 0};

hw_timer_t* g_hw_timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// PID e Malha Fechada
const float PID_KP[3] = { 0.8, 0.8, 0.8 };
const float PID_KD[3] = { 2.0, 2.0, 2.0 };
const float PID_GAMMA[3] = { 8.0, 8.0, 8.0 };
float g_pid_last_err[3] = { 0.0, 0.0, 0.0 };

// Protótipos das funções
uint8_t computeChecksum(const UnoFrame& frame);
bool sendFrameToUno(int16_t s4, int16_t s5, int16_t s6, uint8_t vel, uint8_t accel);
bool waitForAck(uint32_t timeout_ms);

void select_tca_channel(uint8_t channel);
uint16_t read_as5600_raw(uint8_t channel);
void update_encoder(uint8_t idx, uint8_t channel);

void get_dh_matrix(float theta, float d, float a, float alpha, float out[4][4]);
void multiply_matrix(float A[4][4], float B[4][4], float out[4][4]);
void forward_kinematics(const float q_rad[6], float out_pos[3], float R[3][3]);
void matrix_to_euler(float R[3][3], float& rx, float& ry, float& rz);
void euler_to_matrix(float rx_deg, float ry_deg, float rz_deg, float R[3][3]);
bool inverse_kinematics(const float target_pos[3], float target_rot[3][3], float out_q_rad[6]);
bool check_soft_limits(const float q_deg[6]);

bool push_segment(const float q_deg[6], uint32_t duration_ms);
void plan_move_j(const float target_joints_deg[6], float speed_pct);
void plan_move_l(const CartesianPose& target_pose, float speed_pct);
void execute_trajectory_tick();
void update_closed_loop_pid();

void setupHttp();
void handleCORS();

// ISR (Fase 4 - inalterada)
void IRAM_ATTR onHardwareTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  for (int i = 0; i < 3; i++) {
    if (isr_steps_current[i] != isr_steps_target[i]) {
      isr_step_accum[i]++;
      if (isr_step_accum[i] >= isr_step_delay[i]) {
        isr_step_accum[i] = 0;
        bool dir = (isr_steps_target[i] > isr_steps_current[i]);
        if (i == 0) {
          digitalWrite(DIR_J1, dir); digitalWrite(PUL_J1, HIGH); delayMicroseconds(2); digitalWrite(PUL_J1, LOW);
        } else if (i == 1) {
          digitalWrite(DIR_J2, dir); digitalWrite(PUL_J2, HIGH); delayMicroseconds(2); digitalWrite(PUL_J2, LOW);
        } else if (i == 2) {
          digitalWrite(DIR_J3, dir); digitalWrite(PUL_J3, HIGH); delayMicroseconds(2); digitalWrite(PUL_J3, LOW);
        }
        isr_steps_current[i] += (dir ? 1 : -1);
      }
    }
  }
  portEXIT_CRITICAL_ISR(&timerMux);
}

// ============================================================================
// CONCORRÊNCIA MULTI-CORE: WI-FI, WEBSERVER E SOQUETE RTDE (CORE 0)
// ============================================================================
void wifi_rtde_core_task(void* pvParameters) {
  Serial.println("[Core 0] Inicializando tarefas de rede e conectividade...");

  // Configura ponto de acesso (AP) padrão de fábrica
  WiFi.mode(WIFI_AP);
  WiFi.softAP("EB15-Robotics", "admin12345");
  Serial.print("[Core 0] WiFi AP 'EB15-Robotics' ativo. IP: ");
  Serial.println(WiFi.softAPIP());

  // Inicia o WebServer HTTP
  setupHttp();

  // Inicia o Soquete RTDE TCP
  rtde_server.begin();
  Serial.println("[Core 0] Servidor Soquete RTDE ativado na porta TCP 5000");

  while (true) {
    // Escuta novas conexões no soquete RTDE
    WiFiClient client = rtde_server.available();
    if (client) {
      Serial.println("[Core 0] Cliente RTDE conectado!");
      
      while (client.connected()) {
        unsigned long start_loop = millis();

        // Constrói e empacota o frame binário de telemetria estruturada
        RtdeFrame frame;
        for (int i = 0; i < 6; i++) {
          frame.joints[i] = g_joint_angles[i];
          
          // Erro de malha fechada teórica (ou lida nos encoders locais)
          if (i < 3) {
            frame.errors[i] = g_pid_last_err[i];
          } else {
            frame.errors[i] = 0.0; // Uno realiza malha fechada local
          }
        }
        frame.temperature = temperatureRead(); // Leitura da temperatura interna do ESP32 S3

        // Transmite o frame de 52 bytes compactos
        client.write((const uint8_t*)&frame, sizeof(RtdeFrame));

        // Mantém a taxa estável de atualização a 50 Hz (20 ms por ciclo)
        long wait = 20 - (millis() - start_loop);
        if (wait > 0) {
          vTaskDelay(pdMS_TO_TICKS(wait));
        }
      }
      Serial.println("[Core 0] Cliente RTDE desconectado.");
    }

    // Processa requisições HTTP do WebServer
    server.handleClient();
    vTaskDelay(pdMS_TO_TICKS(2)); // Evita starvation do cão de guarda do RTOS
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n[Mestre - ESP32 S3] Inicializando Sistema (Fase 6)...");

  SerialUno.begin(SERIAL2_BAUD, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);

  pinMode(PUL_J1, OUTPUT); pinMode(DIR_J1, OUTPUT);
  pinMode(PUL_J2, OUTPUT); pinMode(DIR_J2, OUTPUT);
  pinMode(PUL_J3, OUTPUT); pinMode(DIR_J3, OUTPUT);

  digitalWrite(PUL_J1, LOW); digitalWrite(DIR_J1, LOW);
  digitalWrite(PUL_J2, LOW); digitalWrite(DIR_J2, LOW);
  digitalWrite(PUL_J3, LOW); digitalWrite(DIR_J3, LOW);

  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, HIGH);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, 400000);

  g_hw_timer = timerBegin(1000000);
  timerAttachInterrupt(g_hw_timer, &onHardwareTimer);
  timerAlarm(g_hw_timer, 50, true, 0);

  // Pose inicial
  float pos[3]; float R[3][3]; float q_rad[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  forward_kinematics(q_rad, pos, R);
  g_current_pose.x = pos[0]; g_current_pose.y = pos[1]; g_current_pose.z = pos[2];
  matrix_to_euler(R, g_current_pose.rx, g_current_pose.ry, g_current_pose.rz);

  // Cria a tarefa concorrente de Rede e RTDE vinculada ao Core 0
  // Deixando o Core 1 100% exclusivo para as tarefas de controle em tempo real (PID/Passos)
  xTaskCreatePinnedToCore(
    wifi_rtde_core_task,    // Função executora da tarefa
    "WiFi_RTDE_Task",       // Identificador textual da tarefa
    8192,                   // Tamanho do Stack alocado (8 KB)
    NULL,                   // Parâmetros passados
    3,                      // Prioridade da tarefa
    NULL,                   // Ponteiro de referência
    0                       // Vinculado estritamente ao Core 0
  );
  Serial.println("[Mestre] Concorrência FreeRTOS Multicore inicializada (Rede/RTDE no Core 0).");
}

void loop() {
  static uint32_t last_sensor_read = 0;
  static uint32_t last_trajectory_tick = 0;

  // Varredura síncrona I2C e execução da Malha Fechada PID a 200 Hz (Core 1 principal)
  if (micros() - last_sensor_read >= TIMER_PERIOD_US) {
    last_sensor_read = micros();
    update_encoder(0, 0);
    update_encoder(1, 1);
    update_encoder(2, 2);
    update_closed_loop_pid();
  }

  // Laço do planejador de trajetória global a 20 Hz (Core 1 principal)
  if (millis() - last_trajectory_tick >= SEG_SLICE_MS) {
    last_trajectory_tick = millis();
    execute_trajectory_tick();
  }

  vTaskDelay(pdMS_TO_TICKS(1)); // Libera controle de task FreeRTOS
}

// ============================================================================
// CONFIGURAÇÃO DO SERVIDOR WEB HTTP (CORS + ENDPOINTS)
// ============================================================================
void handleCORS() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

void setupHttp() {
  server.on("/", HTTP_GET, [](){
    server.send_P(200, "text/html", index_html);
  });
  
  server.on("/api/status", HTTP_GET, [](){
    handleCORS();
    StaticJsonDocument<200> doc;
    doc["state"] = "READY";
    doc["fault"] = 0;
    doc["arduino_connected"] = true;
    String out; serializeJson(doc, out);
    server.send(200, "application/json", out);
  });

  server.on("/api/joints", HTTP_GET, [](){
    handleCORS();
    StaticJsonDocument<400> doc;
    JsonObject j = doc.createNestedObject("joints");
    for (int i = 0; i < 6; i++) {
      JsonObject jx = j.createNestedObject("j" + String(i + 1));
      jx["deg"] = g_joint_angles[i];
      jx["homed"] = true;
    }
    String out; serializeJson(doc, out);
    server.send(200, "application/json", out);
  });

  server.on("/api/pose", HTTP_GET, [](){
    handleCORS();
    StaticJsonDocument<200> doc;
    JsonObject p = doc.createNestedObject("pose");
    p["x"] = g_current_pose.x;
    p["y"] = g_current_pose.y;
    p["z"] = g_current_pose.z;
    p["rx"] = g_current_pose.rx;
    p["ry"] = g_current_pose.ry;
    p["rz"] = g_current_pose.rz;
    String out; serializeJson(doc, out);
    server.send(200, "application/json", out);
  });

  server.on("/api/home", HTTP_POST, [](){
    handleCORS();
    // Simula homing completo zerando posições
    for (int i = 0; i < 6; i++) g_joint_angles[i] = 0.0;
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });

  server.on("/api/stop", HTTP_POST, [](){
    handleCORS();
    // E-STOP: limpa a fila de movimento
    g_seg_tail = g_seg_head;
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });

  server.on("/api/move/joints", HTTP_POST, [](){
    handleCORS();
    StaticJsonDocument<512> doc;
    deserializeJson(doc, server.arg("plain"));
    float target[6];
    for (int i = 0; i < 6; i++) {
      target[i] = doc["target"]["j" + String(i + 1)] | g_joint_angles[i];
    }
    plan_move_j(target, doc["speed_pct"] | 30.0);
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });

  server.on("/api/move/linear", HTTP_POST, [](){
    handleCORS();
    StaticJsonDocument<512> doc;
    deserializeJson(doc, server.arg("plain"));
    CartesianPose target;
    target.x = doc["target"]["x"] | g_current_pose.x;
    target.y = doc["target"]["y"] | g_current_pose.y;
    target.z = doc["target"]["z"] | g_current_pose.z;
    target.rx = doc["target"]["rx"] | g_current_pose.rx;
    target.ry = doc["target"]["ry"] | g_current_pose.ry;
    target.rz = doc["target"]["rz"] | g_current_pose.rz;
    plan_move_l(target, doc["speed_pct"] | 30.0);
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });

  server.onNotFound([](){
    if (server.method() == HTTP_OPTIONS) {
      handleCORS();
      server.send(204);
    } else {
      server.send(404, "text/plain", "Not Found");
    }
  });

  server.begin();
  Serial.println("[Core 0] Servidor HTTP embarcado ativo na porta 80");
}

// ============================================================================
// MATEMÁTICA E TRAJETÓRIA (Fase 5 - inalterada)
// ============================================================================
void update_closed_loop_pid() {
  const float dt = 0.005;
  portENTER_CRITICAL(&timerMux);
  for (int i = 0; i < 3; i++) {
    float desired_deg = isr_steps_target[i] / STEPS_PER_DEG[i];
    float real_deg = g_encoders[i].current_deg;
    float error = desired_deg - real_deg;
    float err_diff = (error - g_pid_last_err[i]) / dt;
    g_pid_last_err[i] = error;
    float tanh_factor = tanh(PID_GAMMA[i] * abs(error));
    float u_speed_deg_s = PID_KP[i] * error * tanh_factor + PID_KD[i] * err_diff;
    u_speed_deg_s = constrain(u_speed_deg_s, -MAX_SPEED_DEG_S[i], MAX_SPEED_DEG_S[i]);
    float speed_steps_s = abs(u_speed_deg_s) * STEPS_PER_DEG[i];
    if (speed_steps_s > 1.0 && abs(error) > 0.05) {
      uint32_t delay_val = (uint32_t)(20000.0 / speed_steps_s);
      if (delay_val < 1) delay_val = 1;
      isr_step_delay[i] = delay_val;
    } else {
      if (abs(error) <= 0.05) isr_steps_target[i] = isr_steps_current[i];
    }
  }
  portEXIT_CRITICAL(&timerMux);
}

// Funções Matemáticas, I2C, UART da Fase 5 (inalteradas)
void get_dh_matrix(float theta, float d, float a, float alpha, float out[4][4]) {
  float c_t = cos(theta), s_t = sin(theta);
  float c_a = cos(alpha), s_a = sin(alpha);
  out[0][0] = c_t; out[0][1] = -s_t * c_a; out[0][2] = s_t * s_a;  out[0][3] = a * c_t;
  out[1][0] = s_t; out[1][1] = c_t * c_a;  out[1][2] = -c_t * s_a; out[1][3] = a * s_t;
  out[2][0] = 0.0; out[2][1] = s_a;        out[2][2] = c_a;        out[2][3] = d;
  out[3][0] = 0.0; out[3][1] = 0.0;        out[3][2] = 0.0;        out[3][3] = 1.0;
}
void multiply_matrix(float A[4][4], float B[4][4], float out[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      out[i][j] = 0.0;
      for (int k = 0; k < 4; k++) out[i][j] += A[i][k] * B[k][j];
    }
  }
}
void forward_kinematics(const float q_rad[6], float out_pos[3], float R[3][3]) {
  float dh_params[6][4] = {
    { LINK_L1, 0.0,     (float)M_PI/2,  0.0          },
    { 0.0,     LINK_L2, 0.0,            (float)-M_PI/2 },
    { 0.0,     LINK_L3, 0.0,            0.0          },
    { 0.0,     0.0,     (float)-M_PI/2, (float)-M_PI/2 },
    { 0.0,     0.0,     (float)M_PI/2,  0.0          },
    { LINK_L6, 0.0,     0.0,            0.0          }
  };
  float T[4][4] = { {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} };
  for (int i = 0; i < 6; i++) {
    float A[4][4]; float theta = q_rad[i] + dh_params[i][3];
    get_dh_matrix(theta, dh_params[i][0], dh_params[i][1], dh_params[i][2], A);
    float temp[4][4]; multiply_matrix(T, A, temp); memcpy(T, temp, sizeof(temp));
  }
  out_pos[0] = T[0][3]; out_pos[1] = T[1][3]; out_pos[2] = T[2][3];
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) R[i][j] = T[i][j];
  }
}
void matrix_to_euler(float R[3][3], float& rx, float& ry, float& rz) {
  ry = asin(-R[2][0]);
  if (cos(ry) > 0.001) {
    rz = atan2(R[1][0], R[0][0]); rx = atan2(R[2][1], R[2][2]);
  } else {
    rz = 0.0; rx = atan2(-R[1][2], R[1][1]);
  }
  rx *= (180.0 / M_PI); ry *= (180.0 / M_PI); rz *= (180.0 / M_PI);
}
void euler_to_matrix(float rx_deg, float ry_deg, float rz_deg, float R[3][3]) {
  float rx = rx_deg * (M_PI / 180.0); float ry = ry_deg * (M_PI / 180.0); float rz = rz_deg * (M_PI / 180.0);
  float cx = cos(rx), sx = sin(rx); float cy = cos(ry), sy = sin(ry); float cz = cos(rz), sz = sin(rz);
  R[0][0] = cz * cy; R[0][1] = cz * sy * sx - sz * cx; R[0][2] = cz * sy * cx + sz * sx;
  R[1][0] = sz * cy; R[1][1] = sz * sy * sx + cz * cx; R[1][2] = sz * sy * cx - cz * sx;
  R[2][0] = -sy; R[2][1] = cy * sx; R[2][2] = cy * cx;
}
bool inverse_kinematics(const float target_pos[3], float target_rot[3][3], float out_q_rad[6]) {
  float wcp[3]; wcp[0] = target_pos[0] - LINK_L6 * target_rot[0][2]; wcp[1] = target_pos[1] - LINK_L6 * target_rot[1][2]; wcp[2] = target_pos[2] - LINK_L6 * target_rot[2][2];
  out_q_rad[0] = atan2(wcp[1], wcp[0]); float r = sqrt(wcp[0]*wcp[0] + wcp[1]*wcp[1]); float z_prime = wcp[2] - LINK_L1; float X_p = r; float Y_p = -z_prime;
  float d_sq = X_p*X_p + Y_p*Y_p; float d = sqrt(d_sq); if (d > (LINK_L2 + LINK_L3) || d < abs(LINK_L2 - LINK_L3)) return false;
  float cos_q3 = (d_sq - LINK_L2*LINK_L2 - LINK_L3*LINK_L3) / (2.0 * LINK_L2 * LINK_L3); cos_q3 = constrain(cos_q3, -1.0, 1.0); out_q_rad[2] = -acos(cos_q3);
  float A = LINK_L2 + LINK_L3 * cos(out_q_rad[2]); float B = LINK_L3 * sin(out_q_rad[2]); float sin_q2 = (A * X_p - B * Y_p) / d_sq; float cos_q2 = (B * X_p + A * Y_p) / d_sq;
  out_q_rad[1] = atan2(sin_q2, cos_q2);
  float dh_params[3][4] = { { LINK_L1, 0.0, (float)M_PI/2, 0.0 }, { 0.0, LINK_L2, 0.0, (float)-M_PI/2 }, { 0.0, LINK_L3, 0.0, 0.0 } };
  float T0_3[4][4] = { {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} };
  for (int i = 0; i < 3; i++) {
    float A_mat[4][4]; float theta = out_q_rad[i] + dh_params[i][3]; get_dh_matrix(theta, dh_params[i][0], dh_params[i][1], dh_params[i][2], A_mat);
    float temp[4][4]; multiply_matrix(T0_3, A_mat, temp); memcpy(T0_3, temp, sizeof(temp));
  }
  float R0_3_T[3][3]; for (int i = 0; i < 3; i++) { for (int j = 0; j < 3; j++) R0_3_T[i][j] = T0_3[j][i]; }
  float R3_6[3][3]; for (int i = 0; i < 3; i++) { for (int j = 0; j < 3; j++) { R3_6[i][j] = 0.0; for (int k = 0; k < 3; k++) R3_6[i][j] += R0_3_T[i][k] * target_rot[k][j]; } }
  float cos_q5 = R3_6[2][2]; cos_q5 = constrain(cos_q5, -1.0, 1.0); out_q_rad[4] = acos(cos_q5);
  if (abs(out_q_rad[4]) > 0.001) { out_q_rad[3] = atan2(R3_6[0][2], -R3_6[1][2]); out_q_rad[5] = atan2(R3_6[2][1], -R3_6[2][0]); }
  else { out_q_rad[3] = 0.0; out_q_rad[5] = atan2(-R3_6[0][1], R3_6[0][0]); }
  return true;
}
bool check_soft_limits(const float q_deg[6]) {
  for (int i = 0; i < 6; i++) { if (q_deg[i] < LIMIT_MIN_DEG[i] || q_deg[i] > LIMIT_MAX_DEG[i]) return false; }
  return true;
}
bool push_segment(const float q_deg[6], uint32_t duration_ms) {
  int next_head = (g_seg_head + 1) % MAX_SEGMENTS; if (next_head == g_seg_tail) return false;
  TrajectorySegment& seg = g_segments[g_seg_head]; for (int i = 0; i < 6; i++) seg.steps_target[i] = (int32_t)(q_deg[i] * STEPS_PER_DEG[i]);
  seg.duration_ms = duration_ms; seg.active = true; g_seg_head = next_head; return true;
}
void plan_move_j(const float target_joints_deg[6], float speed_pct) {
  if (!check_soft_limits(target_joints_deg)) return;
  float max_diff = 0.0; for (int i = 0; i < 6; i++) { float diff = abs(target_joints_deg[i] - g_joint_angles[i]); if (diff > max_diff) max_diff = diff; }
  if (max_diff < 0.01) return;
  float speed_factor = speed_pct / 100.0; float duration_s = max_diff / (MAX_SPEED_DEG_S[0] * speed_factor); if (duration_s < 0.5) duration_s = 0.5;
  int slices = (int)(duration_s * 1000.0 / SEG_SLICE_MS); if (slices < 1) slices = 1;
  float start_joints[6]; memcpy(start_joints, g_joint_angles, sizeof(start_joints));
  for (int s = 1; s <= slices; s++) {
    float tau = (float)s / slices; float factor = 10 * pow(tau, 3) - 15 * pow(tau, 4) + 6 * pow(tau, 5); float slice_joints[6];
    for (int i = 0; i < 6; i++) slice_joints[i] = start_joints[i] + (target_joints_deg[i] - start_joints[i]) * factor;
    while (!push_segment(slice_joints, SEG_SLICE_MS)) delay(5);
  }
}
void plan_move_l(const CartesianPose& target_pose, float speed_pct) {
  float R_target[3][3]; euler_to_matrix(target_pose.rx, target_pose.ry, target_pose.rz, R_target);
  float target_pos[3] = {target_pose.x, target_pose.y, target_pose.z}; float target_joints_rad[6];
  if (!inverse_kinematics(target_pos, R_target, target_joints_rad)) return;
  float target_joints_deg[6]; for (int i = 0; i < 6; i++) target_joints_deg[i] = target_joints_rad[i] * (180.0 / M_PI);
  if (!check_soft_limits(target_joints_deg)) return;
  float start_pos[3] = {g_current_pose.x, g_current_pose.y, g_current_pose.z}; float start_rot[3] = {g_current_pose.rx, g_current_pose.ry, g_current_pose.rz};
  float diff_pos = sqrt(pow(target_pos[0] - start_pos[0], 2) + pow(target_pos[1] - start_pos[1], 2) + pow(target_pos[2] - start_pos[2], 2));
  if (diff_pos < 0.1) return;
  float speed_factor = speed_pct / 100.0; float duration_s = diff_pos / (80.0 * speed_factor); if (duration_s < 0.5) duration_s = 0.5;
  int slices = (int)(duration_s * 1000.0 / SEG_SLICE_MS); if (slices < 1) slices = 1;
  for (int s = 1; s <= slices; s++) {
    float tau = (float)s / slices; float factor = 10 * pow(tau, 3) - 15 * pow(tau, 4) + 6 * pow(tau, 5);
    float slice_pos[3]; slice_pos[0] = start_pos[0] + (target_pos[0] - start_pos[0]) * factor; slice_pos[1] = start_pos[1] + (target_pos[1] - start_pos[1]) * factor; slice_pos[2] = start_pos[2] + (target_pos[2] - start_pos[2]) * factor;
    float slice_rot[3]; slice_rot[0] = start_rot[0] + (target_pose.rx - start_rot[0]) * factor; slice_rot[1] = start_rot[1] + (target_pose.ry - start_rot[1]) * factor; slice_rot[2] = start_rot[2] + (target_pose.rz - start_rot[2]) * factor;
    float R_slice[3][3]; euler_to_matrix(slice_rot[0], slice_rot[1], slice_rot[2], R_slice); float slice_joints_rad[6];
    if (inverse_kinematics(slice_pos, R_slice, slice_joints_rad)) {
      float slice_joints_deg[6]; for (int i = 0; i < 6; i++) slice_joints_deg[i] = slice_joints_rad[i] * (180.0 / M_PI);
      while (!push_segment(slice_joints_deg, SEG_SLICE_MS)) delay(5);
    } else return;
  }
}
void execute_trajectory_tick() {
  if (g_seg_tail == g_seg_head) return;
  TrajectorySegment& seg = g_segments[g_seg_tail];
  int16_t s4 = (int16_t)seg.steps_target[3]; int16_t s5 = (int16_t)seg.steps_target[4]; int16_t s6 = (int16_t)seg.steps_target[5];
  bool handshake_ok = sendFrameToUno(s4, s5, s6, 50, 20);
  if (handshake_ok) {
    portENTER_CRITICAL(&timerMux);
    for (int i = 0; i < 3; i++) {
      int32_t delta = abs(seg.steps_target[i] - isr_steps_current[i]);
      if (delta > 0) {
        isr_steps_target[i] = seg.steps_target[i]; isr_step_delay[i] = (uint32_t)(((float)seg.duration_ms * 1000.0 / delta) / 50.0);
        if (isr_step_delay[i] < 1) isr_step_delay[i] = 1;
        isr_step_accum[i] = 0;
      }
    }
    portEXIT_CRITICAL(&timerMux);
    digitalWrite(TRIGGER_PIN, LOW); delayMicroseconds(5); digitalWrite(TRIGGER_PIN, HIGH);
    for (int i = 0; i < 6; i++) g_joint_angles[i] = seg.steps_target[i] / STEPS_PER_DEG[i];
    float pos[3]; float R[3][3]; float q_rad[6];
    for (int i = 0; i < 6; i++) q_rad[i] = g_joint_angles[i] * (M_PI / 180.0);
    forward_kinematics(q_rad, pos, R);
    g_current_pose.x = pos[0]; g_current_pose.y = pos[1]; g_current_pose.z = pos[2];
    matrix_to_euler(R, g_current_pose.rx, g_current_pose.ry, g_current_pose.rz);
    seg.active = false; g_seg_tail = (g_seg_tail + 1) % MAX_SEGMENTS;
  }
}
void select_tca_channel(uint8_t channel) {
  Wire.beginTransmission(TCA9548A_ADDR); Wire.write(1 << channel); Wire.endTransmission();
}
uint16_t read_as5600_raw(uint8_t channel) {
  select_tca_channel(channel); Wire.beginTransmission(0x36); Wire.write(0x0C); if (Wire.endTransmission() != 0) return 0xFFFF;
  Wire.requestFrom(0x36, 2); if (Wire.available() >= 2) return (Wire.read() << 8) | Wire.read(); return 0xFFFF;
}
void update_encoder(uint8_t idx, uint8_t channel) {
  uint16_t raw = read_as5600_raw(channel); if (raw == 0xFFFF) return;
  EncoderTracker& tracker = g_encoders[idx];
  if (!tracker.initialized) {
    tracker.last_raw = raw; tracker.turns = 0; tracker.initialized = true; tracker.accumulated_ticks = raw;
  } else {
    int16_t diff = raw - tracker.last_raw; if (diff < -2048) tracker.turns++; else if (diff > 2048) tracker.turns--;
    tracker.last_raw = raw; tracker.accumulated_ticks = ((int32_t)tracker.turns * 4096) + raw;
  }
  tracker.current_deg = tracker.accumulated_ticks * (360.0 / 4096.0);
}
uint8_t computeChecksum(const UnoFrame& frame) {
  const uint8_t* ptr = (const uint8_t*)&frame; uint8_t xor_sum = 0; for (size_t i = 0; i < 9; i++) xor_sum ^= ptr[i]; return xor_sum;
}
bool sendFrameToUno(int16_t s4, int16_t s5, int16_t s6, uint8_t vel, uint8_t accel) {
  UnoFrame frame; frame.preamble = 0xAA; frame.steps_j4 = s4; frame.steps_j5 = s5; frame.steps_j6 = s6; frame.velocity = vel; frame.acceleration = accel; frame.checksum = computeChecksum(frame);
  while (SerialUno.available()) SerialUno.read(); SerialUno.write((const uint8_t*)&frame, sizeof(UnoFrame)); return waitForAck(500);
}
bool waitForAck(uint32_t timeout_ms) {
  uint32_t start_time = millis();
  while (millis() - start_time < timeout_ms) {
    if (SerialUno.available() >= 1) { uint8_t resp = SerialUno.read(); if (resp == 0x06) return true; if (resp == 0x15) return false; }
    delay(1);
  }
  return false;
}
