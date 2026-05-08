# Targeted Test Suite

These tests probe specific edge cases that might be failing in Mooshak's private tests.

## Files

- **test_1_overloading_widening.java** - Method overloading with type widening
- **test_2_complex_expressions.java** - Complex valid expressions for AST annotation
- **test_3_expression_errors.java** - Many expression errors in one file
- **test_4_return_errors.java** - Return statement edge cases
- **test_5_print_calls.java** - Print and method calls combinations
- **test_6_field_method_interaction.java** - Field/method same name patterns
- **test_7_nested_assignments.java** - Chained and nested assignments
- **test_8_ast_complete.java** - Comprehensive valid program (AST annotation focus)
- **test_9_if_while_with_body_errors.java** - If/while with both condition and body errors
- **test_10_method_signature_annotation.java** - Method signature annotation in calls

## How to Run

```bash
# First make sure the compiler is built
cd ../../meta3
make clean && make
cd ../testes/meta3_targeted

# Run all targeted tests
bash run_tests.sh

# Or run a specific one
../../meta3/jucompiler -s < test_1_overloading_widening.java
```

## What to Look For

After running, paste the output back so we can analyze:
1. Does the AST look correctly annotated?
2. Are all expected errors reported?
3. Are there any errors reported that shouldn't be?
4. Is the order of errors what you'd expect?

We're particularly interested in:
- Tests 1, 2, 8, 10: AST annotation correctness (the 4 AST points)
- Tests 3, 4, 5, 9: Error reporting (the 4 expression/statement points)
