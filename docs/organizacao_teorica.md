# Organização da Fundamentação Teórica

> Estrutura proposta para o Capítulo 2 do TCC — do mais geral ao mais específico.

---

## 2.1 Robótica Industrial e Manipuladores

### 2.1.1 Robôs Manipuladores — Classificação e Anatomia

Contextualização geral da robótica industrial: tipos de manipuladores (articulado, SCARA, cartesiano, Delta), graus de liberdade (DOF), espaço de trabalho e cadeia cinemática. Posicionar o EB-15 (articulado, 6-DOF) e o UR-10 (cobot articulado) dentro da taxonomia consolidada de robôs manipuladores, diferenciando plataformas industriais de plataformas experimentais.

### 2.1.2 Projeto Mecânico de Braços Robóticos de Baixo Custo

Abordagem open-source na robótica: uso de impressão 3D, peças em alumínio e componentes padronizados para construção de protótipos funcionais. Panorama de projetos abertos (BCN3D Moveo, Thor, EB-15) e suas contribuições para a democratização da robótica educacional e experimental. Discutir trade-offs entre custo, rigidez estrutural e precisão em relação a plataformas industriais.

### 2.1.3 Reduções Mecânicas — Engrenagens Planetárias

Princípio de funcionamento de trens epicicloidais (sol, planeta, anel). Relação de redução, amplificação de torque, eficiência mecânica e backlash. Contextualizar o papel das caixas de redução planetárias nas juntas do EB-15 e como influenciam diretamente o torque disponível e a precisão angular do protótipo.

---

## 2.2 Cinemática de Manipuladores

### 2.2.1 Representação Espacial — Matrizes de Transformação Homogênea

Fundamentos de rotação (matrizes R 3×3), translação e composição de transformações em coordenadas homogêneas (matrizes 4×4). Apresentar os sistemas de coordenadas utilizados em robótica e como a multiplicação de matrizes de transformação permite mapear a posição e orientação relativa entre os elos de uma cadeia cinemática.

### 2.2.2 Parâmetros de Denavit-Hartenberg (DH)

Convenção DH padrão: definição dos quatro parâmetros (d, a, α, θ) e procedimento de atribuição para uma cadeia cinemática serial. Apresentar como os parâmetros são extraídos da geometria do robô e como definem univocamente a relação entre juntas consecutivas. Relacionar com a tabela DH do EB-15 codificada no firmware.

### 2.2.3 Cinemática Direta (FK — Forward Kinematics)

Cálculo da pose do efetuador final (posição + orientação) a partir dos ângulos das juntas por encadeamento das matrizes DH. Apresentar o procedimento matemático geral e sua implementação computacional. Discutir a extração de ângulos de Euler (roll-pitch-yaw) a partir da matriz de rotação resultante.

### 2.2.4 Cinemática Inversa (IK — Inverse Kinematics)

Problema inverso: dada uma pose cartesiana desejada, determinar os ângulos das juntas. Apresentar métodos analíticos (solução fechada para geometrias específicas) e métodos numéricos/iterativos (CCD — Cyclic Coordinate Descent, Jacobiano transposto, Newton-Raphson). Discutir singularidades, múltiplas soluções, espaço de trabalho alcançável e as implicações de cada abordagem em hardware com recursos limitados.

### 2.2.5 Tool Center Point (TCP)

Breve descrição do conceito de TCP (ponto central da ferramenta), frame da base e frame da ferramenta como referências fundamentais para programação de robôs e para a representação da pose no espaço cartesiano.

---

## 2.3 Planejamento de Trajetória e Controle de Movimento

### 2.3.1 Planejamento no Espaço de Juntas vs. Espaço Cartesiano

Diferenças fundamentais entre movimentação no espaço de juntas (MoveJ — interpolação angular direta) e no espaço cartesiano (MoveL — trajetória linear do TCP com conversão via IK). Apresentar critérios de seleção: suavidade de movimento, previsibilidade da trajetória no espaço operacional e custo computacional.

### 2.3.2 Perfis de Velocidade — Trapezoidal e S-Curve

Conceito de perfil de velocidade para controle de aceleração e desaceleração em trajetórias robóticas. Apresentar o perfil trapezoidal (aceleração constante, velocidade de cruzeiro, desaceleração constante) como abordagem fundamental, e o perfil S-curve como evolução para suavidade mecânica. Discutir parâmetros de configuração e impacto na vida útil dos componentes mecânicos.

### 2.3.3 Segmentação Temporal de Trajetórias

Técnica de divisão de movimentos longos em micro-segmentos com duração fixa (time slicing). Apresentar o conceito de taxa de interpolação, fila de segmentos (buffer circular) e a importância da sincronização temporal entre controladores distribuídos para garantir fluidez de movimento.

### 2.3.4 Coordenação Multi-Eixo

Breve apresentação da interpolação de Bresenham adaptada para coordenação de múltiplos eixos motorizados e da sincronização temporal entre eixos com acoplamentos mecânicos distintos.

---

## 2.4 Atuadores e Eletrônica de Potência

### 2.4.1 Motores de Passo — Princípios e Características

Princípio de funcionamento de motores de passo (full step, half step, micro-stepping). Curvas de torque vs. velocidade, holding torque, detent torque. Apresentar as especificações tipicas do NEMA 17 e discutir as limitações inerentes (perda de torque em alta velocidade, resonância mecânica, aquecimento) que impactam o desempenho de protótipos robóticos.

### 2.4.2 Drivers de Motor de Passo — TB6600

Arquitetura do driver TB6600: ponte H com controle de corrente constante, interface Pulso/Direção (PUL/DIR), configuração de microstepping via DIP switches. Relação entre a resolução de microstepping configurada e a precisão angular resultante (steps/degree).

### 2.4.3 Controle em Malha Aberta vs. Malha Fechada

Conceitos de controle open-loop (comando direto sem retroalimentação) e closed-loop (com sensores de posição como encoders). Comparar os trade-offs: custo, complexidade, robustez à perda de passos, precisão e repetibilidade. Posicionar a escolha de malha aberta do projeto e suas implicações experimentais.

---

## 2.5 Sistemas Embarcados para Controle Robótico

### 2.5.1 Microcontroladores — ESP32-S3 e ATmega328P

Apresentação dos dois microcontroladores utilizados: ESP32-S3 (dual-core Xtensa LX7, 512KB SRAM, Wi-Fi, Bluetooth) e ATmega328P/Arduino Uno (8-bit AVR, 2KB SRAM, 32KB Flash). Comparar recursos computacionais, periféricos e justificar a divisão de responsabilidades no sistema.

### 2.5.2 Interrupções e Temporização em Sistemas de Tempo Real

Conceito de ISR (Interrupt Service Routines) e hardware timers para geração precisa de sinais em sistemas embarcados. Apresentar o uso de seções críticas para proteção de dados compartilhados entre ISR e loop principal. Discutir a importância da temporização para geração de pulsos de controle de motores de passo em frequências da ordem de kHz.

### 2.5.3 Comunicação UART e Protocolo Binário

Breve apresentação da interface UART (camada física) e dos princípios de projeto de protocolos binários estruturados para comunicação entre microcontroladores: framing com bytes delimitadores, verificação de integridade via CRC e mecanismos de confirmação (ACK/NACK).

### 2.5.4 Arquitetura Mestre-Escravo

Breve conceituação do paradigma mestre-escravo em controle distribuído: divisão entre um nó decisor (que planeja e coordena) e nós executores (que recebem e realizam comandos), com mecanismos de watchdog para garantia de conectividade.

### 2.5.5 Persistência de Dados em Microcontroladores

Breve apresentação das técnicas de armazenamento não volátil disponíveis em microcontroladores: NVS (Non-Volatile Storage) para pares chave-valor e sistemas de arquivos leves (SPIFFS) para armazenamento de arquivos, com suas limitações de ciclos de escrita.

---

## 2.6 IoT e Interface de Operação

### 2.6.1 Internet das Coisas (IoT) — Conceitos e Aplicações na Robótica

Definição de IoT, arquiteturas de conectividade em dispositivos embarcados e o papel da IoT na evolução da automação industrial (Indústria 4.0). Apresentar como a integração de robôs a redes locais amplia possibilidades de monitoramento, controle remoto e interoperabilidade entre dispositivos.

### 2.6.2 API REST e Interface Web em Sistemas Embarcados

Breve apresentação dos princípios REST aplicados a dispositivos de recursos limitados e do conceito de interfaces web embarcadas (IHM leves / dashboards supervisórios) servidas diretamente pelo microcontrolador, sem dependências externas.

---

## 2.7 Segurança Lógica em Sistemas Robóticos

### 2.7.1 Segurança Lógica — Soft Limits, Watchdog e Parada de Emergência

Mecanismos de proteção por software em robôs: limites de juntas (soft limits), limites de velocidade, watchdog de comunicação, parada de emergência (E-Stop) e transições de estado protegidas. Discutir a importância dessas camadas de segurança em protótipos experimentais de baixo custo que carecem das proteções mecânicas e redundâncias de sistemas industriais.

---

## 2.8 Validação Experimental em Robótica

### 2.8.1 Métricas de Desempenho — Precisão, Repetibilidade e Tempo de Resposta

Definições conforme a norma ISO 9283: precisão posicional (accuracy), repetibilidade (repeatability), tempo de estabilização (settling time). Apresentar metodologias de mensuração e análise estatística utilizadas na comparação de desempenho entre plataformas robóticas, fundamentando os critérios selecionados para a comparação entre EB-15 e UR-10.

---

## Mapa Visual da Estrutura

```
Fundamentação Teórica
│
├── 2.1 Robótica (3 seções)           ← Contexto geral e mecânica
│   ├── Classificação de robôs
│   ├── Projeto open-source / baixo custo
│   └── Reduções planetárias
│
├── 2.2 Cinemática (5 seções)         ← Fundamento matemático
│   ├── Transformações homogêneas
│   ├── Parâmetros DH
│   ├── Cinemática Direta (FK)
│   ├── Cinemática Inversa (IK)
│   └── TCP (breve)
│
├── 2.3 Trajetória (4 seções)         ← Planejamento de movimento
│   ├── Joint-space vs. task-space
│   ├── Perfis de velocidade
│   ├── Segmentação temporal
│   └── Coordenação multi-eixo (breve)
│
├── 2.4 Atuadores (3 seções)          ← Hardware de acionamento
│   ├── Motores de passo / NEMA 17
│   ├── Drivers TB6600
│   └── Malha aberta vs. fechada
│
├── 2.5 Embarcados (5 seções)         ← Plataformas e controle
│   ├── ESP32-S3 e ATmega328P
│   ├── ISR e temporização
│   ├── UART e protocolo binário (breve)
│   ├── Mestre-escravo (breve)
│   └── Persistência NVS/SPIFFS (breve)
│
├── 2.6 IoT e Interface (2 seções)    ← Conectividade e operação
│   ├── IoT na robótica / Indústria 4.0
│   └── REST API + IHM embarcada (breve)
│
├── 2.7 Segurança (1 seção)           ← Proteção lógica
│   └── Soft limits, watchdog, E-Stop
│
└── 2.8 Validação (1 seção)           ← Métricas experimentais
    └── ISO 9283, precisão, repetibilidade
```
