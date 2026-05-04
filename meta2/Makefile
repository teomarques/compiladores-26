CC = cc
CFLAGS = -Wall -Wno-unused-function

OBJS = lex.yy.o y.tab.o ast.o

jucompiler: $(OBJS)
	$(CC) -o jucompiler $(OBJS) $(CFLAGS) -lfl

y.tab.c y.tab.h: src/jucompiler.y src/ast.h
	lex src/jucompiler.l && yacc -d src/jucompiler.y

y.tab.o: y.tab.c y.tab.h src/ast.h
	$(CC) $(CFLAGS) -c y.tab.c

lex.yy.c: src/jucompiler.l
	lex src/jucompiler.l

lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c

ast.o: src/ast.c src/ast.h
	$(CC) $(CFLAGS) -c src/ast.c

clean:
	rm -f jucompiler lex.yy.* y.tab.* y.output y.gv *.o

rebuild: clean jucompiler

.PHONY: clean rebuild
