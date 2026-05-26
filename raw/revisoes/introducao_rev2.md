# Relatório de Revisão: Capítulo 1 - Introdução
**ID da Revisão:** `introducao_rev2.md`  
**Data:** 25 de Maio de 2026  
**Avaliador:** Revisor TCC (Agente de Revisão Científica - Rigor Extremo)

Este relatório apresenta uma auditoria técnica, acadêmica e gramatical de **Rigor Acadêmico Extremo** do arquivo `TCC_escrito/1-introducao.tex` sob as diretrizes do workflow atualizado `/revisor-tcc`. O objetivo é certificar a conformidade máxima para publicação e conformidade ABNT.

---

## 📊 Notas de Avaliação (Rigor Extremo)

* **Gramática e Normas ABNT (Peso 1.0):** `9.0 / 10.0` (Penalidade: -1.0)
* **Qualidade de Citações e ref.bib (Peso 1.0):** `8.5 / 10.0` (Penalidade: -1.5)
* **Rigores Técnicos (Mecatrônica/Robótica) (Peso 1.0):** `10.0 / 10.0` (Sem penalidades)
* **Conformidade Científica (Objetividade) (Peso 1.0):** `8.5 / 10.0` (Penalidade: -1.5)
* **Sintaxe LaTeX Acadêmica (Peso 1.0):** `9.0 / 10.0` (Penalidade: -1.0)

**Nota Final Ponderada:** `9.0 / 10.0` (Rigor Científico Extremo)

---

## 🔍 Detalhamento das Avaliações

### 1. Gramática e Normas ABNT (Nota: 9.0/10.0)
* **Pontos Críticos:** 
  * **Linha 15 (Sigla não definida):** O termo `I2C` (escrito como `$I^2C$`) é introduzido sem que seu significado original em inglês e português (*Inter-Integrated Circuit*) seja formalmente explicitado na primeira ocorrência do capítulo. (Penalidade: -0.5)
  * **Linha 26 (Sigla não definida):** A sigla `UART` (*Universal Asynchronous Receiver-Transmitter*) é introduzida sem definição formal no texto em seu primeiro uso. (Penalidade: -0.5)
* **Alertas:** A concordância verbal, nominal e a impessoalidade científica na terceira pessoa do singular estão perfeitamente executadas em todo o corpo do texto.
* **Sugestões de Melhoria:**
  * *Correção na Linha 15:* Substituir por: `...como os encoders magnéticos absolutos AS5600 via barramento de comunicação serial síncrona I\textsuperscript{2}C (\textit{Inter-Integrated Circuit})---torna-se indispensável...`
  * *Correção na Linha 26:* Substituir por: `...por um canal de comunicação serial assíncrona UART (\textit{Universal Asynchronous Receiver-Transmitter}) otimizado...`

---

### 2. Citação e ref.bib (Nota: 8.5/10.0)
* **Pontos Críticos:**
  * **Infração do Limite de Citação por Parágrafo:** A nova diretriz estabelece o limite estrito de no máximo uma citação por parágrafo (exceto se agrupadas na mesma frase). Foram detectadas infrações estruturais com múltiplas citações em frases distintas no mesmo parágrafo:
    * *Parágrafo 1 (linhas 4):* Contém `\cite{siciliano2009robotics}` na primeira frase e `\cite{craig2005introduction}` na terceira frase. (Penalidade: -0.25)
    * *Parágrafo 2 (linha 6):* Contém `\cite{okter2025ros}` e `\cite{marton2009distributed}` em frases distintas. (Penalidade: -0.25)
    * *Parágrafo 4 (linha 13):* Contém `\cite{lynch2017modern}` e `\cite{lewin2023scurve}` em frases distintas. (Penalidade: -0.25)
    * *Parágrafo 5 (linha 15):* Contém `\cite{kowsalyadevi2025comprehensive}` e `\cite{tang2022research}` em frases distintas. (Penalidade: -0.25)
    * *Parágrafo 8 (linha 24):* Contém `\cite{siciliano2016springer}` e `\cite{kowsalyadevi2025comprehensive}` em frases distintas. (Penalidade: -0.25)
    * *Parágrafo 10 (linha 28):* Contém `\cite{tang2022research}` e `\cite{okter2025ros}` em frases distintas. (Penalidade: -0.25)
* **Alertas:** A integridade de metadados do `ref.bib` está excelente. Todas as chaves mapeadas existem no arquivo bibliográfico.
* **Sugestões de Melhoria:** Consolidar as citações na mesma frase final do parágrafo ou fazer a transição textual de modo que o parágrafo conte com uma única referência contextualizada, separando as orações se necessário.

---

### 3. Rigor Técnico e Alinhamento EB15 (Nota: 10.0/10.0)
* **Pontos Críticos:** Nenhum desvio técnico em relação ao hardware do protótipo EB15.
* **Alertas:** A distinção de controle dinâmico entre o ESP32 S3 (J1-J3 e controle supervisório) e o Arduino Uno (J4-J6 de baixo nível de temporização) está explicada com precisão. O trigger físico de hardware é devidamente caracterizado como elemento de eliminação de latência serial no início síncrono da movimentação das juntas.

---

### 4. Conformidade Científica e Estilo (Nota: 8.5/10.0)
* **Pontos Críticos:**
  * **Uso de Qualificadores Subjetivos não Respaldados (Adjetivação):**
    * *Linha 4:* O termo `investimentos vultosos` apresenta cunho subjetivo. (Penalidade: -0.5)
    * *Linha 24:* A expressão `drivers robustos TB6600` qualifica o componente de forma informal. (Penalidade: -0.5)
    * *Linha 28:* A expressão `sistemas avançados` utiliza termo impreciso cientificamente. (Penalidade: -0.5)
* **Alertas:** A clareza de escrita, simplicidade e vocabulário técnico geral são de alto padrão.
* **Sugestões de Melhoria:**
  * *Substituição na Linha 4:* Mudar `investimentos vultosos de capital` por `elevados investimentos de capital`.
  * *Substituição na Linha 24:* Mudar `drivers robustos TB6600` por `drivers industriais TB6600`.
  * *Substituição na Linha 28:* Mudar `conectividade de sistemas avançados` por `conectividade de sistemas de nível industrial` ou remover a palavra `avançados`.

---

### 5. Sintaxe e Tipografia LaTeX (Nota: 9.0/10.0)
* **Pontos Críticos:**
  * **Uso de Modo Matemático Incorreto para Sigla de Barramento (Linhas 15 e 28):**
    * *Ocorrências:* Uso do termo `$I^2C$` nas linhas 15 e 28.
    * *Problema:* A notação `$I^2C$` renderiza as letras "I" e "C" em modo itálico matemático de produto de variáveis (equivalente a $I \times C$). Sob a tipografia acadêmica LaTeX padrão, apenas o algarismo "2" deve estar sobrescrito, e as letras do acrônimo devem permanecer em fonte romana reta regular. (Penalidade: -0.5 por ocorrência; total: -1.0)
* **Alertas:** O uso de hifens, aspas duplas, ambientes e rótulos (`\label{...}`) está perfeito.
* **Sugestões de Melhoria:**
  * *Correção na Linha 15 e 28:* Substituir o código `$I^2C$` por `I\textsuperscript{2}C` (ou `I$^2$C`).
