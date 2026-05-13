# AI Project Protocol: Claude Code

## 1. Contexto de Conhecimento e Estado
- **Wiki Central (`./wiki/`):** Base de conhecimento destilada. Contém arquivos Markdown estruturados e interconectados.
- **Log de Atividade (`./log.md`):** Registro cronológico do progresso, decisões rápidas e estado atual.

## 2. Instruções de Leitura (Mandatório)
- Antes de qualquer tarefa, leia o `./log.md` para situar-se no cronograma e a pasta `./wiki/` para entender as definições técnicas vigentes.

## 3. Instruções de Manutenção
- **Pós-Tarefa:** Atualize o `./log.md` com o que foi realizado e o que resta fazer.
- **Sincronização:** Se uma implementação alterar uma definição da Wiki, atualize o arquivo correspondente imediatamente.

## 4. Protocolo de Compilação (Raw → Wiki)
- **Extração:** Ao processar novos arquivos em `./raw/`, identifique conceitos-chave e verifique se já existem na `./wiki/`.
- **Atomicidade:** Crie páginas focadas em tópicos únicos (ex: um sensor, um algoritmo, uma regra de negócio).
- **Interconexão:** Use links no estilo `[[nome-da-pagina]]` para conectar conceitos relacionados.
- **Destilação:** Remova ruídos, logs desnecessários e redundâncias dos arquivos brutos ao movê-los para a Wiki.