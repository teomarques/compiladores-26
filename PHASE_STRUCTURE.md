# Meta 3 - Phase-Based Semantic Analyzer Structure

## Folder Organization

Your `compiladores-26/` directory now contains three phase-based folders:

```
compiladores-26/
├── phase1_symboltable/
│   ├── semantic.h
│   ├── semantic.c
│   ├── symbol_table.h
│   └── symbol_table.c
│
├── phase2_typechecking/
│   ├── type_checker.h
│   └── type_checker.c
│
├── phase3_semantic/
│   ├── error_handler.h
│   └── error_handler.c
│
├── Meta3/                    (Documentation & reference)
│   ├── source_code/
│   ├── documentation/
│   ├── QUICK_GUIDE.txt
│   └── README.md
│
├── Makefile                 (Updated for phase-based compilation)
├── jucompiler.y
├── jucompiler.l
├── ast.h
├── ast.c
└── ... other files ...
```

---

## Phase Breakdown

### Phase 1: Symbol Table Building

**Location:** `phase1_symboltable/`

**Files:**
- `semantic.h/c` - Core type system and symbol table structures
- `symbol_table.h/c` - AST traversal and symbol table building

**Responsibilities:**
1. Extract all declarations from AST (fields, methods, parameters, locals)
2. Build class symbol table with all fields and methods
3. Build method-specific symbol tables
4. Maintain declaration order
5. Detect duplicate symbols

**Testing:**
```bash
make phase1              # Build Phase 1 only
./jucompiler -s < test.java | head -20
```

---

### Phase 2: Type Checking & Annotation

**Location:** `phase2_typechecking/`

**Files:**
- `type_checker.h/c` - Type inference and AST annotation

**Responsibilities:**
1. Traverse AST expressions recursively
2. Infer types for all expressions
3. Validate operators (unary and binary)
4. Handle method calls with parameter matching
5. Annotate AST nodes with type information

**Testing:**
```bash
make phase2              # Build Phase 1 & 2
./jucompiler -s < test.java | tail -20
```

---

### Phase 3: Semantic Analysis & Error Handling

**Location:** `phase3_semantic/`

**Files:**
- `error_handler.h/c` - Error reporting and validation

**Responsibilities:**
1. Report semantic errors with proper format
2. Error message templates (specification-compliant)
3. Reserved symbol checking
4. Error counting and tracking
5. Error recovery and continuation

**Testing:**
```bash
make phase3              # Build all phases
./jucompiler -s < test.java
./jucompiler < invalid.java
```

---

## Makefile Usage

### Build Targets

```bash
# Build everything (default)
make all

# Build and run
make run

# Clean build artifacts
make clean

# Rebuild from scratch
make rebuild

# Build Phase 1 only
make phase1

# Build Phase 1 & 2
make phase2

# Build all phases
make phase3
```

### New Makefile Features

✅ Phase-based compilation rules
✅ Proper dependencies between phases
✅ Organized object files (.o in phase directories)
✅ Clean phase cleanup
✅ Phase-specific test targets
✅ Better organization and clarity

---

## Implementation Workflow

### Step 1: Implement Phase 1

1. Open `phase1_symboltable/symbol_table.c`
2. Implement `build_class_symbol_table()` 
3. Implement `build_method_symbol_tables()`
4. Test: `make phase1 && ./jucompiler -s < Factorial.java | head -20`

### Step 2: Implement Phase 2

1. Open `phase2_typechecking/type_checker.c`
2. Implement `check_expression()` 
3. Add operator validation functions
4. Test: `make phase2 && ./jucompiler -s < Factorial.java | tail -30`

### Step 3: Implement Phase 3

1. Phase 3 is mostly complete
2. Integrate error reporting from Phase 1 & 2
3. Test: `make phase3 && ./jucompiler < test.java`

---

## Makefile Targets Summary

| Target | Purpose |
|--------|---------|
| `make all` | Build complete compiler |
| `make run` | Build and run compiler |
| `make clean` | Remove all build artifacts |
| `make rebuild` | Clean + build |
| `make phase1` | Build Phase 1 only |
| `make phase2` | Build Phase 1 + 2 |
| `make phase3` | Build all phases |

---

## Key Functions by Phase

### Phase 1
- `build_class_symbol_table()` - Extract class members
- `build_method_symbol_tables()` - Extract method details
- `create_class_table()` - Initialize class table
- `add_method_to_table()` - Add methods
- `add_param_to_method()` - Add parameters
- `add_local_to_method()` - Add locals

### Phase 2
- `check_expression()` - Type inference
- `check_statement()` - Statement validation
- `is_unary_operator_valid()` - Validate unary ops
- `is_binary_operator_valid()` - Validate binary ops
- `validate_method_call()` - Check method calls

### Phase 3
- `error_*()` functions - Error reporting
- `is_reserved_symbol()` - Check reserved symbols
- `get_semantic_error_count()` - Error tracking

---

**Status:** Phase-based structure ready. Begin with Phase 1 in `phase1_symboltable/`
