#include <SoftwareSerial.h>

// RX=A4, TX=A5
SoftwareSerial espSerial(A4, A5); 

// Pinos Shield CNC v3 para Uno
#define EN_PIN 8
#define M4_PUL 2
#define M4_DIR 3
#define M5_PUL 4
#define M5_DIR 5
#define M6_PUL 6
#define M6_DIR 7

void setup() {
  Serial.begin(115200);   // Monitor serial
  delay(500);
  Serial.println("\n[SISTEMA] Iniciando Teste 4: Arduino Escravo RX Motores");
  
  espSerial.begin(115200); // Conexao com o ESP
  Serial.println("[SISTEMA] Conexao SoftwareSerial instanciada em A4/A5");

  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW); // Ativa os drivers CNC
  Serial.println("[SISTEMA] Pino ENABLE global CNC em ALTA(Driver Ativo).");

  pinMode(M4_PUL, OUTPUT); pinMode(M4_DIR, OUTPUT);
  pinMode(M5_PUL, OUTPUT); pinMode(M5_DIR, OUTPUT);
  pinMode(M6_PUL, OUTPUT); pinMode(M6_DIR, OUTPUT);
  Serial.println("[SISTEMA] Direcao e Pulso Motores M4,M5,M6 instanciados.");

  Serial.println("\n[PRONTO] Arduino: Aguardando comandos do ESP no formato MOVE_REMOTO:M,PASSOS,DIR");
}

void loop() {
  if (espSerial.available()) {
    String cmd = espSerial.readStringUntil('\n');
    cmd.trim();
    if(cmd.startsWith("MOVE_REMOTO:")) {
      Serial.println("[RX] Recebido Pkt de Movimento: " + cmd);
      // Ex: MOVE_REMOTO:4,200,1
      int c1 = cmd.indexOf(':');
      int c2 = cmd.indexOf(',');
      int c3 = cmd.lastIndexOf(',');

      if(c1 > 0 && c2 > c1 && c3 > c2) {
        int m = cmd.substring(c1+1, c2).toInt();
        int steps = cmd.substring(c2+1, c3).toInt();
        int dir = cmd.substring(c3+1).toInt();
        
        Serial.printf("[PARSER] Sucesso! Motor Alvo: %d | Passos Requeridos: %d | Sentido: %d\n", m, steps, dir);

        int actPul = M4_PUL, actDir = M4_DIR;
        if(m == 5) { actPul = M5_PUL; actDir = M5_DIR; }
        else if (m == 6) { actPul = M6_PUL; actDir = M6_DIR; }

        Serial.println("[EXECUCAO] Gerando pulsos step/dir para as portas logicas...");
        digitalWrite(actDir, dir);
        for(int i=0; i<steps; i++) {
          digitalWrite(actPul, HIGH);
          delayMicroseconds(1000);
          digitalWrite(actPul, LOW);
          delayMicroseconds(1000);
        }
        Serial.println("[EXECUCAO] Concluida. Aguardando novo pacote de link da serial ESP.");
      } else {
        Serial.println("[PARSER] Erro de descompactacao. Padrao posicional de virgulas invalido.");
      }
    } else if (cmd.length() > 0) { // Ignora LFs isolados
      Serial.println("[RX] Pacote nao reconhecido pelo filtro MOVE_REMOTO: " + cmd);
    }
  }
}
