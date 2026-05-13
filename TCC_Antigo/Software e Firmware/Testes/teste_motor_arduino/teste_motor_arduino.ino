/*
 * ============================================================================
 *  EB-15 Robotic Arm — Motor Test (Arduino UNO + CNC Shield v3)
 * ============================================================================
 *  HARDWARE:
 *  - Arduino UNO R3 + CNC Shield v3
 *  - 3x A4988 drivers (plugados nos slots X, Y, Z)
 *  - Full-step (sem jumpers de microstepping)
 *
 *  PINAGEM CNC Shield v3:
 *  ┌──────────┬──────────┬──────────┐
 *  │  Eixo    │  STEP    │  DIR     │
 *  ├──────────┼──────────┼──────────┤
 *  │  X (J1)  │  D2      │  D5      │
 *  │  Y (J2)  │  D3      │  D6      │
 *  │  Z (J3)  │  D4      │  D7      │
 *  └──────────┴──────────┴──────────┘
 *  Enable global: D8 (LOW = habilitado)
 *
 *  CONTROLE VIA SERIAL (115200 baud):
 *  - Comandos enviados pelo Monitor Serial do Arduino IDE
 *  - Menu interativo com seleção de junta, velocidade, aceleração e modo
 *
 *  MODOS DE OPERAÇÃO:
 *  1. Giro Contínuo  — Velocidade constante sem parar
 *  2. Giro Alternado  — Move por 6s e para
 *  3. Back-and-Forth  — 6s frente, pausa, 6s trás, pausa, repete
 * ============================================================================
 */

#include <AccelStepper.h>

// ============================================================================
//  PINAGEM — CNC Shield v3
// ============================================================================
#define STEP_X  2   // J1
#define DIR_X   5
#define STEP_Y  3   // J2
#define DIR_Y   6
#define STEP_Z  4   // J3
#define DIR_Z   7

#define EN_PIN  8   // Enable global (active LOW)

// ============================================================================
//  OBJETOS E VARIÁVEIS GLOBAIS
// ============================================================================
AccelStepper stepperJ1(AccelStepper::DRIVER, STEP_X, DIR_X);
AccelStepper stepperJ2(AccelStepper::DRIVER, STEP_Y, DIR_Y);
AccelStepper stepperJ3(AccelStepper::DRIVER, STEP_Z, DIR_Z);
AccelStepper* activeStepper = &stepperJ1;

int activeMotor  = 1;        // 1=J1, 2=J2, 3=J3
float curSpeed   = 500.0;    // Passos por segundo
float curAccel   = 200.0;    // Passos por segundo²
int curMode      = 0;        // 0=Idle, 1=Contínuo, 2=Alternado, 3=BackForth
bool running     = false;

// Controle de sequência (Modo 3 — Back & Forth)
int seqStep      = 0;        // 0=Fwd, 1=Pause, 2=Bwd, 3=Pause
unsigned long seqTimer = 0;

// ============================================================================
//  UTILITÁRIOS SERIAL
// ============================================================================
void printDivider() {
    Serial.println(F("════════════════════════════════════════"));
}

void printMenu() {
    Serial.println();
    printDivider();
    Serial.println(F("  EB-15 Motor Test — Arduino + CNC v3"));
    printDivider();
    Serial.print(F("  Motor ativo: J"));
    Serial.print(activeMotor);
    Serial.print(F("  |  Vel: "));
    Serial.print(curSpeed, 0);
    Serial.print(F(" p/s  |  Acel: "));
    Serial.print(curAccel, 0);
    Serial.println(F(" p/s²"));
    Serial.print(F("  Estado: "));
    Serial.println(running ? F("MOVIMENTANDO") : F("PARADO"));
    printDivider();
    Serial.println(F("  COMANDOS:"));
    Serial.println(F("  j1 / j2 / j3    — Selecionar junta"));
    Serial.println(F("  v<valor>         — Velocidade (ex: v800)"));
    Serial.println(F("  a<valor>         — Aceleração (ex: a300)"));
    Serial.println(F("  m1               — Modo Contínuo"));
    Serial.println(F("  m2               — Modo Alternado (6s)"));
    Serial.println(F("  m3               — Modo Back & Forth"));
    Serial.println(F("  go               — Iniciar teste"));
    Serial.println(F("  stop / s         — Parar motor"));
    Serial.println(F("  menu             — Exibir este menu"));
    printDivider();
}

void selectMotor(int m) {
    if (running) {
        activeStepper->stop();
        activeStepper->runToPosition();
        running = false;
        curMode = 0;
    }

    activeMotor = m;
    switch (m) {
        case 1: activeStepper = &stepperJ1; break;
        case 2: activeStepper = &stepperJ2; break;
        case 3: activeStepper = &stepperJ3; break;
    }
    Serial.print(F(">> Motor selecionado: J"));
    Serial.println(activeMotor);
}

void startMotor() {
    if (curMode == 0) {
        Serial.println(F(">> ERRO: Selecione um modo primeiro (m1, m2 ou m3)"));
        return;
    }

    activeStepper->setMaxSpeed(curSpeed);
    activeStepper->setAcceleration(curAccel);

    if (curMode == 1) {
        activeStepper->setSpeed(curSpeed);
    } else if (curMode == 2) {
        long steps = (long)(curSpeed * 6);
        activeStepper->move(steps);
    } else if (curMode == 3) {
        seqStep = 0;
        seqTimer = millis();
        long steps = (long)(curSpeed * 6);
        activeStepper->move(steps);
    }

    running = true;
    Serial.print(F(">> START J"));
    Serial.print(activeMotor);
    Serial.print(F(" | Vel: "));
    Serial.print(curSpeed, 0);
    Serial.print(F(" | Acel: "));
    Serial.print(curAccel, 0);
    Serial.print(F(" | Modo: "));
    Serial.println(curMode);
}

void stopMotor() {
    activeStepper->stop();
    activeStepper->runToPosition();
    running = false;
    curMode = 0;
    Serial.print(F(">> STOP J"));
    Serial.println(activeMotor);
}

// ============================================================================
//  PROCESSADOR DE COMANDOS SERIAL
// ============================================================================
void processCommand(String cmd) {
    cmd.trim();
    cmd.toLowerCase();

    if (cmd.length() == 0) return;

    if (cmd == "j1")        { selectMotor(1); }
    else if (cmd == "j2")   { selectMotor(2); }
    else if (cmd == "j3")   { selectMotor(3); }
    else if (cmd.startsWith("v")) {
        float val = cmd.substring(1).toFloat();
        if (val > 0) {
            curSpeed = val;
            Serial.print(F(">> Velocidade: "));
            Serial.print(curSpeed, 0);
            Serial.println(F(" p/s"));
            if (running && curMode == 1) {
                activeStepper->setMaxSpeed(curSpeed);
                activeStepper->setSpeed(curSpeed);
            }
        } else {
            Serial.println(F(">> ERRO: Valor inválido (ex: v800)"));
        }
    }
    else if (cmd.startsWith("a")) {
        float val = cmd.substring(1).toFloat();
        if (val > 0) {
            curAccel = val;
            Serial.print(F(">> Aceleração: "));
            Serial.print(curAccel, 0);
            Serial.println(F(" p/s²"));
            if (running) {
                activeStepper->setAcceleration(curAccel);
            }
        } else {
            Serial.println(F(">> ERRO: Valor inválido (ex: a300)"));
        }
    }
    else if (cmd == "m1") {
        curMode = 1;
        Serial.println(F(">> Modo: Contínuo"));
    }
    else if (cmd == "m2") {
        curMode = 2;
        Serial.println(F(">> Modo: Alternado (6s)"));
    }
    else if (cmd == "m3") {
        curMode = 3;
        Serial.println(F(">> Modo: Back & Forth"));
    }
    else if (cmd == "go") {
        startMotor();
    }
    else if (cmd == "stop" || cmd == "s") {
        stopMotor();
    }
    else if (cmd == "menu") {
        printMenu();
    }
    else {
        Serial.print(F(">> Comando desconhecido: "));
        Serial.println(cmd);
        Serial.println(F("   Digite 'menu' para ver os comandos."));
    }
}

// ============================================================================
//  SETUP
// ============================================================================
void setup() {
    Serial.begin(115200);

    // Enable pin — ativa os drivers (LOW = habilitado)
    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, LOW);

    // Configura largura mínima de pulso para A4988
    stepperJ1.setMinPulseWidth(5);
    stepperJ2.setMinPulseWidth(5);
    stepperJ3.setMinPulseWidth(5);

    // Valores iniciais
    stepperJ1.setMaxSpeed(curSpeed);
    stepperJ2.setMaxSpeed(curSpeed);
    stepperJ3.setMaxSpeed(curSpeed);
    stepperJ1.setAcceleration(curAccel);
    stepperJ2.setAcceleration(curAccel);
    stepperJ3.setAcceleration(curAccel);

    printMenu();
    Serial.println(F(">> Pronto. Digite um comando e pressione Enter."));
}

// ============================================================================
//  LOOP PRINCIPAL
// ============================================================================
void loop() {
    // Leitura de comandos serial (não-bloqueante)
    static String inputBuffer = "";
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            if (inputBuffer.length() > 0) {
                processCommand(inputBuffer);
                inputBuffer = "";
            }
        } else {
            inputBuffer += c;
        }
    }

    // Lógica de movimento
    if (running) {
        if (curMode == 1) {
            // Contínuo — velocidade constante
            activeStepper->runSpeed();
        }
        else if (curMode == 2) {
            // Alternado — move e para
            if (activeStepper->distanceToGo() != 0) {
                activeStepper->run();
            } else {
                running = false;
                curMode = 0;
                Serial.println(F(">> Movimento concluído (Modo Alternado)."));
            }
        }
        else if (curMode == 3) {
            // Back & Forth — loop contínuo
            if (seqStep == 0) {
                if (activeStepper->distanceToGo() != 0) {
                    activeStepper->run();
                } else {
                    seqStep = 1;
                    seqTimer = millis();
                    Serial.println(F("   [B&F] Pausa..."));
                }
            }
            else if (seqStep == 1) {
                if (millis() - seqTimer > 1000) {
                    long steps = (long)(curSpeed * 6);
                    activeStepper->move(-steps);
                    seqStep = 2;
                    Serial.println(F("   [B&F] Voltando..."));
                }
            }
            else if (seqStep == 2) {
                if (activeStepper->distanceToGo() != 0) {
                    activeStepper->run();
                } else {
                    seqStep = 3;
                    seqTimer = millis();
                    Serial.println(F("   [B&F] Pausa..."));
                }
            }
            else if (seqStep == 3) {
                if (millis() - seqTimer > 1000) {
                    long steps = (long)(curSpeed * 6);
                    activeStepper->move(steps);
                    seqStep = 0;
                    Serial.println(F("   [B&F] Indo..."));
                }
            }
        }
    }
}
