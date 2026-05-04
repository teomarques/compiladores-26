# Meta 3 Semantic Analyzer - Scaffolding Summary

## What Has Been Created

A complete scaffolding for the semantic analysis phase of your Java compiler, organized into four main modules:

### Module 1: Core Semantic System (`semantic.h/c`)
- Type system enumeration
- Symbol table data structures (class, method, symbol)
- Node annotation system for AST decoration
- High-level API for semantic analysis

### Module 2: Symbol Table Building (`symbol_table.h/c`)
- AST traversal to extract declarations
- Class symbol table construction
- Method symbol table construction
- Type extraction from AST nodes
- Duplicate symbol detection

### Module 3: Type Checking (`type_checker.h/c`)
- Expression type inference
- Operator validation (unary and binary)
- Statement type checking
- Method call validation with parameter matching
- AST node annotation with inferred types

### Module 4: Error Handling (`error_handler.h/c`)
- Standardized error reporting
- Error message templates (matches spec exactly)
- Reserved symbol checking
- Error count tracking

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                    Input: AST from Parser                   │
└──────────────────────┬──────────────────────────────────────┘
                       │
          ┌────────────┴────────────┐
          │   No semantic errors?   │
          └────┬─────────────────┬──┘
              NO                 YES
              │                   │
              ▼                   ▼
     ┌─────────────────┐   PHASE 1: Symbol Table Building
     │ Report Errors   │   ├─ Extract type information
     │ (continue)      │   ├─ Build class symbol table
     └─────────────────┘   └─ Build method symbol tables
              │                   │
              │                   ▼
              │           PHASE 2: Type Checking
              │           ├─ Check all expressions
              │           ├─ Validate operators
              │           ├─ Annotate AST nodes
              │           └─ Track control flow
              │                   │
              │                   ▼
              │           PHASE 3: Error Reporting
              │           ├─ Compile error messages
              │           ├─ Print symbol tables
              │           └─ Print annotated AST
              │                   │
              └───────────────────┴──────────────────────────┐
                                                             │
                              ┌──────────────────────────────┘
                              │
                              ▼
                   ┌──────────────────────────┐
                   │ Output: Error Messages   │
                   │ Symbol Tables            │
                   │ Annotated AST            │
                   └──────────────────────────┘
```

## Key Data Structures

### Type System
```c
TYPE_INT              // 32-bit integer
TYPE_DOUBLE           // 64-bit floating point
TYPE_BOOL             // boolean value
TYPE_STRING_ARRAY     // String[] (array of strings)
TYPE_VOID             // void return type
TYPE_UNDEF            // error/undefined type (for propagation)
```

### Symbol Table Hierarchy
```
ClassTable {
  class_name: "Factorial"
  fields: [           // Global variables
    "field1" → int
  ]
  methods: [          // Methods in class
    MethodTable {
      method_name: "factorial"
      return_type: int
      params: [       // Formal parameters
        "n" → int
      ]
      locals: [       // Local variables
        "temp" → int
      ]
      all: [          // All symbols in order
        return → int
        n → int (param)
        temp → int
      ]
    }
  ]
}
```

### Expression Annotation
```c
node_annotation_t {
  type: TYPE_INT       // The type of this expression
  is_array: 0          // 0 = scalar, 1 = array
}
```

## Output Format

When compiled with `-s` flag:

```
===== Class ClassName Symbol Table =====
methodName ( type1 , type2 ) returnType

===== Method methodName ( types ) Symbol Table =====
return          returnType
paramName       paramType       param
localVarName    localVarType

Program
..Identifier(name) - type
..MethodDecl
....MethodHeader
......Type
......Identifier(name)
...
```

## Functions You'll Need to Implement Fully

### High Priority (Core Functionality)
1. **`build_class_symbol_table()`** - Walk AST and extract all fields
2. **`build_method_symbol_tables()`** - Extract parameters and locals for each method
3. **`check_expression()`** - Recursively type-check expressions
4. **`is_binary_operator_valid()`** - Implement operator type rules
5. **Annotation mapping** - Need a way to store and retrieve annotations per node

### Medium Priority
6. **`validate_method_call()`** - Method overloading resolution
7. **`check_statement()`** - Validate statement semantics
8. **Return type checking** - Ensure returns match method signature

### Lower Priority (Refinement)
9. Proper error recovery and location tracking
10. Optimization of annotation storage
11. Comprehensive operator validation table

## Implementation Path

**Phase 1 (Symbol Building)** - Should focus here first
- [ ] Complete AST traversal in `symbol_table.c`
- [ ] Ensure declaration order is preserved
- [ ] Test symbol table printing format

**Phase 2 (Type Checking)** - Once Phase 1 works
- [ ] Implement expression type inference
- [ ] Add operator validation rules
- [ ] Test with simple expressions

**Phase 3 (Error Handling)** - As you find edge cases
- [ ] Proper error message formatting
- [ ] Error recovery and continuation
- [ ] Full error message set

## Testing with Factorial.java

Expected output structure:

```bash
$ ./jucompiler -s < testes/Factorial.java

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
[... rest of AST with type annotations ...]
```

## Files Provided

All files in `outputs/` folder:

1. `semantic.h` (100 lines) - Core definitions
2. `semantic.c` (380 lines) - Basic implementations
3. `symbol_table.h` (40 lines) - Phase 1 interface
4. `symbol_table.c` (350 lines) - Partial implementation
5. `type_checker.h` (80 lines) - Phase 2 interface
6. `type_checker.c` (400 lines) - Partial implementation
7. `error_handler.h` (60 lines) - Error definitions
8. `error_handler.c` (120 lines) - Error reporting
9. `SEMANTIC_ANALYZER_SCAFFOLD.md` - Detailed architecture
10. `INTEGRATION_GUIDE.md` - How to integrate with your project
11. `README_SCAFFOLD.md` - This file

## Next Steps

1. **Copy files** to your compiladores-26/ directory
2. **Update Makefile** to include new compilation rules
3. **Modify jucompiler.y** to:
   - Include semantic headers
   - Add main() function with flag handling
   - Call semantic analysis phases
4. **Implement AST traversal** in symbol_table.c
5. **Complete operator validation** in type_checker.c
6. **Test incrementally** with test cases

## Common Patterns

### Traversing AST for declarations
```c
struct node_list *child = root->children;
while (child) {
    struct node *node = child->node;
    if (node->category == N_FieldDecl) {
        /* Process field */
    }
    child = child->next;
}
```

### Checking expressions recursively
```c
type_t left_type = check_expression(left_operand, ...);
type_t right_type = check_expression(right_operand, ...);
if (is_binary_operator_valid(op, left_type, right_type)) {
    result = get_operation_result_type(op, left_type, right_type);
}
```

### Reporting errors
```c
error_cannot_find_symbol(line, col, symbol_name);
// Sets TYPE_UNDEF to propagate error
```

## Type Compatibility Rules (Java Semantics)

- `int` ↔ `double`: widening allowed (int → double)
- `boolean`: only boolean operations, no numeric coercion
- `String[]`: limited operations (.length, Integer.parseInt, method params)
- `void`: return statements only, never in expressions
- `undef`: propagates errors through expressions

## Questions to Answer During Implementation

1. How should duplicate symbols in different scopes be handled?
2. How will you map AST node pointers to annotations?
3. How will method overloading be resolved (simplified rules)?
4. Should type checking continue after errors (yes - for better diagnostics)?
5. How will you track line/column info through recursive checking?

These are design decisions you'll make as you flesh out the implementation.

---

**Total scaffolding provided**: ~1500 lines of starter code with clear function signatures and documentation ready for implementation.

Good luck! The structure is solid; now it's about filling in the traversal logic and type rules.
