# Plano de Testes — EB-15

## 1. Estratégia Geral

Testes em sistemas embarcados sem simulador são predominantemente manuais e incrementais. A estratégia é:

1. **Compilação** — Verificar que ambos firmwares compilam sem erros
2. **Teste unitário de mesa** — Testar módulos isolados via Serial Monitor
3. **Teste de integração** — Testar comunicação entre placas
4. **Teste de sistema** — Testar fluxos completos via web UI / API
5. **Teste de stress** — Operação contínua, recuperação de falhas

---

## 2. Testes de Compilação

| ID | Teste | Critério |
|---|---|---|
| TC-01 | Compilar ESP32 firmware (board: ESP32S3 Dev Module) | 0 erros, 0 warnings críticos |
| TC-02 | Compilar Arduino Uno firmware (board: Arduino Uno) | 0 erros, 0 warnings críticos |
| TC-03 | Abrir web/index.html no Chrome | Sem erros no console JS |

---

## 3. Testes de Protocolo Serial

| ID | Teste | Procedimento | Esperado |
|---|---|---|---|
| TS-01 | Heartbeat ESP→Arduino | Conectar UART, monitor ambos | Arduino responde heartbeat |
| TS-02 | CRC inválido | Enviar frame com CRC errado | NACK com ERROR_CRC |
| TS-03 | Comando desconhecido | Enviar CMD 0xFF | NACK com ERROR_UNKNOWN_CMD |
| TS-04 | Envio de segmento | Enviar SEGMENT válido | ACK + execução |
| TS-05 | Fila cheia | Enviar 16+ segmentos rápido | NACK com ERROR_QUEUE_FULL |
| TS-06 | Timeout heartbeat | Desconectar Arduino | ESP marca FAULT_SERIAL_TIMEOUT |
| TS-07 | Watchdog Arduino | Parar heartbeats do ESP | Arduino auto-stop após 2s |

---

## 4. Testes de API REST

| ID | Endpoint | Método | Teste |
|---|---|---|---|
| TA-01 | /api/status | GET | Retorna JSON com estado válido |
| TA-02 | /api/joints | GET | Retorna 6 juntas com valores |
| TA-03 | /api/pose | GET | Retorna pose XYZ + rotações |
| TA-04 | /api/home | POST | Inicia homing, estado → HOMING |
| TA-05 | /api/stop | POST | Para movimento, estado → READY |
| TA-06 | /api/move/joints | POST | Aceita target válido, rejeita inválido |
| TA-07 | /api/move/joints | POST | Rejeita se não homed (409) |
| TA-08 | /api/fk | POST | Calcula FK para juntas dadas |
| TA-09 | /api/ik | POST | Calcula IK para pose dada |
| TA-10 | /api/obstacles | POST/GET | CRUD de obstáculos |
| TA-11 | /api/jog | POST | Move junta incremental |
| TA-12 | /api/config | GET/POST | Lê e atualiza configuração |

---

## 5. Testes de Movimento

| ID | Teste | Procedimento | Esperado |
|---|---|---|---|
| TM-01 | Jog J1 | Via web UI, botão +/- | Motor J1 gira |
| TM-02 | MoveJ 1 eixo | API: mover J1 para 30° | Movimento suave, para em 30° |
| TM-03 | MoveJ 6 eixos | API: mover todos | Todos chegam sincronizados |
| TM-04 | Soft limits | API: mover J1 para 200° | Rejeição (limite 170°) |
| TM-05 | Pause durante MoveJ | API: pause + resume | Pausa e retoma corretamente |
| TM-06 | Stop durante MoveJ | API: stop | Parada imediata |
| TM-07 | MoveL simples | API: mover para XYZ | Trajetória reta no cartesiano |

---

## 6. Testes de Robustez

| ID | Teste | Procedimento | Esperado |
|---|---|---|---|
| TR-01 | Reconexão serial | Desconectar/reconectar cabo | Recupera comunicação |
| TR-02 | Wi-Fi reconexão | Desligar/ligar roteador | ESP reconecta automaticamente |
| TR-03 | Reset Arduino | Reset durante movimento | ESP detecta fault, para |
| TR-04 | API spam | 100 requests /api/status | Todas respondidas sem crash |
| TR-05 | Receita longa | 50 steps em receita | Executa até o fim |

---

## 7. Testes da Interface Web

| ID | Teste | Procedimento | Esperado |
|---|---|---|---|
| TW-01 | Dashboard carrega | Abrir IP no browser | Todos os painéis visíveis |
| TW-02 | Status atualiza | Aguardar polling | Valores atualizam a cada 1s |
| TW-03 | Jog funciona | Clicar botão jog | Motor responde |
| TW-04 | Home funciona | Clicar botão Home | Inicia homing |
| TW-05 | Stop funciona | Clicar botão Stop | Para imediatamente |
| TW-06 | Logs exibidos | Gerar eventos | Aparecem no log viewer |

---

## 8. Ferramentas Necessárias

| Ferramenta | Uso |
|---|---|
| Arduino IDE 2.x | Compilação e flash |
| Serial Monitor | Debug de protocolo |
| curl / Postman | Testes de API |
| Chrome DevTools | Debug de web UI |
| Osciloscópio | Validar pulsos dos motores |
| Multímetro | Verificar levels de tensão |
