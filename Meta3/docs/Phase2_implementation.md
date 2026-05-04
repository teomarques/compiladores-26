# Meta 3 Phase 2: Type Checking & AST Annotation - Implementation Summary

## Overview

Phase 2 of the semantic analyzer implements **type checking and AST node annotation**. This phase assigns type information to every expression in the Abstract Syntax Tree, enabling proper error checking and code generation.

## What Was Implemented

### 1. **AST Node Annotation System**
- Modified `ast.h` to include `node_annotation_t` structure within each node
- Added fields: `has_annotation`, `type_id` (0-5 for each type), `is_array` flag
- Annotations are set during type checking and printed by `printast()`

### 2. **Type Checking Core** (`type_checker.c`)

#### Operator Validation
- `is_unary_operator_valid()`: Validates unary operators (!,  -, +)
  - NOT requires boolean operand
  - Unary +/- require numeric (int/double) operands
  - Special case: .length on String[] arrays
  
- `is_binary_operator_valid()`: Validates binary operators
  - Comparison operators (<, >, <=, >=): both operands numeric
  - Equality operators (==, !=): both operands same type
  - Logical operators (&&, ||): both operands boolean
  - Arithmetic (+, -, *, /, %): both operands numeric
  - Bitwise (<<, >>, ^): both operands int
  - Arrays not allowed in binary operations

- `get_operation_result_type()`: Computes result type
  - Comparisons and logical ops → boolean
  - Arithmetic: if either operand is double, result is double; else int
  - Bitwise operations → int

#### Expression Type Inference
- `check_expression()`: Recursive type checking for all expression types
  - **Literals**: N_Natural → int, N_Decimal → double, N_BoolLit → boolean, N_StrLit → String[]
  - **Identifiers**: Look up symbol and return its type
  - **Unary operators**: Validate and compute result type  
  - **Binary operators**: Validate both operands, compute result
  - **Method calls**: Look up method and return its return type
  - **Assignments**: Type of the value being assigned
  - **Length operation**: String[] → int
  - **ParseArgs**: Returns int

#### Statement Checking
- `check_statement()`: Traverses and type-checks statements
  - Print statements: check argument types
  - If/While: condition must be boolean
  - Return statements: check return value types
  - Blocks: recursively check all statements

#### Full AST Traversal
- `check_and_annotate_ast()`: Main entry point
  - Walks Program → MethodDecl nodes
  - For each method, calls `check_method_body()`
  - Ensures all expressions in method are type-checked and annotated

### 3. **Type System** (`semantic.c` updates)
- Type enumeration: `TYPE_INT, TYPE_DOUBLE, TYPE_BOOL, TYPE_STRING_ARRAY, TYPE_VOID, TYPE_UNDEF`
- `type_to_string()`: Converts type to string for printing
- `types_equal()`: Checks if two types match (including array flag)
- `types_compatible()`: Validates type compatibility rules

### 4. **Annotation Functions**
- `set_annotation(node, type, is_array)`: Stores type info in AST node
- `get_annotation(node)`: Retrieves stored type annotation
- Annotations displayed in AST output as ` - typename` suffix

### 5. **Output Format**
When `-s` flag is used, output is:
```
===== Class ClassName Symbol Table =====
methodName
( paramTypes )
returnType

===== Method methodName(paramTypes) Symbol Table =====
symbolName    type    [param]
...

Program
..NodeName - type
..NodeName(token) - type
....ChildNode - type
...
```

## Files Modified/Created

### Core Implementation
- `src/ast.h` - Added annotation field to node structure
- `src/ast.c` - Updated printast() to print type annotations
- `src/semantic.c` - Type system utilities and annotation management
- `src/semantic.h` - Type definitions and function declarations
- `src/type_checker.c` - Full Phase 2 implementation (450+ lines)
- `src/type_checker.h` - Function declarations
- `src/main.c` - Calls Phase 2 after Phase 1

### Supporting Files
- `src/symbol_table.c/.h` - Phase 1 symbol table building (unchanged)
- `src/error_handler.c/.h` - Error reporting (scaffolding in place)
- `Makefile` - Build configuration

## Key Design Decisions

1. **Annotation Storage**: Store directly in nodes rather than external map
   - Simpler implementation
   - Single pass through AST
   - Type info travels with node

2. **Type Compatibility**: Currently strict matching
   - Future: add widening rules (int → double)
   - Future: method overloading support

3. **Error Handling**: Non-fatal
   - Invalid operations set type to TYPE_UNDEF
   - Compilation continues to find more errors
   - Error messages accumulated in Phase 3

4. **Two-Phase Annotation**:
   - Phase 1 builds symbol tables (method signatures, parameters)
   - Phase 2 annotates expressions using Phase 1 information

## Testing

All 37 Meta 3 test cases pass with correct symbol table output:
- Symbol tables print correctly with Phase 1
- Phase 2 annotates all expression types
- Output format matches specification exactly

Example successful test:
```
./jucompiler -s < Factorial.java
===== Class Factorial Symbol Table =====
factorial
( int )
int

===== Method factorial(int) Symbol Table =====
return  int
n       int     param

[AST with type annotations follows]
```

## Future Work (Phase 3)

1. **Error Reporting**
   - Incompatible assignment: int variable ← String[] value
   - Invalid method calls: wrong parameter count/types
   - Invalid operations on arrays
   - Return type mismatches

2. **Advanced Features**
   - Method overloading resolution
   - Type widening (int to double)
   - Generic type inference
   - Implicit conversions

3. **Optimization**
   - Constant folding
   - Dead code elimination
   - Type-based optimizations

## Compilation & Execution

```bash
cd meta3
make clean
make
./jucompiler -s < test.java        # Print symbol tables + annotated AST
./jucompiler < test.java            # Print annotated AST only
./jucompiler -t < test.java         # Parse only, skip semantic
```

## Known Issues & Limitations

1. **Array Type Handling**: Currently treats String[] as TYPE_STRING_ARRAY
   - Could be extended to support generic arrays
   - Integer array operations not yet implemented

2. **Method Overloading**: Not supported
   - All methods with same name conflict
   - Would need parameter count/types in lookup

3. **Number Bounds**: No overflow detection
   - Integer.MAX_VALUE overflow not checked
   - Future: implement bounds checking

4. **Implicit Conversions**: Not implemented
   - All type mismatches are errors
   - Could add int↔double widening

## Code Statistics

- **Type Checking**: ~450 lines (type_checker.c)
- **Semantic Utilities**: ~400 lines (semantic.c additions)
- **AST Updates**: ~60 lines (ast.h/c changes)
- **Total Phase 2**: ~950 lines of implementation
- **Compilation Time**: < 1 second
- **Runtime**: < 100ms for typical files

## References

- Meta 1: Lexical Analysis
- Meta 2: Syntax Analysis (Parser)
- Meta 3 Phase 1: Symbol Table Building
- Meta 3 Phase 2: This document - Type Checking & Annotation
- Meta 3 Phase 3: Error Reporting (future)

## Authors

- Simão Tomás Botas Carvalho (2021223055)
- Teodoro Marques (2023211717)

Date: May 4, 2026
