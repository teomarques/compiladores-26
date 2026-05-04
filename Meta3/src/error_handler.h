#ifndef _ERROR_HANDLER_H
#define _ERROR_HANDLER_H

#include "semantic.h"
#include "ast.h"

/* ============================================================
 * ERROR HANDLING AND REPORTING
 * ============================================================ */

/**
 * Error message templates
 */
#define ERR_SYMBOL_ALREADY_DEFINED  "Symbol %s already defined"
#define ERR_SYMBOL_RESERVED         "Symbol %s is reserved"
#define ERR_CANNOT_FIND_SYMBOL      "Cannot find symbol %s"
#define ERR_OPERATOR_NO_TYPE        "Operator %s cannot be applied to type %s"
#define ERR_OPERATOR_TWO_TYPES      "Operator %s cannot be applied to types %s , %s"
#define ERR_INCOMPATIBLE_TYPE       "Incompatible type %s in %s statement"
#define ERR_NUMBER_OUT_OF_BOUNDS    "Number %s out of bounds"
#define ERR_METHOD_AMBIGUOUS        "Reference to method %s is ambiguous"
#define ERR_INCOMPATIBLE_RETURN     "Incompatible type void in return statement"

/**
 * Error reporting with line and column info
 */
void report_error(int line, int col, const char *message);

/**
 * Semantic-specific error reporting
 */
void error_symbol_already_defined(int line, int col, const char *symbol);
void error_symbol_reserved(int line, int col, const char *symbol);
void error_cannot_find_symbol(int line, int col, const char *symbol);
void error_operator_no_type(int line, int col, const char *op, const char *type);
void error_operator_two_types(int line, int col, const char *op, const char *type1, const char *type2);
void error_incompatible_type(int line, int col, const char *type, const char *statement);
void error_number_out_of_bounds(int line, int col, const char *number);
void error_method_ambiguous(int line, int col, const char *method_name);

/**
 * Check if a symbol is reserved (like "_")
 */
int is_reserved_symbol(const char *symbol);

/**
 * Get the semantic error count
 */
int get_semantic_error_count(void);
void reset_error_count(void);

#endif
