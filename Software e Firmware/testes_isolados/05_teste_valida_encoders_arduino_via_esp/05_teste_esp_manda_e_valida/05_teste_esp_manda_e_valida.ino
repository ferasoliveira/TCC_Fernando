#include <WiFi.h>
#include <WebServer.h>

#define RX_PIN 19
#define TX_PIN 20

WebServer server(80);

const char* ssid = "EB15_TESTE_REMOTO_ENC";
const char* password = "";

String lastResult = "Nenhum resultado recebido ainda.";

void handleRoot() {
  Serial.println("[WEB] Solicitacao HTTP para home (/) recebida.");
  String html = "<html><head><meta charset=\"UTF-8\">";
  html += "<title>Teste Encoders Remoto</title>";
  html += "<style>body{font-family:Arial;text-align:center;background:#222;color:#fff;padding:20px;}";
  html += "button{font-size:20px;padding:15px;margin:10px;border-radius:8px;cursor:pointer;background:#ff9800;color:white;}";
  html += "select,input{font-size:20px;padding:10px;margin:10px;}</style></head><body>";
  html += "<h1>ESP32 Pede pro Arduino Mover e Medir</h1>";
  
  html += "<form action='/ask' method='GET'>";
  html += "Motor Arduino: <select name='m'><option value='4'>M4</option><option value='5'>M5</option><option value='6'>M6</option></select><br>";
  html += "Passos p/ andar no Arduino: <input type='number' name='steps' value='500'><br>";
  html += "Direção (1/0): <input type='number' name='dir' value='1'><br>";
  html += "<button type='submit'>Solicitar Teste ao Arduino</button>";
  html += "</form>";
  
  html += "<div style='background:#444;padding:20px;margin:20px;border-radius:10px;'>";
  html += "<h3>Resposta do Arduino:</h3>";
  html += "<h2 style='color:#8bc34a;'>" + lastResult + "</h2>";
  html += "</div>";
  html += "<a href='/'><button style='background:#555;'>Atualizar Tela</button></a>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
  Serial.println("[WEB] Pagina HTML home enviada.");
}

void handleAsk() {
  Serial.println("[WEB] Solicitacao HTTP para acionamento na URL /ask!");
  if (server.hasArg("m") && server.hasArg("steps") && server.hasArg("dir")) {
    int m = server.arg("m").toInt();
    int s = server.arg("steps").toInt();
    int d = server.arg("dir").toInt();
    
    Serial.printf("[COMANDO] WEB quer enviar ordem: Motor %d, %d passos, sentido %d\n", m, s, d);
    Serial.println("[TX] Transformando em TEST_ENC e disparando via UART 19/20...");
    Serial1.printf("TEST_ENC:%d,%d,%d\n", m, s, d);
    lastResult = "Aguardando resposta do Arduino para M" + String(m) + "... (Atualize a aba em breve)";
  } else {
    Serial.println("[WEB] Formato invalido no forms de requisicao. Sem dados enviados.");
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);   
  delay(500);
  Serial.println("\n[SISTEMA] Iniciando Teste 5: Feedback Loop Hibrido (ESP Manda, Uno Verifica)");
  
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN); 
  Serial.printf("[SISTEMA] Link Serial RX=%d, TX=%d Pronto.\n", RX_PIN, TX_PIN);

  WiFi.softAP(ssid, password);
  Serial.printf("[WIFI] ESP Access point %s esta rodando livre no IP ", ssid);
  Serial.println(WiFi.softAPIP());
  
  server.on("/", handleRoot);
  server.on("/ask", handleAsk);
  server.begin();
  Serial.println("[SISTEMA] ESP server iniciado, tudo pronto!");
}

void loop() {
  server.handleClient();
  
  // Escuta se o Uno terminou o evento no alem e mandou feedBack 
  while (Serial1.available()) {
    String msg = Serial1.readStringUntil('\n');
    msg.trim();
    if (msg.length() > 0) {
      Serial.println("[RX] <- Dados vindos pela porta UART (19/20): " + msg);
    }
    
    if(msg.startsWith("RES_ENC:")) {
      Serial.println("[PARSER] Pacote validado como um RESPONSE de medicao do hardware Escravo Arduino.");
      int c = msg.indexOf(',');
      if(c > 0) {
        String m = msg.substring(8, c);
        String delta = msg.substring(c+1);
        lastResult = "Motor " + m + " reportou remotamente que girou " + delta + " graus físicos medidos pelo encoder do Arduino!";
        Serial.printf("[FEEDBACK] Sucesso de validacao do Mestre-Escravo. Motor %s andou %s graus.\n", m.c_str(), delta.c_str());
      } else {
        Serial.println("[PARSER] Erro na formatação comma-separated vinda da resposta do Arduino.");
      }
    }
  }
}
