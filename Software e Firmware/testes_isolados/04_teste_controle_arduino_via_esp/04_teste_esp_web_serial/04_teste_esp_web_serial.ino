#include <WiFi.h>
#include <WebServer.h>

#define RX_PIN 19
#define TX_PIN 20

// Motores Locais
#define M1_PUL 4
#define M1_DIR 5
#define M2_PUL 6
#define M2_DIR 7
#define M3_PUL 8
#define M3_DIR 3

WebServer server(80);

const char* ssid = "EB15_TESTE_LOCAL_REMOTO";
const char* password = "";

void handleRoot() {
  Serial.println("[WEB] Requisicao Home / (GUI Mestre Escravo) chamada.");
  String html = "<html><head><meta charset=\"UTF-8\">";
  html += "<title>Teste Mistro</title>";
  html += "<style>body{font-family:Arial;text-align:center;background:#222;color:#fff;padding:20px;}";
  html += "button{font-size:20px;padding:15px;margin:10px;border-radius:8px;cursor:pointer;background:#2196F3;color:white;}";
  html += "select,input{font-size:20px;padding:10px;margin:10px;}</style></head><body>";
  html += "<h1>Teste Motores - Local (ESP) vs Remoto (Arduino)</h1>";
  
  html += "<form action='/move' method='GET'>";
  html += "<p>Alvo: <select name='target'>";
  html += "<optgroup label='ESP32 Local'>";
  html += "<option value='L1'>Motor 1 (Local)</option>";
  html += "<option value='L2'>Motor 2 (Local)</option>";
  html += "<option value='L3'>Motor 3 (Local)</option>";
  html += "</optgroup>";
  html += "<optgroup label='Arduino Remoto via Serial'>";
  html += "<option value='R4'>Motor 4 (Arduino)</option>";
  html += "<option value='R5'>Motor 5 (Arduino)</option>";
  html += "<option value='R6'>Motor 6 (Arduino)</option>";
  html += "</optgroup>";
  html += "</select></p>";
  
  html += "<p>Passos: <input type='number' name='steps' value='400'></p>";
  html += "<p>Direção (1/0): <input type='number' name='dir' value='1'></p>";
  html += "<button type='submit'>Enviar Comando de Teste</button>";
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleMove() {
  Serial.println("[WEB] Endpoint /move chamado via HTTP GET.");
  if (server.hasArg("target") && server.hasArg("steps") && server.hasArg("dir")) {
    String t = server.arg("target");
    int steps = server.arg("steps").toInt();
    int dir = server.arg("dir").toInt();
    
    Serial.printf("[COMANDO] Movimentar -> ALVO: %s, QTD: %d, DIRECAO: %d\n", t.c_str(), steps, dir);
    
    // É comando para ESP Local?
    if (t.startsWith("L")) {
      Serial.println("[CONTROLE] Destino LOCAL interpretado. Modulando PWM por bitbang nativo no ESP32.");
      int m = t.substring(1).toInt();
      int actPul, actDir;
      if (m == 1) { actPul = M1_PUL; actDir = M1_DIR; }
      else if (m == 2) { actPul = M2_PUL; actDir = M2_DIR; }
      else { actPul = M3_PUL; actDir = M3_DIR; }

      digitalWrite(actDir, dir);
      for(int i=0; i<steps; i++) {
        digitalWrite(actPul, HIGH);
        delayMicroseconds(1000);
        digitalWrite(actPul, LOW);
        delayMicroseconds(1000);
      }
      Serial.println("[CONTROLE] Movimento LOCAL concluido e sincronizado.");
    } 
    // É comando para Arduino Remoto?
    else if (t.startsWith("R")) {
      Serial.println("[CONTROLE] Destino REMOTO interpretado. Estruturando e enviando pacote Serial para o Arduino Escravo.");
      int m = t.substring(1).toInt();
      // Envia pacote serial pro arduino
      Serial1.printf("MOVE_REMOTO:%d,%d,%d\n", m, steps, dir);
      Serial.printf("[TX] Pacote serial enviado: (MOVE_REMOTO:%d,%d,%d\\n)\n", m, steps, dir);
    }
  } else {
    Serial.println("[WEB] ERRO: Falta paramentros na string GET do HTTP Header.");
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);   // PC
  delay(1000);
  Serial.println("\n[SISTEMA] Iniciando Teste 4: Roteamento ESP(Web) -> Motores(Local vs Remoto)");
  
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); // Arduino
  Serial.printf("[SISTEMA] Serial1 configurada em RX=%d TX=%d\n", RX_PIN, TX_PIN);

  pinMode(M1_PUL, OUTPUT); pinMode(M1_DIR, OUTPUT);
  pinMode(M2_PUL, OUTPUT); pinMode(M2_DIR, OUTPUT);
  pinMode(M3_PUL, OUTPUT); pinMode(M3_DIR, OUTPUT);

  WiFi.softAP(ssid, password);
  Serial.printf("[WIFI] Access Point %s rodando no IP: ", ssid);
  Serial.println(WiFi.softAPIP());
  
  server.on("/", handleRoot);
  server.on("/move", handleMove);
  server.begin();
}

void loop() {
  server.handleClient();
}
