---
name: bibliography-management
description: Bibliography management principles. Source organization, citation keys, article summarization, reference file maintenance. Use when searching, organizing, or citing references.
allowed-tools: Read, Write, Edit
version: 1.0
---

# Bibliography Management

> Organized, traceable, and correctly formatted references for the TCC.

---

## Reference Organization

### File Structure

```
docs/referencias/
├── fontes.md              # Master reference list (ABNT format)
├── resumos/               # Article summaries
│   ├── siciliano2009.md
│   ├── ur10-datasheet.md
│   └── ...
└── notas/                 # Research notes by topic
    ├── robotica-baixo-custo.md
    ├── iot-embarcado.md
    └── ...
```

### Citation Key Convention

```
authorYEAR       →  siciliano2009
authorYEARtopic  →  silva2020iot (when same author has multiple works in same year)
```

---

## Reference Entry Template

```markdown
## [Citation Key]

**ABNT:**
SOBRENOME, Nome. Título. Edição. Cidade: Editora, Ano.

**Type:** Book | Article | Conference | Website | Datasheet | Thesis

**Relevance:** [Why this source matters for the TCC]

**Key findings:**
- [Main point 1]
- [Main point 2]

**Used in chapters:** [List of chapters that cite this source]

**Status:** ✅ Read | 📖 Reading | ⏳ To read
```

---

## Article Summary Template

```markdown
# Summary: [Title]

**Citation key:** [key]
**Authors:** [names]
**Year:** [year]
**Source:** [journal/conference/publisher]

## Main Contribution
[1-2 sentences]

## Key Concepts
- [Concept 1]
- [Concept 2]

## Methodology
[Brief description of how the research was conducted]

## Results
[Key findings relevant to TCC]

## Relevance to TCC
[How this connects to the EB-15 project]

## Quotes (with page numbers)
- "[direct quote]" (p. XX)
```

---

## Search Strategy

| Database | Best For |
|---|---|
| Google Scholar | Broad academic search |
| IEEE Xplore | Robotics, embedded systems, automation |
| SciELO | Brazilian academic publications |
| Scopus | Citation analysis, impact factor |
| Manufacturer sites | Datasheets (UR-10, ESP32, NEMA 17, TB6600) |

### Search Terms (PT/EN)

| Portuguese | English |
|---|---|
| braço robótico de baixo custo | low-cost robotic arm |
| automação didática | educational automation |
| robótica IoT | IoT robotics |
| cinemática inversa | inverse kinematics |
| motor de passo | stepper motor |
| ESP32 robótica | ESP32 robotics |

---

## Quality Checklist

- [ ] Every reference in `fontes.md` follows ABNT NBR 6023
- [ ] Every citation in text has a corresponding entry in `fontes.md`
- [ ] No orphan references (cited but not in list, or in list but not cited)
- [ ] Summaries exist for all primary references
- [ ] DOI or URL included when available
- [ ] Access date included for web sources
