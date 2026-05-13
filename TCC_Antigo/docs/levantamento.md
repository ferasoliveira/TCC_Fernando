# Levantamento para estruturação do TCC no Antigravity + AG-kit

---

## 1. Objetivo do levantamento

Este documento registra o diagnóstico completo da infraestrutura do workspace e a proposta de utilização do ecossistema Antigravity + AG-kit para suportar **todas as frentes de trabalho do TCC**:

- Desenvolvimento técnico do protótipo (firmware, software, mecânica, eletrônica).
- Pesquisa, escrita, revisão e evolução do trabalho acadêmico.
- Testes experimentais e comparação com o robô industrial de referência (UR-10).
- Documentação, rastreabilidade e geração de relatórios.

O propósito é produzir um mapeamento técnico que sirva como **base real de planejamento** para a próxima etapa de implementação, separando claramente o que já existe, o que falta e o que é recomendado criar.

---

## 2. Resumo interpretado do TCC

### 2.1 Tema

**Protótipo de Braço Robótico de Baixo Custo com Integração IoT** — desenvolvimento, validação experimental e comparação com um robô industrial.

### 2.2 Problemáticas

| # | Problemática | Foco |
|---|---|---|
| Principal | Viabilidade de um braço de baixo custo com IoT executando manipulação básica, com desempenho comparável ao UR-10 em critérios selecionados | Viabilidade técnica e funcional |
| Complementar | Limitações e potencialidades do protótipo em precisão, repetibilidade, estabilidade e capacidade de movimento | Desempenho comparativo |

### 2.3 Objetivos

**Gerais:**
1. Desenvolver e validar o protótipo de braço robótico com IoT.
2. Investigar a viabilidade técnica para automação experimental.

**Específicos:**
1. Projetar estrutura mecânica e eletrônica.
2. Implementar controle embarcado e comunicação IoT.
3. Desenvolver interface de operação (web).
4. Executar testes de movimentação básica.
5. Comparar resultados com UR-10.
6. Explorar funções intermediárias e comparar.

### 2.4 Escopo técnico do protótipo

| Aspecto | Detalhe |
|---|---|
| Modelo estrutural | EB-15 (6-DOF, caixas de redução planetárias) |
| Fabricação Mecânica | Impressão 3D (Bambu Lab A1, PETG 1.4kg, bico 0.16mm, infill gyroid/linear 50-60%) |
| Referência industrial | UR-10 |
| Controladores | ESP32-S3 (supervisor, J1–J3) + Arduino Uno (executor, J4–J6) |
| Motores | 6× NEMA 17 |
| Drivers | 6× TB6600 (um por eixo) |
| Sensores / Homing | Sem fim de curso ou encoders (Controle open-loop, zero mapeado manualmente e persistido via NVS) |
| Comunicação | UART serial binário (feedback de recebimento OK e sucesso na finalização do movimento entre ESP e Arduino) |
| Rede | Wi-Fi Access Point (nativo do ESP32 por facilidade) com API local embarcada |
| Interface | HTML/CSS/JS vanilla, servida pelo ESP32-S3 (PROGMEM) subproduto atuando como painel supervisor |

### 2.5 Escopo do software

O software já possui documentação técnica considerável e firmware funcional:

| Artefato | Status |
|---|---|
| `escopo.md` | ✅ Completo — contrato principal do sistema |
| `arquitetura_sistema.md` | ✅ Completo — módulos, máquina de estados, fluxos |
| `protocolo_serial.md` | ✅ Completo — framing, CRC, comandos |
| `api_local.md` | ✅ Completo — endpoints REST |
| `decisoes_arquiteturais.md` | ✅ Completo — 10 ADRs documentados |
| `escopo_detalhado.md` | ✅ Completo — detalhamento extenso |
| `plano_implementacao.md` | ✅ Fase 1 completa, fases 2–5 pendentes |
| `plano_testes.md` | ✅ Completo — 35+ casos de teste definidos |
| `eb15_esp32_supervisor.ino` | ✅ Firmware base (~32 KB) |
| `eb15_uno_executor.ino` | ✅ Firmware base (~19 KB) |

### 2.6 Estratégia experimental (camadas)

1. Testes progressivos de montagem: junta isolada, base (3 juntas mecânicas), e movimentos complexos globais.
2. Testes lógicos independentes: comunicação serial, malha API web e manipulação de scripts externos.
3. Validação paramétrica simplificada por inspeção (transferidor e aproximação linear/angular) avaliando tempo de repouso e alcance target.
4. Funções contínuas de laboratório: *pick & place* e exploração de *endpoints* de visão computacional (webcam no TCP).
5. Comparação espelhada empírica com o robô UR-10 (acesso físico laboratorial), utilizando scripts em Python para *data analytics* e geração gráfica.

### 2.7 Necessidades de execução identificadas

A partir do resumo e da infraestrutura existente, os eixos de trabalho do TCC podem ser sintetizados em:

| Eixo | Natureza |
|---|---|
| Pesquisa e fundamentação teórica | Acadêmico |
| Escrita e estruturação do TCC | Acadêmico |
| Desenvolvimento de firmware (ESP32 + Arduino) | Técnico/embarcado |
| Desenvolvimento da interface web | Técnico/frontend |
| Integração mecânica/eletrônica/embarcada | Técnico/hardware |
| Testes experimentais e validação | Experimental |
| Comparação com UR-10 | Experimental/acadêmico |
| Documentação técnica do protótipo | Técnico/documental |
| Revisão e padronização acadêmica | Acadêmico |
| Registro e análise de resultados | Experimental/acadêmico |

---

## 3. Diagnóstico da infraestrutura atual

### 3.1 Estrutura do workspace

```
TCC Fernando Artur/
├── .agent/                          # Infraestrutura AG-kit
│   ├── ARCHITECTURE.md              # Mapa geral do AG-kit
│   ├── agents/                      # 20 agentes especialistas
│   ├── skills/                      # 37 pastas de skills
│   ├── workflows/                   # 11 workflows (slash commands)
│   ├── scripts/                     # 4 scripts master
│   ├── rules/                       # 1 arquivo (GEMINI.md)
│   └── .shared/                     # Assets compartilhados (ui-ux-pro-max)
├── Software e Firmware/
│   ├── Firmware Final/
│   │   ├── esp32/                   # eb15_esp32_supervisor.ino (32 KB)
│   │   └── arduino_uno/             # eb15_uno_executor.ino (19 KB)
│   └── docs/                        # 8 documentos técnicos
└── docs/
    └── Resumo_tcc.md                # Resumo do TCC
```

### 3.2 Agentes existentes (20)

| Agente | Foco | Relevância para o TCC |
|---|---|---|
| `orchestrator` | Coordenação multi-agente | 🟢 Alta — coordenar frentes paralelas |
| `project-planner` | Planejamento e decomposição de tarefas | 🟢 Alta — planejar fases do TCC |
| `frontend-specialist` | Web UI/UX | 🟢 Alta — interface web do robô |
| `backend-specialist` | API, lógica de servidor | 🟡 Média — REST API do ESP é embarcada, não backend convencional |
| `database-architect` | Schema, SQL | 🔴 Baixa — projeto não usa banco de dados |
| `mobile-developer` | Apps iOS/Android | 🔴 Nula — não há componente mobile |
| `game-developer` | Jogos | 🔴 Nula |
| `devops-engineer` | CI/CD, Docker | 🔴 Baixa — firmware não usa pipelines tradicionais |
| `security-auditor` | Segurança | 🟡 Média — segurança da API local e rede |
| `penetration-tester` | Testes ofensivos | 🔴 Baixa |
| `test-engineer` | Estratégias de teste | 🟢 Alta — testes de firmware, API e integração |
| `debugger` | Análise de causa raiz | 🟢 Alta — debugging de firmware embarcado |
| `performance-optimizer` | Web Vitals | 🟡 Baixa — UI do ESP é mínima |
| `seo-specialist` | SEO | 🔴 Nula |
| `documentation-writer` | Documentação técnica | 🟢 Alta — documentação do protótipo |
| `product-manager` | Requisitos, user stories | 🟡 Média — pode auxiliar na gestão de requisitos |
| `product-owner` | Estratégia, backlog | 🟡 Média |
| `qa-automation-engineer` | E2E, CI | 🟡 Baixa |
| `code-archaeologist` | Refatoração | 🟡 Média — refatoração dos firmwares |
| `explorer-agent` | Análise de codebase | 🟢 Alta — navegação pelo workspace |

### 3.3 Skills existentes (37)

**Skills com relevância direta para o TCC:**

| Skill | Relevância | Motivo |
|---|---|---|
| `clean-code` | 🟢 Alta | Padrões globais de código |
| `architecture` | 🟢 Alta | Decisões arquiteturais do sistema |
| `plan-writing` | 🟢 Alta | Planejamento de tarefas |
| `brainstorming` | 🟢 Alta | Socratic questioning para requisitos |
| `systematic-debugging` | 🟢 Alta | Debug de firmware |
| `documentation-templates` | 🟢 Alta | Templates de documentação |
| `frontend-design` | 🟡 Média | UI do dashboard web |
| `testing-patterns` | 🟡 Média | Padrões de teste (adaptação necessária para embarcado) |
| `api-patterns` | 🟡 Média | Design da REST API |
| `powershell-windows` | 🟡 Média | Automação local (Windows) |
| `behavioral-modes` | 🟡 Média | Modos operacionais do agente |
| `code-review-checklist` | 🟡 Média | Revisão de código |
| `performance-profiling` | 🟡 Baixa | Profiling da web UI |

**Skills sem relevância direta:** `react-best-practices`, `tailwind-patterns`, `web-design-guidelines`, `nestjs-expert`, `nodejs-best-practices`, `python-patterns`, `database-design`, `prisma-expert`, `typescript-expert`, `docker-expert`, `deployment-procedures`, `server-management`, `webapp-testing`, `tdd-workflow`, `lint-and-validate`, `vulnerability-scanner`, `red-team-tactics`, `app-builder`, `mobile-design`, `game-development`, `seo-fundamentals`, `geo-fundamentals`, `bash-linux`, `mcp-builder`, `i18n-localization`, `parallel-agents`, `rust-pro`.

### 3.4 Workflows existentes (11)

| Workflow | Relevância para o TCC |
|---|---|
| `/brainstorm` | 🟢 Alta — exploração de requisitos e abordagens |
| `/create` | 🟡 Média — criação de componentes novos (adaptação necessária) |
| `/debug` | 🟢 Alta — debugging de firmware |
| `/plan` | 🟢 Alta — planejamento de fases |
| `/enhance` | 🟡 Média — melhorias iterativas no firmware |
| `/orchestrate` | 🟢 Alta — coordenação de frentes |
| `/test` | 🟡 Média — execução de testes (focado em web, precisa adaptar) |
| `/status` | 🟡 Média — acompanhamento de progresso |
| `/preview` | 🔴 Baixa — servidor de preview (não aplicável a firmware) |
| `/deploy` | 🔴 Baixa — deploy para cloud (firmware usa flash) |
| `/ui-ux-pro-max` | 🟡 Baixa — design avançado (a UI do ESP é simples) |

### 3.5 Scripts existentes (4)

| Script | Relevância |
|---|---|
| `checklist.py` | 🟡 Média — validação geral (foco em web) |
| `verify_all.py` | 🟡 Média — verificação abrangente (foco em web) |
| `auto_preview.py` | 🔴 Baixa |
| `session_manager.py` | 🟡 Baixa |

### 3.6 Lacunas percebidas

| Lacuna | Descrição | Impacto |
|---|---|---|
| **Ausência de agentes acadêmicos** | Não há agentes para escrita de TCC, revisão acadêmica, gestão bibliográfica ou padronização ABNT | 🔴 Crítico — metade do projeto é acadêmico |
| **Ausência de skills para C/C++ embarcado** | AG-kit foca em web/mobile; não há skill para Arduino, ESP32, firmware ou robótica | 🔴 Crítico — o core técnico é embarcado |
| **Ausência de workflows para hardware** | Nenhum workflow para flash, teste de hardware, calibração ou protocolo serial | 🟡 Alto |
| **Ausência de skill para análise experimental** | Sem suporte para registro de dados, análise estatística, geração de gráficos ou comparação quantitativa | 🟡 Alto |
| **Ausência de gestão de referências bibliográficas** | Sem integração com Zotero, BibTeX, ou similar | 🟡 Médio |
| **Ausência de integração com LaTeX/ABNT** | Sem templates ou skills para formatação acadêmica | 🟡 Médio |

---

## 4. Necessidades operacionais do projeto

### 4.1 Frentes de trabalho

| # | Frente | Natureza | Frequência | Fase principal |
|---|---|---|---|---|
| F1 | Pesquisa e levantamento bibliográfico | Acadêmica | Contínua | Estruturação / Pesquisa |
| F2 | Estruturação e escrita do TCC | Acadêmica | Contínua | Pesquisa / Escrita |
| F3 | Revisão técnica do texto | Acadêmica/Técnica | Iterativa | Escrita / Revisão |
| F4 | Revisão acadêmica e padronização (ABNT) | Acadêmica | Iterativa | Escrita / Revisão |
| F5 | Desenvolvimento do firmware (ESP32 + Arduino) | Embarcada | Fases 2–5 | Desenvolvimento |
| F6 | Desenvolvimento da interface web | Frontend/Embarcada | Fases 2–5 | Desenvolvimento |
| F7 | Integração mecânica/eletrônica | Hardware | Fases 2–3 | Desenvolvimento |
| F8 | Planejamento e execução de experimentos | Experimental | Fases 3–5 | Testes |
| F9 | Comparação com UR-10 | Experimental/Acadêmica | Fases 3, 5 | Testes |
| F10 | Documentação técnica do protótipo | Documental | Contínua | Todas |
| F11 | Registro e análise de resultados | Experimental | Fases 3–5 | Testes / Escrita |
| F12 | Geração de relatórios e materiais auxiliares | Documental | Pontual | Revisão final |

---

## 5. Proposta de agentes

### 5.1 Agentes existentes a reutilizar ou adaptar

| Agente | Finalidade no TCC | Responsabilidades | Entradas | Saídas | Momento de uso | Prioridade | Status |
|---|---|---|---|---|---|---|---|
| `orchestrator` | Coordenar frentes paralelas do TCC | Delegar tarefas a agentes especializados, sintetizar resultados | Requisição complexa multi-domínio | Relatório consolidado | Quando a tarefa cruza domínios (ex: firmware + escrita + testes) | Alta | ✅ Reutilizar como está |
| `project-planner` | Planejar fases e decompor em tarefas | Criar planos de fase, task breakdown, dependências | Escopo da fase, requisitos | `{task-slug}.md` com tarefas e critérios | Início de cada fase do TCC | Alta | ✅ Reutilizar como está |
| `frontend-specialist` | Desenvolver interface web do robô | UI dashboard, controles de jog, log viewer | Escopo da web UI, API spec | HTML/CSS/JS para PROGMEM | Fases 2–5 (interface) | Alta | 🔧 Adaptar — precisa entender que o "frontend" é servido por ESP em PROGMEM, sem frameworks |
| `documentation-writer` | Documentar o protótipo | README, API docs, guias de operação, ADRs | Código-fonte, specs, diagramas | Documentação técnica em markdown | Contínuo | Alta | ✅ Reutilizar como está |
| `debugger` | Debug de firmware e comunicação serial | Análise de causa raiz em problemas de hardware/software | Logs seriais, comportamento observado, código | Diagnóstico e correção | Fases 2–5 (quando surgirem bugs) | Alta | 🔧 Adaptar — precisa de contexto sobre protocolo serial e limitações de embarcado |
| `test-engineer` | Planejar e guiar testes de firmware/API | Estratégias de teste para embarcados, cobertura | Plano de testes, código | Casos de teste, roteiros de validação | Fases 2–5 | Média | 🔧 Adaptar — foco atual é Jest/web; precisa cobrir testes manuais e semi-automatizados em hardware |
| `explorer-agent` | Navegação e análise do workspace | Mapear estrutura, dependências, estado atual | Workspace | Survey report | Início de sessão, mudanças estruturais | Média | ✅ Reutilizar como está |
| `code-archaeologist` | Refatoração dos firmwares | Análise de complexidade, sugestões de melhoria | Código `.ino` | Relatório de dívida técnica, refatorações | Fases 4–5 | Baixa | ✅ Reutilizar como está |

### 5.2 Agentes novos recomendados

| Agente | Finalidade | Responsabilidades | Entradas | Saídas | Momento de uso | Prioridade | Status |
|---|---|---|---|---|---|---|---|
| `academic-writer` | Escrita e estruturação do TCC | Redigir capítulos, manter coerência argumentativa, seguir normas acadêmicas, citação correta | Resumo do TCC, referências, dados experimentais, anotações | Texto acadêmico estruturado em markdown | Fases de pesquisa, escrita e revisão | 🔴 Alta | 🆕 Criar novo |
| `academic-reviewer` | Revisão e padronização do texto | Revisar coerência, gramática, lógica argumentativa, padronização ABNT, completude | Rascunho de capítulo/seção | Relatório de revisão com correções e sugestões | Após cada ciclo de escrita | 🔴 Alta | 🆕 Criar novo |
| `firmware-specialist` | Desenvolvimento C/C++ para ESP32 e Arduino | Escrever e revisar firmware, otimizar para memória limitada, gerenciar I/O, protocolo serial | Specs, escopo, ADRs, código existente | Código `.ino`, correções, otimizações | Fases 2–5 (desenvolvimento) | 🔴 Alta | 🆕 Criar novo |
| `experiment-analyst` | Planejamento e análise experimental | Desenhar experimentos, definir métricas, analisar dados, gerar gráficos, comparar com UR-10 | Plano de testes, dados coletados, métricas do UR-10 | Relatórios de análise, tabelas comparativas, gráficos | Fases 3–5 (testes e comparação) | 🟡 Média | 🆕 Criar novo |
| `research-assistant` | Pesquisa e gestão bibliográfica | Buscar referências, resumir artigos, organizar bibliografia, sugerir fontes | Tema, palavras-chave, perguntas de pesquisa | Resumos de artigos, mapa bibliográfico, sugestões | Fases de pesquisa e escrita | 🟡 Média | 🆕 Criar novo |

---

## 6. Proposta de skills

### 6.1 Skills existentes relevantes (reutilizar)

| Skill | Finalidade no TCC | Tipo de tarefa | Agentes que usariam | Dependências | Prioridade | Status |
|---|---|---|---|---|---|---|
| `clean-code` | Padrões de código no firmware | Qualidade de código | `firmware-specialist`, `frontend-specialist` | Nenhuma | Alta | ✅ Reutilizar |
| `architecture` | Decisões arquiteturais do sistema | Arquitetura | `firmware-specialist`, `project-planner` | Nenhuma | Alta | ✅ Reutilizar |
| `plan-writing` | Criação de planos de fase | Planejamento | `project-planner` | Nenhuma | Alta | ✅ Reutilizar |
| `brainstorming` | Exploração de requisitos e abordagens | Descoberta | `project-planner`, `orchestrator` | Nenhuma | Alta | ✅ Reutilizar |
| `systematic-debugging` | Debug de firmware | Debugging | `debugger` | Nenhuma | Alta | ✅ Reutilizar |
| `documentation-templates` | Templates de documentação | Documentação | `documentation-writer` | Nenhuma | Alta | ✅ Reutilizar |
| `api-patterns` | Design da REST API do ESP | API | `firmware-specialist` | Nenhuma | Média | ✅ Reutilizar |
| `code-review-checklist` | Revisão de código do firmware | Revisão | `firmware-specialist`, `code-archaeologist` | Nenhuma | Média | ✅ Reutilizar |
| `powershell-windows` | Automação local | Automação | Qualquer agente | Windows | Média | ✅ Reutilizar |

### 6.2 Skills novas recomendadas

| Skill | Finalidade | Tipo de tarefa | Agentes que usariam | Dependências | Prioridade | Status |
|---|---|---|---|---|---|---|
| `embedded-firmware` | Princípios de desenvolvimento C/C++ para microcontroladores (ESP32, Arduino) | Firmware, I/O, protocolos, gerenciamento de memória | `firmware-specialist`, `debugger` | Nenhuma | 🔴 Alta | 🆕 Criar |
| `academic-writing` | Princípios de escrita acadêmica, estruturação de TCC, coerência argumentativa | Escrita, estruturação, citação | `academic-writer` | Nenhuma | 🔴 Alta | 🆕 Criar |
| `abnt-formatting` | Regras ABNT para formatação de trabalhos acadêmicos (NBR 14724, NBR 6023, etc.) | Padronização, formatação, referências | `academic-reviewer` | Nenhuma | 🔴 Alta | 🆕 Criar |
| `experiment-design` | Planejamento de experimentos, definição de métricas, análise estatística básica | Experimentos, coleta, análise | `experiment-analyst` | Nenhuma | 🟡 Média | 🆕 Criar |
| `robotics-fundamentals` | Conceitos de robótica (DH, FK, IK, perfis de movimento, controle) relevantes ao EB-15 | Desenvolvimento, revisão técnica, escrita | `firmware-specialist`, `academic-writer` | Nenhuma | 🟡 Média | 🆕 Criar |
| `bibliography-management` | Gestão de referências bibliográficas, BibTeX, organização de fontes | Pesquisa, citação, bibliografia | `research-assistant`, `academic-writer` | Nenhuma | 🟡 Média | 🆕 Criar |
| `data-analysis` | Análise de dados experimentais, geração de tabelas e gráficos comparativos | Análise, visualização | `experiment-analyst` | Python (matplotlib, pandas) | 🟡 Média | 🆕 Criar |
| `hardware-testing` | Procedimentos de teste para sistemas embarcados (flash, serial, osciloscópio, multímetro) | Testes de hardware | `test-engineer`, `firmware-specialist` | Ferramentas físicas | 🟡 Média | 🆕 Criar |

---

## 7. Proposta de workflows

### 7.1 Workflows existentes a reutilizar

| Workflow | Objetivo no TCC | Etapas principais | Agentes/skills envolvidos | Gatilho | Resultado | Prioridade | Status |
|---|---|---|---|---|---|---|---|
| `/brainstorm` | Explorar requisitos de cada fase | Perguntas → Opções → Decisão | `project-planner`, `brainstorming` | Início de fase ou decisão complexa | Decisões documentadas | Alta | ✅ Reutilizar |
| `/plan` | Planejar tarefas de cada fase | Análise → Decomposição → `{slug}.md` | `project-planner`, `plan-writing` | Início de cada fase | Plano com tarefas e critérios | Alta | ✅ Reutilizar |
| `/debug` | Debug de firmware ou comunicação | Reproduzir → Isolar → Diagnosticar → Corrigir | `debugger`, `systematic-debugging` | Quando surgir bug | Correção e documentação da causa | Alta | ✅ Reutilizar |
| `/orchestrate` | Coordenar frentes simultâneas | Decompor → Selecionar agentes → Executar → Sintetizar | `orchestrator`, múltiplos agentes | Tarefas multi-domínio | Relatório consolidado | Alta | ✅ Reutilizar |
| `/enhance` | Evoluir firmware iterativamente | Entender → Planejar → Implementar → Verificar | Agente selecionado por domínio | Melhoria incremental | Código melhorado | Média | ✅ Reutilizar |
| `/status` | Acompanhar progresso geral | Ler planos → Resumir estado | `explorer-agent` | Quando necessário | Relatório de progresso | Média | ✅ Reutilizar |

### 7.2 Workflows novos recomendados

| Workflow | Objetivo | Etapas principais | Agentes/skills envolvidos | Gatilho | Resultado | Prioridade | Status |
|---|---|---|---|---|---|---|---|
| `/write-chapter` | Redigir capítulo ou seção do TCC | 1. Reunir insumos (referências, dados, anotações) → 2. Gerar esboço → 3. Redigir texto → 4. Auto-revisar → 5. Salvar em `docs/tcc/` | `academic-writer`, `academic-writing`, `abnt-formatting` | Quando iniciar escrita de um capítulo | Rascunho de capítulo em markdown | 🔴 Alta | 🆕 Criar |
| `/review-chapter` | Revisar capítulo do TCC | 1. Ler capítulo → 2. Verificar coerência → 3. Verificar ABNT → 4. Verificar citações → 5. Gerar relatório com correções | `academic-reviewer`, `abnt-formatting` | Após rascunho de capítulo | Relatório de revisão + correções sugeridas | 🔴 Alta | 🆕 Criar |
| `/flash-test` | Compilar e testar firmware | 1. Compilar → 2. Flash (manual) → 3. Registrar resultado → 4. Atualizar plano de implementação | `firmware-specialist`, `hardware-testing` | Após modificação de firmware | Registro de teste + atualização de status | 🟡 Média | 🆕 Criar |
| `/run-experiment` | Executar e registrar experimento | 1. Selecionar experimento do plano → 2. Executar procedimento → 3. Coletar dados → 4. Salvar em `dados/` → 5. Analisar → 6. Gerar relatório | `experiment-analyst`, `experiment-design`, `data-analysis` | Quando executar teste experimental | Dados coletados + relatório de análise | 🟡 Média | 🆕 Criar |
| `/compare-ur10` | Comparar desempenho com UR-10 | 1. Selecionar métrica → 2. Coletar dados do EB-15 → 3. Obter referência do UR-10 → 4. Comparar → 5. Gerar tabela/gráfico → 6. Documentar conclusões | `experiment-analyst`, `data-analysis` | Fases de comparação | Tabela comparativa + análise textual | 🟡 Média | 🆕 Criar |
| `/firmware-dev` | Desenvolver feature de firmware | 1. Ler escopo/ADR → 2. Planejar implementação → 3. Desenvolver → 4. Guiar teste → 5. Documentar | `firmware-specialist`, `embedded-firmware`, `clean-code` | Quando implementar nova feature no firmware | Código + documentação de mudança | 🟡 Média | 🆕 Criar |
| `/bibliography` | Buscar e organizar referências | 1. Definir tema/palavras-chave → 2. Buscar fontes → 3. Resumir → 4. Classificar por relevância → 5. Adicionar a `docs/referencias/` | `research-assistant`, `bibliography-management` | Quando pesquisar sobre tema específico | Referências organizadas + resumos | 🟡 Média | 🆕 Criar |
| `/progress-report` | Gerar relatório de progresso | 1. Varrer planos das fases → 2. Verificar itens completados → 3. Listar pendências → 4. Gerar relatório | `orchestrator`, `explorer-agent` | Semanalmente ou sob demanda | Relatório em markdown | Baixa | 🆕 Criar |

---

## 8. Mapeamento por fase do TCC

### Fase 1: Estruturação

**Foco:** Organizar o workspace, criar agentes e skills, definir estrutura de capítulos.

| Componente | Atividade |
|---|---|
| Agentes | Criar `academic-writer`, `academic-reviewer`, `firmware-specialist` |
| Skills | Criar `academic-writing`, `abnt-formatting`, `embedded-firmware` |
| Workflows | Criar `/write-chapter`, `/review-chapter`, `/firmware-dev` |
| Tarefas | Definir estrutura do TCC em `docs/tcc/`, configurar convenções de escrita |

### Fase 2: Pesquisa

**Foco:** Fundamentação teórica, levantamento bibliográfico, escrita dos capítulos iniciais.

| Componente | Atividade |
|---|---|
| Agentes | `research-assistant`, `academic-writer` |
| Skills | `bibliography-management`, `academic-writing` |
| Workflows | `/bibliography`, `/write-chapter`, `/brainstorm` |
| Tarefas | Pesquisar robótica, automação, IoT, UR-10; redigir introdução, fundamentação teórica |

### Fase 3: Desenvolvimento

**Foco:** Evolução do firmware, integração hardware, interface web.

| Componente | Atividade |
|---|---|
| Agentes | `firmware-specialist`, `frontend-specialist`, `debugger` |
| Skills | `embedded-firmware`, `clean-code`, `architecture`, `systematic-debugging` |
| Workflows | `/firmware-dev`, `/flash-test`, `/debug`, `/enhance` |
| Tarefas | Implementar fases 2–4 do `plano_implementacao.md` (validação de hardware, movimento real, robustez) |

### Fase 4: Testes e comparação

**Foco:** Execução de experimentos, coleta de dados, comparação com UR-10.

| Componente | Atividade |
|---|---|
| Agentes | `experiment-analyst`, `test-engineer`, `firmware-specialist` |
| Skills | `experiment-design`, `data-analysis`, `hardware-testing` |
| Workflows | `/run-experiment`, `/compare-ur10`, `/flash-test` |
| Tarefas | Executar plano de testes, coletar dados, comparar métricas com UR-10 |

### Fase 5: Escrita e integração de resultados

**Foco:** Redigir capítulos de metodologia, resultados, discussão.

| Componente | Atividade |
|---|---|
| Agentes | `academic-writer`, `experiment-analyst` |
| Skills | `academic-writing`, `data-analysis`, `abnt-formatting` |
| Workflows | `/write-chapter`, `/compare-ur10`, `/progress-report` |
| Tarefas | Escrever metodologia, resultados, discussão, integrando dados experimentais |

### Fase 6: Revisão final

**Foco:** Revisão completa, padronização, preparação para entrega.

| Componente | Atividade |
|---|---|
| Agentes | `academic-reviewer`, `documentation-writer` |
| Skills | `abnt-formatting`, `academic-writing`, `documentation-templates` |
| Workflows | `/review-chapter`, `/progress-report` |
| Tarefas | Revisar todos os capítulos, padronizar referências, verificar ABNT, gerar documentação final do protótipo |

---

## 9. Recomendações de organização do workspace

### 9.1 Estrutura de pastas sugerida

A estrutura sugerida abaixo **respeita o padrão já existente** e adiciona apenas subpastas necessárias:

```
TCC Fernando Artur/
├── .agent/                              # (existente) AG-kit — sem alterações na raiz
│   ├── agents/                          # (existente) + novos agentes
│   │   ├── academic-writer.md           # 🆕
│   │   ├── academic-reviewer.md         # 🆕
│   │   ├── firmware-specialist.md       # 🆕
│   │   ├── experiment-analyst.md        # 🆕
│   │   └── research-assistant.md        # 🆕
│   ├── skills/                          # (existente) + novas skills
│   │   ├── embedded-firmware/           # 🆕
│   │   ├── academic-writing/            # 🆕
│   │   ├── abnt-formatting/            # 🆕
│   │   ├── experiment-design/           # 🆕
│   │   ├── robotics-fundamentals/       # 🆕
│   │   ├── bibliography-management/     # 🆕
│   │   ├── data-analysis/              # 🆕
│   │   └── hardware-testing/           # 🆕
│   └── workflows/                       # (existente) + novos workflows
│       ├── write-chapter.md             # 🆕
│       ├── review-chapter.md            # 🆕
│       ├── flash-test.md                # 🆕
│       ├── run-experiment.md            # 🆕
│       ├── compare-ur10.md              # 🆕
│       ├── firmware-dev.md              # 🆕
│       ├── bibliography.md              # 🆕
│       └── progress-report.md           # 🆕
├── Software e Firmware/                 # (existente) — sem alterações
├── docs/                                # (existente) — expandir
│   ├── Resumo_tcc.md                    # (existente)
│   ├── levantamento.md                  # (este documento)
│   ├── tcc/                             # 🆕 Capítulos do TCC em markdown
│   │   ├── 00-pre-textuais.md
│   │   ├── 01-introducao.md
│   │   ├── 02-fundamentacao.md
│   │   ├── 03-metodologia.md
│   │   ├── 04-resultados.md
│   │   ├── 05-discussao.md
│   │   ├── 06-conclusao.md
│   │   └── 07-referencias.md
│   └── referencias/                     # 🆕 Referências bibliográficas organizadas
├── dados/                               # 🆕 Dados experimentais
│   ├── eb15/                            # Dados do protótipo
│   └── ur10/                            # Dados de referência do UR-10
└── relatorios/                          # 🆕 Relatórios de análise e progresso
```

### 9.2 Convenções recomendadas

| Convenção | Regra |
|---|---|
| Capítulos do TCC | Prefixo numérico `XX-nome.md` em `docs/tcc/` |
| Dados experimentais | CSV com cabeçalhos descritivos em `dados/` |
| Referências | Um arquivo `.md` por fonte ou um arquivo central em `docs/referencias/` |
| Relatórios | Markdown com data no nome em `relatorios/` |
| Commits | Mensagens em português, prefixadas por frente: `[firmware]`, `[tcc]`, `[exp]`, `[infra]` |

---

## 10. Lacunas, riscos e dependências

### 10.1 Lacunas críticas

| # | Lacuna | Consequência se não resolvida |
|---|---|---|
| L1 | Não há agente para escrita acadêmica | Escrita do TCC ficará sem assistência estruturada |
| L2 | Não há skill para C/C++ embarcado | Desenvolvimento de firmware sem orientação do AG-kit |
| L3 | Não há workflow para ciclo de escrita acadêmica | Escrita desorganizada e não rastreável |
| L4 | Não há modelo de análise experimental | Dados coletados sem metodologia padronizada |

### 10.2 Riscos

| # | Risco | Probabilidade | Impacto | Mitigação |
|---|---|---|---|---|
| R1 | Escopo de agentes/skills novos ser grande demais e atrasar o início real do trabalho | Média | Alto | Implementar em ondas (ver §11), começando pelo mínimo viável |
| R2 | Skills de firmware/robótica ficarem genéricas e pouco úteis | Média | Médio | Basear skills nos docs técnicos já existentes (`escopo.md`, ADRs, `protocolo_serial.md`) |
| R3 | Padronização ABNT ser complexa demais para codificar em uma skill | Baixa | Médio | Focar nas regras mais frequentes; deixar verificação detalhada para revisão manual |
| R4 | Dados do UR-10 não estarem disponíveis para comparação | Alta | Alto | Definir fontes de dados do UR-10 cedo (datasheets, artigos, testes no laboratório) |
| R5 | Firmwares monolíticos (.ino) dificultarem a assistência do agente | Baixa | Médio | Os arquivos já são bem setorizados internamente; o agente pode navegar por seções |

### 10.3 Dependências externas

| Dependência | Descrição | Status |
|---|---|---|
| Medidas reais do EB-15 (DH parameters) | Necessárias para FK/IK real | ⏳ Pendente |
| Chaves de fim de curso | Necessárias para homing funcional | ⏳ Pendente |
| Level shifter 3.3V ↔ 5V | Necessário para comunicação serial ESP ↔ Arduino | ⏳ Pendente |
| Steps/degree por junta | Necessário para movimentos precisos | ⏳ Pendente |
| Acesso ao UR-10 em laboratório | Necessário para testes comparativos | ⏳ A confirmar |
| Norma ABNT vigente (NBR 14724, etc.) | Referência para padronização | ✅ Disponível publicamente |

---

## 11. Ordem recomendada de implementação

A criação dos componentes deve seguir uma ordem que gere **impacto máximo com esforço mínimo**, desbloqueando frentes de trabalho o mais cedo possível.

### Onda 1 — Fundação (prioridade máxima)

> Desbloqueia a escrita do TCC e o desenvolvimento de firmware.

| # | Componente | Tipo | Justificativa |
|---|---|---|---|
| 1.1 | `firmware-specialist` (agente) | 🆕 Criar | Desbloqueia toda a frente de firmware |
| 1.2 | `embedded-firmware` (skill) | 🆕 Criar | Fornece princípios e padrões para o agente de firmware |
| 1.3 | `academic-writer` (agente) | 🆕 Criar | Desbloqueia a escrita do TCC |
| 1.4 | `academic-writing` (skill) | 🆕 Criar | Princípios de escrita acadêmica |
| 1.5 | `abnt-formatting` (skill) | 🆕 Criar | Padronização ABNT |
| 1.6 | `/write-chapter` (workflow) | 🆕 Criar | Fluxo de trabalho para escrita |
| 1.7 | `/firmware-dev` (workflow) | 🆕 Criar | Fluxo de trabalho para firmware |
| 1.8 | Criar estrutura `docs/tcc/` | 📁 Pasta | Organização dos capítulos |

### Onda 2 — Revisão e testes (alta prioridade)

> Adiciona capacidade de revisão acadêmica e teste de hardware.

| # | Componente | Tipo | Justificativa |
|---|---|---|---|
| 2.1 | `academic-reviewer` (agente) | 🆕 Criar | Revisão estruturada do texto |
| 2.2 | `/review-chapter` (workflow) | 🆕 Criar | Fluxo de revisão |
| 2.3 | `hardware-testing` (skill) | 🆕 Criar | Procedimentos de teste de hardware |
| 2.4 | `/flash-test` (workflow) | 🆕 Criar | Compilação e teste guiado |

### Onda 3 — Experimentação e comparação (média prioridade)

> Prepara a infraestrutura para a fase experimental.

| # | Componente | Tipo | Justificativa |
|---|---|---|---|
| 3.1 | `experiment-analyst` (agente) | 🆕 Criar | Análise de dados experimentais |
| 3.2 | `experiment-design` (skill) | 🆕 Criar | Desenho de experimentos |
| 3.3 | `data-analysis` (skill) | 🆕 Criar | Análise e visualização de dados |
| 3.4 | `/run-experiment` (workflow) | 🆕 Criar | Fluxo de experimentação |
| 3.5 | `/compare-ur10` (workflow) | 🆕 Criar | Comparação com referência industrial |
| 3.6 | Criar estrutura `dados/` | 📁 Pasta | Organização de dados experimentais |

### Onda 4 — Pesquisa e apoio (prioridade variável)

> Complementa o ecossistema com capacidades auxiliares.

| # | Componente | Tipo | Justificativa |
|---|---|---|---|
| 4.1 | `research-assistant` (agente) | 🆕 Criar | Assistência bibliográfica |
| 4.2 | `bibliography-management` (skill) | 🆕 Criar | Gestão de referências |
| 4.3 | `robotics-fundamentals` (skill) | 🆕 Criar | Conceitos de robótica para contexto |
| 4.4 | `/bibliography` (workflow) | 🆕 Criar | Fluxo de pesquisa |
| 4.5 | `/progress-report` (workflow) | 🆕 Criar | Relatórios de progresso |
| 4.6 | Criar estrutura `relatorios/` | 📁 Pasta | Organização de relatórios |

---

## 12. Conclusão

O workspace atual possui uma **infraestrutura AG-kit robusta para desenvolvimento web/mobile**, com 20 agentes, 37 skills e 11 workflows. No entanto, o TCC do braço robótico EB-15 tem necessidades que cruzam dois mundos distintos:

1. **Desenvolvimento técnico embarcado** — firmware C/C++ para ESP32-S3 e Arduino Uno, integração de hardware, protocolos seriais, robótica, IoT.
2. **Trabalho acadêmico** — pesquisa bibliográfica, escrita estruturada, padronização ABNT, análise experimental, comparação com referência industrial.

O AG-kit existente cobre parcialmente o primeiro mundo (planejamento, debugging, documentação, coordenação) mas **não possui nenhum componente para o segundo mundo**. Além disso, falta suporte específico para C/C++ embarcado, que é o core técnico do projeto.

A proposta deste levantamento é preencher essas lacunas com **5 agentes novos**, **8 skills novas** e **8 workflows novos**, organizados em 4 ondas de implementação que priorizam o desbloqueio das frentes mais críticas (escrita acadêmica e firmware).

Com a implementação das Ondas 1 e 2, o AG-kit se torna capaz de **sustentar o ciclo completo do TCC**: desde a pesquisa e estruturação até a escrita final, passando pelo desenvolvimento, testes e comparação com o UR-10. As Ondas 3 e 4 complementam o ecossistema para a fase experimental e auxiliar.

O diferencial dessa abordagem é que **o AG-kit passa a funcionar como um sistema de trabalho contínuo**, onde cada fase do TCC alimenta a próxima através de agentes especializados, skills reutilizáveis e workflows encadeados — mantendo rastreabilidade entre requisitos, implementação, testes e escrita acadêmica.
