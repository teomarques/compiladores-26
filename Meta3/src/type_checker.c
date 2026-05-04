/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 *
 * Meta 3 -- Type Checking and Annotation (Phase 2)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type_checker.h"
#include "symbol_table.h"
#include "ast.h"
#include "semantic.h"

/* ============================================================
 * OPERATOR VALIDATION
 * ============================================================ */

int is_unary_operator_valid(const char *op, type_t operand_type, int operand_array)
{
    /* Handle array types - most unary ops don't apply */
    if (operand_array && operand_type == TYPE_STRING_ARRAY) {
        /* .length is allowed on String[] */
        if (strcmp(op, "length") == 0) {
            return 1;
        }
        return 0;
    }

    /* NOT operator: requires boolean */
    if (strcmp(op, "!") == 0) {
        return operand_type == TYPE_BOOL;
    }

    /* Unary minus/plus: requires numeric type */
    if (strcmp(op, "-") == 0 || strcmp(op, "+") == 0) {
        return operand_type == TYPE_INT || operand_type == TYPE_DOUBLE;
    }

    return 0;
}

int is_binary_operator_valid(
    const char *op,
    type_t left_type,
    int left_array,
    type_t right_type,
    int right_array)
{
    /* Arrays not allowed in binary operations */
    if (left_array || right_array) {
        return 0;
    }

    /* Comparison operators: require numeric types on both sides */
    if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 ||
        strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0) {
        return (left_type == TYPE_INT || left_type == TYPE_DOUBLE) &&
               (right_type == TYPE_INT || right_type == TYPE_DOUBLE);
    }

    /* Equality operators: both sides must be same type */
    if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) {
        return types_equal(left_type, right_type, left_array, right_array);
    }

    /* Logical operators: both sides must be boolean */
    if (strcmp(op, "&&") == 0 || strcmp(op, "||") == 0) {
        return left_type == TYPE_BOOL && right_type == TYPE_BOOL;
    }

    /* Arithmetic operators: both sides must be numeric */
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
        strcmp(op, "*") == 0 || strcmp(op, "/") == 0 ||
        strcmp(op, "%") == 0) {
        return (left_type == TYPE_INT || left_type == TYPE_DOUBLE) &&
               (right_type == TYPE_INT || right_type == TYPE_DOUBLE);
    }

    /* Bitwise operators: require integer types */
    if (strcmp(op, "<<") == 0 || strcmp(op, ">>") == 0 ||
        strcmp(op, "^") == 0) {
        return left_type == TYPE_INT && right_type == TYPE_INT;
    }

    return 0;
}

type_t get_operation_result_type(
    const char *op,
    type_t left_type,
    type_t right_type)
{
    /* Comparison operators return boolean */
    if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 ||
        strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0 ||
        strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) {
        return TYPE_BOOL;
    }

    /* Logical operators return boolean */
    if (strcmp(op, "&&") == 0 || strcmp(op, "||") == 0) {
        return TYPE_BOOL;
    }

    /* Arithmetic operators: return common type */
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
        strcmp(op, "*") == 0 || strcmp(op, "/") == 0 ||
        strcmp(op, "%") == 0) {
        /* If either operand is double, result is double */
        if (left_type == TYPE_DOUBLE || right_type == TYPE_DOUBLE) {
            return TYPE_DOUBLE;
        }
        return TYPE_INT;
    }

    return TYPE_UNDEF;
}

/* ============================================================
 * EXPRESSION TYPE CHECKING
 * ============================================================ */

type_t check_expression(
    struct node *expr,
    class_table_t *class_table,
    method_table_t *current_method,
    int *is_array)
{
    type_t result_type = TYPE_UNDEF;
    *is_array = 0;

    if (!expr) {
        return TYPE_UNDEF;
    }

    switch (expr->category) {
        /* Literals */
        case N_Natural:
        case N_Decimal:
            result_type = (expr->category == N_Natural) ? TYPE_INT : TYPE_DOUBLE;
            set_annotation(expr, result_type, 0);
            break;

        case N_BoolLit:
            result_type = TYPE_BOOL;
            set_annotation(expr, result_type, 0);
            break;

        case N_StrLit:
            result_type = TYPE_STRING_ARRAY;
            *is_array = 1;
            set_annotation(expr, result_type, 1);
            break;

        /* Identifier */
        case N_Identifier: {
            if (current_method) {
                symbol_t *sym = lookup_symbol(current_method, expr->token);
                if (sym) {
                    result_type = sym->type;
                    *is_array = sym->is_array;
                    set_annotation(expr, result_type, *is_array);
                } else {
                    semantic_error(0, 0, "Cannot find symbol %s", expr->token);
                    result_type = TYPE_UNDEF;
                }
            } else {
                semantic_error(0, 0, "No current method context");
            }
            break;
        }

        /* Unary operators */
        case N_Not: {
            struct node_list *child = expr->children;
            if (child && child->node) {
                int child_array = 0;
                type_t child_type = check_expression(
                    child->node,
                    class_table,
                    current_method,
                    &child_array
                );

                if (is_unary_operator_valid("!", child_type, child_array)) {
                    result_type = TYPE_BOOL;
                    set_annotation(expr, result_type, 0);
                } else {
                    semantic_error(0, 0,
                        "Operator ! cannot be applied to type %s",
                        type_to_string(child_type, child_array)
                    );
                    result_type = TYPE_UNDEF;
                }
            }
            break;
        }

        case N_Minus:
        case N_Plus: {
            struct node_list *child = expr->children;
            if (child && child->node) {
                int child_array = 0;
                type_t child_type = check_expression(
                    child->node,
                    class_table,
                    current_method,
                    &child_array
                );

                const char *op = (expr->category == N_Minus) ? "-" : "+";
                if (is_unary_operator_valid(op, child_type, child_array)) {
                    result_type = child_type;
                    set_annotation(expr, result_type, 0);
                } else {
                    semantic_error(0, 0,
                        "Operator %s cannot be applied to type %s",
                        op,
                        type_to_string(child_type, child_array)
                    );
                    result_type = TYPE_UNDEF;
                }
            }
            break;
        }

        /* Binary operators */
        case N_Add:
        case N_Sub:
        case N_Mul:
        case N_Div:
        case N_Mod:
        case N_Eq:
        case N_Ne:
        case N_Lt:
        case N_Gt:
        case N_Le:
        case N_Ge:
        case N_And:
        case N_Or:
        case N_Xor:
        case N_Lshift:
        case N_Rshift: {
            struct node_list *child = expr->children;

            if (child && child->node) {
                int left_array = 0;
                type_t left_type = check_expression(
                    child->node,
                    class_table,
                    current_method,
                    &left_array
                );

                child = child->next;
                if (child && child->node) {
                    int right_array = 0;
                    type_t right_type = check_expression(
                        child->node,
                        class_table,
                        current_method,
                        &right_array
                    );

                    /* Get operator name for error messages */
                    const char *op_name = "?";
                    switch (expr->category) {
                        case N_Add: op_name = "+"; break;
                        case N_Sub: op_name = "-"; break;
                        case N_Mul: op_name = "*"; break;
                        case N_Div: op_name = "/"; break;
                        case N_Mod: op_name = "%"; break;
                        case N_Eq: op_name = "=="; break;
                        case N_Ne: op_name = "!="; break;
                        case N_Lt: op_name = "<"; break;
                        case N_Gt: op_name = ">"; break;
                        case N_Le: op_name = "<="; break;
                        case N_Ge: op_name = ">="; break;
                        case N_And: op_name = "&&"; break;
                        case N_Or: op_name = "||"; break;
                        case N_Xor: op_name = "^"; break;
                        case N_Lshift: op_name = "<<"; break;
                        case N_Rshift: op_name = ">>"; break;
                        default: op_name = "?"; break;
                    }

                    if (is_binary_operator_valid(
                        op_name, left_type, left_array,
                        right_type, right_array)) {
                        result_type = get_operation_result_type(op_name, left_type, right_type);
                        set_annotation(expr, result_type, 0);
                    } else {
                        semantic_error(0, 0,
                            "Operator %s cannot be applied to types %s , %s",
                            op_name,
                            type_to_string(left_type, left_array),
                            type_to_string(right_type, right_array)
                        );
                        result_type = TYPE_UNDEF;
                    }
                }
            }
            break;
        }

        /* Method calls */
        case N_Call: {
            struct node_list *child = expr->children;
            if (child && child->node && child->node->category == N_Identifier) {
                char *method_name = child->node->token;
                method_table_t *called_method =
                    validate_method_call(method_name, expr, class_table, current_method);

                if (called_method) {
                    result_type = called_method->return_type;
                    set_annotation(expr, result_type, 0);
                } else {
                    result_type = TYPE_UNDEF;
                }
            }
            break;
        }

        /* Assignment */
        case N_Assign: {
            struct node_list *child = expr->children;
            if (child && child->node) {
                /*  int target_array = 0;
             type_t target_type = check_expression(
                    child->node,
                    class_table,
                    current_method,
                    &target_array
                );  */

                 /* For now, we only check the value type and annotate the assignment expression with it.
                    Full type compatibility checks will be implemented later. */

                child = child->next;
                if (child && child->node) {
                    int value_array = 0;
                    type_t value_type = check_expression(
                        child->node,
                        class_table,
                        current_method,
                        &value_array
                    );

                    /* Assignment value type becomes the assignment expression type */
                    result_type = value_type;
                    *is_array = value_array;
                    set_annotation(expr, result_type, *is_array);
                }
            }
            break;
        }

        default:
            break;
    }

    return result_type;
}

/* ============================================================
 * STATEMENT TYPE CHECKING
 * ============================================================ */

int check_statement(
    struct node *stmt,
    class_table_t *class_table,
    method_table_t *current_method)
{
    if (!stmt) return 1;

    switch (stmt->category) {
        case N_VarDecl:
            check_var_decl(stmt, class_table, current_method);
            return 1;

        case N_Print:
        case N_ParseArgs:
        case N_Return:
            /* TODO: Implement return type checking */
            return 1;

        case N_If:
        case N_While:
            /* TODO: Check condition type */
            return 1;

        default:
            return 1;
    }
}

void check_var_decl(
    struct node *var_decl,
    class_table_t *class_table,
    method_table_t *current_method)
{
    if (!var_decl || var_decl->category != N_VarDecl) return;

    /* Already processed during symbol table building */
}

/* ============================================================
 * METHOD VALIDATION
 * ============================================================ */

method_table_t *validate_method_call(
    const char *method_name,
    struct node *call_node,
    class_table_t *class_table,
    method_table_t *current_method)
{
    if (!class_table || !method_name) return NULL;

    /* TODO: Count and type-check arguments */
    return lookup_method(class_table, method_name, -1);
}

/* ============================================================
 * MAIN AST ANNOTATION FUNCTION
 * ============================================================ */

void check_and_annotate_ast(
    struct node *root,
    class_table_t *class_table,
    method_table_t *current_method)
{
    if (!root) return;

    /* TODO: Implement full AST traversal and annotation */
}

void check_method_body(
    struct node *body,
    class_table_t *class_table,
    method_table_t *method)
{
    if (!body || body->category != N_Metho