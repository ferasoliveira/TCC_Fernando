/*
 * ============================================================================
 *  EB-15 Robotic Arm — Base Test (J1, J2, J3)
 * ============================================================================
 *  FUNCIONALIDADES:
 *  - Controle por ângulo (graus) para as 3 primeiras juntas.
 *  - Interface Web responsiva com feedback em tempo real.
 *  - Arquitetura FreeRTOS: Servidor Web no Core 0, Motores no Core 1.
 *  - Baseado no pinout do Teste Motor Individual.
 * ============================================================================
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <AccelStepper.h>

// ============================================================================
//  CONFIGURAÇÕES DE HARDWARE E REDE
// ============================================================================
#define PUL_J1 4
#define DIR_J1 15
#define PUL_J2 5
#define DIR_J2 16
#define PUL_J3 6
#define DIR_J3 17

const char* WIFI_SSID = "OLIVEIRA_2G";
const char* WIFI_PASS = "360@@FG1";

IPAddress local_IP(192, 168, 0, 26);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

// Constantes de Conversão
const float STEPS_PER_DEG = 71.1; // Baseado no supervisor firmware (Redução 8:1 + Microstepping 1/16)

// ============================================================================
//  VARIÁVEIS GLOBAIS
// ============================================================================
AccelStepper stepper1(AccelStepper::DRIVER, PUL_J1, DIR_J1);
AccelStepper stepper2(AccelStepper::DRIVER, PUL_J2, DIR_J2);
AccelStepper stepper3(AccelStepper::DRIVER, PUL_J3, DIR_J3);

WebServer server(80);

// Status compartilhado
volatile float g_target_angles[3] = {0, 0, 0};
volatile float g_current_angles[3] = {0, 0, 0};
volatile bool g_is_moving = false;

// Parâmetros de movimento (passos/s e passos/s²)
volatile float g_max_speed = 2000.0;
volatile float g_min_speed = 100.0;   // Velocidade inicial do perfil trapezoidal
volatile float g_accel     = 500.0;

// ============================================================================
//  INTERFACE WEB (PROGMEM)
// ============================================================================
const char INDEX_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>EB-15 Base Control</title>
    <style>
        :root { 
            --bg: #0f172a; 
            --card: #1e293b; 
            --accent: #38bdf8; 
            --text: #f8fafc; 
            --border: #334155;
            --success: #10b981;
            --danger: #ef4444;
        }
        * { box-sizing: border-box; margin: 0; padding: 0; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; }
        body { background: var(--bg); color: var(--text); display: flex; flex-direction: column; align-items: center; padding: 2rem; min-height: 100vh; }
        .container { width: 100%; max-width: 600px; }
        .header { text-align: center; margin-bottom: 2rem; }
        .header h1 { font-size: 2rem; color: var(--accent); }
        .header p { color: #94a3b8; font-size: 0.9rem; }

        .card { background: var(--card); border-radius: 1rem; border: 1px solid var(--border); padding: 1.5rem; margin-bottom: 1.5rem; box-shadow: 0 4px 6px -1px rgba(0,0,0,0.1); }
        .section-title { font-size: 0.8rem; font-weight: bold; text-transform: uppercase; color: var(--accent); margin-bottom: 1rem; letter-spacing: 1px; }

        .joint-row { display: grid; grid-template-columns: 80px 1fr 100px; gap: 1rem; align-items: center; margin-bottom: 1rem; }
        .joint-label { font-weight: 600; color: #cbd5e1; }
        input[type="number"] { background: #0f172a; border: 1px solid var(--border); color: white; padding: 0.5rem; border-radius: 0.5rem; width: 100%; text-align: center; }
        input[type="range"] { accent-color: var(--accent); }

        .actions { display: grid; grid-template-columns: 1fr 1fr; gap: 1rem; margin-top: 1rem; }
        button { border: none; padding: 0.8rem; border-radius: 0.5rem; font-weight: bold; cursor: pointer; transition: 0.2s; }
        button:active { transform: scale(0.98); }
        #btn-go { background: var(--success); color: white; }
        #btn-stop { background: var(--danger); color: white; }

        .telemetry { display: grid; grid-template-columns: repeat(3, 1fr); gap: 1rem; margin-bottom: 1.5rem; }
        .t-box { background: #0f172a; border: 1px solid var(--border); padding: 1rem; border-radius: 0.75rem; text-align: center; }
        .t-label { font-size: 0.7rem; color: #94a3b8; margin-bottom: 0.25rem; }
        .t-val { font-size: 1.2rem; font-weight: bold; color: var(--accent); }

        .param-row { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 1rem; margin-bottom: 1rem; }
        .param-group label { display: block; font-size: 0.75rem; color: #94a3b8; margin-bottom: 0.3rem; }
        .param-group input { width: 100%; }
        .btn-apply { background: var(--accent); color: #0f172a; width: 100%; margin-top: 0.5rem; }

        .status-badge { text-align: center; padding: 0.5rem; border-radius: 2rem; font-size: 0.8rem; font-weight: bold; margin-bottom: 1.5rem; }
        .st-idle { background: rgba(56, 189, 248, 0.1); color: var(--accent); }
        .st-moving { background: rgba(16, 185, 129, 0.1); color: var(--success); }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>EB-15 Base Control</h1>
            <p>Juntas J1, J2 e J3 — Teste de Posição</p>
        </div>

        <div id="status-badge" class="status-badge st-idle">ESTADO: PRONTO</div>

        <div class="telemetry">
            <div class="t-box">
                <div class="t-label">J1 (Graus)</div>
                <div class="t-val" id="cur-j1">0.0°</div>
            </div>
            <div class="t-box">
                <div class="t-label">J2 (Graus)</div>
                <div class="t-val" id="cur-j2">0.0°</div>
            </div>
            <div class="t-box">
                <div class="t-label">J3 (Graus)</div>
                <div class="t-val" id="cur-j3">0.0°</div>
            </div>
        </div>

        <div class="card">
            <div class="section-title">Parâmetros de Movimento</div>
            <div class="param-row">
                <div class="param-group">
                    <label>Vel. Inicial (p/s)</label>
                    <input type="number" id="in-minspd" value="100">
                </div>
                <div class="param-group">
                    <label>Vel. Máxima (p/s)</label>
                    <input type="number" id="in-maxspd" value="2000">
                </div>
                <div class="param-group">
                    <label>Aceleração (p/s²)</label>
                    <input type="number" id="in-accel" value="500">
                </div>
            </div>
            <button class="btn-apply" onclick="applyParams()">APLICAR PARÂMETROS</button>
        </div>

        <div class="card">
            <div class="section-title">Controle de Ângulo</div>
            
            <div class="joint-row">
                <span class="joint-label">J1 Base</span>
                <input type="range" id="rng-j1" min="-180" max="180" value="0" oninput="sync('j1', 'rng')">
                <input type="number" id="num-j1" value="0" oninput="sync('j1', 'num')">
            </div>

            <div class="joint-row">
                <span class="joint-label">J2 Ombro</span>
                <input type="range" id="rng-j2" min="-45" max="180" value="0" oninput="sync('j2', 'rng')">
                <input type="number" id="num-j2" value="0" oninput="sync('j2', 'num')">
            </div>

            <div class="joint-row">
                <span class="joint-label">J3 Cotovelo</span>
                <input type="range" id="rng-j3" min="-120" max="120" value="0" oninput="sync('j3', 'rng')">
                <input type="number" id="num-j3" value="0" oninput="sync('j3', 'num')">
            </div>

            <div class="actions">
                <button id="btn-go" onclick="sendTarget()">MOVER PARA POSIÇÃO</button>
                <button id="btn-stop" onclick="sendCommand('stop')">PARADA EMERGÊNCIA</button>
            </div>
        </div>
    </div>

    <script>
        const sync = (j, src) => {
            if(src === 'rng') document.getElementById('num-'+j).value = document.getElementById('rng-'+j).value;
            else document.getElementById('rng-'+j).value = document.getElementById('num-'+j).value;
        };

        const updateUI = (data) => {
            document.getElementById('cur-j1').innerText = data.pos[0].toFixed(1) + '°';
            document.getElementById('cur-j2').innerText = data.pos[1].toFixed(1) + '°';
            document.getElementById('cur-j3').innerText = data.pos[2].toFixed(1) + '°';
            
            const badge = document.getElementById('status-badge');
            badge.innerText = data.moving ? 'ESTADO: MOVIMENTANDO' : 'ESTADO: PRONTO';
            badge.className = data.moving ? 'status-badge st-moving' : 'status-badge st-idle';

            // Parâmetros NÃO são sincronizados aqui para não sobrescrever
            // o que o usuário está digitando. Os valores são enviados via APLICAR.
        };

        async function fetchStatus() {
            try {
                const res = await fetch('/api/status');
                const data = await res.json();
                updateUI(data);
            } catch(e) {}
        }

        async function sendTarget() {
            const body = {
                j1: parseFloat(document.getElementById('num-j1').value),
                j2: parseFloat(document.getElementById('num-j2').value),
                j3: parseFloat(document.getElementById('num-j3').value)
            };
            await fetch('/api/move', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify(body)
            });
        }

        async function sendCommand(cmd) {
            await fetch('/api/cmd?type=' + cmd, { method: 'POST' });
        }

        async function applyParams() {
            const body = {
                maxSpd: parseFloat(document.getElementById('in-maxspd').value),
                minSpd: parseFloat(document.getElementById('in-minspd').value),
                accel:  parseFloat(document.getElementById('in-accel').value)
            };
            await fetch('/api/params', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify(body)
            });
        }

        setInterval(fetchStatus, 500);
    </script>
</body>
</html>
)=====";

// ============================================================================
//  HANDLERS WEB
// ============================================================================
void handleStatus() {
    StaticJsonDocument<300> doc;
    JsonArray pos = doc.createNestedArray("pos");
    pos.add(g_current_angles[0]);
    pos.add(g_current_angles[1]);
    pos.add(g_current_angles[2]);
    doc["moving"] = g_is_moving;
    doc["maxSpd"] = g_max_speed;
    doc["minSpd"] = g_min_speed;
    doc["accel"]  = g_accel;
    
    String out; serializeJson(doc, out);
    server.send(200, "application/json", out);
}

void handleParams() {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, server.arg("plain"));
    
    g_max_speed = doc["maxSpd"] | g_max_speed;
    g_min_speed = doc["minSpd"] | g_min_speed;
    g_accel     = doc["accel"]  | g_accel;
    
    stepper1.setMaxSpeed(g_max_speed); stepper1.setAcceleration(g_accel);
    stepper2.setMaxSpeed(g_max_speed); stepper2.setAcceleration(g_accel);
    stepper3.setMaxSpeed(g_max_speed); stepper3.setAcceleration(g_accel);
    
    Serial.printf("Params: MaxSpd=%.1f MinSpd=%.1f Accel=%.1f\n", g_max_speed, g_min_speed, g_accel);
    server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleMove() {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, server.arg("plain"));
    
    g_target_angles[0] = doc["j1"] | g_target_angles[0];
    g_target_angles[1] = doc["j2"] | g_target_angles[1];
    g_target_angles[2] = doc["j3"] | g_target_angles[2];

    // Aplica parâmetros atuais antes do movimento
    stepper1.setMaxSpeed(g_max_speed); stepper1.setAcceleration(g_accel);
    stepper2.setMaxSpeed(g_max_speed); stepper2.setAcceleration(g_accel);
    stepper3.setMaxSpeed(g_max_speed); stepper3.setAcceleration(g_accel);
    
    stepper1.moveTo((long)(g_target_angles[0] * STEPS_PER_DEG));
    stepper2.moveTo((long)(g_target_angles[1] * STEPS_PER_DEG));
    stepper3.moveTo((long)(g_target_angles[2] * STEPS_PER_DEG));

    // Velocidade inicial (mínima) do perfil trapezoidal
    if(stepper1.distanceToGo() != 0) stepper1.setSpeed(g_min_speed * (stepper1.distanceToGo() > 0 ? 1 : -1));
    if(stepper2.distanceToGo() != 0) stepper2.setSpeed(g_min_speed * (stepper2.distanceToGo() > 0 ? 1 : -1));
    if(stepper3.distanceToGo() != 0) stepper3.setSpeed(g_min_speed * (stepper3.distanceToGo() > 0 ? 1 : -1));
    
    g_is_moving = true;
    server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleCmd() {
    String type = server.arg("type");
    if(type == "stop") {
        stepper1.stop(); stepper2.stop(); stepper3.stop();
        g_target_angles[0] = stepper1.currentPosition() / STEPS_PER_DEG;
        g_target_angles[1] = stepper2.currentPosition() / STEPS_PER_DEG;
        g_target_angles[2] = stepper3.currentPosition() / STEPS_PER_DEG;
        g_is_moving = false;
    }
    server.send(200, "application/json", "{\"status\":\"ok\"}");
}

// ============================================================================
//  TASK WEB (CORE 0)
// ============================================================================
void TaskWeb(void *pvParameters) {
    for(;;) {
        server.handleClient();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// ============================================================================
//  SETUP E LOOP (CORE 1)
// ============================================================================
void setup() {
    Serial.begin(115200);

    // Configuração dos Motores (usa valores globais)
    stepper1.setMaxSpeed(g_max_speed); stepper1.setAcceleration(g_accel);
    stepper2.setMaxSpeed(g_max_speed); stepper2.setAcceleration(g_accel);
    stepper3.setMaxSpeed(g_max_speed); stepper3.setAcceleration(g_accel);

    // WiFi
    WiFi.mode(WIFI_STA);
    if (!WiFi.config(local_IP, gateway, subnet, dns)) {
        Serial.println("STA Failed to configure");
    }
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
    Serial.println("\nWiFi Conectado! IP: " + WiFi.localIP().toString());

    // Rotas
    server.on("/", HTTP_GET, [](){ server.send_P(200, "text/html", INDEX_HTML); });
    server.on("/api/status", HTTP_GET, handleStatus);
    server.on("/api/move", HTTP_POST, handleMove);
    server.on("/api/cmd", HTTP_POST, handleCmd);
    server.on("/api/params", HTTP_POST, handleParams);
    server.begin();

    // Inicia Task Web no Core 0
    xTaskCreatePinnedToCore(TaskWeb, "TaskWeb", 8192, NULL, 1, NULL, 0);
}

void loop() {
    // Executa movimento
    stepper1.run();
    stepper2.run();
    stepper3.run();

    // Atualiza status local para telemetria
    g_current_angles[0] = stepper1.currentPosition() / STEPS_PER_DEG;
    g_current_angles[1] = stepper2.currentPosition() / STEPS_PER_DEG;
    g_current_angles[2] = stepper3.currentPosition() / STEPS_PER_DEG;

    // Verifica se parou
    if (!stepper1.isRunning() && !stepper2.isRunning() && !stepper3.isRunning()) {
        g_is_moving = false;
    } else {
        g_is_moving = true;
    }
    
    // Yield opcional se necessário pelo framework, mas AccelStepper run() já é eficiente
}
