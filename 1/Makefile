CC = cc
CFLAGS = -Wall -Wno-unused-function

jucompiler: lex.yy.o
	$(CC) -o jucompiler lex.yy.o $(CFLAGS) -lfl

lex.yy.c: src/jucompiler.l
	lex src/jucompiler.l

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

clean:
	rm -f jucompiler lex.yy.* *.o

rebuild: clean jucompiler

.PHONY: clean rebuild
