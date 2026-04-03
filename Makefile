CC = cc
CFLAGS = -Wall -Wno-unused-function

all: jucompiler

jucompiler: y.tab.c lex.yy.c ast.c
	$(CC) -o jucompiler lex.yy.c y.tab.c ast.c $(CFLAGS)

y.tab.c y.tab.h: jucompiler.y
	yacc -d -v -t -g --report=all jucompiler.y

lex.yy.c: jucompiler.l y.tab.h
	lex jucompiler.l

clean:
	rm -f jucompiler lex.yy.c y.tab.c y.tab.h y.output