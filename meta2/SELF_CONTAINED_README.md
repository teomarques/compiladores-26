# Meta 2: Syntactic Analysis (Self-Contained)

This folder contains everything needed to build and run the parser for Meta 2.

## Contents

- `jucompiler.l` - Lexer specification (from Meta 1)
- `jucompiler.y` - Parser specification
- `ast.h` - AST node definitions
- `ast.c` - AST implementation
- `Makefile` - Build configuration
- `README.md` - Documentation (from parent)
- `SELF_CONTAINED_README.md` - This file

## How to Use

### Build
```bash
cd meta2
make clean
make
```

### Run
```bash
./jucompiler < test_input.java
./jucompiler -t < test_input.java      # Syntax check only
./jucompiler -t -s < test_input.java   # Syntax check + AST output
```

### Test
```bash
echo "class Test { public static void main(String[] args) {} }" | ./jucompiler -t
```

## What This Meta Does

The parser builds an Abstract Syntax Tree (AST) from a stream of tokens.

- Takes tokens from lexer (Meta 1)
- Applies grammar rules to validate syntax
- Builds hierarchical AST representing program structure
- Reports syntax errors with line/column information

## Output

When run with `-t -s` flags, outputs the AST in tree format:

```
Program
..ClassName
..MethodDecl
...MethodHeader
...MethodBody
...etc
```

## Submitting to Mooshak

To submit Meta 2 to Mooshak:

```bash
cd meta2
# Copy files
cp jucompiler.l <mooshak_submission_dir>/
cp jucompiler.y <mooshak_submission_dir>/
cp ast.h <mooshak_submission_dir>/
cp ast.c <mooshak_submission_dir>/
cp Makefile <mooshak_submission_dir>/
```

## Files Explained

- **jucompiler.l**: Lexer specification (from Meta 1)
  - Token patterns and generation
  - Line/column tracking

- **jucompiler.y**: Parser specification in yacc format
  - Grammar rules for Java subset
  - Shift/reduce action code
  - AST node creation
  - Operator precedence declarations

- **ast.h**: AST definitions
  - Node category enumeration (N_Program, N_MethodDecl, etc.)
  - Node structure with children and siblings
  - Function declarations for tree manipulation

- **ast.c**: AST implementation
  - Node creation functions
  - Tree linking functions
  - Tree printing functions

- **Makefile**: Build rules
  - Runs `lex` to generate lexer from jucompiler.l
  - Runs `yacc` to generate parser from jucompiler.y
  - Compiles all objects
  - Links to produce executable

## Status

✅ Meta 2 is complete and self-contained
All code needed to build the parser and AST is in this folder
Independent of Meta 3
