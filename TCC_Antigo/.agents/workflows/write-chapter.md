---
description: Write a TCC chapter in pure LaTeX format. Enforces strict paragraph structures, cohesion rules, and exact ABNT citation syntax.
---

# /write-chapter - LaTeX Academic Chapter Writing

$ARGUMENTS

---

## Purpose

Guidance and automated writing for TCC chapters focusing fully on Brazilian Portuguese norms, LaTeX structure, strict paragraph formatting, and precise ABNT citations.

---

## Behavior

When `/write-chapter` is triggered:

### Step 1: Pre-flight Constraints Checks
- Identify chapter topic.
- Verify `docs/Resumo_tcc.md` and read `docs/Normas ABNT` requirements.
- Reinforce mental model: *NO MARKDOWN*. All outputs generated for the chapter must be `.tex` files or raw text that follows LaTeX logic (`\chapter{}`, `\section{}`, `\cite{}`).

### Step 2: Extract Outline (LaTeX)
Structure the outline to present to the user using the proper syntax:
```latex
\chapter{[TITLE]}
\section{[SUBTITLE]}
```
*Wait for user approval.*

### Step 3: Drafting under the "Golden Rules"
Trigger the `academic-writer` to generate the text focusing relentlessly on:
- Paragraph length: Minimum 4 lines, Maximum 8 lines.
- Paragraph composition: Topic sentence → Supporting Evidence/Citation → Applied Argument → Concluding Transition.
- Cohesion: EVERY paragraph MUST start with a cohesive connector (e.g. *Com base nisso, Todavia, Deste modo, Portanto, Adicionalmente*), excluding the opening paragraph of a section.

### Step 4: Citation Rules integration
- Ensure all indirect citations use `\cite{}` or `\citeonline{}`.
- Ensure any direct short citations use `"..."` + `\cite[p. X]{}`.
- Ensure any direct long citations use the custom LaTeX environment `\begin{citacao} ... \end{citacao}`.

### Step 5: Save
- Write the final document to `docs/tcc/[nome_do_arquivo].tex`.
- Automatically invoke the `academic-reviewer` logic internally via self-check before returning the file payload to the user.

---

## Outputs

A finished `.tex` file in `docs/tcc/` ready to be integrated into an overarching `main.tex` template, alongside a summary of the generation containing word counts and structural checks.
