#include <WiFi.h>
#include <WebServer.h>

// Pinos ESP32 pros Motores
#define M1_PUL 4
#define M1_DIR 5
#define M2_PUL 6
#define M2_DIR 7
#define M3_PUL 8
#define M3_DIR 3

WebServer server(80);

const char* ssid = "EB15_TESTE";
const char* password = "";

int targetMotor = 1;      // 1, 2 ou 3
int delayPulseMicro = 1000;  // controle de velocidade (inverso)
bool running = false;
unsigned long lastChangeTime = 0;
bool currentDir = false;

void handleRoot() {
  Serial.println("[WEB] Solicitacao para carregar a pagina raiz (/)");
  String html = "<html><head><meta charset=\"UTF-8\">";
  html += "<title>Teste Motores ESP32</title>";
  html += "<style>body{font-family:Arial;text-align:center;background:#222;color:#fff;padding:20px;}";
  html += "button{font-size:24px;padding:15px;margin:10px;border-radius:8px;border:none;cursor:pointer;}";
  html += ".btn-green{background:#4CAF50;color:white;} .btn-red{background:#f44336;color:white;}";
  html += "select,input{font-size:20px;padding:10px;}</style></head><body>";
  html += "<h1>Teste Motores - ESP32</h1>";
  html += "<p>Motor Selecionado: " + String(targetMotor) + "</p>";
  html += "<p>Velocidade (Delay us): " + String(delayPulseMicro) + "</p>";
  html += "<p>Status: " + String(running ? "RODANDO" : "PARADO") + "</p>";
  
  html += "<form action='/set' method='GET'>";
  html += "<label>Motor: </label>";
  html += "<select name='m'><option value='1'>M1 (PUL 4, DIR 5)</option><option value='2'>M2 (PUL 6, DIR 7)</option><option value='3'>M3 (PUL 8, DIR 3)</option></select><br><br>";
  html += "<label>Delay do Pulso (uS) - Menor é mais rapido: </label>";
  html += "<input type='number' name='d' value='1000'><br><br>";
  html += "<button type='submit' class='btn-green' name='start' value='1'>INICIAR TEXTE VAI E VOLTA</button>";
  html += "<button type='submit' class='btn-red' name='stop' value='1'>PARAR TUDO</button>";
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
  Serial.println("[WEB] Pagina enviada com sucesso.");
}

void handleSet() {
  Serial.println("[WEB] Recebeu comando no endpoint /set");
  if (server.hasArg("stop")) {
    running = false;
    Serial.println("[COMANDO] PARADA SOLICITADA. Modificando estado running para FALSE.");
  } else {
    if (server.hasArg("m")) {
      targetMotor = server.arg("m").toInt();
      Serial.print("[COMANDO] Motor alterado para: "); Serial.println(targetMotor);
    }
    if (server.hasArg("d")) {
      delayPulseMicro = server.arg("d").toInt();
      Serial.print("[COMANDO] Delay alterado para: "); Serial.println(delayPulseMicro);
    }
    running = true;
    lastChangeTime = millis();
    Serial.println("[COMANDO] Execucao INICIADA. Modificando estado running para TRUE.");
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n[SISTEMA] Iniciando Teste 1: Controle Isolado de Motores");
  
  pinMode(M1_PUL, OUTPUT); pinMode(M1_DIR, OUTPUT);
  pinMode(M2_PUL, OUTPUT); pinMode(M2_DIR, OUTPUT);
  pinMode(M3_PUL, OUTPUT); pinMode(M3_DIR, OUTPUT);
  Serial.println("[SISTEMA] Pinos configurados.");

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("[WIFI] Rede AP Aberta: "); Serial.println(ssid);
  Serial.print("[WIFI] Server IP: ");
  Serial.println(IP);

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println("[SISTEMA] Servidor HTTP pronto para receber dados.");
}

void loop() {
  server.handleClient();
  
  if (running) {
    if (millis() - lastChangeTime > 5000) { // Alterna a cada 5 segundos
      currentDir = !currentDir;
      lastChangeTime = millis();
      Serial.print("[MOTOR] 5s passaram! Invertendo direcao para: ");
      Serial.println(currentDir ? "HIGH" : "LOW");
    }
    
    int actPul = M1_PUL, actDir = M1_DIR;
    if (targetMotor == 2) { actPul = M2_PUL; actDir = M2_DIR; }
    else if (targetMotor == 3) { actPul = M3_PUL; actDir = M3_DIR; }
    
    digitalWrite(actDir, currentDir);
    digitalWrite(actPul, HIGH);
    delayMicroseconds(delayPulseMicro);
    digitalWrite(actPul, LOW);
    delayMicroseconds(delayPulseMicro);
  }
}
