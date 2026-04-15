/*
 * ============================================================================
 *  EB-15 Robotic Arm — MVP ESP32 Control (J1, J2, J3)
 * ============================================================================
 *  FUNCIONALIDADES:
 *  - Somente modo Access Point (SSID: ESP-robot-teste, Pass: roborobo).
 *  - Controle de 3 juntas (Base, Ombro, Cotovelo).
 *  - Configuração dinâmica de: Redução de Caixa, Velocidade e Aceleração.
 *  - LED Neopixel com cores por junta: J1 (Vermelho), J2 (Verde), J3 (Azul).
 * ============================================================================
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <AccelStepper.h>

// ============================================================================
//  CONFIGURAÇÕES DE HARDWARE
// ============================================================================
#define PUL_J1 4
#define DIR_J1 15
#define PUL_J2 5
#define DIR_J2 16
#define PUL_J3 6
#define DIR_J3 17

#define LED_PIN 48
#define NUM_LEDS 1

// ============================================================================
//  PARAMETROS INICIAIS
// ============================================================================
const char* AP_SSID = "ESP-robot-teste";
const char* AP_PASS = "roborobo";

float g_reduction = 3.5;      // Valor inicial (editável via UI)
float g_speed = 1000.0;       // Passos por segundo
float g_accel = 500.0;        // Passos por segundo^2
float g_steps_per_deg_motor = 4.44; // Ex: 1600 passos / 360 deg = 4.44 (sem redução)

// Motores (AccelStepper)
AccelStepper stepper1(AccelStepper::DRIVER, PUL_J1, DIR_J1);
AccelStepper stepper2(AccelStepper::DRIVER, PUL_J2, DIR_J2);
AccelStepper stepper3(AccelStepper::DRIVER, PUL_J3, DIR_J3);
AccelStepper* steppers[3] = {&stepper1, &stepper2, &stepper3};

WebServer server(80);
Adafruit_NeoPixel pixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// ============================================================================
//  INTERFACE WEB
// ============================================================================
const char INDEX_HTML[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>EB-15 MVP Advanced</title>
    <style>
        :root { --bg: #0f172a; --card: #1e293b; --accent: #38bdf8; --text: #f8fafc; --danger: #ef4444; --border: #334155; --success: #10b981; }
        * { box-sizing: border-box; margin: 0; padding: 0; font-family: 'Segoe UI', sans-serif; }
        body { background: var(--bg); color: var(--text); padding: 20px; display: flex; flex-direction: column; align-items: center; }
        .container { width: 100%; max-width: 600px; }
        .card { background: var(--card); border-radius: 12px; padding: 20px; margin-bottom: 20px; border: 1px solid var(--border); box-shadow: 0 4px 6px -1px rgba(0,0,0,0.1); }
        h1 { font-size: 1.5rem; color: var(--accent); margin-bottom: 15px; text-align: center; }
        h2 { font-size: 1rem; color: #94a3b8; margin-bottom: 10px; border-bottom: 1px solid var(--border); padding-bottom: 5px; }
        .row { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; margin-bottom: 10px; align-items: center; }
        label { font-size: 0.9rem; font-weight: 500; }
        input { background: #0f172a; border: 1px solid var(--border); color: white; padding: 8px; border-radius: 6px; width: 100%; outline: none; }
        input:focus { border-color: var(--accent); }
        .btns { display: grid; grid-template-columns: 2fr 1fr; gap: 10px; margin-top: 15px; }
        button { padding: 12px; border-radius: 8px; border: none; font-weight: bold; cursor: pointer; transition: 0.2s; }
        #btn-move { background: var(--accent); color: #0f172a; }
        #btn-stop { background: var(--danger); color: white; }
        #btn-save { background: var(--success); color: white; width: 100%; margin-top: 5px; }
        .telemetry { display: grid; grid-template-columns: repeat(3, 1fr); gap: 10px; text-align: center; }
        .t-box { background: #0f172a; padding: 10px; border-radius: 8px; border: 1px solid var(--border); }
        .t-label { font-size: 0.7rem; color: #64748b; }
        .t-val { font-size: 1.1rem; font-weight: bold; color: var(--accent); }
    </style>
</head>
<body>
    <div class="container">
        <div class="card">
            <h1>EB-15 Control Dashboard</h1>
            
            <h2>Configurações de Movimento</h2>
            <div class="row">
                <label>Redução (Caixa)</label>
                <input type="number" id="cfg-reduction" value="3.5" step="0.1">
            </div>
            <div class="row">
                <label>Velocidade (Passos/s)</label>
                <input type="number" id="cfg-speed" value="1000">
            </div>
            <div class="row">
                <label>Aceleração (p/s²)</label>
                <input type="number" id="cfg-accel" value="500">
            </div>
            <button id="btn-save">SALVAR CONFIGURAÇÕES</button>
        </div>

        <div class="card">
            <h2>Comando de Ângulos</h2>
            <div class="row"><label>Base J1 (°)</label><input type="number" id="j1" value="0"></div>
            <div class="row"><label>Ombro J2 (°)</label><input type="number" id="j2" value="0"></div>
            <div class="row"><label>Cotovelo J3 (°)</label><input type="number" id="j3" value="0"></div>
            
            <div class="btns">
                <button id="btn-move">EXECUTAR</button>
                <button id="btn-stop">E-STOP</button>
            </div>
        </div>

        <div class="card">
            <h2>Posição Atual</h2>
            <div class="telemetry">
                <div class="t-box"><div class="t-label">J1</div><div class="t-val" id="v1">0.0°</div></div>
                <div class="t-box"><div class="t-label">J2</div><div class="t-val" id="v2">0.0°</div></div>
                <div class="t-box"><div class="t-label">J3</div><div class="t-val" id="v3">0.0°</div></div>
            </div>
            <p id="st" style="text-align:center; margin-top:15px; font-weight:bold; color:var(--success);">Idle</p>
        </div>
    </div>

    <script>
        const $=(id)=>document.getElementById(id);
        
        async function post(u,b){
            return fetch(u,{method:'POST',body:JSON.stringify(b),headers:{'Content-Type':'application/json'}});
        }

        $('btn-save').onclick = async () => {
             const b = {
                 reduction: parseFloat($('cfg-reduction').value),
                 speed: parseFloat($('cfg-speed').value),
                 accel: parseFloat($('cfg-accel').value)
             };
             await post('/api/config', b);
             alert('Configurações Salvas!');
        };

        $('btn-move').onclick = async () => {
             const b = {
                 j1: parseFloat($('j1').value),
                 j2: parseFloat($('j2').value),
                 j3: parseFloat($('j3').value)
             };
             await post('/api/move', b);
        };

        $('btn-stop').onclick = () => post('/api/stop', {});

        async function update(){
            try {
                const r = await fetch('/api/status');
                const d = await r.json();
                $('v1').innerText = d.joints.j1.toFixed(1) + '°';
                $('v2').innerText = d.joints.j2.toFixed(1) + '°';
                $('v3').innerText = d.joints.j3.toFixed(1) + '°';
                $('st').innerText = d.is_moving ? 'Movimentando...' : 'Pronto';
                $('st').style.color = d.is_moving ? '#38bdf8' : '#10b981';
                
                // Atualiza configs se for a primeira vez
                if(!$('cfg-reduction').getAttribute('data-init')){
                    $('cfg-reduction').value = d.config.reduction;
                    $('cfg-speed').value = d.config.speed;
                    $('cfg-accel').value = d.config.accel;
                    $('cfg-reduction').setAttribute('data-init','true');
                }
            } catch(e){}
        }
        setInterval(update, 500);
    </script>
</body>
</html>
)=====";

// ============================================================================
//  LÓGICA DE MOVIMENTAÇÃO
// ============================================================================
void updateLedStatus() {
    uint32_t color_idle = pixel.Color(10, 10, 10);
    uint32_t colors[3] = { pixel.Color(255, 0, 0), pixel.Color(0, 255, 0), pixel.Color(0, 0, 255) };

    bool isMoving = false;
    for(int i=0; i<3; i++) {
        if(steppers[i]->distanceToGo() != 0) {
            pixel.setPixelColor(0, colors[i]);
            isMoving = true;
            break; 
        }
    }
    if(!isMoving) pixel.setPixelColor(0, color_idle);
    pixel.show();
}

// ============================================================================
//  API HANDLERS
// ============================================================================
void handleStatus() {
    StaticJsonDocument<300> doc;
    doc["is_moving"] = (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0 || stepper3.distanceToGo() != 0);
    
    JsonObject j = doc.createNestedObject("joints");
    j["j1"] = stepper1.currentPosition() / (g_steps_per_deg_motor * g_reduction);
    j["j2"] = stepper2.currentPosition() / (g_steps_per_deg_motor * g_reduction);
    j["j3"] = stepper3.currentPosition() / (g_steps_per_deg_motor * g_reduction);
    
    JsonObject c = doc.createNestedObject("config");
    c["reduction"] = g_reduction;
    c["speed"] = g_speed;
    c["accel"] = g_accel;
    
    String s; serializeJson(doc, s);
    server.send(200, "application/json", s);
}

void handleConfig() {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, server.arg("plain"));
    
    if(doc.containsKey("reduction")) g_reduction = doc["reduction"];
    if(doc.containsKey("speed")) g_speed = doc["speed"];
    if(doc.containsKey("accel")) g_accel = doc["accel"];
    
    for(int i=0; i<3; i++) {
        steppers[i]->setMaxSpeed(g_speed);
        steppers[i]->setAcceleration(g_accel);
    }
    
    server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleMove() {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, server.arg("plain"));
    
    long t1 = (long)(doc["j1"].as<float>() * g_steps_per_deg_motor * g_reduction);
    long t2 = (long)(doc["j2"].as<float>() * g_steps_per_deg_motor * g_reduction);
    long t3 = (long)(doc["j3"].as<float>() * g_steps_per_deg_motor * g_reduction);
    
    stepper1.moveTo(t1);
    stepper2.moveTo(t2);
    stepper3.moveTo(t3);
    
    server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void handleStop() {
    for(int i=0; i<3; i++) steppers[i]->stop();
    server.send(200, "application/json", "{\"status\":\"stopped\"}");
}

// ============================================================================
//  MAIN
// ============================================================================
void setup() {
    Serial.begin(115200);
    
    pixel.begin();
    pixel.setPixelColor(0, pixel.Color(255, 100, 0)); // Booting
    pixel.show();

    // Config Steppers
    for(int i=0; i<3; i++) {
        steppers[i]->setMaxSpeed(g_speed);
        steppers[i]->setAcceleration(g_accel);
    }

    // WiFi AP
    WiFi.softAP(AP_SSID, AP_PASS);
    Serial.println("AP Iniciado: ESP-robot-teste");
    Serial.print("IP: "); Serial.println(WiFi.softAPIP());

    // API
    server.on("/", HTTP_GET, [](){ server.send_P(200, "text/html", INDEX_HTML); });
    server.on("/api/status", HTTP_GET, handleStatus);
    server.on("/api/move", HTTP_POST, handleMove);
    server.on("/api/stop", HTTP_POST, handleStop);
    server.on("/api/config", HTTP_POST, handleConfig);
    server.begin();

    pixel.setPixelColor(0, pixel.Color(10, 10, 10)); // Idle
    pixel.show();
}

void loop() {
    server.handleClient();
    
    stepper1.run();
    stepper2.run();
    stepper3.run();
    
    static unsigned long lastUpdate = 0;
    if(millis() - lastUpdate > 100) {
        updateLedStatus();
        lastUpdate = millis();
    }
}
