# Protocolo Serial ESP32 ↔ Arduino — EB-15

## 1. Camada Física

| Parâmetro | Valor |
|---|---|
| Interface | UART |
| Baud rate | 115200 |
| Data bits | 8 |
| Stop bits | 1 |
| Parity | None |
| Flow control | None |
| Pinos ESP32 | TX=GPIO20, RX=GPIO19 (c/ divisor 5V→3.3V) |
| Pinos Arduino | RX=A4, TX=A5 |

> **Nota:** Usar level shifter 3.3V↔5V entre ESP32 e Arduino.

---

## 2. Formato de Frame

```
┌──────┬──────┬───────┬─────────────────┬──────┬──────┐
│ START│ LEN  │  CMD  │    PAYLOAD      │ CRC8 │  END │
│ 0xAA │ 1B   │  1B   │  0..N bytes     │ 1B   │ 0x55 │
└──────┴──────┴───────┴─────────────────┴──────┴──────┘

START  = 0xAA (byte de início)
LEN   = comprimento total de CMD + PAYLOAD (não inclui START, LEN, CRC, END)
CMD   = código do comando
PAYLOAD = dados específicos do comando (0 a N bytes)
CRC8  = CRC-8 sobre bytes de LEN + CMD + PAYLOAD
END   = 0x55 (byte de fim)
```

**Tamanho máximo de frame:** 64 bytes (incluindo header/footer)
**Payload máximo:** 59 bytes

---

## 3. CRC-8

Polinômio: `0x07` (CRC-8/ITU)

```c
uint8_t crc8(const uint8_t* data, uint8_t len) {
    uint8_t crc = 0x00;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x07;
            else
                crc = crc << 1;
        }
    }
    return crc;
}
```

---

## 4. Comandos

### 4.1 Tabela de Comandos

| Código | Nome | Direção | Payload | Descrição |
|---|---|---|---|---|
| `0x01` | HEARTBEAT | Bidirecional | 0 bytes | Sinal de vida |
| `0x02` | ACK | Bidirecional | 1 byte (cmd acknowledged) | Confirmação |
| `0x03` | NACK | Bidirecional | 2 bytes (cmd + error code) | Rejeição |
| `0x10` | SEGMENT | ESP→Arduino | 16 bytes | Segmento de movimento |
| `0x11` | STOP | ESP→Arduino | 0 bytes | Parada imediata |
| `0x12` | HOME | ESP→Arduino | 1 byte (mask) | Iniciar homing |
| `0x13` | STATUS_REQ | ESP→Arduino | 0 bytes | Solicitar status |
| `0x14` | SET_POSITION | ESP→Arduino | 12 bytes | Definir posição atual |
| `0x20` | STATUS_RSP | Arduino→ESP | 17 bytes | Resposta de status |
| `0x21` | FAULT_NOTIFY | Arduino→ESP | 2 bytes | Notificação de falha |
| `0x22` | HOME_DONE | Arduino→ESP | 1 byte (result) | Homing concluído |
| `0x23` | SEGMENT_DONE | Arduino→ESP | 1 byte (seq) | Segmento executado |

---

### 4.2 Detalhes dos Payloads

#### SEGMENT (0x10) — ESP → Arduino
```
Offset  Bytes  Campo           Descrição
0       1      seq             Número de sequência (0-255)
1       2      steps_j4        Passos J4 (int16_t, sinalizado)
3       2      steps_j5        Passos J5 (int16_t, sinalizado)
5       2      steps_j6        Passos J6 (int16_t, sinalizado)
7       2      duration_ms     Duração do segmento em ms (uint16_t)
9       1      flags           Bit 0: último segmento
                               Bit 1: sincronizar
Total: 10 bytes
```

#### STATUS_REQ (0x13) — ESP → Arduino
Sem payload.

#### STATUS_RSP (0x20) — Arduino → ESP
```
Offset  Bytes  Campo           Descrição
0       1      state           Estado do executor (enum)
1       4      pos_j4          Posição J4 em passos (int32_t)
5       4      pos_j5          Posição J5 em passos (int32_t)
9       4      pos_j6          Posição J6 em passos (int32_t)
13      1      queue_free      Slots livres na fila
14      1      fault_code      Código de falha atual
15      1      seq_current     Sequência em execução
Total: 16 bytes
```

#### STOP (0x11) — ESP → Arduino
Sem payload. Efeito: parada imediata, limpa fila.

#### HOME (0x12) — ESP → Arduino
```
Offset  Bytes  Campo           Descrição
0       1      axis_mask       Bit 0=J4, Bit 1=J5, Bit 2=J6
Total: 1 byte
```

#### HOME_DONE (0x22) — Arduino → ESP
```
Offset  Bytes  Campo           Descrição
0       1      result          0=sucesso, 1=falha J4, 2=falha J5, 4=falha J6
Total: 1 byte
```

#### FAULT_NOTIFY (0x21) — Arduino → ESP
```
Offset  Bytes  Campo           Descrição
0       1      fault_code      Código de falha
1       1      detail          Detalhe adicional
Total: 2 bytes
```

---

## 5. Handshake e Heartbeat

### 5.1 Inicialização
1. ESP32 inicia e aguarda 1 segundo
2. ESP32 envia HEARTBEAT
3. Arduino responde com HEARTBEAT
4. Comunicação estabelecida

Se Arduino não responder após 3 tentativas (intervalo 500ms):
→ ESP32 marca FAULT_SERIAL_TIMEOUT

### 5.2 Heartbeat Periódico
- ESP32 envia HEARTBEAT a cada **500ms**
- Arduino deve responder com HEARTBEAT em até **200ms**
- Se 3 heartbeats consecutivos sem resposta → FAULT_SERIAL_TIMEOUT
- Arduino: se não receber heartbeat por **2000ms** → auto-stop, aguardar reconexão

---

## 6. Protocolo de Envio de Segmentos

### 6.1 Fluxo Normal
```
ESP32                               Arduino
  │                                    │
  ├──── SEGMENT (seq=0) ────────────►  │
  │                                    ├── ACK (0x10)
  │  ◄─────────────────────────────────┤
  │                                    │
  ├──── SEGMENT (seq=1) ────────────►  │
  │                                    ├── ACK (0x10)
  │  ◄─────────────────────────────────┤
  │                                    │
  │  ... Arduino executa seq=0 ...     │
  │                                    │
  │                                    ├── SEGMENT_DONE (seq=0)
  │  ◄─────────────────────────────────┤
  │                                    │
```

### 6.2 Fila Cheia
Se Arduino recebe SEGMENT mas fila está cheia:
→ Responde NACK (0x10, ERROR_QUEUE_FULL)
→ ESP32 reenvia após 50ms

### 6.3 Erro de CRC
Se CRC não bate:
→ Receptor envia NACK (cmd, ERROR_CRC)
→ Remetente reenvia (máx 3 tentativas)

### 6.4 Timeout
Se ACK não chegar em **100ms**:
→ Reenviar (máx 3 tentativas)
→ Após 3 falhas → FAULT_SERIAL_TIMEOUT

---

## 7. Códigos de Erro (NACK)

| Código | Nome | Descrição |
|---|---|---|
| `0x01` | ERROR_CRC | Falha de CRC |
| `0x02` | ERROR_UNKNOWN_CMD | Comando não reconhecido |
| `0x03` | ERROR_QUEUE_FULL | Fila de segmentos cheia |
| `0x04` | ERROR_INVALID_STATE | Comando inválido para estado atual |
| `0x05` | ERROR_PAYLOAD_SIZE | Tamanho de payload inválido |

---

## 8. Exemplos de Frames

### Heartbeat (ESP → Arduino)
```
AA 01 01 [CRC] 55
│  │  │    │    └─ END
│  │  │    └────── CRC8 de [01, 01]
│  │  └─────────── CMD = HEARTBEAT
│  └────────────── LEN = 1 (só CMD, sem payload)
└───────────────── START
```

### Segment (ESP → Arduino)
```
AA 0B 10 00 0064 FF9C 0032 03E8 01 [CRC] 55
│  │   │  │  │    │    │    │    │    │    └─ END
│  │   │  │  │    │    │    │    │    └────── CRC8
│  │   │  │  │    │    │    │    └─────────── flags = 0x01 (último)
│  │   │  │  │    │    │    └──────────────── duration = 1000ms
│  │   │  │  │    │    └───────────────────── steps_j6 = +50
│  │   │  │  │    └────────────────────────── steps_j5 = -100
│  │   │  │  └─────────────────────────────── steps_j4 = +100
│  │   │  └──────────────────────────────── seq = 0
│  │   └───────────────────────────────────── CMD = SEGMENT
│  └───────────────────────────────────────── LEN = 11
└──────────────────────────────────────────── START
```

### ACK (Arduino → ESP)
```
AA 02 02 10 [CRC] 55
│  │  │  │    │    └─ END
│  │  │  │    └────── CRC8
│  │  │  └─────────── acknowledged cmd = SEGMENT
│  │  └────────────── CMD = ACK
│  └───────────────── LEN = 2
└──────────────────── START
```
