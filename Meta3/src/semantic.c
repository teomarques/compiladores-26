/*
 * Semantic Analyzer Implementation - Meta 3
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

extern const char *category_name[];

const char *jtype_to_string(JType t)
{
    switch (t) {
        case JT_INT: return "int";
        case JT_DOUBLE: return "double";
        case JT_BOOLEAN: return "boolean";
        case JT_VOID: return "void";
        case JT_STRING_ARRAY: return "String[]";
        case JT_UNDEF: return "undef";
    }
    return "unknown";
}

JType node_to_jtype(struct node *type_node)
{
    if (!type_node) return JT_UNDEF;

    switch (type_node->category) {
        case N_Int: return JT_INT;
        case N_Double: return JT_DOUBLE;
        case N_Bool: return JT_BOOLEAN;
        case N_Void: return JT_VOID;
        case N_StringArray: return JT_STRING_ARRAY;
        default: return JT_UNDEF;
    }
}

static int is_reserved_id(const char *name)
{
    return (strcmp(name, "_") == 0) || (strcmp(name, "$") == 0);
}

static Symbol *find_symbol(Symbol *symbols, const char *name)
{
    for (Symbol *s = symbols; s; s = s->next) {
        if (strcmp(s->name, name) == 0) return s;
    }
    return NULL;
}

static void add_symbol(Symbol **symbols, const char *name, JType type, int is_param, int line, int col)
{
    Symbol *s = malloc(sizeof(Symbol));
    s->name = malloc(strlen(name) + 1);
    strcpy(s->name, name);
    s->type = type;
    s->is_param = is_param;
    s->line = line;
    s->col = col;
    s->next = NULL;

    if (!*symbols) {
        *symbols = s;
    } else {
        Symbol *tail = *symbols;
        while (tail->next) tail = tail->next;
        tail->next = s;
    }
}

static MethodEntry *find_method_by_signature(MethodEntry *methods, const char *name, int n_params)
{
    for (MethodEntry *m = methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n_params) return m;
    }
    return NULL;
}

static void add_method(MethodEntry **methods, const char *name, JType return_type,
                       JType *param_types, int n_params, int line, int col)
{
    MethodEntry *m = malloc(sizeof(MethodEntry));
    m->name = malloc(strlen(name) + 1);
    strcpy(m->name, name);
    m->return_type = return_type;
    m->n_params = n_params;
    m->param_types = NULL;
    if (n_params > 0) {
        m->param_types = malloc(n_params * sizeof(JType));
        memcpy(m->param_types, param_types, n_params * sizeof(JType));
    }
    m->symbols = NULL;
    m->line = line;
    m->col = col;
    m->next = NULL;

    if (!*methods) {
        *methods = m;
    } else {
        MethodEntry *tail = *methods;
        while (tail->next) tail = tail->next;
        tail->next = m;
    }
}

static JType extract_param_type(struct node *param_node)
{
    if (!param_node) return JT_UNDEF;
    if (param_node->category == N_StringArray) return JT_STRING_ARRAY;
    return node_to_jtype(param_node);
}

static void process_method_header(struct node *header, JType *ret_type,
                                   char **method_name, JType **param_types, int *n_params)
{
    *ret_type = JT_UNDEF;
    *method_name = NULL;
    *param_types = NULL;
    *n_params = 0;

    if (!header || header->category != N_MethodHeader) return;

    struct node_list *c = header->children;
    if (c) c = c->next;  /* Skip sentinel */
    if (!c || !c->node) return;

    /* First child: return type */
    *ret_type = node_to_jtype(c->node);
    if (c->node->category == N_Void) *ret_type = JT_VOID;

    /* Second child: identifier (method name) */
    c = c->next;
    if (c && c->node && c->node->category == N_Identifier) {
        *method_name = c->node->token;
    }

    /* Third child: MethodParams */
    c = c->next;
    if (c && c->node && c->node->category == N_MethodParams) {
        struct node_list *pc = c->node->children;
        if (pc) pc = pc->next;  /* Skip sentinel */
        int count = 0;
        for (struct node_list *tmp = pc; tmp && tmp->node; tmp = tmp->next) {
            if (tmp->node->category == N_ParamDecl) count++;
        }

        if (count > 0) {
            *param_types = malloc(count * sizeof(JType));
            int idx = 0;
            for (pc = c->node->children; pc && pc->node; pc = pc->next) {
                if (pc->node->category == N_ParamDecl) {
                    struct node_list *pd = pc->node->children;
                    if (pd && pd->node) {
                        (*param_types)[idx++] = extract_param_type(pd->node);
                    }
                }
            }
        }
        *n_params = count;
    }
}

ClassTable *build_symbol_tables(struct node *program)
{
    if (!program || program->category != N_Program) return NULL;

    ClassTable *ct = malloc(sizeof(ClassTable));
    ct->name = NULL;
    ct->fields = NULL;
    ct->methods = NULL;

    /* First child is class Identifier (skip sentinel) */
    struct node_list *c = program->children;
    if (c) c = c->next;  /* Skip sentinel */
    if (c && c->node && c->node->category == N_Identifier) {
        if (c->node->token) {
            ct->name = malloc(strlen(c->node->token) + 1);
            strcpy(ct->name, c->node->token);
        }
    }

    /* Process FieldDecl and MethodDecl */
    for (c = c->next; c; c = c->next) {
        if (!c->node) {
            continue;
        }

        if (c->node->category == N_FieldDecl) {
            struct node_list *fd = c->node->children;
            if (fd) fd = fd->next;  /* Skip sentinel */
            if (fd && fd->node && fd->next && fd->next->node) {
                struct node *type_node = fd->node;
                struct node *id_node = fd->next->node;

                JType type = node_to_jtype(type_node);
                const char *id = id_node->token;
                int line = id_node->line;
                int col = id_node->col;

                if (is_reserved_id(id)) {
                    printf("Line %d, col %d: Symbol %s is reserved\n", line, col, id);
                } else if (find_symbol(ct->fields, id)) {
                    printf("Line %d, col %d: Symbol %s already defined\n", line, col, id);
                } else {
                    add_symbol(&ct->fields, id, type, 0, line, col);
                }
            }
        }
        else if (c->node->category == N_MethodDecl) {
            struct node_list *md = c->node->children;
            if (md) md = md->next;  /* Skip sentinel */
            if (md && md->node && md->node->category == N_MethodHeader) {
                JType ret_type;
                char *method_name;
                JType *param_types;
                int n_params;

                process_method_header(md->node, &ret_type, &method_name, &param_types, &n_params);

                if (method_name) {
                    int method_line = md->node->line;
                    int method_col = md->node->col;
                    struct node_list *hdr_c = md->node->children;
                    hdr_c = hdr_c->next;
                    if (hdr_c && hdr_c->node && hdr_c->node->category == N_Identifier) {
                        method_line = hdr_c->node->line;
                        method_col = hdr_c->node->col;
                    }

                    MethodEntry *existing = find_method_by_signature(ct->methods, method_name, n_params);
                    if (existing) {
                        printf("Line %d, col %d: Symbol %s() already defined\n",
                               method_line, method_col, method_name);
                    } else {
                        add_method(&ct->methods, method_name, ret_type, param_types, n_params,
                                   method_line, method_col);

                        MethodEntry *method = find_method_by_signature(ct->methods, method_name, n_params);
                        if (method) {
                            add_symbol(&method->symbols, "return", ret_type, 0, method_line, method_col);

                            struct node_list *params = md->node->children;
                            params = params->next;
                            params = params->next;
                            if (params && params->node && params->node->category == N_MethodParams) {
                                for (struct node_list *pc = params->node->children; pc; pc = pc->next) {
                                    if (pc->node && pc->node->category == N_ParamDecl) {
                                        struct node_list *pd = pc->node->children;
                                        if (pd) pd = pd->next;  /* Skip sentinel */
                                        if (pd && pd->node && pd->next && pd->next->node) {
                                            JType ptype = extract_param_type(pd->node);
                                            const char *pname = pd->next->node->token;
                                            int pline = pd->next->node->line;
                                            int pcol = pd->next->node->col;

                                            if (is_reserved_id(pname)) {
                                                printf("Line %d, col %d: Symbol %s is reserved\n",
                                                       pline, pcol, pname);
                                            } else if (find_symbol(method->symbols, pname)) {
                                                printf("Line %d, col %d: Symbol %s already defined\n",
                                                       pline, pcol, pname);
                                            } else {
                                                add_symbol(&method->symbols, pname, ptype, 1,
                                                           pline, pcol);
                                            }
                                        }
                                    }
                                }
                            }

                            if (md->next && md->next->node && md->next->node->category == N_MethodBody) {
                                struct node_list *body = md->next->node->children;
                                for (; body; body = body->next) {
                                    if (body->node && body->node->category == N_VarDecl) {
                                        struct node_list *vd = body->node->children;
                                        if (vd) vd = vd->next;  /* Skip sentinel */
                                        if (vd && vd->node && vd->next && vd->next->node) {
                                            JType vtype = node_to_jtype(vd->node);
                                            const char *vname = vd->next->node->token;
                                            int vline = vd->next->node->line;
                                            int vcol = vd->next->node->col;

                                            if (is_reserved_id(vname)) {
                                                printf("Line %d, col %d: Symbol %s is reserved\n",
                                                       vline, vcol, vname);
                                            } else if (find_symbol(method->symbols, vname)) {
                                                printf("Line %d, col %d: Symbol %s already defined\n",
                                                       vline, vcol, vname);
                                            } else {
                                                add_symbol(&method->symbols, vname, vtype, 0,
                                                           vline, vcol);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (param_types) free(param_types);
                }
            }
        }
    }

    return ct;
}

void print_symbol_tables(ClassTable *ct)
{
    if (!ct || !ct->name) return;

    printf("===== Class %s Symbol Table =====\n", ct->name);

    for (Symbol *f = ct->fields; f; f = f->next) {
        printf("%s\t\t%s\n", f->name, jtype_to_string(f->type));
    }

    printf("\n");

    for (MethodEntry *m = ct->methods; m; m = m->next) {
        printf("%s(", m->name);
        for (int i = 0; i < m->n_params; i++) {
            if (i > 0) printf(",");
            printf("%s", jtype_to_string(m->param_types[i]));
        }
        printf(")\t%s\n", jtype_to_string(m->return_type));
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

void free_class_table(ClassTable *ct)
{
    if (!ct) return;

    Symbol *f = ct->fields;
    while (f) {
        Symbol *tmp = f;
        f = f->next;
        free(tmp->name);
        free(tmp);
    }

    MethodEntry *m = ct->methods;
    while (m) {
        MethodEntry *tmp = m;
        m = m->next;

        Symbol *s = tmp->symbols;
        while (s) {
            Symbol *stmp = s;
            s = s->next;
            free(stmp->name);
            free(stmp);
        }

        if (tmp->param_types) free(tmp->param_types);
        free(tmp->name);
        free(tmp);
    }

    if (ct->name) free(ct->name);
    free(ct);
}
