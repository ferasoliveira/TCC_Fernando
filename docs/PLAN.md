# PLANO DE IMPLEMENTAÇÃO E ATUALIZAÇÃO (TCC Fernando)

## Objetivo
1. Atualizar a documentação de pinagem do ESP32-S3 e do Arduino UNO em todos os arquivos Markdown presentes nas pastas `docs/` e `Software e Firmware/docs/`.
2. Planejar e mapear 5 testes individuais (Firmwares) que isolam e validam o funcionamento global de todas as partes do sistema (motores, encoders e serial), provendo uma Web UI à prova de falhas ("para o testador burro").

## 1. Atualização da Documentação
Os seguintes arquivos possuem registros arquiteturais e de pinagem e passarão por varredura manual com buscas e substituição:
- `docs/PLAN-metodologia.md`
- `Software e Firmware/docs/arquitetura_sistema.md`
- `Software e Firmware/docs/escopo.md`
- `Software e Firmware/docs/escopo_detalhado.md`
- `Software e Firmware/docs/protocolo_serial.md`

### Novo Pinout de Referência
**ESP32 S3:**
- **TCA9548A #1 (Motores 1, 2, 3 - I2C):** GPIO 1 (SDA), GPIO 2 (SCL)
    - *Setup Interno TCA #1:* Enc1 (SDA6/SCL6), Enc2 (SDA4/SCL4), Enc3 (SDA2/SCL2)
- **Serial p/ Arduino:** GPIO 19 (RX c/ divisor de tensão), GPIO 20 (TX)
- **Motor 1:** GPIO 4 (PUL) / GPIO 5 (DIR)
- **Motor 2:** GPIO 6 (PUL) / GPIO 7 (DIR)
- **Motor 3:** GPIO 8 (PUL) / GPIO 3 (DIR)

**Arduino UNO R3 + CNC Shield v3:**
- **TCA9548A #2 (Motores 4, 5, 6 - I2C):** A2 (SDA), A3 (SCL)
    - *Setup Interno TCA #2:* Enc4 (SDA6/SCL6), Enc5 (SDA4/SCL4), Enc6 (SDA2/SCL2)
- **Serial p/ ESP32:** A4 (RX), A5 (TX)
- **Motores 4, 5, 6:** Pinos padrão da CNC Shield v3 (PUL X/Y/Z, DIR X/Y/Z, ENABLE 8).

---

## 2. Implementação dos Testes Isolados

Os testes serão todos criados na pasta `Software e Firmware/testes_isolados/`, cada qual em seu diretório específico. Todos os testes no ESP terão uma **Página Web** extremamente simples e didática embutida no firmware para controle, com interface responsiva e fácil de usar.

### Teste 2.1: `01_teste_motores_esp`
- **Foco:** Validar o envio de Pulsos (PUL) e Direção (DIR) do ESP para os drivers correspondentes.
- **Funcionalidade Web:** Escolha do motor (1, 2 ou 3), definição da velocidade e aceleração, e o gatilho para execução (ida e volta alternando a cada 5s).

### Teste 2.2: `02_teste_encoders_esp`
- **Foco:** Validar via TCA9548A #1 os encoders no barramento I2C do ESP32.
- **Funcionalidade Web:** Definir quantos graus devem ser andados no motor (ex: M1), acionar o pulso na direção correspondente, ler os valores atuais dos encoders (graus) logo após o fim da movimentação para comparar com o programado.

### Teste 2.3: `03_teste_serial_ping`
- **Foco:** Validar apenas a integridade da comunicação via pinos RX/TX.
- **Ação:** Um arquivo pro ESP (`03_esp.ino`) e outro pro Arduino (`03_arduino.ino`). Enviar pacotes e receber ACKs. Log serial em ambos mostrando os pacotes trocados com sucesso.

### Teste 2.4: `04_teste_controle_arduino_via_esp`
- **Foco:** Controlar movimento do Arduino vindo do ESP.
- **Funcionalidade Web:** A página existirá somente no ESP, mas o usuário escolhe se os comandos (movimento de vai/volta de motor) aplicarão no ESP local ou se serão repassados via pacote serial para os motores remotos do Arduino (M4, M5, M6). O arduino rodará o parser que aciona a CNC Shield.

### Teste 2.5: `05_teste_valida_encoders_arduino_via_esp`
- **Foco:** Comprovar a integração ponta-a-ponta (ESP manda movimento para Arduino -> Arduino executa CNC e checa TCA9548A #2 -> Arduino devolve o Status pro ESP).
- **Funcionalidade Web:** ESP pede para mover o atuador X no Arduino, e o Arduino retorna dizendo "Andei Y graus no meu próprio encoder". Será mostrada na tela os graus atualizados do sistema acoplado no Arduino.

---

## Execução da Fase 2 (Pós-Aprovação)
Uma vez aprovado, irei invocar em paralelo:
- **`documentation-writer`** para varrer e substituir as menções de pinouts.
- **`frontend-specialist`** para desenhar as páginas html do esp (emulando um React-like em vanilla ou bootstrap dentro da string C++)
- **`backend-specialist` (embedded)** para a programação dos firmwares e da comunicação C++ no ESP e Arduino.
