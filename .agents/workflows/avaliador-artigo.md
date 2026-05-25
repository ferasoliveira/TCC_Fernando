---
name: avaliador-artigo
description: Guia a leitura estruturada, interpretação e catalogação automática de artigos científicos em PDF da pasta /raw para a Wiki.
---

# Workflow: Avaliador de Artigo Acadêmico (avaliador_artigo)

Este workflow é responsável por guiar a leitura estruturada, interpretação e catalogação automática de artigos científicos colocados na pasta `/raw`. O objetivo é enriquecer a Wiki do projeto com fundamentação teórica conectada diretamente aos desafios técnicos do TCC.

## Passo a Passo:

1. **Leitura de Contexto:**
   - Ler o arquivo `wiki/index.md` e o arquivo `brainstorming.md` (se disponível) para recuperar os objetivos gerais do TCC e a arquitetura detalhada do braço robótico EB15 (ESP32 Master, Arduino Slave, sensores AS5600, protocolo RTDE, etc.).

2. **Leitura e Interpretação do Artigo:**
   - Localizar o arquivo PDF indicado na pasta `raw/` e realizar a leitura completa e interpretação técnica do mesmo.

3. **Análise de Conteúdo e Catalogação na Wiki:**
   - Aplicar a skill **Pesquisador de Artigo Acadêmico (`pesquisador_artigo`)** para analisar o artigo.
   - Extrair os seguintes campos de forma detalhada e técnica:
     - **Título:** Título oficial do artigo.
     - **Arquivo:** Caminho do arquivo relativo (ex: `raw/[nome_do_arquivo.pdf]`).
     - **Relevância para o TCC:** Nota objetiva de 0 a 10 (ex: `8.5/10`), sem qualquer explicação textual no mesmo campo.
     - **Objetivo:** Foco principal e propósito do trabalho.
     - **Resumo:** Abordagem metodológica, testes e conclusões obtidas.
     - **Insights:** Conexão direta de valor para o TCC (como o trabalho ajuda nas escolhas de hardware, controle distribuído, comunicação serial ou controle em malha fechada).
     - **Citação:** Citação completa no formato BibTeX aceito pelo LaTeX.
   - Adicionar o registro formatado ao final do arquivo `wiki/artigos.md`.

4. **Retorno de Resumo no Chat:**
   - Responder ao usuário no chat com o resumo exato gerado, de forma extremamente objetiva e direta, sem enrolações.
   - A resposta deve começar exatamente com a frase:
     ```text
     Pesquisa concluída, segue o resumo:
     ```
   - E conter a mesma ficha gerada no passo anterior:
     ```markdown
     ### Título: [Título do Artigo]
     - **Arquivo:** `raw/[nome_do_arquivo.pdf]`
     - **Relevância para o TCC:** [Nota de 0 a 10, ex: 8.5/10]
     - **Objetivo:** [Objetivo do Artigo]
     - **Resumo:** [Resumo das Contribuições]
     - **Insights:** [Insights Técnicos para o TCC]
     - **Citação:**
     ```bibtex
     [Código BibTeX]
     ```
     ```
   - Finalizar a resposta logo após a citação.
