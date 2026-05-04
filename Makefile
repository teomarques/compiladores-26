# Compiler Configuration
CC = gcc
CFLAGS = -Wall -Wno-unused-function -std=c99

# ============================================================
# META 1 & META 2: Lexer and Parser (existing)
# ============================================================

# Lexer and Parser objects
LEXER = lex.yy.o
PARSER = y.tab.o

# AST objects
AST = ast.o

# ============================================================
# META 3: Semantic Analyzer
# ============================================================

# Meta3 Source files
META3_SRC = meta3/semantic.c meta3/symbol_table.c meta3/type_checker.c meta3/error_handler.c
META3_OBJ = meta3/semantic.o meta3/symbol_table.o meta3/type_checker.o meta3/error_handler.o
META3_HEADERS = meta3/semantic.h meta3/symbol_table.h meta3/type_checker.h meta3/error_handler.h

# All objects
ALL_OBJ = $(LEXER) $(PARSER) $(AST) $(META3_OBJ)

# ============================================================
# BUILD TARGETS
# ============================================================

all: jucompiler

jucompiler: $(ALL_OBJ)
	$(CC) $(CFLAGS) -o jucompiler $(ALL_OBJ) -lfl

# ============================================================
# META 1 & 2: Lexer and Parser
# ============================================================

y.tab.c y.tab.h: jucompiler.y ast.h $(META3_HEADERS)
	yacc -d jucompiler.y

y.tab.o: y.tab.c y.tab.h ast.h $(META3_HEADERS)
	$(CC) $(CFLAGS) -c y.tab.c

lex.yy.c: jucompiler.l
	lex jucompiler.l

lex.yy.o: lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

# ============================================================
# META 3: Semantic Analyzer
# ============================================================

meta3/semantic.o: meta3/semantic.c meta3/semantic.h ast.h
	$(CC) $(CFLAGS) -c meta3/semantic.c -o meta3/semantic.o

meta3/symbol_table.o: meta3/symbol_table.c meta3/symbol_table.h meta3/semantic.h ast.h
	$(CC) $(CFLAGS) -c meta3/symbol_table.c -o meta3/symbol_table.o

meta3/type_checker.o: meta3/type_checker.c meta3/type_checker.h meta3/semantic.h ast.h
	$(CC) $(CFLAGS) -c meta3/type_checker.c -o meta3/type_checker.o

meta3/error_handler.o: meta3/error_handler.c meta3/error_handler.h meta3/semantic.h
	$(CC) $(CFLAGS) -c meta3/error_handler.c -o meta3/error_handler.o

# ============================================================
# UTILITY TARGETS
# ============================================================

run: all
	./jucompiler

clean:
	rm -f jucompiler lex.yy.* y.tab.* y.output y.gv *.o
	rm -f meta3/*.o

rebuild: clean all

# ============================================================
# PHONY TARGETS
# ============================================================

.PHONY: all run clean rebuild
