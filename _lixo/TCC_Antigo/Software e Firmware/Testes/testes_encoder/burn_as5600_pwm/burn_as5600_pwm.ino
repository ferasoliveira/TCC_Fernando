/*
 * ============================================================================
 *  EB-15 Robotic Arm — AS5600 PWM BURN (Arduino Uno)
 * ============================================================================
 *  CUIDADO: ESTE CÓDIGO REALIZA UMA OPERAÇÃO FÍSICA E PERMANENTE NO CHIP.
 * 
 *  Este código permite:
 *  1. [p] Testar temporariamente a saída PWM na RAM do AS5600.
 *  2. [B] Queimar a configuração para sempre na memória OTP (One-Time-Programmable).
 *     * O AS5600 só aceita até 3 "burns" durante toda a sua vida útil!
 * 
 *  Conexões Arduino Uno -> AS5600:
 *  - 5V  -> VDD5V (Alimentação obrigatória de 5V para queimar OTP)
 *  - GND -> GND
 *  - A4  -> SDA
 *  - A5  -> SCL
 *  - DIR -> GND (Para girar no sentido natural)
 * ============================================================================
 */

#include <Wire.h>

#define AS5600_ADDR 0x36
#define CONF_REG_MSB 0x07
#define CONF_REG_LSB 0x08
#define BURN_REG 0xFF

// Endereço do registrador ZMCO que checa quantas vezes o OTP já foi queimado.
#define ZMCO_REG 0x00 

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  delay(1000); // Aguarda serial estabilizar
  
  Serial.println("\n\n=======================================================");
  Serial.println("  *** EB-15 AS5600: OTP BURN PARA SAIDA PWM ***");
  Serial.println("=======================================================");
  Serial.println("[AVISO 1] Certifique-se de que o VDD5V esta recebendo 5 Volts!");
  Serial.println("[AVISO 2] A operacao [B]urn e IRREVERSIVEL e limitada (Max 3).");
  
  // Checa quantas vezes já foi queimado:
  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(ZMCO_REG);
  byte err = Wire.endTransmission();
  if (err != 0) {
    Serial.println("\n[ERRO FATAL] AS5600 nao detectado no I2C (Endereço 0x36). Verifique A4, A5, VCC e GND.");
    while(1);
  }
  
  Wire.requestFrom(AS5600_ADDR, 1);
  if (Wire.available()) {
    byte zmco = Wire.read();
    Serial.print("-> ZMCO (Vezes que o chip ja foi queimado): ");
    Serial.println(zmco);
    if (zmco >= 3) {
      Serial.println("[AVISO] O limite de 3 escritas na memoria OTP ja foi atingido neste chip!");
    }
  }

  Serial.println("\nCONTROLES (digite a letra no monitor serial e de ENTER):");
  Serial.println(" [p] -> Simular saida PWM temporariamente na RAM (Seguro)");
  Serial.println(" [B] -> EXECUTAR QUEIMA PERMANENTE [BURN] PARA PWM");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == 'p' || cmd == 'P') {
      Serial.println("\nEnviando configuracao temporaria para PWM...");
      
      // OUTS está no registrador 0x08, bits [5:4].
      // PWM = 0b01 -> ou seja, valor 0x10.
      Wire.beginTransmission(AS5600_ADDR);
      Wire.write(CONF_REG_LSB);
      Wire.write(0x10); 
      byte err = Wire.endTransmission();
      
      if (err == 0) {
        Serial.println("[SUCESSO] Configuracao temporaria escrita. O pino OUT agora eh PWM (115Hz).");
        Serial.println("OBS: Se voce desligar a energia, o chip voltara a ser Analogico.");
      } else {
        Serial.println("[ERRO] Falha ao comunicar I2C.");
      }
    }
    
    else if (cmd == 'B') {
      Serial.println("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      Serial.println(" INICIANDO OPERACAO DE QUEIMA OTP (BURN) ");
      Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      
      // Passo 1: Tem que certificar de escrever o registro CONF na RAM primeiro
      Wire.beginTransmission(AS5600_ADDR);
      Wire.write(CONF_REG_LSB);
      Wire.write(0x10);
      Wire.endTransmission();
      delay(10); // Pausa rapida p chip processar
      
      // Passo 2: Mandar o comando Burn_Settings de 0x80 no registrador 0xFF
      Wire.beginTransmission(AS5600_ADDR);
      Wire.write(BURN_REG);
      Wire.write(0x80); 
      byte err = Wire.endTransmission();
      
      if (err == 0) {
        Serial.println("[BURN SUCESSO] Comando de queima fisica enviado.");
        Serial.println("-> Reinicie o Arduino e cheque se o valor de ZMCO subiu 1 ponto!");
        Serial.println("-> Desligue a energia do chip AS5600 depois para validar o PWM definitivo.");
      } else {
        Serial.println("[ERRO] Nao foi possivel escrever o comando de queima I2C.");
      }
    }
  }
}
