---
description: Generate a progress report for the TCC project. Scans plans, checks completed items, and lists pending work.
---

# /progress-report - TCC Progress Report

$ARGUMENTS

---

## Purpose

This command generates a consolidated progress report across all TCC fronts: writing, firmware development, experiments, and documentation.

---

## Behavior

When `/progress-report` is triggered:

### Step 1: Scan Plans
- Read `Software e Firmware/docs/plano_implementacao.md` for firmware progress
- Read `Software e Firmware/docs/plano_testes.md` for test status
- Check `docs/tcc/` for chapter completion status
- Check `dados/` for experimental data status
- Check `docs/referencias/fontes.md` for bibliography status

### Step 2: Assess Each Front

| Front | Source | Metrics |
|-------|--------|---------|
| Firmware | `plano_implementacao.md` | Tasks done / total |
| Tests | `plano_testes.md` | Tests passed / total |
| Writing | `docs/tcc/` files | Chapters drafted / total |
| References | `docs/referencias/fontes.md` | Sources found / needed |
| Experiments | `dados/` | Experiments completed / planned |

### Step 3: Generate Report

```markdown
# 📊 Relatório de Progresso — TCC EB-15

**Data:** [YYYY-MM-DD]

## Visão Geral

| Frente | Progresso | Status |
|--------|-----------|--------|
| Firmware | X/Y tarefas | 🟢/🟡/🔴 |
| Testes | X/Y testes | 🟢/🟡/🔴 |
| Escrita | X/Y capítulos | 🟢/🟡/🔴 |
| Referências | X fontes | 🟢/🟡/🔴 |
| Experimentos | X/Y | 🟢/🟡/🔴 |

## Conquistas Recentes
- [What was completed recently]

## Bloqueios Atuais
- [What is blocked and why]

## Próximas Prioridades
1. [Most important next step]
2. [Second priority]
3. [Third priority]

## Linha do Tempo
[Assessment of overall timeline health]
```

### Step 4: Save
- Save to `relatorios/progresso-[date].md`

---

## Examples

```
/progress-report
/progress-report firmware only
/progress-report semana 12
```
