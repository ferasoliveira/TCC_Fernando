---
name: programador
description: Desenvolvimento e programação de firmware distribuído entre o ESP32 S3 (Mestre) e o Arduino Uno (Escravo) do braço EB15.
---

# Workflow: Programador (programador)

Este workflow é dedicado ao desenvolvimento do firmware distribuído entre o ESP32 S3 (Mestre) e o Arduino Uno (Escravo).

## Passo a Passo:

1. **Contextualização Técnica:**
   - Ler `wiki/index.md` e o progresso da escrita em `TCC_escrito/`.
   - Mapear os requisitos de hardware (Drivers TB6600, CNC Shield, Sensores AS5600).

2. **Socratic Gate:**
   - Coletar informações pendentes sobre a lógica de controle, limites de junta ou endereçamento I2C necessário.

3. **Análise de Implementação:**
   - Verificar se a programação segue os diagramas de sistema e fluxos de operação definidos na wiki.
   - Garantir que a lógica de sincronização (Trigger Digital) seja respeitada.

4. **Desenvolvimento de Código:**
   - Utilizar skills de programação para gerar o firmware do robô (ESP32 S3 e Arduino Uno).
   - Implementar endpoints RTDE e interface web conforme especificado.

5. **Testes e Autovalidação de Código:**
   - Realizar testes próprios e autovalidação dos códigos gerados.
   - Caso necessário, retornar para a etapa 4 atualizando e corrigindo o código.

6. **Atualização de Lições Aprendidas:**
   - Atualizar o documento `wiki/lessons-learned.md` com as lições aprendidas ao longo do desenvolvimento.
   - Identificar problemas esbarrados, dificuldades encontradas ou desperdício de tokens.

7. **Feedback de Entrega:**
   - Retornar um resumo super objetivo do que foi feito, no seguinte formato:
     ```text
     Foi feito:
     1- X
     2- X
     3 - Teste do 2 validado da forma X
     ...
     ```