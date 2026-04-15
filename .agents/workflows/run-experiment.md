---
description: Plan, execute, and analyze a robotics experiment. Guides from hypothesis to data collection and reporting.
---

# /run-experiment - Experimental Execution

$ARGUMENTS

---

## Purpose

This command guides the execution of a robotics experiment for the EB-15, from protocol selection to data analysis and reporting.

---

## Behavior

When `/run-experiment` is triggered:

### Step 1: Select Experiment
- What is being tested? (repeatability, accuracy, cycle time, etc.)
- Which test from `plano_testes.md` applies?
- What is the hypothesis?

### Step 2: Prepare Protocol
Using `experiment-analyst` agent:
- Define variables (independent, dependent, controlled)
- Set number of repetitions (minimum 10)
- Define warm-up procedure
- Prepare data collection template (CSV)

### Step 3: Execute (User Action)
```
🔬 Experiment ready. Please:
1. Home the robot
2. Follow the protocol step by step
3. Record data in the provided CSV template
4. Note any anomalies

Report back when data collection is complete.
```

### Step 4: Analyze
- Load CSV data
- Compute descriptive statistics (mean, std, min, max)
- Generate comparison visualizations
- Flag outliers or anomalies

### Step 5: Report
Save analysis to `relatorios/`:

```markdown
## Experiment Report: [Name]

**Date:** [YYYY-MM-DD]
**N:** [repetitions]

### Statistics
| Metric | Mean | Std | Min | Max |
|--------|------|-----|-----|-----|
| ...    | ...  | ... | ... | ... |

### Charts
![Chart](dados/graficos/[name].png)

### Conclusion
[Interpretation relative to research questions]
```

---

## Examples

```
/run-experiment teste de repetibilidade J1
/run-experiment ciclo de pick and place
/run-experiment tempo de resposta MoveJ 6 eixos
```
