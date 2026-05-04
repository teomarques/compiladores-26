/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 *
 * Meta 3 -- Error Handler (Phase 3)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_handler.h"
#include "semantic.h"

/* ============================================================
 * ERROR TRACKING
 * ============================================================ */

static int semantic_error_count = 0;

int get_semantic_error_count(void)
{
    return semantic_error_count;
}

void reset_error_count(void)
{
    semantic_error_count = 0;
}

/* ============================================================
 * RESERVED SYMBOL CHECKING
 * ============================================================ */

int is_reserved_symbol(const char *symbol)
{
    if (!symbol) return 0;

    /* Check for underscore (_) as reserved */
    if (strcmp(symbol, "_") == 0) {
        return 1;
    }

    return 0;
}

/* ============================================================
 * GENERIC ERROR REPORTING
 * ============================================================ */

void report_error(int line, int col, const char *message)
{
    printf("Line %d, col %d: %s\n", line, col, message);
    semantic_error_count++;
}

/* ============================================================
 * SPECIFIC ERROR REPORTING FUNCTIONS
 * ============================================================ */

void error_symbol_already_defined(int line, int col, const char *symbol)
{
    printf("Line %d, col %d: Symbol %s already defined\n", line, col, symbol);
    semantic_error_count++;
}

void error_symbol_reserved(int line, int col, const char *symbol)
{
    printf("Line %d, col %d: Symbol %s is reserved\n", line, col, symbol);
    semantic_error_count++;
}

void error_cannot_find_symbol(int line, int col, const char *symbol)
{
    printf("Line %d, col %d: Cannot find symbol %s\n", line, col, symbol);
    semantic_error_count++;
}

void error_operator_no_type(int line, int col, const char *op, const char *type)
{
    printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",
           line, col, op, type);
    semantic_error_count++;
}

void error_operator_two_types(int line, int col, const char *op,
                               const char *type1, const char *type2)
{
    printf("Line %d, col %d: Operator %s cannot be applied to types %s , %s\n",
           line, col, op, type1, type2);
    semantic_error_count++;
}

void error_incompatible_type(int line, int col, const char *type,
                              const char *statement)
{
    printf("Line %d, col %d: Incompatible type %s in %s statement\n",
           line, col, type, statement);
    semantic_error_count++;
}

void error_number_out_of_bounds(int line, int col, const char *number)
{
    printf("Line %d, col %d: Number %s out of bounds\n", line, col, number);
    semantic_error_count++;
}

void error_method_ambiguous(int line, int col, const char *method_name)
{
    printf("Line %d, col %d: Reference to method %s is ambiguous\n",
           line, col, method_name);
    semantic_error_count++;
}
