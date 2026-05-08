# Meta 3: Semantic Analysis (Self-Contained)

This folder contains everything needed to build and run the semantic analyzer for Meta 3.

## Contents

- `jucompiler.l` - Lexer specification (from Meta 1)
- `jucompiler.y` - Parser specification (from Meta 2, updated for Meta 3)
- `ast.h` - AST definitions (from Meta 2)
- `ast.c` - AST implementation (from Meta 2)
- `semantic.h` - Semantic analyzer definitions
- `semantic.c` - Type system and symbol table structures
- `symbol_table.h` - Phase 1 declarations
- `symbol_table.c` - Phase 1 implementation (symbol table building)
- `type_checker.h` - Phase 2 declarations
- `type_checker.c` - Phase 2 implementation (type checking, stub)
- `error_handler.h` - Phase 3 declarations
- `error_handler.c` - Phase 3 implementation (error handling, stub)
- `Makefile` - Build configuration
- `SELF_CONTAINED_README.md` - This file

## How to Use

### Build
```bash
cd meta3
make clean
make
```

### Run
```bash
./jucompiler < test_input.java                  # Full semantic analysis
./jucompiler -t < test_input.java               # Syntax check only
./jucompiler -s < test_input.java               # With symbol tables
./jucompiler -t -s < test_input.java            # Syntax + AST
```

### Test
```bash
./jucompiler -s < Factorial.java
```

## What This Meta Does

The semantic analyzer performs semantic analysis on the AST from Meta 2.

**Phase 1: Symbol Table Building**
- Extracts all declarations from AST
- Builds class symbol table with fields and methods
- Builds method symbol tables with parameters and local variables
- Maintains declaration order
- Prints symbol tables when requested

**Phase 2: Type Checking (Scaffolding Ready)**
- Infers expression types
- Validates operators with type rules
- Annotates AST nodes with type information
- Validates method calls

**Phase 3: Error Handling (Scaffolding Ready)**
- Specification-compliant error reporting
- Error message generation
- Error recovery and continuation

## Output

When run with `-s` flag, outputs symbol tables:

```
===== Class ClassName Symbol Table =====
methodName ( paramType ) returnType
...

===== Method methodName ( paramTypes ) Symbol Table =====
return      returnType
paramName   paramType   param
localVar    varType
```

## Submitting to Mooshak

To submit Meta 3 to Mooshak:

```bash
cd meta3
# Copy all files
cp jucompiler.l <mooshak_submission_dir>/
cp jucompiler.y <mooshak_submission_dir>/
cp ast.h <mooshak_submission_dir>/
cp ast.c <mooshak_submission_dir>/
cp semantic.h <mooshak_submission_dir>/
cp semantic.c <mooshak_submission_dir>/
cp symbol_table.h <mooshak_submission_dir>/
cp symbol_table.c <mooshak_submission_dir>/
cp type_checker.h <mooshak_submission_dir>/
cp type_checker.c <mooshak_submission_dir>/
cp error_handler.h <mooshak_submission_dir>/
cp error_handler.c <mooshak_submission_dir>/
cp Makefile <mooshak_submission_dir>/
```

## Files Explained

### From Meta 1 & 2 (Included for completeness)
- **jucompiler.l**: Lexer specification
- **jucompiler.y**: Parser specification (updated with Phase 1 integration)
- **ast.h/c**: AST structures

### Meta 3 Core
- **semantic.h/c**: 
  - Type system enumeration (TYPE_INT, TYPE_DOUBLE, etc.)
  - Symbol table structures (symbol_t, method_table_t, class_table_t)
  - Basic symbol table management functions
  - Symbol table printing

- **symbol_table.h/c** (Phase 1):
  - AST traversal functions
  - Type extraction from AST nodes
  - build_class_symbol_table() - Extracts fields and methods
  - build_method_symbol_tables() - Extracts parameters and locals
  - Helper functions for duplicate detection

- **type_checker.h/c** (Phase 2):
  - Expression type checking skeleton
  - Operator validation framework
  - AST annotation with type information
  - Method call validation

- **error_handler.h/c** (Phase 3):
  - Error message templates
  - Error reporting functions
  - Error tracking

### Build System
- **Makefile**: 
  - Compiles all phases
  - Proper dependency ordering
  - Links all modules

## Phase 1 Status

✅ Phase 1 (Symbol Table Building) is **FULLY IMPLEMENTED**

What works:
- Extracts all field declarations from AST
- Extracts all method declarations with return types
- Extracts parameters from each method
- Extracts local variables from method bodies
- Maintains declaration order using linked lists
- Prints formatted symbol tables

## Phase 2 & 3 Status

🔄 Phase 2 (Type Checking) - Scaffolding ready for implementation
🔄 Phase 3 (Error Handling) - Scaffolding ready for implementation

## How to Test Phase 1

```bash
cd meta3
make clean
make
./jucompiler -s < ../testes/Factorial.java
```

Expected output: Class and method symbol tables

## Integration with Previous Phases

- **Meta 1 Code**: jucompiler.l generates lexer that produces tokens
- **Meta 2 Code**: jucompiler.y (parser) builds AST from tokens
- **Meta 3 Code**: Semantic analysis operates on the AST

All three phases work together:
```
Source Code → Lexer (Meta 1) → Tokens 
           → Parser (Meta 2) → AST
           → Semantic (Meta 3) → Type-checked AST + Symbols
```

## Status

✅ Meta 3 Phase 1 is complete and fully tested
✅ All code is self-contained in this folder
✅ Can be built and tested independently
✅ Ready to submit to Mooshak

Next: Implement Phase 2 (type checking) and Phase 3 (error handling)
