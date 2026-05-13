---
name: hardware-testing
description: Hardware testing procedures for embedded systems. Flash, serial monitor, oscilloscope, multimeter, validation checklists. Use when testing firmware on physical hardware.
allowed-tools: Read, Write, Edit
version: 1.0
---

# Hardware Testing — Embedded Validation

> Systematic procedures for testing firmware on real ESP32 and Arduino hardware.

---

## Test Pyramid for Embedded

```
        ┌──────────┐
        │  System   │  ← Full robot operation (rare, expensive)
       ┌┴──────────┴┐
       │ Integration │  ← ESP ↔ Arduino communication
      ┌┴────────────┴┐
      │  Unit (Board) │  ← Single MCU, isolated function
     ┌┴──────────────┴┐
     │   Compilation   │  ← Does it build without errors?
     └────────────────┘
```

---

## Test Stages

### Stage 1: Compilation

| Check | Command/Tool | Pass Criteria |
|---|---|---|
| ESP32 firmware compiles | Arduino IDE → Verify | 0 errors, 0 critical warnings |
| Arduino Uno firmware compiles | Arduino IDE → Verify | 0 errors, 0 critical warnings |
| RAM/Flash usage | Compiler output | Within limits |

### Stage 2: Unit (Single Board)

| Test | Tool | What to Check |
|---|---|---|
| Serial output | Serial Monitor (115200) | Boot messages, state transitions |
| Wi-Fi connection | Browser → IP | HTTP response from ESP |
| API endpoints | curl / Postman | JSON responses match spec |
| Motor pulse | Oscilloscope on PUL pin | Square wave at expected frequency |
| Direction signal | Multimeter on DIR pin | HIGH/LOW toggles correctly |
| NeoPixel | Visual inspection | Correct color for each state |

### Stage 3: Integration (ESP ↔ Arduino)

| Test | Procedure | Expected |
|---|---|---|
| Heartbeat | Power both, monitor serial | Bidirectional heartbeat established |
| Segment delivery | Send MoveJ via API | Arduino receives and ACKs segments |
| Fault propagation | Disconnect Arduino | ESP detects FAULT_SERIAL_TIMEOUT |
| Recovery | Reconnect Arduino | Communication resumes after reset |

### Stage 4: System (Full Robot)

| Test | Procedure | Expected |
|---|---|---|
| Homing | Trigger via API/UI | All joints home to reference |
| Point-to-point | MoveJ to known position | Motors move, position updated |
| Trajectory | Multiple waypoints | Smooth coordinated movement |
| Emergency stop | Stop via API during move | Immediate halt |

---

## Tools Required

| Tool | Purpose | When |
|---|---|---|
| Arduino IDE 2.x | Compile and flash | Every code change |
| Serial Monitor | Debug output, protocol inspection | Every test |
| curl / Postman | API endpoint testing | After boot |
| Oscilloscope | Pulse timing, signal quality | Motor issues |
| Multimeter | Voltage levels, continuity | Wiring issues |
| Logic Analyzer | Serial protocol debugging | Protocol issues |

---

## Test Record Template

```markdown
## Test Record: [Test Name]

**Date:** YYYY-MM-DD
**Firmware versions:** ESP=[hash/version], Arduino=[hash/version]
**Hardware state:** [description]

### Results

| Test ID | Description | Result | Notes |
|---------|-------------|--------|-------|
| TC-XX   | [desc]      | ✅/❌ | [obs] |

### Issues Found
- [Issue description, severity, resolution]

### Follow-up Actions
- [ ] [Action item]
```

---

## Flash Procedure Checklist

- [ ] Correct board selected in Arduino IDE
- [ ] Correct COM port selected
- [ ] Upload speed appropriate (ESP32: 921600, Uno: 115200)
- [ ] Previous serial monitor closed (avoid port conflict)
- [ ] BOOT button held if ESP32 requires (DevKitC-1 usually auto-resets)
- [ ] Upload successful (no errors)
- [ ] Serial monitor opened after upload
- [ ] Boot messages appear correctly
