all:
	flex jucompiler.l
	cc lex.yy.c -o jucompiler

run: all
	./jucompiler

clean:
	rm -f jucompiler lex.yy.c

.PHONY: all run clean
