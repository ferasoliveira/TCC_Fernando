#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>

#define I2C_SDA 1
#define I2C_SCL 2

#define TCA_ADDR 0x70
#define AS5600_ADDR 0x36

#define M1_PUL 4
#define M1_DIR 5
#define M2_PUL 6
#define M2_DIR 7
#define M3_PUL 8
#define M3_DIR 3

WebServer server(80);

const char* ssid = "EB15_TESTE_ENCODER";
const char* password = "";

int targetMotor = 1;      // 1, 2 ou 3
int targetDegrees = 90;
int delayPulseMicro = 1500; // velocidade

long startDegrees[3] = {0,0,0};
long endDegrees[3] = {0,0,0};
String testResult = "Nenhum teste executado ainda.";

void tcaSelect(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
  // Serial.print("[I2C] TCA9548A comutado para canal: "); Serial.println(channel); // Muitas impressoes se descomentado
}

long readEncoderDegrees(uint8_t tcaChannel) {
  tcaSelect(tcaChannel);
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(0x0E); // Raw Angle Register
  Wire.endTransmission(false);
  Wire.requestFrom(AS5600_ADDR, 2);
  if (Wire.available() == 2) {
    uint16_t rawAngle = (Wire.read() << 8) | Wire.read();
    long deg = map(rawAngle, 0, 4095, 0, 360);
    return deg;
  }
  Serial.print("[I2C_ERRO] Nao foi possivel ler encoder no canal "); Serial.println(tcaChannel);
  return -1; // erro
}

uint8_t getChannelForMotor(int m) {
  if (m == 1) return 6;
  if (m == 2) return 4;
  if (m == 3) return 2;
  return 6;
}

void handleRoot() {
  Serial.println("[WEB] Pagina inicial carregada.");
  String html = "<html><head><meta charset=\"UTF-8\">";
  html += "<title>Teste Encoders ESP32</title>";
  html += "<style>body{font-family:Arial;text-align:center;background:#222;color:#fff;padding:20px;}";
  html += "button{font-size:24px;padding:15px;margin:10px;border-radius:8px;border:none;cursor:pointer;background:#4CAF50;color:white;}";
  html += "select,input{font-size:20px;padding:10px;margin:10px;}</style></head><body>";
  html += "<h1>Teste de Encoders - ESP32</h1>";
  html += "<div style='background:#444;padding:20px;border-radius:10px;margin-bottom:20px;'>";
  html += "<h3>Leitura Atual:</h3>";
  
  long e1 = readEncoderDegrees(6);
  long e2 = readEncoderDegrees(4);
  long e3 = readEncoderDegrees(2);
  Serial.printf("[SENSOR] M1: %ldº | M2: %ldº | M3: %ldº\n", e1, e2, e3);
  
  html += "Motor 1 (Canal 6): " + String(e1) + "º<br>";
  html += "Motor 2 (Canal 4): " + String(e2) + "º<br>";
  html += "Motor 3 (Canal 2): " + String(e3) + "º<br>";
  html += "</div>";
  
  html += "<div style='background:#555;padding:20px;border-radius:10px;'>";
  html += "<h3>Executar Movimento e Validar</h3>";
  html += "<form action='/test' method='GET'>";
  html += "Motor: <select name='m'><option value='1'>M1</option><option value='2'>M2</option><option value='3'>M3</option></select><br>";
  html += "Passos p/ andar: <input type='number' name='steps' value='200'><br>";
  html += "Direção (1 ou 0): <input type='number' name='dir' value='1'><br>";
  html += "<button type='submit'>Executar e Medir</button>";
  html += "</form>";
  html += "<p style='color:#ffeb3b;'><b>Resultado do Último Teste:</b><br>" + testResult + "</p>";
  html += "</div>";
  html += "<br><a href='/'><button style='background:#2196F3'>Atualizar Leituras</button></a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleTest() {
  Serial.println("[WEB] Solicitacao HTTP para rotina /test recebida");
  if (server.hasArg("m") && server.hasArg("steps") && server.hasArg("dir")) {
    int m = server.arg("m").toInt();
    int steps = server.arg("steps").toInt();
    int d = server.arg("dir").toInt();
    
    Serial.printf("[COMANDO] Vai mover Motor %d. Qtd: %d passsos, Direcao: %d\n", m, steps, d);
    
    int actPul, actDir;
    if (m == 1) { actPul = M1_PUL; actDir = M1_DIR; }
    else if (m == 2) { actPul = M2_PUL; actDir = M2_DIR; }
    else { actPul = M3_PUL; actDir = M3_DIR; }

    uint8_t channel = getChannelForMotor(m);
    Serial.printf("[SENSOR] Lendo angulo inicial no canal %d...\n", channel);
    long startDeg = readEncoderDegrees(channel);
    Serial.printf("[SENSOR] Angulo inicial lido: %ldº\n", startDeg);
    
    Serial.println("[MOTOR] Movimentando...");
    digitalWrite(actDir, d);
    for(int i=0; i<steps; i++){
      digitalWrite(actPul, HIGH);
      delayMicroseconds(delayPulseMicro);
      digitalWrite(actPul, LOW);
      delayMicroseconds(delayPulseMicro);
    }
    Serial.println("[MOTOR] O Motor parou fisicamente. Aguardando estabilizacao mecanica (500ms)...");
    delay(500); // estabilizador
    
    Serial.printf("[SENSOR] Lendo angulo final no canal %d...\n", channel);
    long endDeg = readEncoderDegrees(channel);
    Serial.printf("[SENSOR] Angulo final lido: %ldº\n", endDeg);
    
    testResult = "Motor " + String(m) + " | Inicio: " + String(startDeg) + "º | Fim: " + String(endDeg) + "º | Delta: " + String(endDeg - startDeg) + "º";
    Serial.println("[SISTEMA] Teste finalizado com sucesso. Atualizando strings na Home.");
  } else {
    Serial.println("[WEB] Parametros incompletos no GET request da rota /test");
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n[SISTEMA] Iniciando Teste 2: Motores Locais e Encoders I2C");
  
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.println("[SISTEMA] I2C inicializado.");
  
  pinMode(M1_PUL, OUTPUT); pinMode(M1_DIR, OUTPUT);
  pinMode(M2_PUL, OUTPUT); pinMode(M2_DIR, OUTPUT);
  pinMode(M3_PUL, OUTPUT); pinMode(M3_DIR, OUTPUT);

  WiFi.softAP(ssid, password);
  Serial.print("[WIFI] Rede AP Aberta: "); Serial.println(ssid);
  Serial.print("[WIFI] IP: "); Serial.println(WiFi.softAPIP());
  
  server.on("/", handleRoot);
  server.on("/test", handleTest);
  server.begin();
  Serial.println("[SISTEMA] Servidor iniciado!\n");
}

void loop() {
  server.handleClient();
}
