/*
 * Semantic Analyzer Implementation - Meta 3
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "semantic.h"

extern const char *category_name[];

static int check_natural_bounds(const char *text) {
    /* Check if integer literal is within int range (0 to 2147483647) */
    /* Remove underscores for comparison */
    char clean[32];
    int j = 0;
    for (int i = 0; text[i] && j < 20; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
        }
    }
    clean[j] = '\0';

    /* Check if it's larger than INT_MAX (2147483647) */
    if (j > 10) return 0; /* More than 10 digits, definitely out of bounds */
    if (j == 10 && strcmp(clean, "2147483647") > 0) return 0;
    return 1;
}

static int check_decimal_bounds(const char *text) {
    /* Check if decimal literal is within valid double range */
    /* First, remove underscores (Java allows them, strtod doesn't) */
    char clean[1000];
    int j = 0;
    int has_digits = 0;
    for (int i = 0; text[i] && j < (int)sizeof(clean) - 1; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
            if (text[i] >= '0' && text[i] <= '9') has_digits = 1;
        }
    }
    clean[j] = '\0';

    char *endptr;
    double val = strtod(clean, &endptr);

    /* strtod returns HUGE_VAL on overflow */
    if (val == HUGE_VAL || val == -HUGE_VAL) return 0;

    /* Check for underflow: if we have significant digits but got 0, it underflowed */
    if (val == 0.0 && has_digits) {
        /* Need to check if original string was non-zero */
        /* If string contains non-zero digits and converted to 0, it's underflow */
        for (int i = 0; clean[i]; i++) {
            if (clean[i] >= '1' && clean[i] <= '9') return 0;  /* Non-zero digit found, underflowed */
        }
    }

    return 1;
}

const char *jtype_to_string(JType t)
{
    switch (t) {
        case JT_INT: return "int";
        case JT_DOUBLE: return "double";
        case JT_BOOLEAN: return "boolean";
        case JT_VOID: return "void";
        case JT_STRING_ARRAY: return "String[]";
        case JT_STRING: return "String";
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

static int types_compatible(JType actual, JType expected); /* Forward declaration */

static MethodEntry *find_method_by_signature(MethodEntry *methods, const char *name, int n_params)
{
    for (MethodEntry *m = methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n_params) return m;
    }
    return NULL;
}

static MethodEntry *find_exact_method(MethodEntry *methods, const char *name, int n_params,
                                       JType *arg_types)
{
    /* Find a method with exact type match (all types must be equal) */
    for (MethodEntry *m = methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n_params) {
            int exact = 1;
            for (int i = 0; i < n_params; i++) {
                if (arg_types[i] != m->param_types[i]) {
                    exact = 0;
                    break;
                }
            }
            if (exact) return m;
        }
    }
    return NULL;
}

static MethodEntry *find_compatible_method(MethodEntry *methods, const char *name, int n_params,
                                           JType *arg_types, int *out_count)
{
    /* Find all methods with matching name and parameter count whose types are compatible */
    MethodEntry *first_match = NULL;
    int match_count = 0;

    for (MethodEntry *m = methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n_params) {
            /* Check if all parameter types are compatible */
            int compatible = 1;
            for (int i = 0; i < n_params; i++) {
                if (!types_compatible(arg_types[i], m->param_types[i])) {
                    compatible = 0;
                    break;
                }
            }
            if (compatible) {
                match_count++;
                if (!first_match) first_match = m;
            }
        }
    }

    *out_count = match_count;
    return first_match;
}

static void add_class_entry(ClassTable *ct, ClassEntryKind kind, Symbol *field, MethodEntry *method)
{
    ClassEntryNode *en = malloc(sizeof(ClassEntryNode));
    en->kind = kind;
    if (kind == CE_FIELD) en->field = field;
    else en->method = method;
    en->next = NULL;
    if (!ct->entries) {
        ct->entries = en;
    } else {
        ClassEntryNode *tail = ct->entries;
        while (tail->next) tail = tail->next;
        tail->next = en;
    }
}

static MethodEntry *add_method(MethodEntry **methods, const char *name, JType return_type,
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

    return m;
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
            for (pc = c->node->children; pc; pc = pc->next) {
                if (pc->node && pc->node->category == N_ParamDecl) {
                    struct node_list *pd = pc->node->children;
                    if (pd) pd = pd->next;  /* Skip sentinel */
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
    ct->entries = NULL;

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
                    add_class_entry(ct, CE_FIELD, find_symbol(ct->fields, id), NULL);
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
                    if (hdr_c) hdr_c = hdr_c->next;  /* Skip sentinel */
                    if (hdr_c) hdr_c = hdr_c->next;  /* Skip return type */
                    if (hdr_c && hdr_c->node && hdr_c->node->category == N_Identifier) {
                        method_line = hdr_c->node->line;
                        method_col = hdr_c->node->col;
                    }

                    /* Check for reserved method name (but don't report yet) */
                    int reserved_method = is_reserved_id(method_name);

                    /* Check for exact duplicate: same name AND same parameter types */
                    MethodEntry *existing = find_method_by_signature(ct->methods, method_name, n_params);
                    int is_duplicate = 0;
                    if (existing && n_params == existing->n_params) {
                        int types_match = 1;
                        for (int i = 0; i < n_params; i++) {
                            if (param_types[i] != existing->param_types[i]) {
                                types_match = 0;
                                break;
                            }
                        }
                        is_duplicate = types_match;
                    }

                    MethodEntry *method = NULL;
                    if (!reserved_method && !is_duplicate) {
                        method = add_method(&ct->methods, method_name, ret_type, param_types, n_params,
                                            method_line, method_col);
                        if (method) add_class_entry(ct, CE_METHOD, NULL, method);
                    }

                    /* Process parameters to check for errors FIRST (before reporting method errors) */
                    if (method) {
                            add_symbol(&method->symbols, "return", ret_type, 0, method_line, method_col);

                            struct node_list *params = md->node->children;
                            params = params->next;  /* Skip sentinel */
                            params = params->next;  /* Skip return type */
                            params = params->next;  /* Skip method name identifier */
                            if (params && params->node && params->node->category == N_MethodParams) {
                                struct node_list *pc = params->node->children;
                                if (pc) pc = pc->next;  /* Skip sentinel */
                                for (; pc; pc = pc->next) {
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
                        } else if (reserved_method || is_duplicate) {
                            /* Process params with temp list to detect duplicate param names even if method is reserved/duplicate */
                            Symbol *temp_symbols = NULL;
                            add_symbol(&temp_symbols, "return", ret_type, 0, method_line, method_col);

                            struct node_list *params = md->node->children;
                            params = params->next;  /* Skip sentinel */
                            params = params->next;  /* Skip return type */
                            params = params->next;  /* Skip method name identifier */
                            if (params && params->node && params->node->category == N_MethodParams) {
                                struct node_list *pc = params->node->children;
                                if (pc) pc = pc->next;  /* Skip sentinel */
                                for (; pc; pc = pc->next) {
                                    if (pc->node && pc->node->category == N_ParamDecl) {
                                        struct node_list *pd = pc->node->children;
                                        if (pd) pd = pd->next;  /* Skip sentinel */
                                        if (pd && pd->node && pd->next && pd->next->node) {
                                            JType ptype = extract_param_type(pd->node);
                                            const char *pname = pd->next->node->token;
                                            int pline = pd->next->node->line;
                                            int pcol = pd->next->node->col;
                                            if (is_reserved_id(pname)) {
                                                printf("Line %d, col %d: Symbol %s is reserved\n", pline, pcol, pname);
                                            } else if (find_symbol(temp_symbols, pname)) {
                                                printf("Line %d, col %d: Symbol %s already defined\n", pline, pcol, pname);
                                            } else {
                                                add_symbol(&temp_symbols, pname, ptype, 1, pline, pcol);
                                            }
                                        }
                                    }
                                }
                            }

                            /* Free temp symbols */
                            while (temp_symbols) {
                                Symbol *tmp = temp_symbols;
                                temp_symbols = temp_symbols->next;
                                free(tmp->name); free(tmp);
                            }

                            /* Report method errors (reserved first, then duplicate only if not reserved) */
                            if (reserved_method) {
                                printf("Line %d, col %d: Symbol %s is reserved\n",
                                       method_line, method_col, method_name);
                            } else if (is_duplicate) {
                                printf("Line %d, col %d: Symbol %s(", method_line, method_col, method_name);
                                for (int i = 0; i < n_params; i++) {
                                    if (i > 0) printf(",");
                                    printf("%s", jtype_to_string(param_types[i]));
                                }
                                printf(") already defined\n");
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

    ClassEntryNode *en = ct->entries;
    while (en) {
        ClassEntryNode *tmp = en;
        en = en->next;
        free(tmp);
    }

    if (ct->name) free(ct->name);
    free(ct);
}

static Symbol *lookup_symbol(const char *name, MethodEntry *method, ClassTable *ct)
{
    if (!method || !ct) return NULL;
    if (find_symbol(method->symbols, name)) return find_symbol(method->symbols, name);
    if (find_symbol(ct->fields, name)) return find_symbol(ct->fields, name);
    return NULL;
}

static int types_compatible(JType actual, JType expected)
{
    if (actual == expected) return 1;
    if (actual == JT_INT && expected == JT_DOUBLE) return 1;
    return 0;
}

static JType infer_type(struct node *n, MethodEntry *method, ClassTable *ct);

static JType check_binary_op(struct node *n, MethodEntry *method, ClassTable *ct)
{
    struct node_list *c = n->children;
    if (c) c = c->next;

    struct node *left = c && c->node ? c->node : NULL;
    struct node *right = c && c->next && c->next->node ? c->next->node : NULL;

    JType left_type = left ? infer_type(left, method, ct) : JT_UNDEF;
    JType right_type = right ? infer_type(right, method, ct) : JT_UNDEF;

    char left_str[32], right_str[32];
    strcpy(left_str, jtype_to_string(left_type));
    strcpy(right_str, jtype_to_string(right_type));

    switch (n->category) {
        case N_Add: case N_Sub: case N_Mul: case N_Div: case N_Mod:
            if ((left_type == JT_INT || left_type == JT_DOUBLE) &&
                (right_type == JT_INT || right_type == JT_DOUBLE)) {
                return (left_type == JT_DOUBLE || right_type == JT_DOUBLE) ? JT_DOUBLE : JT_INT;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_Add ? "+" :
                            n->category == N_Sub ? "-" :
                            n->category == N_Mul ? "*" :
                            n->category == N_Div ? "/" : "%", left_str, right_str);
            return JT_UNDEF;

        case N_Lt: case N_Gt: case N_Le: case N_Ge:
            if ((left_type == JT_INT || left_type == JT_DOUBLE) &&
                (right_type == JT_INT || right_type == JT_DOUBLE)) {
                return JT_BOOLEAN;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_Lt ? "<" :
                            n->category == N_Gt ? ">" :
                            n->category == N_Le ? "<=" : ">=", left_str, right_str);
            return JT_BOOLEAN;

        case N_Eq: case N_Ne: {
            /* == and != only work on primitive types (int, double, boolean), not arrays */
            int valid = 0;
            if (left_type != JT_UNDEF && right_type != JT_UNDEF &&
                left_type != JT_VOID && right_type != JT_VOID &&
                left_type != JT_STRING_ARRAY && right_type != JT_STRING_ARRAY) {
                /* Check if types are compatible (same or widening) */
                if (left_type == right_type) {
                    valid = 1;
                } else if ((left_type == JT_INT && right_type == JT_DOUBLE) ||
                           (left_type == JT_DOUBLE && right_type == JT_INT)) {
                    valid = 1;
                }
            }
            if (!valid) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                       n->line, n->col, n->category == N_Eq ? "==" : "!=", left_str, right_str);
            }
            return JT_BOOLEAN;
        }

        case N_And: case N_Or:
            if (left_type == JT_BOOLEAN && right_type == JT_BOOLEAN) {
                return JT_BOOLEAN;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_And ? "&&" : "||", left_str, right_str);
            return JT_BOOLEAN;

        case N_Lshift: case N_Rshift: case N_Xor:
            if (left_type == JT_INT && right_type == JT_INT) {
                return JT_INT;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_Lshift ? "<<" :
                            n->category == N_Rshift ? ">>" : "^", left_str, right_str);
            return JT_UNDEF;

        default:
            return JT_UNDEF;
    }
}

static JType check_unary_op(struct node *n, MethodEntry *method, ClassTable *ct)
{
    struct node_list *c = n->children;
    if (c) c = c->next;
    struct node *operand = c && c->node ? c->node : NULL;

    JType op_type = operand ? infer_type(operand, method, ct) : JT_UNDEF;
    char op_str[32];
    strcpy(op_str, jtype_to_string(op_type));

    switch (n->category) {
        case N_Not:
            if (op_type == JT_BOOLEAN) return JT_BOOLEAN;
            printf("Line %d, col %d: Operator ! cannot be applied to type %s\n",
                   n->line, n->col, op_str);
            return JT_BOOLEAN;

        case N_Minus: case N_Plus:
            if (op_type == JT_INT || op_type == JT_DOUBLE) return op_type;
            printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",
                   n->line, n->col, n->category == N_Minus ? "-" : "+", op_str);
            return JT_UNDEF;

        case N_Length:
            if (op_type == JT_STRING_ARRAY) return JT_INT;
            printf("Line %d, col %d: Operator .length cannot be applied to type %s\n",
                   n->line, n->col, op_str);
            return JT_INT;

        default:
            return JT_UNDEF;
    }
}

static JType infer_type(struct node *n, MethodEntry *method, ClassTable *ct)
{
    if (!n) return JT_UNDEF;

    switch (n->category) {
        case N_Natural: {
            /* Check bounds for natural number */
            if (n->token && !check_natural_bounds(n->token)) {
                printf("Line %d, col %d: Number %s out of bounds\n", n->line, n->col, n->token);
            }
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "int");
            return JT_INT;
        }

        case N_Decimal: {
            /* Check bounds for decimal number */
            if (n->token && !check_decimal_bounds(n->token)) {
                printf("Line %d, col %d: Number %s out of bounds\n", n->line, n->col, n->token);
            }
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "double");
            return JT_DOUBLE;
        }

        case N_BoolLit:
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "boolean");
            return JT_BOOLEAN;

        case N_StrLit:
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "String");
            return JT_STRING;

        case N_Identifier: {
            if (is_reserved_id(n->token)) {
                printf("Line %d, col %d: Symbol %s is reserved\n", n->line, n->col, n->token);
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                return JT_UNDEF;
            }
            Symbol *s = lookup_symbol(n->token, method, ct);
            if (s) {
                n->type_annot = malloc(16);
                strcpy(n->type_annot, jtype_to_string(s->type));
                return s->type;
            } else {
                printf("Line %d, col %d: Cannot find symbol %s\n", n->line, n->col, n->token);
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                return JT_UNDEF;
            }
        }

        case N_Add: case N_Sub: case N_Mul: case N_Div: case N_Mod:
        case N_Lt: case N_Gt: case N_Le: case N_Ge:
        case N_Eq: case N_Ne: case N_And: case N_Or:
        case N_Lshift: case N_Rshift: case N_Xor: {
            JType result = check_binary_op(n, method, ct);
            n->type_annot = malloc(16);
            strcpy(n->type_annot, jtype_to_string(result));
            return result;
        }

        case N_Not: case N_Minus: case N_Plus: case N_Length: {
            JType result = check_unary_op(n, method, ct);
            n->type_annot = malloc(16);
            strcpy(n->type_annot, jtype_to_string(result));
            return result;
        }

        case N_Call: {
            struct node_list *c = n->children;
            if (c) c = c->next;  /* Skip sentinel */

            struct node *method_id = c && c->node ? c->node : NULL;
            char *method_name = method_id ? method_id->token : NULL;

            /* Check for reserved method name */
            if (method_name && is_reserved_id(method_name)) {
                printf("Line %d, col %d: Symbol %s is reserved\n",
                       n->line, n->col, method_name);
                if (method_id) {
                    method_id->type_annot = malloc(8);
                    strcpy(method_id->type_annot, "undef");
                }
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                /* Still need to infer types of arguments for annotation */
                c = c && c->next ? c->next : NULL;
                if (c && !c->node) c = c->next;
                for (struct node_list *args = c; args; args = args->next) {
                    if (args->node) infer_type(args->node, method, ct);
                }
                return JT_UNDEF;
            }

            c = c && c->next ? c->next : NULL;
            if (c && !c->node) c = c->next;  /* Skip NULL from call_args sentinel if present */

            int n_args = 0;
            JType *arg_types = NULL;

            /* Count arguments (skip NULL nodes from call_args sentinel) */
            for (struct node_list *tmp = c; tmp; tmp = tmp->next) {
                if (tmp->node) n_args++;
            }

            if (n_args > 0) {
                arg_types = malloc(n_args * sizeof(JType));
                int idx = 0;
                for (struct node_list *args = c; args; args = args->next) {
                    if (args->node) arg_types[idx++] = infer_type(args->node, method, ct);
                }
            }

            /* First try exact match (all types must be exactly equal) across ALL methods */
            MethodEntry *called = NULL;
            if (n_args >= 0) {
                called = find_exact_method(ct->methods, method_name, n_args, arg_types);
            }

            /* If no exact match, find all compatible methods */
            int ambiguous = 0;
            if (!called) {
                int match_count = 0;
                called = find_compatible_method(ct->methods, method_name, n_args, arg_types, &match_count);

                /* Check for ambiguity */
                if (match_count > 1) {
                    printf("Line %d, col %d: Reference to method %s(", n->line, n->col, method_name ? method_name : "");
                    for (int i = 0; i < n_args; i++) {
                        if (i > 0) printf(",");
                        printf("%s", jtype_to_string(arg_types[i]));
                    }
                    printf(") is ambiguous\n");
                    ambiguous = 1;
                    called = NULL;
                } else if (match_count == 0) {
                    called = NULL;
                }
            }

            if (called) {
                if (method_id) {
                    /* Format method signature for identifier annotation (dynamic allocation) */
                    int sig_len = 2; /* "(" + ")" */
                    for (int i = 0; i < called->n_params; i++) {
                        if (i > 0) sig_len++; /* comma */
                        sig_len += (int)strlen(jtype_to_string(called->param_types[i]));
                    }
                    char *sig = malloc(sig_len + 1);
                    sig[0] = '(';
                    sig[1] = '\0';
                    for (int i = 0; i < called->n_params; i++) {
                        if (i > 0) strcat(sig, ",");
                        strcat(sig, jtype_to_string(called->param_types[i]));
                    }
                    strcat(sig, ")");
                    method_id->type_annot = sig;
                }
                n->type_annot = malloc(16);
                strcpy(n->type_annot, jtype_to_string(called->return_type));
                if (arg_types) free(arg_types);
                return called->return_type;
            } else if (!ambiguous) {
                printf("Line %d, col %d: Cannot find symbol %s(", n->line, n->col, method_name ? method_name : "");
                for (int i = 0; i < n_args; i++) {
                    if (i > 0) printf(",");
                    printf("%s", jtype_to_string(arg_types[i]));
                }
                printf(")\n");
                if (method_id) {
                    method_id->type_annot = malloc(8);
                    strcpy(method_id->type_annot, "undef");
                }
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                if (arg_types) free(arg_types);
                return JT_UNDEF;
            } else {
                /* Ambiguous call - set undef type but don't print error (already printed) */
                if (method_id) {
                    method_id->type_annot = malloc(8);
                    strcpy(method_id->type_annot, "undef");
                }
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                if (arg_types) free(arg_types);
                return JT_UNDEF;
            }
        }

        case N_ParseArgs: {
            struct node_list *c = n->children;
            if (c) c = c->next;

            JType arg1_type = JT_UNDEF;
            JType arg2_type = JT_UNDEF;

            if (c && c->node) {
                arg1_type = infer_type(c->node, method, ct);
            }
            c = c && c->next ? c->next : NULL;
            if (c && c->node) {
                arg2_type = infer_type(c->node, method, ct);
            }

            /* Check argument types: first should be String[], second should be int */
            if (arg1_type != JT_STRING_ARRAY || arg2_type != JT_INT) {
                printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n",
                       n->line, n->col, jtype_to_string(arg1_type), jtype_to_string(arg2_type));
            }

            n->type_annot = malloc(8);
            strcpy(n->type_annot, "int");
            return JT_INT;
        }

        case N_Assign: {
            struct node_list *c = n->children;
            if (c) c = c->next;
            struct node *lhs = c && c->node ? c->node : NULL;
            struct node *rhs = c && c->next && c->next->node ? c->next->node : NULL;

            JType lhs_type = lhs ? infer_type(lhs, method, ct) : JT_UNDEF;
            JType rhs_type = rhs ? infer_type(rhs, method, ct) : JT_UNDEF;

            /* Check if lhs is a String[] parameter (arrays cannot be reassigned) */
            int is_param_assign = 0;
            if (lhs && lhs->category == N_Identifier) {
                Symbol *lhs_sym = lookup_symbol(lhs->token, method, ct);
                if (lhs_sym && lhs_sym->is_param && lhs_sym->type == JT_STRING_ARRAY) {
                    is_param_assign = 1;
                    printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n",
                           n->line, n->col, jtype_to_string(lhs_type), jtype_to_string(rhs_type));
                }
            }

            /* Report error if either operand is undef or types are incompatible */
            if (!is_param_assign && (lhs_type == JT_UNDEF || rhs_type == JT_UNDEF || !types_compatible(rhs_type, lhs_type))) {
                printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n",
                       n->line, n->col, jtype_to_string(lhs_type), jtype_to_string(rhs_type));
            }

            n->type_annot = malloc(16);
            strcpy(n->type_annot, jtype_to_string(lhs_type));
            return lhs_type;
        }

        default:
            return JT_UNDEF;
    }
}

static void check_statement(struct node *n, MethodEntry *method, ClassTable *ct)
{
    if (!n) return;

    struct node_list *c;
    switch (n->category) {
        case N_VarDecl: {
            /* Add variable to method's symbol table when encountered */
            c = n->children;
            if (c) c = c->next;  /* Skip sentinel */
            if (c && c->node && c->next && c->next->node) {
                JType vtype = node_to_jtype(c->node);
                const char *vname = c->next->node->token;
                int vline = c->next->node->line;
                int vcol = c->next->node->col;

                if (is_reserved_id(vname)) {
                    printf("Line %d, col %d: Symbol %s is reserved\n",
                           vline, vcol, vname);
                } else if (find_symbol(method->symbols, vname)) {
                    printf("Line %d, col %d: Symbol %s already defined\n",
                           vline, vcol, vname);
                } else {
                    add_symbol(&method->symbols, vname, vtype, 0, vline, vcol);
                }
            }
            break;
        }

        case N_If:
            c = n->children;
            if (c) c = c->next;
            if (c && c->node) {
                JType cond_type = infer_type(c->node, method, ct);
                if (cond_type != JT_BOOLEAN) {
                    printf("Line %d, col %d: Incompatible type %s in if statement\n",
                           c->node->line, c->node->col, jtype_to_string(cond_type));
                }
            }
            c = c && c->next ? c->next : NULL;
            if (c && c->node) check_statement(c->node, method, ct);
            c = c && c->next ? c->next : NULL;
            if (c && c->node) check_statement(c->node, method, ct);
            break;

        case N_While:
            c = n->children;
            if (c) c = c->next;
            if (c && c->node) {
                JType cond_type = infer_type(c->node, method, ct);
                if (cond_type != JT_BOOLEAN) {
                    printf("Line %d, col %d: Incompatible type %s in while statement\n",
                           c->node->line, c->node->col, jtype_to_string(cond_type));
                }
            }
            c = c && c->next ? c->next : NULL;
            if (c && c->node) check_statement(c->node, method, ct);
            break;

        case N_Return:
            c = n->children;
            if (c) c = c->next;
            if (c && c->node) {
                JType ret_type = infer_type(c->node, method, ct);
                if (ret_type == JT_VOID) {
                    /* Returning a void expression is always an error */
                    printf("Line %d, col %d: Incompatible type void in return statement\n",
                           c->node->line, c->node->col);
                } else if (method && !types_compatible(ret_type, method->return_type)) {
                    printf("Line %d, col %d: Incompatible type %s in return statement\n",
                           c->node->line, c->node->col, jtype_to_string(ret_type));
                }
            } else {
                /* No return value (void return) */
                if (method && method->return_type != JT_VOID) {
                    printf("Line %d, col %d: Incompatible type void in return statement\n",
                           n->line, n->col);
                }
            }
            break;

        case N_Print:
            c = n->children;
            if (c) c = c->next;
            if (c && c->node) {
                JType arg_type = infer_type(c->node, method, ct);
                if (arg_type == JT_UNDEF || arg_type == JT_VOID || arg_type == JT_STRING_ARRAY) {
                    printf("Line %d, col %d: Incompatible type %s in System.out.print statement\n",
                           c->node->line, c->node->col, jtype_to_string(arg_type));
                }
            }
            break;

        case N_Assign:
        case N_Call:
        case N_ParseArgs:
            infer_type(n, method, ct);
            break;

        case N_Block:
            c = n->children;
            if (c) c = c->next;
            for (; c && c->node; c = c->next) {
                check_statement(c->node, method, ct);
            }
            break;

        default:
            break;
    }
}

void check_and_annotate_ast(struct node *program, ClassTable *ct)
{
    if (!program || !ct) return;

    struct node_list *c = program->children;
    if (c) c = c->next;
    c = c && c->next ? c->next : NULL;

    /* Track which MethodEntry instances have been processed to skip duplicates */
    MethodEntry *processed[256];
    int n_processed = 0;

    for (; c && c->node; c = c->next) {
        if (c->node->category == N_MethodDecl) {
            struct node_list *md = c->node->children;
            if (md) md = md->next;
            if (md && md->node && md->node->category == N_MethodHeader) {
                JType ret_type;
                char *method_name;
                JType *param_types;
                int n_params;

                process_method_header(md->node, &ret_type, &method_name, &param_types, &n_params);
                MethodEntry *method = find_exact_method(ct->methods, method_name, n_params, param_types);

                /* Check if this method was already processed (duplicate) */
                int already_processed = 0;
                if (method) {
                    for (int i = 0; i < n_processed; i++) {
                        if (processed[i] == method) { already_processed = 1; break; }
                    }
                }

                if (method && !already_processed && md->next && md->next->node &&
                    md->next->node->category == N_MethodBody) {
                    if (n_processed < 256) processed[n_processed++] = method;
                    struct node_list *body = md->next->node->children;
                    if (body) body = body->next;  /* Skip sentinel */
                    for (; body && body->node; body = body->next) {
                        check_statement(body->node, method, ct);
                    }
                }

                if (param_types) free(param_types);
            }
        }
    }
}
