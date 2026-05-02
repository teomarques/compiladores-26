# jucompiler — Compiladores 2025/26

Compilador para a linguagem **Juc**, um subconjunto do Java (Java SE 9), desenvolvido no âmbito da unidade curricular de Compiladores.

## Autores

| Nome | Nº de Aluno |
|------|-------------|
| Simão Tomás Botas Carvalho | 2021223055 |
| Teodoro Marques | 2023211717 |

## Estado Atual

| Meta | Descrição | Estado | Pontuação Mooshak |
|------|-----------|--------|-------------------|
| **Meta 1** | Análise Léxica | Concluída | — |
| **Meta 2** | Análise Sintática + AST | Concluída | **242 / 250** |
| Meta 3 | Análise Semântica | Por iniciar | — |
| Meta 4 | Geração de Código | Por iniciar | — |

## Estrutura do Projeto

```
jucompiler.l       # Especificação léxica (Flex)
jucompiler.y       # Gramática (Yacc/Bison) + main() + AST printer
ast.c / ast.h      # Estruturas e construtores da AST
Makefile           # Compilação (yacc -d → lex → cc)
Factorial.java     # Programa de exemplo (do enunciado)
input.txt          # Ficheiro de teste (Factorial)
enunciadotxt.txt   # Texto do enunciado
tests/             # 25 casos de teste próprios (e01.java … e25.java)
prof-results/      # Resultados oficiais Mooshak por meta
```

## Compilação e Execução

```bash
# Compilar (gera y.tab.c, y.tab.h, lex.yy.c e o binário jucompiler)
make

# Limpar ficheiros gerados
make clean

# Executar (lê de stdin)
./jucompiler -t < Factorial.java
```

### Flags de execução

| Flag | Modo | Descrição |
|------|------|-----------|
| `-l`  | LEX   | Imprime todos os tokens reconhecidos |
| `-e1` | E1    | Imprime apenas erros léxicos (Meta 1) |
| `-e2` | E2    | Imprime erros léxicos e sintáticos (modo por omissão) |
| `-t`  | TREE  | Constrói e imprime a árvore sintática (AST) — só é impressa se não houver erros |

```bash
# Exemplos
./jucompiler -l  < Factorial.java
./jucompiler -e2 < tests/e01.java
./jucompiler -t  < input.txt
```

---

## Meta 1 — Análise Léxica

Analisador léxico gerado com **Flex**. Reconhece todos os elementos da linguagem Juc.

### Tokens reconhecidos

| Categoria | Exemplos |
|-----------|----------|
| **Palavras-chave** | `boolean`, `class`, `double`, `else`, `if`, `int`, `public`, `return`, `static`, `String`, `void`, `while` |
| **Construções pré-definidas** | `.length`, `System.out.print`, `Integer.parseInt` |
| **Palavras reservadas** | `++`, `--`, `null`, `Integer`, `System`, `abstract`, `assert`, `break`, `byte`, `case`, `catch`, `char`, `const`, `continue`, `default`, `do`, `enum`, `extends`, `final`, `finally`, `float`, `for`, `goto`, `implements`, `import`, `instanceof`, `interface`, `long`, `native`, `new`, `package`, `private`, `protected`, `short`, `strictfp`, `super`, `switch`, `synchronized`, `this`, `throw`, `throws`, `transient`, `try`, `volatile` |
| **Operadores** | `==`, `!=`, `>=`, `<=`, `<<`, `>>`, `->`, `&&`, `\|\|`, `>`, `<`, `=`, `!`, `+`, `-`, `*`, `/`, `%`, `^` |
| **Delimitadores** | `,`, `;`, `(`, `)`, `{`, `}`, `[`, `]` |
| **Literais** | `NATURAL`, `DECIMAL`, `STRLIT`, `BOOLLIT` (`true`/`false`) |
| **Identificadores** | sequências de letras, dígitos, `_` e `$` |

### Tratamento de erros léxicos

- Caracteres ilegais
- Sequências de escape inválidas em strings
- Strings não terminadas (com EOF ou newline no meio)
- Comentários de bloco não terminados

Cada erro é reportado no formato `Line L, col C: <mensagem>`, com a coluna a apontar para o início do lexema problemático.

---

## Meta 2 — Análise Sintática + AST

Analisador sintático gerado com **Yacc/Bison**, com construção da AST durante o parsing.

### Funcionalidades

- Gramática completa para a linguagem Juc (declaração de classe, atributos `public static`, métodos, blocos, expressões, controlo de fluxo).
- Resolução do *dangling-else* via precedências (`%nonassoc IFX` < `ELSE`).
- Tabela de precedências completa para operadores (atribuição, lógicos, relacionais, deslocamento, aritméticos, unários).
- Recuperação de erros através de regras com `error SEMICOLON` em pontos estratégicos (ao nível da classe e dos `field_decl`).
- Construção da AST conforme as categorias do enunciado (`Program`, `FieldDecl`, `MethodDecl`, `MethodHeader`, `MethodBody`, `Block`, `If`, `While`, `Return`, `Call`, `Print`, `ParseArgs`, `Assign`, operadores binários e unários, literais, etc.).
- Impressão da AST via `printast()` no formato exigido pelo Mooshak (apenas quando o código está livre de erros sintáticos).

### Tratamento de erros sintáticos

- Mensagens no formato `Line L, col C: syntax error: <token>`.
- Recuperação até ao próximo `;` para permitir reportar múltiplos erros numa só execução.
- Combinação correta com erros léxicos: se a análise abortar antes do EOF, o restante do ficheiro é consumido pelo lexer para que **todos** os erros léxicos sejam reportados.

### Resultados de testes (Mooshak — Meta 2)

```
PASS = 36
FAIL = 0
TOTAL = 36
Pontuação: 242 / 250
```

Os resultados detalhados (ficheiros `.got` e `.diff` por teste) estão em `prof-results/meta2/`.

---

## Requisitos

- **Flex** (gerador de analisadores léxicos)
- **Yacc** ou **Bison** (gerador de parsers)
- **GCC** (compilador C)
