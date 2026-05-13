# Resumo do TCC

## 1. Tema

**Protótipo de Braço Robótico de Baixo Custo com Integração IoT**

O trabalho terá como foco o desenvolvimento de um protótipo funcional de braço robótico de baixo custo, com arquitetura de controle distribuída e integração IoT, capaz de executar funções básicas de manipulação e, posteriormente, evoluir para funções intermediárias e aplicações mais avançadas. O desempenho do protótipo será validado por meio de comparações com um robô industrial de referência.

---

## 2. Problemáticas

### Problemática principal
**Até que ponto é viável desenvolver um braço robótico de baixo custo com integração IoT capaz de executar funções básicas de manipulação, apresentando desempenho funcional comparável, em critérios selecionados, a um robô industrial de referência?**

### Problemática complementar
**Quais são as principais limitações e potencialidades de um braço robótico de baixo custo quanto à precisão, repetibilidade, estabilidade e capacidade de execução de movimentos, quando comparado a um robô industrial de referência?**

Essas duas problemáticas se complementam, pois uma trata da **viabilidade técnica e funcional do protótipo**, enquanto a outra direciona a análise para **desempenho, limitações e potencialidades** em comparação com um sistema industrial consolidado.

---

## 3. Objetivos

### Objetivos gerais
1. **Desenvolver e validar um protótipo de braço robótico de baixo custo com integração IoT para execução de funções básicas de manipulação.**
2. **Investigar a viabilidade técnica de um braço robótico de baixo custo com conectividade IoT para aplicações experimentais de automação.**

### Objetivos específicos
1. **Projetar a estrutura mecânica e eletrônica do braço robótico de baixo custo.**
2. **Implementar o sistema de controle embarcado e a comunicação IoT do protótipo.**
3. **Desenvolver uma interface de operação para comando e monitoramento do braço robótico.**
4. **Executar testes de movimentação básica, como posicionamento, deslocamento e repetição de trajetórias.**
5. **Comparar os resultados do protótipo com os obtidos em um robô industrial de referência, considerando critérios como tempo, repetibilidade e estabilidade de movimento.**
6. **Explorar a implementação de funções intermediárias no protótipo, avaliando seu desempenho e comparando os resultados com o robô industrial de referência.**

---

## 4. Observações

### Estrutura geral proposta
O TCC pretende unir três frentes principais:
- **fundamentação teórica** sobre braços robóticos, automação, controle, robótica industrial e integração IoT;
- **implementação prática** de um protótipo físico de baixo custo;
- **validação experimental** por comparação com um robô industrial de referência.

### Modelo alvo do protótipo
- O protótipo será baseado no **modelo EB-15**, utilizado como referência estrutural e mecânica para a construção do braço robótico.

### Modelo de referência industrial
- O robô industrial de referência para validação e comparação será o **UR-10**.
- A comparação deverá considerar principalmente:
  - execução de movimentos;
  - tempo de resposta;
  - repetibilidade;
  - estabilidade;
  - desempenho em tarefas básicas e intermediárias.

### Arquitetura de controle pretendida
A proposta envolve uma arquitetura distribuída com dois controladores principais:

- **ESP como controlador mestre**
  - responsável pela lógica principal do sistema;
  - hospedagem de interface HTML local;
  - integração IoT;
  - gerenciamento de endpoints de comunicação na rede;
  - coordenação geral do braço.

- **Arduino como controlador escravo**
  - responsável por parte do acionamento e do posicionamento final;
  - comunicação com o ESP para sincronização de comandos, estados e rotinas.

### Componentes eletrônicos e de acionamento previstos
- **6 motores NEMA 17** para a motorização do braço;
- **3 drivers TB6600** para os eixos mais exigentes, especialmente os relacionados à base;
- **3 drivers A4988**, associados a uma **CNC Shield** e ao Arduino, para os eixos de posicionamento final;
- **6 encoders magnéticos AS5600** para leitura e controle de posição dos motores;
- comunicação entre **ESP e Arduino** para coordenação dos subsistemas.

### Estrutura mecânica prevista
- uso de **caixas de redução planetárias** do próprio modelo EB-15;
- montagem do braço robótico com base em uma arquitetura de seis graus de movimentação motorizada;
- foco inicial em obter um protótipo funcional e estável antes de avançar para aplicações mais sofisticadas.

### Interface e integração IoT
- o **ESP deverá hospedar uma página HTML** com múltiplas funcionalidades de operação, parametrização e supervisão;
- a interface deverá permitir:
  - comando manual dos eixos;
  - monitoramento do estado do sistema;
  - ajuste de parâmetros operacionais;
  - visualização de dados de posição, execução e diagnóstico;
  - envio de comandos e acionamento de rotinas pela rede local.

### Principais aplicações e funções do software do robô
O software do robô deverá ser desenvolvido para servir não apenas como interface de teste, mas como uma **plataforma de operação e experimentação** do braço robótico. Entre as principais funções e aplicações previstas, destacam-se:

#### 1. Operação manual e supervisão do braço
- movimentação manual dos eixos individualmente;
- comando de posições e testes de deslocamento;
- monitoramento em tempo real dos estados do sistema;
- visualização de posição, status de execução e informações de diagnóstico.

#### 2. Controle básico de movimentação
- execução de movimentos simples programados;
- posicionamento ponto a ponto;
- repetição de trajetórias;
- testes de estabilidade, tempo de resposta e consistência de movimento.

#### 3. Rotinas automáticas e sequências de operação
- gravação e execução de sequências pré-programadas;
- disparo de rotinas automáticas de teste;
- execução repetitiva de ciclos para validação experimental;
- comparação das rotinas executadas com aquelas realizadas no robô industrial de referência.

#### 4. Aplicações iniciais de manipulação
- implementação de **pick and place com garra na extremidade**;
- movimentação, captura e reposicionamento de pequenos objetos;
- validação de tarefas básicas de automação semelhantes às executadas em células robotizadas didáticas e industriais.

#### 5. Funções intermediárias de automação
- exploração de funcionalidades além da movimentação básica, como:
  - sequências coordenadas de múltiplos eixos;
  - rotinas intermediárias de manipulação;
  - ajustes de posicionamento final;
  - comparação de desempenho dessas funções com o robô industrial de referência.

#### 6. Rastreamento visual e evolução para visão computacional
- integração futura de **webcam conectada ao sistema**;
- exploração de rotinas de rastreamento visual simples;
- uso de visão computacional como suporte ao posicionamento e à detecção de objetos;
- análise da viabilidade de aplicações visuais no protótipo em comparação com a plataforma industrial.

#### 7. Comunicação em rede e integração com outros dispositivos
- disponibilização de **endpoints de rede** para integração com outros dispositivos na mesma rede local;
- envio e recepção de comandos externos;
- possibilidade de conexão por softwares e bibliotecas de controle, com interface inspirada em fluxos de comunicação de robôs industriais;
- abertura para integração futura com sistemas supervisórios, aplicações externas e outros módulos IoT.

#### 8. Interface amigável para experimentação acadêmica
- desenvolvimento de um ambiente web de uso simples;
- foco em acessibilidade para testes, calibração e demonstração;
- uso da interface como recurso didático para operação, análise e validação do protótipo.

### Comunicação em rede
- pretende-se implementar **endpoints de rede** para permitir integração com outros dispositivos na mesma rede local;
- a intenção é possibilitar comunicação externa por meio de bibliotecas e softwares de controle, inclusive em lógica inspirada em interfaces utilizadas em robôs industriais;
- no texto do TCC, essa parte pode ser apresentada como **camada de integração em rede inspirada em arquiteturas industriais**, especialmente se a implementação não reproduzir integralmente o comportamento nativo de protocolos proprietários.

### Estratégia experimental
O desenvolvimento deverá ocorrer em camadas:
1. construção e movimentação básica do protótipo;
2. validação de posicionamento e repetição de trajetórias;
3. comparação com o UR-10 em testes equivalentes;
4. implementação de funções intermediárias;
5. aplicação de rotinas de manipulação, como pick and place;
6. exploração de integração visual com webcam;
7. nova rodada de comparação com o robô industrial.

### Possíveis funções intermediárias e avançadas
As funções intermediárias e avançadas do sistema podem incluir, por exemplo:
- **pick and place com garra na extremidade**;
- sequências programadas de manipulação;
- rastreamento visual com **webcam conectada**;
- integração de lógica de supervisão remota e automação em rede;
- expansão futura para rotinas mais inteligentes de controle e monitoramento.

### Delimitação importante
O foco do trabalho não será competir diretamente com um robô industrial em robustez ou desempenho absoluto, mas sim:
- verificar a **viabilidade técnica** de um protótipo de baixo custo;
- analisar suas **limitações e potencialidades**;
- demonstrar sua utilidade para fins acadêmicos, experimentais e de pesquisa aplicada;
- mostrar como uma arquitetura acessível, modular e conectada pode servir de base para estudos em robótica, automação e integração IoT.