# Meta 1: Lexical Analysis

## Overview

Meta 1 implements the **lexical analysis** phase of the Java compiler using the `lex` tool. This phase reads the source code character by character and converts it into a stream of tokens.

**Files in this folder:**
- `jucompiler.l` - Lexer specification file (input to lex)
- `README.md` - This file

## What the Lexer Does

The lexer reads Java source code and tokenizes it by:

1. **Pattern Matching** - Uses regular expressions to identify token patterns
2. **Token Generation** - Creates tokens with type and value
3. **Error Handling** - Reports lexical errors (invalid characters, unterminated strings, etc.)
4. **Line/Column Tracking** - Maintains position information for error reporting

## Token Types Recognized

The lexer recognizes the following token categories:

- **Keywords**: class, public, static, void, int, double, boolean, String, return, if, else, while, for, extends, new, this, System, out, println, length, Integer, parseInt
- **Operators**: +, -, *, /, %, ==, !=, <, >, <=, >=, &&, ||, !, =
- **Delimiters**: (, ), {, }, [, ], ;, ,, .
- **Identifiers**: Variable and class names (alphanumeric + underscore)
- **Numbers**: Integers and decimal numbers
- **Strings**: String literals with escape sequence support
- **Comments**: Single-line (//) and multi-line (/* */)
- **Whitespace**: Handled implicitly (not tokenized)

## File Structure

### jucompiler.l

The lexer specification file contains:

1. **Header Section** (C code)
   - Include statements
   - Variable declarations
   - Helper function definitions

2. **Rules Section** (lex patterns)
   - Regular expressions for each token type
   - Associated C code to execute when pattern matches

3. **Auxiliary Functions Section**
   - Helper functions (if needed)

## How to Use

### Compile the Lexer

The lexer is compiled as part of the full project build:

```bash
cd /path/to/compiladores-26
make clean
make
```

This runs:
```bash
lex meta1/jucompiler.l        # Generates lex.yy.c from jucompiler.l
cc -c lex.yy.c               # Compiles the generated C code
```

### Generated Files

When `lex` processes `jucompiler.l`, it generates:

- `lex.yy.c` - C source code implementing the lexer
- `lex.yy.h` - Header file with token definitions (optional)

These are generated in the parent directory (root of project).

## Integration with Parser (Meta 2)

The lexer output feeds into the parser:

1. Lexer generates tokens from source code
2. Parser consumes tokens to build Abstract Syntax Tree (AST)
3. This is handled transparently by yacc (Parser generator)

The communication happens via:
- `yylex()` function - Called by parser to get next token
- `yylval` variable - Holds token value
- `yylineno` variable - Tracks current line number

## Building and Testing

### Full Build
```bash
make clean && make
```

### Test with Sample File
```bash
./jucompiler < testes/Factorial.java
```

## Key Variables and Functions

### yytext
- Holds the matched string from the input
- Use: Capturing identifier names, number values, etc.

### yylval
- Union that holds token value
- Populated by lexer actions
- Read by parser

### yylineno
- Current line number in input
- Automatically maintained by lex
- Used for error reporting

### yylex()
- Main lexer function
- Called by parser to get next token
- Returns token type (integer constant)

## Relationship to Other Metas

```
┌─────────────────────────────────────┐
│     META 1: Lexical Analysis        │
│     (meta1/jucompiler.l)            │
│  Input: Source code (text)           │
│  Output: Token stream                │
└─────────────────┬────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────┐
│     META 2: Syntactic Analysis      │
│     (meta2/*.y /*.h /*.c)           │
│  Input: Token stream                 │
│  Output: Abstract Syntax Tree (AST)  │
└─────────────────┬────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────┐
│    META 3: Semantic Analysis        │
│  Input: AST                          │
│  Output: Type-checked AST + Symbols  │
└─────────────────┬────────────────────┘
                  │
                  ▼
┌─────────────────────────────────────┐
│      META 4: Code Generation        │
│  Input: Type-checked AST             │
│  Output: LLVM IR / Machine code      │
└─────────────────────────────────────┘
```

## Next Steps

1. Review the complete `jucompiler.l` file to understand token definitions
2. Study how tokens are generated and their values captured
3. Understand how line numbers are tracked
4. See how the lexer integrates with the parser in Meta 2
5. Test the full pipeline with sample Java files

---

**Note**: The lexer works in conjunction with the parser (Meta 2). The lexical analysis phase is complete and ready; Meta 2 builds upon this to create the syntactic analysis layer.
