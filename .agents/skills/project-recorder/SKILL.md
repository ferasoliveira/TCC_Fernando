---
name: project-recorder
description: Registra de forma pragmática o progresso técnico do projeto na documentação interna e audita a eficiência cognitiva (tokens) do agente. Deve ser ativado automaticamente quando uma tarefa é concluída, finalizada, resumida, registrada ou entregue.
---

# Project Recorder & Cognitive Auditor Skill

Esta skill atua como o registro oficial e imutável de evolução do projeto. Ela deve ser invocada ao final de cada entrega ou correção relevante para garantir rastreabilidade técnica e policiar o desperdício de tokens na janela de contexto, mitigando loops de raciocínio ineficientes.

## Quando usar esta skill

- Logo após finalizar o desenvolvimento de uma feature, refatoração ou correção de bug.
- Quando um obstáculo complexo (como comportamentos inesperados de registradores ou protocolos de rede) for superado e precisar ser catalogado.
- Ao encerrar sessões de trabalho para consolidar o estado atual do repositório.

## Como usar e Convenções

Sempre adote uma postura analítica e fria: Foque estritamente no padrão **"Input -> Processo -> Output"**, eliminando termos subjetivos ou adjetivos desnecessários.

### 1. Protocolo de Atualização do `wiki/changelog.md`
Toda tarefa finalizada exige uma nova linha inserida no topo do arquivo (ordem cronológica estritamente inversa). Utilize o template exato abaixo:

```markdown
- [YYYY-MM-DD] - [CATEGORIA]: [Resumo direto e objetivo em uma única frase do que foi implementado ou corrigido].
```
*Categorias permitidas:* `FEATURE`, `BUGFIX`, `REFACTOR`, `DOCS`, `INFRA`.

### 2. Protocolo de Atualização do `wiki/lessons_learned.md`
Se a tarefa envolveu mais de duas tentativas erradas, caminhos redundantes ou análises prolixas por parte do agente (Token Bloat), uma nova entrada deve ser registrada obrigatoriamente utilizando a estrutura abaixo:

```markdown
### [Título Curto do Desafio Técnico] (Data: YYYY-MM-DD)
- **Problema:** [Descrição exata do obstáculo técnico ou do loop de pensamento ineficiente ocorrido].
- **Solução:** [A abordagem de engenharia que efetivamente resolveu o problema].
- **Regra Futura (Boa Prática):** [Uma diretriz imperativa e curta de "Atalho de Execução" para pular direto para a resposta em interações futuras].
```

### 3. Rotina de Auditoria de Contexto (Meta-cognição)
Antes de salvar as alterações, o agente deve fazer uma varredura interna do histórico recente da conversa respondendo mentalmente:
- *Eu demorei para chegar na solução por falta de dados ou por preciosismo de escrita?*
- *O código final poderia ter sido gerado na primeira resposta se eu tivesse seguido qual premissa?*

Se houver um atalho cognitivo claro, ele deve ser explicitado como uma **Regra Futura** para blindar os próximos prompts contra desperdício de tokens.

## Anti-Padrões a Evitar

- [ ] **Registrar por registrar:** Alterações cosméticas que envolvam apenas comentários ou formatações simples de arquivos não devem gerar entradas no `lessons_learned.md`, apenas no `changelog.md`.
- [ ] **Poluição da Raiz:** Nunca crie arquivos de log ou notas soltas no diretório raiz do projeto. Toda a documentação e histórico devem residir exclusivamente dentro da pasta estruturada `docs/`.
- [ ] **Falta de Linha Temporal:** Omitir ou errar a data das entradas. O ano corrente é **2026**.