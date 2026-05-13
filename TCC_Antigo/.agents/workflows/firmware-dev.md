---
description: Develop a firmware feature for the EB-15. Reads scope, plans implementation, writes code, and guides testing.
---

# /firmware-dev - Firmware Feature Development

$ARGUMENTS

---

## Purpose

This command activates the firmware development workflow for the EB-15 robotic arm. It ensures every code change aligns with the system scope and ADRs.

---

## Behavior

When `/firmware-dev` is triggered:

### Step 1: Read Scope
- Read `Software e Firmware/docs/escopo.md`
- Read `Software e Firmware/docs/decisoes_arquiteturais.md`
- Read `Software e Firmware/docs/plano_implementacao.md`
- Identify which phase and task applies

### Step 2: Plan the Change
- Which firmware? (ESP32 / Arduino / Both)
- Which sections of the `.ino` are affected?
- What state transitions are involved?
- Impact on serial protocol?
- Memory implications?

**Present plan to user. Wait for approval.**

### Step 3: Implement
- Apply `firmware-specialist` agent expertise
- Follow monolithic `.ino` convention (sections, not files)
- Non-blocking patterns only
- Safety checks (soft limits, state validation)
- Update section comments if new section added

### Step 4: Verify
- [ ] Code aligns with scope and ADRs
- [ ] No `delay()` in main loop
- [ ] Memory usage acceptable
- [ ] State machine transitions correct
- [ ] Serial protocol maintained
- [ ] Soft limits checked

### Step 5: Update Implementation Plan
- Mark completed items in `plano_implementacao.md`

---

## Output Format

```markdown
## ⚡ Firmware Change: [Feature Name]

### Files Modified
- `Software e Firmware/Firmware Final/esp32/eb15_esp32_supervisor.ino` — [description]
- `Software e Firmware/Firmware Final/arduino_uno/eb15_uno_executor.ino` — [description]

### Sections Affected
- [Section name]: [what changed]

### Testing Required
- [ ] [Test case 1]
- [ ] [Test case 2]

### Implementation Plan Updated
- [x] [Completed item]
```

---

## Examples

```
/firmware-dev implementar homing real para J1-J3
/firmware-dev adicionar indicador LED de estado ao NeoPixel
/firmware-dev implementar IK analítica
/firmware-dev otimizar perfil trapezoidal
```

---

## Key Principles

- **Read scope first** — every change must align with the system contract
- **Safety first** — movement code must respect limits and state machine
- **Non-blocking** — never use `delay()` in firmware loop
- **Document** — update ADRs if a new architectural decision is made
