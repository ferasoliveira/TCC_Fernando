---
name: conhecimentos-robo
description: Base de conhecimento centralizada sobre o projeto e hardware do braço robótico de 6 graus de liberdade EB15.
---

# Skill: Conhecimentos Técnicos do EB15

Base de conhecimento centralizada sobre o projeto do braço robótico de 6 graus de liberdade.

## Especificações Técnicas:
- **Arquitetura Distribuída:** - **Mestre (ESP32 S3):** Controle de J1 a J3, planejamento de trajetória global, interface Web e RTDE.
  - **Escravo (Arduino Uno):** Execução de movimento de J4 a J6 e monitoramento local.
- **Hardware:** Utilização de drivers TB6600 para a base, CNC Shield para o topo e sensores magnéticos AS5600 para feedback em malha fechada.
- **Sincronização:** Lógica baseada em Handshake via Serial UART e disparo simultâneo por Trigger Digital.
- **Contexto:** Validação de baixo custo e alta eficiência de processamento.