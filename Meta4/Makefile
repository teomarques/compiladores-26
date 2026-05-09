CC = cc
CFLAGS = -Wall -Wno-unused-function -Isrc

all: jucompiler

lex.yy.c: src/jucompiler.l
	cd src && lex jucompiler.l && mv lex.yy.c .. && cd ..

y.tab.c y.tab.h: src/jucompiler.y
	cd src && yacc -d jucompiler.y && mv y.tab.c y.tab.h .. && cd ..

# Source files - note semantics.c (with 's')
SOURCES = lex.yy.c y.tab.c \
          src/ast.c \
          src/semantics.c \
          src/symtab.c \
          src/codegen.c

jucompiler: $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ -lfl

clean:
	rm -f jucompiler lex.yy.c y.tab.c y.tab.h y.output y.gv *.o

.PHONY: all test clean