#!/bin/sh
rm -f jucompiler lex.yy.c y.tab.c y.tab.h

command -v bison >/dev/null 2>&1 || command -v yacc >/dev/null 2>&1 || { echo "Missing yacc/bison (run in WSL or install bison)."; exit 1; }
command -v flex  >/dev/null 2>&1 || command -v lex  >/dev/null 2>&1 || { echo "Missing lex/flex (run in WSL or install flex)."; exit 1; }
command -v cc    >/dev/null 2>&1 || { echo "Missing C compiler (cc)."; exit 1; }

yacc -d -v -t -g --report=all jucompiler.y 2>/dev/null || bison -y -d -v -t -g --report=all jucompiler.y
lex jucompiler.l 2>/dev/null || flex jucompiler.l
cc -o jucompiler lex.yy.c y.tab.c ast.c -Wall -Wno-unused-function -lfl
