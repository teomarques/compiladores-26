# JuCompiler - Java Compiler Project

A four-phase compiler for a Java subset, organized by compilation phase.

## Project Structure

```
compiladores-26/
├── meta1/              # Lexical Analysis (lex)
│   ├── jucompiler.l   # Token patterns
│   └── README.md      # Phase details
├── meta2/              # Syntactic Analysis (yacc)
│   ├── jucompiler.y   # Grammar rules
│   ├── ast.h/c        # AST structures
│   └── README.md      # Phase details
├── meta3/              # Semantic Analysis (scaffolding)
│   ├── semantic.*      # Type system
│   ├── symbol_table.*  # Symbol tables
│   ├── type_checker.*  # Type checking
│   ├── error_handler.* # Error reporting
│   └── documentation/  # Guides
├── testes/             # Test cases
├── Makefile            # Build configuration
└── enunciadotxt.txt   # Assignment spec
```

## Quick Start

### Build
```bash
make clean && make
```

### Test
```bash
./jucompiler -s < testes/Factorial.java  # Semantic analysis
./jucompiler -t < testes/Factorial.java  # Syntax check only
```

## Compilation Pipeline

```
Source Code → [Meta 1: Lexer] → Tokens
           → [Meta 2: Parser] → AST
           → [Meta 3: Semantic] → Type-checked AST + Symbols
           → [Meta 4: Codegen] → LLVM IR (future)
```

## Each Meta Phase

- **Meta 1**: Tokenizes source code using lex patterns
- **Meta 2**: Parses tokens into Abstract Syntax Tree using yacc grammar
- **Meta 3**: Validates types, builds symbol tables, reports errors
- **Meta 4**: Generates LLVM IR code (to be implemented)

## Documentation

- **meta1/README.md** - Lexer overview and patterns
- **meta2/README.md** - Parser grammar and AST nodes
- **meta3/QUICK_GUIDE.txt** - Semantic analyzer quick start
- **meta3/documentation/** - Comprehensive semantic analysis guides
- **PROJECT_ORGANIZATION.md** - Full organizational details

## Implementation Status

✅ **Meta 1 & 2** - Complete (lexer and parser working)
🔄 **Meta 3** - Scaffolding ready, implementation needed
⏳ **Meta 4** - Planned

## Key Files

| File | Purpose | Size |
|------|---------|------|
| meta1/jucompiler.l | Lexer specification | 12 KB |
| meta2/jucompiler.y | Parser grammar | 20 KB |
| meta2/ast.c | AST implementation | 1.2 KB |
| meta3/semantic.c | Type system | 11 KB |
| meta3/symbol_table.c | Symbol extraction | 11 KB |
| meta3/type_checker.c | Type validation | 13 KB |
| meta3/error_handler.c | Error reporting | 3 KB |

## Type System

- `int`, `double`, `boolean`, `String[]`, `void`, `undef` (error type)

## Building with Makefile

The Makefile handles:
1. Generating lexer from `meta1/jucompiler.l`
2. Generating parser from `meta2/jucompiler.y`
3. Compiling all object files
4. Linking Meta 1, 2, 3 modules
5. Producing `jucompiler` executable

## Testing

Use `testes/Factorial.java` as reference test case demonstrating:
- Class declarations
- Multiple methods
- Local variables
- Control flow

## Next Steps

1. Run `make clean && make` to build
2. Test with `./jucompiler -s < testes/Factorial.java`
3. Implement Meta 3 semantic analysis (follow `meta3/QUICK_GUIDE.txt`)
4. Plan Meta 4 code generation

---

**For detailed information**, see:
- `PROJECT_ORGANIZATION.md` - Complete structure overview
- `meta1/README.md`, `meta2/README.md` - Phase-specific details
- `meta3/documentation/` - Semantic analyzer guides
- `enunciadotxt.txt` - Full assignment specification
