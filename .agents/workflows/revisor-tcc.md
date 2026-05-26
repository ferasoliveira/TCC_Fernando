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

3. **Critérios de Avaliação (Notas 0 a 10 - Rigor Acadêmico Extremo):**
   - **3.1 Gramática/ABNT (Peso 1.0):**
     * **Critérios Estritos:** Verificação ortográfica minuciosa; aplicação impecável da norma padrão; impessoalidade científica absoluta (sem traços de 1ª pessoa ou informalidade); coesão interparágrafos por conectivos adequados; definição obrigatória de todas as siglas em seu primeiro uso (ex: *RTDE (Real-Time Data Exchange)*); e formatação progressiva rigorosa NBR 14724.
     * **Penalidades:** -0.5 pontos para cada desvio ortográfico/gramatical; -0.5 para cada sigla não explicada; -1.0 para quebra de impessoalidade.
   - **3.2 Citação e ref.bib (Peso 1.0):**
     * **Critérios Estritos:** Todo parágrafo com afirmação científica ou técnica complexa deve conter citação direta/indireta; citações diretas com mais de 3 linhas devem usar recuo de 4 cm e fonte menor; todas as chaves citadas devem existir no `ref.bib`; metadados BibTeX devem estar completos (autor, título, editora/periódico, ano, DOI/URL e ISBN/ISSN); limite estrito de no máximo uma citação por parágrafo (exceto agrupamentos na mesma frase).
     * **Penalidades:** -1.0 ponto para cada parágrafo teórico sem fundamentação; -0.5 para cada erro mecanicista em citações ou metadados de BibTeX faltantes.
   - **3.3 Rigor Técnico e Alinhamento EB15 (Peso 1.0):**
     * **Critérios Estritos:** Fidelidade absoluta à arquitetura física e lógica documentada na Wiki: juntas (J1-J3 no ESP32 S3; J4-J6 no Arduino Uno); barramentos de instrumentação (I2C para encoders AS5600; UART para comunicação inter-processador); eletrônica de potência (TB6600 e CNC Shield v3.1); lógica de sincronização (UART handshake e hardware trigger físico). Qualquer contradição técnica ou imprecisão mecânica invalida a nota máxima.
     * **Penalidades:** -1.5 pontos para cada erro ou contradição técnica em relação ao hardware e fluxos do EB15.
   - **3.4 Conformidade Científica e Estilo (Peso 1.0):**
     * **Critérios Estritos:** Texto extremamente objetivo e direto, sem prolixidade, adjetivações excessivas ou termos subjetivos não provados (ex: evitar "excelente", "incrível" ou "perfeito"); cada parágrafo deve apresentar uma tese estruturada de desenvolvimento coerente com gancho lógico para o parágrafo subsequente.
     * **Penalidades:** -1.0 ponto para cada parágrafo redundante, adjetivação subjetiva sem dados de suporte ou quebra drástica de fluxo lógico.
   - **3.5 Sintaxe e Tipografia LaTeX (Peso 1.0):**
     * **Critérios Estritos:** Código limpo e compilável; uso obrigatório de ambientes formais de equações (`equation` ou `align`); uso correto de aspas tipográficas em LaTeX (``texto'' em vez de "texto"); hifenizações e travessões corretos (`--` para intervalos e `---` para explicações); tipografia rigorosa para termos de computação/física (ex: `I\textsuperscript{2}C` ou `I$^2$C` para barramento I2C, em vez de `$I^2C$`, que coloca as letras em itálico de variável).
     * **Penalidades:** -0.5 pontos por erros tipográficos menores ou uso incorreto de caracteres especiais matemáticos em blocos de texto.

4. **Registro de Feedback (Auditoria Minuciosa):**
   - Para cada critério avaliado, o revisor deve detalhar rigorosamente:
     * **Pontos Críticos (Erros que demandam correção imediata):** Indicação da linha e substituição exata de código/texto recomendada.
     * **Alertas (Potenciais fragilidades de coesão ou compilação):** Riscos associados e justificativas científicas.
     * **Sugestões de Melhoria (Refinamentos estilísticos para nota 10):** Propostas de elevação de maturidade acadêmica.

5. **Finalização:**
   - Retornar ao usuário apenas que a revisão foi concluída, informando as notas finais de cada critério.