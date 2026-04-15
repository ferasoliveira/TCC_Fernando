# Planejamento: Estruturação da Metodologia do TCC

## 1. Visão Geral
**Objetivo:** Estruturar e organizar a redação do Capítulo 3 (Metodologia) do TCC, detalhando os procedimentos técnicos, a implementação do protótipo EB-15 e os protocolos rigorosos de validação frente ao robô UR-10.
**Contexto:** Baseado nos Capítulos 1 (Introdução) e 2 (Fundamentação Teórica), a metodologia deve conectar a teoria previamente revisada com o cenário altamente documentado das técnicas experimentais, decisões de eletrônica embarcada e comparações analíticas em robótica estabelecidas ao longo do projeto.

---

## 2. Estrutura Detalhada para o Capítulo 3 (Metodologia)

### 3.1. Tipo e Caracterização da Pesquisa
- Definição do método de pesquisa quantitativa/experimental.
- Demonstração sobre como as características da prototipagem em impressora 3D avançada atendem aos limites estipulados no projeto focado em baixo custo versus viabilidade de execução.

### 3.2. Plataforma e Materiais Utilizados
- **3.2.1. Fabricação e Estrutura do Protótipo EB-15:** Definição mecânica das peças em 3D. Emprego da impressora Bambu Lab A1 com bico de precisão 0.16mm. Características adotadas que induzem robustez mecânica sistêmica: filamento PETG (1,4 kg empregando 128h de produção intermitente), adoção de espessura constante de 6 camadas (perímetros) em face ao preenchimento otimizado (uso do padrão *Gyroid* entre 50-60% para engrenagens e eixos focando em flexo-absorção de impacto, além de padrão Linear nas superfícies convencionais).
- **3.2.2. Eixo de Atuadores e Eletrônica de Controle Mestre-Escravo:** Descrição do esquema de tração (Drivers TB6600 atrelados aos NEMA 17) orquestrado por um processamento de dupla via: Microcontrolador ESP32-S3 como nó central decisor (Supervisor) atrelado via sub-barramento serial a uma plataforma dedicada em tempo rígido de interpolação em Arduino Uno (Escravo).
- **3.2.3. O Robô Industrial de Referência (UR-10):** Limites e justificativa do uso em laboratório com disponibilidade física e instrumental focado na execução de tarefas comparáveis na validação qualitativa.

### 3.3. Procedimentos de Implementação Tecnológica
- **3.3.1. Mitigação Mecânica e Restauro de Posição (Homing persistente):** Solucionando ausência de hardware analítico via lógica. Como, pela não adoção de chaves de fim de curso ou *encoders* mecânicos nativos da estrutura EB-15, implementou-se em firmware o salvamento das origens e ângulos na memória Flash não-volátil persistente (NVS) garantindo retorno sistêmico livre de falhas (Soft Limits dinâmicos via Dashboard atuando como "zero elétrico").
- **3.3.2. Protocolo de Enlace Mestre-Escravo (Serial UART):** Detalhamento das regras explícitas programadas. O processo transiente baseado em envios segmentários de pulso acompanhado de redundância via retorno de confirmações (Status de Recebimento do Envelope "OK" e sinal lógico de Encerramento do Evento Mecânico inter-eixos). 
- **3.3.3. IHM e Rede de Interconectividade Otimizada:** Explanação sobre a Interface Web desenvolvida como um produto supervisor encapsulando as bibliotecas HTML/JS embarcadas. Decisão pelo roteamento isolacionista: o ESP agindo como *Access Point* irradiador isolado (concedendo estabilidade remota via redes *ad hoc* focadas e de fácil manutenção), com abertura intrínseca a infraestruturas preexistentes.

### 3.4. Protocolos Experimentais e Configuração de Testes
- **3.4.1. Fluxograma de Experimentações Lógicas e Mecânicas:** Convalidação sistêmica baseada na regra contínua do TCC:
  - Ensaios de Validação de Peça: Calibragem de juntas isoladas conectadas com os drives e encoder da rotina, subida linear pela cinemática engrenada (base/3 juntas).
  - Ensaios de Programação Avançada: Avaliação dos parâmetros transientes puramente computacionais e de fluxo elétrico de pontes seriais, teste da malha servida pela interface AP nativa. 
- **3.4.2. Qualificação Funcional Limitada e Resposta Sensorial Prática:** Por ausência impeditiva mercadológica, superou-se as verificações microscópicas focando nos critérios experimentais de campo abertos (Avaliação mecânica de angulação com auxílio metrológico primário em transferidores rígidos ou alcance visual). Movimentação planar de espaços arbitrários conferindo latência ("atingiu o subponto no momento Y ou não atingiu").
- **3.4.3. Aplicações Práticas Complexas e Modelagem Lógica:** Consolidação do TCC na via pragmática por intermédio de demonstração real: Integração de visão computacional guiando as manipulações (câmera) e testes dinâmicos de exatidão de repetição no plano temporal prolongado (*Pick-and-place* exaustivo de bancada).
- **3.4.4. O Comparativo Definitivo - Python Scripts & Universal Robots UR-10:** Roteirização das métricas consolidadas pelo UR-10 replicando as diretrizes laboratoriais, orquestradas ativamente sob a óptica das bibliotecas programadas em ambiente da linguagem *Python*, visando padronização e a produção final rigorosa de relatórios, gráficos modulares e avaliação conclusiva final da eficácia do aparelho testado contra limites de uma infraestrutura que baliza toda a indústria moderna.

---

## 3. Breakdown de Tarefas para a Escrita (Execução Futura)

Abaixo estão as próximas premissas que os Agentes seguirão quando a escrita do Capítulo começar:

| Tarefa | Agente Recomendado | Skill Principal | Descrição |
|--------|---------------------|-----------------|-----------|
| **T1** | `academic-writer` | `academic-writing` | Escrever Seções 3.1 e 3.2 (Pesquisa, Setup Físico do hardware e Impressão Estrutural 3D do PEGT do braço). |
| **T2** | `academic-writer` | `embedded-firmware` | Escrever Seção 3.3 (Zero Virtual em NVS, Lógica Serial UART Escravo-Mestre e AP Nativo/Web Interface). |
| **T3** | `academic-writer` | `experiment-design` | Escrever Seção 3.4 (Roteiro e bateria seqüêncial sistêmica qualitativa, visao p/câmera, *pick-and-place* prático interativo versus Dados Python sobre UR-10 real). |
| **T4** | `academic-reviewer` | `abnt-formatting` | Revisão gramatical ABNT e checagem da estrutura da Regra do Ouro do parágrafo da pesquisa (4-8 linhas unificadas intercedendo conexões conjunturais). |

---

## ✅ Fase X: Verificação Final da Redação

Antes de compilar e dar por encerada a versão do capítulo do usuário, a plataforma agirá para atestar:
- [ ] A estrutura segue o modelo mandatório de 4-8 linhas por parágrafo (`academic-writing`).
- [ ] Conectores de coesão obrigatórios estão logicamente presentes nos trechos conjunturais dos parágrafos textuais (`academic-writer/abnTEX`).
- [ ] O texto está perfeitamente formatado no repositório final e entregável `.tex` com conformidade à classe de documentos `abntex2`.
- [ ] O capítulo engloba perfeitamente a calibração pragmática de custo com o arranjo engrenagens/filamento Bambu Lab, a falta estipulada e contornável de endstops superada e atrelada inteiramente à Memória NVS conectada com a IHM remota.
- [ ] Explanações acerca de rotinas contínuas do robô final e da replicação comparada real orientada em análises Python do UR-10.
