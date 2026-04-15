/*
 * ============================================================================
 *  EB-15 Robotic Arm — Simulador de Encoder AS5600 + Leitor
 * ============================================================================
 *  FUNCIONALIDADES:
 *  - Gera um PWM de 115Hz no PINO 19 simulando perfeitamente um AS5600.
 *  - Lê esse PWM no PINO 18 usando pulseIn() no Core 1.
 *  - Painel Web (Core 0) permite alterar o ângulo simulado via Slider (0 a 360) 
 *    e conferir a leitura devolvida pelo cabo.
 * 
 *  PINAGEM OBRIGATÓRIA FÍSICA NO ESP32 (Loopback):
 *  - Conecte um jumper ligando o D19 ao D18!
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
#define ENC_PWM_IN_PIN  18  // Pino que LE o encoder
#define ENC_PWM_OUT_PIN 19  // Pino que GERA o sinal fake de encoder

#define PWM_CHANNEL 0
#define PWM_FREQ 115
#define PWM_RES 12  // 12-bit resolution: 0 to 4095

const char* WIFI_SSID = "OLIVEIRA_2G";
const char* WIFI_PASS = "360@@FG1";

IPAddress local_IP(192, 168, 0, 26);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

// ============================================================================
//  VARIÁVEIS DE TELEMETRIA
// ============================================================================
volatile unsigned long g_raw_pwm_high = 0;
volatile unsigned long g_raw_pwm_low = 0;
volatile float g_duty_cycle = 0.0;
volatile float g_absolute_angle = 0.0;
volatile float g_offset_angle = 0.0;
volatile float g_final_angle = 0.0;

volatile float g_simul_angle = 180.0; // Angulo que estamos gerando artificialmente

WebServer server(80);
Preferences prefs;
TaskHandle_t TaskWebHandle;

// ============================================================================
//  CABEÇALHOS DO FREERTOS & FUNCOES
// ============================================================================
void TaskWeb(void *pvParameters);
void handleStatus();
void handleCalibrate();
void handleSimul();

// ============================================================================
//  INTERFACE WEB (HTML + CSS + JS)
// ============================================================================
// Adicionei CSS responsivo p/ o Slider
const char INDEX_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>EB-15 PWM Simulator</title>
    <style>
        :root { 
            --bg: #0b0e14; 
            --card: #151921; 
            --accent: #3b82f6; 
            --text: #e2e8f0; 
            --border: #2d3748; 
            --success: #10b981; 
            --warning: #f59e0b;
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
        h1 { font-size: 1.4rem; font-weight: 800; color: white; margin-bottom: 5px; text-align: center; }
        .subtitle { font-size: 0.85rem; color: var(--warning); text-align: center; margin-bottom: 30px; font-weight:bold; }
        
        .section-title { font-size: 0.75rem; font-weight: 700; color: var(--accent); text-transform: uppercase; letter-spacing: 0.05em; margin-bottom: 15px; border-bottom: 1px solid var(--border); padding-bottom: 8px; }
        
        .telemetry { display: grid; grid-template-columns: 1fr 1fr; gap: 15px; margin-bottom:15px; }
        .telemetry.single { grid-template-columns: 1fr; }
        .t-box { background: #0b0e14; padding: 15px; border-radius: 15px; border: 1px solid var(--border); text-align: center; }
        .t-label { font-size: 0.7rem; color: #64748b; margin-bottom: 5px; }
        .t-val { font-size: 1.5rem; font-weight: 800; color: var(--accent); }
        .t-unit { font-size: 0.65rem; color: #64748b; margin-left: 2px; }

        .slider-container { margin-top: 10px; margin-bottom: 20px; text-align: center;}
        input[type=range] { width: 100%; margin: 15px 0; accent-color: var(--accent); }

        .btn-calibrate { 
            background: var(--success); color: white; border: none; padding: 15px;
            width: 100%; border-radius: 12px; font-weight: 800; font-size: 1rem;
            cursor: pointer; transition: 0.3s;
        }
        .btn-calibrate:active { transform: scale(0.97); }

        .raw-data {
            font-size: 0.7rem; color: #64748b; display: flex; justify-content: space-between;
            background: #0b0e14; padding: 10px; border-radius: 8px; margin-top: 15px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Simulador Hardware-in-the-Loop</h1>
        <p class="subtitle">⚠️ CONECTE UM FIO ENTRE O PINO D19 E O PINO D18!</p>

        <div class="card" style="border-color: var(--accent);">
            <div class="section-title">Gerador de Sinal (D19 Simula AS5600)</div>
            <div class="slider-container">
                <div class="t-label">Ângulo Físico Injetado no Cabo: <strong id="lbl-simul" style="color:white; font-size:1.1rem;">180.0</strong>°</div>
                <input type="range" id="simul-slider" min="0" max="360" step="0.5" value="180" 
                       oninput="$('lbl-simul').innerText=this.value; setSimul(this.value);">
            </div>
        </div>
        
        <div class="card">
            <div class="section-title">Leitura Realizada (D18 Lê o Sinal)</div>
            <div class="telemetry single">
                <div class="t-box" style="border-color: var(--success);">
                    <div class="t-label">Ângulo Relativo (Offset Aplicado)</div>
                    <div class="t-val" id="disp-final-angle" style="color:var(--success);">0.00</div>
                    <span class="t-unit">graus</span>
                </div>
            </div>
            <div class="telemetry">
                <div class="t-box">
                    <div class="t-label">PWM LIdo</div>
                    <div class="t-val" id="disp-duty" style="color:white; font-size:1.2rem;">0.0</div>
                    <span class="t-unit">%</span>
                </div>
                <div class="t-box">
                    <div class="t-label">Ângulo Bruto Calculado</div>
                    <div class="t-val" id="disp-abs-angle" style="color:white; font-size:1.2rem;">0.00</div>
                    <span class="t-unit">graus</span>
                </div>
            </div>
            
            <button class="btn-calibrate" onclick="calibrate()">Zerar Encoder (Salvar Offset)</button>

            <div class="raw-data">
                <span>Offset Gravado: <span id="disp-offset" style="color:white">0.00</span>°</span>
                <span>High: <span id="disp-high">0</span>µs | Low: <span id="disp-low">0</span>µs</span>
            </div>
        </div>
    </div>

    <script>
        const $=(id)=>document.getElementById(id);

        async function setSimul(val) {
            await fetch('/api/simul', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({angle: parseFloat(val)})
            });
        }

        async function calibrate() {
            try {
                const res = await fetch('/api/calibrate', { method: 'POST' });
                if(res.ok) alert('Encoder Zerado! Offset gravado na memoria (NVS).');
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

        setInterval(updateTelemetry, 150);
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
    g_offset_angle = g_absolute_angle;
    prefs.begin("enc_sim", false);
    prefs.putFloat("offset", g_offset_angle);
    prefs.end();
    server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleSimul() {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, server.arg("plain"));
    g_simul_angle = doc["angle"] | 180.0;
    
    // Matemática Inversa para gerar sinal fiel ao datasheet do AS5600:
    // O range do duty cycle é de 128 (0 graus) a 4096 (360 graus) em um perídodo de 4119.
    // Como a resolucao do ledcWrite é 12-bit (0 a 4095), 4119 vira escala cheia.
    // 128 / 4119 * 4095 = 127 (Min Duty de 0 graus)
    // 4096 / 4119 * 4095 = 4072 (Max Duty de 360 graus)
    
    int pwm_val = 127 + (int)((g_simul_angle / 360.0) * (4072 - 127));
    ledcWrite(PWM_CHANNEL, pwm_val);
    
    server.send(200, "application/json", "{\"status\":\"ok\"}");
}


// ============================================================================
//  TASK WEBSERVER (Core 0)
// ============================================================================
void TaskWeb(void *pvParameters) {
    for(;;) {
        server.handleClient();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


// ============================================================================
//  SETUP & MAIN LOOP (Core 1)
// ============================================================================
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // NVS: Recuperar
    prefs.begin("enc_sim", true); 
    g_offset_angle = prefs.getFloat("offset", 0.0);
    prefs.end();

    pinMode(ENC_PWM_IN_PIN, INPUT);

    // INICIALIZA GERADOR PWM ESP32
    #if ESP_ARDUINO_VERSION_MAJOR >= 3
      // Core version 3.x
      ledcAttach(ENC_PWM_OUT_PIN, PWM_FREQ, PWM_RES);
      ledcWrite(ENC_PWM_OUT_PIN, 2100); // ~= 180 graus (duty 50%)
    #else
      // Core version 1.x / 2.x
      ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RES);
      ledcAttachPin(ENC_PWM_OUT_PIN, PWM_CHANNEL);
      ledcWrite(PWM_CHANNEL, 2100); // ~= 180 graus
    #endif

    // Conexao WiFi
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.println("STA Failed to configure");
    }
    
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Conectando WiFi " + String(WIFI_SSID));
    while (WiFi.status() != WL_CONNECTED) {
        delay(500); Serial.print(".");
    }
    Serial.println("\nWiFi Conectado! Acesse: http://192.168.0.26");

    server.on("/", HTTP_GET, [](){ server.send_P(200, "text/html", INDEX_HTML); });
    server.on("/api/status", HTTP_GET, handleStatus);
    server.on("/api/calibrate", HTTP_POST, handleCalibrate);
    server.on("/api/simul", HTTP_POST, handleSimul);
    server.begin();

    xTaskCreatePinnedToCore(TaskWeb, "TaskWeb", 8192, NULL, 1, &TaskWebHandle, 0);
}


void loop() {
    // Leitura bloqueante protegida no CORE 1
    unsigned long highT = pulseIn(ENC_PWM_IN_PIN, HIGH, 20000); 
    unsigned long lowT = pulseIn(ENC_PWM_IN_PIN, LOW, 20000);

    if (highT > 0 && lowT > 0) {
        unsigned long period = highT + lowT;
        float dc = (float)highT / (float)period;
        
        float minDuty = 128.0 / 4119.0;
        float maxDuty = 4096.0 / 4119.0;
        
        if (dc < minDuty) dc = minDuty;
        if (dc > maxDuty) dc = maxDuty;
        
        float abs_angle = ((dc - minDuty) / (maxDuty - minDuty)) * 360.0;
        
        g_raw_pwm_high = highT;
        g_raw_pwm_low = lowT;
        g_duty_cycle = dc;
        g_absolute_angle = abs_angle;
        
        float diff = abs_angle - g_offset_angle;
        if (diff < 0.0) diff += 360.0;
        g_final_angle = diff;
    }
    
    delay(5);
}
