# Arquitetura do Sistema — EB-15

## 1. Visão Geral

```
┌─────────────────────────────────────────────────────────────────┐
│                        REDE Wi-Fi LOCAL                         │
│                     (IP estático: 192.168.1.100)                │
│                                                                 │
│  ┌──────────┐    HTTP/REST     ┌───────────────────────────┐    │
│  │ Browser  │◄────────────────►│      ESP32-S3             │    │
│  │ ou App   │                  │  (Supervisor Central)     │    │
│  └──────────┘                  │                           │    │
│                                │  • Wi-Fi + HTTP Server    │    │
│  ┌──────────┐    HTTP/REST     │  • API REST               │    │
│  │Dispositivo│◄───────────────►│  • FK / IK / Planner      │    │
│  │ Externo  │                  │  • Motor Control J1–J3    │    │
│  └──────────┘                  │  • Serial Master          │    │
│                                │  • Recipe Engine          │    │
│                                │  • Persistência (NVS)     │    │
│                                │  • Telemetria / Logs      │    │
│                                └──────────┬────────────────┘    │
│                                           │                     │
│                                    UART Serial                  │
│                                   (115200 baud)                 │
│                                           │                     │
│                                ┌──────────┴────────────────┐    │
│                                │      Arduino Uno          │    │
│                                │  (Executor Subordinado)   │    │
│                                │                           │    │
│                                │  • Serial Slave           │    │
│                                │  • Segment Queue          │    │
│                                │  • Motor Control J4–J6    │    │
│                                │  • Homing J4–J6           │    │
│                                │  • Watchdog               │    │
│                                └───────────────────────────┘    │
└─────────────────────────────────────────────────────────────────┘
```

---

## 2. Topologia de Hardware

### 2.1 Barramentos de Comunicação

| Barramento | Dispositivos | Protocolo | Velocidade |
|---|---|---|---|
| Wi-Fi | ESP32-S3 ↔ Rede | TCP/IP (HTTP) | 802.11 b/g/n |
| UART | ESP32-S3 ↔ Arduino Uno | Serial binário | 115200 baud |
| GPIO (PWM/Step) | ESP32-S3 → TB6600 (J1–J3) | Pulso/Direção | Variável |
| GPIO (PWM/Step) | Arduino → TB6600 (J4–J6) | Pulso/Direção | Variável |

### 2.2 Mapa de GPIOs

#### ESP32-S3
| GPIO | Função | Notas |
|---|---|---|
| 1 | I2C SDA | SDA do TCA9548A #1 |
| 2 | I2C SCL | SCL do TCA9548A #1 |
| 4 | PUL_J1 | Pulso motor J1 |
| 5 | DIR_J1 | Direção motor J1 |
| 6 | PUL_J2 | Pulso motor J2 |
| 7 | DIR_J2 | Direção motor J2 |
| 8 | PUL_J3 | Pulso motor J3 |
| 3 | DIR_J3 | Direção motor J3 |
| 19 | UART RX | RX ← Arduino TX (com divisor tensão) |
| 20 | UART TX | TX → Arduino RX |
| 48 | NeoPixel | LED status RGB |

*Nota TCA9548A #1: Enc M1 (SDA6/SCL6), Enc M2 (SDA4/SCL4), Enc M3 (SDA2/SCL2).*

#### Arduino Uno
| Pino | Função | Notas |
|---|---|---|
| A2 | I2C SDA | SDA do TCA9548A #2 |
| A3 | I2C SCL | SCL do TCA9548A #2 |
| A4 | UART RX | ← ESP32 20/TX |
| A5 | UART TX | → ESP32 19/RX |
| 2 | PUL_J4 | Pulso motor J4 |
| 3 | DIR_J4 | Direção motor J4 |
| 4 | PUL_J5 | Pulso motor J5 |
| 5 | DIR_J5 | Direção motor J5 |
| 6 | PUL_J6 | Pulso motor J6 |
| 7 | DIR_J6 | Direção motor J6 |
| 8 | ENABLE | Enable Global Motores |

*Nota TCA9548A #2: Enc M4 (SDA6/SCL6), Enc M5 (SDA4/SCL4), Enc M6 (SDA2/SCL2).*

---

## 3. Módulos de Software

### 3.1 ESP32-S3 — Mapa de Módulos

```
┌─────────────────────────────────────────────────┐
│                  ESP32-S3 Firmware               │
│                                                  │
│  ┌──────────┐  ┌──────────┐  ┌──────────────┐   │
│  │  Wi-Fi   │  │  HTTP    │  │  API REST    │   │
│  │  Manager │  │  Server  │  │  Handlers    │   │
│  └────┬─────┘  └────┬─────┘  └──────┬───────┘   │
│       │              │               │           │
│  ┌────┴──────────────┴───────────────┴───────┐   │
│  │           Estado Global (Robot State)      │   │
│  └────┬──────────┬──────────┬────────────────┘   │
│       │          │          │                     │
│  ┌────┴────┐ ┌───┴────┐ ┌──┴─────────┐          │
│  │   FK    │ │   IK   │ │  Motion    │          │
│  │         │ │        │ │  Planner   │          │
│  └────┬────┘ └───┬────┘ └──┬─────────┘          │
│       │          │         │                     │
│  ┌────┴──────────┴─────────┴─────────┐           │
│  │    Trajectory Segmenter            │           │
│  └────┬──────────────────┬───────────┘           │
│       │                  │                       │
│  ┌────┴────┐      ┌──────┴───────┐               │
│  │ Motor   │      │  Serial      │               │
│  │ J1–J3   │      │  Protocol    │               │
│  │ (local) │      │  (to Uno)    │               │
│  └─────────┘      └──────────────┘               │
│                                                  │
│  ┌──────────┐  ┌──────────┐  ┌──────────────┐   │
│  │ Recipe   │  │ Persist  │  │  Telemetry   │   │
│  │ Engine   │  │ (NVS)    │  │  & Logs      │   │
│  └──────────┘  └──────────┘  └──────────────┘   │
└─────────────────────────────────────────────────┘
```

### 3.2 Arduino Uno — Mapa de Módulos

```
┌─────────────────────────────────────────────┐
│              Arduino Uno Firmware            │
│                                              │
│  ┌──────────────┐     ┌────────────────┐    │
│  │ Serial       │────►│  Command       │    │
│  │ Protocol     │     │  Dispatcher    │    │
│  └──────────────┘     └───────┬────────┘    │
│                               │             │
│  ┌────────────────────────────┴──────────┐  │
│  │         Segment Queue (FIFO)          │  │
│  └────────────────────────┬──────────────┘  │
│                           │                 │
│  ┌────────────────────────┴──────────────┐  │
│  │    Motor Controller J4–J6             │  │
│  │  (coordinated multi-axis stepping)    │  │
│  └───────────────────────────────────────┘  │
│                                              │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  │
│  │ Homing   │  │ Watchdog │  │ Status   │  │
│  │ J4–J6    │  │ Timer    │  │ Reporter │  │
│  └──────────┘  └──────────┘  └──────────┘  │
└─────────────────────────────────────────────┘
```

---

## 4. Máquina de Estados — Robô

```
                    ┌──────────┐
         ┌─────────│  BOOT    │
         │         └────┬─────┘
         │              │ inicialização OK
         │              ▼
         │         ┌──────────┐
         │    ┌───►│  IDLE    │◄──────────────┐
         │    │    └────┬─────┘               │
         │    │         │ comando HOME         │
         │    │         ▼                      │
         │    │    ┌──────────┐                │
         │    │    │ HOMING   │────── ok ──────┘
         │    │    └────┬─────┘       │
         │    │         │ ok          │ falha
         │    │         ▼             ▼
         │    │    ┌──────────┐  ┌──────────┐
         │    │    │  READY   │  │  FAULT   │
         │    └────│          │  │          │
         │         └────┬─────┘  └────┬─────┘
         │              │ move        │ reset
         │              ▼             │
         │         ┌──────────┐       │
         │         │ MOVING   │───────┘
         │         └────┬─────┘
         │              │ pause
         │              ▼
         │         ┌──────────┐
         │         │ PAUSED   │
         │         └────┬─────┘
         │              │ resume → MOVING
         │              │ stop → READY
         └──────────────┘
              fault (qualquer estado)
```

### 4.1 Estados

| Estado | Descrição | Transições permitidas |
|---|---|---|
| `BOOT` | Inicialização do sistema | → IDLE (sucesso) ou → FAULT (falha) |
| `IDLE` | Sistema ligado, não homed | → HOMING |
| `HOMING` | Executando sequência de homing | → READY (sucesso) ou → FAULT (falha) |
| `READY` | Homed e pronto para operar | → MOVING, → HOMING |
| `MOVING` | Executando movimento | → READY (fim), → PAUSED, → FAULT |
| `PAUSED` | Movimento pausado | → MOVING (resume), → READY (stop) |
| `FAULT` | Falha detectada | → IDLE (reset) |

### 4.2 Faults

| Código | Descrição |
|---|---|
| `FAULT_NONE` | Sem falha |
| `FAULT_SERIAL_TIMEOUT` | Comunicação serial perdida |
| `FAULT_LIMIT_EXCEEDED` | Limite de junta excedido |
| `FAULT_HOMING_FAILED` | Homing não completou |
| `FAULT_COLLISION` | Colisão detectada com obstáculo |
| `FAULT_ESTOP` | Parada de emergência |
| `FAULT_ARDUINO_FAULT` | Arduino reportou falha |
| `FAULT_WATCHDOG` | Watchdog timeout |

---

## 5. Fluxo de Dados — Movimento Típico

```
  Usuário (Browser/API)
       │
       ▼
  POST /api/move/joints {j1:30, j2:45, ...}
       │
       ▼
  ┌─────────────────┐
  │ API Handler     │ Valida limites, verifica estado
  └────────┬────────┘
           │
           ▼
  ┌─────────────────┐
  │ Motion Planner  │ Calcula perfil trapezoidal,
  │                 │ gera waypoints interpolados
  └────────┬────────┘
           │
           ▼
  ┌─────────────────┐
  │ Obstacle Check  │ Verifica colisão em cada waypoint
  └────────┬────────┘
           │
           ▼
  ┌─────────────────┐
  │ Segmenter       │ Divide em segmentos temporizados
  └────────┬────────┘
           │
     ┌─────┴──────┐
     │            │
     ▼            ▼
  J1–J3        J4–J6
  (local)     (serial → Arduino)
     │            │
     ▼            ▼
  TB6600       TB6600
  Drivers      Drivers
```

---

## 6. Rede

### 6.1 Configuração Padrão

| Parâmetro | Valor |
|---|---|
| SSID | Configurável (NVS) |
| Password | Configurável (NVS) |
| IP | 192.168.1.100 |
| Gateway | 192.168.1.1 |
| Subnet | 255.255.255.0 |
| Porta HTTP | 80 |

### 6.2 Modo AP (Fallback)

Se a conexão Station falhar após 3 tentativas:
- SSID: `EB15-Setup`
- Password: `eb15admin`
- IP: 192.168.4.1
- Permite reconfigurar credenciais Wi-Fi

---

## 7. Persistência

| Dado | Storage | Formato |
|---|---|---|
| Config Wi-Fi | NVS | Key-value |
| Limites de juntas | NVS | Key-value |
| Offsets de homing | NVS | Key-value |
| Parâmetros DH | NVS | Key-value |
| Receitas | SPIFFS | JSON files |
| Logs | RAM (circular) | Struct array |

---

## 8. Segurança Lógica

1. **Soft limits** — Verificação antes de cada movimento
2. **Velocity limits** — Velocidade máxima por junta
3. **Collision check** — Obstáculos verificados no planejador
4. **Communication watchdog** — Auto-stop se Arduino não responder
5. **State protection** — Movimentos só permitidos em estado READY
6. **API validation** — Payloads validados antes de processamento
7. **Homing gate** — Movimentos bloqueados até homing completo
