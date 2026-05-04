#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include "semantic.h"
#include "ast.h"

/* ============================================================
 * PHASE 1: SYMBOL TABLE BUILDING
 * ============================================================ */

/**
 * First pass: Extract type information from AST
 * Builds the class symbol table with all fields and methods
 */
void build_class_symbol_table(struct node *root, class_table_t *class_table);

/**
 * Build method-specific symbol tables
 * Extracts parameters, local variables, and return type
 */
void build_method_symbol_tables(struct node *root, class_table_t *class_table);

/**
 * Extract type information from a type node
 */
type_t extract_type_from_node(struct node *type_node, int *is_array);

/**
 * Get method signature as string for display
 */
char *get_method_signature(method_table_t *method);

/**
 * Check for duplicate symbols
 */
int symbol_exists(method_table_t *method, const char *name);
int field_exists(class_table_t *table, const char *name);

#endif
