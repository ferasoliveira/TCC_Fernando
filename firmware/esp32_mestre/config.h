/**
 * ============================================================================
 *               EB-15 Robotic Arm — Mestre (ESP32-S3)
 *                       CONFIGURAÇÃO DO ROBÔ
 * ============================================================================
 * Este arquivo centraliza todas as constantes mecânicas, geométricas, elétricas
 * e limites de junta do braço robótico de 6 eixos EB15.
 * Altere estes valores para adaptar o firmware a modificações no hardware.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// 1. PINAGEM ELÉTRICA (Mapeamento físico de hardware)
// ============================================================================

// --- Motores da Base (J1, J2, J3) ---
#define PUL_J1  4    // Pino de pulso para junta 1
#define DIR_J1  5    // Pino de direção para junta 1
#define PUL_J2  6    // Pino de pulso para junta 2
#define DIR_J2  7    // Pino de direção para junta 2
#define PUL_J3  8    // Pino de pulso para junta 3
#define DIR_J3  3    // Pino de direção para junta 3

// --- Barramento I2C para Encoders AS5600 (Mestre) ---
#define I2C_SDA_PIN  1  // Pino SDA do barramento I2C principal
#define I2C_SCL_PIN  2  // Pino SCL do barramento I2C principal
#define TCA9548A_ADDR 0x70 // Endereço I2C do multiplexador TCA9548A #1

// --- Conexão Serial UART com Arduino Uno (Escravo) ---
#define UART_RX_PIN  19  // Pino RX da Serial2
#define UART_TX_PIN  20  // Pino TX da Serial2
#define SERIAL2_BAUD 115200 // Baudrate para comunicação entre as placas

// --- Sinal Físico de Trigger de Sincronismo ---
#define TRIGGER_PIN  4   // Pino de trigger digital de sincronização sub-microssegundo

// --- Indicador Visual (NeoPixel) ---
#define LED_PIN  48  // GPIO do NeoPixel LED embutido no ESP32 S3
#define NUM_LEDS 1   // Número de LEDs RGB endereçáveis

// ============================================================================
// 2. PARÂMETROS GEOMÉTRICOS E MECÂNICOS DO ROBÔ
// ============================================================================

// --- Dimensões dos Elos (Milímetros - mm) ---
// Utilizado nos cálculos de cinemática direta e inversa
#define LINK_L1  150.0  // Altura da base ao ombro (eixo J1 a J2)
#define LINK_L2  200.0  // Comprimento do braço (eixo J2 a J3)
#define LINK_L3  200.0  // Comprimento do antebraço (eixo J3 a J4)
#define LINK_L4   0.0   // Deslocamento do punho (se houver)
#define LINK_L5   0.0   // Deslocamento da garra
#define LINK_L6   80.0  // Comprimento da garra/punho ao atuador final (TCP)

// --- Relações de Redução Mecânica (Transmissão) ---
// Relação de redução de cada caixa de engrenagem / polias para cada junta
// Exemplo: se o motor gira 5 vezes para dar 1 volta na junta, a redução é 5.0
const float REDUCTION_J1 = 1.0;
const float REDUCTION_J2 = 1.0;
const float REDUCTION_J3 = 1.0;
const float REDUCTION_J4 = 1.0;
const float REDUCTION_J5 = 1.0;
const float REDUCTION_J6 = 1.0;

// --- Resolução Elétrica dos Motores (Passos por Volta) ---
// Passos nativos do motor (geralmente 200 passos/volta, ou seja, 1.8 graus)
#define MOTOR_STEPS_J1 200
#define MOTOR_STEPS_J2 200
#define MOTOR_STEPS_J3 200
#define MOTOR_STEPS_J4 200
#define MOTOR_STEPS_J5 200
#define MOTOR_STEPS_J6 200

// --- Configuração de Micropassos dos Drivers (TB6600 e A4988) ---
// Configurado fisicamente por chaves (TB6600) ou jumpers (CNC Shield)
// Exemplo: 4 significa modo micropasso 1/4 (800 passos por volta para motor de 200)
#define MICROSTEP_J1 4
#define MICROSTEP_J2 4
#define MICROSTEP_J3 4
#define MICROSTEP_J4 4
#define MICROSTEP_J5 4
#define MICROSTEP_J6 4

// --- Relação Geral de Passos por Grau Mecânico ---
// Calculado automaticamente baseado nas reduções e micropassos:
// Passos/Grau = (Passos_Motor * Micropassos * Relacao_Reducao) / 360.0
const float STEPS_PER_DEG[6] = {
  (MOTOR_STEPS_J1 * MICROSTEP_J1 * REDUCTION_J1) / 360.0,
  (MOTOR_STEPS_J2 * MICROSTEP_J2 * REDUCTION_J2) / 360.0,
  (MOTOR_STEPS_J3 * MICROSTEP_J3 * REDUCTION_J3) / 360.0,
  (MOTOR_STEPS_J4 * MICROSTEP_J4 * REDUCTION_J4) / 360.0,
  (MOTOR_STEPS_J5 * MICROSTEP_J5 * REDUCTION_J5) / 360.0,
  (MOTOR_STEPS_J6 * MICROSTEP_J6 * REDUCTION_J6) / 360.0
};

// ============================================================================
// 3. LIMITES DE MOVIMENTAÇÃO (Segurança por Software / Soft Limits)
// ============================================================================

// --- Limites Angulares Mínimos (Graus - deg) ---
const float LIMIT_MIN_DEG[6] = {
  -170.0, // Junta 1
  -45.0,  // Junta 2
  -120.0, // Junta 3
  -180.0, // Junta 4
  -90.0,  // Junta 5
  -360.0  // Junta 6
};

// --- Limites Angulares Máximos (Graus - deg) ---
const float LIMIT_MAX_DEG[6] = {
  170.0,  // Junta 1
  180.0,  // Junta 2
  120.0,  // Junta 3
  180.0,  // Junta 4
  90.0,   // Junta 5
  360.0   // Junta 6
};

// --- Limites Dinâmicos de Velocidade (Graus por Segundo - deg/s) ---
const float MAX_SPEED_DEG_S[6] = {
  45.0,  // J1
  45.0,  // J2
  45.0,  // J3
  90.0,  // J4
  90.0,  // J5
  90.0   // J6
};

// ============================================================================
// 4. PARÂMETROS DO CONTROLADOR DE MOVIMENTO
// ============================================================================

// --- Tempo de Ciclo e Frequência do Planejador ---
#define SEG_SLICE_MS  50   // Período de cada fatia de trajetória no interpolador (ms)
#define CONTROL_HZ   200  // Frequência do laço de controle de malha fechada local (Hz)
#define TIMER_PERIOD_US (1000000 / CONTROL_HZ) // Período de interrupção (5000 us)

#endif // CONFIG_H
