CC = cc
CFLAGS = -Wall -Wno-unused-function
LEX = flex
TARGET = jucompiler

# Try to find .l file in current directory or src/
SOURCE := $(wildcard jucompiler.l src/jucompiler.l)
ifeq ($(SOURCE),)
$(error Cannot find jucompiler.l in current directory or src/)
endif

all: $(TARGET)

$(TARGET): lex.yy.c
	$(CC) $(CFLAGS) -o $@ $^

lex.yy.c: $(SOURCE)
	$(LEX) $<

clean:
	rm -f $(TARGET) lex.yy.c

rebuild: clean all

.PHONY: all clean rebuild