#include <SoftwareSerial.h>

// RX do Esp = A4, TX = A5 (então pro Arduino RX=A4, TX=A5)
SoftwareSerial espSerial(A4, A5); // RX, TX

unsigned long lastPingTime = 0;

void setup() {
  Serial.begin(115200);   // Log no PC
  delay(500);
  espSerial.begin(115200); // Comunicacao c/ ESP
  
  Serial.println("\n=====================================");
  Serial.println("   TESTE 3: ARDUINO SERIAL (A4/A5)   ");
  Serial.println("=====================================\n");
  Serial.println("[SISTEMA] SoftwareSerial inicializada com sucesso em RX=A4 TX=A5.");
}

void loop() {
  // PING a cada 2 segundos
  if(millis() - lastPingTime > 2000) {
    espSerial.println("PING_FROM_ARDUINO");
    Serial.print("[TX] -> PING_FROM_ARDUINO (T_");
    Serial.print(millis());
    Serial.println("ms)");
    lastPingTime = millis();
  }
  
  // Recebe pacotes do ESP e joga na tela
  if (espSerial.available()) {
    Serial.println("[RX] Buffer serial contem dados. Lendo...");
    String msg = espSerial.readStringUntil('\n');
    msg.trim();
    if(msg.length() > 0) {
      Serial.println("[RX] <- Recebido do ESP32: " + msg);
      // Se recebeu PING_FROM_ESP, envia um ACK
      if(msg == "PING_FROM_ESP") {
        Serial.println("[LINK] Pacote eh PING. Respondendo com ACK_FROM_ARDUINO...");
        espSerial.println("ACK_FROM_ARDUINO");
      }
    } else {
      Serial.println("[RX] Linha recebida estava vazia.");
    }
  }
}
