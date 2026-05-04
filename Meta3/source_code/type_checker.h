#ifndef _TYPE_CHECKER_H
#define _TYPE_CHECKER_H

#include "semantic.h"
#include "ast.h"

/* ============================================================
 * PHASE 2: TYPE CHECKING AND AST ANNOTATION
 * ============================================================ */

/**
 * Main type checking function
 * Traverses AST and annotates expression nodes with types
 * Reports type errors
 */
void check_and_annotate_ast(
    struct node *root,
    class_table_t *class_table,
    method_table_t *current_method
);

/**
 * Check expression and return its type
 * Annotates the node with the type
 */
type_t check_expression(
    struct node *expr,
    class_table_t *class_table,
    method_table_t *current_method,
    int *is_array
);

/**
 * Check statement
 * Returns 1 if valid, 0 if error
 */
int check_statement(
    struct node *stmt,
    class_table_t *class_table,
    method_table_t *current_method
);

/**
 * Check method body and collect local variables
 */
void check_method_body(
    struct node *body,
    class_table_t *class_table,
    method_table_t *method
);

/**
 * Check variable declaration
 */
void check_var_decl(
    struct node *var_decl,
    class_table_t *class_table,
    method_table_t *current_method
);

/**
 * Check if an operator can be applied to operand types
 */
int is_unary_operator_valid(const char *op, type_t operand_type, int operand_array);
int is_binary_operator_valid(
    const char *op,
    type_t left_type,
    int left_array,
    type_t right_type,
    int right_array
);

/**
 * Get result type of an operation
 */
type_t get_operation_result_type(
    const char *op,
    type_t left_type,
    type_t right_type
);

/**
 * Validate method call (parameter types, argument count)
 */
method_table_t *validate_method_call(
    const char *method_name,
    struct node *call_node,
    class_table_t *class_table,
    method_table_t *current_method
);

#endif
