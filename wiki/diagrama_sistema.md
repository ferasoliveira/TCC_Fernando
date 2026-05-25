# Diagrama do Sistema - Arquitetura e Componentes

Este documento descreve a arquitetura de hardware e software do braço robótico de baixo custo, baseada no arquivo `Diagrama sistema TCC.drawio`. 

A arquitetura foi projetada de forma distribuída para garantir que funções pesadas (como interface Wi-Fi) não interfiram na geração de pulsos em tempo real dos motores. O sistema é dividido em três blocos principais:

## 1. Controle Mestre: ESP32 S3 (Supervisor)
O ESP32 atua como o cérebro principal e gerenciador de comunicações do robô.
*   **Conectividade (Servidor Web / STA-AP):** Hospeda internamente a Página Web Interativa e processa as requisições (endpoints RTDE) recebidas via Wi-Fi.
*   **Planejamento Computacional:** Executa o cálculo da cinemática e o **Planejamento de Trajetória** completo para todos os motores.
*   **Atuação Direta:** Gera os pulsos físicos de controle diretamente para os drivers das juntas da base do robô (J1, J2 e J3), que exigem maior torque.

## 2. Controle Secundário: Arduino UNO (Slave / Executor)
O Arduino atua como um co-processador focado exclusivamente em aliviar a carga do ESP32 na geração de passos.
*   **Comunicação:** Recebe os comandos e parâmetros de trajetória pré-calculados do ESP32 através de barramento **Serial (UART)**.
*   **Atuação Direta:** Fica responsável pela geração de pulsos em tempo real para o shield das juntas superiores (J4, J5 e J6).

## 3. Atuadores e Sensores (Hardware Físico)
Este bloco engloba os componentes de potência e a malha de instrumentação do braço:
*   **Eletrônica de Potência:** 
    *   **Drivers TB6600:** Controladores industriais mais robustos, usados pelo ESP32 para movimentar os grandes motores da base.
    *   **CNC Shield v3.1:** Acoplada ao Arduino, gerencia drivers menores (geralmente A4988 ou DRV8825) para os motores menores da "mão" do robô.
*   **Malha Fechada (Feedback):** Sensores **Encoders Magnéticos AS5600** estão instalados no eixo de cada uma das juntas. A leitura contínua da angulação física real é transmitida através de barramento **I2C**, retornando informações precisas para o ESP32 e para o Arduino corrigirem ou validarem a posição final.