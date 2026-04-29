#include <WiFi.h>
#include <WebServer.h>

// Usaremos a Serial2 (nativa na maioria dos ESP32 para comunicação)
// RX, TX
#define RX_PIN 16
#define TX_PIN 17

WebServer server(80);

const char* ssid = "EB15_TESTE_SERIAL";
const char* password = "";

String serialLog = "";
unsigned long lastPingTime = 0;

void handleRoot() {
  Serial.println("[WEB] HOME PAGE chamada.");
  String html = "<html><head><meta charset=\"UTF-8\">";
  html += "<title>Teste Serial ESP32</title>";
  html += "<style>body{font-family:Arial;text-align:center;background:#222;color:#fff;padding:20px;}";
  html += "textarea{width:80%;height:300px;font-size:16px;background:#000;color:#0f0;padding:10px;}</style></head><body>";
  html += "<h1>Teste de Serial ESP32 -> Arduino</h1>";
  html += "<p>O ESP envia PING automaticamente a cada 2s e aguarda recebimento.</p>";
  
  html += "<textarea readonly>" + serialLog + "</textarea><br><br>";
  html += "<a href='/'><button style='padding:15px;font-size:20px;'>Atualizar Log</button></a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
  Serial.println("[WEB] Resposta HOME PAGE despachada.");
}

void setup() {
  Serial.begin(115200);        // Log no PC
  delay(1000);
  Serial.println("\n[SISTEMA] Iniciando Teste 3: Link Serial Ping ESP32 <-> Arduino");
  
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN); // USART2
  Serial.printf("[SISTEMA] Serial2 atrelada aos pinos RX=%d TX=%d a 9600 baud bps.\n", RX_PIN, TX_PIN);

  WiFi.softAP(ssid, password);
  Serial.print("[WIFI] Access Point aberto. SSID: "); Serial.println(ssid);
  Serial.print("[WIFI] Acesso nativo WEB GUI via IP: "); Serial.println(WiFi.softAPIP());
  
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
  
  if (millis() - lastPingTime > 2000) {
    Serial.println("[TX] Timer 2s disparado. Preparando pacote PING...");
    Serial2.println("PING_FROM_ESP");
    Serial.printf("[TX] -> PING_FROM_ESP (T_%ldms)\n", millis());
    serialLog += "-> Enviado: PING_FROM_ESP\n";
    lastPingTime = millis();
    
    // mantém o log curto
    if(serialLog.length() > 2000) {
       serialLog = serialLog.substring(serialLog.length() - 1000);
    }
  }

  while (Serial2.available()) {
    Serial.println("[RX] Buffer de recebimento HardwareSerial contem bytes.");
    String resp = Serial2.readStringUntil('\n');
    resp.trim();
    if(resp.length() > 0) {
      Serial.println("[RX] <- Recebido pacote do link: " + resp);
      serialLog += "<- Recebido: " + resp + "\n";
      
      if(resp == "PING_FROM_ARDUINO") {
         Serial.println("[LINK] Pacote do Arduino validado como PING. Emitindo ACK_FROM_ESP na porta Serial2.");
         Serial2.println("ACK_FROM_ESP");
      } else {
         Serial.println("[LINK] Pacote do Arduino recebido e verificado (nao e um ping direto da base de tempo do arduino).");
      }
    } else {
      Serial.println("[RX] String vazia ignorada.");
    }
  }
}
