# API REST Local — EB-15

## 1. Informações Gerais

| Parâmetro | Valor |
|---|---|
| Protocolo | HTTP/1.1 |
| Content-Type | application/json |
| Host padrão | 192.168.1.100 |
| Porta | 80 |
| Autenticação | Nenhuma (rede local) |

---

## 2. Endpoints

### 2.1 Status e Monitoramento

#### `GET /api/status`
Retorna estado geral do robô.

**Response 200:**
```json
{
  "state": "READY",
  "fault": "NONE",
  "homed": true,
  "uptime_s": 3600,
  "arduino_connected": true,
  "wifi_rssi": -45
}
```

#### `GET /api/joints`
Retorna posição estimada de todas as juntas.

**Response 200:**
```json
{
  "joints": {
    "j1": { "deg": 0.0, "steps": 0, "homed": true },
    "j2": { "deg": 45.0, "steps": 3200, "homed": true },
    "j3": { "deg": -30.0, "steps": -2133, "homed": true },
    "j4": { "deg": 0.0, "steps": 0, "homed": true },
    "j5": { "deg": 90.0, "steps": 6400, "homed": true },
    "j6": { "deg": 0.0, "steps": 0, "homed": true }
  }
}
```

#### `GET /api/pose`
Retorna pose cartesiana atual do TCP.

**Response 200:**
```json
{
  "pose": {
    "x": 250.0,
    "y": 0.0,
    "z": 300.0,
    "rx": 0.0,
    "ry": 90.0,
    "rz": 0.0
  },
  "tcp_offset": { "x": 0, "y": 0, "z": 0, "rx": 0, "ry": 0, "rz": 0 }
}
```

#### `GET /api/logs`
Retorna últimos logs e faults.

**Response 200:**
```json
{
  "logs": [
    { "t": 1234567, "level": "INFO", "msg": "Homing completed" },
    { "t": 1234560, "level": "WARN", "msg": "Serial retry on segment 3" }
  ]
}
```

---

### 2.2 Controle de Movimento

#### `POST /api/home`
Inicia sequência de homing.

**Request (opcional):**
```json
{
  "axes": [1, 2, 3, 4, 5, 6]
}
```
Se omitido, faz homing de todos os eixos.

**Response 200:**
```json
{ "status": "ok", "message": "Homing started" }
```

**Response 409:**
```json
{ "status": "error", "message": "Cannot home while moving" }
```

#### `POST /api/stop`
Parada imediata de todos os eixos.

**Response 200:**
```json
{ "status": "ok", "message": "Emergency stop executed" }
```

#### `POST /api/pause`
Pausa o movimento atual.

**Response 200:**
```json
{ "status": "ok", "message": "Motion paused" }
```

#### `POST /api/resume`
Retoma movimento pausado.

**Response 200:**
```json
{ "status": "ok", "message": "Motion resumed" }
```

#### `POST /api/move/joints`
Movimento no espaço de juntas.

**Request:**
```json
{
  "target": {
    "j1": 30.0,
    "j2": 45.0,
    "j3": -20.0,
    "j4": 0.0,
    "j5": 90.0,
    "j6": 0.0
  },
  "speed_pct": 50,
  "wait": true
}
```

| Campo | Tipo | Obrigatório | Descrição |
|---|---|---|---|
| target | object | Sim | Ângulos alvo em graus |
| speed_pct | int | Não (default: 50) | Velocidade 1–100% |
| wait | bool | Não (default: false) | Se true, responde ao finalizar |

**Response 200:**
```json
{ "status": "ok", "message": "Motion started", "segments": 12 }
```

**Response 400:**
```json
{ "status": "error", "message": "Joint j2 exceeds limit (max: 135.0)" }
```

#### `POST /api/move/linear`
Movimento linear cartesiano.

**Request:**
```json
{
  "target": {
    "x": 200.0,
    "y": 100.0,
    "z": 250.0,
    "rx": 0.0,
    "ry": 90.0,
    "rz": 45.0
  },
  "speed_mm_s": 50.0,
  "wait": false
}
```

**Response 200:**
```json
{ "status": "ok", "message": "Linear motion started", "segments": 24 }
```

**Response 400:**
```json
{ "status": "error", "message": "IK failed: target unreachable" }
```

---

### 2.3 Cinemática (Consulta)

#### `POST /api/fk`
Calcula cinemática direta sem mover.

**Request:**
```json
{
  "joints": { "j1": 0, "j2": 45, "j3": -30, "j4": 0, "j5": 90, "j6": 0 }
}
```

**Response 200:**
```json
{
  "pose": { "x": 250.0, "y": 0.0, "z": 300.0, "rx": 0.0, "ry": 90.0, "rz": 0.0 },
  "valid": true
}
```

#### `POST /api/ik`
Calcula cinemática inversa sem mover.

**Request:**
```json
{
  "pose": { "x": 200, "y": 100, "z": 250, "rx": 0, "ry": 90, "rz": 0 }
}
```

**Response 200:**
```json
{
  "joints": { "j1": 26.57, "j2": 38.2, "j3": -12.5, "j4": 0.0, "j5": 83.7, "j6": 0.0 },
  "valid": true,
  "solutions": 1
}
```

**Response 400:**
```json
{ "valid": false, "message": "No IK solution found" }
```

---

### 2.4 Obstáculos

#### `POST /api/obstacles`
Define lista de obstáculos ativos.

**Request:**
```json
{
  "obstacles": [
    { "type": "sphere", "center": [200, 0, 100], "radius": 50 },
    { "type": "box", "min": [100, -50, 0], "max": [200, 50, 100] }
  ]
}
```

**Response 200:**
```json
{ "status": "ok", "count": 2 }
```

#### `GET /api/obstacles`
Lista obstáculos ativos.

**Response 200:**
```json
{
  "obstacles": [
    { "id": 0, "type": "sphere", "center": [200, 0, 100], "radius": 50 },
    { "id": 1, "type": "box", "min": [100, -50, 0], "max": [200, 50, 100] }
  ]
}
```

---

### 2.5 Planejamento de Trajetória

#### `POST /api/plan`
Planeja trajetória sem executar.

**Request:**
```json
{
  "type": "moveJ",
  "target": { "j1": 30, "j2": 45, "j3": -20, "j4": 0, "j5": 90, "j6": 0 },
  "speed_pct": 50,
  "check_obstacles": true
}
```

**Response 200:**
```json
{
  "valid": true,
  "segments": 12,
  "duration_ms": 2400,
  "collision_free": true
}
```

**Response 200 (colisão):**
```json
{
  "valid": false,
  "collision_at_segment": 5,
  "obstacle_id": 0,
  "message": "Collision with sphere at segment 5"
}
```

---

### 2.6 Receitas / Programas

#### `POST /api/recipe`
Enviar e executar uma receita.

**Request:**
```json
{
  "name": "pick_and_place",
  "steps": [
    { "cmd": "moveJ", "target": { "j1": 0, "j2": 0, "j3": 0, "j4": 0, "j5": 0, "j6": 0 }, "speed_pct": 80 },
    { "cmd": "moveL", "target": { "x": 200, "y": 100, "z": 50, "rx": 0, "ry": 90, "rz": 0 }, "speed_mm_s": 30 },
    { "cmd": "delay", "ms": 500 },
    { "cmd": "moveL", "target": { "x": 200, "y": 100, "z": 200, "rx": 0, "ry": 90, "rz": 0 }, "speed_mm_s": 30 },
    { "cmd": "moveJ", "target": { "j1": 90, "j2": 0, "j3": 0, "j4": 0, "j5": 0, "j6": 0 }, "speed_pct": 60 }
  ],
  "loop": false,
  "execute": true
}
```

**Response 200:**
```json
{ "status": "ok", "message": "Recipe started", "total_steps": 5 }
```

#### `GET /api/recipe/status`
Status da receita em execução.

**Response 200:**
```json
{
  "running": true,
  "name": "pick_and_place",
  "current_step": 2,
  "total_steps": 5,
  "paused": false
}
```

---

### 2.7 Configuração

#### `GET /api/config`
Leitura da configuração.

**Response 200:**
```json
{
  "wifi": { "ssid": "MyNetwork", "ip": "192.168.1.100" },
  "joints": {
    "j1": { "min_deg": -170, "max_deg": 170, "steps_per_deg": 71.11, "max_speed_dps": 90 },
    "j2": { "min_deg": -90, "max_deg": 135, "steps_per_deg": 71.11, "max_speed_dps": 90 }
  },
  "tcp_offset": { "x": 0, "y": 0, "z": 50, "rx": 0, "ry": 0, "rz": 0 }
}
```

#### `POST /api/config`
Atualizar configuração (parcial).

**Request:**
```json
{
  "tcp_offset": { "x": 0, "y": 0, "z": 65, "rx": 0, "ry": 0, "rz": 0 }
}
```

**Response 200:**
```json
{ "status": "ok", "message": "Configuration updated" }
```

---

## 3. Códigos de Erro HTTP

| Código | Significado |
|---|---|
| 200 | Sucesso |
| 400 | Parâmetros inválidos |
| 404 | Endpoint não encontrado |
| 409 | Conflito de estado (ex: mover sem homing) |
| 500 | Erro interno |

---

## 4. Jog (Movimento Incremental)

#### `POST /api/jog`
Movimento incremental de uma junta.

**Request:**
```json
{
  "joint": 1,
  "direction": 1,
  "speed_pct": 20
}
```

| Campo | Valores | Descrição |
|---|---|---|
| joint | 1–6 | Número da junta |
| direction | -1 ou 1 | Sentido |
| speed_pct | 1–100 | Velocidade |

**Response 200:**
```json
{ "status": "ok" }
```

#### `POST /api/jog/stop`
Para o jog.

**Response 200:**
```json
{ "status": "ok" }
```
