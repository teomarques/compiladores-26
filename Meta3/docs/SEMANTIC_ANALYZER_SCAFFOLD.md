# Meta 3 - Semantic Analyzer Scaffolding

## Overview

This document outlines the scaffolding for the semantic analyzer that builds on the syntactic analyzer from Meta 2.

## Files Created

### Core Semantic Analysis
1. **semantic.h / semantic.c**
   - Main semantic analysis module
   - Type system definitions (TYPE_INT, TYPE_DOUBLE, TYPE_BOOL, TYPE_STRING_ARRAY, TYPE_VOID, TYPE_UNDEF)
   - Symbol table data structures (symbol_t, symbol_list_t, method_table_t, class_table_t)
   - Node annotation system for AST type decoration
   - Core API functions

### Phase 1: Symbol Table Building
2. **symbol_table.h / symbol_table.c**
   - Extract type information from AST nodes
   - Build class symbol table with all fields and methods
   - Build method-specific symbol tables with parameters and local variables
   - Duplicate symbol checking
   - Helper functions for type extraction

### Phase 2: Type Checking & Annotation
3. **type_checker.h / type_checker.c**
   - Expression type checking with recursive traversal
   - Statement type checking
   - Operator validation (unary and binary)
   - Method call validation
   - AST node annotation with type information
   - Operation result type determination

### Phase 3: Error Handling
4. **error_handler.h / error_handler.c**
   - Standardized error reporting with line/column info
   - Error message templates matching the specification
   - Reserved symbol checking
   - Error counting

## Architecture

### Three-Phase Approach

```
Input (jucompiler.y with -s flag)
    ↓
Phase 1: Symbol Table Building
    ├─ Scan AST for class members (fields, methods)
    ├─ Extract type information
    └─ Build method-specific tables
    ↓
Phase 2: Type Checking & Annotation
    ├─ Traverse expressions recursively
    ├─ Validate operator usage
    ├─ Check method calls and variable references
    └─ Annotate AST nodes with types
    ↓
Phase 3: Error Reporting & Output
    ├─ Print symbol tables in declaration order
    ├─ Print annotated AST (with type information)
    └─ Report all accumulated errors
    ↓
Output (symbol tables + annotated AST)
```

## Data Structures

### Type System
```c
typedef enum {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_BOOL,
    TYPE_STRING_ARRAY,
    TYPE_VOID,
    TYPE_UNDEF
} type_t;
```

### Symbol Table Hierarchy
```
class_table_t
├─ class_name
├─ fields (global variables)
└─ methods (method_list_t)
    └─ method_table_t
        ├─ method_name
        ├─ return_type
        ├─ params (formal parameters)
        ├─ locals (local variables)
        └─ all (all symbols in order)
```

### Node Annotation
```c
typedef struct {
    type_t type;
    int is_array;
} node_annotation_t;
```

## Key Functions Overview

### Symbol Table Management
- `create_class_table()` - Initialize class symbol table
- `add_field_to_table()` - Add global variable
- `add_method_to_table()` - Add method
- `create_method_table()` - Initialize method table
- `add_param_to_method()` - Add formal parameter
- `add_local_to_method()` - Add local variable
- `lookup_symbol()` - Search method scope
- `lookup_field()` - Search class scope
- `lookup_method()` - Find method by name and parameters

### Type System
- `extract_type_from_node()` - Get type from AST type node
- `type_to_string()` - Convert type_t to string representation
- `types_equal()` - Check type equality
- `types_compatible()` - Check type compatibility with coercion rules

### Type Checking & Annotation
- `check_expression()` - Recursively check and annotate expressions
- `check_statement()` - Validate statements
- `check_method_body()` - Analyze method body
- `is_unary_operator_valid()` - Validate unary operation
- `is_binary_operator_valid()` - Validate binary operation
- `get_operation_result_type()` - Determine operation result type
- `validate_method_call()` - Check method invocation validity

### Error Reporting
- `report_error()` - Generic error output
- `error_*()` - Specific error functions for each error type
- `is_reserved_symbol()` - Check for reserved identifiers

### Output
- `print_class_table()` - Print class symbol table
- `print_method_table()` - Print method symbol table

## Integration with jucompiler.y

The main yacc file needs modifications:

1. **Include headers**:
   ```c
   #include "semantic.h"
   #include "symbol_table.h"
   #include "type_checker.h"
   #include "error_handler.h"
   ```

2. **After syntax tree is built**, call semantic analysis:
   ```c
   if (syn_errs == 0 && !should_skip_semantic) {
       semantic_analysis(ast);
   }
   ```

3. **Modify main** to handle `-s` flag for symbol table output

## Implementation Strategy

### Step 1: Complete symbol_table.c
- Implement proper AST traversal to extract all declarations
- Handle multiple variable declarations on same line
- Maintain declaration order for output

### Step 2: Complete type_checker.c
- Implement full expression type checking
- Add all operator validations
- Handle method call resolution with parameter matching
- Track control flow for return type checking

### Step 3: Enhance error_handler
- Track error locations more precisely
- Implement error recovery and continuation
- Build error buffer for deferred reporting

### Step 4: Annotation System
- Implement proper node-to-annotation mapping (consider hash table)
- AST traversal with annotation during type checking
- Format annotated output for `-s` flag

### Step 5: Output Formatting
- Symbol table printing in exact specification format
- Annotated AST printing with proper indentation
- Blank line management between tables

## Type Compatibility Rules

Based on Java semantics:

- **Numeric types**: int and double are interoperable (double is widening)
- **Boolean**: Only with boolean expressions
- **String[]**: Only operations: .length, Integer.parseInt, method parameters
- **void**: Return statements only, method return type
- **undef**: Error propagation type for invalid expressions

## Output Format Example

```
===== Class Factorial Symbol Table =====
factorial ( int ) int
main ( String []) void

===== Method factorial ( int ) Symbol Table =====
return	int
n	int	param

===== Method main ( String []) Symbol Table =====
return	void
args	String []	param
argument	int

Program
..Identifier(Factorial)
..MethodDecl
....MethodHeader
......Int
......Identifier(factorial)
...
```

## Testing Strategy

Use the provided Factorial.java test case:
1. Verify symbol tables are printed correctly
2. Check AST annotation with type information
3. Ensure error messages are in correct format
4. Test edge cases for number bounds, reserved symbols, etc.

## TODO Items

Key areas requiring implementation:

1. ✓ Header files and basic structures defined
2. ⏳ Complete AST traversal in symbol_table.c
3. ⏳ Full operator validation matrix
4. ⏳ Method overloading resolution
5. ⏳ Proper annotation mapping
6. ⏳ Control flow analysis for returns
7. ⏳ Integration with main yacc file
8. ⏳ Format output to specification
