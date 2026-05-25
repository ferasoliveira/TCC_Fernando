# Plano de Implementação — EB-15

## Fase 1: Fundação (Esta entrega)
- [x] Escopo e requisitos
- [x] Arquitetura do sistema
- [x] Protocolo serial
- [x] API REST
- [x] Decisões arquiteturais
- [x] Firmware base ESP32-S3
- [x] Firmware base Arduino Uno
- [x] Interface web inicial
- [x] Plano de testes

## Fase 2: Validação de Hardware
- [ ] Flash e teste do Arduino Uno isolado (loopback serial)
- [ ] Flash e teste do ESP32-S3 isolado (Wi-Fi + web)
- [ ] Teste de comunicação serial entre placas
- [ ] Teste de geração de pulsos (osciloscopio)
- [ ] Teste de heartbeat bidirecional
- [ ] Calibrar steps_per_degree por junta
- [ ] Definir e conectar chaves de fim de curso
- [ ] Implementar homing real

## Fase 3: Movimento Real
- [ ] Ajustar perfil trapezoidal com hardware real
- [ ] Testar MoveJ com 1 eixo
- [ ] Testar MoveJ com 6 eixos sincronizados
- [ ] Validar FK com medições reais
- [ ] Implementar IK completa (atualmente stub)
- [ ] Testar MoveL com FK/IK reais
- [ ] Ajustar parâmetros DH com medições do EB-15

## Fase 4: Robustez
- [ ] Testes de recuperação de falha serial
- [ ] Testes de limites de juntas
- [ ] Testes de watchdog
- [ ] Implementar persistência real (NVS + SPIFFS)
- [ ] Testes de receitas multi-step
- [ ] Testes de pause/resume durante movimento
- [ ] Implementar checagem de obstáculos real

## Fase 5: Refinamento
- [ ] MoveC (movimento circular)
- [ ] UI avançada
- [ ] Calibração assistida
- [ ] Otimização de performance
- [ ] Documentação de operação

---

## Dependências Críticas

| Dependência | Impacto | Status |
|---|---|---|
| Medidas reais do EB-15 (DH parameters) | FK/IK não produzem valores reais | **Pendente** |
| Chaves de fim de curso | Homing não funcional | **Pendente** |
| Level shifter 3.3V↔5V | Comunicação serial | **Pendente** |
| Steps/degree por junta | Movimentos imprecisos | **Pendente** |
