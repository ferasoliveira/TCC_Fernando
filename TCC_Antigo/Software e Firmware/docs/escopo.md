# Escopo do Projeto — EB-15 Braço Robótico

> Documento de escopo e contrato principal do sistema de controle distribuído.

---

## 1. Visão Geral

O sistema EB-15 é um braço robótico de **6 graus de liberdade (6-DOF)** controlado por uma arquitetura distribuída:

- **ESP32-S3** — Controlador principal (supervisor, servidor web, API, planejador de movimento, FK/IK, eixos J1–J3)
- **Arduino Uno** — Executor subordinado (eixos J4–J5–J6)

Os dois microcontroladores se comunicam via **serial UART** com protocolo estruturado.

---

## 2. Hardware

### 2.1 Controladores
| Componente | Função | Eixos |
|---|---|---|
| ESP32-S3 (DevKitC-1) | Supervisor, Wi-Fi, Web, API, J1–J3 | J1, J2, J3 |
| Arduino Uno | Executor subordinado, J4–J6 | J4, J5, J6 |

### 2.2 Drivers de Motor
- **6x TB6600** — Drivers de motor de passo (um por eixo)
- Sinais: PUL (pulso) + DIR (direção) por driver
- Microstepping configurável via DIP switches no TB6600

### 2.3 Pinagem ESP32-S3 (J1–J3)
| Sinal | GPIO |
|---|---|
| I2C SDA (TCA9548A #1) | 1 |
| I2C SCL (TCA9548A #1) | 2 |
| PUL_J1 | 4 |
| DIR_J1 | 5 |
| PUL_J2 | 6 |
| DIR_J2 | 7 |
| PUL_J3 | 8 |
| DIR_J3 | 3 |
| UART RX | 19 |
| UART TX | 20 |
| NeoPixel LED | 48 |

### 2.4 Pinagem Arduino Uno (J4–J6)
| Sinal | Pino |
|---|---|
| I2C SDA (TCA9548A #2) | A2 |
| I2C SCL (TCA9548A #2) | A3 |
| UART RX | A4 |
| UART TX | A5 |
| PUL_J4 | 2 |
| DIR_J4 | 3 |
| PUL_J5 | 4 |
| DIR_J5 | 5 |
| PUL_J6 | 6 |
| DIR_J6 | 7 |
| MOTORS_ENABLE | 8 |

### 2.5 Sensores
- **Sem encoders nas juntas** — O sistema trabalha com **estado estimado** (contagem de passos)
- Chaves de fim de curso para homing (a definir)
- O NeoPixel integrado do ESP32-S3 serve como indicador visual de status

### 2.6 Comunicação ESP ↔ Arduino
- Serial UART (TX/RX)
- ESP32 TX → Arduino RX
- ESP32 RX ← Arduino TX
- Baud rate: 115200
- Protocolo binário estruturado com framing, CRC, ACK/NACK

---

## 3. Arquitetura de Software

### 3.1 Restrição de Firmware
- **1 arquivo `.ino` para ESP32-S3** — monolítico, mas internamente bem estruturado
- **1 arquivo `.ino` para Arduino Uno** — monolítico, mas internamente bem estruturado
- Nenhuma lógica de firmware principal fora desses dois arquivos
- Assets web podem ser separados em arquivos auxiliares

### 3.2 Responsabilidades do ESP32-S3
1. Gerenciamento de Wi-Fi (Station mode, IP estático, fallback de manutenção)
2. Servidor HTTP (interface web)
3. API REST local
4. Estado global do robô
5. Parser e executor de receitas/programas
6. Modelo cinemático do robô (DH parameters)
7. Cinemática Direta (FK)
8. Cinemática Inversa (IK)
9. Frames e TCP (Tool Center Point)
10. Modelo de obstáculos simples
11. Planejador de movimento (MoveJ, MoveL, MoveC)
12. Segmentador de trajetória
13. Controle direto dos eixos J1–J3 (geração de pulsos)
14. Comunicação serial com Arduino (protocolo, fila, ACK)
15. Persistência de configuração (NVS/SPIFFS)
16. Telemetria e logs
17. Detecção e gerenciamento de faults

### 3.3 Responsabilidades do Arduino Uno
1. Parser do protocolo serial
2. Fila de segmentos de movimento
3. Execução local dos eixos J4–J6
4. Homing local dos eixos J4–J6
5. Watchdog de comunicação
6. Resposta de status
7. Fault handling local

### 3.4 Regra de ouro
> O Arduino **não planeja trajetória** e **não toma decisões de movimento**. Ele apenas executa segmentos que recebeu do ESP32-S3.

---

## 4. Protocolo Serial ESP ↔ Arduino

### 4.1 Características
- Protocolo binário com framing
- Start byte + comando + payload + CRC8 + end byte
- ACK/NACK para confirmação
- Heartbeat periódico
- Fila de segmentos no Arduino (buffer circular)
- Timeout e recuperação de erro

### 4.2 Comandos principais
| Comando | Direção | Descrição |
|---|---|---|
| SEGMENT | ESP → Arduino | Segmento de movimento para J4–J6 |
| STOP | ESP → Arduino | Parada imediata |
| HOME | ESP → Arduino | Iniciar homing de J4–J6 |
| STATUS_REQ | ESP → Arduino | Solicitar status |
| STATUS_RSP | Arduino → ESP | Resposta de status |
| ACK | Bidirecional | Confirmação de recebimento |
| NACK | Bidirecional | Erro no recebimento |
| HEARTBEAT | Bidirecional | Sinal de vida |
| FAULT | Arduino → ESP | Notificação de falha |

---

## 5. API REST Local

### 5.1 Configuração de Rede
- Wi-Fi Station mode
- IP estático configurável
- Porta HTTP: 80
- Fallback: modo AP para manutenção/provisionamento

### 5.2 Endpoints mínimos
| Método | Endpoint | Descrição |
|---|---|---|
| GET | `/api/status` | Estado geral do robô |
| GET | `/api/joints` | Posições das 6 juntas |
| GET | `/api/pose` | Pose cartesiana atual (TCP) |
| POST | `/api/home` | Comando de homing |
| POST | `/api/stop` | Parada imediata |
| POST | `/api/pause` | Pausar movimento |
| POST | `/api/resume` | Retomar movimento |
| POST | `/api/move/joints` | Movimento por juntas |
| POST | `/api/move/linear` | Movimento cartesiano linear |
| POST | `/api/fk` | Consulta de FK (sem mover) |
| POST | `/api/ik` | Consulta de IK (sem mover) |
| POST | `/api/obstacles` | Definir/consultar obstáculos |
| POST | `/api/plan` | Planejar trajetória (sem executar) |
| POST | `/api/recipe` | Enviar e executar receita |
| GET | `/api/logs` | Últimos logs/faults |
| GET | `/api/config` | Leitura de configuração |
| POST | `/api/config` | Atualização de configuração |

---

## 6. Interface Web

### 6.1 Hospedagem
- Servida pelo ESP32-S3 (SPIFFS ou embutida no firmware)
- HTML + CSS + JS vanilla
- Sem dependências externas (CDN)

### 6.2 Componentes mínimos
1. Dashboard de status geral
2. Display de posição das juntas (J1–J6)
3. Display de pose cartesiana (X, Y, Z, Rx, Ry, Rz)
4. Controles de Jog (botões +/- por junta)
5. Botões: Home, Stop, Pause, Resume
6. Área de teste de API/comandos
7. Log viewer simples

---

## 7. Planejamento de Movimento

### 7.1 Tipos de movimento
| Tipo | Descrição | Status inicial |
|---|---|---|
| MoveJ | Movimento joint-space | Implementar |
| MoveL | Movimento linear cartesiano | Implementar |
| MoveC | Movimento circular | Stub/estrutura |

### 7.2 Estratégia
- Interpolação no joint-space (MoveJ) ou task-space (MoveL)
- Segmentação em intervalos de tempo fixo
- Perfil trapezoidal de velocidade
- Distribuição de segmentos: J1–J3 local, J4–J6 via serial
- Sincronização temporal entre ESP e Arduino

### 7.3 Obstáculos
- Primitivas simples: esfera, caixa (AABB)
- Checagem de colisão no planejador
- Estratégia simples de desvio (waypoints adicionais)
- Não é collision-free planning completo nesta fase

---

## 8. Receitas / Programas

- Lista sequencial de movimentos e ações
- Formato JSON simples
- Armazenamento em SPIFFS ou recebimento via API
- Execução sequencial com pause/resume
- Suporte a loops simples e delays

---

## 9. Persistência

- NVS (Non-Volatile Storage) para configuração rápida
- SPIFFS para receitas e logs
- Dados persistidos: configuração de rede, limites de juntas, offsets de homing, parâmetros DH, receitas

---

## 10. Segurança Lógica

- Limites de juntas (soft limits)
- Verificação de velocidade máxima
- Emergency stop (via API ou botão físico futuro)
- Timeout de comunicação serial
- Watchdog no Arduino
- Transições de estado protegidas
- Validação de payloads na API

---

## 11. Restrições Críticas

1. **Sem encoders** — O sistema confia exclusivamente em contagem de passos (open-loop)
2. **Homing obrigatório** — O robô DEVE completar homing antes de operar
3. **Arduino não decide** — Apenas executa segmentos recebidos
4. **Dois firmwares monolíticos** — Um `.ino` por placa, bem estruturado internamente
5. **Sem streaming serial em tempo real** — Segmentos são enviados em lotes com ACK
6. **Rede local apenas** — Sem cloud, sem internet obrigatória
7. **Evolução incremental** — A base deve ser extensível sem reescrever do zero

---

## 12. Evolução Futura (Fora do Escopo Atual)

- Encoders e feedback real
- Visão computacional
- Controle de gripper/end-effector
- Integração com ROS2
- Controle de força/impedância
- Calibração automática
- UI avançada com simulação 3D
- Receitas com branching e condições
