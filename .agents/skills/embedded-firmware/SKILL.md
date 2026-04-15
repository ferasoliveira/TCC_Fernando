---
name: embedded-firmware
description: C/C++ embedded development principles for ESP32 and Arduino. Memory management, GPIO, ISRs, serial protocols, PROGMEM, NVS, watchdog. Use when developing or reviewing firmware.
allowed-tools: Read, Write, Edit, Glob, Grep
version: 1.0
priority: HIGH
---

# Embedded Firmware — C/C++ for Microcontrollers

> Principles for writing robust firmware on resource-constrained hardware (ESP32-S3, Arduino Uno).

---

## Core Constraints

| Platform | Flash | RAM | Key Limitation |
|---|---|---|---|
| ESP32-S3 | ~8 MB | ~512 KB | Multi-task via FreeRTOS, Wi-Fi stack consumes RAM |
| Arduino Uno | 32 KB | 2 KB | Extremely limited RAM — every byte counts |

---

## Memory Management Principles

| Rule | Why |
|---|---|
| **Avoid dynamic allocation** (`malloc`/`new`) | Fragmentation on small heaps causes crashes |
| **Use stack variables** for short-lived data | Automatically freed, no fragmentation |
| **Use `PROGMEM`** for constant data (strings, HTML, lookup tables) | Keeps RAM free on AVR/ESP |
| **Pre-allocate buffers** with fixed size | Predictable memory usage |
| **Measure RAM usage** with `ESP.getFreeHeap()` or `freeMemory()` | Monitor before deploying |

### PROGMEM Pattern (ESP32)

```cpp
// Store large strings in flash
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html>...</html>
)rawliteral";

// Serve from flash
server.send_P(200, "text/html", HTML_PAGE);
```

---

## GPIO and Motor Control

| Principle | Detail |
|---|---|
| **Configure pins in `setup()`** | Never change `pinMode` at runtime unless necessary |
| **Use hardware timers** for pulse generation | `timerAlarmWrite()` on ESP32 for precise step timing |
| **Debounce limit switches** | 10–50 ms debounce for mechanical switches |
| **Pull-up/pull-down explicitly** | Don't rely on floating pins |

### Stepper Motor Pattern

```cpp
// Pulse generation — one step
void stepMotor(uint8_t pulPin, uint8_t dirPin, bool direction, uint16_t delayUs) {
    digitalWrite(dirPin, direction);
    digitalWrite(pulPin, HIGH);
    delayMicroseconds(delayUs);
    digitalWrite(pulPin, LOW);
    delayMicroseconds(delayUs);
}
```

---

## Serial Protocol Principles

| Rule | Why |
|---|---|
| **Use binary framing** (START + CMD + PAYLOAD + CRC + END) | Efficient, reliable, deterministic size |
| **CRC8 on every frame** | Detects corruption |
| **ACK/NACK with timeout** | Guarantees delivery or triggers fault |
| **Buffer circular for queue** | Predictable memory on Arduino Uno |
| **Heartbeat periódico** | Detects disconnection |

### Frame Structure Reference

```
[0xAA] [CMD] [LEN] [PAYLOAD...] [CRC8] [0x55]
  ^                                        ^
  START                                   END
```

---

## State Machine Pattern

```cpp
enum RobotState { BOOT, IDLE, HOMING, READY, MOVING, PAUSED, FAULT };
RobotState currentState = BOOT;

void loop() {
    switch (currentState) {
        case BOOT:    handleBoot();    break;
        case IDLE:    handleIdle();    break;
        case HOMING:  handleHoming();  break;
        case READY:   handleReady();   break;
        case MOVING:  handleMoving();  break;
        case PAUSED:  handlePaused();  break;
        case FAULT:   handleFault();   break;
    }
}
```

> **Rule:** All state transitions must be explicit and validated. Never allow direct jumps between non-adjacent states.

---

## Persistence (NVS / SPIFFS)

| Storage | Use For | Format |
|---|---|---|
| NVS | Small config (Wi-Fi creds, joint limits, offsets) | Key-value |
| SPIFFS/LittleFS | Large files (recipes, logs, web assets) | JSON/binary |

```cpp
// NVS read/write pattern
Preferences prefs;
prefs.begin("config", false);
float limit = prefs.getFloat("j1_max", 170.0);
prefs.putFloat("j1_max", newLimit);
prefs.end();
```

---

## Safety Checklist (Firmware)

- [ ] Soft limits checked before every movement command
- [ ] Watchdog timer enabled and fed regularly
- [ ] Serial timeout triggers fault state
- [ ] Homing required before movement
- [ ] Emergency stop halts all motors immediately
- [ ] Invalid API payloads rejected with appropriate HTTP status

---

## Anti-Patterns

| ❌ Don't | ✅ Do |
|---|---|
| `delay()` in main loop | Use non-blocking timing (`millis()`) |
| `String` on Arduino Uno | Use `char[]` with fixed buffers |
| Global interrupt disable for long periods | Minimize critical sections |
| Ignore return values of serial writes | Check and retry or fault |
| Hardcode pin numbers scattered in code | Define as constants at top |
