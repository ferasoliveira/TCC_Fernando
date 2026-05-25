---
description: Compile, flash, and test firmware. Guides through build verification, flash procedure, and test recording.
---

# /flash-test - Firmware Build & Test

$ARGUMENTS

---

## Purpose

This command guides the process of compiling, flashing, and testing firmware on physical hardware.

---

## Behavior

When `/flash-test` is triggered:

### Step 1: Pre-flight
- Which firmware? (ESP32 / Arduino / Both)
- Check board selection in Arduino IDE
- Check COM port availability

### Step 2: Compile
- Verify firmware compiles without errors
- Check memory usage (RAM and Flash)
- Report any warnings

### Step 3: Flash (User Action)
```
⚡ Ready to flash. Please:
1. Connect [board] via USB
2. Select correct COM port in Arduino IDE
3. Click Upload
4. Confirm upload success

Report back when complete.
```

### Step 4: Test
- Open Serial Monitor at 115200 baud
- Verify boot sequence
- Run applicable test cases from `plano_testes.md`
- Record results

### Step 5: Record
Save test record to `relatorios/`:

```markdown
## Flash & Test: [Date]

**Board:** [ESP32 / Arduino]
**Firmware:** [filename]
**Result:** ✅ Pass / ❌ Fail

| Test | Result | Notes |
|------|--------|-------|
| Boot | ✅/❌ | ... |
| ...  | ...    | ... |
```

---

## Examples

```
/flash-test ESP32 após mudança no homing
/flash-test Arduino Uno novo protocolo
/flash-test ambos para teste de integração
```
