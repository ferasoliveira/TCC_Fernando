---
name: experiment-design
description: Experimental design principles for robotics validation. Variables, controls, metrics, repetitions, protocols, data collection. Use when planning or analyzing experiments.
allowed-tools: Read, Write, Edit
version: 1.0
---

# Experiment Design — Robotics Validation

> Structured approach to planning, executing, and analyzing experiments for the EB-15 vs UR-10 comparison.

---

## Experiment Planning Framework

### 1. Define the Question

| Field | Example |
|---|---|
| **Research question** | "Is the EB-15 repeatable within ±2mm for point-to-point moves?" |
| **Null hypothesis (H0)** | "The EB-15 shows no significant difference in repeatability vs random placement." |
| **Alternative (H1)** | "The EB-15 achieves repeatability < ±5mm over 30 repetitions." |

### 2. Identify Variables

| Type | Description | Example |
|---|---|---|
| **Independent** | What you control/change | Target position, speed, payload |
| **Dependent** | What you measure | Actual position, time, deviation |
| **Controlled** | What stays constant | Temperature, power supply, firmware version |

### 3. Define Metrics

| Metric | Unit | How to Measure |
|---|---|---|
| **Repeatability** | mm | Std deviation of position over N repetitions |
| **Accuracy** | mm | Mean absolute error from target |
| **Cycle time** | ms | Time from command to idle |
| **Settling time** | ms | Time for oscillation to stop |
| **Stability** | mm/min | Drift over sustained hold |

### 4. Plan Repetitions

| Guideline | Value |
|---|---|
| Minimum repetitions per test | 10 (ideal: 30) |
| Warm-up runs (discard) | 3–5 |
| Cool-down between sets | 30s minimum |

---

## Test Protocol Template

```markdown
## Experiment: [Name]

### Objective
[What is being tested and why]

### Setup
- Robot: [EB-15 / UR-10]
- Firmware version: [X.Y]
- Date: [YYYY-MM-DD]
- Environment: [Temperature, conditions]

### Procedure
1. Home the robot
2. [Step-by-step instructions]
3. Record data for each repetition

### Data Collection
| Run # | Metric 1 | Metric 2 | Notes |
|-------|----------|----------|-------|
| 1     |          |          |       |
| ...   |          |          |       |

### Expected Outcome
[What constitutes success/failure]
```

---

## Comparison Framework (EB-15 vs UR-10)

| Criterion | EB-15 Measurement | UR-10 Reference | Source |
|---|---|---|---|
| Repeatability | Measured experimentally | ±0.1 mm (datasheet) | Universal Robots spec |
| Payload | Measured | 10 kg | Datasheet |
| Reach | Measured | 1300 mm | Datasheet |
| Cycle time | Measured | Measured in lab | Same test protocol |

> **Fair comparison rule:** Use the SAME test protocol on both robots. Differences should come from robot capability, not test methodology.

---

## Data Recording Rules

| Rule | Why |
|---|---|
| Record raw data, not processed | Allows re-analysis later |
| Include timestamps | Correlation with events |
| Note anomalies in real-time | Don't rely on memory |
| Version control data files | Traceability |
| CSV format with headers | Universal, easy to process |

---

## Common Pitfalls

| ❌ Pitfall | ✅ Prevention |
|---|---|
| Running tests without homing first | Always home before test set |
| Comparing hot robot vs cold robot | Warm-up procedure for both |
| Ignoring environmental factors | Record temperature, humidity |
| Too few repetitions | Minimum 10, prefer 30 |
| Cherry-picking good runs | Report ALL runs, mention outliers |
