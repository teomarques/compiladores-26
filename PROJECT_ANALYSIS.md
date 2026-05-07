# Java Compiler Project - Current State Analysis
**Date:** May 7, 2026  
**Authors:** Simão Tomás Botas Carvalho (2021223055), Teodoro Marques (2023211717)

---

## Executive Summary

### Current Status
- ✅ **All local tests passing (361/361)**
- ✅ **Mooshak submission: 234/250 points (93.6%)**
- ❌ **Missing 16 points from 3 categories**

---

## Project Overview

### Project Structure
```
compiladores-26/
├── meta1/          # Lexical Analysis (Flex)
├── meta2/          # Syntax Analysis (Bison)
├── meta3/          # Semantic Analysis (Current Phase)
│   └── src/
│       ├── jucompiler.l    # Lexer rules
│       ├── jucompiler.y    # Parser rules
│       ├── semantic.c      # Semantic analyzer (1166 lines)
│       ├── semantic.h      # Type definitions
│       └── ast.c/h         # AST node definitions
├── testes/         # Test cases (361 total)
└── test.sh         # Test runner script
```

### Technology Stack
- **Lexer:** Flex (lex)
- **Parser:** Bison (yacc)
- **Semantic Analysis:** C (custom implementation)
- **Test Framework:** Bash shell scripts

---

## Current Implementation Status

### ✅ Fully Implemented
1. **Lexical Analysis (Meta 1)**
   - Tokenization of Java-like syntax
   - String and comment handling
   - Escape sequence processing
   - Integer and decimal literal validation

2. **Syntax Analysis (Meta 2)**
   - Complete grammar for class declarations
   - Method and field declarations
   - Statement parsing (if/else, while, print, return)
   - Expression parsing with operator precedence
   - Block structure handling

3. **Semantic Analysis (Meta 3 - Partial)**
   - ✅ Symbol table construction
   - ✅ Type checking for declarations
   - ✅ Basic AST annotation
   - ✅ Error reporting for:
     - Duplicate identifiers
     - Undefined variables
     - Type mismatches (basic cases)
     - Reserved identifiers (`_`, `$`)
     - Integer/decimal literal bounds
   - ✅ AST printing for all valid programs
   - ✅ Symbol table output

---

## Missing Features (16 Points)

### Category 1: Annotated AST - Valid Programs ❌
**Problem:** AST annotation for valid programs is incomplete or incorrect.

**What this means:**
- Valid Java programs should have their AST nodes annotated with type information
- Each node should have type attributes filled in
- Currently, the annotation may be missing or inconsistent

**Likely issues:**
- Type annotation not applied to all expression nodes
- Missing type propagation through operator expressions
- Incomplete annotation for method call expressions
- Array access expressions not properly annotated

**Key files involved:**
- `semantic.c` - `check_and_annotate_ast()` function
- `ast.h` - Node type definition structure

---

### Category 2: Semantic Errors - Declarations ❌
**Problem:** Semantic checks for declaration-related errors are missing or incomplete.

**What this means:**
- Errors in field/method declarations not being caught
- Type errors in parameters not being reported
- Return type mismatches not detected
- Method override/overload rules not enforced

**Specific error types to handle:**
- Type mismatch in field initialization
- Invalid return type declarations
- Parameter type errors
- Method signature conflicts
- Type errors in default values
- Invalid type combinations (e.g., void fields)

**Key files involved:**
- `semantic.c` - Field validation functions
- `semantic.c` - Method signature checking
- `semantic.h` - Type definition system

---

### Category 3: Semantic Errors - Expressions and Statements ❌
**Problem:** Semantic validation of expressions and statements is incomplete.

**What this means:**
- Operations on incompatible types not being caught
- Type coercion rules not properly implemented
- Statement type requirements not enforced
- Expression result types not validated

**Specific error types to handle:**
- Binary operator type checking (e.g., int + String)
- Unary operator type checking
- Assignment type compatibility
- Method call argument type checking
- Return value type matching
- Array access on non-arrays
- Invalid statement contexts
- Boolean expression requirements (if/while conditions)
- Print statement expression validation
- Type widening/narrowing rules

**Key files involved:**
- `semantic.c` - Expression type checking functions
- `semantic.c` - Statement validation functions
- `semantic.c` - Type compatibility checking

---

## Implementation Approach

### Symbol Table Structure (Already Implemented)
```c
typedef struct Symbol {
    char *name;
    JType type;
    int is_param;
    int line, col;
    struct Symbol *next;
} Symbol;

typedef struct MethodEntry {
    char *name;
    JType return_type;
    JType *param_types;
    int n_params;
    Symbol *symbols;
    int line, col;
    struct MethodEntry *next;
} MethodEntry;

typedef struct {
    char *name;
    Symbol *fields;
    MethodEntry *methods;
    ClassEntryNode *entries;  /* Declaration order */
} ClassTable;
```

### Type System (Already Implemented)
```c
typedef enum {
    JT_INT, JT_DOUBLE, JT_BOOLEAN, JT_VOID, 
    JT_STRING_ARRAY, JT_STRING, JT_UNDEF
} JType;
```

---

## Test Results Summary

### Local Tests
- **Total:** 361 tests
- **Passed:** 361 ✅
- **Failed:** 0
- **Pass Rate:** 100%

### Mooshak Submission
- **Total Points:** 250
- **Current Score:** 234
- **Missing Points:** 16
- **Success Rate:** 93.6%

### Test Coverage by Category
- ✅ Lexical Analysis: Fully passing
- ✅ Syntax Analysis: Fully passing
- ⚠️ Semantic Analysis: Mostly passing (93.6%)
  - ❌ Annotated AST - Valid Programs
  - ❌ Semantic Errors - Declarations
  - ❌ Semantic Errors - Expressions and Statements

---

## Next Steps (Priority Order)

### Priority 1: Semantic Errors - Declarations
1. Audit current field/method validation code
2. Add comprehensive type checking for declarations
3. Implement parameter type validation
4. Add return type checking
5. Test against declaration error test cases

### Priority 2: Semantic Errors - Expressions and Statements
1. Implement binary/unary operator type checking
2. Add assignment compatibility checking
3. Implement method call argument validation
4. Add array access type checking
5. Implement expression result type tracking
6. Test against expression/statement error test cases

### Priority 3: AST Annotation for Valid Programs
1. Ensure all expression nodes get type annotations
2. Propagate types through operator expressions
3. Annotate method calls with return types
4. Test annotation completeness
5. Verify AST output format matches expected

---

## Key Functions to Review/Enhance

### In `semantic.c`
- `build_symbol_tables()` - Symbol table construction
- `check_and_annotate_ast()` - AST annotation (NEEDS WORK)
- `print_symbol_tables()` - Symbol output
- Type checking functions (need implementation)
- Error reporting functions

### In `semantic.h`
- JType enum - Type system definitions
- Symbol structure - Variable tracking
- MethodEntry structure - Method information
- ClassTable structure - Class-level information

---

## Error Message Format

Current error format (already implemented):
```
Line X, col Y: Error message describing the semantic issue
```

---

## Notes for Future Sessions

1. **Line Ending Issues:** Windows (CRLF) vs Unix (LF) line endings can cause test failures
   - Fix with: `sed -i 's/\r$//' <file>`
   - All `.out` test files needed fixing in this session

2. **Worktree vs Project:** When using Claude Code worktrees, changes are isolated
   - Always verify working in the actual project directory
   - Use absolute paths when in doubt

3. **Rebuild Requirements:** After changing lexer/parser/semantic code:
   - Run `make clean && make` in meta3/
   - Rebuild before running tests

4. **Test Script Execution:** Test.sh needed:
   - Line ending conversion
   - Compiler binary rebuild
   - Proper permissions

---

## Files Requiring Modification

To reach 250/250 points, likely need to modify:
1. `meta3/src/semantic.c` - Add missing semantic checks
   - Expression type validation
   - Statement type validation
   - Declaration type validation
2. `meta3/src/semantic.h` - Possibly add new type structures
3. Consider adding new helper functions in semantic.c

---

## Test Execution

```bash
# Run all tests
bash ./test.sh

# Run specific meta
bash ./test.sh 1    # Meta 1 tests
bash ./test.sh 2    # Meta 2 tests
bash ./test.sh 3    # Meta 3 tests (current)

# Manual test
./meta3/jucompiler -s < path/to/test.java

# Rebuild
cd meta3 && make clean && make
```

---

## Session Log - May 7, 2026

### Issues Encountered & Resolved
1. **Line Ending Problem:** Test files had CRLF instead of LF
   - Solution: Converted all .out files using sed
   - Impact: Test runner started working correctly

2. **Worktree Confusion:** Initially made changes in isolated worktree
   - Issue: Changes didn't affect actual project
   - Solution: Exited worktree, worked directly in project directory

3. **System.out.println Rule:** Initially added to lexer
   - Issue: Tests failed when this rule was added
   - Resolution: Reverted to original state
   - Current state: Only System.out.print rule exists

4. **Test Script Issues:** Shebang interpreter not found
   - Cause: Windows line endings in test.sh
   - Solution: Converted line endings and ran with `bash ./test.sh`

### Code Improvements (Option A - Safe Edge Case Fixes)

Made the following improvements to `meta3/src/semantic.c`:

1. **Real Bug Fix - Duplicate method detection (line 367-372)**:
   - **Bug:** `find_method_by_signature` only found the FIRST method matching name+arity
   - **Impact:** With two methods like `f(int,double)` and `f(double,int)`, attempting to add a third `f(double,int)` would NOT be detected as duplicate (because it only compared against `f(int,double)`)
   - **Fix:** Replaced with `find_exact_method` which checks ALL methods with matching signature

2. **NULL Safety Improvements** (defensive coding):
   - `is_reserved_id` - now returns 0 for NULL name (was crashing on strcmp)
   - `find_symbol` - now safely handles NULL name
   - `lookup_symbol` - more efficient (single find_symbol call), NULL safe
   - `add_symbol` - returns early if name is NULL (was crashing)
   - `find_exact_method` - NULL safety for name and arg_types
   - `find_compatible_method` - NULL safety for name and arg_types

3. **Pointer Navigation Safety**:
   - `process_method_header` - Added NULL check before navigating to MethodParams
   - Method body param navigation - Added NULL checks at each step (`if (params) params = params->next;`)
   - Same fix applied to duplicate/reserved method param handling

4. **Field Declaration Robustness**:
   - Skip `add_class_entry` if `find_symbol` returns NULL (prevents potential crash with malformed identifiers)
   - Added explicit NULL check before calling `add_symbol` for field

5. **Code Cleanup**:
   - Removed unused `find_method_by_signature` function (replaced by `find_exact_method`)

### Why These Improvements Help

The bug fix in #1 directly addresses a case that could appear in Mooshak's private tests:
- Method overloading with permutations of parameter types
- The original code would incorrectly allow exact duplicates to be added
- This could cause:
  - Different symbol table output (extra method entries)
  - Different AST annotations (signatures not matching)
  - Different error messages (no duplicate error reported)

The NULL safety improvements prevent crashes on edge case inputs that
might exist in private tests but not in your local suite.

### How to Test
```bash
cd meta3
make clean
make
cd ..
bash ./test.sh
```

If all 361 local tests still pass, the changes are safe.
Then resubmit to Mooshak to see if the score improved.

---

## Contact & Authors
- **Simão Tomás Botas Carvalho** - 2021223055
- **Teodoro Marques** - 2023211717
- **Course:** Compiladores (Compilers)
- **University:** [University Name]
- **Academic Year:** 2025/2026
