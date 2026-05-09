/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#include "symtab.h"

Symbol *create_symbol(const char *name, JType type, int is_param, int line, int col) {
    if (!name) return NULL;
    Symbol *s = (Symbol *)malloc(sizeof(Symbol));
    s->name = strdup(name);
    s->type = type;
    s->is_param = is_param;
    s->line = line;
    s->col = col;
    s->next = NULL;
    return s;
}

void add_symbol_to_list(Symbol **list, Symbol *s) {
    if (!s) return;
    if (!*list) {
        *list = s;
    } else {
        Symbol *curr = *list;
        while (curr->next) curr = curr->next;
        curr->next = s;
    }
}

Symbol *find_symbol_in_list(Symbol *list, const char *name) {
    if (!name) return NULL;
    for (Symbol *curr = list; curr; curr = curr->next) {
        if (curr->name && strcmp(curr->name, name) == 0) return curr;
    }
    return NULL;
}

MethodEntry *create_method(const char *name, JType return_type, JType *param_types, int n_params, int line, int col) {
    if (!name) return NULL;
    MethodEntry *m = (MethodEntry *)malloc(sizeof(MethodEntry));
    m->name = strdup(name);
    m->return_type = return_type;
    m->n_params = n_params;
    m->param_types = NULL;
    if (n_params > 0 && param_types) {
        m->param_types = (JType *)malloc(n_params * sizeof(JType));
        memcpy(m->param_types, param_types, n_params * sizeof(JType));
    }
    m->symbols = NULL;
    m->line = line;
    m->col = col;
    m->next = NULL;
    return m;
}

void add_method_to_list(MethodEntry **list, MethodEntry *m) {
    if (!m) return;
    if (!*list) {
        *list = m;
    } else {
        MethodEntry *curr = *list;
        while (curr->next) curr = curr->next;
        curr->next = m;
    }
}

ClassTable *create_class_table(const char *name) {
    ClassTable *ct = (ClassTable *)malloc(sizeof(ClassTable));
    ct->name = strdup(name);
    ct->fields = NULL;
    ct->methods = NULL;
    ct->entries = NULL;
    return ct;
}

void add_class_entry(ClassTable *ct, ClassEntryKind kind, Symbol *field, MethodEntry *method) {
    ClassEntryNode *en = (ClassEntryNode *)malloc(sizeof(ClassEntryNode));
    en->kind = kind;
    if (kind == CE_FIELD) en->field = field;
    else en->method = method;
    en->next = NULL;
    if (!ct->entries) {
        ct->entries = en;
    } else {
        ClassEntryNode *curr = ct->entries;
        while (curr->next) curr = curr->next;
        curr->next = en;
    }
}

void print_symbol_tables(ClassTable *ct) {
    if (!ct || !ct->name) return;
    printf("===== Class %s Symbol Table =====\n", ct->name);
    for (ClassEntryNode *e = ct->entries; e; e = e->next) {
        if (e->kind == CE_FIELD) {
            printf("%s\t\t%s\n", e->field->name, jtype_to_string(e->field->type));
        } else {
            MethodEntry *m = e->method;
            printf("%s\t(", m->name);
            for (int i = 0; i < m->n_params; i++) {
                if (i > 0) printf(",");
                printf("%s", jtype_to_string(m->param_types[i]));
            }
            printf(")\t%s\n", jtype_to_string(m->return_type));
        }
    }
    printf("\n");
    for (MethodEntry *m = ct->methods; m; m = m->next) {
        printf("===== Method %s(", m->name);
        for (int i = 0; i < m->n_params; i++) {
            if (i > 0) printf(",");
            printf("%s", jtype_to_string(m->param_types[i]));
        }
        printf(") Symbol Table =====\n");
        for (Symbol *s = m->symbols; s; s = s->next) {
            printf("%s\t\t%s", s->name, jtype_to_string(s->type));
            if (s->is_param) printf("\tparam");
            printf("\n");
        }
        printf("\n");
    }
}

void free_class_table(ClassTable *ct) {
    if (!ct) return;
    Symbol *f = ct->fields;
    while (f) {
        Symbol *tmp = f; f = f->next;
        free(tmp->name); free(tmp);
    }
    MethodEntry *m = ct->methods;
    while (m) {
        MethodEntry *tmp = m; m = m->next;
        Symbol *s = tmp->symbols;
        while (s) {
            Symbol *stmp = s; s = s->next;
            free(stmp->name); free(stmp);
        }
        if (tmp->param_types) free(tmp->param_types);
        free(tmp->name); free(tmp);
    }
    ClassEntryNode *en = ct->entries;
    while (en) {
        ClassEntryNode *tmp = en; en = en->next;
        free(tmp);
    }
    if (ct->name) free(ct->name);
    free(ct);
}

const char *jtype_to_string(JType t) {
    switch (t) {
        case JT_INT: return "int";
        case JT_DOUBLE: return "double";
        case JT_BOOLEAN: return "boolean";
        case JT_VOID: return "void";
        case JT_STRING_ARRAY: return "String[]";
        case JT_STRING: return "String";
        case JT_UNDEF: return "undef";
        default: return "unknown";
    }
}
