# Revisão Técnica e Acadêmica — Capítulo 3: Metodologia e Desenvolvimento
**Arquivo revisado:** `TCC_escrito/3-metodologia.tex`  
**Revisor:** Agente /revisor-tcc  
**Data:** 2026-05-28  
**Rodada:** Rev 1

---

## Sumário de Notas

| Critério | Peso | Nota | Desconto |
|---|---|---|---|
| 3.1 Gramática / ABNT | 1,0 | **8,5 / 10** | -1,5 |
| 3.2 Citação e ref.bib | 1,0 | **7,5 / 10** | -2,5 |
| 3.3 Rigor Técnico e Alinhamento EB15 | 1,0 | **9,5 / 10** | -0,5 |
| 3.4 Conformidade Científica e Estilo | 1,0 | **8,0 / 10** | -2,0 |
| 3.5 Sintaxe e Tipografia LaTeX | 1,0 | **6,5 / 10** | -3,5 |

**Nota Geral: 8,0 / 10**

---

## 3.1 Gramática / ABNT — Nota: 8,5

### Pontos Positivos
- Impessoalidade científica bem mantida em quase todo o texto.
- Uso consistente de 3ª pessoa do singular/plural.
- Estrutura de capítulo e seções segue numeração progressiva ABNT.

### CRÍTICO — Pontos Críticos (correção imediata)

**PC-01 — Sigla `XOR` nunca definida no texto (linha 83/98):**

```latex
% ATUAL (linha 98, na tabela):
Verificação de paridade (\textit{XOR} cumulativo de bytes 0 a 8)

% CORRETO — inserir definição na primeira ocorrência de XOR:
Verificação de paridade (XOR --- do inglês, \textit{eXclusive OR} --- cumulativo de bytes 0 a 8)
```

**PC-02 — Sigla `CNC` definida de forma deslocada (linha 34):**

```latex
% ATUAL:
\textbf{CNC} (\textit{Computer Numerical Control}) \textbf{Shield v3.1}

% CORRETO:
\textbf{CNC Shield v3.1} (do inglês, \textit{Computer Numerical Control})
```

**PC-03 — Ortografia fora do Novo Acordo: "activa" (linha 175):**

```latex
% ATUAL:
o Arduino Uno detecta a queda do pino de trigger e activa os contadores

% CORRETO:
o Arduino Uno detecta a borda de descida do sinal de trigger e ativa os contadores
```

### Alertas
- A sigla `ISR` (linha 167) não é definida. Inserir `ISR (\textit{Interrupt Service Routine})` na 1ª ocorrência.
- A sigla `PWM` (linha 27) não é definida. Inserir `PWM (\textit{Pulse Width Modulation})`.
- A sigla `ESD` (linha 70) não é definida. Inserir `ESD (\textit{ElectroStatic Discharge})`.

---

## 3.2 Citação e ref.bib — Nota: 7,5

### CRÍTICO — Parágrafos sem citação

**PC-04 — Subseção 3.3.1 FreeRTOS (linhas 136–151) sem nenhuma citação:**

```latex
% Sugestão — adicionar ao final do parágrafo na linha 151:
minimizando o \textit{jitter} de geração de pulsos de passos~\cite{leng2014improving, banuelos2026timing}.
```

**PC-05 — Subseção 3.3.2 Interface Web (linhas 156–163) sem citação:**

```latex
% Sugestão — adicionar ao final do último parágrafo:
permitindo a monitoração em tempo real do manipulador~\cite{dong2022comparative}.
```

**PC-06 — Subseção 3.3.3 Bare-metal (linhas 168–176) sem citação:**

```latex
% Sugestão — adicionar ao final da linha 176:
em perfeita sincronia com o Nó Mestre~\cite{nissov2025simultaneous, banuelos2026timing}.
```

### Alertas — Metadados BibTeX incompletos

| Chave BibTeX | Problema |
|---|---|
| `nissov2025simultaneous` | ID arXiv 2507 (julho/2025) mas tipo `@article`; usar `@misc` com `howpublished={arXiv:2507.05717}` |
| `lewin2023scurve` | Sem `volume`, `number`, `pages`, `doi`; reclassificar como `@misc` ou `@online` |
| `dong2022comparative` | Faltam `volume`, `number`, `pages`, `doi` |
| `ahmed2022inverse` | Faltam `volume`, `number`, `pages`, `doi` |
| `jleilaty2023distributed` | Faltam `volume`, `number`, `pages`, `doi` |
| `rew2021closed` | Faltam `volume`, `number`, `pages`, `doi` |

- Entradas `gomes2026micro` e `alsheikhy2025developing` não são citadas no Cap. 3. Verificar uso nos demais capítulos.

---

## 3.3 Rigor Técnico e Alinhamento EB15 — Nota: 9,5

### Pontos Positivos
- Divisão J1–J3 (ESP32 S3) e J4–J6 (Arduino Uno) correta em todas as seções.
- Tensões de alimentação (24 V / 12 V), micropasso 1/16 e resolução AS5600 tecnicamente precisos.
- Protocolo de trigger (GPIO 4 → Pino 2/INT0) coerente com a wiki.

### CRÍTICO — Contradição Técnica

**PC-07 — Canal I2C do Arduino Uno: contradição com o firmware real (linha 55):**

> Na linha 55, afirma-se uso de `A4/SDA` e `A5/SCL` com biblioteca `Wire`. O registros.md (Fase 2, linha 78) documenta que a solução real adota **Software I2C em A2 e A3** (bit-banging) para evitar conflito com a `SoftwareSerial`.

```latex
% ATUAL (linha 55):
O Arduino Uno utiliza sua interface física síncrona de hardware dedicada
(pinos analógicos A4/SDA e A5/SCL) acionada pela biblioteca otimizada \texttt{Wire}

% CORRETO:
O Arduino Uno adota uma solução de \textit{Software I\textsuperscript{2}C}
(\textit{bit-banging}) nos pinos analógicos \texttt{A2} e \texttt{A3}, operando
a aproximadamente 200 kHz. Esta abordagem elimina o conflito de barramento
com a biblioteca \texttt{SoftwareSerial}, que reserva os pinos \texttt{A4} (SDA)
e \texttt{A5} (SCL) do canal I\textsuperscript{2}C de hardware nativo.
```

---

## 3.4 Conformidade Científica e Estilo — Nota: 8,0

### CRÍTICO

**PC-08 — Adjetivação subjetiva não provada (linha 50):**

```latex
% ATUAL:
Uma inovação mecatrônica essencial no braço EB15 reside na fixação física

% CORRETO:
Uma decisão de projeto relevante no braço EB15 consiste na fixação física
```

**PC-09 — Redundância descritiva (linha 50):**

```latex
% ATUAL:
eixo rotativo físico móvel de cada elo articulado

% CORRETO:
eixo de saída rotativo de cada elo articulado
```

**PC-10 — Parágrafo monstro (linha 70–78):**
> Parágrafo de +10 linhas contínuas antes da equação do divisor de tensão. Dividir em dois:
> 1. Primeiro parágrafo: o problema (incompatibilidade de nível lógico 3,3 V vs. 5 V).
> 2. Segundo parágrafo: a solução (divisor resistivo) + equação.

### Alertas
- A expressão "de forma a" repete-se três vezes (linhas 58, 124, 151). Variar com "para" ou "a fim de".
- Verbos "planejou-se e implementou-se" (linha 203/204) devem ser consistentes com o tempo verbal geral do TCC (passado, após conclusão dos experimentos).

---

## 3.5 Sintaxe e Tipografia LaTeX — Nota: 6,5

Esta seção concentra o maior volume de erros. Lista exaustiva a seguir.

---

### CRÍTICO — Erro de Compilação

**PC-11 — Uso de sintaxe Markdown `**texto**` dentro de LaTeX (linha 134):**

```latex
% ATUAL (linha 134) — CAUSA FALHA DE COMPILAÇÃO:
o sistema adota o sistema operacional de tempo real **FreeRTOS** para realizar

% CORRETO:
o sistema adota o sistema operacional de tempo real \textbf{FreeRTOS} para realizar
```

> Este erro faz o compilador LaTeX inserir dois asteriscos literais no PDF, quebrando a tipografia.

---

**PC-12 — Aspas tipográficas incorretas (múltiplas linhas):**

> LaTeX exige ` `` ` (abre) e `''` (fecha). Aspas retas `"texto"` produzem caracteres errados no PDF.

| Linha | Incorreto | Correto |
|---|---|---|
| 27 | `"Dual In-line Package"` | ` ``Dual In-line Package'' ` |
| 107 | Verificar aspas em `"jitter"` | `\textit{jitter}` (sem aspas) |
| 156 | `"Jog"` | `\textit{Jog}` (sem aspas) |
| 159 | `"Teach-and-Play"` | `\textit{Teach-and-Play}` |

---

**PC-13 — Til `~` ausente antes de `\cite{}` e `\ref{}` (~25–30 ocorrências):**

> O espaço não-separável evita que a referência quebre para a linha seguinte separada da palavra que a precede.

```latex
% INCORRETO (exemplos):
...oscila normalmente entre 1 e 3 milissegundos. \cite{dong2022comparative}
...conforme a Equação \ref{eq:wrist_center}
...descrito na Figura \ref{fig:diagrama}

% CORRETO:
...oscila normalmente entre 1 e 3 milissegundos~\cite{dong2022comparative}.
...conforme a Equação~\ref{eq:wrist_center}
```

> Aplicar em TODAS as ocorrências. Script de busca: `grep -n "\\\\cite\|\\\\ref" 3-metodologia.tex`

---

**PC-14 — Modo matemático `$I^2C$` incorreto para barramento I2C:**

> No modo matemático, as letras ficam em itálico de variável (𝐼²𝐶), o que é semanticamente incorreto para uma sigla de protocolo.

```latex
% INCORRETO:
barramento $I^2C$

% CORRETO (já usado na maioria do documento):
barramento I\textsuperscript{2}C

% Sugestão de macro no preâmbulo:
\newcommand{\ItwoC}{I\textsuperscript{2}C}
% Uso: barramento \ItwoC{}
```

---

**PC-15 — Unidades de medida sem espaço não-separável `\,` (linhas 27, 41, 102, 165–166):**

```latex
% INCORRETO:
suporta tensões de entrada de até 42 V e correntes ajustáveis de até 4,0 A

% CORRETO:
suporta tensões de entrada de até 42\,V e correntes ajustáveis de até 4{,}0\,A
% ou com siunitx:
suporta tensões de entrada de até \SI{42}{V} e correntes ajustáveis de até \SI{4.0}{A}
```

> Aplicar em todas as unidades físicas: V, A, kHz, MHz, GHz, mm, Hz, ms, µs, ns, kgf·cm.

---

**PC-16 — Ponto final após ambiente `equation` (linha 75):**

> Quando uma equação encerra o período, o ponto deve estar dentro do ambiente:

```latex
% VERIFICAR linha 75:
\begin{equation}
    V_{RX\_ESP32} = 5{,}0 \cdot \left( \frac{2000}{1000 + 2000} \right) = 3{,}33\text{ V}
\end{equation}

% CORRETO (ponto dentro do ambiente, ao final):
\begin{equation}
    V_{RX\_ESP32} = 5{,}0 \cdot \left( \frac{2000}{1000 + 2000} \right) = 3{,}33\text{ V}.
\end{equation}
```

> Verificar também as Equações~\ref{eq:erro_instantaneo}, \ref{eq:pid_discreto} e \ref{eq:fm_tangente}.

---

**PC-17 — Underscores em `\texttt{}` não escapados (linhas 113, 139, 167, 173):**

```latex
% RISCO — verificar cada ocorrência:
\texttt{Task_BaseControl}   % INCORRETO — _ fora de modo math
\texttt{TIMER1_COMPA_vect}  % INCORRETO
\texttt{OCR1A}              % OK, sem underscore

% CORRETO:
\texttt{Task\_BaseControl}
\texttt{TIMER1\_COMPA\_vect}
```

> Executar: `grep -n "\\\\texttt{[^}]*_" 3-metodologia.tex` para localizar todas as ocorrências.

---

**PC-18 — `kgf$\cdot$cm` — mistura de modo texto e matemático (linhas 22–38):**

```latex
% ATUAL:
15 kgf$\cdot$cm

% MAIS CONSISTENTE (usando \cdot apenas em math mode, sem quebrar o fluxo):
15\,kgf{\cdot}cm
% ou com siunitx (recomendado):
\SI{15}{kgf.cm}
```

---

### Alertas de Compilação

- **Pacote `xcolor` ausente no preâmbulo:** O Cap. 4 usa `\textcolor{blue}{...}`. Adicionar `\usepackage[dvipsnames]{xcolor}` no `main.tex`.
- **Pacote `siunitx` recomendado:** Adicionar `\usepackage{siunitx}` e `\sisetup{output-decimal-marker={,}}` para padronizar unidades em português brasileiro.
- **Pacote `microtype` recomendado:** Melhora justificação e reduz overfull hbox.

---

### Sugestões de Melhoria LaTeX

1. Criar macro `\newcommand{\ItwoC}{I\textsuperscript{2}C}` no preâmbulo.
2. Criar macro `\newcommand{\EB}{\textbf{EB15}}` para consistência tipográfica.
3. Adicionar `\small` dentro do ambiente `tabular` da Tabela~\ref{tab:frame_serial} para evitar overflow horizontal com margens ABNT.
4. Usar `\SI{}{}` do pacote `siunitx` para todas as grandezas físicas.

---

## Resumo de Correções Prioritárias

| # | Prioridade | Linha(s) | Descrição |
|---|---|---|---|
| PC-11 | CRÍTICO | 134 | `**FreeRTOS**` → `\textbf{FreeRTOS}` (erro de compilação) |
| PC-07 | CRÍTICO | 55 | Contradição técnica: Wire.h vs. Software I2C real |
| PC-03 | Alta | 175 | `activa` → `ativa` |
| PC-01 | Alta | 98 | Sigla XOR sem definição |
| PC-13 | Média | Múltiplas | Til `~` ausente antes de `\cite{}` e `\ref{}` |
| PC-12 | Média | 27, 107, 156, 159 | Aspas retas → aspas tipográficas LaTeX |
| PC-15 | Média | 27, 41, 102 | Unidades sem `\,` não-separável |
| PC-04 | Média | 136–151 | Subseção FreeRTOS sem citação |
| PC-05 | Média | 156–163 | Subseção WebServer sem citação |
| PC-08 | Média | 50 | "inovação essencial" → "decisão de projeto relevante" |
| PC-17 | Média | 113, 139, 167 | Underscores `_` não escapados em `\texttt{}` |
| PC-16 | Baixa | 75 | Ponto final dentro do ambiente `equation` |
| PC-06 | Baixa | 168–176 | Subseção bare-metal sem citação |
| PC-14 | Baixa | Verificar | `$I^2C$` residual → `I\textsuperscript{2}C` |
| PC-18 | Baixa | 22–38 | `kgf$\cdot$cm` → padronizar com `siunitx` |
