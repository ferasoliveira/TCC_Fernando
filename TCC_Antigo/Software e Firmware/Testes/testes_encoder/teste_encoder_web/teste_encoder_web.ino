/*
 * ============================================================================
 *  EB-15 Robotic Arm — Leitura de Encoder AS5600 PWM Mode
 * ============================================================================
 *  FUNCIONALIDADES:
 *  - Servidor Web (porta 80) rodando no Core 0 usando Task.
 *  - pulseIn() bloqueante isolado no Core 1 para nao estrangular o ESP32.
 *  - Interface web escura profissional c/ calibracao e persistencia via NVS.
 *  - IP: 192.168.0.26, SSID: OLIVEIRA_2G.
 * 
 *  PINAGEM:
 *  - Pino 18 (Input) -> Conectado ao OUT do AS5600
 * ============================================================================
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <ArduinoJson.h>

// ============================================================================
//  CONFIGURAÇÕES DE HARDWARE & WIFI
// ============================================================================
#define ENC_PWM_PIN 18

const char* WIFI_SSID = "OLIVEIRA_2G";
const char* WIFI_PASS = "360@@FG1";

IPAddress local_IP(192, 168, 0, 26);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

// ============================================================================
//  VARIÁVEIS DE TELEMETRIA (Compartilhadas via Cores)
// ============================================================================
volatile unsigned long g_raw_pwm_high = 0;
volatile unsigned long g_raw_pwm_low = 0;
volatile float g_duty_cycle = 0.0;
volatile float g_absolute_angle = 0.0;
volatile float g_offset_angle = 0.0;
volatile float g_final_angle = 0.0;

WebServer server(80);
Preferences prefs;
TaskHandle_t TaskWebHandle;

// ============================================================================
//  CABEÇALHOS DO FREERTOS & FUNCOES
// ============================================================================
void TaskWeb(void *pvParameters);
void handleStatus();
void handleCalibrate();

// ============================================================================
//  INTERFACE WEB (HTML + CSS + JS)
// ============================================================================
const char INDEX_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>EB-15 Encoder Calibrator</title>
    <style>
        :root { 
            --bg: #0b0e14; 
            --card: #151921; 
            --accent: #a855f7; 
            --text: #e2e8f0; 
            --border: #2d3748; 
            --success: #10b981; 
            --danger: #ef4444;
        }
        * { box-sizing: border-box; margin: 0; padding: 0; font-family: 'Inter', sans-serif; }
        body { 
            background: var(--bg); color: var(--text); min-height: 100vh;
            display: flex; flex-direction: column; align-items: center; padding: 40px 20px;
        }
        .container { width: 100%; max-width: 500px; }
        .card { 
            background: var(--card); border-radius: 20px; padding: 30px; 
            margin-bottom: 25px; border: 1px solid var(--border); 
            box-shadow: 0 10px 25px -5px rgba(0,0,0,0.3);
            backdrop-filter: blur(10px);
        }
        h1 { font-size: 1.5rem; font-weight: 800; color: white; margin-bottom: 5px; text-align: center; }
        .subtitle { font-size: 0.85rem; color: #94a3b8; text-align: center; margin-bottom: 30px; }
        
        .section-title { font-size: 0.75rem; font-weight: 700; color: var(--accent); text-transform: uppercase; letter-spacing: 0.05em; margin-bottom: 15px; border-bottom: 1px solid var(--border); padding-bottom: 8px; }
        
        .telemetry { display: grid; grid-template-columns: 1fr 1fr; gap: 15px; margin-bottom:15px; }
        .telemetry.single { grid-template-columns: 1fr; }
        .t-box { background: #0b0e14; padding: 15px; border-radius: 15px; border: 1px solid var(--border); text-align: center; }
        .t-label { font-size: 0.7rem; color: #64748b; margin-bottom: 5px; }
        .t-val { font-size: 1.5rem; font-weight: 800; color: var(--accent); }
        .t-unit { font-size: 0.65rem; color: #64748b; margin-left: 2px; }

        .btn-calibrate { 
            background: var(--success); color: white; border: none; padding: 15px;
            width: 100%; border-radius: 12px; font-weight: 800; font-size: 1rem;
            cursor: pointer; transition: 0.3s;
        }
        .btn-calibrate:active { transform: scale(0.97); }
        .btn-calibrate:hover { background: #059669; }

        .raw-data {
            font-size: 0.7rem; color: #64748b; display: flex; justify-content: space-between;
            background: #0b0e14; padding: 10px; border-radius: 8px; margin-top: 10px;
        }
        
    </style>
</head>
<body>
    <div class="container">
        <h1>AS5600 Encoder via PWM</h1>
        <p class="subtitle">Módulo EB-15 — Isolamento de Núcleos & Persistência NVS</p>
        
        <div class="card">
            <div class="section-title">Visualização de Ângulo</div>
            <div class="telemetry single">
                <div class="t-box" style="border-color: var(--accent);">
                    <div class="t-label">Ângulo Relativo (Offset Aplicado)</div>
                    <div class="t-val" id="disp-final-angle">0.00</div>
                    <span class="t-unit">graus</span>
                </div>
            </div>
            <div class="telemetry">
                <div class="t-box">
                    <div class="t-label">PWM Duty Cycle</div>
                    <div class="t-val" id="disp-duty" style="color:white; font-size:1.2rem;">0.0</div>
                    <span class="t-unit">%</span>
                </div>
                <div class="t-box">
                    <div class="t-label">Ângulo Bruto (Físico)</div>
                    <div class="t-val" id="disp-abs-angle" style="color:white; font-size:1.2rem;">0.00</div>
                    <span class="t-unit">graus</span>
                </div>
            </div>
            
            <button class="btn-calibrate" onclick="calibrate()">Zerar Encoder (Set Offset)</button>

            <div class="raw-data">
                <span>Offset: <span id="disp-offset">0.00</span>°</span>
                <span>High: <span id="disp-high">0</span>µs | Low: <span id="disp-low">0</span>µs</span>
            </div>
        </div>
    </div>

    <script>
        const $=(id)=>document.getElementById(id);

        async function calibrate() {
            try {
                const res = await fetch('/api/calibrate', { method: 'POST' });
                if(res.ok) {
                    alert('Encoder Zerado! Offset gravado na memoria (NVS).');
                    updateTelemetry();
                }
            } catch(e) { alert('Erro de conexao.'); }
        }

        async function updateTelemetry() {
            try {
                const res = await fetch('/api/status');
                const data = await res.json();
                
                $('disp-final-angle').innerText = data.final_angle.toFixed(2);
                $('disp-abs-angle').innerText = data.absolute_angle.toFixed(2);
                $('disp-duty').innerText = (data.duty_cycle * 100).toFixed(1);
                
                $('disp-offset').innerText = data.offset_angle.toFixed(2);
                $('disp-high').innerText = data.raw_high;
                $('disp-low').innerText = data.raw_low;

            } catch(e) {}
        }

        setInterval(updateTelemetry, 100); // Polling extremamente rapido (100ms) por ser dedicado a encoder
    </script>
</body>
</html>
)=====";


// ============================================================================
//  API HANDLERS
// ============================================================================
void handleStatus() {
    StaticJsonDocument<256> doc;
    doc["raw_high"] = g_raw_pwm_high;
    doc["raw_low"] = g_raw_pwm_low;
    doc["duty_cycle"] = g_duty_cycle;
    doc["absolute_angle"] = g_absolute_angle;
    doc["offset_angle"] = g_offset_angle;
    doc["final_angle"] = g_final_angle;
    
    String s; serializeJson(doc, s);
    server.send(200, "application/json", s);
}

void handleCalibrate() {
    // Ao zerar, salvamos o atual angulo absoluto como o novo offset.
    g_offset_angle = g_absolute_angle;
    
    prefs.begin("encoder", false); // Falha em false = RW
    prefs.putFloat("offset", g_offset_angle);
    prefs.end();
    
    server.send(200, "application/json", "{\"status\":\"ok\"}");
    Serial.printf("[NVS] Calibracao feita. Novo offset: %.2f\n", g_offset_angle);
}


// ============================================================================
//  TASK WEBSERVER (Core 0)
// ============================================================================
void TaskWeb(void *pvParameters) {
    for(;;) {
        server.handleClient();
        vTaskDelay(pdMS_TO_TICKS(10)); // Impede uso de 100% da CPU do Core 0 (preserva o Wi-Fi Stack)
    }
}


// ============================================================================
//  SETUP & MAIN LOOP (Core 1)
// ============================================================================
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // NVS: Puxar o offset gravado
    prefs.begin("encoder", true); // True = ReadOnly
    g_offset_angle = prefs.getFloat("offset", 0.0);
    prefs.end();
    Serial.printf("\n[BOOT] Offset recuperado da NVS: %.2f graus\n", g_offset_angle);

    pinMode(ENC_PWM_PIN, INPUT);

    // Conexao WiFi
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("STA Failed to configure");
    }
    
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Conectando ao WiFi " + String(WIFI_SSID));
    while (WiFi.status() != WL_CONNECTED) {
        delay(500); Serial.print(".");
    }
    Serial.println("\nWiFi Conectado!");
    Serial.print("IP Web GUI: http://"); Serial.println(WiFi.localIP());

    // API Routes do Server (que ira rodar no Core 0)
    server.on("/", HTTP_GET, [](){ server.send_P(200, "text/html", INDEX_HTML); });
    server.on("/api/status", HTTP_GET, handleStatus);
    server.on("/api/calibrate", HTTP_POST, handleCalibrate);
    server.begin();

    // Cria a Task Fixada no Core 0 para tocar a API
    xTaskCreatePinnedToCore(
        TaskWeb,        
        "TaskWeb",      
        8192,         
        NULL,           
        1,              
        &TaskWebHandle, 
        0               // CORE 0
    );
    
    Serial.println("Sistema de Encoder e Servidor Assincrono inciados.");
}


void loop() {
    // =================================================================
    //  LEITURA BLOQUEANTE PROTEGIDA NO CORE 1 (Não afeta o Wi-Fi)
    // =================================================================
    // Timeout fixo de 20.000 microsegundos (20ms) max para n travar qdo cabo soltar.
    unsigned long highT = pulseIn(ENC_PWM_PIN, HIGH, 20000); 
    unsigned long lowT = pulseIn(ENC_PWM_PIN, LOW, 20000);

    // Protecao contra ruido ou cabo desligado
    if (highT > 0 && lowT > 0) {
        // Exatamente como especificado pelo AS5600 datasheet p/ duty cycle e Min/Max range:
        unsigned long period = highT + lowT;
        float dc = (float)highT / (float)period;
        
        // AS5600 specs de PWM Duty Cycle variam de 128/4119 (0 graus) a 4096/4119 (360 graus).
        // Aproximadamente = 3% a 99%
        float minDuty = 128.0 / 4119.0;
        float maxDuty = 4096.0 / 4119.0;
        
        // Clamp duty apenas em margens de seguranca caso flutue:
        if (dc < minDuty) dc = minDuty;
        if (dc > maxDuty) dc = maxDuty;
        
        float abs_angle = ((dc - minDuty) / (maxDuty - minDuty)) * 360.0;
        
        // Aplica o calculo na memoria global para o servidor consumir via JSON API
        g_raw_pwm_high = highT;
        g_raw_pwm_low = lowT;
        g_duty_cycle = dc;
        g_absolute_angle = abs_angle;
        
        // Transicao circular suave para o Final Angle (-180 a +180, ou apenas offsetado):
        float diff = abs_angle - g_offset_angle;
        if (diff < 0.0) diff += 360.0;
        g_final_angle = diff;
    }
    
    // yield() do IDLE Task do esp32 core
    delay(5);  // Delay minimo para nao alarmar o watchdog do core1 atoa
}
