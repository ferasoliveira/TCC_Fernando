---
description: Search, summarize, and organize academic references. Guides literature review and bibliography management.
---

# /bibliography - Literature Research & Management

$ARGUMENTS

---

## Purpose

This command activates a structured literature research workflow, helping find, summarize, and organize references for the TCC.

---

## Behavior

When `/bibliography` is triggered:

### Step 1: Define Scope
- What topic needs references?
- Which chapter will use them?
- How many references are needed?
- Language preference (PT/EN/both)?

### Step 2: Search Strategy
Using `research-assistant` agent:
- Define search terms (Portuguese and English)
- Select databases (Google Scholar, IEEE Xplore, SciELO, Scopus)
- Prioritize: academic papers > books > conference proceedings > web

### Step 3: Review & Summarize
For each relevant source found:
- Create summary in `docs/referencias/resumos/[key].md`
- Format reference entry in ABNT NBR 6023
- Extract key quotes with page numbers
- Assess relevance to TCC

### Step 4: Register
- Add to `docs/referencias/fontes.md`
- Tag with chapters where it will be cited
- Mark status (✅ Read / 📖 Reading / ⏳ To read)

---

## Output Format

```markdown
## 📚 Bibliography Update: [Topic]

### References Found: X
### Added to fontes.md: Y
### Summaries created: Z

### New References
| Key | Type | Relevance | Status |
|-----|------|-----------|--------|
| [key] | [type] | [high/med/low] | [status] |

### Suggested Reading Order
1. [Most important first]
2. ...
```

---

## Examples

```
/bibliography robótica de baixo custo e educacional
/bibliography cinemática inversa para manipuladores 6-DOF
/bibliography ESP32 IoT applications in robotics
/bibliography UR-10 specifications and comparisons
```
