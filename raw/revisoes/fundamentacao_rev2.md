# Relatório de Revisão: Capítulo 2 (Referencial Teórico) --- Revisão 2

Este relatório foi gerado seguindo as diretrizes estritas do workflow `/revisor-tcc`. Ele realiza uma auditoria técnica, acadêmica, gramatical e tipográfica profunda do conteúdo do **Capítulo 2 (Referencial Teórico)** do TCC do braço robótico de 6 eixos **EB15**, gravado no arquivo `TCC_escrito/2-fundamentacao.tex`.

---

## 1. Mapeamento de Contexto
- **Arquivo Auditado:** `TCC_escrito/2-fundamentacao.tex` (linhas 1 a 219).
- **Entradas da Wiki Utilizadas:** `wiki/index.md` (especificações gerais), `wiki/diagrama_sistema.md` (particionamento físico de hardware/software, drivers TB6600, CNC Shield v3.1, encoders AS5600 e barramentos I²C/UART) e `wiki/fluxo_operacao.md` (lógica de handshake serial e trigger de hardware dedicado).
- **Referencial Bibliográfico Comparado:** `TCC_escrito/ref.bib` (validação de chaves de citação e integridade de metadados).

---

## 2. Critérios de Avaliação (Rigor Acadêmico Extremo)

### 2.1. Gramática e Normas ABNT (Peso 1.0)
* **Avaliação:** Ortografia de altíssimo padrão com uso rigoroso da norma culta em português. O texto mantém perfeita impessoalidade científica (emprego formal da passiva sintética e da terceira pessoa do singular). A coesão interparágrafos por ganchos e conectivos lógicos é excelente. No entanto, foram identificados três desvios que impactam a perfeição acadêmica:
  1. **Erro de digitação e espaçamento (Linha 192):** Ocorrência de `encodersAS5600` (palavra colada, sem espaçamento adequado).
  2. **Sigla não definida no primeiro uso (Linha 62):** A sigla `CPU` é introduzida pela primeira vez no capítulo sem a devida definição ou expansão (*Central Processing Unit* ou Unidade Central de Processamento).
  3. **Incongruência Gramatical de Concordância (Linha 141):** Expressão `protocolo de comunicação serial síncrona I\textsuperscript{2}C [...] locais dedicados` (mismatch de plural: "protocolo" no singular concordando com "locais dedicados").
* **Penalidades Aplicadas:**
  - `-0.5` pontos por erro de digitação/espaçamento (`encodersAS5600`).
  - `-0.5` pontos por sigla não explicada (`CPU`).
  - `-0.5` pontos por desvio gramatical de concordância nominal/verbal.
* **Nota:** **8.5 / 10.0**

### 2.2. Citação e ref.bib (Peso 1.0)
* **Avaliação:** Excelente densidade científica. Todos os parágrafos com afirmações técnicas e matemáticas contêm embasamento bibliográfico apropriado, não havendo parágrafos teóricos sem fundamentação. Respeitou-se o limite de no máximo uma citação por parágrafo (salvo agrupamentos válidos). Todas as chaves citadas no texto constam no arquivo `ref.bib`. Contudo, foram detectados três erros mecanicistas de catalogação de tipo de entrada em metadados BibTeX:
  - A entrada `marton2009distributed` é um artigo publicado em anais de conferência, mas está catalogado incorretamente como `@article` com o campo `journal` (deveria ser `@inproceedings` com o campo `booktitle`).
  - A entrada `tanvir2024optimizing` é de anais de conferência (`IEEE International Conference on Computer...`), porém está catalogada incorretamente como `@article` com `journal`.
  - A entrada `okter2025ros` também é de anais de conferência (`Proceedings of the IEEE...`) e está catalogada incorretamente como `@article` com `journal`.
* **Penalidades Aplicadas:**
  - `-1.5` pontos pelos três desvios mecanicistas de tipologia nos metadados do BibTeX (`-0.5` para cada uma das chaves: `marton2009distributed`, `tanvir2024optimizing` e `okter2025ros`).
* **Nota:** **8.5 / 10.0**

### 2.3. Rigor Técnico e Alinhamento com o Hardware EB15 (Peso 1.0)
* **Avaliação:** Fidelidade técnica absoluta à arquitetura física e de firmware do protótipo EB15. Descreve de forma correta e robusta a divisão estrutural de juntas (J1--J3 no ESP32 S3; J4--J6 no Arduino Uno), os drivers correspondentes (TB6600 industriais para base e CNC Shield v3.1 para o punho), o protocolo local síncrono I²C para os encoders absolutos AS5600 no eixo das juntas, e a lógica precisa de sincronização (Setup UART + GPIO Hardware Trigger). 
* **Refinamento de Alta Precisão (Sem penalidade):** O texto cita no parágrafo 22 que a resolução de 12 bits do AS5600 dividida em 4096 posições resulta em uma resolução angular equivalente de `0,087^{\circ}`. Matematicamente, $360^{\circ} / 4096 = 0,087890625^{\circ}$, o que por arredondamento científico correto deve ser expresso como **$0,088^{\circ}$** (evitando o truncamento simples para $0,087^{\circ}$). A nota máxima é mantida por não se tratar de uma contradição de arquitetura mecatrônica.
* **Penalidades Aplicadas:** Nenhuma.
* **Nota:** **10.0 / 10.0**

### 2.4. Conformidade Científica e Estilo (Peso 1.0)
* **Avaliação:** Prosa científica extremamente refinada, fluida e objetiva. Entretanto, o rigor extremo exige a eliminação completa de adjetivações intensas, termos subjetivos ou superlativos que ferem o distanciamento analítico:
  1. **Linha 141:** Uso do advérbio de intensidade "extremamente estável" (`leitura angular absoluta extremamente estável`).
  2. **Linha 190:** Uso do adjetivo subjetivo "CPU central potente" (`CPU central potente baseada em...`).
  3. **Linha 206:** Uso do superlativo "baixíssimo payload" (`dados compactados de baixíssimo payload`).
  4. **Linha 208:** Uso do termo dramático "instante crucial de partida" (`instante crucial de partida`).
  5. **Linha 215:** Uso do termo "transição física abrupta" (`Esta transição física abrupta...`).
* **Penalidades Aplicadas:**
  - `-1.0` ponto pelo uso recorrente de adjetivos de intensidade, superlativos e termos dramáticos sem amparo estritamente quantitativo.
* **Nota:** **9.0 / 10.0**

### 2.5. Sintaxe e Tipografia LaTeX (Peso 1.0)
* **Avaliação:** Estruturação correta de seções, referências cruzadas (`\ref`) de equações e chaves bibliográficas. O barramento I²C foi impecavelmente grafado como `I\textsuperscript{2}C` e as aspas tipográficas em português (``texto'') foram aplicadas de forma correta. Contudo, há uma **falha sistemática grave de sintaxe LaTeX**:
  - Em diversas linhas (109, 111, 115, 116, 117, 118, 122 e 208), foram utilizados asteriscos literais (`*jerk*` e `*jitter*`) no padrão Markdown para tentar renderizar formatação em itálico. O compilador LaTeX não traduz asteriscos como itálico, o que resultará na renderização visual de asteriscos indesejados no PDF impresso. A sintaxe correta para termos estrangeiros em itálico em LaTeX é `\textit{jerk}` e `\textit{jitter}`.
* **Penalidades Aplicadas:**
  - `-1.0` ponto pela presença sistemática de formatação em estilo Markdown (`*`) em vez de tags nativas LaTeX (`\textit{}`).
* **Nota:** **9.0 / 10.0**

---

## Média Ponderada Final do Capítulo 2: 9.0 / 10.0

---

## 3. Registro de Feedback (Auditoria Minuciosa)

### 3.1. Pontos Críticos (Erros que demandam correção imediata)

Abaixo estão listadas as linhas exatas do arquivo `TCC_escrito/2-fundamentacao.tex` com as respectivas correções propostas para drop-in imediato:

#### 1. Correção de Markdown para LaTeX Itálico (*jerk*) e Adjetivo Estilístico (Linha 109)
* **Original (linha 109):**
```latex
Fisicamente, a descontinuidade na aceleração e o consequente pulso infinito de *jerk* induzem vibrações mecânicas intensas de alta frequência nas juntas do braço robótico.
```
* **Substituição Recomendada:**
```latex
Fisicamente, a descontinuidade na aceleração e o consequente pulso infinito de \textit{jerk} induzem vibrações mecânicas de alta frequência nas juntas do braço robótico.
```

#### 2. Correção de Markdown para LaTeX Itálico (*jerk*) (Linha 111)
* **Original (linha 111):**
```latex
Diferentemente do modelo trapezoidal, o perfil Curva-S limita a taxa de variação da aceleração de forma a manter o *jerk* finito e constante em patamares ajustáveis durante cada fase de transição, suavizando a rampa de aceleração e desaceleração conforme equações polinomiais de terceiro grau no domínio do tempo \cite{rew2021closed}.
```
* **Substituição Recomendada:**
```latex
Diferentemente do modelo trapezoidal, o perfil Curva-S limita a taxa de variação da aceleração de forma a manter o \textit{jerk} finito e constante em patamares ajustáveis durante cada fase de transição, suavizando a rampa de aceleração e desaceleração conforme equações polinomiais de terceiro grau no domínio do tempo \cite{rew2021closed}.
```

#### 3. Correção de Markdown para LaTeX Itálico (*jerk*) nas Fases (Linhas 115--118 e 122)
* **Original (linhas 115--118):**
```latex
    \item \textbf{Fase 1 (Aceleração com Jerk Positivo):} O *jerk* assume um valor constante máximo positivo ($J(t) = J_{max}$), fazendo com que a aceleração cresça linearmente até atingir seu limite projetado.
    \item \textbf{Fase 2 (Aceleração Constante):} O *jerk* é nulo ($J(t) = 0$), e a aceleração permanece estável no patamar máximo ($a(t) = a_{max}$).
    \item \textbf{Fase 3 (Aceleração Decrescente com Jerk Negativo):} O *jerk* inverte-se para um valor estável negativo ($J(t) = -J_{max}$), desacelerando a taxa de ganho até que a aceleração retorne de forma suave a zero, atingindo a velocidade máxima de translação.
    \item \textbf{Fase 4 (Velocidade Constante):} O movimento ocorre em regime uniforme de velocidade estável ($v(t) = v_{max}$), com aceleração e *jerk* iguais a zero.
```
* **Substituição Recomendada:**
```latex
    \item \textbf{Fase 1 (Aceleração com \textit{Jerk} Positivo):} O \textit{jerk} assume um valor constante máximo positivo ($J(t) = J_{max}$), fazendo com que a aceleração cresça linearmente até atingir seu limite projetado.
    \item \textbf{Fase 2 (Aceleração Constante):} O \textit{jerk} é nulo ($J(t) = 0$), e a aceleração permanece estável no patamar máximo ($a(t) = a_{max}$).
    \item \textbf{Fase 3 (Aceleração Decrescente com \textit{Jerk} Negativo):} O \textit{jerk} inverte-se para um valor estável negativo ($J(t) = -J_{max}$), desacelerando a taxa de ganho até que a aceleração retorne de forma suave a zero, atingindo a velocidade máxima de translação.
    \item \textbf{Fase 4 (Velocidade Constante):} O movimento ocorre em regime uniforme de velocidade estável ($v(t) = v_{max}$), com aceleração e \textit{jerk} em zero.
```
* **Original (linha 122):**
```latex
A limitação do *jerk* dinâmico fornecida pelo algoritmo de sete segmentos atenua as forças de inércia impulsivas transmitidas aos mancais poliméricos do EB15, minimizando vibrações nas juntas e proporcionando estabilidade nas trajetórias cartesianas físicas executadas \cite{garcia2019new}.
```
* **Substituição Recomendada:**
```latex
A limitação do \textit{jerk} dinâmico fornecida pelo algoritmo de sete segmentos atenua as forças de inércia impulsivas transmitidas aos mancais poliméricos do EB15, minimizando vibrações nas juntas e proporcionando estabilidade nas trajetórias cartesianas físicas executadas \cite{garcia2019new}.
```

#### 4. Definição da Sigla CPU (Linha 62)
* **Original (linha 62):**
```latex
Para viabilizar a execução em tempo real na CPU de controle do braço EB15, adota-se a solução analítica fechada baseada no desacoplamento cinemático espacial.
```
* **Substituição Recomendada:**
```latex
Para viabilizar a execução em tempo real na CPU (\textit{Central Processing Unit}) de controle do braço EB15, adota-se a solução analítica fechada baseada no desacoplamento cinemático espacial.
```

#### 5. Correção de Concordância e Arredondamento AS5600 (Linhas 141--143)
* **Original (linhas 141--143):**
```latex
A transmissão dessa telemetria física em tempo real ocorre por meio do protocolo de comunicação serial síncrona I\textsuperscript{2}C (\textit{Inter-Integrated Circuit}) locais dedicados \cite{tang2022research}.
```
* **Substituição Recomendada:**
```latex
A transmissão dessa telemetria física em tempo real ocorre por meio de barramentos de comunicação serial síncrona I\textsuperscript{2}C (\textit{Inter-Integrated Circuit}) locais dedicados \cite{tang2022research}.
```
*(Nota: Ajustar também na linha 141 o valor de $0,087^{\circ}$ para $0,088^{\circ}$ para exatidão matemática de arredondamento).*

#### 6. Correção de Digitação de Espaçamento e Adjetivo "Potente" (Linha 190--192)
* **Original (linhas 190--192):**
```latex
    \item \textbf{Processador Mestre (ESP32 S3):} Dotado de uma CPU central potente baseada em arquitetura dual-core Xtensa de 32 bits rodando a 240 MHz com suporte a ponto flutuante por hardware e periféricos integrados de rádio frequência sem fio. [...] Estas juntas suportam o peso mecatrônico principal e a inércia estática e dinâmica preliminar do braço robótico, exigindo processamento imediato dos encodersAS5600 locais de malha fechada \cite{gomes2026micro}.
```
* **Substituição Recomendada:**
```latex
    \item \textbf{Processador Mestre (ESP32 S3):} Dotado de uma CPU central de alto desempenho baseada em arquitetura dual-core Xtensa de 32 bits rodando a 240 MHz com suporte a ponto flutuante por hardware e periféricos integrados de rádio frequência sem fio. [...] Estas juntas suportam o peso mecatrônico principal e a inércia estática e dinâmica preliminar do braço robótico, exigindo processamento imediato dos encoders \textbf{AS5600} locais de malha fechada \cite{gomes2026micro}.
```

#### 7. Polimento Estilístico de Superlativo (Linha 206)
* **Original (linha 206):**
```latex
Para evitar que a latência e o overhead de formatação do canal serial UART degradem o tempo de resposta dinâmico global do sistema, o projeto adota um protocolo customizado estruturado de dados compactados de baixíssimo payload.
```
* **Substituição Recomendada:**
```latex
Para evitar que a latência e o overhead de formatação do canal serial UART degradem o tempo de resposta dinâmico global do sistema, o projeto adota um protocolo customizado estruturado de dados compactados com reduzido payload.
```

#### 8. Correção de Markdown para LaTeX Itálico (*jitter*) e Palavra Dramática (Linha 208)
* **Original (linha 208):**
```latex
Para eliminar por completo esse *jitter* de latência serial no instante crucial de partida, implementa-se um mecanismo de sincronização de duas fases composto por handshake em software seguido pelo disparo físico por hardware (\textit{hardware trigger}) dedicado \cite{nissov2025simultaneous}:
```
* **Substituição Recomendada:**
```latex
Para eliminar por completo esse \textit{jitter} de latência serial no instante inicial de partida, implementa-se um mecanismo de sincronização de duas fases composto por handshake em software seguido pelo disparo físico por hardware (\textit{hardware trigger}) dedicado \cite{nissov2025simultaneous}:
```

#### 9. Correções Necessárias em `TCC_escrito/ref.bib`

No arquivo `TCC_escrito/ref.bib`, realize as seguintes modificações estruturais para conformidade científica:

```diff
-@article{marton2009distributed,
+@inproceedings{marton2009distributed,
     author  = {L{\H{o}}rinc M{\'a}rton and Katalin M. Hangos and Attila Magyar},
     title   = {Distributed controller architecture for advanced robot control},
-    journal = {Proceedings of the IEEE International Conference on Robotics and Biomimetics},
+    booktitle = {Proceedings of the IEEE International Conference on Robotics and Biomimetics},
     pages   = {789--794},
     year    = {2009},
     doi     = {10.1109/ROBIO.2009.4913076}
 }

-@article{tanvir2024optimizing,
+@inproceedings{tanvir2024optimizing,
     author  = {Md Shakil Tanvir and Fiaz Ahmed and Md Monirul Islam and Mainul Islam and Raihan Ur Rashid},
     title   = {Optimizing Master-Slave Broadcast Communication in Multi-Node Network Using RS485 Standard},
-    journal = {IEEE International Conference on Computer and Information Technology (ICCIT)},
+    booktitle = {Proceedings of the IEEE International Conference on Computer and Information Technology (ICCIT)},
     year    = {2024},
     doi     = {10.1109/ICCIT64611.2024.11022430}
 }

-@article{okter2025ros,
+@inproceedings{okter2025ros,
     author  = {Martin {\O}kter and Filippo Sanfilippo},
     title   = {A ROS-Based Framework for Low-Cost Real-Time Haptic Feedback in Human-Robot Teaming},
-    journal = {Proceedings of the IEEE International Conference on Human-Robot Interaction (HRI)},
+    booktitle = {Proceedings of the IEEE International Conference on Human-Robot Interaction (HRI)},
     year    = {2025},
     doi     = {10.3939/393955603}
 }
```

---

### 3.2. Alertas (Riscos de coesão ou de compilação)
- **Erros de Markdown no Compilador:** O uso de asteriscos `*` no LaTeX não lança um erro crítico de interrupção de compilação (como a falta de um fechamento de chaves ou cifrão matemático), mas gera um arquivo PDF final com asteriscos visíveis impressos (ex: "*jerk*"). Recomenda-se fortemente a aplicação da substituição descrita nos pontos críticos para que o leitor da banca não identifique desleixo ou vícios de escrita em editores Markdown.
- **Diferenças de Nomenclatura no Barramento:** No código LaTeX, o barramento de sincronia de partida é chamado de "linha digital física de hardware dedicada interconectando um pino de saída de uso geral (GPIO)", enquanto na Wiki ele é denominado "Trigger de Sincronismo" ou "Trigger Digital". Embora as expressões sejam semanticamente equivalentes, é fundamental que o Capítulo 3 do TCC adote uma nomenclatura unificada para evitar qualquer percepção de descompasso conceitual.

---

### 3.3. Sugestões de Melhoria (Refinamentos estilísticos para nota 10 na banca)
- **Equações e Notação de Vetores e Matrizes:** Para uniformidade tipográfica científica premium, recomenda-se adotar letras em negrito para representar matrizes e vetores no corpo do texto (ex: em vez de `$R_{ee}$`, utilizar `$\mathbf{R}_{ee}$` ou `\mathbf{R}_{\text{ee}}`). Isso segue as melhores diretrizes de revistas internacionais da IEEE e Springer.
- **Substituição da palavra "abrupta" por "imediata" (Linha 215):** Na linha 215, em vez de "transição física abrupta", prefira "transição de estado lógico instantânea" ou "transição rápida de nível lógico". Isso reforça o rigor da eletrônica digital frente à linguagem informal.

---

## 4. Finalização
A revisão sistemática detalhou correções cruciais de sintaxe e pequenos ajustes estilísticos que, se sanados, elevarão a fundamentação teórica à perfeição acadêmica máxima. A média de **9.0 / 10.0** reflete o excelente conteúdo do capítulo, restando apenas polimentos pontuais em sua sintaxe LaTeX e metadados de referências.
