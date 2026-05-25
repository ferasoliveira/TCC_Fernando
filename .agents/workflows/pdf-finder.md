---
name: pdf-finder
description: Localiza e realiza o download automático de PDFs de artigos científicos a partir de links ou descrições do usuário.
---

# Workflow: Localizador de PDFs (pdf-finder)

Este workflow é responsável por localizar e realizar o download automático de artigos científicos, livros ou trabalhos acadêmicos a partir de uma lista de links ou descrições fornecidas pelo usuário, salvando-os de forma organizada.

## Passo a Passo:

1. **Identificação dos Alvos:**
   - Interpretar a lista de links ou instruções com detalhes dos artigos (título, autor, ano) enviada pelo usuário no prompt.

2. **Pesquisa na Web:**
   - Para cada artigo listado:
     - Realizar pesquisas estruturadas na internet (usando buscadores acadêmicos como Google Scholar, arXiv, ResearchGate, servidores de universidades) para localizar a versão completa e aberta em PDF do artigo/livro.
     - Obter o link de download direto do arquivo PDF.

3. **Download e Armazenamento:**
   - Garantir a existência do diretório `/raw/artigos/` na raiz do projeto (criando-o caso não exista).
   - Efetuar o download do PDF completo e salvá-lo em `/raw/artigos/` utilizando nomes de arquivos claros e normalizados (ex: `titulo_do_artigo.pdf`).

4. **Confirmação Padronizada no Chat:**
   - **CRÍTICO:** Para cada artigo encontrado e baixado com sucesso, a resposta no chat deve conter única e exclusivamente a frase:
     ```text
     Artigo [nome] achado com sucesso
     ```
     *(onde `[nome]` é o título real do artigo correspondente).*
   - Não adicione explicações, saudações ou textos adicionais na resposta do chat.
