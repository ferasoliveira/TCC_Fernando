/*
 * ============================================================================
 *  EB-15 Robotic Arm — Simulador AS5600 PWM Mode (Para Arduino Uno)
 * ============================================================================
 *  FUNCIONALIDADES:
 *  - Gera um PWM via software de exatos 115Hz simulando um AS5600 fisico.
 *  - Saida do sinal fake: Pino Digital 9 do Arduino Uno.
 *  - Controle: Digite no Serial Monitor (115200) um ngulo de 0 a 360 e dê Enter.
 * 
 *  COMO TESTAR:
 *  1. Ligue o GND do Arduino no GND do ESP32.
 *  2. Ligue o Pino 9 do Arduino no Pino 18 (Input) do ESP32.
 *  3. Digite "90", "180" ou "320" no monitor serial do Arduino e veja se a 
 *     página web do ESP32 responde precisamente do outro lado.
 * ============================================================================
 */

#include <Arduino.h>

#define PWM_PIN 9
#define PERIOD_US 8695 // 1 div 115Hz = 8.695ms = 8695us

// O Datasheet do AS5600 dita que o duty cycle útil varia de 3.1% (0 graus) a 99.4% (360 graus).
// Em 8695us, isso significa:
#define MIN_HIGH_US 270  // Corresponde a 0 graus
#define MAX_HIGH_US 8646 // Corresponde a 360 graus

float current_angle = 180.0;
unsigned long high_time_us;

unsigned long period_start = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PWM_PIN, OUTPUT);
  delay(100);
  
  Serial.println("\n\n================================================");
  Serial.println("  Simulador Físico do Encoder AS5600");
  Serial.println("================================================");
  Serial.println("-> Sinal sendo gerado no PINO 9.");
  Serial.println("-> Digite qualquer angulo (0 a 360) no topo do");
  Serial.println("   monitor serial e aperte ENTER.");
  
  updatePWM(current_angle);
}

void loop() {
  // Lida com Input do Usuário
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if(input.length() > 0) {
       float in_angle = input.toFloat();
       if (in_angle >= 0.0 && in_angle <= 360.0) {
         current_angle = in_angle;
         updatePWM(current_angle);
       } else {
         Serial.println("[Aviso] Digite apenas valores de 0 a 360.");
       }
    }
  }

  // Gerador de Software PWM exato (Não adicione delays neste loop)
  unsigned long now = micros();
  unsigned long elapsed = now - period_start;
  
  if (elapsed >= PERIOD_US) {
    period_start = now;
    digitalWrite(PWM_PIN, HIGH);
  } else if (elapsed >= high_time_us) {
    digitalWrite(PWM_PIN, LOW);
  }
}

void updatePWM(float angle) {
  // Matematica linear mapeando Angulo Físico para Tempo High
  float mapped_us = MIN_HIGH_US + (angle / 360.0) * (MAX_HIGH_US - MIN_HIGH_US);
  high_time_us = (unsigned long)mapped_us;
  
  Serial.print("=> Angulo Injetado no PIN 9: ");
  Serial.print(angle, 1);
  Serial.print(" graus | Largura de Pulso Gerado: ");
  Serial.print(high_time_us);
  Serial.println(" us");
}
