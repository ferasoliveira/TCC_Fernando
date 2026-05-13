---
description: Compare EB-15 performance against UR-10 reference. Selects metrics, collects data, and generates comparison reports.
---

# /compare-ur10 - Industrial Reference Comparison

$ARGUMENTS

---

## Purpose

This command guides a structured comparison between the EB-15 prototype and the UR-10 industrial robot, ensuring fair methodology and clear reporting.

---

## Behavior

When `/compare-ur10` is triggered:

### Step 1: Select Metrics
Which metrics to compare?
- [ ] Repeatability (mm)
- [ ] Accuracy (mm)
- [ ] Cycle time (ms)
- [ ] Payload capacity (kg)
- [ ] Reach (mm)
- [ ] Settling time (ms)
- [ ] Other: ___

### Step 2: Gather References
- UR-10 datasheet specifications
- EB-15 experimental data from `dados/eb15/`
- ISO 9283 test methodology (if applicable)

### Step 3: Apply Fair Comparison Rules
> 🔴 **Same test protocol for both robots.**
> 🔴 **Report conditions (temperature, payload, speed).**
> 🔴 **Report number of repetitions and statistical measures.**

### Step 4: Generate Comparison

```markdown
## Comparação: EB-15 vs UR-10

### Condições do Teste
- Data: [date]
- Protocolo: [description]
- Repetições: N = [number]

### Resultados

| Métrica | EB-15 (μ ± σ) | UR-10 (ref) | Razão | Observação |
|---------|---------------|-------------|-------|------------|
| ...     | ...           | ...         | ...   | ...        |

### Gráfico Comparativo
![Comparison](dados/graficos/eb15_vs_ur10_[metric].png)

### Análise
- [What the numbers mean]
- [Limitations of the comparison]
- [Implications for the research question]
```

### Step 5: Save
- Save to `relatorios/comparacao-[metric]-[date].md`
- Save charts to `dados/graficos/`

---

## Examples

```
/compare-ur10 repetibilidade
/compare-ur10 tempo de ciclo em pick and place
/compare-ur10 todas as métricas
```

---

## Key Principles

- **Fairness** — same protocol, same conditions
- **Transparency** — report all data, not just favorable results
- **Context** — UR-10 costs ~$35K; EB-15 is a low-cost prototype
- **Academic honesty** — acknowledge where comparison is limited
