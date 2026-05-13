/*
 * ============================================================================
 *  EB-15 Robotic Arm — Leitor de Pino OUT Universal c/ Auto-Config PWM
 * ============================================================================
 *  FUNCIONALIDADES:
 *  1. No setup(), o Arduino usa I2C para forçar a RAM do AS5600 para PWM.
 *  2. No loop(), o Arduino escuta o pino "OUT" e auto-detecta o pulso.
 *  3. Se a comunicacao I2C der certo, a tela vai mudar magicamente de 
 *     [MODO: ANALOGICO] para [MODO: PWM] indicando o sucesso da configuração!
 * 
 *  PINAGEM OBRIGATORIA:
 *  - 5V  -> VDD5V do AS5600
 *  - GND -> GND do AS5600
 *  - A4  -> SDA do AS5600 (Para o comando I2C)
 *  - A5  -> SCL do AS5600 (Para o comando I2C)
 *  - A0  -> OUT do AS5600 (Para a leitura física do sinal PWM ou Analogico)
 * ============================================================================
 */

#include <Arduino.h>
#include <Wire.h> // Adicionado para a comunicacao I2C

#define AS5600_ADDR 0x36
#define CONF_REG_LSB 0x08
#define AS5600_OUT_PIN A0

void setup() {
  Serial.begin(115200);
  Wire.begin(); 
  
  delay(1000);
  Serial.println("\n\n================================================");
  Serial.println("  Teste de Diagnose do Registrador CONF (AS5600)");
  Serial.println("================================================");
  
  // 1. LER O VALOR ATUAL (ANTES)
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(CONF_REG_LSB);
  Wire.endTransmission();
  Wire.requestFrom(AS5600_ADDR, 1);
  byte conf_antes = 0;
  if (Wire.available()) conf_antes = Wire.read();
  
  Serial.print("[I2C] Registrador CONF_LSB antes do comando: 0x");
  Serial.println(conf_antes, HEX);

  // 2. ENVIAR A MUDANÇA PARA PWM
  Serial.println("\n[I2C] Injetando configuracao PWM (0x10) na RAM...");
  Wire.beginTransmission(AS5600_ADDR);
  // Escrevendo nos dois bytes do CONF para forçar (0x07 e 0x08)
  Wire.write(0x07); 
  Wire.write(0x00); // 0x07: Filtros e watchdog padrao
  Wire.write(0x10); // 0x08: OUTS = 01 (PWM)
  byte err = Wire.endTransmission();
  
  delay(50); // Aguarda o chip processar internamente

  // 3. LER O VALOR NOVO (DEPOIS)
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(CONF_REG_LSB);
  Wire.endTransmission();
  Wire.requestFrom(AS5600_ADDR, 1);
  byte conf_depois = 0;
  if (Wire.available()) conf_depois = Wire.read();

  Serial.print("[I2C] Registrador CONF_LSB DEPOIS do comando: 0x");
  Serial.println(conf_depois, HEX);

  if (err == 0) {
    if (conf_depois == 0x10) {
      Serial.println("\n[I2C] SUCESSO ABSOLUTO! O chip aceitou virar PWM.");
    } else {
      Serial.println("\n[ERRO CRITICO DE HARDWARE] O chip REJEITOU a configuracao!");
      Serial.println("Sintoma: O I2C entregou os dados, mas o AS5600 ignorou e manteve o valor antigo.");
      Serial.println("Motivo Provavel: Como seu ZMCO e 1, este chip teve o modo Analogico QUEIMADO definitivamente (Permalock).");
      Serial.println("O pino OUT estara forcadamente travado em Analogico para sempre p/ este modulo as5600.");
    }
  } else {
    Serial.println("\n[I2C_ERRO] Falha na comunicacao I2C com o Encoder.");
  }

  Serial.println("\n[LEITURA] Escutando o pino fisico A0 do Arduino...\n");
}

void loop() {
  // Tenta ler pulsos na saída para ver se a configuracao I2C deu certo
  // 30 milissegundos de timeout é tempo de sobra para detectar os 115Hz do PWM
  // Lembrete: No Arduino Uno, os pinos Analogicos mapeiam p/ digital na ordem: A0 = 14
  unsigned long highT = pulseIn(14, HIGH, 30000); 
  
  if (highT > 0) {
    unsigned long lowT = pulseIn(14, LOW, 30000);
    
    if (lowT > 0) {
      unsigned long period = highT + lowT;
      float dc = (float)highT / (float)period;
      
      float minDuty = 128.0 / 4119.0;
      float maxDuty = 4096.0 / 4119.0;
      
      // Limites de segurança fisicos
      float dc_clamped = dc;
      if (dc_clamped < minDuty) dc_clamped = minDuty;
      if (dc_clamped > maxDuty) dc_clamped = maxDuty;
      
      float angle = ((dc_clamped - minDuty) / (maxDuty - minDuty)) * 360.0;
      
      Serial.print("[MODO: PWM] \t Duty: ");
      Serial.print(dc * 100.0, 1);
      Serial.print("% \t| Angulo: ");
      Serial.print(angle, 1);
      Serial.println(" graus");
    }
  } 
  else {
    // Timeout absoluto - Sem onda quadrada. O chip voltou ou continua analogico por falha do I2C
    int analogVal = analogRead(AS5600_OUT_PIN);
    
    float angle = ((float)analogVal / 1023.0) * 360.0;
    
    Serial.print("[MODO: ANALOGICO] \t V_Raw: ");
    Serial.print(analogVal);
    Serial.print("/1023 \t| Angulo: ");
    Serial.print(angle, 1);
    Serial.println(" graus");
  }

  // Refresha a tela 5x por segundo
  delay(200);
}