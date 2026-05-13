---
description: Review a TCC chapter (.tex) under strict lenses - LaTeX syntax, grammar, paragraph golden rules, cohesion, and absolute ABNT rules.
---

# /review-chapter - Rigorous Academic Auditor

$ARGUMENTS

---

## Purpose

Triggers the `academic-reviewer` to audit a given chapter. The audit focuses intensely on compliance with LaTeX formatting, Brazilian Portuguese grammar, the "golden rules" of paragraph framing, and ABNT standards for citations.

---

## The 5 Audit Lenses

| # | Lens | O que o agente procura incansavelmente |
|---|--------|------------------------------------|
| 1 | **Gramática e Idioma** | Impessoalidade severa, correção gramatical absoluta. |
| 2 | **Regra do Parágrafo** | 4 a 8 linhas; Tópico → Sustentação → Argumento → Conclusão; Falta de Conectivos Iniciais. |
| 3 | **Sintaxe LaTeX** | Quebras falsas de `markdown`, seções fora de comandos nativos `\chapter{} \section{}`, indentações avulsas. |
| 4 | **Conformidade ABNT**| Uso de `\cite`, `\citeonline`, o ambiente `\begin{citacao}` para diretas longas marcando página correta. Baseado na pasta `docs/Normas ABNT`. |
| 5 | **Profundidade** | Argumentos soltos que não fazem sentido sistêmico em relação ao "Resumo\_tcc.md". |

---

## Behavior

When `/review-chapter` is triggered:

1. Identify the chapter file (preferring `.tex` files in `docs/tcc/`).
2. Dispatch `academic-reviewer`.
3. Generate the markdown report `[chapter-name]-review-[date].md` inside `docs/tcc/revisoes/`.
4. The generated report provides an executive, highly demanding summary indicating what paragraphs violated the "golden rules", where commas and verb conjugations missed the mark, and if the LaTeX parsing aligns with ABNT logic.
