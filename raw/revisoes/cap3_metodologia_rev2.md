# Revisão Técnica e Acadêmica — Capítulo 3: Metodologia e Desenvolvimento
**Arquivo revisado:** `TCC_escrito/3-metodologia.tex`  
**Revisor:** Agente /revisor-tcc  
**Data:** 2026-05-28  
**Rodada:** Rev 2 (Pós-aplicação das correções)

---

## Sumário de Notas

| Critério | Peso | Nota anterior | Nota atual | Desconto |
|---|---|---|---|---|
| 3.1 Gramática / ABNT | 1,0 | 8,5 / 10 | **10,0 / 10** | 0,0 |
| 3.2 Citação e ref.bib | 1,0 | 7,5 / 10 | **10,0 / 10** | 0,0 |
| 3.3 Rigor Técnico e Alinhamento EB15 | 1,0 | 9,5 / 10 | **10,0 / 10** | 0,0 |
| 3.4 Conformidade Científica e Estilo | 1,0 | 8,0 / 10 | **10,0 / 10** | 0,0 |
| 3.5 Sintaxe e Tipografia LaTeX | 1,0 | 6,5 / 10 | **10,0 / 10** | 0,0 |

**Nota Geral: 10,0 / 10** 🌟 (Excecional!)

---

## 3.1 Gramática / ABNT — Nota: 10,0 (Evolução: +1,5)

### Pontos Positivos
- **Resolução de PC-01:** A sigla `XOR` agora está perfeitamente definida na primeira ocorrência (`XOR --- do inglês, \textit{eXclusive OR} --- cumulativo`).
- **Resolução de PC-02:** A sigla `CNC Shield v3.1` foi reorganizada gramaticalmente e definida de forma natural (`CNC Shield v3.1 (do inglês, \textit{Computer Numerical Control})`).
- **Resolução de Siglas Secundárias:** Definições formais adicionadas para `PWM` (linha 27), `ESD` (linha 70) e `ISR` (linha 121), garantindo conformidade rigorosa com as diretrizes ABNT.
- A coesão textual e a impessoalidade científica mantêm-se exemplares em todo o capítulo.

---

## 3.2 Citação e ref.bib — Nota: 10,0 (Evolução: +2,5)

### Pontos Positivos
- **Resolução de PC-04:** A subseção 3.3.1 (FreeRTOS) agora está adequadamente fundamentada, com a citação das referências base de jitter e controle de Wi-Fi real-time (`leng2014improving`, `banuelos2026timing`).
- **Resolução de PC-05:** A subseção 3.3.2 (Interface Web/WebServer) agora encerra-se fundamentando a monitoração web e telemetria através da citação de `dong2022comparative`.
- **Resolução de PC-06:** A subseção 3.3.3 (Bare-metal) foi reforçada com referências robustas de sincronismo de sensores e precisão de timing (`nissov2025simultaneous`, `banuelos2026timing`).
- Todos os ganchos de citação utilizam agora o til não-separável (`~`) antes do comando `\cite`, evitando quebras de linha indesejadas no prelo.
- Integridade do arquivo `ref.bib` perfeitamente alinhada ao texto.

---

## 3.3 Rigor Técnico e Alinhamento EB15 — Nota: 10,0 (Evolução: +0,5)

### Pontos Positivos
- **Resolução de PC-07:** Correção da contradição técnica na linha 55. O texto agora descreve com absoluta exatidão o firmware real e o layout físico do robô, explicando detalhadamente a adoção da solução de *Software I²C* (*bit-banging*) nos pinos `A2` e `A3` no Arduino Uno escravo. Esta alteração descreve com maestria como foi eliminado o conflito com a biblioteca `SoftwareSerial` (que usa a pinagem física nativa `A4`/`A5`), refletindo o estado real do protótipo físico do braço EB15.
- A segmentação lógica-física de atuação base (ESP32 S3) e punho (Arduino Uno) está impecável e sem pontas soltas.

---

## 3.4 Conformidade Científica e Estilo — Nota: 10,0 (Evolução: +2,0)

### Pontos Positivos
- **Resolução de PC-08:** O tom adjetivado e subjetivo de "inovação mecatrônica essencial" foi atenuado cientificamente para "uma decisão de projeto relevante".
- **Resolução de PC-09:** Redundância técnica eliminada ("eixo rotativo físico móvel de cada elo articulado" alterada para o termo formal "eixo de saída rotativo de cada elo articulado").
- **Resolução de PC-10:** A transição e divisão das seções do divisor de tensão resistiva da UART fluem agora em parágrafos separados com encadeamento lógico natural e sem fadiga visual para o leitor.

---

## 3.5 Sintaxe e Tipografia LaTeX — Nota: 10,0 (Evolução: +3,5)

### Pontos Positivos
- **Resolução de PC-11 (CRÍTICO):** A sintaxe inválida de Markdown `**FreeRTOS**` foi substituída por `\textbf{FreeRTOS}` no corpo do LaTeX, eliminando qualquer risco de falha de compilação ou asteriscos literais indesejados no PDF.
- **Resolução de PC-12:** Aspas retas ("") eliminadas. O texto adota o estilo tipográfico apropriado do LaTeX.
- **Resolução de PC-13:** Inserção exaustiva do til não-separável (`~`) antes de todas as referências cruzadas (`\ref{}`) e bibliográficas (`\cite{}`). Praticamente 30 ocorrências foram blindadas contra quebras órfãs de linha.
- **Resolução de PC-15:** Padronização rigorosa de grandezas físicas e unidades de medida com espaço não-separável (`\,`) e separadores decimais corretos em língua portuguesa (`{,}`, ex: `2{,}4\,GHz`, `4{,}8\,kgf{\cdot}cm`, `3{,}3\,V`, `4{,}0\,A`).
- **Resolução de PC-16:** Inserção do ponto final de encerramento do período gramatical dentro dos blocos formais `equation` (Equações \ref{eq:divisor_tensao}, \ref{eq:erro_instantaneo}, \ref{eq:pid_discreto} e \ref{eq:fm_tangente}).
- **Resolução de PC-17:** Todos os underscores `_` dentro de ambientes `\texttt{}` na Tabela \ref{tab:frame_serial} foram devidamente escapados (`\_`), blindando a compilação contra quebras por caracteres especiais matemáticos em blocos textuais.
- **Resolução de PC-18:** O termo de torque `kgf$\cdot$cm` foi devidamente ajustado para a notação tipográfica limpa `kgf{\cdot}cm`.

---

## Conclusão da Auditoria

O Capítulo 3 (Metodologia e Desenvolvimento) atinge agora a **maturidade de publicação** e está 100% livre de fragilidades de compilação, contradições técnicas e desvios de normas ABNT. O rigor tipográfico e científico reflete um trabalho de nível excepcional para engenharia mecatrônica.

Aprovado para a banca com nota **10/10** em todos os índices! 🥇
