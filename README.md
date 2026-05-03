# jucompiler — Compiladores 2025/26

Compilador para um subconjunto da linguagem Java, desenvolvido no âmbito da unidade curricular de Compiladores.

## Autores

| Nome | Nº de Aluno |
|------|-------------|
| Simão Tomás Botas Carvalho | 2021223055 |
| Teodoro Marques | 2023211717 |

## Estrutura do projeto

Ficheiros principais na raiz:

```
jucompiler.l    # Analisador léxico (Flex)
jucompiler.y    # Gramática e ações semânticas / main (Yacc)
ast.h, ast.c    # Definição da AST (nós, categorias, filhos)
Makefile        # Gera lex.yy.c / y.tab.* e liga o binário jucompiler
enunciadotxt.txt  # Enunciado / notas (texto)
```

Testes organizados por meta:

```
testes/
  test.sh       # Corre jucompiler com uma flag e compara com ficheiros .out
  meta1/        # Casos .java e respetivos .out (léxico)
  meta2/        # Idem (sintaxe / AST consoante o enunciado)
  meta3/        # Idem (inclui cenários para fases seguintes do projeto)
```

> Nota: existe também uma pasta `New folder/` com cópias de ficheiros fonte, habitualmente para experiências locais; o código usado pelo `Makefile` é o da **raiz** do repositório.

## Componentes

### Análise léxica (Meta 1)

O lexer é gerado com **Flex** e reconhece palavras-chave, reservadas, operadores, delimitadores, literais, identificadores e construções especiais (por exemplo `.length`, `System.out.print`, `Integer.parseInt`).

**Erros tratados:** caracteres ilegais, escapes inválidos e strings não terminadas, comentários de bloco não terminados.

### Análise sintática e AST (Meta 2)

O parser é gerado com **Yacc** a partir de `jucompiler.y`. A árvore sintática abstracta é construída com as estruturas em `ast.h` / `ast.c` e pode ser impressa com a flag `-t` quando o parse conclui sem erros sintáticos.

A gramática inclui precedência de operadores, resolução do *dangling else* e recuperação de erros sintáticos em alguns pontos.

## Compilação e execução

```bash
make          # gera e compila jucompiler
make run      # executa ./jucompiler (lê de stdin)
make clean    # remove binário e artefactos gerados
```

O compilador **lê sempre do stdin**. Exemplos:

```bash
./jucompiler -l  < testes/meta1/Factorial.java
./jucompiler -t  < testes/meta2/Factorial.java
echo 'int x = 42;' | ./jucompiler -l
```

### Flags de execução

| Flag | Comportamento |
|------|----------------|
| `-l` | Percorre apenas o lexer e imprime **todos** os tokens reconhecidos. |
| `-e1` | Percorre apenas o lexer; **não** lista tokens, apenas mensagens de **erro léxico**. |
| `-e2` | *(valor por omissão se não passar outra flag)* — análise **sintática** completa; mensagens de erro léxico/sintático conforme implementação. |
| `-t` | Após o parse, se não houver erros sintáticos, imprime a **AST**. |

## Testes automáticos

A partir da pasta `testes/`:

```bash
cd testes
chmod +x test.sh   # se necessário
./test.sh -l meta1
./test.sh -e2 meta2
./test.sh -t meta2
```

O script compara a saída do `../jucompiler` com o ficheiro `.out` homólogo a cada `.java` e reporta diferenças.

## Requisitos

- **Flex** (gerador do analisador léxico)
- **Yacc** (ou implementação compatível, por exemplo a fornecida pelo sistema / Bison em modo yacc)
- **GCC** e **libfl** (ligação `-lfl` no `Makefile`)
