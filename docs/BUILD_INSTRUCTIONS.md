# Meta 3 Phase 1 Implementation - Build Instructions

## What Has Been Done

1. ✅ Updated `meta2/jucompiler.y` with:
   - Added includes: `semantic.h`, `symbol_table.h`, `type_checker.h`, `error_handler.h`
   - Added global variable: `int skip_semantic = 0;`
   - Updated main() function to handle `-s` flag and call semantic analysis
   - Phase 1 symbol table building is now integrated

2. ✅ Meta 3 semantic files are ready:
   - `meta3/semantic.h/c` - Type system and symbol table structures
   - `meta3/symbol_table.h/c` - Phase 1: Symbol table building (IMPLEMENTED)
   - `meta3/type_checker.h/c` - Phase 2: Type checking (stub ready)
   - `meta3/error_handler.h/c` - Error handling framework

3. ✅ Makefile is configured to compile all modules

## How to Build on Your System

### Prerequisites
You need the following tools installed:
- `lex` or `flex` - Lexical analyzer generator
- `yacc` or `bison` - Parser generator
- `cc` or `gcc` - C compiler

### Build Steps

```bash
# 1. Navigate to project directory
cd /path/to/compiladores-26

# 2. Clean any old build artifacts
make clean

# 3. Build the entire project
make

# If successful, you'll have the jucompiler executable
```

### Expected Output

```
lex meta1/jucompiler.l           # Generate lexer
cc -Wall -Wno-unused-function -c lex.yy.c  # Compile lexer
yacc -d meta2/jucompiler.y       # Generate parser
cc -Wall -Wno-unused-function -c y.tab.c   # Compile parser
cc -Wall -Wno-unused-function -c meta2/ast.c -o ast.o    # Compile AST
cc -Wall -Wno-unused-function -c meta3/semantic.c -o meta3/semantic.o    # Compile semantic
cc -Wall -Wno-unused-function -c meta3/symbol_table.c -o meta3/symbol_table.o  # Phase 1
cc -Wall -Wno-unused-function -c meta3/type_checker.c -o meta3/type_checker.o  # Phase 2 stub
cc -Wall -Wno-unused-function -c meta3/error_handler.c -o meta3/error_handler.o # Errors
cc -o jucompiler lex.yy.o y.tab.o ast.o meta3/semantic.o meta3/symbol_table.o meta3/type_checker.o meta3/error_handler.o -lfl
```

## Testing Phase 1

Once built, test with:

```bash
# Test syntax checking only (skips semantic analysis)
./jucompiler -t < testes/Factorial.java

# Test Phase 1 symbol table building
./jucompiler -s < testes/Factorial.java
```

### Expected Output for Phase 1

When you run `./jucompiler -s < testes/Factorial.java`, you should see:

```
===== Class Factorial Symbol Table =====
factorial ( int ) int
main ( String []) void

===== Method factorial ( int ) Symbol Table =====
return  int
n       int     param

===== Method main ( String []) Symbol Table =====
return  void
args    String []      param
argument        int

Program
..Identifier(Factorial)
..MethodDecl
...MethodHeader
...
```

## What Phase 1 Does

The symbol table building phase:

1. **Traverses the AST** to find all declarations
2. **Extracts field declarations** and adds them to the class symbol table
3. **Extracts method declarations** with return types
4. **Extracts parameters** from each method signature
5. **Extracts local variables** from method bodies
6. **Maintains declaration order** in linked lists
7. **Prints symbol tables** when `-s` flag is used

## Implementation Details

### Symbol Table Hierarchy

```
ClassTable
├── class_name: "Factorial"
├── fields: [linked list of field symbols]
└── methods: [linked list of method tables]
    ├── MethodTable
    │   ├── method_name: "factorial"
    │   ├── return_type: TYPE_INT
    │   ├── params: [n: int]
    │   ├── locals: [none]
    │   └── all: [return, n]
    └── MethodTable
        ├── method_name: "main"
        ├── return_type: TYPE_VOID
        ├── params: [args: String[]]
        ├── locals: [argument: int]
        └── all: [return, args, argument]
```

### Key Functions in Phase 1

**From symbol_table.c:**
- `extract_type_from_node()` - Extracts type from AST node
- `build_class_symbol_table()` - First pass: extracts fields and methods
- `build_method_symbol_tables()` - Second pass: extracts parameters and locals
- `symbol_exists()` - Checks for duplicate symbols
- `field_exists()` - Checks for duplicate fields

**From semantic.c:**
- `create_class_table()` - Creates class symbol table structure
- `create_method_table()` - Creates method symbol table structure
- `add_field_to_table()` - Adds field to class table
- `add_param_to_method()` - Adds parameter to method
- `add_local_to_method()` - Adds local variable to method
- `add_return_to_method()` - Adds return symbol to method
- `print_class_table()` - Prints class symbol table
- `print_method_table()` - Prints method symbol table

## Troubleshooting

### Error: "lex: No such file or directory"
- Solution: Install `flex` on your system
  - Ubuntu/Debian: `sudo apt-get install flex`
  - macOS: `brew install flex`
  - Windows: Use MinGW or WSL

### Error: "yacc: No such file or directory"
- Solution: Install `bison` (compatible with yacc)
  - Ubuntu/Debian: `sudo apt-get install bison`
  - macOS: `brew install bison`
  - Windows: Use MinGW or WSL

### Error: "undefined reference to `build_class_symbol_table`"
- Solution: Ensure Makefile is compiling all .o files and linking them correctly
- Check: All meta3/*.o files are listed in ALL_OBJ

### Symbol tables not printing
- Solution: Check that `-s` flag is being passed correctly
- Test: `./jucompiler -s < test.java` (not just `-t`)

### Wrong symbol table format
- Solution: Check `print_class_table()` and `print_method_table()` implementations
- Format should match assignment specification exactly

## Next Steps After Phase 1

Once Phase 1 builds and runs successfully:

1. **Verify symbol table output** matches expected format
2. **Test with different Java programs** to ensure robustness
3. **Move to Phase 2**: Type checking (already scaffolded in type_checker.c)
4. **Move to Phase 3**: Error handling (framework in error_handler.c)

## Files Modified

- `meta2/jucompiler.y` - Added semantic includes and Phase 1 integration in main()
- No other files were modified
- All semantic files (`meta3/*.c` and `.h`) are ready to use

## Status

✅ **Phase 1 Implementation**: READY FOR COMPILATION
- All code is in place
- Symbol table building is fully implemented
- Just needs to be compiled on a system with lex and yacc

🔄 **Next**: Compile on your system and test
