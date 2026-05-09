CC = cc
CFLAGS = -Wall -Wno-unused-function -g

all: jucompiler

lex.yy.c: jucompiler.l y.tab.h
	lex jucompiler.l

y.tab.c y.tab.h: jucompiler.y
	yacc -d jucompiler.y

jucompiler: y.tab.c lex.yy.c ast.c semantic.c codegen.c
	$(CC) $(CFLAGS) -o jucompiler y.tab.c lex.yy.c ast.c semantic.c codegen.c -lfl

clean:
	rm -f jucompiler lex.yy.c y.tab.c y.tab.h y.output *.o

.PHONY: all clean
