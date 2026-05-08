/*
 * Semantic Analyzer Header - Meta 3
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "ast.h"

typedef enum {
    JT_INT, JT_DOUBLE, JT_BOOLEAN, JT_VOID, JT_STRING_ARRAY, JT_STRING, JT_UNDEF
} JType;

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

/* Ordered list node for class-level entries (fields and methods mixed) */
typedef enum { CE_FIELD, CE_METHOD } ClassEntryKind;
typedef struct ClassEntryNode {
    ClassEntryKind kind;
    union {
        Symbol *field;
        MethodEntry *method;
    };
    struct ClassEntryNode *next;
} ClassEntryNode;

typedef struct {
    char *name;
    Symbol *fields;
    MethodEntry *methods;
    ClassEntryNode *entries;  /* Fields and methods in declaration order */
} ClassTable;

ClassTable *build_symbol_tables(struct node *program);
void print_symbol_tables(ClassTable *ct);
void check_and_annotate_ast(struct node *program, ClassTable *ct);
void free_class_table(ClassTable *ct);

const char *jtype_to_string(JType t);
JType node_to_jtype(struct node *type_node);

#endif
