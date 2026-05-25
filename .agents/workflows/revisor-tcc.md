---
name: revisor-tcc
description: Realiza auditoria técnica, acadêmica e gramatical de capítulos do TCC, fornecendo notas e feedbacks sem alterar os originais.
---

# Workflow: Revisor TCC (revisor_tcc)

Workflow de auditoria focado em garantir a excelência acadêmica e técnica, sem realizar alterações diretas no arquivo principal do TCC.

## Passo a Passo:

1. **Mapeamento de Contexto:**
   - Ler `wiki/index.md` e os arquivos em `TCC_escrito/`.

2. **Geração de Documento de Revisão:**
   - Criar um arquivo em `raw/revisoes/[nome_do_capitulo]_rev[N].md` (ex: `cap1_rev3.md`).

3. **Critérios de Avaliação (Notas 0 a 10):**
   - **3.1 Gramática/ABNT:** Avaliar lógica, formatação de graduação e regras ABNT.
   - **3.2 Citação:** Validar presença no `ref.bib`, adequação do contexto e limite de uma citação por parágrafo.
   - **3.3 Técnica:** Verificar alinhamento com o planejamento do EB15 e conceitos de robótica.
   - **3.4 Conformidade:** Checar se o texto é objetivo, simples e formal, evitando complexidade desnecessária.
   - **3.5 Sintaxe LaTeX:** Validar padrões de escrita em código LaTeX acadêmico.

4. **Registro de Feedback:**
   - Para cada critério, listar: Pontos Críticos, Alertas e Sugestões de Melhoria, indicando a localização exata no texto.

5. **Finalização:**
   - Retornar ao usuário apenas que a revisão foi concluída, informando as notas finais de cada critério.