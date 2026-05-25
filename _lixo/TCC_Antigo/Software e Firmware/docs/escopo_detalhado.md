# Escopo do Projeto — Sistema de Controle e Interface Web para Robô EB-15 com ESP32-S3 + Arduino Uno

## 1. Visão geral do projeto

Este projeto tem como objetivo desenvolver a arquitetura completa de controle, supervisão, programação de receitas, interface web e integração em rede para um braço robótico EB-15 open source, impresso em PETG, controlado por um sistema distribuído composto por:

- **1x ESP32-S3** como controlador principal e supervisor do sistema
- **1x Arduino Uno** como executor subordinado de parte dos eixos
- **Rede Wi-Fi local**, com o robô conectado em modo cliente (`station mode`) usando **IP estático**
- **Interface web hospedada no ESP32-S3**
- **Endpoint de rede** para integração com dispositivos externos na mesma LAN

O sistema será projetado para oferecer uma experiência de uso inspirada em softwares de robôs colaborativos como o UR-10, adaptada às limitações e características do EB-15.

---

## 2. Premissas técnicas do projeto

### 2.1 Premissas mecânicas e de sensoriamento

O EB-15, nesta versão do projeto:

- **não utilizará encoders nas juntas**
- **não utilizará leitura de torque/corrente para realimentação fina**
- trabalhará com **controle em malha aberta**
- dependerá de:
  - homing confiável
  - calibração geométrica
  - mapeamento cinemático
  - preservação da referência lógica das juntas ao longo da operação

Isso significa que as posições do robô serão tratadas como:

- **posição articular estimada**
- **posição cartesiana estimada**

e não como posições medidas diretamente por sensores de junta.

### 2.2 Premissas de software

O projeto adotará deliberadamente uma abordagem de firmware **monolítica em dois arquivos `.ino`**:

- **1 arquivo `.ino` do ESP32-S3**
- **1 arquivo `.ino` do Arduino Uno**

Apesar de grandes, os códigos serão organizados internamente por:

- blocos bem definidos
- seções comentadas
- estruturas de dados
- máquinas de estado
- funções agrupadas por responsabilidade

A escolha por dois firmwares monolíticos visa:

- simplificar o ambiente de desenvolvimento
- facilitar upload e testes rápidos
- reduzir a complexidade inicial do projeto
- manter alta legibilidade, desde que o código seja rigidamente organizado

---

## 3. Objetivo da arquitetura

A arquitetura deve permitir que o robô:

- seja acessado por navegador através de um IP fixo na rede
- ofereça interface gráfica para operação e programação
- execute movimentos por juntas e por coordenadas cartesianas
- exponha a posição atual estimada em juntas e em espaço cartesiano
- receba comandos de dispositivos externos pela rede
- faça planejamento de trajetória
- aplique validação de limites e workspace
- considere obstáculos informados externamente
- execute receitas e sequências de processo
- mantenha sincronismo entre os 6 eixos sem gargalos de comunicação

---

## 4. Arquitetura geral do sistema

## 4.1 Visão de alto nível

```text
[PC / Tablet / Celular / Sistema Externo]
                 |
            Rede Wi-Fi LAN
                 |
       IP estático do ESP32-S3
                 |
   ------------------------------------
   |                |                 |
[Interface Web]   [REST API]      [WebSocket]
   |                |                 |
   -------- Camada Supervisora Central --------
                    ESP32-S3
   | UI | API | Planner | FK/IK | Estado | Receitas |
   | Obstáculos | Logs | Homing global | Sync |
   ---------------------------------------------
              |                         |
      Controle local J1-J3         UART mestre
              |                         |
         Drivers/motores            Arduino Uno
                                        |
                               Executor local J4-J6
                               + homing local + IO
                                        |
                                  drivers / motores
```

---

## 5. Estratégia principal da arquitetura

A lógica central do sistema será:

- o **ESP32-S3 é o cérebro do sistema**
- o **Arduino Uno é um executor subordinado**
- o **ESP não enviará passos em tempo real para o Arduino**
- o **ESP enviará segmentos de movimento já planejados**
- cada placa executará localmente sua parte da trajetória, em sincronismo temporal

Essa decisão é essencial para evitar:

- gargalo de UART
- dependência de streaming contínuo de passos
- jitter excessivo por comunicação serial
- perda de sincronismo por latência de rede ou de aplicação

---

## 6. Distribuição de responsabilidades

## 6.1 Responsabilidades do ESP32-S3

O ESP32-S3 será responsável por:

### Rede e interface
- conexão ao Wi-Fi em modo cliente
- operação com IP estático
- fallback para modo de provisionamento/manutenção
- servidor web da interface gráfica
- API REST para integração externa
- WebSocket para telemetria em tempo real

### Supervisão global
- máquina de estados global do robô
- coordenação da execução
- supervisão de fault, stop, pause e resume
- sincronização dos 6 eixos
- watchdog de comunicação com o Arduino

### Modelo do robô
- modelo cinemático do EB-15
- cinemática direta (FK)
- cinemática inversa (IK)
- checagem de alcançabilidade
- limites articulares
- limites cartesianos
- validação de orientação e TCP

### Planejamento
- geração de trajetórias
- movimentos por junta
- movimentos lineares cartesianos
- movimentos circulares
- blending entre pontos
- segmentação da trajetória
- sincronização temporal entre J1-J3 e J4-J6

### Ambiente e validação
- cadastro e gerenciamento de obstáculos
- checagem simplificada de colisão com regiões proibidas
- desvio básico de obstáculos
- validação prévia de receitas

### Dados e persistência
- armazenamento de parâmetros
- receitas/programas
- TCPs
- frames
- offsets
- configuração de rede
- limites
- logs e eventos

### Controle local
- execução direta dos motores J1, J2 e J3
- homing local dos eixos controlados pelo ESP
- leitura de fins de curso locais
- IOs locais de alto nível

---

## 6.2 Responsabilidades do Arduino Uno

O Arduino Uno será responsável por:

- receber comandos do ESP32-S3 via UART
- validar e interpretar o protocolo serial
- manter uma fila local de segmentos
- executar os movimentos de J4, J5 e J6
- executar homing local dos seus eixos
- ler fins de curso desses eixos
- responder com status e ACK/NACK
- parar em caso de timeout do mestre
- relatar faults locais

O Arduino Uno **não deve**:

- calcular FK
- calcular IK
- decidir trajetória
- interpretar comandos externos de rede
- armazenar receitas completas
- tomar decisões de navegação ou desvio

Ele deve ser tratado como um **executor remoto determinístico**.

---

## 7. Divisão dos eixos entre as placas

A divisão recomendada é:

- **ESP32-S3:** J1, J2, J3
- **Arduino Uno:** J4, J5, J6

### Justificativa

Essa divisão faz sentido porque:

- J1, J2 e J3 são os eixos que mais impactam a posição global do TCP
- o mesmo dispositivo que calcula a cinemática controla a parte principal da geometria do braço
- J4, J5 e J6 podem ser executados como um subconjunto subordinado
- reduz a dependência do Uno para decisões mais críticas de posicionamento global

---

## 8. Modelo operacional do robô

## 8.1 Conceito de estado estimado

Como o robô não terá encoders, o estado oficial do sistema será sempre um **estado estimado**.

Serão mantidas internamente:

- `joint_position_estimated`
- `cartesian_pose_estimated`
- `homed`
- `state_valid`
- `last_homing_time`
- `last_fault`
- `robot_mode`
- `motion_mode`
- `active_tcp`
- `active_frame`

### Regra fundamental

Se ocorrer qualquer evento que possa invalidar a referência, como:

- perda de passos
- colisão mecânica
- travamento de eixo
- acionamento inesperado de fim de curso
- deslocamento manual do braço
- falha de alimentação
- fault de driver

então o sistema deve marcar o estado como:

- `state_valid = false`

e exigir novo homing antes de liberar operação normal.

---

## 9. Rede e conectividade

## 9.1 Modo de operação principal

O modo principal será:

- **ESP32-S3 conectado ao Wi-Fi local em modo station**
- **IP estático**
- acesso ao robô por endereço fixo na rede

Exemplo conceitual:

- IP: `192.168.0.50`
- porta HTTP: `80`
- porta WebSocket: integrada ao servidor
- API REST: `/api/v1/...`

## 9.2 Modo de fallback

Caso o Wi-Fi configurado não esteja disponível, o sistema poderá entrar em:

- modo de provisionamento
- modo de manutenção
- modo AP temporário

Esse modo não será o principal de operação, apenas contingência.

---

## 10. Interface web

A interface web hospedada no ESP32-S3 deverá concentrar as funções principais de operação, programação e monitoramento.

## 10.1 Telas principais sugeridas

### Dashboard
- estado geral
- IP
- conexão com Arduino
- modo atual
- alarmes
- posição cartesiana estimada
- juntas estimadas
- TCP e frame ativos

### JOG
- jog por juntas
- jog cartesiano
- velocidades configuráveis
- passo incremental
- home
- stop / pause / resume

### Programação / Receitas
- criação de pontos
- edição de waypoints
- movimentos MoveJ / MoveL / MoveC
- waits
- IOs
- loops
- variáveis simples
- salvar / carregar receita

### Frames e Ferramentas
- configurar TCP
- configurar frame da base
- configurar frame da peça
- configurar frame de pallet
- offsets

### Obstáculos e Workspace
- cadastrar obstáculos simples
- visualizar regiões proibidas
- margens de segurança
- validação de rota

### Diagnóstico
- logs
- fila de execução
- fault history
- status serial
- estado de homing
- parâmetros do sistema

---

## 11. API externa do robô

O sistema terá um endpoint de rede para que outros dispositivos na mesma LAN possam se conectar ao robô.

## 11.1 Objetivos da API

Permitir que um sistema externo:

- consulte estado atual
- consulte juntas atuais estimadas
- consulte pose cartesiana estimada
- envie comando de homing
- envie movimentos articulares
- envie movimentos cartesianos
- solicite FK
- solicite IK
- envie obstáculos
- solicite planejamento com desvio
- acione stop, pause e resume

## 11.2 Estrutura da API

### Leitura de estado
- `GET /api/v1/state`
- `GET /api/v1/joints`
- `GET /api/v1/pose`
- `GET /api/v1/frames`
- `GET /api/v1/recipes`
- `GET /api/v1/logs`

### Comandos principais
- `POST /api/v1/home`
- `POST /api/v1/move/joint`
- `POST /api/v1/move/linear`
- `POST /api/v1/move/circular`
- `POST /api/v1/fk`
- `POST /api/v1/ik`
- `POST /api/v1/plan`
- `POST /api/v1/obstacles`
- `POST /api/v1/stop`
- `POST /api/v1/pause`
- `POST /api/v1/resume`

## 11.3 Exemplos conceituais de payload

### Movimento por juntas

```json
{
  "target_joints_deg": [10, -25, 40, 0, 30, 0],
  "speed_pct": 40,
  "accel_pct": 30,
  "blend_mm": 0
}
```

### Movimento cartesiano

```json
{
  "frame": "world",
  "tcp": "gripper_1",
  "target_pose": {
    "x": 320.0,
    "y": 120.0,
    "z": 180.0,
    "rx": 180.0,
    "ry": 0.0,
    "rz": 90.0
  },
  "speed_mm_s": 60,
  "accel_mm_s2": 120
}
```

### Planejamento com obstáculo

```json
{
  "start_mode": "current_estimated",
  "motion_type": "linear",
  "target_pose": {
    "x": 280.0,
    "y": 160.0,
    "z": 210.0,
    "rx": 180.0,
    "ry": 0.0,
    "rz": 0.0
  },
  "obstacles": [
    {
      "id": "box_1",
      "type": "box",
      "frame": "world",
      "x": 230,
      "y": 120,
      "z": 120,
      "sx": 100,
      "sy": 80,
      "sz": 140,
      "margin": 20
    }
  ],
  "execute": true
}
```

---

## 12. Canal de telemetria em tempo real

Além da API REST, o sistema terá um canal de telemetria em tempo real via WebSocket.

### Objetivo
Evitar polling excessivo da interface e permitir atualização fluida de:

- juntas estimadas
- pose cartesiana estimada
- estado do robô
- fila de execução
- fault/alarm
- progresso de receita
- heartbeat
- estado do link com Arduino

### Endpoint sugerido
- `/ws/state`

---

## 13. Planejamento de movimento

## 13.1 Tipos de movimento a suportar

O sistema deverá suportar:

- movimento articular (`MoveJ`)
- movimento linear cartesiano (`MoveL`)
- movimento circular (`MoveC`)
- movimentos incrementais
- retorno para home
- movimento com blending
- movimentos em frame da base
- movimentos em frame da ferramenta
- movimentos em frame de usuário/peça

## 13.2 Estratégia de planejamento

O ESP32-S3 será responsável por:

1. receber o comando
2. validar o estado do robô
3. validar limites
4. calcular FK/IK conforme necessário
5. validar alcançabilidade
6. verificar regiões proibidas
7. considerar obstáculos cadastrados
8. gerar trajetória
9. quebrar a trajetória em segmentos sincronizados
10. enfileirar execução local e remota

## 13.3 Estratégia de desvio de obstáculos

Na primeira versão, os obstáculos serão tratados como primitivas simples:

- caixa
- cilindro
- esfera

A estratégia recomendada será em camadas:

### Camada 1 — tentativa direta
Tenta executar a trajetória direta planejada.

### Camada 2 — desvio simples
Caso haja conflito, usar lógica simples de desvio, por exemplo:

- subir para Z seguro
- deslocar lateralmente
- aproximar por cima
- descer ao alvo

### Camada 3 — fallback de planejamento discreto
Se necessário, aplicar uma estratégia mais conservadora de planejamento simplificado em grade ou pontos intermediários.

---

## 14. Comunicação ESP32-S3 ↔ Arduino Uno

## 14.1 Filosofia da comunicação

A comunicação serial entre as placas não deve ser usada para transmitir passo a passo em tempo real.

Ela deve ser usada para transmitir:

- comandos de alto nível
- segmentos de trajetória
- parâmetros
- status
- faults
- handshakes e sincronização

## 14.2 Protocolo recomendado

Formato conceitual do frame serial:

```text
SOF | version | msg_type | seq | payload_len | payload | crc16
```

## 14.3 Tipos de mensagens recomendados

- `HELLO`
- `PING`
- `SET_CONFIG`
- `HOME_AXES`
- `QUEUE_SEGMENT`
- `START_PLAN`
- `PAUSE`
- `RESUME`
- `STOP`
- `CLEAR_QUEUE`
- `GET_STATUS`
- `STATUS`
- `FAULT`
- `ACK`
- `NACK`

## 14.4 Regras principais do protocolo

- toda mensagem crítica deve ter confirmação
- segmentos devem possuir número de sequência
- o Arduino deve informar espaço livre da fila
- o ESP só inicia o plano quando ambos estiverem prontos
- em timeout ou corrupção de pacote, deve ocorrer parada segura
- o Arduino nunca toma decisões de planejamento

---

## 15. Estratégia de sincronismo

## 15.1 Conceito

O ESP32-S3 calculará a trajetória dos 6 eixos e a dividirá em pequenos segmentos temporizados.

Para cada segmento:

- o ESP executa localmente os deltas de J1-J3
- o Arduino recebe e executa os deltas de J4-J6
- ambos executam com a mesma referência temporal de segmento

## 15.2 Benefícios

Essa abordagem:

- reduz tráfego serial
- evita gargalo de comunicação
- melhora o sincronismo
- permite pausa/retomada mais controladas
- facilita inspeção de fila
- permite execução determinística do lado do Arduino

---

## 16. Máquina de estados global do robô

Estados sugeridos:

- `BOOT`
- `NET_CONNECTING`
- `NET_READY`
- `UNO_CONNECTING`
- `IDLE_NOT_HOMED`
- `HOMING`
- `IDLE_READY`
- `PLANNING`
- `EXECUTING`
- `PAUSED`
- `STOPPING`
- `FAULT`

### Regras gerais

- nenhum movimento normal é permitido em `IDLE_NOT_HOMED`
- em `FAULT`, comandos de movimento devem ser bloqueados
- `PAUSE` preserva o contexto do plano atual quando possível
- `STOP` aborta execução e pode invalidar o estado estimado
- perda de comunicação com o Arduino deve levar a `FAULT`

---

## 17. Segurança lógica e consistência

Mesmo sem segurança funcional industrial, o sistema deve implementar uma camada de segurança lógica mínima.

## 17.1 Regras obrigatórias

- não executar se não estiver homed
- não executar se `state_valid = false`
- validar limites articulares antes de qualquer comando
- validar pose cartesiana antes de aceitar movimento
- bloquear execução se houver obstáculo inválido ou inconsistência geométrica
- permitir `stop` em qualquer estado de movimento
- invalidar estado em fault crítico
- exigir re-home após eventos que comprometam referência

## 17.2 Limites a suportar

- limites de junta
- limites de workspace
- regiões proibidas
- orientação máxima da ferramenta
- velocidade máxima por junta
- aceleração máxima por junta
- velocidade máxima do TCP
- payload configurado manualmente

---

## 18. Persistência e armazenamento

O ESP32-S3 será o ponto central de armazenamento do sistema.

## 18.1 Dados persistentes

- SSID e configuração de rede
- IP estático
- parâmetros de conexão
- offsets de junta
- parâmetros de homing
- TCPs
- frames
- limites
- payload
- presets de velocidade
- receitas
- obstáculos salvos
- logs resumidos
- preferências da UI

## 18.2 Organização lógica de dados

### Configuração
Parâmetros do sistema e rede.

### Calibração
Offsets, homing, parâmetros geométricos.

### Operação
TCP, frames, payload, limites, presets.

### Programas
Receitas e listas de etapas.

### Diagnóstico
Falhas, contadores, últimos eventos.

---

## 19. Estrutura de firmware monolítico

## 19.1 Arquivo do ESP32-S3

Nome sugerido:

- `eb15_esp32_supervisor.ino`

### Estrutura interna sugerida do arquivo

```text
1. Includes e defines globais
2. Configuração de pinos
3. Estruturas e enums
4. Variáveis globais
5. Configuração de rede
6. Configuração do servidor web
7. Handlers da API REST
8. Handlers do WebSocket
9. Persistência e armazenamento
10. Modelo cinemático
11. FK
12. IK
13. Frames e TCP
14. Obstáculos e workspace
15. Planejador de trajetória
16. Segmentador de movimento
17. Controle local J1-J3
18. Protocolo serial com Arduino
19. Máquina de estados global
20. Gerenciador de receitas
21. Logs e diagnósticos
22. Rotinas de homing
23. Setup()
24. Loop() principal cooperativo
```

## 19.2 Arquivo do Arduino Uno

Nome sugerido:

- `eb15_uno_executor.ino`

### Estrutura interna sugerida do arquivo

```text
1. Includes e defines globais
2. Configuração de pinos
3. Estruturas e enums
4. Variáveis globais
5. Parser do protocolo serial
6. CRC e validação de pacotes
7. Fila de segmentos
8. Executor de segmentos J4-J6
9. Rotinas de homing
10. Leitura de fins de curso
11. Controle de IO local
12. Máquina de estados local
13. Fault handling
14. Heartbeat / status
15. Setup()
16. Loop() principal cooperativo
```

---

## 20. Filosofia de implementação dos dois `.ino`

Mesmo sendo monolíticos, os dois códigos devem seguir as seguintes regras:

- funções agrupadas por domínio
- seções bem marcadas com comentários grandes
- nomes padronizados
- enums para estados e mensagens
- structs para dados de pose, juntas, segmento e fault
- evitar lógica espalhada
- evitar duplicação desnecessária
- usar máquina de estados clara
- separar “planejamento” de “execução”
- separar “estado estimado” de “comando desejado”

---

## 21. Estruturas de dados principais

## 21.1 Dados do robô

```text
JointState
- j[6]
- valid
- homed
- timestamp

PoseState
- x, y, z
- rx, ry, rz
- frame_id
- valid
- timestamp
```

## 21.2 Dados de movimento

```text
MotionCommand
- type
- target_joints
- target_pose
- speed
- accel
- blend
- frame
- tcp
```

```text
MotionSegment
- segment_id
- duration_ms
- delta_j1
- delta_j2
- delta_j3
- delta_j4
- delta_j5
- delta_j6
- profile_id
- flags
```

## 21.3 Dados de obstáculo

```text
Obstacle
- id
- type
- frame
- x, y, z
- sx, sy, sz
- radius
- height
- margin
- enabled
```

## 21.4 Dados de receita

```text
RecipeStep
- step_type
- motion_type
- joints_target
- pose_target
- io_action
- wait_ms
- comment
```

---

## 22. Fluxos principais do sistema

## 22.1 Fluxo de boot

1. ligar sistema
2. inicializar variáveis
3. carregar configurações persistentes
4. conectar ao Wi-Fi
5. aplicar IP estático
6. subir interface web e API
7. abrir canal serial com Arduino
8. fazer handshake com Arduino
9. entrar em `IDLE_NOT_HOMED` ou `IDLE_READY`, conforme contexto

## 22.2 Fluxo de homing

1. usuário solicita homing
2. sistema entra em `HOMING`
3. ESP faz homing de J1-J3
4. Arduino faz homing de J4-J6
5. offsets são aplicados
6. estado estimado é zerado/referenciado
7. sistema marca `homed = true`
8. sistema entra em `IDLE_READY`

## 22.3 Fluxo de movimento cartesiano

1. usuário/API envia pose alvo
2. sistema verifica `state_valid`
3. sistema verifica `homed`
4. sistema calcula IK
5. sistema valida juntas resultantes
6. sistema verifica limites/obstáculos
7. sistema gera trajetória
8. sistema segmenta trajetória
9. sistema carrega filas locais/remotas
10. sistema executa
11. sistema atualiza estado estimado
12. sistema retorna a `IDLE_READY`

## 22.4 Fluxo de receita

1. receita é carregada
2. cada etapa é lida
3. movimento/IO/wait é validado
4. planner prepara execução
5. executor roda
6. progressos e faults são logados
7. término retorna a `IDLE_READY`

---

## 23. Funções de software previstas no escopo

Este escopo contempla, no mínimo, as seguintes classes de função:

### Operação manual
- JOG por juntas
- JOG cartesiano
- passo incremental
- home
- stop / pause / resume

### Programação
- salvar receitas
- carregar receitas
- editar waypoints
- loops simples
- waits
- IOs
- presets de velocidade

### Modelo do robô
- FK
- IK
- frames
- TCP
- payload manual
- limites

### Planejamento
- MoveJ
- MoveL
- MoveC
- blending
- validação de workspace
- desvio simples de obstáculos

### Monitoramento
- posição articular estimada
- posição cartesiana estimada
- estado atual
- faults
- logs
- fila de execução
- conexão com Arduino

### Integração externa
- REST API
- WebSocket
- leitura remota
- comando remoto
- envio de obstáculos
- solicitação de planejamento

---

## 24. Limitações assumidas nesta versão

Este projeto, nesta etapa, **não contempla**:

- medição real das juntas por encoder
- detecção real de colisão por torque
- controle em força
- hand guiding real
- freedrive real
- replanejamento dinâmico avançado em tempo real
- certificação de segurança funcional
- sincronismo industrial hard real-time entre múltiplos controladores

Esses itens poderão ser estudados em versões futuras.

---

## 25. Resultado esperado desta arquitetura

Ao final da implementação dessa estrutura, espera-se que o sistema permita:

- controlar o EB-15 por navegador
- operar o robô em rede local por IP fixo
- programar receitas simples e intermediárias
- expor posição estimada em juntas e em coordenadas cartesianas
- integrar dispositivos externos via API
- receber comandos em juntas ou em pose
- validar workspace e obstáculos simples
- sincronizar os 6 eixos com arquitetura distribuída
- manter uma base sólida para evolução futura

---

## 26. Diretriz final de implementação

A implementação deverá seguir a seguinte lógica central:

- **ESP32-S3 = supervisor, UI, API, cinemática, planejamento, persistência e controle local de J1-J3**
- **Arduino Uno = executor subordinado, fila local, homing e controle local de J4-J6**
- **comunicação serial baseada em segmentos de trajetória**
- **estado do robô tratado como estimado**
- **interface web e integração externa centralizadas no ESP**
- **firmwares monolíticos, porém rigidamente organizados**

Esse documento define o **escopo estrutural do sistema** e servirá como base para o desenho detalhado e posterior implementação do projeto.