# Revisão Acadêmica — Capítulo 2: Fundamentação Teórica

**Arquivo:** `2_fundamentacao.tex`
**Data:** 2026-03-18
**Revisor:** academic-reviewer (AG-kit)

---

## Sumário Executivo

| Lente | Nota | Status |
|-------|------|--------|
| 1. Gramática e Idioma | 8/10 | 🟢 Bom |
| 2. Regra do Parágrafo | 7/10 | 🟡 Atenção |
| 3. Sintaxe LaTeX | 7/10 | 🟡 Atenção |
| 4. Conformidade ABNT | 8/10 | 🟢 Bom |
| 5. Profundidade Sistêmica | 7/10 | 🟡 Atenção |

**Veredicto geral:** Capítulo sólido, com boa coesão e fluidez argumentativa. Há **13 achados** que demandam correção, sendo 3 críticos, 5 moderados e 5 leves.

---

## Lente 1 — Gramática e Idioma

### ✅ Pontos positivos
- Impessoalidade mantida em praticamente todo o texto (voz passiva e construções impessoais).
- Vocabulário técnico adequado e preciso.
- Norma culta respeitada de forma consistente.

### ⚠️ Achados

| # | Linha | Severidade | Achado |
|---|-------|------------|--------|
| G1 | 19 | 🟡 Moderado | "investimentos na ordem de centenas de reais" → falta artigo: "investimentos **da** ordem de centenas de reais". Expressão fixa "da ordem de". |
| G2 | 105 | 🟡 Moderado | "garantindo que todos concluam seu percurso no mesmo intervalo de tempo" → ambiguidade de sujeito. Sugestão: "garantindo que **todos os eixos** concluam **seus respectivos** percursos". |
| G3 | 93 | 🟢 Leve | "variações bruscas de aceleração que podem causar vibrações excessivas" → preferir: "que **podem provocar** vibrações excessivas" (evitar repetição do verbo "causar", usado na linha 95). |

---

## Lente 2 — Regra do Parágrafo (4-8 linhas / Tópico-Sustentação-Argumento-Conclusão)

### ✅ Pontos positivos
- A maioria dos parágrafos segue fielmente a estrutura Tópico → Sustentação → Argumento → Conclusão.
- Conectivos de coesão presentes no início de todos os parágrafos não-iniciais de seção.

### ⚠️ Achados (parágrafos que violam as regras)

| # | Linha | Severidade | Achado |
|---|-------|------------|--------|
| P1 | 3-5 | 🔴 Crítico | **Parágrafos introdutórios do capítulo excedem 8 linhas.** O parágrafo da L3 é extremamente longo (uma única frase de ~6 linhas compiladas, mas ao renderizar no LaTeX com margens ABNT, ultrapassará 8 linhas impressas). O parágrafo da L5 também é extenso. **Ação:** dividir cada um em dois parágrafos menores (4-6 linhas cada). |
| P2 | 75 | 🟡 Moderado | **Parágrafo excede 8 linhas.** O segundo parágrafo da seção IK (L75) combina 3 citações + argumentação + conclusão em um único bloco que renderizará com mais de 8 linhas. **Ação:** separar os métodos analíticos e numéricos em parágrafos distintos. |
| P3 | 105 | 🟡 Moderado | **Parágrafo com apenas 3 linhas renderizadas.** A subseção "Coordenação Multi-Eixo" (L105) tem um único parágrafo curto demais (~3 linhas impressas). **Ação:** expandir a argumentação com 1-2 frases adicionais conectando ao projeto (Bresenham usado no Arduino Uno para J4-J6). |
| P4 | 147 | 🟢 Leve | **Parágrafo marginal (3-4 linhas).** A subseção "UART e Protocolo Binário" (L147) tende a renderizar com exatamente 3-4 linhas, no limite inferior. Aceitável por ser seção breve, mas considerar adicionar uma frase de fechamento. |
| P5 | 155 | 🟢 Leve | **Igual ao anterior.** A subseção "Persistência" (L155) renderizará com ~3 linhas. Mesmo caso. |

---

## Lente 3 — Sintaxe LaTeX

### ✅ Pontos positivos
- Uso correto de `\chapter{}`, `\section{}`, `\subsection{}`.
- Equações numeradas com `\label{}` e `\ref{}`.
- Uso adequado de `\textit{}` para termos em inglês.
- Ausência de marcação Markdown — 100% LaTeX.

### ⚠️ Achados

| # | Linha | Severidade | Achado |
|---|-------|------------|--------|
| L1 | 11 | 🟡 Moderado | **Travessão em título de subseção.** `\subsection{Robôs Manipuladores — Classificação e Anatomia}` — o caractere `—` (em-dash) pode causar problemas de codificação dependendo do compilador. **Ação:** usar `\textemdash{}` ou substituir por `:` ou `--`. Este padrão se repete em múltiplos títulos (L23, L33, L44, L77, L85, L91, L103, L111, L117, L133, L175, L185). |
| L2 | 37-40 | 🟢 Leve | **Equação sem pacote amsmath declarado.** Os ambientes `bmatrix` e `\mathbb` requerem os pacotes `amsmath` e `amssymb`. Verificar se o `main.tex` já os carrega. |
| L3 | 183 | 🟡 Moderado | **Citação dupla redundante.** `Conforme \citeonline{siciliano2009}, a norma ISO 9283 \cite{iso9283}` — há duas citações distintas na mesma frase, o que gera uma construção com duas chamadas consecutivas. **Ação:** reformular: "Conforme a norma ISO 9283 \cite{iso9283}, os critérios de desempenho..." ou atribuir a frase apenas a Siciliano. |

---

## Lente 4 — Conformidade ABNT (Citações e Referências)

### ✅ Pontos positivos
- Uso correto de `\cite{}` (final de frase) e `\citeonline{}` (autor no texto) em praticamente todas as ocorrências.
- Todas as citações são indiretas (paráfrase) → sem necessidade de página.
- Nenhuma citação direta encontrada → nenhuma necessidade de `\begin{citacao}`.

### ⚠️ Achados

| # | Linha | Severidade | Achado |
|---|-------|------------|--------|
| A1 | ref.bib | 🔴 Crítico | **Entrada `silva2020` é placeholder.** A referência "João Silva, Robótica de baixo custo, Revista Brasileira de Robótica" não corresponde a uma publicação real verificável. **Ação:** substituir por referência real ou remover a citação da L9. |
| A2 | ref.bib | 🔴 Crítico | **Entradas `@standard` não são tipo BibTeX válido.** As entradas `iso9283` e `iso10218` usam `@standard`, que não é reconhecido pelo abntex2cite. **Ação:** alterar para `@misc` ou `@manual` e adicionar campos `note` com a URL de acesso. |
| A3 | ref.bib | 🟡 Moderado | **Entrada `schwab2016` é artigo de opinião (WEF), não periódico revisado.** Aceitável como fonte complementar, mas recomenda-se adicionar pelo menos o campo `urldate` (data de acesso) no BibTeX. |
| A4 | ref.bib | 🟢 Leve | **Campo `isbn` em `@manual`.** As entradas `espressif2023` e `atmel2018` são do tipo `@manual`, o que é adequado para datasheets, mas o campo `note` com URL deveria incluir `Acesso em: [data]` conforme ABNT NBR 6023 para documentos eletrônicos. |

---

## Lente 5 — Profundidade Sistêmica (vs. Resumo do TCC)

### ✅ Pontos positivos
- Cobertura sólida de todos os tópicos da organização aprovada.
- Conexão constante entre teoria e aplicação no protótipo EB-15.
- Boa transição entre seções de nível superior.

### ⚠️ Achados

| # | Seção | Severidade | Achado |
|---|-------|------------|--------|
| D1 | 2.1.1 | 🟡 Moderado | **UR-10 mencionado mas não fundamentado.** O UR-10 é citado como referência de comparação, mas não há descrição técnica do mesmo (DOF, payload, precisão especificada, características de cobot). O `Resumo_tcc.md` enfatiza a comparação com o UR-10 como eixo central — seria valioso adicionar 1 parágrafo descrevendo-o. |
| D2 | 2.4.1 | 🟢 Leve | **Encoders mencionados no Resumo, ausentes na fundamentação.** O `Resumo_tcc.md` L77 menciona "6 encoders magnéticos AS5600". A seção 2.4.3 (malha aberta vs. fechada) discute encoders genericamente, mas o AS5600 especificamente não aparece. Se os encoders fazem parte do projeto, poderiam ser mencionados como possível evolução. |
| D3 | 2.6 | 🟢 Leve | **Visão computacional omitida.** O `Resumo_tcc.md` menciona "rastreamento visual com webcam" como funcionalidade futura. Embora seja um trabalho futuro e não necessite fundamentação profunda, uma menção breve na seção IoT reforçaria a coerência com o resumo. |

---

## Plano de Correções Recomendadas

### Prioridade Alta (🔴 Crítica)
1. **A1:** Substituir `silva2020` por referência real de robótica de baixo custo.
2. **A2:** Alterar `@standard` para `@misc` em `iso9283` e `iso10218`.
3. **P1:** Dividir os 2 parágrafos introdutórios do capítulo para respeitar o limite de 8 linhas.

### Prioridade Média (🟡 Moderada)
4. **P2:** Dividir o parágrafo de IK (L75) em dois.
5. **P3:** Expandir o parágrafo de Coordenação Multi-Eixo (L105).
6. **L1:** Padronizar travessões nos títulos de subseção com `--` ou `:`.
7. **L3:** Reformular a citação dupla na L183.
8. **D1:** Adicionar descrição técnica do UR-10 na seção 2.1.1.

### Prioridade Baixa (🟢 Leve)
9-13. G1, G2, G3, A3, A4 — correções gramaticais menores e ajustes de formatação BibTeX.

---

## Contadores

| Métrica | Valor |
|---------|-------|
| Parágrafos totais | ~38 |
| Parágrafos conformes (4-8 linhas + estrutura) | ~33 (87%) |
| Citações `\cite{}` | 11 |
| Citações `\citeonline{}` | 28 |
| Equações numeradas | 3 |
| Referências no `.bib` | 22 |
| Referências efetivamente citadas | 19 |
| Referências não citadas no texto | 3 (`iso10218`, `gonzalezgomez2012` linha 19 OK, `silva2020` L9 OK) → `iso10218` não é citado no capítulo |
