# TCC - Brainstorming

## Tema: 
Arquitetura de controle distribuída para braços robóticos de baixo custo

## Problemática:

O desenvolvimento de braços robóticos de baixo custo é limitado pelo baixo poder de processamento dos microcontroladores acessíveis. Ao adicionar funções de conectividade, como interfaces web e protocolos de comunicação, o sistema fica sobrecarregado e não consegue manter a precisão das tarefas em tempo real. Isso causa atrasos na geração de pulsos para os motores e falhas na leitura dos sensores de ângulo.

## Objetivo Geral:

Desenvolver um firmware com arquitetura distribuída focado no processamento de requisições externas, planejamento de trajetória e sincronização de juntas.

### Objetivos Específicos:

Programar a distribuição das tarefas de comando e controle das juntas entre diferentes unidades de processamento para otimizar a execução física dos movimentos.

Validar a funcionalidade da arquitetura proposta por meio de testes práticos de movimentação em um protótipo de braço robótico de baixo custo.

## Informações Úteis

### 1. Arquitetura de Hardware
*   **Processador Central (Master):** ESP32 S3.
    *   **Responsabilidades:** Hospedagem da página web, processamento de requisições externas (RTDE), planejamento de trajetória global, controle das 3 juntas da base e comunicação serial.
    *   **Drivers:** 3x TB6600.
*   **Processador Auxiliar (Slave):** Arduino Uno.
    *   **Responsabilidades:** Execução do movimento das 3 juntas superiores e monitoramento de sensores locais.
    *   **Drivers:** CNC Shield v3.1.
*   **Feedback:** Sensores magnéticos AS5600 em todas as juntas para leitura de angulação bruta (suporte a malha fechada).
*   **Comunicação Inter-processador:** Conexão Serial (UART) entre ESP32 S3 e Arduino Uno.

### 2. Requisitos de Software e Conectividade
*   **Modos de Operação Wi-Fi:** O sistema deve suportar tanto o modo **STA** (Station) para conexão em redes existentes quanto o modo **AP** (Access Point) para conexão direta.
*   **Interface de Usuário:** Página web interativa armazenada e servida internamente pelo ESP32 S3, permitindo funções de *Jog*, inserção de pontos e configuração.
*   **Protocolo de Comunicação:** Implementação de endpoints seguindo a lógica do protocolo RTDE para integração com dispositivos externos.
*   **Parametrização:** O software deve permitir a configuração personalizada das **relações de redução** das caixas de transmissão para cada junta de forma independente.

### 3. Lógica de Sincronização e Fluxo de Dados
1.  O **ESP32 S3** recebe o comando (via Web ou RTDE) e realiza o planejamento de trajetória para as juntas.
2.  O **ESP32 S3** envia os parâmetros das 3 juntas superiores para o **Arduino Uno** via Serial.
3.  Após a confirmação do Arduino, o **ESP32 S3** envia um sinal de início (trigger) para sincronismo.
4.  Ambas as unidades executam o movimento simultaneamente, monitorando os sensores **AS5600**.
5.  O **Arduino** reporta o status final (Sucesso/Erro) e o **ESP32** finaliza a requisição externa com o feedback consolidado.

### 4. Protótipo de Validação
*   **Modelo:** Braço robótico EB15 (6 graus de liberdade).
*   **Contexto:** Validação de baixo custo e alta eficiência de processamento via arquitetura distribuída.