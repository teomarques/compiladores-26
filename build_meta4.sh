#!/usr/bin/env bash
set -euo pipefail

# 1. Entrar na pasta da Meta 4
cd /home/teomarques/Desktop/0_UC/0_2S2A/COMP/praticas/compiladores-26/Meta4/src

# 2. Limpar ficheiros antigos
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
gcc -std=gnu11 -O2 -Wall -Wno-unused -c codegen.c -o codegen.o

# 6. Fazer o link final
gcc -std=gnu11 -O2 -Wall -Wno-unused y.tab.o lex.yy.o ast.o semantic.o codegen.o -o jucompiler -lfl -lm

echo "Meta 4 compilada com sucesso: Meta4/src/jucompiler"
