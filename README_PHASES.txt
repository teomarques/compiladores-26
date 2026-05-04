================================================================================
                    META 3 SEMANTIC ANALYZER - PHASE STRUCTURE
                              Implementation Guide
================================================================================

Welcome! Your project is now organized into three phases for development.

================================================================================
📁 FOLDER STRUCTURE
================================================================================

compiladores-26/
├── phase1_symboltable/         ← Symbol Table Building
│   ├── semantic.h              (type system definitions)
│   ├── semantic.c              (symbol table management)
│   ├── symbol_table.h          (phase 1 interface)
│   └── symbol_table.c          (AST traversal - IMPLEMENT HERE)
│
├── phase2_typechecking/        ← Type Checking & Annotation
│   ├── type_checker.h          (phase 2 interface)
│   └── type_checker.c          (type inference - IMPLEMENT HERE)
│
├── phase3_semantic/            ← Error Handling
│   ├── error_handler.h         (error definitions - mostly complete)
│   └── error_handler.c         (error reporting - mostly complete)
│
├── Meta3/                      ← Documentation & Reference
│   ├── source_code/
│   ├── documentation/          (INTEGRATION_GUIDE.md is important!)
│   └── QUICK_GUIDE.txt
│
├── Makefile                    (UPDATED for phase-based compilation)
├── PHASE_STRUCTURE.md          (detailed phase information)
├── jucompiler.y                (needs modification)
├── jucompiler.l
├── ast.h
├── ast.c
└── ... other files ...

================================================================================
🎯 WHAT EACH PHASE DOES
================================================================================

PHASE 1: Symbol Table Building
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Location:  phase1_symboltable/
Main File: phase1_symboltable/symbol_table.c

What it does:
  1. Reads the AST produced by the parser
  2. Traverses the AST to find all declarations
  3. Extracts: fields, methods, parameters, local variables
  4. Builds symbol tables in declaration order
  5. Detects duplicate symbols

Output:
  Symbol tables printed in this format:
  ===== Class ClassName Symbol Table =====
  methodName ( int ) returnType
  
  ===== Method methodName ( int ) Symbol Table =====
  return       int
  paramName    int     param
  localVarName int

Test:
  make phase1
  ./jucompiler -s < Factorial.java | head -20


PHASE 2: Type Checking & Annotation
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Location:  phase2_typechecking/
Main File: phase2_typechecking/type_checker.c

What it does:
  1. Traverses AST expressions recursively
  2. Infers the type of each expression
  3. Validates operators (can we do int + boolean?)
  4. Handles method calls with parameter checking
  5. Annotates AST nodes with type information

Output:
  Annotated AST with type information:
  Program
  ..Identifier(Factorial)
  ..MethodDecl
  ....MethodHeader
  ......Int
  ......Identifier(factorial)
  ....MethodBody
  ......If
  ........Eq - boolean
  ..........Identifier(n) - int
  ..........Natural(0) - int

Test:
  make phase2
  ./jucompiler -s < Factorial.java | tail -30


PHASE 3: Semantic Analysis & Error Handling
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Location:  phase3_semantic/
Main File: phase3_semantic/error_handler.c

What it does:
  1. Reports semantic errors (mostly already implemented)
  2. Error message formatting with line/column info
  3. Reserved symbol checking
  4. Error counting and tracking
  5. Error recovery (continue despite errors)

Output:
  Error messages in format:
  Line 5, col 10: Cannot find symbol x
  Line 7, col 15: Operator + cannot be applied to types int , String []

Test:
  make phase3
  ./jucompiler < invalid.java

================================================================================
🔧 UPDATED MAKEFILE
================================================================================

The Makefile has been updated to support phase-based compilation:

BUILD TARGETS:
  make all       - Build complete compiler (default)
  make phase1    - Build Phase 1 only (symbol tables)
  make phase2    - Build Phase 1 + 2 (with type checking)
  make phase3    - Build all phases (complete)
  make clean     - Remove all build artifacts
  make rebuild   - Clean + build everything
  make run       - Build and run jucompiler

FEATURES:
  ✓ Phase-based compilation rules
  ✓ Proper dependency management (phase2 depends on phase1, etc.)
  ✓ Organized object files (stay in phase directories)
  ✓ Phase-specific cleanup
  ✓ Better error messages and organization

EXAMPLE WORKFLOW:
  1. Implement Phase 1
  2. Test: make phase1 && ./jucompiler -s < test.java | head -20
  3. Implement Phase 2
  4. Test: make phase2 && ./jucompiler -s < test.java | tail -30
  5. Implement Phase 3
  6. Test: make phase3 && ./jucompiler < test.java

================================================================================
📝 IMPLEMENTATION STEPS
================================================================================

STEP 1: Implement Phase 1 (4-6 hours)
────────────────────────────────────

File to edit: phase1_symboltable/symbol_table.c

Functions to implement:
  1. build_class_symbol_table()
     - Walk the AST
     - Find all N_FieldDecl nodes (fields)
     - Find all N_MethodDecl nodes (methods)
     - Extract types and names
     - Add to class symbol table

  2. build_method_symbol_tables()
     - For each method in class
     - Extract formal parameters
     - Extract local variables
     - Add return symbol
     - Build method symbol table

Testing:
  make phase1
  ./jucompiler -s < Factorial.java | head -20

Expected Output:
  ===== Class Factorial Symbol Table =====
  factorial ( int ) int
  main ( String []) void

  ===== Method factorial ( int ) Symbol Table =====
  return  int
  n       int     param


STEP 2: Implement Phase 2 (6-8 hours)
────────────────────────────────────

File to edit: phase2_typechecking/type_checker.c

Functions to implement:
  1. check_expression()
     - Handle literals (Natural, Decimal, BoolLit, StrLit)
     - Handle identifiers (lookup in symbol table)
     - Handle unary operators (-, +, !)
     - Handle binary operators (+, -, *, /, etc.)
     - Recursively check sub-expressions
     - Annotate nodes with inferred types

  2. Operator validation functions
     - is_unary_operator_valid()
     - is_binary_operator_valid()
     - get_operation_result_type()

Testing:
  make phase2
  ./jucompiler -s < Factorial.java | tail -30

Expected Output:
  Program
  ..Identifier(Factorial)
  ..MethodDecl
  ....MethodHeader
  ......Int
  ......Identifier(factorial)
  ......MethodParams
  ........ParamDecl
  ..........Int
  ..........Identifier(n)
  ....MethodBody
  ......If
  ........Eq - boolean
  ..........Identifier(n) - int
  ..........Natural(0) - int


STEP 3: Integrate Phase 3 (1-2 hours)
────────────────────────────────────

Phase 3 is mostly complete!

What you need to do:
  1. Ensure Phase 1 calls error_symbol_already_defined() for duplicates
  2. Ensure Phase 2 calls error_* functions when type checking fails
  3. Hook up semantic_error() function in jucompiler.y
  4. Handle -t flag to skip semantic analysis
  5. Handle -s flag to print symbol tables and annotated AST

Testing:
  make phase3
  ./jucompiler -s < Factorial.java      (full output)
  ./jucompiler < invalid.java            (error messages)
  ./jucompiler -t < test.java            (syntax only)

================================================================================
🎯 QUICK REFERENCE: BUILD COMMANDS
================================================================================

Test Phase 1 (Symbol Tables):
  $ make phase1
  $ ./jucompiler -s < testes/Factorial.java | head -20

Test Phase 1 + 2 (Type Annotations):
  $ make phase2
  $ ./jucompiler -s < testes/Factorial.java | tail -30

Test All Phases (Complete):
  $ make phase3
  $ ./jucompiler -s < testes/Factorial.java

Test Error Handling:
  $ make phase3
  $ ./jucompiler < testes/invalid.java

Test with Flags:
  $ ./jucompiler -t < test.java           # Syntax only
  $ ./jucompiler -t -s < test.java        # Syntax + AST
  $ ./jucompiler -s < test.java           # Full semantic analysis
  $ ./jucompiler < test.java              # Default (full)

Clean and Rebuild:
  $ make clean
  $ make rebuild

================================================================================
📚 DOCUMENTATION
================================================================================

For more information, see:

PHASE_STRUCTURE.md
  - Detailed phase information
  - Key functions by phase
  - Makefile usage
  - Debugging tips

Meta3/documentation/INTEGRATION_GUIDE.md
  - How to modify jucompiler.y
  - Main() function with flag handling
  - Code examples and snippets

Meta3/documentation/SEMANTIC_ANALYZER_SCAFFOLD.md
  - Architecture details
  - Data structures explained
  - Type system rules

================================================================================
🔍 FILE LOCATIONS FOR REFERENCE
================================================================================

Phase 1 Symbol Table Building:
  phase1_symboltable/semantic.h
  phase1_symboltable/semantic.c
  phase1_symboltable/symbol_table.h
  phase1_symboltable/symbol_table.c ← IMPLEMENT HERE

Phase 2 Type Checking:
  phase2_typechecking/type_checker.h
  phase2_typechecking/type_checker.c ← IMPLEMENT HERE

Phase 3 Error Handling:
  phase3_semantic/error_handler.h
  phase3_semantic/error_handler.c    ← MOSTLY COMPLETE

Documentation:
  PHASE_STRUCTURE.md                 ← Phase details
  Meta3/documentation/INTEGRATION_GUIDE.md ← jucompiler.y changes

================================================================================
✨ KEY POINTS
================================================================================

✓ Each phase has its own folder
✓ Makefile handles compilation and dependencies
✓ Use 'make phase1', 'make phase2', 'make phase3' to test incrementally
✓ Object files stay organized in phase directories
✓ Easy to debug each phase independently
✓ Clear separation of concerns

================================================================================
🚀 NEXT ACTION
================================================================================

1. Read PHASE_STRUCTURE.md (5 minutes)
2. Read Meta3/documentation/INTEGRATION_GUIDE.md (15 minutes)
3. Open phase1_symboltable/symbol_table.c
4. Implement build_class_symbol_table()
5. Test: make phase1 && ./jucompiler -s < Factorial.java | head -20

Good luck! 🎉

================================================================================
