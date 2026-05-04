# Compiladores Project Structure

## Folder Organization

Your project is now organized into three meta assignments:

```
compiladores-26/
├── meta1/                    (Lexical Analysis)
│   └── (files for meta1)
│
├── meta2/                    (Syntactic Analysis)
│   ├── jucompiler.l         (Lexer from meta1)
│   ├── jucompiler.y         (Parser - main file)
│   ├── ast.h / ast.c        (AST structures)
│   └── README.md
│
├── meta3/                    (Semantic Analysis) ← YOU ARE HERE
│   ├── semantic.h           (Type system definitions)
│   ├── semantic.c           (Symbol table management)
│   ├── symbol_table.h       (Phase 1 interface)
│   ├── symbol_table.c       (Phase 1 - implement here)
│   ├── type_checker.h       (Phase 2 interface)
│   ├── type_checker.c       (Phase 2 - implement here)
│   ├── error_handler.h      (Phase 3 interface)
│   ├── error_handler.c      (Phase 3 - mostly complete)
│   └── docs/                (Documentation)
│       ├── INTEGRATION_GUIDE.md
│       ├── START_HERE.txt
│       └── ... other docs
│
├── testes/                   (Test cases)
│   ├── meta1/
│   ├── meta2/
│   └── meta3/
│
├── Makefile                 (UPDATED - handles all metas)
├── jucompiler              (final executable)
└── ... other files ...
```

## What Changed

✅ **Organized into meta folders:**
- meta1/ - Lexical analysis
- meta2/ - Syntactic analysis  
- meta3/ - Semantic analysis (all files in one folder, no phase subdivision)

✅ **Updated Makefile:**
- Compiles all metas together
- Meta3 files are compiled along with meta2 lexer/parser
- Handles dependencies between metas
- Single `jucompiler` executable contains all three metas

✅ **Meta3 Structure:**
- All 8 semantic analyzer files in one `meta3/` folder
- Clear, flat structure without phase subdivision
- Documentation in `meta3/docs/`

## Build Commands

```bash
# Build complete compiler (all metas)
make all

# Build and run
make run

# Clean build artifacts
make clean

# Rebuild from scratch
make rebuild
```

## Meta3 Files

All in `meta3/` folder:

**Core Type System:**
- `semantic.h/c` - Type definitions and symbol table structures

**Symbol Table Building:**
- `symbol_table.h/c` - AST traversal to build symbol tables

**Type Checking:**
- `type_checker.h/c` - Type inference and expression annotation

**Error Handling:**
- `error_handler.h/c` - Error reporting and validation

**Documentation:**
- `docs/` - Guides and reference material

## Implementation

Start implementing in this order:

1. **symbol_table.c** - Implement `build_class_symbol_table()` and `build_method_symbol_tables()`
2. **type_checker.c** - Implement `check_expression()` and operator validation
3. **error_handler.c** - Integrate error reporting (mostly done)

All three work together in the semantic analysis phase of the compiler.

## Integration with jucompiler.y

You'll need to modify `jucompiler.y` to:
1. Include meta3 headers
2. Call symbol table building functions
3. Call type checking functions
4. Call error reporting functions

See `meta3/docs/INTEGRATION_GUIDE.md` for detailed instructions.

---

**Status:** Ready for Meta 3 implementation in a unified meta3/ folder
