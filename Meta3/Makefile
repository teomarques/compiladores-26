CC = cc
CFLAGS = -Wall -Wno-unused-function

OBJS = lex.yy.o y.tab.o ast.o semantic.o symbol_table.o type_checker.o error_handler.o

jucompiler: $(OBJS)
	$(CC) -o jucompiler $(OBJS) $(CFLAGS) -lfl

y.tab.c y.tab.h: src/jucompiler.y src/ast.h src/semantic.h src/symbol_table.h src/type_checker.h src/error_handler.h
	lex src/jucompiler.l && yacc -d src/jucompiler.y

y.tab.o: y.tab.c y.tab.h src/ast.h src/semantic.h src/symbol_table.h src/type_checker.h src/error_handler.h
	$(CC) $(CFLAGS) -c y.tab.c

lex.yy.c: src/jucompiler.l
	lex src/jucompiler.l

lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c

ast.o: src/ast.c src/ast.h
	$(CC) $(CFLAGS) -c src/ast.c

semantic.o: src/semantic.c src/semantic.h src/ast.h
	$(CC) $(CFLAGS) -c src/semantic.c

symbol_table.o: src/symbol_table.c src/symbol_table.h src/semantic.h src/ast.h
	$(CC) $(CFLAGS) -c src/symbol_table.c

type_checker.o: src/type_checker.c src/type_checker.h src/semantic.h src/ast.h
	$(CC) $(CFLAGS) -c src/type_checker.c

error_handler.o: src/error_handler.c src/error_handler.h src/semantic.h
	$(CC) $(CFLAGS) -c src/error_handler.c

clean:
	rm -f jucompiler lex.yy.* y.tab.* y.output y.gv *.o

rebuild: clean jucompiler

.PHONY: clean rebuild
