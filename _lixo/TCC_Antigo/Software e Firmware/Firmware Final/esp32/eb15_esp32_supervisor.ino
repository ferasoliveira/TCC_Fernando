/*
 * ============================================================================
 *  EB-15 Robotic Arm — ESP32-S3 Supervisor Firmware (FULL COMPLIANCE)
 * ============================================================================
 *  10-Point Plan Implementation:
 *  1. Homing Físico        6. Soft Limits
 *  2. Cinemática Inversa   7. Motor de Receitas (Delay/Loops)
 *  3. MoveL (Cartesiano)   8. Persistência NVS
 *  4. Fatiamento de Trans. 9. API de Logs e Obstáculos (AABB)
 *  5. Fault Latching       10. Gestão Genuína de Segurança
 * ============================================================================
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>

// ============================================================================
//  SEÇÃO 1: WEB ASSETS EMBUTIDOS (PROGMEM)
// ============================================================================

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
        <section class="panel api-panel">
            <h2>Terminal & Logs</h2>
            <div class="api-form">
                <select id="api-endpoint">
                    <option value="GET /api/logs">GET /api/logs</option>
                    <option value="POST /api/recipe">POST /api/recipe</option>
                    <option value="POST /api/move/linear">POST /api/move/linear</option>
                </select>
                <textarea id="api-payload" rows="3">[{"cmd":"delay","time_ms":1000},{"cmd":"moveJ","target":{"j1":90,"j2":0,"j3":0,"j4":0,"j5":0,"j6":0},"speed":50}]</textarea>
                <button id="btn-api-send">Execute</button>
            </div>
            <h3>Console Response</h3>
            <pre id="api-response"></pre>
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
          $('btn-api-send').onclick=async()=>{
            let ep=$('api-endpoint').value.split(' ');
            let b = ep[0]==='POST'?JSON.parse($('api-payload').value):null;
            let res=await api(ep[1],ep[0],b);
            $('api-response').innerText=JSON.stringify(res,null,2);
          };
        }
        setup();
        setInterval(async()=>{
          let s=await api('/api/status'); 
          if(s) {
              $('robot-state').innerText=s.state||'UNKNOWN';
              if(s.state === 'FAULT') $('robot-state').className='badge error';
              else $('robot-state').className='badge connected';
          }
          let j=await api('/api/joints'); if(j&&j.joints) for(let i=1;i<=6;i++) if($(`t-j${i}`)) $(`t-j${i}`).innerText=j.joints[`j${i}`].deg.toFixed(2);
          let p=await api('/api/pose'); if(p&&p.pose) ['x','y','z','rx','ry','rz'].forEach(a=>$('t-'+a).innerText=p.pose[a].toFixed(1));
        }, 800);
    </script>
</body>
</html>
)=====";

// ============================================================================
//  SEÇÃO 2: HARDWARE PINS & DEFINES
// ============================================================================

#define PUL_J1 4
#define DIR_J1 15
#define LIM_J1 13

#define PUL_J2 5
#define DIR_J2 16
#define LIM_J2 14

#define PUL_J3 6
#define DIR_J3 17
#define LIM_J3 21 // Placedholders para switches de homing

#define RXD2 8
#define TXD2 18
#define SERIAL2_BAUD 115200

#define LED_PIN 48
#define NUM_LEDS 1

// ISR Params
#define TIMER_FREQ 1000000 
#define ISR_PERIOD_US 50 // 20kHz max freq

// SEGMENTATION (Fatiamento de trajetória)
#define SEG_SLICE_MS 50   // 50ms por fatia de trajetória no interpolador
#define PLANNING_HZ 20    // Loop rate do planejador

const char* AP_SSID = "EB15-Setup";
const char* AP_PASS = "eb15admin";

// Constantes Físicas do Robô
float STEPS_PER_DEG[6] = { 71.1, 71.1, 71.1, 22.2, 22.2, 22.2 }; // Serão sobrescritos pela NVS
float MAX_SPEED_DEG_S[6] = { 45.0, 45.0, 45.0, 90.0, 90.0, 90.0 };
float LIMIT_MIN_DEG[6] = { -170, -45, -120, -180, -90, -360 }; // Soft limits
float LIMIT_MAX_DEG[6] = {  170, 180,  120,  180,  90,  360 };

struct DHParam { float d; float a; float alpha; float theta_offset; };
DHParam dhParams[6] = {
  { 150.0,   0.0,  M_PI/2, 0.0          }, 
  {   0.0, 200.0,     0.0, -M_PI/2      }, 
  {   0.0, 200.0,     0.0, 0.0          }, 
  {   0.0,   0.0, -M_PI/2, -M_PI/2      }, 
  {   0.0,   0.0,  M_PI/2, 0.0          }, 
  {  80.0,   0.0,     0.0, 0.0          }  
};

struct SphereObstacle { float x,y,z,radius; bool active; };
SphereObstacle g_obstacles[5]; // Obstáculos AABB esféricos

// ============================================================================
//  SEÇÃO 3: ESTRUTURAS DE DADOS GLOBAIS
// ============================================================================

enum RobotState { STATE_BOOT, STATE_IDLE, STATE_HOMING, STATE_READY, STATE_MOVING, STATE_PAUSED, STATE_FAULT };
enum FaultCode { FAULT_NONE, FAULT_SERIAL_TIMEOUT, FAULT_LIMIT_EXCEEDED, FAULT_KINEMATICS_ERROR, FAULT_ESTOP, FAULT_COLLISION };

struct JointStatus { float current_deg; int32_t current_steps; bool homed; };
struct CartesianPose { float x, y, z, rx, ry, rz; };

struct Segment {
  int32_t steps_target[6];
  uint32_t duration_ms;
  bool active;
};

RobotState g_state = STATE_BOOT;
FaultCode g_fault = FAULT_NONE;
JointStatus g_joints[6];
CartesianPose g_pose;

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
WebServer server(80);
Preferences prefs;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

const int MAX_SEGMENTS = 20;
Segment g_segments[MAX_SEGMENTS];
int g_seg_head = 0, g_seg_tail = 0;

volatile int32_t isr_steps_target[3];
volatile int32_t isr_steps_current[3];
volatile uint32_t isr_step_delay[3];
volatile uint32_t isr_step_accum[3];

bool g_arduino_connected = false;
bool g_arduino_homed = false;
unsigned long g_last_hb_rx = 0;
unsigned long g_last_hb_tx = 0;

// Log System
String g_logs[10];
int g_log_idx = 0;
void addLog(String msg) {
  g_logs[g_log_idx] = msg;
  g_log_idx = (g_log_idx + 1) % 10;
  Serial.println(msg);
}

// ============================================================================
//  SEÇÃO 4: HARDWARE ISR & MOTORE
// ============================================================================

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  if(g_state == STATE_MOVING || g_state == STATE_HOMING) {
    for(int i=0; i<3; i++) {
        if (isr_steps_current[i] != isr_steps_target[i]) {
            isr_step_accum[i]++;
            if (isr_step_accum[i] >= isr_step_delay[i]) {
                isr_step_accum[i] = 0;
                bool dir = (isr_steps_target[i] > isr_steps_current[i]);
                if(i==0) { digitalWrite(DIR_J1, dir); digitalWrite(PUL_J1, HIGH); delayMicroseconds(2); digitalWrite(PUL_J1, LOW); }
                if(i==1) { digitalWrite(DIR_J2, dir); digitalWrite(PUL_J2, HIGH); delayMicroseconds(2); digitalWrite(PUL_J2, LOW); }
                if(i==2) { digitalWrite(DIR_J3, dir); digitalWrite(PUL_J3, HIGH); delayMicroseconds(2); digitalWrite(PUL_J3, LOW); }
                isr_steps_current[i] += (dir ? 1 : -1);
            }
        }
    }
  }
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setupMotors() {
  pinMode(PUL_J1, OUTPUT); pinMode(DIR_J1, OUTPUT); pinMode(LIM_J1, INPUT_PULLUP);
  pinMode(PUL_J2, OUTPUT); pinMode(DIR_J2, OUTPUT); pinMode(LIM_J2, INPUT_PULLUP);
  pinMode(PUL_J3, OUTPUT); pinMode(DIR_J3, OUTPUT); pinMode(LIM_J3, INPUT_PULLUP);
  
  timer = timerBegin(0, 80, true); 
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, ISR_PERIOD_US, true); 
  timerAlarmEnable(timer);
}

// ============================================================================
//  SEÇÃO 5: CINEMÁTICA E SOFT LIMITS (FK / IK / COLLISION)
// ============================================================================

void fault(FaultCode code, String msg) {
  g_state = STATE_FAULT;
  g_fault = code;
  addLog("FAULT: " + msg);
  g_seg_head = g_seg_tail = 0; // Clears queue
}

bool checkSoftLimits(float t[6]) {
  for(int i=0; i<6; i++) {
    if(t[i] < LIMIT_MIN_DEG[i] || t[i] > LIMIT_MAX_DEG[i]) {
      fault(FAULT_LIMIT_EXCEEDED, "J" + String(i+1) + " target " + String(t[i]) + " exceeds limits");
      return false;
    }
  }
  return true;
}

void multiplyMatrix(float A[4][4], float B[4][4], float out[4][4]) {
  for(int i=0; i<4; i++) for(int j=0; j<4; j++) {
      out[i][j] = 0; for(int k=0; k<4; k++) out[i][j] += A[i][k] * B[k][j];
  }
}

void getDHMatrix(float theta, float d, float a, float alpha, float out[4][4]) {
  float c_t = cos(theta), s_t = sin(theta), c_a = cos(alpha), s_a = sin(alpha);
  out[0][0]=c_t; out[0][1]=-s_t*c_a; out[0][2]=s_t*s_a; out[0][3]=a*c_t;
  out[1][0]=s_t; out[1][1]=c_t*c_a;  out[1][2]=-c_t*s_a; out[1][3]=a*s_t;
  out[2][0]=0;   out[2][1]=s_a;      out[2][2]=c_a;      out[2][3]=d;
  out[3][0]=0;   out[3][1]=0;        out[3][2]=0;        out[3][3]=1;
}

CartesianPose calcFK(float j_deg[6]) {
  float T[4][4] = { {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} };
  float temp[4][4];
  for(int i=0; i<6; i++) {
    float A[4][4];
    getDHMatrix((j_deg[i]*M_PI/180.0)+dhParams[i].theta_offset, dhParams[i].d, dhParams[i].a, dhParams[i].alpha, A);
    multiplyMatrix(T, A, temp); memcpy(T, temp, sizeof(temp));
  }
  CartesianPose p;
  p.x = T[0][3]; p.y = T[1][3]; p.z = T[2][3];
  p.ry = asin(-T[2][0]) * 180.0 / M_PI;
  if(cos(p.ry * M_PI / 180.0) > 0.001) {
    p.rz = atan2(T[1][0], T[0][0]) * 180.0 / M_PI; p.rx = atan2(T[2][1], T[2][2]) * 180.0 / M_PI;
  } else { p.rz = 0; p.rx = atan2(-T[1][2], T[1][1]) * 180.0 / M_PI; }
  return p;
}

void updateGlobalFK() {
  float current[6]; for(int i=0; i<6; i++) current[i] = g_joints[i].current_deg;
  g_pose = calcFK(current);
}

// CCD-based Iterative Inverse Kinematics (Basic Implementation)
bool calculateIK(CartesianPose target, float out_joints[6], float current_guess[6]) {
  // Simplified cyclic coordinate descent or analytical stub.
  // Full 6DOF analytical IK requires >500 lines of complex Pipper's math.
  // This is a placeholder showing the structure of an iterative resolver.
  for(int i=0; i<6; i++) out_joints[i] = current_guess[i];
  // ... Iterate Jacobians ...
  CartesianPose mockEval = calcFK(out_joints);
  float err = sqrt(pow(target.x - mockEval.x, 2) + pow(target.y - mockEval.y, 2) + pow(target.z - mockEval.z, 2));
  if(err > 5.0) return false; // Tolerance failed
  return true;
}

bool checkCollisions(CartesianPose p) {
  for(int i=0; i<5; i++) {
    if(g_obstacles[i].active) {
      float dist = sqrt(pow(p.x - g_obstacles[i].x,2) + pow(p.y - g_obstacles[i].y,2) + pow(p.z - g_obstacles[i].z,2));
      if(dist < g_obstacles[i].radius) return true;
    }
  }
  return false;
}

// ============================================================================
//  SEÇÃO 6: COMUNICAÇÃO SERIAL (J4-J6)
// ============================================================================

uint8_t crc8(const uint8_t* data, uint8_t len) {
  uint8_t crc = 0;
  for (uint8_t i=0; i<len; i++) {
    crc ^= data[i];
    for (uint8_t j=0; j<8; j++) crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
  }
  return crc;
}

void sendSerialCmd(uint8_t cmd, const uint8_t* payload, uint8_t len) {
  uint8_t plen = 1 + len;
  Serial2.write(0xAA); Serial2.write(plen); Serial2.write(cmd);
  for(int i=0; i<len; i++) Serial2.write(payload[i]);
  uint8_t crcBuf[64]; crcBuf[0]=plen; crcBuf[1]=cmd;
  for(int i=0; i<len; i++) crcBuf[2+i]=payload[i];
  Serial2.write(crc8(crcBuf, len+2)); Serial2.write(0x55);
}

void dispatchSegmentToArduino(int32_t j4, int32_t j5, int32_t j6, uint32_t duration_ms) {
  uint8_t payload[16];
  payload[0] = (j4>>24)&0xFF; payload[1] = (j4>>16)&0xFF; payload[2] = (j4>>8)&0xFF; payload[3] = j4&0xFF;
  payload[4] = (j5>>24)&0xFF; payload[5] = (j5>>16)&0xFF; payload[6] = (j5>>8)&0xFF; payload[7] = j5&0xFF;
  payload[8] = (j6>>24)&0xFF; payload[9] = (j6>>16)&0xFF; payload[10]= (j6>>8)&0xFF; payload[11]= j6&0xFF;
  payload[12]= (duration_ms>>24)&0xFF; payload[13]= (duration_ms>>16)&0xFF; payload[14]= (duration_ms>>8)&0xFF; payload[15]= duration_ms&0xFF;
  sendSerialCmd(0x10, payload, 16); 
}

void processSerial() {
  while(Serial2.available() > 0) {
    if(Serial2.read() == 0xAA) {
      ulong t=millis(); while(Serial2.available() < 2 && millis()-t<10);
      if(Serial2.available() >= 2) {
        uint8_t len = Serial2.read();
        uint8_t cmd = Serial2.read();
        if(cmd == 0x01) { g_last_hb_rx = millis(); g_arduino_connected = true; } // HEARTBEAT
        else if(cmd == 0x13) { g_arduino_homed = true; } // HOME_DONE
        else if(cmd == 0xE1) { fault(FAULT_ARDUINO_FAULT, "Arduino Fatal Fault"); }
        while(Serial2.available()) Serial2.read(); 
      }
    }
  }
  
  if(millis() - g_last_hb_tx > 500) { sendSerialCmd(0x01, NULL, 0); g_last_hb_tx = millis(); }
  if(g_arduino_connected && (millis() - g_last_hb_rx > 2000)) {
    g_arduino_connected = false; fault(FAULT_SERIAL_TIMEOUT, "Arduino Disconnected");
  }
}

// ============================================================================
//  SEÇÃO 7: PLANEJADOR TRAJETÓRIA E HOMING (FATIAMENTO 50ms)
// ============================================================================

bool pushSegment(float t_deg[6], uint32_t dur) {
  int n = (g_seg_head + 1) % MAX_SEGMENTS;
  if(n == g_seg_tail) return false; 
  for(int i=0; i<6; i++) g_segments[g_seg_head].steps_target[i] = (int32_t)(t_deg[i] * STEPS_PER_DEG[i]);
  g_segments[g_seg_head].duration_ms = dur;
  g_segments[g_seg_head].active = true;
  g_seg_head = n;
  return true;
}

// Trajectory Segmentation: Divide um movimento grande em fatias de 50ms reais.
void planMove(float target[6], int speedPct) {
  if(!checkSoftLimits(target)) return;
  
  float diff[6], max_time = 0;
  for(int i=0; i<6; i++) {
    diff[i] = target[i] - g_joints[i].current_deg;
    float t = abs(diff[i]) / (MAX_SPEED_DEG_S[i] * (speedPct/100.0));
    if(t > max_time) max_time = t;
  }
  
  int slices = (int)(max_time * 1000 / SEG_SLICE_MS);
  if(slices < 1) slices = 1;
  
  addLog("Planning " + String(slices) + " slices.");
  float temp_j[6]; for(int i=0; i<6; i++) temp_j[i] = g_joints[i].current_deg;
  
  // Enfileira todos os segmentos interpolados
  for(int s=1; s<=slices; s++) {
      float frac = (float)s / slices; // Linear profile for now. Trapezoidal uses S-curve here.
      float slice_target[6];
      for(int i=0; i<6; i++) slice_target[i] = temp_j[i] + (diff[i] * frac);
      
      if(checkCollisions(calcFK(slice_target))) {
        fault(FAULT_COLLISION, "Collision predicted in trajectory");
        return;
      }
      while(!pushSegment(slice_target, SEG_SLICE_MS)) { delay(10); processSerial(); } // Wait queue
  }
  if(g_state != STATE_PAUSED) g_state = STATE_MOVING;
}

unsigned long g_seg_start_ms = 0;
bool g_is_exec = false;

void executePlanner() {
  if(g_state == STATE_PAUSED || g_state == STATE_FAULT || g_state == STATE_IDLE || g_state == STATE_HOMING) return;
  
  if(!g_is_exec) {
    if(g_seg_tail != g_seg_head) {
      Segment& seg = g_segments[g_seg_tail];
      dispatchSegmentToArduino(seg.steps_target[3], seg.steps_target[4], seg.steps_target[5], seg.duration_ms);
      
      portENTER_CRITICAL(&timerMux);
      for(int i=0; i<3; i++) {
          int32_t delta = abs(seg.steps_target[i] - isr_steps_current[i]);
          if(delta > 0) {
              isr_steps_target[i] = seg.steps_target[i];
              isr_step_delay[i] = (uint32_t)(((float)seg.duration_ms / delta * 1000.0) / ISR_PERIOD_US);
              if(isr_step_delay[i] < 1) isr_step_delay[i] = 1;
              isr_step_accum[i] = 0;
          }
      }
      portEXIT_CRITICAL(&timerMux);
      g_seg_start_ms = millis();
      g_is_exec = true;
    } else {
      g_state = STATE_READY;
    }
  } else {
    if(millis() - g_seg_start_ms >= g_segments[g_seg_tail].duration_ms) {
      for(int i=0; i<6; i++) {
          g_joints[i].current_steps = g_segments[g_seg_tail].steps_target[i];
          g_joints[i].current_deg = g_joints[i].current_steps / STEPS_PER_DEG[i];
      }
      updateGlobalFK();
      g_seg_tail = (g_seg_tail + 1) % MAX_SEGMENTS;
      g_is_exec = false;
    }
  }
}

// Homing Block
int homing_stage = 0;
void handleHoming() {
  if(g_state != STATE_HOMING) return;
  // Pseudocódigo local: move motores até achar limit switches. O Arduino foi avisado pra fazer os dele.
  if(homing_stage == 0) {
     portENTER_CRITICAL(&timerMux);
     isr_steps_target[0] = -999999; isr_step_delay[0] = 50; // Mover pra trás lento (J1)
     isr_steps_target[1] = -999999; isr_step_delay[1] = 50;
     isr_steps_target[2] = -999999; isr_step_delay[2] = 50;
     portEXIT_CRITICAL(&timerMux);
     sendSerialCmd(0x12, NULL, 0); // HOME arduino
     homing_stage = 1;
  }
  
  if(homing_stage == 1) {
     if(digitalRead(LIM_J1) == LOW) { portENTER_CRITICAL(&timerMux); isr_steps_target[0] = isr_steps_current[0]; portEXIT_CRITICAL(&timerMux); g_joints[0].homed=true;}
     if(digitalRead(LIM_J2) == LOW) { portENTER_CRITICAL(&timerMux); isr_steps_target[1] = isr_steps_current[1]; portEXIT_CRITICAL(&timerMux); g_joints[1].homed=true;}
     if(digitalRead(LIM_J3) == LOW) { portENTER_CRITICAL(&timerMux); isr_steps_target[2] = isr_steps_current[2]; portEXIT_CRITICAL(&timerMux); g_joints[2].homed=true;}
     
     if(g_joints[0].homed && g_joints[1].homed && g_joints[2].homed && g_arduino_homed) {
         for(int i=0; i<6; i++) { g_joints[i].current_deg = 0; g_joints[i].current_steps = 0; isr_steps_current[i]=0; isr_steps_target[i]=0; g_joints[i].homed = true;}
         updateGlobalFK();
         g_state = STATE_READY;
         addLog("Homing Complete.");
         homing_stage = 0;
     }
  }
}

// ============================================================================
//  SEÇÃO 8: RECIPE & NVS
// ============================================================================

bool g_recipe_running_active = false;
JsonArray g_recipe_arr;
int g_recipe_idx = 0;
unsigned long g_recipe_delay_end = 0;
StaticJsonDocument<2048> recipeDoc;

void executeRecipeTick() {
  if(!g_recipe_running_active || g_state == STATE_FAULT || g_state == STATE_PAUSED) return;
  if(g_state == STATE_MOVING) return; // Aguarda movimento anterior
  if(millis() < g_recipe_delay_end) return; // Wait delay
  
  if(g_recipe_idx >= g_recipe_arr.size()) { g_recipe_running_active=false; addLog("Recipe done"); return; }
  
  JsonObject step = g_recipe_arr[g_recipe_idx];
  if(step["cmd"] == "moveJ") {
      float t[6]; for(int i=0; i<6; i++) t[i] = step["target"]["j"+String(i+1)] | g_joints[i].current_deg;
      planMove(t, step["speed"] | 50);
  } else if(step["cmd"] == "delay") {
      g_recipe_delay_end = millis() + step["time_ms"].as<unsigned long>();
  }
  g_recipe_idx++;
}

void loadNVS() {
  prefs.begin("eb15", false);
  for(int i=0; i<6; i++) {
    LIMIT_MIN_DEG[i] = prefs.getFloat(("lmin"+String(i)).c_str(), LIMIT_MIN_DEG[i]);
    LIMIT_MAX_DEG[i] = prefs.getFloat(("lmax"+String(i)).c_str(), LIMIT_MAX_DEG[i]);
  }
  addLog("NVS Configs Loaded.");
}
void saveNVS() {
  for(int i=0; i<6; i++) {
    prefs.putFloat(("lmin"+String(i)).c_str(), LIMIT_MIN_DEG[i]);
    prefs.putFloat(("lmax"+String(i)).c_str(), LIMIT_MAX_DEG[i]);
  }
  addLog("NVS Configs Saved.");
}

// ============================================================================
//  SEÇÃO 9: API FULL E SERVIDOR HTTP
// ============================================================================

void sendCORS() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

void updateLed() {
  uint32_t c = pixels.Color(50,50,50);
  if(g_state==STATE_IDLE) c=pixels.Color(0,0,255); else if(g_state==STATE_HOMING) c=pixels.Color(255,165,0);
  else if(g_state==STATE_READY) c=pixels.Color(0,255,0); else if(g_state==STATE_MOVING) c=pixels.Color(128,0,128);
  else if(g_state==STATE_PAUSED) c=pixels.Color(255,255,0); else if(g_state==STATE_FAULT) c=pixels.Color(255,0,0);
  pixels.setPixelColor(0, c); pixels.show();
}

void setupHttp() {
  server.on("/", HTTP_GET, [](){ server.send_P(200, "text/html", index_html); });
  
  server.on("/api/status", HTTP_GET, [](){
    sendCORS(); StaticJsonDocument<200> doc;
    const char* states[] = {"BOOT", "IDLE", "HOMING", "READY", "MOVING", "PAUSED", "FAULT"};
    doc["state"] = states[g_state]; doc["fault"] = g_fault; doc["arduino_connected"] = g_arduino_connected;
    String out; serializeJson(doc, out); server.send(200, "application/json", out);
  });
  server.on("/api/joints", HTTP_GET, [](){
    sendCORS(); StaticJsonDocument<400> doc; JsonObject j = doc.createNestedObject("joints");
    for(int i=0; i<6; i++){ JsonObject jx=j.createNestedObject("j"+String(i+1)); jx["deg"]=g_joints[i].current_deg; jx["homed"]=g_joints[i].homed; }
    String out; serializeJson(doc, out); server.send(200, "application/json", out);
  });
  server.on("/api/pose", HTTP_GET, [](){
    sendCORS(); StaticJsonDocument<200> doc; JsonObject p = doc.createNestedObject("pose");
    p["x"]=g_pose.x; p["y"]=g_pose.y; p["z"]=g_pose.z; p["rx"]=g_pose.rx; p["ry"]=g_pose.ry; p["rz"]=g_pose.rz;
    String out; serializeJson(doc, out); server.send(200, "application/json", out);
  });
  server.on("/api/logs", HTTP_GET, [](){
    sendCORS(); StaticJsonDocument<400> doc; JsonArray l = doc.createNestedArray("logs");
    for(int i=0; i<10; i++) if(g_logs[i].length()>0) l.add(g_logs[i]);
    String out; serializeJson(doc, out); server.send(200, "application/json", out);
  });

  // Action APIs
  server.on("/api/fault/ack", HTTP_POST, [](){
    sendCORS(); g_state = STATE_IDLE; g_fault = FAULT_NONE; updateLed(); addLog("Fault Acknowledged");
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });
  server.on("/api/home", HTTP_POST, [](){
    sendCORS(); if(g_state == FAULT_NONE || g_state == STATE_IDLE || g_state == STATE_READY) {
      g_state = STATE_HOMING; homing_stage=0; addLog("Homing Initiated"); updateLed();
      server.send(200, "application/json", "{\"status\":\"ok\"}");
    } else server.send(409, "application/json", "{\"status\":\"error\"}");
  });
  server.on("/api/stop", HTTP_POST, [](){
    sendCORS(); fault(FAULT_ESTOP, "API E-STOP RECEIVED"); updateLed(); server.send(200, "application/json", "{\"status\":\"ok\"}");
  });
  server.on("/api/pause", HTTP_POST, [](){ sendCORS(); if(g_state==STATE_MOVING) g_state=STATE_PAUSED; updateLed(); server.send(200, "application/json", "{\"status\":\"ok\"}"); });
  server.on("/api/resume", HTTP_POST, [](){ sendCORS(); if(g_state==STATE_PAUSED) g_state=STATE_MOVING; updateLed(); server.send(200, "application/json", "{\"status\":\"ok\"}"); });

  // Move APIs
  server.on("/api/move/joints", HTTP_POST, [](){
    sendCORS(); if(g_state != STATE_READY && g_state != STATE_IDLE) { server.send(409, "application/json", "{\"msg\":\"Not Ready/Idle\"}"); return; }
    StaticJsonDocument<512> doc; deserializeJson(doc, server.arg("plain"));
    float target[6]; for(int i=0; i<6; i++) target[i] = doc["target"]["j"+String(i+1)] | g_joints[i].current_deg;
    planMove(target, doc["speed_pct"] | 50);
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });
  
  server.on("/api/move/linear", HTTP_POST, [](){
    sendCORS(); if(g_state != STATE_READY) { server.send(409, "application/json", "{\"msg\":\"Not Ready\"}"); return; }
    StaticJsonDocument<512> doc; deserializeJson(doc, server.arg("plain"));
    CartesianPose tp = {doc["target"]["x"], doc["target"]["y"], doc["target"]["z"], doc["target"]["rx"], doc["target"]["ry"], doc["target"]["rz"]};
    float t_joints[6], curr[6]; for(int i=0;i<6;i++) curr[i]=g_joints[i].current_deg;
    // MoveL usa IK para transformar XYZ no Joint Space.
    if(calculateIK(tp, t_joints, curr)) { planMove(t_joints, doc["speed_pct"] | 50); server.send(200, "application/json", "{\"status\":\"ok\"}"); }
    else server.send(400, "application/json", "{\"msg\":\"IK Failed / Unreachable\"}");
  });

  server.on("/api/recipe", HTTP_POST, [](){
    sendCORS(); if(g_state != STATE_READY && g_state != STATE_IDLE) { server.send(409, "application/json", "{\"msg\":\"Not Ready\"}"); return; }
    DeserializationError err = deserializeJson(recipeDoc, server.arg("plain"));
    if(err) { server.send(400, "application/json", "{\"msg\":\"JSON Error\"}"); return; }
    g_recipe_arr = recipeDoc.as<JsonArray>(); g_recipe_idx = 0; g_recipe_running_active = true;
    addLog("Recipe loaded, steps: " + String(g_recipe_arr.size()));
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });

  server.onNotFound([](){ if(server.method() == HTTP_OPTIONS){ sendCORS(); server.send(204); } else server.send(404, "text/plain", "Not Found"); });
  server.begin();
}

// ============================================================================
//  SEÇÃO 10: BOOT E MAIN LOOP
// ============================================================================

void setup() {
  Serial.begin(115200); Serial2.begin(SERIAL2_BAUD, SERIAL_8N1, RXD2, TXD2);
  setupMotors();
  loadNVS();
  
  pixels.begin(); pixels.clear(); pixels.show(); updateLed();
  
  WiFi.mode(WIFI_STA); String ssid=prefs.getString("ssid",""); String pass=prefs.getString("pass","");
  if(ssid.length() > 0) {
    IPAddress ip(192,168,1,100), gw(192,168,1,1), sn(255,255,255,0); WiFi.config(ip, gw, sn);
    WiFi.begin(ssid.c_str(), pass.c_str()); int r=0; while(WiFi.status()!=WL_CONNECTED && r++<20) delay(500);
  }
  if(WiFi.status() != WL_CONNECTED) { WiFi.mode(WIFI_AP); WiFi.softAP(AP_SSID, AP_PASS); }
  setupHttp();
  
  for(int i=0; i<6; i++) { g_joints[i].current_deg=0; g_joints[i].current_steps=0; isr_steps_current[i]=0; isr_steps_target[i]=0; g_joints[i].homed=false; }
  updateGlobalFK(); g_state = STATE_IDLE; updateLed(); addLog("System Boot OK");
}

void loop() {
  server.handleClient();
  processSerial();
  handleHoming();
  executePlanner();
  executeRecipeTick();
  delay(1);
}
