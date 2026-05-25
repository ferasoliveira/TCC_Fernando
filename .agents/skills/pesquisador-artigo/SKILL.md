---
name: pesquisador-artigo
description: Leitura crítica, destilação e citação BibTeX de artigos acadêmicos em PDF para justificar escolhas técnicas do TCC.
---

# Skill: Pesquisador de Artigo Acadêmico (pesquisador_artigo)

Habilidade especializada na leitura crítica de trabalhos acadêmicos (artigos, livros, teses) em PDF, com foco em destilar conhecimentos para a fundamentação teórica de robótica e arquiteturas de controle distribuído.

## Instruções de Análise e Extração:

1. **Leitura Estruturada e Rápida:**
   - **Objetivo do Artigo:** Focar na leitura do *Abstract/Resumo*, da *Introdução* (especialmente no final, onde o problema e a solução proposta são apresentados) e das *Conclusões*.
   - **Resumo Acadêmico:** Identificar a metodologia adotada, as ferramentas e plataformas de hardware/software utilizadas, os experimentos realizados e os principais resultados obtidos.

2. **Geração de Insights para o TCC (Crucial):**
   - Cruzar diretamente as descobertas do artigo com o escopo e arquitetura do TCC atual:
     - **Braço Robótico EB15:** Hardware de baixo custo (6 graus de liberdade).
     - **Arquitetura de Controle Distribuído:** ESP32 S3 (Master) coordenando tarefas complexas e interface web, e Arduino Uno (Slave) executando movimentos críticos em tempo real.
     - **Feedback de Junta:** Sensores magnéticos AS5600 atuando em malha fechada.
     - **Comunicação Inter-processador:** Canal de comunicação Serial (UART) e protocolo de sincronização robusto.
     - **Conectividade:** Interface Web embarcada e comunicação via protocolo RTDE.
   - Responder ativamente à pergunta: *"De que forma este artigo justifica as escolhas técnicas do meu TCC ou pode ser utilizado para validar meus resultados?"*

3. **Avaliação de Relevância (0 a 10):**
   - Atribuir uma nota de 0 a 10 (ex: `8.5/10`) indicando de forma totalmente objetiva o quanto o trabalho auxilia no desenvolvimento do TCC.
   - **CRÍTICO:** Fornecer apenas a nota numérica, sem qualquer tipo de explicação ou texto adicional associado a este campo.

4. **Formatação da Citação (LaTeX/BibTeX):**
   - Fornecer a referência bibliográfica em formato BibTeX estrito, pronta para inclusão no arquivo `.bib` do LaTeX.
   - Exemplo para Livros (`@book`):
     ```bibtex
     @book{siciliano2009,
         author    = {Bruno Siciliano and Lorenzo Sciavicco and Luigi Villani and Giuseppe Oriolo},
         title     = {Robotics: Modelling, Planning and Control},
         edition   = {1},
         address   = {London},
         publisher = {Springer},
         year      = {2009},
         isbn      = {978-1-84628-641-4}
     }
     ```
   - Exemplo para Artigos de Periódicos (`@article`):
     ```bibtex
     @article{ramirez2022,
         author  = {Irvin G. Ramírez-Alpízar and Brunny Ramírez-Rojas and José A. Brenes-André and Carlos A. Villarreal-Ledezma},
         title   = {Design of an affordable IoT open-source robot arm for online teaching of robotics courses during the pandemic contingency},
         journal = {HardwareX},
         volume  = {12},
         pages   = {e00374},
         year    = {2022},
         doi     = {10.1016/j.ohx.2022.e00374}
     }
     ```

5. **Escrita no `wiki/artigos.md`:**
   - Adicionar o trabalho no final do documento `wiki/artigos.md` respeitando estritamente o seguinte padrão (use separador `---` entre os artigos):
     ```markdown
     ---

     ### Título: [Título Completo do Artigo]
     - **Arquivo:** `raw/[nome_do_arquivo.pdf]`
     - **Relevância para o TCC:** [Nota de 0 a 10, ex: 8.5/10]
     - **Objetivo:** [Objetivo geral do artigo em 1 ou 2 parágrafos objetivos]
     - **Resumo:** [Resumo detalhado com metodologia, testes e principais resultados obtidos]
     - **Insights:** [Como o artigo se relaciona com o TCC - por exemplo: justificativa do uso de arquitetura distribuída Master/Slave, uso de sensores AS5600, controle de motores com drivers TB6600, ou protocolo de comunicação RTDE]
     - **Citação:**
     ```bibtex
     [Código BibTeX completo aqui]
     ```
     ```

