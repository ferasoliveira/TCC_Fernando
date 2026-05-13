---
name: academic-writing
description: Academic writing principles for TCC/thesis using LaTeX. Structure, argumentation, mandatory cohesion connectors, 4-8 line paragraph rule (Topic-Support-Argument-Conclusion). Use when writing or structuring academic chapters.
allowed-tools: Read, Write, Edit
version: 2.0
priority: HIGH
---

# Academic Writing — TCC Specialist (LaTeX)

> "A escrita acadêmica é formal, impessoal, coesa e perfeitamente estruturada."

---

## 🔴 CRITICAL: The Golden Paragraph Rule

**EVERY SINGLE PARAGRAPH MUST FOLLOW THIS STRUCTURE:**

1. **Tamanho:** Entre 4 e 8 linhas. Nunca menos de 4, nunca mais de 8.
2. **Componentes (Nesta Exata Ordem):**
   - **Tópico frasal:** A ideia central do parágrafo, direta e clara.
   - **Sustentação:** A base teórica ou dado que valida o tópico (com citação).
   - **Argumentação:** A sua análise ou conexão do dado com o seu problema de pesquisa.
   - **Conclusão:** O fechamento do raciocínio conectando à próxima ideia.

### Exemplo de Parágrafo Perfeito
> "A robótica de código aberto tem se apresentado como uma alternativa viável para a democratização do ensino tecnológico *(Tópico frasal)*. Segundo Silva (2020), projetos como o braço robótico Thor demonstram que é possível alcançar precisões milimétricas com componentes de baixo custo fabricados via impressão 3D *(Sustentação)*. Desse modo, a aplicação desses conceitos permite que instituições com orçamentos restritos desenvolvam bancadas didáticas avançadas sem depender de equipamentos industriais proprietários *(Argumentação)*. Portanto, a exploração dessa vertente tecnológica não apenas reduz barreiras financeiras, mas também fomenta a inovação pedagógica no ensino de engenharia *(Conclusão)*."

---

## Coesão e Conectivos (OBRIGATÓRIO)

**É IMPRESCINDÍVEL o uso de conectivos de coesão no início de todos os parágrafos** (exceto o primeiro de cada seção). O texto deve ser **fluido e corrido**.

| Tipo de Transição | Conectivos Obrigatórios |
|---|---|
| **Conclusão/Síntese** | Portanto, Sendo assim, Dessa forma, Por conseguinte, Em suma |
| **Adição/Continuação**| Além disso, Ademais, Outrossim, Soma-se a isso, Sob o mesmo ponto de vista |
| **Oposição/Contraste**| No entanto, Contudo, Por outro lado, Em contrapartida, Não obstante |
| **Causa/Explicação**  | Com efeito, Visto que, Haja vista que, Posto que |
| **Tempo/Sequência**   | Primeiramente, Nesse ínterim, Posteriormente, Em seguida |

---

## Formatação e Fluidez (LaTeX)

A escrita do TCC **NÃO deve usar marcação Markdown** para estruturas. A escrita deve focar puramente em texto corrido e comandos **LaTeX**, mantendo a ausência de indentações falsas (espaços extras) usando apenas o padrão de compilação do LaTeX.

### Estrutura de Seções
```latex
\chapter{INTRODUÇÃO}
% Parágrafo 1 (sem conectivo no primeiro parágrafo do capítulo)

% Parágrafo 2 (começando com conectivo: Além disso...)

\section{Contextualização}
% Texto contínuo...

\subsection{Problema de Pesquisa}
% Texto contínuo...
```

### Elementos Gráficos e Matemáticos
**Imagens:**
```latex
\begin{figure}[H]
    \centering
    \includegraphics[width=0.8\textwidth]{caminho/para/imagem.png}
    \caption{Título da imagem.}
    \label{fig:referencia}
    \fonte{Elaborado pelo autor (2026).}
\end{figure}
```

**Tabelas:**
```latex
\begin{table}[H]
    \centering
    \caption{Resultados dos ensaios}
    \label{tab:resultados}
    \begin{tabular}{|c|c|}
        \hline
        Variável & Valor \\
        \hline
    \end{tabular}
    \fonte{Autor (2026).}
\end{table}
```

---

## Estilo e Tom (Português Brasileiro Formal)

| ✅ Obrigatório | ❌ Proibido |
|---|---|
| Uso de voz impessoal ("observa-se que", "foi desenvolvido") | Primeira pessoa ("eu fiz", "nós achamos") |
| Termos precisos em português formal | Gírias, jargões sem explicação, coloquialismos |
| Frases em ordem direta (Sujeito + Verbo + Predicado) | Frases extremamente longas e repletas de vírgulas |
| Respeito absoluto à norma culta brasileira | Erros de concordância verbal ou nominal |
