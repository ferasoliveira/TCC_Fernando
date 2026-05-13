# Compilação (Raw ➔ Wiki)

Aja como um Arquiteto de Conhecimento. Analise os novos arquivos na pasta `/raw`. Sua tarefa é destilar essa informação para a `/wiki`:

1. Identifique se o conteúdo pertence a uma página existente no `index.md` ou se exige uma nova.
2. Extraia apenas dados técnicos acionáveis (parâmetros, limites, pinagens, exemplos de uso).
3. Crie ou atualize os arquivos Markdown na `/wiki` usando links [[página]] para conectar com conceitos já existentes.
4. Se houver algo irrelevante ou redundante no arquivo bruto, ignore-o.
5. Remova prints de logs extensos ou ruídos, mantendo apenas a essência técnica.

Ao terminar, me dê um resumo das novas páginas criadas ou arquivos atualizados.

---

# Health Check (Manutenção)

Aja como um Revisor Técnico de Sistemas. Faça um Health Check na nossa `/wiki` e no `log.md`:

1. Links Quebrados: Verifique se todos os links [[página]] apontam para arquivos que realmente existem na pasta.
2. Contradições: Compare as entradas mais recentes do `log.md` com as especificações na Wiki. Existe algo na Wiki que ficou obsoleto devido a uma decisão registrada no Log?
3. Furos de Conhecimento: Identifique áreas mencionadas no Log ou no código que ainda não foram documentadas na Wiki.
4. Sugestão de Arquivamento: Indique se algum arquivo ou seção pode ser movido para uma pasta `/archive` por não ser mais útil para a fase atual do projeto.

---

# Sincronização de Memória (Durante o Projeto)

Acabamos de realizar uma mudança/descoberta: [DESCREVA AQUI, ex: 'Trocamos o sensor de corrente pelo modelo Y'].

Execute o seguinte protocolo de sincronização:
1. Atualize o `log.md` com o registro desta mudança, o motivo e o impacto imediato.
2. Localize a página técnica correspondente na `/wiki` (ou crie uma nova) e atualize as especificações para refletir a realidade atual.
3. Verifique no `index.md` se essa mudança afeta outras dependências ou módulos e atualize-os conforme necessário.
4. Responda com 'Sincronização Concluída' e um breve resumo do que mudou na base de conhecimento.