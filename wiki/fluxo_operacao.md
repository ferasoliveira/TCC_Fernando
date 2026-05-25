# Fluxo de Operação - Lógica de Sincronização

Este documento detalha o fluxo sequencial e paralelo das operações do braço robótico, conforme esquematizado no arquivo `Fluxo de Operacao TCC.drawio`. 

Para resolver o problema de gargalo de processamento, o sistema utiliza uma abordagem em que o planejamento e a execução são separados, garantindo assim que a atuação mecânica seja suave e perfeitamente sincronizada em todas as 6 juntas do braço.

## Etapas do Processo de Movimentação

**1. Recebimento de Comando**
Toda a operação se inicia no ESP32 S3 (Mestre), que atua como porta de entrada. Ele aguarda requisições de deslocamento originadas da interface Web ou de sistemas de automação que utilizem a porta RTDE.

**2. Planejamento de Trajetória Global**
Uma vez recebido o comando de destino, o ESP32 realiza os cálculos matemáticos complexos (Cinemática Inversa e Interpolação de Trajetória) para definir as velocidades, rampas de aceleração e a quantidade exata de passos de **todas as 6 juntas**.

**3. Distribuição de Parâmetros (Setup Paralelo)**
Como o Arduino (Slave) vai cuidar das juntas 4, 5 e 6, o ESP32 desmembra as informações e envia apenas os parâmetros dessas 3 juntas superiores para o Arduino, utilizando a comunicação **Serial UART**. 

**4. Handshake (Confirmação)**
Ao receber os dados, o Arduino pré-configura seus contadores e temporizadores para o movimento, e então envia uma confirmação via UART de volta para o ESP32 S3 informando que está "**Pronto**". O ESP32 fica bloqueado aguardando esse aval.

**5. Sinal de Início (Trigger de Sincronismo)**
Com as duas placas prontas, o ESP32 dispara fisicamente um sinal limpo (Trigger Digital). Isso garante que não haja defasagem de latência serial no início do movimento.

**6. Execução e Monitoramento (Ação Simultânea)**
No exato microssegundo em que o sinal de trigger ocorre, as duas placas disparam seus laços de geração de passos:
*   **Ação no Mestre:** O ESP32 move as juntas J1 a J3.
*   **Ação no Slave:** O Arduino move as juntas J4 a J6.
Ambos realizam em paralelo a leitura de segurança através dos **Encoders Magnéticos AS5600** para atestar a precisão em tempo real.

**7. Reporte de Finalização (Feedback Consolidado)**
Após finalizar a rotina dos passos designados, o Arduino reporta pela via Serial o status da sua execução (indicando se houve sucesso ou algum erro de perda de passos nas juntas do topo). O ESP32 consolida essa informação junto aos resultados do seu próprio movimento, respondendo finalmente à requisição original externa com o feedback consolidado de toda a tarefa.