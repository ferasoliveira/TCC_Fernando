# Relatório de Revisão: Capítulo 1 - Introdução
**ID da Revisão:** `introducao_rev1.md`  
**Data:** 25 de Maio de 2026  
**Avaliador:** Revisor TCC (Agente de Revisão Científica)

Este relatório apresenta uma auditoria técnica, acadêmica e gramatical detalhada do arquivo `TCC_escrito/1-introducao.tex` sob as diretrizes do workflow `/revisor-tcc`. O objetivo é garantir a excelência científica, coerência de mecatrônica do protótipo EB15 e integridade de formatação sem alterar o arquivo original.

---

## 📊 Notas de Avaliação (Resumo)

* **Gramática e Normas ABNT:** `10.0 / 10.0`
* **Qualidade de Citações e ref.bib:** `10.0 / 10.0`
* **Rigores Técnicos (Mecatrônica/Robótica):** `10.0 / 10.0`
* **Conformidade Científica (Objetividade):** `10.0 / 10.0`
* **Sintaxe LaTeX Acadêmica:** `9.8 / 10.0`

---

## 🔍 Detalhamento das Avaliações

### 1. Gramática e Normas ABNT (Nota: 10.0/10.0)
* **Pontos Críticos:** Nenhum erro ortográfico ou de concordância verbal/nominal identificado. A aplicação da norma culta do português brasileiro é excelente.
* **Alertas:** A redação está perfeitamente impessoal, priorizando a voz passiva sintética ("descreve-se", "analisa-se", "pretende-se"), o que confere maturidade científica ao texto.
* **Sugestões de Melhoria:** Manter exatamente o mesmo tom impessoal e formal nos capítulos de Referencial Teórico e Metodologia.

---

### 2. Citação e ref.bib (Nota: 10.0/10.0)
* **Pontos Críticos:** Todas as referências utilizadas no arquivo `.tex` (`siciliano2009robotics`, `craig2005introduction`, `okter2025ros`, `marton2009distributed`, `lynch2017modern`, `lewin2023scurve`, `kowsalyadevi2025comprehensive`, `tang2022research`, `siciliano2016springer`, `nissov2025simultaneous`) estão perfeitamente mapeadas no arquivo `ref.bib` com metadados corretos.
* **Alertas:** O limite de densidade de citações foi respeitado (máximo de uma citação relevante por parágrafo, ou grupos combinados coerentes).
* **Sugestões de Melhoria:** Quando for redigir o Capítulo 2, certifique-se de importar os novos artigos para o `ref.bib` usando chaves curtas e limpas seguindo o mesmo padrão do primeiro capítulo (ex: `autorANOsobrenome`).

---

### 3. Rigores Técnicos - EB15 (Nota: 10.0/10.0)
* **Pontos Críticos:** Alinhamento técnico absoluto com o hardware físico e a lógica embarcada descrita nos diagramas do robô:
  * Divisão exata de juntas (J1-J3 no mestre ESP32 S3; J4-J6 no escravo Arduino Uno).
  * Menção correta à topologia de atuadores e eletrônica de potência (Drivers TB6600 na base; CNC Shield v3.1 nas juntas superiores).
  * Explicação detalhada da comunicação inter-processador por UART com protocolo leve de dados, mecanismo de *handshake* e disparo simultâneo por *hardware trigger* (sinal digital físico direto) para eliminação de latência serial na partida física.
  * Integração correta dos encoders AS5600 via barramento $I^2C$ para malha fechada.
* **Alertas:** Nenhum desalinhamento conceitual encontrado.
* **Sugestões de Melhoria:** Nenhuma. O texto serve como excelente introdução para justificar os capítulos metodológicos futuros.

---

### 4. Conformidade Científica e Objetividade (Nota: 10.0/10.0)
* **Pontos Críticos:** O texto foi construído sem "juridiquês" ou rebuscamentos desnecessários. É objetivo, direto e flui logicamente, guiando o leitor da contextualização global à problematização das CPUs, justificativa do arranjo de hardware, objetivos e estrutura.
* **Alertas:** A estrutura de cada parágrafo (Ideia Central -> Desenvolvimento -> Gancho Lógico) está impecável.
* **Sugestões de Melhoria:** Manter a mesma concisão no desenvolvimento metodológico para evitar prolixidade.

---

### 5. Sintaxe LaTeX Acadêmica (Nota: 9.8/10.0)
* **Pontos Críticos:** O código LaTeX está perfeitamente formatado, livre de caracteres especiais desprotegidos ou problemas de compilação.
* **Alertas:**
  * **Uso do Termo I2C em Modo Matemático (Linhas 15 e 28):**  
    * *Localização:* `...encoders magnéticos absolutos AS5600 via barramento de comunicação $I^2C$---torna-se indispensável...` e `...sensores magnéticos AS5600 de malha fechada via barramento $I^2C$ em todas...`
    * *Problema:* A notação `$I^2C$` renderiza as letras em itálico de modo matemático de variáveis (como $I \times C$). Em termos técnicos de engenharia, a grafia correta deve manter as letras "I" e "C" em fonte romana regular (reta), com apenas o "2" sobrescrito.
    * *Correção Sugerida:* Substituir `$I^2C$` por `I\textsuperscript{2}C` (ou `I$^2$C` caso o compilador não tenha o pacote de texto básico configurado).
* **Sugestões de Melhoria:** Aplicar a alteração acima para obter 10.0 completo em sintaxe tipográfica.
