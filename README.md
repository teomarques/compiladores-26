# jucompiler — Compiladores 2025/26

Compilador para um subconjunto da linguagem Java, desenvolvido no âmbito da unidade curricular de Compiladores.

## Autores

| Nome | Nº de Aluno |
|------|-------------|
| Simão Tomás Botas Carvalho | 2021223055 |
| Teodoro Marques | 2023211717 |

## Estrutura do Projeto

```
jucompiler.l    # Especificação léxica (Flex)
Makefile        # Compilação e execução
Factorial.java  # Ficheiro de exemplo (Java)
input.txt       # Ficheiro de teste
```

## Meta 1 — Análise Léxica

O analisador léxico é gerado com **Flex** e reconhece os seguintes elementos da linguagem:

### Tokens reconhecidos

| Categoria | Exemplos |
|-----------|----------|
| **Palavras-chave** | `boolean`, `class`, `double`, `else`, `if`, `int`, `public`, `return`, `static`, `String`, `void`, `while` |
| **Palavras reservadas** | `++`, `--`, `null`, `Integer`, `System`, `abstract`, `assert`, `continue`, `default`, `do`, `goto`, `package`, `private`, `switch`, `synchronized` |
| **Operadores** | `==`, `!=`, `>=`, `<=`, `<<`, `>>`, `->`, `&&`, `\|\|`, `>`, `<`, `=`, `!`, `+`, `-`, `*`, `/`, `%`, `^` |
| **Delimitadores** | `,`, `;`, `(`, `)`, `{`, `}`, `[`, `]` |
| **Literais** | `NATURAL`, `DECIMAL`, `STRLIT`, `BOOLIT` (`true`/`false`) |
| **Identificadores** | sequências de letras, dígitos, `_` e `$` |
| **Especiais** | `.length`, `System.out.print`, `Integer.parseInt` |

### Tratamento de erros

- Caracteres ilegais
- Sequências de escape inválidas em strings
- Strings não terminadas
- Comentários de bloco não terminados

## Compilação e Execução

```bash
# Compilar
make

# Executar (lê de stdin)
make run

# Limpar ficheiros gerados
make clean
```

### Flags de execução

| Flag | Descrição |
|------|-----------|
| `-l` | Imprime todos os tokens reconhecidos |
| `-e1` | Imprime apenas erros léxicos (modo padrão) |

```bash
# Exemplos de utilização
./jucompiler -l < Factorial.java
./jucompiler -e1 < input.txt
echo 'int x = 42;' | ./jucompiler -l
```

## Requisitos

- **Flex** (gerador de analisadores léxicos)
- **GCC** (compilador C)
