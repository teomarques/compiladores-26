/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#ifndef _SYMTAB_H
#define _SYMTAB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    ClassEntryNode *entries;
} ClassTable;

Symbol *create_symbol(const char *name, JType type, int is_param, int line, int col);
void add_symbol_to_list(Symbol **list, Symbol *s);
Symbol *find_symbol_in_list(Symbol *list, const char *name);

MethodEntry *create_method(const char *name, JType return_type, JType *param_types, int n_params, int line, int col);
void add_method_to_list(MethodEntry **list, MethodEntry *m);

ClassTable *create_class_table(const char *name);
void add_class_entry(ClassTable *ct, ClassEntryKind kind, Symbol *field, MethodEntry *method);

void print_symbol_tables(ClassTable *ct);
void free_class_table(ClassTable *ct);

const char *jtype_to_string(JType t);

#endif
