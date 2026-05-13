/*
 * ============================================================================
 *  EB-15 Robotic Arm — Individual Motor Test (Pin 4)
 * ============================================================================
 *  FUNCIONALIDADES:
 *  - Somente modo Station (SSID: OLIVEIRA_2G, IP: 192.168.0.8).
 *  - Controle comutável de 3 juntas (J1, J2, J3).
 *  - Arquitetura FreeRTOS: Motor no Core 1, Servidor Web/Wi-Fi no Core 0.
 *  - 3 Modos de Operação:
 *    1. Giro Contínuo (Velocidade fixa).
 *    2. Giro Alternado (Move 6s e para).
 *    3. Giro Back-and-Forth (Move 6s frente, pausa, 6s trás, pausa, repete).
 * ============================================================================
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <AccelStepper.h>

// ============================================================================
//  CONFIGURAÇÕES DE HARDWARE
// ============================================================================
#define PUL_J1 4
#define DIR_J1 5
#define PUL_J2 6
#define DIR_J2 7
#define PUL_J3 8
#define DIR_J3 3

// ============================================================================
//  PARAMETROS INICIAIS E FREERTOS
// ============================================================================
const char* WIFI_SSID = "OLIVEIRA_2G";
const char* WIFI_PASS = "360@@FG1";

IPAddress local_IP(192, 168, 0, 26);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

// Calibração: Passos por Grau (padrão EB-15)
const float STEPS_PER_DEGREE = 71.1;

// Variáveis voláteis compartilhadas (Escritas pelo Core 1, Lidas pelo Core 0 para telemetria)
volatile float g_speed = 1000.0;    // Passos por segundo
volatile float g_accel = 500.0;     // Passos por segundo^2
volatile int g_mode = 0;            // 0:Idle, 1:Continuous, 2:Single6s, 3:Loop6s
volatile int g_active_motor = 1;    // 1:J1, 2:J2, 3:J3
volatile bool g_running = false;

// Controle de Sequência (Modo 3) - Usado apenas na Task motor
int g_seq_step = 0;        // 0:Fwd, 1:Pause, 2:Bwd, 3:Pause
unsigned long g_timer = 0;

AccelStepper stepper1(AccelStepper::DRIVER, PUL_J1, DIR_J1);
AccelStepper stepper2(AccelStepper::DRIVER, PUL_J2, DIR_J2);
AccelStepper stepper3(AccelStepper::DRIVER, PUL_J3, DIR_J3);
AccelStepper* activeStepper = &stepper1;

WebServer server(80);

struct MotorCommand {
    int motor;
    float speed;
    float accel;
    int mode;       // 0: Stop, 1: Cont, 2: 6s, 3: B&F, 4: Jog
    long steps;     // Para comandos relativos
};

QueueHandle_t cmdQueue;
TaskHandle_t TaskWebHandle;

// ============================================================================
//  INTERFACE WEB
// ============================================================================
const char INDEX_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>EB-15 Motor Test</title>
    <style>
        :root { 
            --bg: #0b0e14; 
            --card: #151921; 
            --accent: #22d3ee; 
            --text: #e2e8f0; 
            --danger: #f43f5e; 
            --border: #2d3748; 
            --success: #10b981; 
            --warning: #f59e0b;
        }
        * { box-sizing: border-box; margin: 0; padding: 0; font-family: 'Inter', sans-serif; }
        body { 
            background: var(--bg); color: var(--text); min-height: 100vh;
            display: flex; flex-direction: column; align-items: center; padding: 20px;
        }
        .container { width: 100%; max-width: 1000px; display: flex; gap: 20px; align-items: stretch; justify-content: center; }
        .column { flex: 1; display: flex; flex-direction: column; gap: 15px; min-width: 320px; }
        .card { 
            background: var(--card); border-radius: 16px; padding: 20px; 
            border: 1px solid var(--border); box-shadow: 0 4px 15px rgba(0,0,0,0.3);
            backdrop-filter: blur(10px);
        }
        h1 { font-size: 1.25rem; font-weight: 800; color: white; margin-bottom: 5px; text-align: center; }
        .subtitle { font-size: 0.75rem; color: #94a3b8; text-align: center; margin-bottom: 15px; }
        
        .section-title { font-size: 0.7rem; font-weight: 700; color: var(--accent); text-transform: uppercase; letter-spacing: 0.05em; margin-bottom: 12px; border-bottom: 1px solid var(--border); padding-bottom: 5px; }
        
        .control-group { margin-bottom: 12px; }
        label { display: block; font-size: 0.75rem; color: #94a3b8; margin-bottom: 5px; }
        input { background: #0b0e14; border: 1px solid var(--border); color: white; padding: 10px; border-radius: 8px; width: 100%; outline: none; font-size: 0.9rem; transition: 0.2s; }
        input:focus { border-color: var(--accent); box-shadow: 0 0 0 2px rgba(34, 211, 238, 0.2); }
        
        .modes { display: grid; grid-template-columns: repeat(3, 1fr); gap: 8px; margin-bottom: 15px; }
        .mode-btn { background: #0b0e14; border: 1px solid var(--border); color: #94a3b8; padding: 10px 4px; border-radius: 8px; cursor: pointer; font-size: 0.7rem; font-weight: 600; text-align: center; transition: 0.2s; }
        .mode-btn.active { background: var(--accent); border-color: var(--accent); color: #0b0e14; }
        
        .main-actions { display: grid; grid-template-columns: 2fr 1fr; gap: 10px; margin-top: 5px; }
        button.action { padding: 12px; border-radius: 10px; border: none; font-weight: 800; cursor: pointer; transition: 0.3s; font-size: 0.85rem; }
        #btn-start { background: var(--success); color: white; }
        #btn-stop { background: var(--danger); color: white; }
        button:active { transform: scale(0.96); }
        
        .telemetry { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; }
        .t-box { background: #0b0e14; padding: 10px; border-radius: 12px; border: 1px solid var(--border); text-align: center; }
        .t-label { font-size: 0.65rem; color: #64748b; margin-bottom: 2px; }
        .t-val { font-size: 1.1rem; font-weight: 800; color: var(--accent); }
        .t-unit { font-size: 0.6rem; color: #64748b; margin-left: 1px; }
        
        #status-indicator { margin-top: 15px; text-align: center; font-size: 0.75rem; font-weight: 600; padding: 8px; border-radius: 8px; }
        .st-idle { color: #94a3b8; background: rgba(148, 163, 184, 0.1); }
        .st-running { color: var(--success); background: rgba(16, 185, 129, 0.1); }

        /* JOG Styles */
        .jog-grid { display: grid; grid-template-columns: 1fr; gap: 10px; }
        .jog-row { display: grid; grid-template-columns: 40px 1fr 1fr; gap: 10px; align-items: center; background: rgba(0,0,0,0.2); padding: 8px; border-radius: 10px; border: 1px solid var(--border); }
        .jog-label { font-size: 0.8rem; font-weight: 700; color: var(--accent); }
        .jog-btn { background: #1e293b; border: 1px solid var(--border); color: white; padding: 12px; border-radius: 8px; cursor: pointer; font-weight: 900; font-size: 1.1rem; transition: 0.2s; }
        .jog-btn:hover { background: var(--accent); color: var(--bg); }
        .jog-btn.neg { border-left: 4px solid var(--danger); }
        .jog-btn.pos { border-right: 4px solid var(--success); }
    </style>
</head>
<body>
    <h1 id="title-h1">Motor J1 Explorer</h1>
    <p class="subtitle">EB-15 Testing Suite — Multi-Motor</p>
    
    <div class="container">
        <!-- Coluna Esquerda: Params e Status -->
        <div class="column">
            <div class="card">
                <div class="section-title">Status & Telemetria</div>
                <div class="telemetry">
                    <div class="t-box"><div class="t-label">Velocidade</div><div class="t-val" id="disp-speed">0</div><span class="t-unit">p/s</span></div>
                    <div class="t-box"><div class="t-label">Aceleração</div><div class="t-val" id="disp-accel">0</div><span class="t-unit">p/s²</span></div>
                </div>
                <div id="status-indicator" class="st-idle">IDLE</div>
            </div>

            <div class="card">
                <div class="section-title">Configurações Base</div>
                <div class="control-group"><label>Velocidade Máxima (p/s)</label><input type="number" id="in-speed" value="1000"></div>
                <div class="control-group"><label>Aceleração (p/s²)</label><input type="number" id="in-accel" value="500"></div>
                
                <label>Motor Principal (Modos Auto)</label>
                <div class="modes" style="margin-bottom: 10px;">
                    <div class="mode-btn active" onclick="setMotor(1)" id="mot1">J1</div>
                    <div class="mode-btn" onclick="setMotor(2)" id="mot2">J2</div>
                    <div class="mode-btn" onclick="setMotor(3)" id="mot3">J3</div>
                </div>

                <label>Modo Automático</label>
                <div class="modes">
                    <div class="mode-btn active" onclick="setMode(1)" id="m1">Contínuo</div>
                    <div class="mode-btn" onclick="setMode(2)" id="m2">6 Segundos</div>
                    <div class="mode-btn" onclick="setMode(3)" id="m3">Vai e Vem</div>
                </div>

                <div class="main-actions">
                    <button class="action" id="btn-start" onclick="sendCommand('start')">START AUTO</button>
                    <button class="action" id="btn-stop" onclick="sendCommand('stop')">E-STOP</button>
                </div>
            </div>
        </div>

        <!-- Coluna Direita: Jog Control -->
        <div class="column">
            <div class="card" style="flex: 1;">
                <div class="section-title">Controle Manual (Jog)</div>
                <div class="control-group" style="margin-bottom: 20px;">
                    <label>Tamanho do Passo (Graus º)</label>
                    <input type="number" id="in-jog-deg" value="5" step="0.5">
                </div>

                <div class="jog-grid">
                    <div class="jog-row">
                        <div class="jog-label">J1</div>
                        <button class="jog-btn neg" onclick="sendJog(1, -1)">-</button>
                        <button class="jog-btn pos" onclick="sendJog(1, 1)">+</button>
                    </div>
                    <div class="jog-row">
                        <div class="jog-label">J2</div>
                        <button class="jog-btn neg" onclick="sendJog(2, -1)">-</button>
                        <button class="jog-btn pos" onclick="sendJog(2, 1)">+</button>
                    </div>
                    <div class="jog-row">
                        <div class="jog-label">J3</div>
                        <button class="jog-btn neg" onclick="sendJog(3, -1)">-</button>
                        <button class="jog-btn pos" onclick="sendJog(3, 1)">+</button>
                    </div>
                </div>
                
                <p style="font-size: 0.65rem; color: #64748b; margin-top: 20px; text-align: center;">
                    * O Jog utiliza a velocidade e aceleração definidas à esquerda.
                </p>
            </div>
        </div>
    </div>

    <script>
        let currentMode = 1;
        let currentMotor = 1;
        const $=(id)=>document.getElementById(id);

        function setMode(m) {
            currentMode = m;
            document.querySelectorAll('#m1, #m2, #m3').forEach(b => b.classList.remove('active'));
            $('m'+m).classList.add('active');
        }

        function setMotor(m) {
            currentMotor = m;
            document.querySelectorAll('#mot1, #mot2, #mot3').forEach(b => b.classList.remove('active'));
            $('mot'+m).classList.add('active');
            sendCommand('set_motor');
        }

        async function sendCommand(cmd, extra = {}) {
            const body = {
                command: cmd,
                speed: parseFloat($('in-speed').value),
                accel: parseFloat($('in-accel').value),
                mode: currentMode,
                motor: currentMotor,
                ...extra
            };
            await fetch('/api/command', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify(body)
            });
        }

        function sendJog(motor, dir) {
            const deg = parseFloat($('in-jog-deg').value) * dir;
            sendCommand('jog', { motor: motor, deg: deg });
        }

        async function updateTelemetry() {
            try {
                const res = await fetch('/api/status');
                const data = await res.json();
                $('disp-speed').innerText = data.speed;
                $('disp-accel').innerText = data.accel;
                const st = $('status-indicator');
                st.innerText = data.running ? 'MOVIMENTANDO' : 'PRONTO';
                st.className = data.running ? 'st-running' : 'st-idle';
                if(data.motor && data.motor !== currentMotor) {
                    currentMotor = data.motor;
                    document.querySelectorAll('#mot1, #mot2, #mot3').forEach(b => b.classList.remove('active'));
                    $('mot'+currentMotor).classList.add('active');
                    $('title-h1').innerText = 'Motor J' + currentMotor + ' Explorer';
                }
            } catch(e) {}
        }
        setInterval(updateTelemetry, 1000);
    </script>
</body>
</html>
)=====";

// ============================================================================
//  API HANDLERS (Rodando no Core 0 via TaskWeb)
// ============================================================================
void handleStatus() {
    StaticJsonDocument<200> doc;
    doc["speed"] = g_speed;
    doc["accel"] = g_accel;
    doc["mode"] = g_mode;
    doc["motor"] = g_active_motor;
    doc["running"] = g_running;
    
    String s; serializeJson(doc, s);
    server.send(200, "application/json", s);
}

void handleCommand() {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, server.arg("plain"));
    
    String cmd = doc["command"] | "";
    MotorCommand mc;
    
    // Configura comando padrao para envio por queue
    mc.motor = doc["motor"] | g_active_motor;
    mc.speed = doc["speed"] | g_speed;
    mc.accel = doc["accel"] | g_accel;

    if(cmd == "set_motor") {
        mc.mode = 0; // Para o atual
        xQueueSend(cmdQueue, &mc, portMAX_DELAY);
    }
    else if(cmd == "start") {
        mc.mode = doc["mode"] | 1;
        xQueueSend(cmdQueue, &mc, portMAX_DELAY);
    } 
    else if(cmd == "jog") {
        mc.mode = 4;
        float deg = doc["deg"] | 0.0;
        mc.steps = (long)(deg * STEPS_PER_DEGREE);
        xQueueSend(cmdQueue, &mc, portMAX_DELAY);
    }
    else if(cmd == "stop") {
        mc.mode = 0;
        xQueueSend(cmdQueue, &mc, portMAX_DELAY);
    }
    
    server.send(200, "application/json", "{\"status\":\"ok\"}");
}

// ============================================================================
//  TASKS FREERTOS
// ============================================================================
// A lógica do motor foi movida para o loop() principal
void TaskWeb(void *pvParameters) {
    for(;;) {
        server.handleClient();
        vTaskDelay(pdMS_TO_TICKS(10)); // Alivia uso de CPU do Core 0 para não estrangular Wi-Fi, mas mantém resposta rápida
    }
}

// ============================================================================
//  MAIN (Setup)
// ============================================================================
void setup() {
    Serial.begin(115200);
    
    pinMode(PUL_J1, OUTPUT); pinMode(DIR_J1, OUTPUT);
    pinMode(PUL_J2, OUTPUT); pinMode(DIR_J2, OUTPUT);
    pinMode(PUL_J3, OUTPUT); pinMode(DIR_J3, OUTPUT);

    // Mínima largura do pulso incrementada para 10us para maior compatibilidade c/ drivers
    stepper1.setMinPulseWidth(10);
    stepper2.setMinPulseWidth(10);
    stepper3.setMinPulseWidth(10);

    // WiFi Station
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); // Desativa power save para reduzir latência (ping) e evitar timeout no WebServer
    
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("STA Failed to configure");
    }
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Conectando ao WiFi " + String(WIFI_SSID));
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Conectado!");
    Serial.print("IP: "); Serial.println(WiFi.localIP());

    // API Routes
    server.on("/", HTTP_GET, [](){ server.send_P(200, "text/html", INDEX_HTML); });
    server.on("/api/status", HTTP_GET, handleStatus);
    server.on("/api/command", HTTP_POST, handleCommand);
    server.begin();

    Serial.println("Servidor HTTP iniciado.");

    // Filas e Tasks
    cmdQueue = xQueueCreate(10, sizeof(MotorCommand));

    xTaskCreatePinnedToCore(
        TaskWeb,
        "TaskWeb",
        8192,
        NULL,
        1,               // Prioridade Baixa
        &TaskWebHandle,
        0                // Fixado no Core 0 (junto das routines de Wi-Fi nativas da Espressif)
    );
}

void loop() {
    MotorCommand mc;
    
    // Verifica se chegou comando novo na Queue (não-bloqueante Timeout 0)
    if(xQueueReceive(cmdQueue, &mc, 0) == pdTRUE) {
        // É um comando de parada ou troca de motor
        if(mc.mode == 0) {
            if (g_running) activeStepper->stop();
            g_running = false;
            g_mode = 0;
            
            if (mc.motor != g_active_motor) {
                g_active_motor = mc.motor;
                if (mc.motor == 1) activeStepper = &stepper1;
                else if (mc.motor == 2) activeStepper = &stepper2;
                else if (mc.motor == 3) activeStepper = &stepper3;
                Serial.printf("Motor selecionado: J%d\n", g_active_motor);
            } else {
                Serial.printf("STOP J%d (Parada acionada via API)\n", g_active_motor);
            }
        } 
        // É um comando de START
        else {
            if (g_active_motor != mc.motor) {
                if (g_running) activeStepper->stop();
                g_active_motor = mc.motor;
                if (mc.motor == 1) activeStepper = &stepper1;
                else if (mc.motor == 2) activeStepper = &stepper2;
                else if (mc.motor == 3) activeStepper = &stepper3;
            }

            g_speed = mc.speed;
            g_accel = mc.accel;
            g_mode = mc.mode;
            
            activeStepper->setMaxSpeed(g_speed);
            activeStepper->setAcceleration(g_accel);
            
            if(g_mode == 1) {
                activeStepper->setSpeed(g_speed);
            } else if(g_mode == 2) {
                long steps = (long)(g_speed * 6); // Move por 6 segundos
                activeStepper->move(steps);
            } else if(g_mode == 3) {
                g_seq_step = 0; // Inicia seq (frente)
                g_timer = millis();
                long steps = (long)(g_speed * 6);
                activeStepper->move(steps);
            } else if(g_mode == 4) {
                activeStepper->move(mc.steps);
            }
            g_running = true;
            Serial.printf("START J%d | Vel: %.1f | Acel: %.1f | Modo: %d\n", g_active_motor, g_speed, g_accel, g_mode);
        }
    }

    // Lógica de Movimento Constante
    if(g_running) {
        if(g_mode == 1) {
            activeStepper->runSpeed();
        } 
        else if(g_mode == 2 || g_mode == 4) {
            if(activeStepper->distanceToGo() != 0) {
                activeStepper->run();
            } else {
                g_running = false;
                g_mode = 0; // Retorna idle se completou
            }
        } 
        else if(g_mode == 3) {
            if(g_seq_step == 0) { // Indo para frente
                if(activeStepper->distanceToGo() != 0) {
                    activeStepper->run();
                } else {
                    g_seq_step = 1; // Pausa
                    g_timer = millis();
                }
            } 
            else if(g_seq_step == 1) { // Pausa pos-frente
                if(millis() - g_timer > 1000) { // 1s pausa
                    long steps = (long)(g_speed * 6);
                    activeStepper->move(-steps);
                    g_seq_step = 2; // Voltando
                }
            }
            else if(g_seq_step == 2) { // Voltando
                if(activeStepper->distanceToGo() != 0) {
                    activeStepper->run();
                } else {
                    g_seq_step = 3; // Pausa
                    g_timer = millis();
                }
            }
            else if(g_seq_step == 3) { // Pausa pos-volta
                if(millis() - g_timer > 1000) {
                    long steps = (long)(g_speed * 6);
                    activeStepper->move(steps);
                    g_seq_step = 0; // Reinicia loop
                }
            }
        }
        // Quando está rodando o motor em alta velocidade, não chamamos delay() 
        // A função yield() do Arduino Core cuidará do WDT internamente se o loop durar muito
    } else {
        // Se o motor não está rodando, aliviamos a CPU substancialmente
        delay(10);
    }
}
