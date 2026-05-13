#include <SoftwareSerial.h>
#include <SoftwareWire.h> // Necessário instalar na IDE para usar A2/A3 como I2C no Uno

// RX=A5, TX=A4
SoftwareSerial espSerial(A5, A4); 

// TCA9548A no I2C alternativo (A2, A3)
SoftwareWire myWire(A2, A3);

#define TCA_ADDR 0x70
#define AS5600_ADDR 0x36

#define EN_PIN 8
#define M4_PUL 2
#define M4_DIR 3
#define M5_PUL 4
#define M5_DIR 5
#define M6_PUL 6
#define M6_DIR 7

void tcaSelect(uint8_t channel) {
  if (channel > 7) return;
  myWire.beginTransmission(TCA_ADDR);
  myWire.write(1 << channel);
  myWire.endTransmission();
  // Serial.print("[I2C_Soft] Selecioando Ch: "); Serial.println(channel);
}

long readEncoderDegrees(uint8_t tcaChannel) {
  tcaSelect(tcaChannel);
  myWire.beginTransmission(AS5600_ADDR);
  myWire.write(0x0E); // Registrador com os dados RAW do encoder
  myWire.endTransmission(false);
  myWire.requestFrom(AS5600_ADDR, 2);
  if (myWire.available() == 2) {
    uint16_t raw = (myWire.read() << 8) | myWire.read();
    long gra = map(raw, 0, 4095, 0, 360);
    return gra;
  }
  Serial.print("[I2C_Soft] Erro fatal. Nao lido dados no canal "); Serial.println(tcaChannel);
  return -1;
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n[SISTEMA] Iniciando Teste 5 Escravo: Recebedor de Comando, Movimentacao e Sensorizacao");
  
  espSerial.begin(9600);
  Serial.println("[SISTEMA] Ponte UART Serial c/ ESP em A5/A4 (9600 bps) ativa.");
  
  myWire.begin();
  Serial.println("[SISTEMA] Ponte SoftwareWire I2C em A2/A3 ativa localmente.");

  pinMode(EN_PIN, OUTPUT); digitalWrite(EN_PIN, LOW);
  pinMode(M4_PUL, OUTPUT); pinMode(M4_DIR, OUTPUT);
  pinMode(M5_PUL, OUTPUT); pinMode(M5_DIR, OUTPUT);
  pinMode(M6_PUL, OUTPUT); pinMode(M6_DIR, OUTPUT);

  Serial.println("[PRONTO] Arduino pronto p/ atuar remotamente.");
}

void loop() {
  if (espSerial.available()) {
    String cmd = espSerial.readStringUntil('\n');
    cmd.trim();
    if(cmd.startsWith("TEST_ENC:")) {
      Serial.println("\n[RX] Recebida a trigger TEST_ENC do ESP32! Processando string...");
      int c1 = cmd.indexOf(':');
      int c2 = cmd.indexOf(',');
      int c3 = cmd.lastIndexOf(',');

      if(c1 > 0 && c2 > c1 && c3 > c2) {
        int m = cmd.substring(c1+1, c2).toInt();
        int steps = cmd.substring(c2+1, c3).toInt();
        int dir = cmd.substring(c3+1).toInt();
        
        Serial.print("[PARSER] Moto Alvo: "); Serial.print(m);
        Serial.print(" | Passos Requeridos: "); Serial.print(steps);
        Serial.print(" | Sentido: "); Serial.println(dir);

        int actPul = M4_PUL, actDir = M4_DIR;
        uint8_t ch = 6; 
        if(m == 5) { actPul = M5_PUL; actDir = M5_DIR; ch = 4; }
        else if(m == 6) { actPul = M6_PUL; actDir = M6_DIR; ch = 2; }

        Serial.print("[SENSOR] Consultando encoder do motor via TCA canal "); Serial.print(ch); Serial.println("...");
        long startDeg = readEncoderDegrees(ch);
        Serial.print("[SENSOR] Angulo inicial capturado pre-acionamento mecanico: "); Serial.print(startDeg); Serial.println(" graus");
        
        Serial.println("[MOTOR] Acionando pulsos da CNC shield...");
        digitalWrite(actDir, dir);
        for(int i=0; i<steps; i++) {
          digitalWrite(actPul, HIGH);
          delayMicroseconds(1000);
          digitalWrite(actPul, LOW);
          delayMicroseconds(1000);
        }
        Serial.println("[MOTOR] Terminadmos as chaves locais na CNC. Pausando 500ms p/ abrandamento...");
        delay(500); 
        
        Serial.println("[SENSOR] Re-lendo via SoftwareWire no encoder...");
        long endDeg = readEncoderDegrees(ch);
        long delta = endDeg - startDeg;
        Serial.print("[SENSOR] Terminologia Pós: "); Serial.print(endDeg);
        Serial.print(" graus | Variacao (Delta) = "); Serial.print(delta); Serial.println(" graus");
        
        // Devolve ao ESP
        // Formato: RES_ENC:M,DELTA
        Serial.print("[TX] Remetendo o payload (RES_ENC:"); Serial.print(m);
        Serial.print(","); Serial.print(delta);
        Serial.println(") de forma assincrona p/ ESP32 via UART.");
        espSerial.print("RES_ENC:");
        espSerial.print(m);
        espSerial.print(",");
        espSerial.println(delta);
        
        Serial.println("[CICLO] Ciclo complexo ESP->Arduino->ESP abortado com sucesso da nossa parte.");
      } else {
        Serial.println("[PARSER] Payload do ESP invalido ou corrompido em bits.");
      }
    } else if (cmd.length() > 0) {
      Serial.println("[RX] Pacote nao reconhecido ou LF isolado: " + cmd);
    }
  }
}
