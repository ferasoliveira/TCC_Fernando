---
name: abnt-formatting
description: ABNT formatting rules strictly adhered to docs/Normas ABNT. Covers the difference between Direct (literal) and Indirect (free interpretation) citations using LaTeX abntex2 commands.
allowed-tools: Read, Write, Edit
version: 2.0
priority: HIGH
---

# ABNT Formatting — Citações e Referências (LaTeX + abntex2)

> Baseado nos documentos normativos locais: `docs/Normas ABNT/citacoes_modelos_abnt.pdf` e `referências_modelos_abnt.pdf`.

---

## 🔴 Diferença Fundamental: Tipos de Citação

A escrita acadêmica deste TCC se baseia em dois tipos de citação. O uso correto delas e de seu pacote LaTeX correspondente (`abntex2cite` ou `cite`) é vital.

### 1. Citação Direta (Transcrição Textual)
É a cópia exata, palavra por palavra, de parte da obra do autor consultado.

**Regras ABNT:**
- **Até 3 linhas:** Mantém-se no corpo do texto principal, obrigatoriamente entre aspas duplas (`"..."`).
- **Mais de 3 linhas:** Deve ter recuo de 4cm da margem esquerda, fonte menor, espaçamento simples e NENHUMA aspa. No LaTeX, usa-se o ambiente `citacao`.
- **Obrigatório:** Em citações diretas, informar a **página** de onde o trecho foi extraído, além da data e do autor.

**Comportamento LaTeX:**
*Exemplo no texto (curta):*
Segundo Silva \citeonline[p. 45]{silva2020}, ``o protótipo falhou''.

*Exemplo recuado (longa):*
```latex
\begin{citacao}
Este é o texto exato do autor copiado integralmente. O LaTeX e o abntex se encarregarão de colocar a fonte tamanho 10 e fazer o recuo de 4cm. A citação vem no final. \cite[p. 112]{siciliano2009}
\end{citacao}
```

### 2. Citação Indireta (Transcrição Livre)
Texto baseado na obra do autor consultado, mas escrito com as **suas próprias palavras** (paráfrase), mantendo o sentido original.

**Regras ABNT:**
- Não usa aspas.
- Não há recuo especial.
- Opcional: uso de página (embora não obrigatório, costuma ser encorajado pelo ABNT).

**Comportamento LaTeX:**
*Citação implícita (no final da frase):*
A robótica de baixo custo barateou a prototipagem rápida nas universidades \cite{silva2020}.

*Citação explícita (parte do texto):*
De acordo com Siciliano \citeonline{siciliano2009}, os manipuladores paralelos são mais rápidos.

---

## Comandos LaTeX para ABNT

O uso do pacote `abntex2cite` padroniza tudo. Ao escrever capítulos, o escritor DEVE usar APENAS estas macros:

| Comando LaTeX | Resultado gerado na compilação PDF | Quando usar |
|---|---|---|
| `\cite{chave}` | (AUTOR, ano) | Final de citação indireta/direta fora da frase (Maiúscula) |
| `\citeonline{chave}` | Autor (ano) | Autor fazendo parte ativa do seu texto (Minúscula) |
| `\cite[p. 15]{chave}` | (AUTOR, ano, p. 15) | Citação direta especificando a página |
| `\apudonline{chave1}{chave2}` | Autor1 (ano *apud* AUTOR2, ano) | Uma obra citada dentro de outra obra original indisponível |

---

## Regras de Referência Bibliográfica (Ref: NBR 6023)

No LaTeX, as referências não são escritas hardcoded nos capítulos. Elas vão em um arquivo `.bib` (BibTeX).

O padrão BibTeX aceito deve seguir classes abntex2:

```bibtex
@book{siciliano2009,
    author    = "Bruno Siciliano and Lorenzo Sciavicco",
    title     = "Robotics: modelling, planning and control",
    edition   = "2",
    address   = "London",
    publisher = "Springer",
    year      = "2009"
}

@article{silva2020,
    author  = "João Silva",
    title   = "Robótica de baixo custo",
    journal = "Revista Brasileira de Robótica",
    volume  = "10",
    number  = "2",
    pages   = "100-115",
    year    = "2020"
}
```

> **Regra de Ouro do TCC:** Cada chamada via `\cite{}` ou `\citeonline{}` DEVE existir no banco bibliográfico oficial do projeto, mapeado no arquivo `docs/referencias/fontes.bib` ou correspondente.
