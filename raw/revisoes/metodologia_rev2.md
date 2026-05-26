# Relatório de Revisão: Capítulo 3 (Metodologia e Desenvolvimento) --- Revisão 2

Este relatório foi gerado seguindo as diretrizes estritas do workflow `/revisor-tcc`. Ele realiza uma auditoria técnica, acadêmica, gramatical e tipográfica profunda do conteúdo do **Capítulo 3 (Metodologia e Desenvolvimento)** do TCC do braço robótico de 6 eixos **EB15**, gravado no arquivo `TCC_escrito/3-metodologia.tex`.

---

## 1. Mapeamento de Contexto
- **Arquivo Auditado:** `TCC_escrito/3-metodologia.tex` (linhas 1 a 219).
- **Entradas da Wiki Utilizadas:** `wiki/index.md` (especificações gerais), `wiki/diagrama_sistema.md` (particionamento de juntas J1--J3/J4--J6, drivers TB6600, placa CNC Shield v3.1, encoders AS5600 e barramentos) e `wiki/fluxo_operacao.md` (handshake UART e INT0 trigger síncrono).
- **Referencial Bibliográfico Comparado:** `TCC_escrito/ref.bib` (verificação de chaves e conformidade de citações).

---

## 2. Critérios de Avaliação (Rigor Acadêmico Extremo)

### 2.1. Gramática e Normas ABNT (Peso 1.0)
* **Avaliação:** O capítulo mantém uma redação científica impessoal bem estruturada com transições coerentes de ideias. No entanto, o rigor absoluto identificou desvios gramaticais, erros de digitação e falhas graves nas normas de siglas da ABNT:
  1. **Erros de Digitação e Grafia (Linhas 121, 178, 195):**
     - **Linha 121:** Duplicação de palavra `pino digital digital 2` (erro de digitação double-word).
     - **Linha 178:** Incongruência de número gramatical `sincronia com o Nós Mestre` (deveria ser "sincronia com o Nó Mestre").
     - **Linha 195:** Erro ortográfico `elotrotécnica` (deveria ser "eletrotécnica").
  2. **Siglas não explicadas em seu primeiro uso no capítulo:**
     - `CNC` (Linha 34): Citada em "CNC Shield v3.1" sem expansão (*Computer Numerical Control* / Controle Numérico Computadorizado).
     - `DIP` (Linha 27): Citada em "DIP switches" sem expansão (*Dual In-line Package*).
     - `CMOS` (Linha 70): Citada sem expansão (*Complementary Metal-Oxide-Semiconductor*).
     - `TTL` (Linha 70): Citada sem expansão (*Transistor-Transistor Logic*).
     - `JSON` (Linha 83): Citada sem expansão (*JavaScript Object Notation*).
     - `XML` (Linha 83): Citada sem expansão (*eXtensible Markup Language*).
     - `HTTP` (Linha 148): Citada sem expansão (*HyperText Transfer Protocol*).
  3. **Siglas utilizadas antes de suas respectivas definições (Out of Order):**
     - A sigla `UART` é utilizada primeiro na linha 6 (`interconexão UART`), mas só é definida na linha 68 (`UART (Universal Asynchronous Receiver-Transmitter)`).
     - A sigla `GPIO` é utilizada primeiro na linha 54 (`pinos GPIO`), mas só é definida na linha 121 (`pino de saída digital rápido [...] (GPIO 4)`).
* **Penalidades Aplicadas:**
  - `-1.5` pontos pelos três erros ortográficos/digitação (`digital digital`, `o Nós Mestre` e `elotrotécnica`).
  - `-1.0` ponto pelo conjunto de siglas não explicadas em seu primeiro uso.
  - `-0.5` pontos pela inversão da ordem de apresentação de siglas (`UART` e `GPIO`).
* **Nota:** **7.0 / 10.0**

### 2.2. Citação e ref.bib (Peso 1.0)
* **Avaliação:** Em geral, o texto apresenta excelente suporte bibliográfico. Porém, há um parágrafo que faz afirmações técnicas complexas sobre o comportamento de hardware de um componente sem nenhuma citação:
  - **Parágrafo 12 (Linha 48):** Descreve detalhadamente o princípio Hall interno, resolução de ADC de 12 bits, as 4096 posições discretas e a fórmula de cálculo da resolução do circuito integrado comercial AS5600 sem atribuir a nenhuma referência (datasheet ou artigos como `tang2022research`).
* **Penalidades Aplicadas:**
  - `-1.0` ponto pelo parágrafo contendo descrição de modelagem e especificações de circuito integrado comercial sem fundamentação bibliográfica.
* **Nota:** **9.0 / 10.0**

### 2.3. Rigor Técnico e Alinhamento com o Hardware EB15 (Peso 1.0)
* **Avaliação:** Fidelidade técnica total à modelagem mecatrônica do braço EB15. O mapeamento de barramentos de controle de base e punho, reduções estruturais das engrenagens ($R_1$ a $R_5$), acoplamento do divisor resistivo calculando $3,33\text{ V}$ e a lógica INT0 de trigger síncrono estão perfeitos.
* **Refinamento de Alta Precisão (Sem penalidade):** Assim como no referencial teórico, na linha 48 cita-se a resolução do AS5600 como `0,087^{\circ}`. Pelo arredondamento matemático correto de $360^{\circ} / 4096 = 0,087890625^{\circ}$, deve-se adotar **$0,088^{\circ}$** para evitar truncamento inadequado em cálculos científicos.
* **Penalidades Aplicadas:** Nenhuma.
* **Nota:** **10.0 / 10.0**

### 2.4. Conformidade Científica e Estilo (Peso 1.0)
* **Avaliação:** O capítulo expõe de forma lógica as escolhas estruturais, contudo apresenta adjetivações e termos qualitativos informais incompatíveis com o rigor acadêmico estrito:
  - **Linha 18:** "poder computacional expressivo" (subjetivo) $\rightarrow$ "capacidade de processamento elevada".
  - **Linha 18:** "recursos ideais" (subjetivo) $\rightarrow$ "recursos adequados".
  - **Linha 18 e 141:** "algoritmos pesados" (informal) $\rightarrow$ "algoritmos computacionalmente intensivos" ou "algoritmos complexos".
  - **Linha 27:** "drivers industriais robustos" (qualitativo) $\rightarrow$ "drivers industriais".
  - **Linha 32:** "baixíssimo jitter" (superlativo) $\rightarrow$ "reduzido jitter".
  - **Linha 68:** "unir de forma limpa e direta" (informal) $\rightarrow$ "conectar diretamente".
  - **Linha 104:** "latência insignificante" (subjetivo) $\rightarrow$ "latência sub-milissegundo".
  - **Linha 162:** "carregada de forma instantânea" (informal) $\rightarrow$ "carregada dinamicamente".
  - **Linha 165:** "dados mecatrônicos complexos" (qualitativo) $\rightarrow$ "dados de telemetria".
  - **Linha 170:** "laço infinito simplificado" (qualitativo) $\rightarrow$ "laço infinito".
  - **Linha 172:** "ruído de fase temporal nulo" (fisicamente impreciso) $\rightarrow$ "ruído de fase temporal desprezível".
  - **Linha 201:** "desvios posicionais expressivos" (qualitativo) $\rightarrow$ "desvios posicionais elevados".
  - **Linha 209:** "condições severas de estresse" (informal) $\rightarrow$ "condições de alta carga de processamento".
* **Penalidades Aplicadas:**
  - `-1.0` ponto pelo uso recorrente de termos de intensidade, superlativos e adjetivações não quantitativas.
* **Nota:** **9.0 / 10.0**

### 2.5. Sintaxe e Tipografia LaTeX (Peso 1.0)
* **Avaliação:** O documento possui formatação de tabelas e equações adequadas. Contudo, há um **vazamento maciço de sintaxe Markdown no código LaTeX**:
  1. **Uso sistemático de asteriscos simples (`*`) para itálico:** Encontrado em `*jitter*` (linha 32, 126, 153), `*backlash*` (linha 50), `*XOR*` (linha 100), `*checksum*` (linha 116), `*Timer1* e *Timer2*` (linha 116), `*Falling Edge*` (linha 123), `*Teach-and-Play*` (linha 161) e `*main loop*` (linha 170). Em LaTeX, o correto é `\textit{}`.
  2. **Uso de crases/backticks (\`) para código monoespaçado:** Encontrado em `` `0xAA` `` (linha 116), `` `0x06` `` (linha 118, 121), `` `Wire` `` (linha 55), `` `Task_BaseControl` `` (linha 141), `` `Task_SerialComm` `` (linha 143), `` `Task_WiFi` `` (linha 146), `` `Task_WebServer` `` (linha 148), `` `Task_RTDE` `` (linha 150). Em LaTeX, deve-se usar `\texttt{}`.
  3. **Uso de asteriscos duplos (`**`) para negrito:** Encontrado em `**Arduino Uno**` (linha 32), `**Modo Jog Manual:**` (linha 160), `**Programação de Pontos:**` (linha 161), `**Configuração de Relações de Redução:**` (linha 162) e `**Acurácia Posicional Cartesiana em Malha Fechada:**` (linha 214). Em LaTeX, deve-se usar `\textbf{}`.
* **Penalidades Aplicadas:**
  - `-2.0` pontos pela presença maciça de sintaxe Markdown para itálico (`*`), negrito (`**`) e monoespaçado (\`), que invalidam a tipografia correta e geram poluição de caracteres literais no PDF compilado.
* **Nota:** **8.0 / 10.0**

---

## Média Ponderada Final do Capítulo 3: 8.6 / 10.0

---

## 3. Registro de Feedback (Auditoria Minuciosa)

### 3.1. Pontos Críticos (Erros que demandam correção imediata)

Abaixo estão listadas as linhas exatas do arquivo `TCC_escrito/3-metodologia.tex` com as respectivas correções propostas para substituição direta (*drop-in*):

#### 1. Correção de Sigla UART no primeiro uso e Estilo (Linha 6)
* **Original (linha 6):**
```latex
Na esfera de software, descreve-se a interconexão UART de baixo payload, a lógica de sincronização instantânea por trigger de hardware dedicado e as diretrizes do firmware sob o sistema operacional FreeRTOS no nó mestre e sob arquitetura bare-metal de baixo nível no nó escravo.
```
* **Substituição Recomendada:**
```latex
Na esfera de software, descreve-se a interconexão UART (\textit{Universal Asynchronous Receiver-Transmitter}) de reduzido payload, a lógica de sincronização instantânea por trigger de hardware dedicado e as diretrizes do firmware sob o sistema operacional FreeRTOS no nó mestre e sob arquitetura bare-metal no nó escravo.
```

#### 2. Definição de Siglas, Estilo e Adjetivos (Linhas 16--18)
* **Original (linhas 16--18):**
```latex
Esta unidade embarca a CPU Xtensa LX7 dual-core de 32 bits, operando a uma frequência de clock nominal de 240 MHz. O processador conta com uma Unidade de Ponto Flutuante (FPU) integrada por hardware, 512 KB de SRAM interna, 384 KB de ROM e coprocessador de ultrabaixo consumo (ULP). A conectividade de rede é garantida por transceptor de rádio integrado com suporte a Wi-Fi 802.11 b/g/n (2,4 GHz) e Bluetooth 5 (LE) \cite{kowsalyadevi2025comprehensive}.

A escolha do ESP32 S3 justifica-se por seu poder computacional expressivo e baixo custo financeiro, fornecendo recursos ideais para assumir o papel de coordenador global do robô manipulador. No Nó Mestre, são centralizados os cálculos matemáticos complexos de cinemática inversa (Equações \ref{eq:wrist_center} e \ref{eq:wrist_orientation}) e de interpolação geométrica em Curva-S de sete segmentos (Fases 1 a 7 descritas no Referencial Teórico).
```
* **Substituição Recomendada:**
```latex
Esta unidade embarca a CPU (\textit{Central Processing Unit}) Xtensa LX7 dual-core de 32 bits, operando a uma frequência de clock nominal de 240 MHz. O processador conta com uma Unidade de Ponto Flutuante (FPU) integrada por hardware, 512 KB de SRAM (\textit{Static Random Access Memory}) interna, 384 KB de ROM (\textit{Read-Only Memory}) e coprocessador de ultrabaixo consumo (ULP). A conectividade de rede é garantida por transceptor de rádio integrado com suporte a Wi-Fi 802.11 b/g/n (2,4 GHz) e Bluetooth 5 (LE) \cite{kowsalyadevi2025comprehensive}.

A escolha do ESP32 S3 justifica-se por sua capacidade de processamento elevada e baixo custo financeiro, fornecendo recursos adequados para assumir o papel de coordenador global do robô manipulador. No Nó Mestre, são centralizados os cálculos matemáticos de cinemática inversa (Equações \ref{eq:wrist_center} e \ref{eq:wrist_orientation}) e de interpolação geométrica em Curva-S de sete segmentos (Fases 1 a 7 descritas no Referencial Teórico).
```

#### 3. Definição de Sigla DIP e Estilo dos Drivers (Linha 27)
* **Original (linha 27):**
```latex
Devido às altas correntes exigidas pelos motores NEMA 23 e NEMA 17 da base do manipulador, o ESP32 S3 interliga-se a três drivers industriais robustos \textbf{TB6600}. [...] Para atenuar ressonâncias mecânicas locais, os DIP switches dos drivers TB6600 foram configurados no modo de micropasso de 1/16 (equivalente a 3200 passos por revolução completa do motor).
```
* **Substituição Recomendada:**
```latex
Devido às altas correntes exigidas pelos motores NEMA 23 e NEMA 17 da base do manipulador, o ESP32 S3 interliga-se a três drivers industriais \textbf{TB6600}. [...] Para atenuar ressonâncias mecânicas locais, os interruptores DIP (\textit{Dual In-line Package}) dos drivers TB6600 foram configurados no modo de micropasso de 1/16 (equivalente a 3200 passos por revolução completa do motor).
```

#### 4. Correção de Markdown para Negrito (**Arduino Uno**), Itálico (*jitter*) e Sigla CNC (Linhas 32--34)
* **Original (linhas 32--34):**
```latex
O Nó Escravo atua estritamente como um processador executor e aliviador de carga computacional, baseado no microcontrolador monolítico de 8 bits \textbf{AVR ATmega328P} operando em clock estável de 16 MHz no módulo **Arduino Uno**. [...] A ausência de sistemas operacionais complexos ou pilhas de rede confere ao ATmega328P um comportamento temporal previsível e determinístico de baixíssimo *jitter* de hardware \cite{okter2025ros}.

O Nó Escravo é acoplado diretamente a uma placa de expansão de barramento comercial \textbf{CNC Shield v3.1}, projetada especificamente para controle de eixos múltiplos. Esta shield simplifica o layout mecatrônico ao encapsular a pinagem de potência e controle lógico em um encapsulamento compacto. O Arduino Uno gerencia de forma direta as três juntas do punho e efetuador do manipulador EB15:
```
* **Substituição Recomendada:**
```latex
O Nó Escravo atua estritamente como um processador executor e aliviador de carga computacional, baseado no microcontrolador monolítico de 8 bits \textbf{AVR ATmega328P} operando em clock estável de 16 MHz no módulo \textbf{Arduino Uno}. [...] A ausência de sistemas operacionais complexos ou pilhas de rede confere ao ATmega328P um comportamento temporal previsível e determinístico de reduzido \textit{jitter} de hardware \cite{okter2025ros}.

O Nó Escravo é acoplado diretamente a uma placa de expansão de barramento comercial \textbf{CNC} (\textit{Computer Numerical Control}) \textbf{Shield v3.1}, projetada especificamente para controle de eixos múltiplos. Esta placa de expansão simplifica o layout mecatrônico ao encapsular a pinagem de potência e controle lógico em um encapsulamento compacto. O Arduino Uno gerencia de forma direta as três juntas do punho e efetuador do manipulador EB15:
```

#### 5. Adicionando Citação Obrigatória do Sensor AS5600, Correção de Arredondamento e Sigla GPIO (Linhas 46--48)
* **Original (linhas 46--48):**
```latex
Para mitigar a perda de passo mecânico e monitorar desvios posicionais decorrentes da flexibilidade e folgas inerentes à manufatura aditiva 3D do braço EB15, implementou-se uma malha fechada local de realimentação baseada em sensores magnéticos absolutos \textbf{AS5600} (ams OSRAM) instalados individualmente nas 6 juntas articuladas \cite{tang2022research}.

O circuito integrado AS5600 baseia-se em uma matriz interna de sensores de efeito Hall de alta sensibilidade que detecta a orientação espacial do vetor campo magnético gerado por um pequeno ímã de neodímio diametralmente magnetizado, o qual deve ser colado exatamente no centro geométrico do eixo rotativo físico móvel de cada elo articulado. A tensão interna diferencial induzida nos sensores Hall é processada e digitalizada por um conversor analógico-digital (conversor A/D) de alta precisão com resolução de 12 bits, gerando uma informação de saída angular absoluta digital de 4096 divisões discretas por volta de 360 graus, correspondente a uma resolução angular real de $0,087^{\circ}$ por divisão.
```
* **Substituição Recomendada:**
```latex
Para mitigar a perda de passo mecânico e monitorar desvios posicionais decorrentes da flexibilidade e folgas inerentes à manufatura aditiva 3D do braço EB15, implementou-se uma malha fechada local de realimentação baseada em sensores magnéticos absolutos \textbf{AS5600} (ams OSRAM) instalados individualmente nas 6 juntas articuladas \cite{tang2022research}.

O circuito integrado AS5600 baseia-se em uma matriz interna de sensores de efeito Hall de alta sensibilidade que detecta a orientação espacial do vetor campo magnético gerado por um pequeno ímã de neodímio diametralmente magnetizado, o qual deve ser colado exatamente no centro geométrico do eixo rotativo físico móvel de cada elo articulado. A tensão interna diferencial induzida nos sensores Hall é processada e digitalizada por um conversor analógico-digital com resolução de 12 bits, gerando uma informação de saída angular absoluta digital de 4096 divisões discretas por volta de 360 graus, correspondente a uma resolução angular real de $0,088^{\circ}$ por divisão \cite{tang2022research}.
```

#### 6. Definição da Sigla GPIO e Correção de Markdown para Itálico (*backlash*) (Linha 50--54)
* **Original (linhas 50--54):**
```latex
A leitura dessa telemetria ocorre via protocolo serial síncrono bidirecional \textbf{I\textsuperscript{2}C} (\textit{Inter-Integrated Circuit}) operando em modo de alta velocidade (\textit{Fast Mode} a 400 kHz). Uma inovação mecatrônica essencial no braço EB15 reside na fixação física dos sensores AS5600 diretamente nos eixos de rotação de saída das juntas estruturais impressas em 3D, após a etapa física de redução mecânica, e não nas partes traseiras dos eixos lineares internos dos motores de passo. Esta disposição física garante que a realimentação de controle de posição meça a rotação real do elo no espaço cartesiano, capturando e eliminando dos cálculos dinâmicos as folgas e perdas mecânicas (*backlash*) acumuladas ao longo das transmissões por engrenagens ou correias \cite{dombre2007robot}.

A distribuição de varredura I\textsuperscript{2}C foi dividida de forma a evitar gargalos de barramento:
\begin{itemize}
    \item \textbf{Canais I\textsuperscript{2}C do Mestre (ESP32 S3):} O ESP32 S3 utiliza seus dois canais físicos independentes de hardware configuráveis por software em seus pinos GPIO para realizar a varredura síncrona dos sensores das juntas inferiores da base (J1, J2 e J3). A leitura é conduzida de forma paralela no Core 1 do ESP32 S3 na taxa fixa de amostragem do laço fechado (200 Hz).
```
* **Substituição Recomendada:**
```latex
A leitura dessa telemetria ocorre via protocolo serial síncrono bidirecional \textbf{I\textsuperscript{2}C} (\textit{Inter-Integrated Circuit}) operando em modo de alta velocidade (\textit{Fast Mode} a 400 kHz). Uma inovação mecatrônica essencial no braço EB15 reside na fixação física dos sensores AS5600 diretamente nos eixos de rotação de saída das juntas estruturais impressas em 3D, após a etapa física de redução mecânica, e não nas partes traseiras dos eixos lineares internos dos motores de passo. Esta disposição física garante que a realimentação de controle de posição meça a rotação real do elo no espaço cartesiano, capturando e eliminando dos cálculos dinâmicos as folgas e perdas mecânicas (\textit{backlash}) acumuladas ao longo das transmissões por engrenagens ou correias \cite{dombre2007robot}.

A distribuição de varredura I\textsuperscript{2}C foi dividida de forma a evitar gargalos de barramento:
\begin{itemize}
    \item \textbf{Canais I\textsuperscript{2}C do Mestre (ESP32 S3):} O ESP32 S3 utiliza seus dois canais físicos independentes de hardware configuráveis por software em seus pinos GPIO (\textit{General Purpose Input/Output}) para realizar a varredura síncrona dos sensores das juntas inferiores da base (J1, J2 e J3). A leitura é conduzida de forma paralela no Core 1 do ESP32 S3 na taxa fixa de amostragem do laço fechado (200 Hz).
```

#### 7. Correção de Markdown para Monobloco (\`Wire\`) (Linha 55)
* **Original (linha 55):**
```latex
    \item \textbf{Canal I\textsuperscript{2}C do Escravo (Arduino Uno):} O Arduino Uno utiliza sua interface física síncrona de hardware dedicada (pinos analógicos A4/SDA e A5/SCL) acionada pela biblioteca otimizada `Wire` para varrer e processar localmente a leitura posicional dos encoders magnéticos das juntas superiores do punho (J4, J5 e J6). A amostragem local ocorre a 200 Hz, sincronizada por interrupções periódicas de temporizadores internos.
```
* **Substituição Recomendada:**
```latex
    \item \textbf{Canal I\textsuperscript{2}C do Escravo (Arduino Uno):} O Arduino Uno utiliza sua interface física síncrona de hardware dedicada (pinos analógicos A4/SDA e A5/SCL) acionada pela biblioteca otimizada \texttt{Wire} para varrer e processar localmente a leitura posicional dos encoders magnéticos das juntas superiores do punho (J4, J5 e J6). A amostragem local ocorre a 200 Hz, sincronizada por interrupções periódicas de temporizadores internos.
```

#### 8. Definição de Siglas CMOS, TTL, JSON e XML (Linhas 70--83)
* **Original (linhas 70--83):**
```latex
Devido à incompatibilidade de tensão lógica entre as duas plataformas mecatrônicas, uma proteção de barramento de alta velocidade torna-se necessária. O ESP32 S3 opera nativamente em lógica CMOS de 3,3 V, de forma que seus pinos de entrada e saída digitais toleram tensões máximas equivalentes à sua alimentação lógica de 3,3 V. Em contrapartida, o Arduino Uno (ATmega328P) opera sob lógica TTL estável de 5,0 V, emitindo sinais de nível lógico alto próximos a 5 V em seu pino de transmissão TX. A injeção direta de um sinal de 5 V na linha RX do ESP32 S3 causaria degradação térmica rápida por sobretensão, ativando os diodos de proteção interna contra descarga eletrostática (ESD) e levando à queima permanente da CPU mestre.
[...]
Para eliminar latências computacionais no tratamento e no parsing de dados na linha UART a 115200 bps, baniu-se a transmissão estruturada em dados textuais de alto overhead (como representações em formato JSON ou XML) \cite{tanvir2024optimizing}. Em seu lugar, desenvolveu-se um protocolo proprietário estruturado em frames de payload binários compactados de tamanho fixo, ocupando exatamente 10 bytes de dados úteis na fiação de comunicação.
```
* **Substituição Recomendada:**
```latex
Devido à incompatibilidade de tensão lógica entre as duas plataformas mecatrônicas, uma proteção de barramento de alta velocidade torna-se necessária. O ESP32 S3 opera nativamente em lógica CMOS (\textit{Complementary Metal-Oxide-Semiconductor}) de 3,3 V, de forma que seus pinos de entrada e saída digitais toleram tensões máximas equivalentes à sua alimentação lógica de 3,3 V. Em contrapartida, o Arduino Uno (ATmega328P) opera sob lógica TTL (\textit{Transistor-Transistor Logic}) estável de 5,0 V, emitindo sinais de nível lógico alto próximos a 5 V em seu pino de transmissão TX. A injeção direta de um sinal de 5 V na linha RX do ESP32 S3 causaria degradação térmica rápida por sobretensão, ativando os diodos de proteção interna contra descarga eletrostática (ESD) e levando à queima permanente da CPU mestre.
[...]
Para eliminar latências computacionais no tratamento e no parsing de dados na linha UART a 115200 bps, baniu-se a transmissão estruturada em dados textuais de alto overhead (como representações em formato JSON (\textit{JavaScript Object Notation}) ou XML (\textit{eXtensible Markup Language})) \cite{tanvir2024optimizing}. Em seu lugar, desenvolveu-se um protocolo proprietário estruturado em frames de payload binários compactados de tamanho fixo, ocupando exatamente 10 bytes de dados úteis na fiação de comunicação.
```

#### 9. Correção de Markdown para Negrito (Intervalos) e Itálico (*XOR*) na Tabela (Linhas 91--100)
* **Original (linhas 91--100):**
```latex
\begin{tabular}{|c|c|c|c|}
\hline
\textbf{Byte Posição} & \textbf{Campo Técnico} & \textbf{Tipo de Dado} & \textbf{Descrição do Conteúdo} \\ \hline
0 & Preâmbulo Sync & `uint8_t` & Padrão fixo de sincronismo elétrico (`0xAA`) \\ \hline
1 -- 2 & Passos Junta J4 & `int16_t` & Número de passos discretos para a Junta J4 (com sinal) \\ \hline
3 -- 4 & Passos Junta J5 & `int16_t` & Número de passos discretos para a Junta J5 (com sinal) \\ \hline
5 -- 6 & Passos Junta J6 & `int16_t` & Número de passos discretos para a Junta J6 (com sinal) \\ \hline
7 & Parâmetro Velocidade & `uint8_t` & Índice da frequência máxima mapeada do Timer \\ \hline
8 & Parâmetro Aceleração & `uint8_t` & Rampa de aceleração dinâmica sintonizada \\ \hline
9 & Soma Modular Checksum & `uint8_t` & Verificação de paridade (*XOR* cumulativo de bytes 0 a 8) \\ \hline
\end{tabular}
```
* **Substituição Recomendada:**
```latex
\begin{tabular}{|c|c|c|c|}
\hline
\textbf{Byte Posição} & \textbf{Campo Técnico} & \textbf{Tipo de Dado} & \textbf{Descrição do Conteúdo} \\ \hline
0 & Preâmbulo Sync & \texttt{uint8_t} & Padrão fixo de sincronismo elétrico (\texttt{0xAA}) \\ \hline
1--2 & Passos Junta J4 & \texttt{int16_t} & Número de passos discretos para a Junta J4 (com sinal) \\ \hline
3--4 & Passos Junta J5 & \texttt{int16_t} & Número de passos discretos para a Junta J5 (com sinal) \\ \hline
5--6 & Passos Junta J6 & \texttt{int16_t} & Número de passos discretos para a Junta J6 (com sinal) \\ \hline
7 & Parâmetro Velocidade & \texttt{uint8_t} & Índice da frequência máxima mapeada do Timer \\ \hline
8 & Parâmetro Aceleração & \texttt{uint8_t} & Rampa de aceleração dinâmica sintonizada \\ \hline
9 & Soma Modular Checksum & \texttt{uint8_t} & Verificação de paridade (\textit{XOR} cumulativo de bytes 0 a 8) \\ \hline
\end{tabular}
```

#### 10. Correção de Markdown para Monobloco e Itálico e Erros de Digitação (*jitter*, *checksum*, *Timer*, *Falling Edge*, *Acknowledge*, digital digital) (Linhas 109--123)
* **Original (linhas 109--123):**
```latex
Esse desvio temporal (\textit{jitter}) oscila normalmente entre 1 e 3 milissegundos.
[...]
    \item \textbf{Etapa de Configuração e Handshake de Software:}
    Ao planejar uma trajetória no espaço cartesiano, o Nó Mestre ESP32 S3 converte as trajetórias das três juntas superiores do punho em dados de passos, direções e rampas discretas. Em seguida, serializa e transmite o frame binário de 10 bytes para a placa Arduino Uno através do barramento UART. Ao receber o preâmbulo `0xAA` via recepção serial síncrona por interrupção, o Arduino Uno suspende qualquer varredura secundária, extrai o payload dinâmico, verifica a integridade do *checksum* linear e realiza a pré-configuração física de seus registradores internos de hardware dos temporizadores (*Timer1* e *Timer2*) com os valores recebidos de rampa de aceleração e passos.
    
    Uma vez configurado e pronto para disparar a movimentação em suas três juntas locais, o Arduino Uno transmite um byte simples de prontidão (`0x06` --- correspondente ao controle ASCII de confirmação \textit{Acknowledge}) via UART de volta para o ESP32 S3. Durante todo esse período de configuração e verificação, o mestre ESP32 S3 permanece no estado lógico de bloqueio, impedido de avançar as juntas J1 a J3 de base.
    
    \item \textbf{Etapa de Disparo Físico de Sincronismo por Hardware:}
    No instante em que o firmware do ESP32 S3 recebe o byte de confirmação `0x06` enviado pela placa escrava, o Nó Mestre altera o estado lógico de uma linha digital física dedicada GPIO por hardware. Esta linha conecta diretamente um pino de saída digital rápido do ESP32 S3 (GPIO 4) a um pino de interrupção externa física do microcontrolador ATmega328P no Arduino Uno (pino digital digital 2 --- correspondente à interrupção externa por hardware INT0).
    
    A comutação de estado elétrico (borda de descida do sinal elétrico de 5 V para 0 V - *Falling Edge*) propaga-se ao longo da linha física de cobre em tempo desprezível (frações de nanossegundos). A recepção da borda de descida no pino INT0 do Arduino Uno provoca um desvio de fluxo imediato a nível de hardware do processador AVR, forçando a execução instantânea da rotina de serviço de interrupção (ISR) de partida física dos temporizadores de passo no escravo. Ao mesmo tempo, o mestre ESP32 S3 aciona de forma síncrona seu próprio timer interno de alta prioridade de geração de passos das juntas J1 a J3 \cite{nissov2025simultaneous}.
```
* **Substituição Recomendada:**
```latex
Esse desvio temporal (\textit{jitter}) oscila normalmente entre 1 e 3 milissegundos.
[...]
    \item \textbf{Etapa de Configuração e Handshake de Software:}
    Ao planejar uma trajetória no espaço cartesiano, o Nó Mestre ESP32 S3 converte as trajetórias das três juntas superiores do punho em dados de passos, direções e rampas discretas. Em seguida, serializa e transmite o frame binário de 10 bytes para a placa Arduino Uno através do barramento UART. Ao receber o preâmbulo \texttt{0xAA} via recepção serial síncrona por interrupção, o Arduino Uno suspende qualquer varredura secundária, extrai o payload dinâmico, verifica a integridade do \textit{checksum} linear e realiza a pré-configuração física de seus registradores internos de hardware dos temporizadores (\textit{Timer1} e \textit{Timer2}) com os valores recebidos de rampa de aceleração e passos.
    
    Uma vez configurado e pronto para disparar a movimentação em suas três juntas locais, o Arduino Uno transmite um byte simples de prontidão (\texttt{0x06} --- correspondente ao controle ASCII de confirmação \textit{Acknowledge}) via UART de volta para o ESP32 S3. Durante todo esse período de configuração e verificação, o mestre ESP32 S3 permanece no estado lógico de bloqueio, impedido de avançar as juntas J1 a J3 de base.
    
    \item \textbf{Etapa de Disparo Físico de Sincronismo por Hardware:}
    No instante em que o firmware do ESP32 S3 recebe o byte de confirmação \texttt{0x06} enviado pela placa escrava, o Nó Mestre altera o estado lógico de uma linha digital física dedicada GPIO por hardware. Esta linha conecta diretamente um pino de saída digital rápido do ESP32 S3 (GPIO 4) a um pino de interrupção externa física do microcontrolador ATmega328P no Arduino Uno (pino digital 2 --- correspondente à interrupção externa por hardware INT0).
    
    A comutação de estado elétrico (borda de descida do sinal elétrico de 5 V para 0 V - \textit{Falling Edge}) propaga-se ao longo da linha física de cobre em tempo desprezível (frações de nanossegundos). A recepção da borda de descida no pino INT0 do Arduino Uno provoca um desvio de fluxo imediato a nível de hardware do processador AVR, forçando a execução instantânea da rotina de serviço de interrupção (ISR) de partida física dos temporizadores de passo no escravo. Ao mesmo tempo, o mestre ESP32 S3 aciona de forma síncrona seu próprio timer interno de alta prioridade de geração de passos das juntas J1 a J3 \cite{nissov2025simultaneous}.
```

#### 11. Correção de Markdown de Itálico e Monobloco no FreeRTOS (Linhas 126, 141--153)
* **Original (linhas 126, 141--153):**
```latex
Essa abordagem de paralelismo determinístico elimina o *jitter* da comunicação UART...
[...]
    O núcleo 1 executa as tarefas prioritárias de controle mecânico e realimentação. A tarefa principal `Task_BaseControl` opera em alta prioridade (prioridade 10 no escalonador preemptivo) e possui um período de amostragem fixo de 5 milissegundos ($200\text{ Hz}$). [...]
    A tarefa de temporização síncrona é acoplada a um temporizador por hardware interno do ESP32 S3 (\textit{Hardware Timer 0}), garantindo desvios temporais negligenciáveis. Também no Core 1 é executada a tarefa `Task_SerialComm` (prioridade 8), responsável por serializar e transmitir os pacotes dinâmicos via UART...
    [...]
    A tarefa `Task_WiFi` (prioridade 3) gerencia a estabilidade da interface de rede sem fio nos modos simultâneos \textbf{Station (STA)} e \textbf{Access Point (AP)}. 
    
    A tarefa `Task_WebServer` (prioridade 2) hospeda o servidor HTTP embarcado do robô. [...]
    
    A tarefa `Task_RTDE` (prioridade 4) processa de forma contínua conexões por soquete TCP/IP na porta padrão de rede, implementando a lógica clássica do protocolo RTDE para transferência de frames de telemetria em tempo real a 50 Hz e processamento de alvos cartesianos...
    [...]
    ...minimizando o *jitter* de geração de pulsos de passos.
```
* **Substituição Recomendada:**
```latex
Essa abordagem de paralelismo determinístico elimina o \textit{jitter} da comunicação UART...
[...]
    O núcleo 1 executa as tarefas prioritárias de controle mecânico e realimentação. A tarefa principal \texttt{Task\_BaseControl} opera em alta prioridade (prioridade 10 no escalonador preemptivo) e possui um período de amostragem fixo de 5 milissegundos ($200\text{ Hz}$). [...]
    A tarefa de temporização síncrona é acoplada a um temporizador por hardware interno do ESP32 S3 (\textit{Hardware Timer 0}), garantindo desvios temporais negligenciáveis. Também no Core 1 é executada a tarefa \texttt{Task\_SerialComm} (prioridade 8), responsável por serializar e transmitir os pacotes dinâmicos via UART...
    [...]
    A tarefa \texttt{Task\_WiFi} (prioridade 3) gerencia a estabilidade da interface de rede sem fio nos modos simultâneos \textbf{Station (STA)} e \textbf{Access Point (AP)}. 
    
    A tarefa \texttt{Task\_WebServer} (prioridade 2) hospeda o servidor HTTP (\textit{HyperText Transfer Protocol}) embarcado do robô. [...]
    
    A tarefa \texttt{Task\_RTDE} (prioridade 4) processa de forma contínua conexões por soquete TCP/IP na porta padrão de rede, implementando a lógica clássica do protocolo RTDE para transferência de frames de telemetria em tempo real a 50 Hz e processamento de alvos cartesianos...
    [...]
    ...minimizando o \textit{jitter} de geração de pulsos de passos.
```

#### 12. Correção de Markdown para Negrito e Itálico nos Itens da Interface Web (Linhas 160--162)
* **Original (linhas 160--162):**
```latex
    \item **Modo Jog Manual:** Controle individual com passos incrementais programáveis (em graus ou milímetros) para todas as 6 juntas e controle manual de abertura e fechamento do efetuador final (J6).
    \item **Programação de Pontos:** Gravação interativa de trajetórias baseada na inserção sequencial de posições angulares ou cartesianas em uma tabela dinâmica armazenada em flash, permitindo a reprodução cíclica posterior de trajetórias (*Teach-and-Play*).
    \item **Configuração de Relações de Redução:** Ajuste dinâmico independente das relações de engrenagens de transmissão mecânica ($R_1$ a $R_6$) para cada uma das juntas. Essa funcionalidade mecatrônica é salva em arquivo JSON local e carregada de forma instantânea na memória para corrigir os parâmetros de conversão física de passos por grau no modelo cinemático computado na CPU.
```
* **Substituição Recomendada:**
```latex
    \item \textbf{Modo Jog Manual:} Controle individual com passos incrementais programáveis (em graus ou milímetros) para todas as 6 juntas e controle manual de abertura e fechamento do efetuador final (J6).
    \item \textbf{Programação de Pontos:} Gravação interativa de trajetórias baseada na inserção sequencial de posições angulares ou cartesianas em uma tabela dinâmica armazenada em flash, permitindo a reprodução cíclica posterior de trajetórias (\textit{Teach-and-Play}).
    \item \textbf{Configuração de Relações de Redução:} Ajuste dinâmico independente das relações de engrenagens de transmissão mecânica ($R_1$ a $R_6$) para cada uma das juntas. Essa funcionalidade mecatrônica é salva em arquivo JSON (\textit{JavaScript Object Notation}) local e carregada dinamicamente na memória para corrigir os parâmetros de conversão física de passos por grau no modelo cinemático computado na CPU.
```

#### 13. Correção de Erros Ortográficos e Gramaticais no Nó Escravo e Arduino (Linhas 170, 178)
* **Original (linhas 170, 178):**
```latex
O firmware executado pelo microcontrolador AVR ATmega328P no Nó Escravo foi programado em linguagem C pura sem o uso de qualquer sistema operacional embarcado. A arquitetura de software baseia-se em um laço infinito simplificado de baixo nível (*main loop*) focado estritamente no processamento de requisições seriais síncronas de interrupção do buffer UART e leituras analógico-digitais secundárias.
[...]
No exato instante elétrico em que ocorre a interrupção externa física no pino INT0 do Arduino Uno decorrente do trigger de hardware comutado pelo ESP32 S3, executa-se a ISR dedicada. Esta rotina lê os parâmetros de passos carregados nos registradores e comuta de forma imediata o estado de ativação dos registradores dos Timers 1 e 2, iniciando as contagens físicas dos pulsos de micropasso em perfeita sincronia com o Nós Mestre \cite{nissov2025simultaneous}.
```
* **Substituição Recomendada:**
```latex
O firmware executado pelo microcontrolador AVR ATmega328P no Nó Escravo foi programado em linguagem C pura sem o uso de qualquer sistema operacional embarcado. A arquitetura de software baseia-se em um laço infinito de baixo nível (\textit{main loop}) focado estritamente no processamento de requisições seriais síncronas de interrupção do buffer UART e leituras analógico-digitais secundárias.
[...]
No exato instante elétrico em que ocorre a interrupção externa física no pino INT0 do Arduino Uno decorrente do trigger de hardware comutado pelo ESP32 S3, executa-se a ISR dedicada. Esta rotina lê os parâmetros de passos carregados nos registradores e comuta de forma imediata o estado de ativação dos registradores dos Timers 1 e 2, iniciando as contagens físicas dos pulsos de micropasso em perfeita sincronia com o \text{Nó Mestre} \cite{nissov2025simultaneous}.
```

#### 14. Correção de Ortografia e Estilo na Malha Fechada (Linha 195)
* **Original (linha 195):**
```latex
Onde $\Delta t = 5\text{ ms}$ é o tempo discreto de amostragem estabelecido, e $u[k]$ é a variável de atuação elotrotécnica linear correspondente. Para atenuar os efeitos dinâmicos de atrito e inércia do braço impresso em 3D, a ação de velocidade gerada sobre os motores de passo obedece à modulação não linear baseada na função de tangente hiperbólica descrita na Equação \ref{eq:tangente_hiperbolica}:
```
* **Substituição Recomendada:**
```latex
Onde $\Delta t = 5\text{ ms}$ é o tempo discreto de amostragem estabelecido, e $u[k]$ é a variável de atuação eletrotécnica linear correspondente. Para atenuar os efeitos dinâmicos de atrito e inércia do braço impresso em 3D, a ação de velocidade gerada sobre os motores de passo obedece à modulação não linear baseada na função de tangente hiperbólica descrita na Equação \ref{eq:tangente_hiperbolica}:
```

#### 15. Correção de Markdown para Negrito na Validação Experimental (Linha 214)
* **Original (linha 214):**
```latex
    \item **Acurácia Posicional Cartesiana em Malha Fechada:**
```
* **Substituição Recomendada:**
```latex
    \item \textbf{Acurácia Posicional Cartesiana em Malha Fechada:}
```

---

### 3.2. Alertas (Riscos de coesão ou de compilação)
- **Vazamentos de Markdown em Compiladores LaTeX:** O uso de asteriscos simples `*` e duplos `**` para formatação em itálico e negrito, assim como crases (\`) para termos monoespaçados, não impedirá necessariamente a compilação do arquivo pelo motor LaTeX, porém gerará glifos indesejados visíveis e erros de layout inaceitáveis para um trabalho acadêmico profissional. A substituição pelas tags nativas `\textit{}`, `\textbf{}` e `\texttt{}` é mandatória antes da submissão à banca de avaliação do TCC.

### 3.3. Sugestões de Melhoria (Refinamentos estilísticos para nota 10 na banca)
- **Substituição de Termos Qualitativos e Informais:** Para maior maturidade científica do texto, substitua expressões vagas como "poder computacional expressivo" por "capacidade de processamento elevada" e "de forma limpa e direta" por "diretamente", mantendo a escrita focada na precisão e distanciamento analítico.

---

## 4. Finalização
A auditoria sistemática indica que o **Capítulo 3 (Metodologia e Desenvolvimento)** possui excelente consistência de hardware, eletrônica e diagramas operacionais, porém necessita urgentemente de uma higienização tipográfica completa para remover vazamentos de formatação Markdown e de pequenos ajustes em ortografia e definição de siglas. A nota média de **8.6 / 10.0** destaca a alta robustez de engenharia do braço EB15, restando apenas o polimento gramatical e a adaptação nativa do LaTeX.
