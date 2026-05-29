/**
 * ============================================================================
 *               EB-15 Robotic Arm — Escravo (Arduino Uno)
 *                       CONFIGURAÇÃO DO ROBÔ
 * ============================================================================
 * Este arquivo centraliza todas as constantes mecânicas, geométricas, elétricas
 * e limites das juntas superiores J4, J5, J6 controladas pelo Arduino Uno.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// 1. PINAGEM ELÉTRICA (Mapeamento CNC Shield v3.1)
// ============================================================================

// --- Drivers das Juntas Superiores (J4, J5, J6) ---
#define PUL_J4  2  // Pino de passo da Junta 4 (CNC Shield X_STEP)
#define DIR_J4  3  // Pino de direção da Junta 4 (CNC Shield X_DIR -> CNC Shield Y_STEP é 3 na verdade, mas seguimos a tabela do usuário)
#define PUL_J5  4  // Pino de passo da Junta 5
#define DIR_J5  5  // Pino de direção da Junta 5
#define PUL_J6  6  // Pino de passo da Junta 6
#define DIR_J6  7  // Pino de direção da Junta 6

#define MOTORS_ENABLE 8 // Pino de habilitação geral dos motores (LOW = ativado)

// --- Barramento I2C para Encoders AS5600 (Escravo) ---
#define I2C_SDA_PIN A2 // Pino SDA dedicado conectado ao TCA9548A #2
#define I2C_SCL_PIN A3 // Pino SCL dedicado conectado ao TCA9548A #2
#define TCA9548A_ADDR 0x70 // Endereço I2C do multiplexador TCA9548A #2

// --- Conexão Serial UART SoftwareSerial com ESP32 S3 (Mestre) ---
#define UART_RX_PIN  A4 // Pino RX no Arduino (conectado ao TX do ESP32 via divisor resistivo)
#define UART_TX_PIN  A5 // Pino TX no Arduino (conectado ao RX do ESP32)
#define SERIAL_BAUD 115200 // Baudrate para recepção e transmissão de frames

// --- Pino de Trigger Digital de Sincronismo ---
#define TRIGGER_PIN  9 // Pino digital 9 para receber a interrupção PCINT de sincronismo

// ============================================================================
// 2. PARÂMETROS MECÂNICOS DO ROBÔ (Juntas J4, J5, J6)
// ============================================================================

// --- Relações de Redução Mecânica (Transmissão) ---
const float REDUCTION_J4 = 1.0;
const float REDUCTION_J5 = 1.0;
const float REDUCTION_J6 = 1.0;

// --- Resolução Elétrica dos Motores (Passos por Volta) ---
#define MOTOR_STEPS_J4 200
#define MOTOR_STEPS_J5 200
#define MOTOR_STEPS_J6 200

// --- Configuração de Micropassos dos Drivers (CNC Shield jumpers) ---
#define MICROSTEP_J4 4 // Micropasso 1/4
#define MICROSTEP_J5 4 // Micropasso 1/4
#define MICROSTEP_J6 4 // Micropasso 1/4

// --- Relação Geral de Passos por Grau Mecânico ---
const float STEPS_PER_DEG[3] = {
  (MOTOR_STEPS_J4 * MICROSTEP_J4 * REDUCTION_J4) / 360.0,
  (MOTOR_STEPS_J5 * MICROSTEP_J5 * REDUCTION_J5) / 360.0,
  (MOTOR_STEPS_J6 * MICROSTEP_J6 * REDUCTION_J6) / 360.0
};

// ============================================================================
// 3. LIMITES DE MOVIMENTAÇÃO (Segurança por Software / Soft Limits)
// ============================================================================

const float LIMIT_MIN_DEG[3] = { -180.0, -90.0, -360.0 }; // Limites J4, J5, J6 mínimos (deg)
const float LIMIT_MAX_DEG[3] = {  180.0,  90.0,  360.0 }; // Limites J4, J5, J6 máximos (deg)

#define MIN_STEP_INTERVAL_US 50 // Frequência máxima de pulso (20 kHz)
#define CONTROL_HZ   200  // Frequência do laço de controle de malha fechada local (Hz)

#endif // CONFIG_H
