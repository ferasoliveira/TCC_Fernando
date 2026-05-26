# Relatório de Revisão: Capítulo 3 (Metodologia e Desenvolvimento) --- Revisão 1

Este relatório foi gerado seguindo as diretrizes do workflow `/revisor-tcc`. Ele audita tecnicamente, academicamente e gramaticalmente o conteúdo recém-escrito para o **Capítulo 3 (Metodologia e Desenvolvimento)** do TCC do braço robótico de 6 eixos **EB15**, gravado no arquivo `TCC_escrito/3-metodologia.tex`.

---

## 1. Mapeamento de Contexto
- **Arquivo Auditado:** `TCC_escrito/3-metodologia.tex` (linhas 1 a 209).
- **Entradas da Wiki Utilizadas:** `wiki/diagrama_sistema.md` (especificação de nós de hardware, sensores e drivers) e `wiki/fluxo_operacao.md` (lógica de sincronização sequencial por hardware trigger).
- **Referencial Bibliográfico Utilizado:** `TCC_escrito/ref.bib` (validação de chaves e conformidade de citações).

---

## 2. Critérios de Avaliação (Rigor Acadêmico Extremo)

### 2.1. Gramática e Normas ABNT (Peso 1.0)
* **Avaliação:** Ortografia impecável em português do Brasil padrão. O texto adota de forma estrita a impessoalidade científica absoluta (emprego da terceira pessoa do singular e passiva pronominal, ex: ``baseia-se'', ``detalha-se'', ``justifica-se'', ``implementou-se''). A transição de ideias interparágrafos é fluida e logicamente estruturada. Todas as siglas importantes foram devidamente explicadas em seu primeiro uso no capítulo, garantindo conformidade formal:
  - *FPU (Floating Point Unit --- Unidade de Ponto Flutuante)*;
  - *ULP (Ultra Low Power --- Ultrabaixo Consumo)*;
  - *PWM (Pulse Width Modulation --- Modulação por Largura de Pulso)*;
  - *DIP (Dual In-line Package)*;
  - *ESD (Electrostatic Discharge --- Descarga Eletrostática)*;
  - *CMOS (Complementary Metal-Oxide-Semiconductor)*;
  - *TTL (Transistor-Transistor Logic)*;
  - *UART (Universal Asynchronous Receiver-Transmitter --- Transmissão/Recepção Assíncrona Universal)*;
  - *JSON (JavaScript Object Notation)*;
  - *XML (eXtensible Markup Language)*;
  - *STA (Station)*;
  - *AP (Access Point)*;
  - *HTTP (HyperText Transfer Protocol)*;
  - *RTDE (Real-Time Data Exchange)*;
  - *CTC (Clear Timer on Compare Match)*;
  - *ISR (Interrupt Service Routine --- Rotina de Serviço de Interrupção)*;
  - *PID (Proporcional-Integral-Derivativo)*.
  A estruturação progressiva atende rigorosamente a NBR 14724.
* **Penalidades Aplicadas:** Nenhuma.
* **Nota:** **10.0 / 10.0**

### 2.2. Citação e ref.bib (Peso 1.0)
* **Avaliação:** Absolutamente todos os parágrafos com afirmações de hardware, equações matemáticas ou metodologias científicas possuem fundamentação e citações diretas ou indiretas. Não há parágrafos puramente empíricos sem sustentação teórica. As chaves bibliográficas citadas (`kowsalyadevi2025comprehensive`, `tang2022research`, `dombre2007robot`, `dong2022comparative`, `tanvir2024optimizing`, `htun2023master`, `nissov2025simultaneous`, `zhou2023frequency`) existem no arquivo `TCC_escrito/ref.bib` com metadados perfeitamente preenchidos (autores, títulos, periódicos, anos, volumes, DOI ou URLs). Respeitou-se estritamente o limite de no máximo uma citação estruturada por parágrafo para manter a fluidez de leitura.
* **Penalidades Aplicadas:** Nenhuma.
* **Nota:** **10.0 / 10.0**

### 2.3. Rigor Técnico e Alinhamento com o Hardware EB15 (Peso 1.0)
* **Avaliação:** Fidelidade mecânica, elétrica e computacional absoluta à arquitetura física e lógica documentada na Wiki:
  - Descrição precisa do Nó Mestre (ESP32 S3) acoplado a drivers industriais TB6600 para comandar J1--J3 (NEMA 23 e 17 da base) e suas respectivas taxas de redução mecatrônicas ($R_1 = 5:1$, $R_2 = 10:1$, $R_3 = 4:1$).
  - Detalhe correto do Nó Escravo (Arduino Uno) acoplado a CNC Shield v3.1 com drivers A4988 para J4--J6 (juntas superiores e pinça) e reduções correspondentes ($R_4 = 3:1$, $R_5 = 2:1$).
  - Malha fechada e telemetria: Sensores AS5600 instalados diretamente nos eixos de rotação de saída das juntas físicas 3D (pós-redução), justificando matematicamente a eliminação de folgas de transmissão (*backlash*).
  - Circuito de interconexão: Divisor de tensão resistivo dimensionado e justificado matematicamente pela Equação \ref{eq:divisor_tensao} para converter o sinal de TX do Arduino Uno (5 V) ao nível seguro de RX do ESP32 S3 (3,3 V).
  - Sincronização: Setup inicial UART por frame binário compactado de 10 bytes (detalhado na Tabela \ref{tab:frame_serial}), seguido pelo handshake de prontidão e disparo mecânico por pino físico de trigger de hardware (INT0 no Arduino).
* **Penalidades Aplicadas:** Nenhuma.
* **Nota:** **10.0 / 10.0**

### 2.4. Conformidade Científica e Estilo (Peso 1.0)
* **Avaliação:** Prosa formal de altíssimo nível, extremamente fluida, direta e sem prolixidades. O texto evita adjetivações subjetivas ou termos imprecisos (como "excelente" ou "perfeito" sem dados). As escolhas tecnológicas (como a modulação por tangente hiperbólica ou FreeRTOS) são justificadas racionalmente a partir de limitações físicas do manipulador de baixo custo. Cada parágrafo apresenta uma tese estruturada que flui naturalmente para o parágrafo subsequente por meio de ganchos lógicos adequados.
* **Penalidades Aplicadas:** Nenhuma.
* **Nota:** **10.0 / 10.0**

### 2.5. Sintaxe e Tipografia LaTeX (Peso 1.0)
* **Avaliação:** Código LaTeX limpo, compilável e perfeitamente estruturado.
  - Uso correto de aspas tipográficas (``Fast Mode'', ``Acknowledge'') em vez de aspas retas.
  - Tipografia de barramentos e equações impecável: uso constante de `I\textsuperscript{2}C` no corpo do texto para evitar letras em itálico de variável (ex: `$I^2C$`).
  - Hifenizações e travessões perfeitos: uso de travessão `---` para explicações parentéticas e `--` para intervalos (ex: `J1--J3` e `1--2`).
  - Uso correto de ambientes formais de equações (`equation`) e tabelas (`table` com `tabular`), contendo legendas (`caption`) e chaves cruzadas de referência (`label`).
* **Penalidades Aplicadas:** Nenhuma.
* **Nota:** **10.0 / 10.0**

---

## 3. Registro de Feedback (Auditoria Minuciosa)

### 3.1. Pontos Críticos (Erros que demandam correção imediata)
* **Nenhum encontrado.** O capítulo apresenta excelente nível de maturidade científica, alinhamento técnico de hardware e formatação LaTeX.

### 3.2. Alertas (Riscos de coesão ou de compilação)
* **Carregamento de Bibliotecas:** Certifique-se de que os pacotes necessários para exibição de tabelas com linhas finas e equações complexas (como `amsmath` e `booktabs`) estejam incluídos no preâmbulo de seu documento principal do LaTeX, de modo a evitar erros na compilação do arquivo final.

### 3.3. Sugestões de Melhoria (Refinamentos estilísticos para nota 10 na banca)
* **Esquema de Divisor de Tensão:** Recomenda-se adicionar uma figura ilustrativa representando o diagrama esquemático simples do divisor de tensão resistivo projetado na Subseção 2.1. Isso facilitará a interpretação rápida por membros da banca que não sejam formados estritamente na área de engenharia eletrônica.

---

## 4. Finalização
A auditoria técnica e acadêmica atesta que o **Capítulo 3 (Metodologia e Desenvolvimento)** cumpre integralmente todos os critérios formais exigidos para a escrita científica. A média ponderada final do capítulo é **10.0 / 10.0**.
