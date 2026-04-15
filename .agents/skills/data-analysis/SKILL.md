---
name: data-analysis
description: Data analysis principles for experimental robotics data. Descriptive statistics, comparison, tables, charts with Python/matplotlib/pandas. Use when analyzing collected data.
allowed-tools: Read, Write, Edit, Bash
version: 1.0
---

# Data Analysis — Experimental Robotics Data

> Transform raw experimental data into insights, tables, and charts for the TCC.

---

## Analysis Pipeline

```
Raw CSV → Load (pandas) → Clean → Describe → Compare → Visualize → Report
```

---

## Descriptive Statistics

| Statistic | Purpose | Python |
|---|---|---|
| **Mean** | Central tendency | `df['col'].mean()` |
| **Std Dev** | Spread/variability | `df['col'].std()` |
| **Min / Max** | Range | `df['col'].min()`, `.max()` |
| **Median** | Robust central tendency | `df['col'].median()` |
| **CV (%)** | Relative variability | `(std / mean) * 100` |

---

## Comparison Table Template

```python
import pandas as pd

comparison = pd.DataFrame({
    'Metric': ['Repeatability (mm)', 'Cycle Time (ms)', 'Accuracy (mm)'],
    'EB-15 Mean': [eb15_repeat, eb15_cycle, eb15_accuracy],
    'EB-15 Std': [eb15_r_std, eb15_c_std, eb15_a_std],
    'UR-10 Ref': [0.1, ur10_cycle, ur10_accuracy],
    'Ratio': [eb15_repeat/0.1, eb15_cycle/ur10_cycle, eb15_accuracy/ur10_accuracy]
})
```

---

## Visualization Principles

| Chart Type | Use For | Library |
|---|---|---|
| **Bar chart** | Comparing metrics between EB-15 and UR-10 | matplotlib |
| **Box plot** | Distribution of repeated measurements | matplotlib/seaborn |
| **Line chart** | Trajectory or time-series data | matplotlib |
| **Scatter plot** | Correlation between variables | matplotlib |
| **Table** | Precise numerical comparison | pandas |

### Chart Template

```python
import matplotlib.pyplot as plt

fig, ax = plt.subplots(figsize=(8, 5))
ax.bar(['EB-15', 'UR-10'], [eb15_value, ur10_value], color=['#2196F3', '#FF5722'])
ax.set_ylabel('Repeatability (mm)')
ax.set_title('Repeatability Comparison: EB-15 vs UR-10')
ax.grid(axis='y', alpha=0.3)
plt.tight_layout()
plt.savefig('dados/graficos/repeatability_comparison.png', dpi=150)
```

---

## File Organization

```
dados/
├── eb15/
│   ├── raw/               # Raw CSV files from experiments
│   ├── processed/         # Cleaned and processed data
│   └── graficos/          # Generated charts
├── ur10/
│   ├── raw/
│   └── processed/
└── comparacoes/           # Comparison analyses
    ├── repeatability.csv
    └── cycle_time.csv
```

---

## Reporting Pattern

```markdown
## Resultado: [Nome do Experimento]

### Dados Coletados
- N = [número de repetições]
- Período: [data início] a [data fim]

### Estatísticas Descritivas

| Métrica | Média | Desvio Padrão | Mín | Máx |
|---------|-------|---------------|-----|-----|
| [...]   | ...   | ...           | ... | ... |

### Visualização
![Gráfico comparativo](dados/graficos/nome_grafico.png)

### Análise
[Interpretação dos resultados, comparação com UR-10, significância]
```

---

## Anti-Patterns

| ❌ Don't | ✅ Do |
|---|---|
| Report only mean without std dev | Always report mean ± std |
| Use pie charts for comparison | Use bar or box plots |
| Truncate axis to exaggerate differences | Start axis at 0 or clearly indicate |
| Mix units in the same chart | One unit per axis |
| Present data without context | Always compare with reference value |
