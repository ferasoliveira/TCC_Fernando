# Wiki do Projeto - Braço Robótico EB15

Bem-vindo à wiki central de desenvolvimento do projeto do **Braço Robótico EB15**. Este repositório de conhecimento compila toda a arquitetura de hardware, software, fluxo lógico e levantamento de referências científicas que sustentam o Trabalho de Conclusão de Curso (TCC).

---

## 🗺️ Mapa de Documentação

Navegue pelos documentos estruturados da wiki para entender os pilares técnicos do projeto:

### 1. [Diagrama do Sistema (Arquitetura e Componentes)](file:///c:/Users/ferna/OneDrive/Área%20de%20Trabalho/TCC_Fernando/wiki/diagrama_sistema.md)
*   **Conteúdo:** Detalhamento do Nó Mestre (ESP32 S3), do Nó Escravo (Arduino Uno) e dos atuadores/sensores periféricos de potência e realimentação.
*   **Foco:** Divisão física e lógica de hardware para o controle de 6 graus de liberdade.

### 2. [Fluxo de Operação (Lógica de Sincronização)](file:///c:/Users/ferna/OneDrive/Área%20de%20Trabalho/TCC_Fernando/wiki/fluxo_operacao.md)
*   **Conteúdo:** O passo a passo cronológico e paralelo de execução de um comando de movimento, desde o recebimento externo até a atuação sincronizada sub-milissegundo.
*   **Foco:** Detalhamento do handshake UART e do disparo por trigger digital físico dedicados.

### 3. [Requisitos de Software e Código](file:///c:/Users/ferna/OneDrive/Área%20de%20Trabalho/TCC_Fernando/wiki/requisitos_codigo.md)
*   **Conteúdo:** Especificação formal de requisitos funcionais e não funcionais para o código-fonte das duas plataformas microcontroladas.
*   **Foco:** Regras de particionamento multi-core via FreeRTOS, controle PID de malha fechada local a 200 Hz com modulação por tangente hiperbólica ($\tanh$), leitura síncrona dos encoders magnéticos AS5600 via I2C, estrutura fixa do payload serial e limites dinâmicos de jitter.

### 4. [Plano de Ação para Implementação](file:///c:/Users/ferna/OneDrive/Área%20de%20Trabalho/TCC_Fernando/wiki/plano_acao.md)
*   **Conteúdo:** Roteiro sequencial de desenvolvimento estruturado em 7 fases, com estratégias de validação via simulação matemática local (Python), testes estáticos e integração de hardware.
*   **Foco:** Planejamento prático de código, validação lógica robusta antes do upload e mapeamento de dados experimentais para escrita das seções do TCC.

### 5. [Indexador e Catálogo de Artigos Científicos](file:///c:/Users/ferna/OneDrive/Área%20de%20Trabalho/TCC_Fernando/wiki/artigos.md)
*   **Conteúdo:** Revisão bibliográfica sistemática catalogada de artigos científicos da área de robótica manipuladora, arquitetura distribuída, malha fechada e motores de passo.
*   **Foco:** Embasamento acadêmico e justificativas de escolhas de hardware.

---

## 🛠️ Resumo da Topologia do Protótipo

*   **Juntas da Base (J1, J2, J3):** Controladas diretamente pelo **ESP32 S3** via drivers industriais **TB6600** com alimentação estável de 24 V.
*   **Juntas do Punho (J4, J5, J6):** Controladas pelo **Arduino Uno** acoplado à **CNC Shield v3.1** com drivers modulares **A4988** operando em 12 V.
*   **Malha Fechada Local:** Sensores **AS5600** de 12 bits lidos fisicamente em barramentos **I2C independentes** a **200 Hz** diretamente nas saídas das juntas móveis pós-redução (minimizando backlash).
*   **Trigger de Partida:** Conexão direta entre GPIO 4 (ESP32 S3) e Pino 2 / INT0 (Arduino Uno) para sinal de trigger síncrono por borda de descida de baixíssima latência.
