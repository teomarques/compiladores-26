#!/usr/bin/env bash
set -euo pipefail

# 1. Entrar na pasta da Meta 3
cd /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/Meta3/src

# 2. Limpar ficheiros antigos (para garantir compilacao limpa)
rm -f *.o y.tab.* lex.yy.* jucompiler

# 3. Gerar o parser (Yacc/Bison)
bison -d -y jucompiler.y

# 4. Gerar o lexer (Lex/Flex)
flex jucompiler.l

# 5. Compilar cada ficheiro C com as flags EXATAS do Mooshak
gcc -std=gnu11 -O2 -Wall -Wno-unused -c y.tab.c -o y.tab.o
gcc -std=gnu11 -O2 -Wall -Wno-unused -c lex.yy.c -o lex.yy.o
gcc -std=gnu11 -O2 -Wall -Wno-unused -c ast.c -o ast.o
gcc -std=gnu11 -O2 -Wall -Wno-unused -c semantic.c -o semantic.o

# 6. Fazer o link final (o Mooshak adiciona -lfl e -lm)
gcc -std=gnu11 -O2 -Wall -Wno-unused y.tab.o lex.yy.o ast.o semantic.o -o jucompiler -lfl -lm

echo "Meta 3 compilada com sucesso: Meta3/src/jucompiler"
