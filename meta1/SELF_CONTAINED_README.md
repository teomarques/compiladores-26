# Meta 1: Lexical Analysis (Self-Contained)

This folder contains everything needed to build and run the lexical analyzer for Meta 1.

## Contents

- `jucompiler.l` - Lexer specification
- `Makefile` - Build configuration
- `README.md` - Documentation (from parent)
- `SELF_CONTAINED_README.md` - This file

## How to Use

### Build
```bash
cd meta1
make clean
make
```

### Run
```bash
./jucompiler < test_input.java
```

### Test
```bash
echo "int x = 5;" | ./jucompiler
```

## What This Meta Does

The lexical analyzer (lexer) tokenizes Java source code into a stream of tokens.

- Reads characters from input
- Recognizes patterns (keywords, identifiers, numbers, operators, etc.)
- Generates tokens with type information
- Tracks line and column numbers for error reporting

## Output

When run, jucompiler in Meta 1 mode outputs token information (if -l flag used).

## Submitting to Mooshak

To submit Meta 1 to Mooshak:

```bash
# Copy only the necessary files
cd meta1
cp jucompiler.l <mooshak_submission_dir>/
cp Makefile <mooshak_submission_dir>/
# Include ast.h if needed by your implementation
```

## Files Explained

- **jucompiler.l**: Lexical analyzer specification in lex format
  - Defines token patterns as regular expressions
  - Contains action code for each pattern
  - Handles comments, whitespace, error cases

- **Makefile**: Build rules for the lexer
  - Runs `lex` to generate lex.yy.c from jucompiler.l
  - Compiles the generated C code
  - Links to produce executable

## Status

✅ Meta 1 is complete and self-contained
All code needed to build the lexer is in this folder
