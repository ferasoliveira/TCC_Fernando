# Revisão Técnica e Acadêmica — Capítulo 3: Metodologia e Desenvolvimento

**Arquivo revisado:** `TCC_escrito/3-metodologia.tex`  
**Workflow aplicado:** `.agents/workflows/revisor-tcc.md`  
**Data:** 2026-05-29  
**Rodada:** Rev 3  
**Modo:** Auditoria sem alteração direta do capítulo

---

## 1. Mapeamento de Contexto

Foram consultados `wiki/index.md`, `wiki/diagrama_sistema.md`, `wiki/fluxo_operacao.md`, `wiki/requisitos_codigo.md`, os capítulos em `TCC_escrito/` e o arquivo `TCC_escrito/ref.bib`.

O Capítulo 3 está bem estruturado e tecnicamente denso, mas a revisão rigorosa identificou divergências relevantes entre o texto e documentos atuais da wiki, principalmente em trigger físico, barramento I2C, modo de micropasso e matriz de hardware. Como o workflow define fidelidade absoluta à wiki, essas divergências reduzem a nota técnica até que a fonte canônica seja unificada.

---

## 2. Notas Finais

| Critério | Peso | Nota |
|---|---:|---:|
| 3.1 Gramática / ABNT | 1,0 | **8,0 / 10** |
| 3.2 Citação e `ref.bib` | 1,0 | **7,5 / 10** |
| 3.3 Rigor Técnico e Alinhamento EB15 | 1,0 | **6,0 / 10** |
| 3.4 Conformidade Científica e Estilo | 1,0 | **8,0 / 10** |
| 3.5 Sintaxe e Tipografia LaTeX | 1,0 | **8,5 / 10** |

**Nota geral:** **7,6 / 10**

---

## 3.1 Gramática / ABNT — Nota: 8,0

### Pontos Críticos

1. **Linha 6 — estrangeirismo e hifenização inadequada**

Trecho atual:
```latex
o co-processador auxiliar Arduino Uno [...] a interconexão UART de baixo payload
```

Substituição recomendada:
```latex
o coprocessador auxiliar Arduino Uno [...] a interconexão UART de carga útil reduzida
```

Justificativa: `coprocessador` é a forma preferível sem hífen no uso técnico atual; `payload` pode ser mantido em contextos de protocolo, mas aqui a versão em português melhora a formalidade.

2. **Linha 50 — grafia de “bidirecional”**

Trecho atual:
```latex
protocolo serial síncrono bi-direcional
```

Substituição recomendada:
```latex
protocolo serial síncrono bidirecional
```

3. **Linha 204 — “ratificar a eficácia” antes dos resultados**

Trecho atual:
```latex
Para ratificar a eficácia da arquitetura distribuída desenvolvida...
```

Substituição recomendada:
```latex
Para avaliar a eficácia da arquitetura distribuída desenvolvida...
```

Justificativa: a metodologia ainda não comprova eficácia; ela define como a eficácia será avaliada.

### Alertas

- As siglas principais estão majoritariamente definidas, mas `GPIO` aparece na linha 54 antes de sua definição explícita. Recomenda-se definir no primeiro uso: `GPIO (\textit{General Purpose Input/Output})`.
- A unidade `100 Hz` na linha 207 deveria seguir o padrão tipográfico já usado no capítulo: `100\,Hz`.

### Sugestões de Melhoria

- Substituir `Desta forma` na linha 216 por `Dessa forma`, mais natural no português acadêmico brasileiro.
- Evitar “baixo nível” repetido na linha 6 e linha 168 quando o contexto já indicar firmware embarcado.

---

## 3.2 Citação e `ref.bib` — Nota: 7,5

### Pontos Críticos

1. **Parágrafos técnicos sem citação direta**

Os seguintes trechos fazem afirmações técnicas específicas sem fonte no próprio parágrafo:

- Linha 18: capacidade do ESP32 S3 para cinemática inversa e Curva-S.
- Linhas 22--27: especificações dos motores, reduções, drivers TB6600, tensão e micropasso.
- Linhas 52--58: estratégia de varredura I2C e conflito com `SoftwareSerial`.
- Linhas 68--78: pinagem UART, divisor resistivo e limites lógicos.
- Linha 102: cálculo de latência do frame UART.
- Linhas 181--199: formulação discreta do PID e função de tangente hiperbólica.
- Linhas 204--213: procedimentos experimentais.

Exemplo de reforço recomendado para a linha 102:
```latex
Esta latência sub-milissegundo garante que o canal serial não sature o fluxo de controle em tempo real, em coerência com a análise comparativa de transmissão UART discutida por Dong~\cite{dong2022comparative}.
```

2. **Metadados incompletos em chaves citadas**

Todas as chaves citadas no Capítulo 3 existem em `ref.bib`. Porém, sob o critério estrito do workflow, há metadados incompletos:

- `dong2022comparative`: ausência de DOI, URL, ISSN ou identificador verificável.
- `nissov2025simultaneous`: referência arXiv sem DOI, URL ou campo `eprint`.

Substituição estrutural recomendada para `nissov2025simultaneous`:
```bibtex
@article{nissov2025simultaneous,
  author  = {Morten Nissov and Nikhil Khedekar and Kostas Alexis},
  title   = {Simultaneous Triggering and Synchronization of Sensors and Onboard Computers},
  journal = {arXiv preprint arXiv:2507.05717},
  year    = {2025},
  eprint  = {2507.05717},
  archivePrefix = {arXiv}
}
```

### Alertas

- Há agrupamentos com duas citações no mesmo parágrafo (`tanvir2024optimizing, dong2022comparative`; `nissov2025simultaneous, banuelos2026timing`). Isso é aceitável porque ocorre na mesma frase, mas deve ser mantido apenas quando as funções das fontes forem distintas e explícitas.

### Sugestões de Melhoria

- Inserir uma citação de datasheet ou referência técnica primária para especificações elétricas do TB6600, A4988, ATmega328P e ESP32 S3, pois o capítulo depende de valores de tensão, corrente, memória e pinagem.

---

## 3.3 Rigor Técnico e Alinhamento EB15 — Nota: 6,0

### Pontos Críticos

1. **Linhas 119 e 121 — trigger no pino 9 contradiz wiki/requisitos**

Capítulo 3:
```latex
GPIO 4 [...] ao pino digital \texttt{9} do microcontrolador ATmega328P
```

Wiki/requisitos:
- `wiki/index.md`: GPIO 4 -> Pino 2 / INT0.
- `wiki/requisitos_codigo.md`: `Trigger Digital: GPIO 4 -> INT0 / Pino 2`.
- `wiki/plano_acao.md`: configurar pino digital 2 para interrupção `INT0`.

Se a wiki for canônica, substituir por:
```latex
Esta linha conecta diretamente um pino de saída digital rápido do ESP32 S3 (GPIO 4) ao pino digital \texttt{2} do Arduino Uno, correspondente à interrupção externa INT0 do ATmega328P.
```

E substituir a descrição de polling por:
```latex
No Arduino Uno, a borda de descida no pino \texttt{2} aciona a rotina de serviço de interrupção associada à INT0, ativando imediatamente os temporizadores de passo.
```

Se o protótipo real foi alterado para pino 9 com polling, então a wiki deve ser atualizada, pois o Capítulo 3 está coerente com uma arquitetura diferente daquela documentada.

2. **Linhas 52--55 — ausência de TCA9548A contradiz `requisitos_codigo.md`**

Capítulo 3 descreve canais I2C separados e `Software I\textsuperscript{2}C` em A2/A3. Já `wiki/requisitos_codigo.md` exige multiplexador **TCA9548A de 8 canais**, endereço `0x70`, canais 0--5.

Se a wiki for canônica, substituir a estratégia por:
```latex
A varredura dos seis encoders AS5600 é realizada por meio do multiplexador I\textsuperscript{2}C TCA9548A, necessário devido ao endereço fixo \texttt{0x36} dos sensores. O ESP32 S3 seleciona os canais 0 a 2 para J1--J3, enquanto o Arduino Uno seleciona os canais 3 a 5 para J4--J6, mantendo a taxa de amostragem de 200\,Hz.
```

Se o projeto físico usa barramentos independentes sem TCA9548A, atualizar `wiki/requisitos_codigo.md`, `wiki/plano_acao.md` e a matriz de rastreabilidade.

3. **Linhas 27 e 41 — micropasso 1/16 contradiz requisito de 1/4**

Capítulo 3:
```latex
modo de micropasso de 1/16
```

Wiki/requisitos:
```markdown
Micropasso 1/4
```

Correção conforme wiki:
```latex
modo de micropasso de 1/4
```

Se 1/16 for configuração física real, atualizar `RNF05` e a matriz de hardware.

4. **Linhas 22--24 e 36--38 — motores e tensões divergem da matriz de rastreabilidade**

Capítulo 3:
- J1/J2: NEMA 23; J3: NEMA 17; J5: NEMA 14; J6: NEMA 11.
- TB6600 em 24\,V.

`wiki/requisitos_codigo.md`:
- J1--J6: NEMA 17.
- TB6600 e CNC Shield em 12\,V.

Essa divergência não pode ficar implícita. Escolher uma fonte canônica e alinhar capítulo + wiki.

### Alertas

- Linha 121: “sinal elétrico de 5\,V para 0\,V” precisa ser auditado eletricamente. O ESP32 S3 opera em 3,3\,V; se o GPIO 4 parte diretamente do ESP32, o texto deveria indicar `3{,}3\,V para 0\,V`, salvo se houver circuito externo de adequação.
- Linha 168: “requisições seriais síncronas de interrupção do buffer UART” é ambíguo, pois UART é assíncrona. Melhor: `requisições seriais recebidas por interrupção no buffer UART`.

### Sugestões de Melhoria

- Incluir uma tabela de “parâmetros físicos efetivamente adotados” no Capítulo 3 ou na wiki, travando: motor, driver, tensão, micropasso, relação de redução, pino de trigger e canal de feedback por junta.

---

## 3.4 Conformidade Científica e Estilo — Nota: 8,0

### Pontos Críticos

1. **Linha 18 — termos avaliativos sem métrica**

Trecho atual:
```latex
poder computacional expressivo [...] recursos ideais
```

Substituição recomendada:
```latex
capacidade de processamento compatível com as rotinas previstas [...] recursos adequados
```

2. **Linha 170 — afirmação absoluta “ruído de fase temporal nulo”**

Trecho atual:
```latex
A geração determinística de passos mecânicos com ruído de fase temporal nulo
```

Substituição recomendada:
```latex
A geração determinística de passos mecânicos com baixo ruído de fase temporal
```

Justificativa: “nulo” é afirmação absoluta e exige comprovação experimental.

3. **Linha 204 — “testes experimentais rigorosos”**

Substituição recomendada:
```latex
testes experimentais controlados
```

### Alertas

- O capítulo é tecnicamente completo, mas alguns parágrafos longos concentram muitas especificações. Linhas 16, 27, 50, 70, 83, 102 e 121 poderiam ser divididas para reduzir carga cognitiva.

### Sugestões de Melhoria

- Trocar “blindar o laço determinístico” (linha 4) por “isolar o laço determinístico”. “Blindar” é compreensível, mas menos acadêmico.
- Evitar “perfeita sincronia” e “tempo desprezível” quando não houver medição no próprio capítulo.

---

## 3.5 Sintaxe e Tipografia LaTeX — Nota: 8,5

### Pontos Críticos

1. **Linhas 22, 36 e demais itens — hífen em títulos de itens**

Trecho atual:
```latex
\textbf{Junta 1 (J1 - Rotação da Base):}
```

Substituição recomendada:
```latex
\textbf{Junta 1 (J1 --- Rotação da Base):}
```

Usar travessão LaTeX `---` para explicações.

2. **Linhas 87 e tabela — unidade “Bytes”**

Trecho atual:
```latex
\caption{Estrutura do Frame Binário Customizado para UART (10 Bytes)}
```

Substituição recomendada:
```latex
\caption{Estrutura do frame binário customizado para UART (10 bytes)}
```

Justificativa: unidade em minúscula e título menos capitalizado.

3. **Linha 75 — subscritos textuais com `\_`**

Trecho atual:
```latex
V_{RX\_ESP32}
```

Substituição recomendada:
```latex
V_{\mathrm{RX,ESP32}}
```

Justificativa: em modo matemático, texto técnico em subscrito fica mais limpo com `\mathrm{}`.

### Alertas

- O arquivo está livre dos vazamentos graves de Markdown observados em revisões anteriores.
- As tabelas compilam, mas ainda usam linhas verticais e `\hline`; para padrão mais profissional, considerar `booktabs` em revisão posterior.

### Sugestões de Melhoria

- Padronizar maiúsculas em termos como `Frame Binário Customizado`, `Byte Posição`, `Timer`. Em texto corrido e tabelas, preferir minúsculas quando não forem nomes próprios.
- Usar `Tabela~\ref{...}` e `Equação~\ref{...}` de modo consistente; o capítulo já usa `Equação~\ref` em vários pontos, mas ainda há `Equações \ref` sem espaço não separável na linha 18.

---

## 4. Checklist de Verificação Executado

- Chaves citadas no Capítulo 3 existem em `TCC_escrito/ref.bib`: **sim**.
- Chaves no `ref.bib` não usadas no Capítulo 3: várias, esperado por se tratar de bibliografia global do TCC.
- Vazamento de Markdown grave (`**`, backticks, `*jitter*`) no Capítulo 3: **não encontrado**.
- Arquivo principal com `\documentclass` para compilação completa: **não localizado nesta auditoria**.
- Divergências técnicas entre Capítulo 3 e wiki: **encontradas** e listadas como pontos críticos.

---

## 5. Conclusão

O Capítulo 3 possui boa estrutura acadêmica e sintaxe LaTeX substancialmente melhorada, mas não deve ser considerado final enquanto houver divergência entre a metodologia e a wiki sobre pino de trigger, INT0/polling, TCA9548A, micropasso e matriz de motores/tensões. A prioridade de correção é decidir qual documento representa o hardware real do EB15 e alinhar todos os artefatos a essa decisão.

