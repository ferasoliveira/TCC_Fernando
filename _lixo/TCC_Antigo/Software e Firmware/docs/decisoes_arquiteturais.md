# Decisões Arquiteturais — EB-15

## ADR-01: Firmware Monolítico (.ino)

**Contexto:** O projeto precisa de firmware para ESP32-S3 e Arduino Uno.

**Decisão:** Um arquivo `.ino` por placa, sem fragmentação em múltiplos `.h`/`.cpp`.

**Razão:**
- Simplifica build e flash (Arduino IDE)
- Evita problemas de include path
- Facilita versionamento (diff de um arquivo)
- Mantém todo o contexto visível

**Mitigação da complexidade:**
- Seções bem demarcadas com comentários-bloco
- Enums e structs no topo
- Funções agrupadas por responsabilidade
- Índice de seções no início do arquivo

---

## ADR-02: Protocolo Serial Binário com CRC8

**Contexto:** Comunicação UART entre ESP32 e Arduino para comandos e segmentos de movimento.

**Alternativas consideradas:**
1. Texto/ASCII (ex: `"MOVE J4 100\n"`)
2. JSON via serial
3. Protocolo binário com CRC

**Decisão:** Protocolo binário com framing (START/END), CRC8, ACK/NACK.

**Razão:**
- Eficiência: payloads menores, parsing mais rápido
- Confiabilidade: CRC detecta corrupção, ACK garante entrega
- Determinismo: tamanho fixo de frames facilita buffer management no Arduino Uno (2KB RAM)
- O Arduino Uno tem apenas 2KB de RAM — JSON ou strings longas são inviáveis

---

## ADR-03: Controle Open-Loop (Sem Encoders)

**Contexto:** O EB-15 não possui encoders nas juntas.

**Decisão:** Controle por contagem de passos (open-loop) com homing obrigatório.

**Consequências:**
- Posição é estimada, não medida
- Perda de passos (stall, sobrecarga) causa drift silencioso
- Homing é a única forma de recalibrar
- Limites de velocidade conservadores para minimizar perda de passos

**Mitigações:**
- Perfil trapezoidal (aceleração/desaceleração suave)
- Velocidades conservadoras
- Homing obrigatório no boot
- Opção de re-home a qualquer momento
- Futuro: adicionar encoders para feedback

---

## ADR-04: Perfil Trapezoidal de Velocidade

**Contexto:** Como gerar movimentos suaves sem jerk excessivo nos steppers.

**Alternativas:**
1. Velocidade constante (bang-bang)
2. Perfil trapezoidal
3. S-curve (jerk limitado)

**Decisão:** Perfil trapezoidal.

**Razão:**
- Simples de implementar
- Suficiente para steppers NEMA
- Evita perda de passos por aceleração brusca
- S-curve é mais complexo sem ganho significativo neste hardware

---

## ADR-05: JSON para API, Binário para Serial

**Contexto:** Dois canais de comunicação com requisitos diferentes.

**Decisão:**
- API REST → JSON (humano-legível, fácil de debugar)
- Serial ESP↔Arduino → Binário (eficiente, confiável)

**Razão:** Cada canal tem audiência diferente. A API fala com browsers/apps que esperam JSON. O serial fala com um microcontrolador com 2KB RAM que precisa de eficiência.

---

## ADR-06: Execução Baseada em Segmentos

**Contexto:** Como enviar movimentos do ESP para o Arduino.

**Alternativas:**
1. Streaming de passos em tempo real
2. Envio de segmentos pré-computados
3. Envio de posição final apenas

**Decisão:** Segmentos pré-computados em fila.

**Razão:**
- Streaming exige latência ultra-baixa na serial (arriscado)
- Posição final não permite sincronização temporal
- Segmentos permitem pré-buffering, tolerância a atrasos, e sincronização por duração

**Formato:** Cada segmento contém passos por eixo (J4/J5/J6) + duração em ms.

---

## ADR-07: Web Assets Embutidos no Firmware (PROGMEM)

**Contexto:** Como servir a interface web pelo ESP32-S3.

**Alternativas:**
1. Arquivos separados, carregados via SPIFFS/LittleFS
2. HTML/CSS/JS embutidos no `.ino` usando strings `PROGMEM`

**Decisão:** Os assets web (HTML, CSS e JS) serão definidos como arrays de caracteres (C-strings) usando `PROGMEM` diretamente no código-fonte do ESP32.

**Razão:**
- Requiremento do usuário de que a UI seja "hospedada pelo próprio ESP" sem depender de uploads de arquivos separados via SPIFFS.
- Garante que ao gravar o `.ino`, a interface já está imediatamente funcional.
- Menos chance de falha do usuário ao omitir o "Sketch Data Upload".
- O ESP32-S3 tem memória Flash suficiente para armazenar esses assets de forma confortável.

---

## ADR-08: IP Estático com Fallback AP

**Contexto:** Como configurar a rede do ESP32.

**Decisão:**
- Modo Station com IP estático (192.168.1.100)
- Se conexão falhar: modo AP (`EB15-Setup`)
- Credenciais persistidas em NVS

**Razão:**
- IP estático facilita acesso previsível
- AP fallback garante que o robô nunca fique inacessível
- NVS permite reconfiguração sem reflash

---

## ADR-09: FK Real, IK como Stub Inicial

**Contexto:** Cinemática para um manipulador 6-DOF requer solução analítica específica ou numérica.

**Decisão:**
- FK implementada com DH parameters (funcional com placeholders)
- IK implementada como stub numérico simplificado
- IK completa requer medidas reais do EB-15 e possivelmente solução analítica

**Razão:**
- FK é genérica e funciona com qualquer DH table
- IK analítica 6-DOF é específica por geometria do robô
- Stub numérico permite testar o fluxo de MoveL
- IK real será implementada na Fase 3

---

## ADR-10: Homing Obrigatório

**Contexto:** Sistema open-loop sem referência absoluta.

**Decisão:** O robô DEVE completar homing antes de aceitar comandos de movimento.

**Razão:**
- Sem homing, a posição estimada não tem referência
- Mover sem referência pode causar colisões mecânicas
- API rejeita movimentos com HTTP 409 se não homed
