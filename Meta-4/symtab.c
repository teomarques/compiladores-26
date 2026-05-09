/* symtab.c - Symbol Table Implementation */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

SymTable *symtab_create(const char *name) {
    SymTable *t = (SymTable *)malloc(sizeof(SymTable));
    t->name = strdup(name);
    t->symbols = NULL;
    t->last = NULL;
    t->next = NULL;
    return t;
}

int symtab_add(SymTable *table, const char *name, const char *type, int is_param, const char *param_types) {
    /* Check for duplicate - for methods check name+param_types, for others just name */
    Symbol *s = table->symbols;
    while (s) {
        if (strcmp(s->name, name) == 0) {
            if (param_types && s->param_types) {
                /* Both are methods - only duplicate if same param types */
                if (strcmp(s->param_types, param_types) == 0)
                    return 1; /* already defined */
            } else if (!param_types && !s->param_types) {
                return 1; /* variable already defined */
            } else if (!param_types && s->param_types) {
                /* Adding variable, existing is method - allowed in Juc/Java */
            } else if (param_types && !s->param_types) {
                /* Adding method, existing is variable - allowed in Juc/Java */
            }
        }
        s = s->next;
    }

    Symbol *sym = (Symbol *)malloc(sizeof(Symbol));
    sym->name = strdup(name);
    sym->type = strdup(type);
    sym->is_param = is_param;
    sym->param_types = param_types ? strdup(param_types) : NULL;
    sym->next = NULL;

    if (!table->last) {
        table->symbols = sym;
        table->last = sym;
    } else {
        table->last->next = sym;
        table->last = sym;
    }
    return 0;
}

Symbol *symtab_lookup(SymTable *table, const char *name) {
    if (!table) return NULL;
    Symbol *s = table->symbols;
    while (s) {
        if (strcmp(s->name, name) == 0) return s;
        s = s->next;
    }
    return NULL;
}

int symtab_find_methods(SymTable *table, const char *name, Symbol **results, int max_results) {
    int count = 0;
    Symbol *s = table->symbols;
    while (s && count < max_results) {
        if (strcmp(s->name, name) == 0 && s->param_types) {
            results[count++] = s;
        }
        s = s->next;
    }
    return count;
}

void symtab_print(SymTable *table) {
    printf("===== %s =====\n", table->name);
    Symbol *s = table->symbols;
    while (s) {
        printf("%s\t", s->name);
        if (s->param_types)
            printf("%s\t", s->param_types);
        else
            printf("\t");
        printf("%s", s->type);
        if (s->is_param)
            printf("\tparam");
        printf("\n");
        s = s->next;
    }
}

void symtab_free(SymTable *table) {
    if (!table) return;
    Symbol *s = table->symbols;
    while (s) {
        Symbol *next = s->next;
        free(s->name);
        free(s->type);
        if (s->param_types) free(s->param_types);
        free(s);
        s = next;
    }
    free(table->name);
    free(table);
}

void symtab_free_all(SymTable *table) {
    while (table) {
        SymTable *next = table->next;
        symtab_free(table);
        table = next;
    }
}
