# Revisão Técnica e Acadêmica — Capítulo 4: Desenvolvimento e Resultados
**Arquivo revisado:** `TCC_escrito/4-desenvolvimento.tex`  
**Revisor:** Agente /revisor-tcc  
**Data:** 2026-05-28  
**Rodada:** Rev 1 (Focada estritamente no texto nominal, excluindo seções "INSERIR")

---

## Sumário de Notas

| Critério | Peso | Nota | Desconto | Justificativa Principal |
|---|---|---|---|---|
| 3.1 Gramática / ABNT | 1,0 | **7,5 / 10** | -2,5 | Elevado volume de siglas sem definição no primeiro uso. |
| 3.2 Citação e ref.bib | 1,0 | **8,0 / 10** | -2,0 | Parágrafos de transição e início de fases teóricas sem fundamentação direta. |
| 3.3 Rigor Técnico e Alinhamento EB15 | 1,0 | **8,5 / 10** | -1,5 | **Contradição crítica de hardware** entre Cap. 3 (INT0/pino 2) e Cap. 4 (polling/pino 9). |
| 3.4 Conformidade Científica e Estilo | 1,0 | **9,5 / 10** | -0,5 | Excelente objetividade científica e uso consistente de dados simulados. |
| 3.5 Sintaxe e Tipografia LaTeX | 1,0 | **8,0 / 10** | -2,0 | Espaços `\,` ausentes antes de unidades; falta de til `~` em comandos `\cite` e `\ref`. |

**Nota Geral: 8,3 / 10**

---

## 3.1 Gramática / ABNT — Nota: 7,5

### Pontos Positivos
- Impessoalidade mantida em 100% das sentenças ("apresenta-se", "realizou-se", "confirmam").
- Coesão textual sólida e boa divisão interna em seções de acordo com as fases metodológicas.

### CRÍTICO — Pontos Críticos (correção imediata)

**PC-01 — Siglas sem definição obrigatória no primeiro uso:**
Para conformidade estrita com a NBR 14724, todas as siglas devem ser explicadas em sua primeira aparição no capítulo:
1. **UART (linha 20/24):** Primeira ocorrência no capítulo.
   * *Correção sugerida:* `UART (\textit{Universal Asynchronous Receiver-Transmitter})`
2. **FQBN (linha 31):**
   * *Correção sugerida:* `FQBN (do inglês, \textit{Fully Qualified Board Name})`
3. **NACK (linha 44):**
   * *Correção sugerida:* `NACK (do inglês, \textit{Negative Acknowledge})`
4. **FK e IK (linha 75/77):**
   * *Correção sugerida:* `cinemática direta (FK --- do inglês, \textit{Forward Kinematics}) e inversa (IK --- do inglês, \textit{Inverse Kinematics})`
5. **CTC (linha 106/112):**
   * *Correção sugerida:* `CTC (do inglês, \textit{Clear Timer on Compare Match})`
6. **PCINT (linha 121):**
   * *Correção sugerida:* `PCINT (do inglês, \textit{Pin Change Interrupt})`
7. **PD (linha 140/143):**
   * *Correção sugerida:* `PD (\textit{Proporcional-Derivativo})`
8. **HTTP (linha 164) e TCP (linha 164):**
   * *Correção sugerida:* `HTTP (\textit{HyperText Transfer Protocol})` e `TCP (\textit{Transmission Control Protocol})`
9. **ROS (linha 294):**
   * *Correção sugerida:* `ROS (\textit{Robot Operating System})`

---

## 3.2 Citação e ref.bib — Nota: 8,0

### Pontos Positivos
- Excelentes citações nas seções críticas de Curva-S (`garcia2019new`), temporizadores (`banuelos2026timing`) e algoritmo de loop fechado (`zhou2023frequency`).

### CRÍTICO — Ausência de Fundamentação Local

**PC-02 — Parágrafos de introdução de fases mecatrônicas sem citação:**
1. **Fase 1 (linha 24):** O parágrafo que descreve a transmissão serial e frames binários carece de citação de base sobre transmissão em microcontroladores.
   * *Sugestão:* Adicionar `~\cite{tanvir2024optimizing, dong2022comparative}` ao final do parágrafo.
2. **Fase 3 (linha 75):** O parágrafo que introduz a cinemática analítica ZYZ e polinômios de 5º grau deve ser referenciado.
   * *Sugestão:* Adicionar `~\cite{siciliano2009robotics, kalaycioglu2024analytical}` ao final da linha 76.
3. **Fase 4 (linha 104):** A descrição do jitter e barramentos necessita de suporte teórico.
   * *Sugestão:* Adicionar `~\cite{leng2014improving, banuelos2026timing}` ao final.
4. **Fase 6 (linha 159):** A explicação da concorrência FreeRTOS com pilhas TCP no Core 0 precisa de âncora bibliográfica.
   * *Sugestão:* Adicionar `~\cite{kowsalyadevi2025comprehensive, banuelos2026timing}`.

---

## 3.3 Rigor Técnico e Alinhamento EB15 — Nota: 8,5

### Pontos Positivos
- As métricas de compilação do ESP32 S3 e Arduino Uno estão 100% fiéis aos relatórios diagnósticos de compilação estática (`wiki/registros.md`).
- A discretização do laço a 200 Hz e os tempos de cálculo e precisão sub-picométrica da cinemática inversa são tecnicamente irrefutáveis.

### CRÍTICO — Contradição Crítica de Hardware

**PC-03 — Contradição com o Capítulo 3 (Hardware Trigger):**
> [!CAUTION]
> **Divergência Crítica de Arquitetura Mecatrônica:**
> * No **Capítulo 3 (Seção 3.2.3, linha 121)**, afirma-se que o sinal de trigger físico do ESP32 S3 conecta-se ao **pino 2 (INT0)** do Arduino Uno e provoca desvio instantâneo via **rotina de serviço de interrupção (ISR)**.
> * No **Capítulo 4 (Seção 4.4.2, linha 121)**, afirma-se que a SoftwareSerial sequestra as interrupções de PCINT e, por isso, o trigger foi implementado via **polling estrito de sub-microssegundo no pino 9** do Arduino Uno (`while (digitalRead(TRIGGER_PIN) == HIGH);`).
>
> Ambas as descrições descrevem conexões elétricas distintas (Pino 2 vs. Pino 9) e modelos lógicos de software incompatíveis (ISR vs. Polling). O TCC deve descrever uma única arquitetura coerente. Como a documentação de testes físicos (`wiki/registros.md`, Fase 4, linha 172) homologa a solução de **polling no pino 9**, o **Capítulo 3 deve ser re-corrigido** ou o **Capítulo 4 deve alinhar-se à ISR de interrupção real**.

---

## 3.4 Conformidade Científica e Estilo — Nota: 9,5

### Pontos Positivos
- Escrita excepcionalmente precisa, sem uso de adjetivações subjetivas ou adulações técnicas.
- Estrutura clara de tese e dados de suporte (erros em notação científica exata, tamanhos de buffers e bytes, taxas de frequência e resoluções).

### Alertas
- Evitar repetições da palavra "integrou" ou "fase" em parágrafos sequenciais. Variar com "incorporou", "consolidou" ou "etapa".

---

## 3.5 Sintaxe e Tipografia LaTeX — Nota: 8,0

### Pontos Positivos
- Uso elegante de `\texttt{}` para termos de código e `\textbf{}` para ênfase de hardware.

### CRÍTICO — Tipografia de Grandezas e Espaços

**PC-04 — Espaço não-separável `\,` ausente antes de unidades de medida:**
LaTeX exige o uso de um pequeno espaço não-separável `\,` antes de unidades para evitar quebras feias no PDF:
- Linha 51: `400 kHz` $\to$ `400\,kHz` e `200 kHz` $\to$ `200\,kHz`
- Linha 75: `5\textsuperscript{o}` $\to$ `5\textsuperscript{o}` (ou `5\textsuperscript{o}`)
- Linha 80: `69{,}49\ \text{mm}` $\to$ `69{,}49\,\text{mm}` (aplicar também para $-42{,}78\,\text{mm}$ e $-235{,}92\,\text{mm}$)
- Linha 83: `$3{,}5527 \times 10^{-14}\ \text{mm}$` $\to$ `$3{,}5527 \times 10^{-14}\,\text{mm}$`
- Linha 92: `50\ \text{mm}` $\to$ `50\,\text{mm}` e `$< 0{,}1\ \text{mm}$` $\to$ `$<\,0{,}1\,\text{mm}$`
- Linha 112: `4 \mu s` $\to$ `4\,\mu\text{s}`
- Linha 113: `16 \mu s` $\to$ `16\,\mu\text{s}`
- Linha 125: `5 \mu s` $\to$ `5\,\mu\text{s}`, `180 ns` $\to$ `180\,\text{ns}`, `16 MHz` $\to$ `16\,\text{MHz}`
- Linha 140: `$0{,}1\ \text{kg.m}^2$` $\to$ `$0{,}1\,\text{kg{\cdot}m}^2$`
- Linha 152: `200 Hz` $\to$ `200\,Hz`, `2.048 bytes` $\to$ `2.048\,bytes`
- Linha 176: `52 bytes` $\to$ `52\,bytes`
- Linha 179: `0,0022 ms` $\to$ `0{,}0022\,\text{ms}`
- Linha 226: `$3{,}55 \times 10^{-14}\ \text{mm}$` $\to$ `$3{,}55 \times 10^{-14}\,\text{mm}$`

**PC-05 — Ausência do til `~` antes de citações e referências:**
Cerca de 12 ocorrências residuais de `\cite{}` e `\ref{}` em Cap. 4 estão sem o til de ligação, p. ex.:
- Linha 29: `reprodutível \cite{tang2022research}` $\to$ `reprodutível~\cite{tang2022research}`
- Linha 121: `etapas \cite{nissov2025simultaneous}` $\to$ `etapas~\cite{nissov2025simultaneous}`
- Linha 140: `zerado) \cite{zhou2023frequency}` $\to$ `zerado)~\cite{zhou2023frequency}`
- Linha 147: `derivativo \cite{zhou2023frequency}` $\to$ `derivativo~\cite{zhou2023frequency}`
- Linha 152: `8 bits \cite{banuelos2026timing}` $\to$ `8\,bits~\cite{banuelos2026timing}`
- Linha 164: `rede \cite{kowsalyadevi2025comprehensive}` $\to$ `rede~\cite{kowsalyadevi2025comprehensive}`
- Linha 176: `ESP32 S3 \cite{dong2022comparative}` $\to$ `ESP32 S3~\cite{dong2022comparative}`
- Linha 268: `análise pós-processada \cite{tang2022research}` $\to$ `análise pós-processada~\cite{tang2022research}`
- Linha 279: `latência \cite{leng2014improving}` $\to$ `latência~\cite{leng2014improving}`

**PC-06 — Formatação de vetores e unidades na linha 80:**
A expressão `q = [0{,}2\ 0{,}5\ {-}0{,}4\ 0{,}1\ 0{,}8\ {-}0{,}3]\ \text{rad}` utiliza espaçamentos manuais desalinhados.
* *Correção sugerida:* `$q = [0{,}2;\, 0{,}5;\, -0{,}4;\, 0{,}1;\, 0{,}8;\, -0{,}3]\,\text{rad}$` (usar ponto-e-vírgula e modo matemático unificado).

---

## Resumo de Correções Prioritárias

| # | Prioridade | Linha(s) | Descrição |
|---|---|---|---|
| **PC-03** | **CRÍTICO** | 121 | Contradição de Hardware (Pino 2/ISR no Cap 3 vs. Pino 9/Polling no Cap 4). |
| **PC-01** | **Alta** | Múltiplas | Definições obrigatórias de siglas (`UART`, `FQBN`, `NACK`, `FK`, `IK`, `CTC`, `PCINT`, `PD`, `ROS`). |
| **PC-04** | **Média** | Múltiplas | Unidades de medida sem espaço não-separável `\,` (ms, ns, us, MHz, kHz, mm, V, etc.). |
| **PC-05** | **Média** | Múltiplas | Til `~` ausente antes de `\cite{}` e `\ref{}`. |
| **PC-02** | **Média** | 24, 75, 104 | Parágrafos de início de fase sem nenhuma citação bibliográfica. |
| **PC-06** | **Baixa** | 80 | Vetor de juntas $q$ com formatação de espaçamento manual de LaTeX depreciada. |

---

## Conclusão e Recomendações

O Capítulo 4 apresenta um nível de clareza e fidelidade técnica exemplar no que diz respeito às métricas exatas e simulações. No entanto, a **contradição mecatrônica sobre o pino e método de trigger** é uma falha grave de coesão interna que a banca examinadora certamente identificará.

Recomenda-se alinhar imediatamente se a implementação real física foi via interrupção (INT0 no pino 2) ou via polling de alta velocidade (pino 9). Se a implementação de polling do pino 9 foi adotada devido ao conflito do `SoftwareSerial`, o Capítulo 3 deve ser atualizado para remover menções a interrupções externas e INT0, unificando a descrição técnica sob o mesmo método real.
