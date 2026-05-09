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

int sem_errs = 0;

extern const char *category_name[];

static int check_natural_bounds(const char *text) {
    /* Check if integer literal is within int range (0 to 2147483647) */
    char clean[32];
    int j = 0;
    for (int i = 0; text[i] && j < 20; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
        }
    }
    clean[j] = '\0';

    if (j > 10) return 0;
    if (j == 10 && strcmp(clean, "2147483647") > 0) return 0;
    return 1;
}

static int check_decimal_bounds(const char *text) {
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
    if (val == HUGE_VAL || val == -HUGE_VAL) return 0;
    if (val == 0.0 && has_digits) {
        for (int i = 0; clean[i]; i++) {
            if (clean[i] >= '1' && clean[i] <= '9') return 0;
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
    if (!name) return 0;
    return (strcmp(name, "_") == 0) || (strcmp(name, "$") == 0);
}

static Symbol *find_symbol(Symbol *symbols, const char *name)
{
    if (!name) return NULL;
    for (Symbol *s = symbols; s; s = s->next) {
        if (s->name && strcmp(s->name, name) == 0) return s;
    }
    return NULL;
}

static void add_symbol(Symbol **symbols, const char *name, JType type, int is_param, int line, int col)
{
    if (!name) return;
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

static int types_compatible(JType actual, JType expected)
{
    if (actual == expected) return 1;
    if (actual == JT_INT && expected == JT_DOUBLE) return 1;
    return 0;
}

static MethodEntry *find_exact_method(MethodEntry *methods, const char *name, int n_params,
                                       JType *arg_types)
{
    if (!name) return NULL;
    for (MethodEntry *m = methods; m; m = m->next) {
        if (m->name && strcmp(m->name, name) == 0 && m->n_params == n_params) {
            int exact = 1;
            for (int i = 0; i < n_params; i++) {
                if (!arg_types || arg_types[i] != m->param_types[i]) {
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
    MethodEntry *first_match = NULL;
    int match_count = 0;
    if (!name) { *out_count = 0; return NULL; }

    for (MethodEntry *m = methods; m; m = m->next) {
        if (m->name && strcmp(m->name, name) == 0 && m->n_params == n_params) {
            int compatible = 1;
            for (int i = 0; i < n_params; i++) {
                if (!arg_types || !types_compatible(arg_types[i], m->param_types[i])) {
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
    *ret_type = JT_UNDEF; *method_name = NULL; *param_types = NULL; *n_params = 0;
    if (!header || header->category != N_MethodHeader) return;
    struct node_list *c = header->children;
    if (c) c = c->next;
    if (!c || !c->node) return;
    *ret_type = node_to_jtype(c->node);
    if (c->node->category == N_Void) *ret_type = JT_VOID;
    c = c->next;
    if (c && c->node && c->node->category == N_Identifier) *method_name = c->node->token;
    if (c) c = c->next;
    if (c && c->node && c->node->category == N_MethodParams) {
        struct node_list *pc = c->node->children;
        if (pc) pc = pc->next;
        int count = 0;
        for (struct node_list *tmp = pc; tmp && tmp->node; tmp = tmp->next) if (tmp->node->category == N_ParamDecl) count++;
        if (count > 0) {
            *param_types = malloc(count * sizeof(JType));
            int idx = 0;
            for (pc = c->node->children; pc; pc = pc->next) {
                if (pc->node && pc->node->category == N_ParamDecl) {
                    struct node_list *pd = pc->node->children;
                    if (pd) pd = pd->next;
                    if (pd && pd->node) (*param_types)[idx++] = extract_param_type(pd->node);
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
    ct->name = NULL; ct->fields = NULL; ct->methods = NULL; ct->entries = NULL;
    struct node_list *c = program->children;
    if (c) c = c->next;
    if (c && c->node && c->node->category == N_Identifier) {
        if (c->node->token) {
            ct->name = malloc(strlen(c->node->token) + 1);
            strcpy(ct->name, c->node->token);
        }
    }

    for (c = c->next; c; c = c->next) {
        if (!c->node) continue;
        if (c->node->category == N_FieldDecl) {
            struct node_list *fd = c->node->children;
            if (fd) fd = fd->next;
            JType type = JT_UNDEF;
            if (fd && fd->node) { type = node_to_jtype(fd->node); fd = fd->next; }
            for (; fd && fd->node; fd = fd->next) {
                const char *id = fd->node->token;
                int line = fd->node->line, col = fd->node->col;
                if (is_reserved_id(id)) { printf("Line %d, col %d: Symbol %s is reserved\n", line, col, id); sem_errs++; }
                else if (find_symbol(ct->fields, id)) { printf("Line %d, col %d: Symbol %s already defined\n", line, col, id); sem_errs++; }
                else if (id) {
                    add_symbol(&ct->fields, id, type, 0, line, col);
                    Symbol *added = find_symbol(ct->fields, id);
                    if (added) add_class_entry(ct, CE_FIELD, added, NULL);
                }
            }
        }
        else if (c->node->category == N_MethodDecl) {
            struct node_list *md = c->node->children;
            if (md) md = md->next;
            if (md && md->node && md->node->category == N_MethodHeader) {
                JType ret_type; char *method_name; JType *param_types; int n_params;
                process_method_header(md->node, &ret_type, &method_name, &param_types, &n_params);
                if (method_name) {
                    int method_line = md->node->line, method_col = md->node->col;
                    struct node_list *hdr_c = md->node->children;
                    if (hdr_c) hdr_c = hdr_c->next;
                    if (hdr_c) hdr_c = hdr_c->next;
                    if (hdr_c && hdr_c->node && hdr_c->node->category == N_Identifier) { method_line = hdr_c->node->line; method_col = hdr_c->node->col; }
                    int reserved_method = is_reserved_id(method_name);
                    int is_duplicate = (find_exact_method(ct->methods, method_name, n_params, param_types) != NULL);
                    MethodEntry *method = NULL;
                    if (!reserved_method && !is_duplicate) {
                        method = add_method(&ct->methods, method_name, ret_type, param_types, n_params, method_line, method_col);
                        if (method) add_class_entry(ct, CE_METHOD, NULL, method);
                    }
                    if (method) {
                            add_symbol(&method->symbols, "return", ret_type, 0, method_line, method_col);
                            struct node_list *params = md->node->children;
                            if (params) params = params->next;
                            if (params) params = params->next;
                            if (params) params = params->next;
                            if (params && params->node && params->node->category == N_MethodParams) {
                                struct node_list *pc = params->node->children;
                                if (pc) pc = pc->next;
                                for (; pc; pc = pc->next) {
                                    if (pc->node && pc->node->category == N_ParamDecl) {
                                        struct node_list *pd = pc->node->children;
                                        if (pd) pd = pd->next;
                                        if (pd && pd->node && pd->next && pd->next->node) {
                                            JType ptype = extract_param_type(pd->node);
                                            const char *pname = pd->next->node->token;
                                            int pline = pd->next->node->line, pcol = pd->next->node->col;
                                            if (is_reserved_id(pname)) { printf("Line %d, col %d: Symbol %s is reserved\n", pline, pcol, pname); sem_errs++; }
                                            else if (find_symbol(method->symbols, pname)) { printf("Line %d, col %d: Symbol %s already defined\n", pline, pcol, pname); sem_errs++; }
                                            else { add_symbol(&method->symbols, pname, ptype, 1, pline, pcol); }
                                        }
                                    }
                                }
                            }
                        } else if (reserved_method || is_duplicate) {
                            Symbol *temp_symbols = NULL;
                            add_symbol(&temp_symbols, "return", ret_type, 0, method_line, method_col);
                            struct node_list *params = md->node->children;
                            if (params) params = params->next;
                            if (params) params = params->next;
                            if (params) params = params->next;
                            if (params && params->node && params->node->category == N_MethodParams) {
                                struct node_list *pc = params->node->children;
                                if (pc) pc = pc->next;
                                for (; pc; pc = pc->next) {
                                    if (pc->node && pc->node->category == N_ParamDecl) {
                                        struct node_list *pd = pc->node->children;
                                        if (pd) pd = pd->next;
                                        if (pd && pd->node && pd->next && pd->next->node) {
                                            JType ptype = extract_param_type(pd->node);
                                            const char *pname = pd->next->node->token;
                                            int pline = pd->next->node->line, pcol = pd->next->node->col;
                                            if (is_reserved_id(pname)) { printf("Line %d, col %d: Symbol %s is reserved\n", pline, pcol, pname); sem_errs++; }
                                            else if (find_symbol(temp_symbols, pname)) { printf("Line %d, col %d: Symbol %s already defined\n", pline, pcol, pname); sem_errs++; }
                                            else { add_symbol(&temp_symbols, pname, ptype, 1, pline, pcol); }
                                        }
                                    }
                                }
                            }
                            while (temp_symbols) { Symbol *tmp = temp_symbols; temp_symbols = temp_symbols->next; free(tmp->name); free(tmp); }
                            if (reserved_method) { printf("Line %d, col %d: Symbol %s is reserved\n", method_line, method_col, method_name); sem_errs++; }
                            else if (is_duplicate) {
                                printf("Line %d, col %d: Symbol %s(", method_line, method_col, method_name);
                                for (int i = 0; i < n_params; i++) { if (i > 0) printf(","); printf("%s", jtype_to_string(param_types[i])); }
                                printf(") already defined\n"); sem_errs++;
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
        if (e->kind == CE_FIELD) printf("%s\t\t%s\n", e->field->name, jtype_to_string(e->field->type));
        else {
            MethodEntry *m = e->method; printf("%s\t(", m->name);
            for (int i = 0; i < m->n_params; i++) { if (i > 0) printf(","); printf("%s", jtype_to_string(m->param_types[i])); }
            printf(")\t%s\n", jtype_to_string(m->return_type));
        }
    }
    printf("\n");
    for (MethodEntry *m = ct->methods; m; m = m->next) {
        printf("===== Method %s(", m->name);
        for (int i = 0; i < m->n_params; i++) { if (i > 0) printf(","); printf("%s", jtype_to_string(m->param_types[i])); }
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
    while (f) { Symbol *tmp = f; f = f->next; free(tmp->name); free(tmp); }
    MethodEntry *m = ct->methods;
    while (m) {
        MethodEntry *tmp = m; m = m->next;
        Symbol *s = tmp->symbols;
        while (s) { Symbol *stmp = s; s = s->next; free(stmp->name); free(stmp); }
        if (tmp->param_types) free(tmp->param_types);
        free(tmp->name); free(tmp);
    }
    ClassEntryNode *en = ct->entries;
    while (en) { ClassEntryNode *tmp = en; en = en->next; free(tmp); }
    if (ct->name) free(ct->name);
    free(ct);
}

static Symbol *lookup_symbol(const char *name, MethodEntry *method, ClassTable *ct)
{
    if (!name || !method || !ct) return NULL;
    Symbol *s = find_symbol(method->symbols, name);
    if (s) return s;
    s = find_symbol(ct->fields, name);
    if (s) return s;
    return NULL;
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
            if ((left_type == JT_INT || left_type == JT_DOUBLE) && (right_type == JT_INT || right_type == JT_DOUBLE))
                return (left_type == JT_DOUBLE || right_type == JT_DOUBLE) ? JT_DOUBLE : JT_INT;
            if (left_type != JT_UNDEF && right_type != JT_UNDEF) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                       n->line, n->col, n->category == N_Add ? "+" : n->category == N_Sub ? "-" : n->category == N_Mul ? "*" : n->category == N_Div ? "/" : "%", left_str, right_str);
                sem_errs++;
            }
            return JT_UNDEF;
        case N_Lt: case N_Gt: case N_Le: case N_Ge:
            if ((left_type == JT_INT || left_type == JT_DOUBLE) && (right_type == JT_INT || right_type == JT_DOUBLE)) return JT_BOOLEAN;
            if (left_type != JT_UNDEF && right_type != JT_UNDEF) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                       n->line, n->col, n->category == N_Lt ? "<" : n->category == N_Gt ? ">" : n->category == N_Le ? "<=" : ">=", left_str, right_str);
                sem_errs++;
            }
            return JT_BOOLEAN;
        case N_Eq: case N_Ne: {
            int valid = 0;
            if (left_type != JT_UNDEF && right_type != JT_UNDEF && left_type != JT_VOID && right_type != JT_VOID && left_type != JT_STRING_ARRAY && right_type != JT_STRING_ARRAY) {
                if (left_type == right_type) valid = 1;
                else if ((left_type == JT_INT && right_type == JT_DOUBLE) || (left_type == JT_DOUBLE && right_type == JT_INT)) valid = 1;
            }
            if (!valid && left_type != JT_UNDEF && right_type != JT_UNDEF) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", n->line, n->col, n->category == N_Eq ? "==" : "!=", left_str, right_str);
                sem_errs++;
            }
            return JT_BOOLEAN;
        }
        case N_And: case N_Or:
            if (left_type == JT_BOOLEAN && right_type == JT_BOOLEAN) return JT_BOOLEAN;
            if (left_type != JT_UNDEF && right_type != JT_UNDEF) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", n->line, n->col, n->category == N_And ? "&&" : "||", left_str, right_str);
                sem_errs++;
            }
            return JT_BOOLEAN;
        case N_Lshift: case N_Rshift:
            if (left_type == JT_INT && right_type == JT_INT) return JT_INT;
            if (left_type != JT_UNDEF && right_type != JT_UNDEF) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", n->line, n->col, n->category == N_Lshift ? "<<" : ">>", left_str, right_str);
                sem_errs++;
            }
            return JT_UNDEF;
        case N_Xor:
            if (left_type == JT_INT && right_type == JT_INT) return JT_INT;
            if (left_type == JT_BOOLEAN && right_type == JT_BOOLEAN) return JT_BOOLEAN;
            if (left_type != JT_UNDEF && right_type != JT_UNDEF) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", n->line, n->col, "^", left_str, right_str);
                sem_errs++;
            }
            return JT_UNDEF;
        default: return JT_UNDEF;
    }
}

static JType check_unary_op(struct node *n, MethodEntry *method, ClassTable *ct)
{
    struct node_list *c = n->children;
    if (c) c = c->next;
    struct node *operand = c && c->node ? c->node : NULL;
    JType op_type = operand ? infer_type(operand, method, ct) : JT_UNDEF;
    char op_str[32]; strcpy(op_str, jtype_to_string(op_type));
    switch (n->category) {
        case N_Not:
            if (op_type == JT_BOOLEAN) return JT_BOOLEAN;
            if (op_type != JT_UNDEF) { printf("Line %d, col %d: Operator ! cannot be applied to type %s\n", n->line, n->col, op_str); sem_errs++; }
            return JT_BOOLEAN;
        case N_Minus: case N_Plus:
            if (op_type == JT_INT || op_type == JT_DOUBLE) return op_type;
            if (op_type != JT_UNDEF) { printf("Line %d, col %d: Operator %s cannot be applied to type %s\n", n->line, n->col, n->category == N_Minus ? "-" : "+", op_str); sem_errs++; }
            return JT_UNDEF;
        case N_Length:
            if (op_type == JT_STRING_ARRAY) return JT_INT;
            if (op_type != JT_UNDEF) { printf("Line %d, col %d: Operator .length cannot be applied to type %s\n", n->line, n->col, op_str); sem_errs++; }
            return JT_INT;
        default: return JT_UNDEF;
    }
}

static JType infer_type(struct node *n, MethodEntry *method, ClassTable *ct)
{
    if (!n) return JT_UNDEF;
    switch (n->category) {
        case N_Natural:
            if (n->token && !check_natural_bounds(n->token)) { printf("Line %d, col %d: Number %s out of bounds\n", n->line, n->col, n->token); sem_errs++; }
            n->type_annot = strdup("int"); return JT_INT;
        case N_Decimal:
            if (n->token && !check_decimal_bounds(n->token)) { printf("Line %d, col %d: Number %s out of bounds\n", n->line, n->col, n->token); sem_errs++; }
            n->type_annot = strdup("double"); return JT_DOUBLE;
        case N_BoolLit: n->type_annot = strdup("boolean"); return JT_BOOLEAN;
        case N_StrLit: n->type_annot = strdup("String"); return JT_STRING;
        case N_Identifier: {
            if (is_reserved_id(n->token)) { printf("Line %d, col %d: Symbol %s is reserved\n", n->line, n->col, n->token); sem_errs++; n->type_annot = strdup("undef"); return JT_UNDEF; }
            Symbol *s = lookup_symbol(n->token, method, ct);
            if (s) { n->type_annot = strdup(jtype_to_string(s->type)); return s->type; }
            else { printf("Line %d, col %d: Cannot find symbol %s\n", n->line, n->col, n->token); sem_errs++; n->type_annot = strdup("undef"); return JT_UNDEF; }
        }
        case N_Add: case N_Sub: case N_Mul: case N_Div: case N_Mod:
        case N_Lt: case N_Gt: case N_Le: case N_Ge:
        case N_Eq: case N_Ne: case N_And: case N_Or:
        case N_Lshift: case N_Rshift: case N_Xor: { JType res = check_binary_op(n, method, ct); n->type_annot = strdup(jtype_to_string(res)); return res; }
        case N_Not: case N_Minus: case N_Plus: case N_Length: { JType res = check_unary_op(n, method, ct); n->type_annot = strdup(jtype_to_string(res)); return res; }
        case N_Call: {
            struct node_list *c = n->children; if (c) c = c->next;
            struct node *method_id = c && c->node ? c->node : NULL; char *method_name = method_id ? method_id->token : NULL;
            if (method_name && is_reserved_id(method_name)) {
                printf("Line %d, col %d: Symbol %s is reserved\n", n->line, n->col, method_name); sem_errs++;
                if (method_id) method_id->type_annot = strdup("undef");
                n->type_annot = strdup("undef");
                c = c && c->next ? c->next : NULL; if (c && !c->node) c = c->next;
                for (struct node_list *args = c; args; args = args->next) if (args->node) infer_type(args->node, method, ct);
                return JT_UNDEF;
            }
            c = c && c->next ? c->next : NULL; if (c && !c->node) c = c->next;
            int n_args = 0; JType *arg_types = NULL;
            for (struct node_list *tmp = c; tmp; tmp = tmp->next) if (tmp->node) n_args++;
            if (n_args > 0) {
                arg_types = malloc(n_args * sizeof(JType)); int idx = 0;
                for (struct node_list *args = c; args; args = args->next) if (args->node) arg_types[idx++] = infer_type(args->node, method, ct);
            }
            MethodEntry *called = (n_args >= 0) ? find_exact_method(ct->methods, method_name, n_args, arg_types) : NULL;
            int ambiguous = 0;
            if (!called) {
                int match_count = 0; called = find_compatible_method(ct->methods, method_name, n_args, arg_types, &match_count);
                if (match_count > 1) {
                    printf("Line %d, col %d: Reference to method %s(", n->line, n->col, method_name ? method_name : "");
                    for (int i = 0; i < n_args; i++) { if (i > 0) printf(","); printf("%s", jtype_to_string(arg_types[i])); }
                    printf(") is ambiguous\n"); sem_errs++; ambiguous = 1; called = NULL;
                }
            }
            if (called) {
                if (method_id) {
                    int sig_len = 2; for (int i = 0; i < called->n_params; i++) { if (i > 0) sig_len++; sig_len += strlen(jtype_to_string(called->param_types[i])); }
                    char *sig = malloc(sig_len + 1); sig[0] = '('; sig[1] = '\0';
                    for (int i = 0; i < called->n_params; i++) { if (i > 0) strcat(sig, ","); strcat(sig, jtype_to_string(called->param_types[i])); }
                    strcat(sig, ")"); method_id->type_annot = sig;
                }
                n->type_annot = strdup(jtype_to_string(called->return_type));
                if (arg_types) free(arg_types);
                return called->return_type;
            } else if (!ambiguous) {
                printf("Line %d, col %d: Cannot find symbol %s(", n->line, n->col, method_name ? method_name : "");
                for (int i = 0; i < n_args; i++) { if (i > 0) printf(","); printf("%s", jtype_to_string(arg_types[i])); }
                printf(")\n"); sem_errs++;
                if (method_id) method_id->type_annot = strdup("undef");
                n->type_annot = strdup("undef");
                if (arg_types) free(arg_types);
                return JT_UNDEF;
            } else {
                if (method_id) method_id->type_annot = strdup("undef");
                n->type_annot = strdup("undef");
                if (arg_types) free(arg_types);
                return JT_UNDEF;
            }
        }
        case N_ParseArgs: {
            struct node_list *c = n->children; if (c) c = c->next;
            JType arg1 = c && c->node ? infer_type(c->node, method, ct) : JT_UNDEF;
            c = c && c->next ? c->next : NULL;
            JType arg2 = c && c->node ? infer_type(c->node, method, ct) : JT_UNDEF;
            if (arg1 != JT_UNDEF && arg2 != JT_UNDEF && (arg1 != JT_STRING_ARRAY || arg2 != JT_INT)) {
                printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n", n->line, n->col, jtype_to_string(arg1), jtype_to_string(arg2));
                sem_errs++;
            }
            n->type_annot = strdup("int"); return JT_INT;
        }
        case N_Assign: {
            struct node_list *c = n->children; if (c) c = c->next;
            struct node *lhs = c && c->node ? c->node : NULL;
            struct node *rhs = c && c->next && c->next->node ? c->next->node : NULL;
            JType lt = lhs ? infer_type(lhs, method, ct) : JT_UNDEF;
            JType rt = rhs ? infer_type(rhs, method, ct) : JT_UNDEF;
            int is_p = 0;
            if (lhs && lhs->category == N_Identifier) {
                Symbol *ls = lookup_symbol(lhs->token, method, ct);
                if (ls && ls->is_param && ls->type == JT_STRING_ARRAY) {
                    is_p = 1; printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n", n->line, n->col, jtype_to_string(lt), jtype_to_string(rt)); sem_errs++;
                }
            }
            if (!is_p && lt != JT_UNDEF && rt != JT_UNDEF && !types_compatible(rt, lt)) {
                printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n", n->line, n->col, jtype_to_string(lt), jtype_to_string(rt)); sem_errs++;
            }
            n->type_annot = strdup(jtype_to_string(lt)); return lt;
        }
        default: return JT_UNDEF;
    }
}

static void check_statement(struct node *n, MethodEntry *method, ClassTable *ct)
{
    if (!n) return;
    struct node_list *c;
    switch (n->category) {
        case N_VarDecl:
            c = n->children; if (c) c = c->next;
            JType vt = (c && c->node) ? node_to_jtype(c->node) : JT_UNDEF;
            if (c) c = c->next;
            for (; c && c->node; c = c->next) {
                if (is_reserved_id(c->node->token)) { printf("Line %d, col %d: Symbol %s is reserved\n", c->node->line, c->node->col, c->node->token); sem_errs++; }
                else if (find_symbol(method->symbols, c->node->token)) { printf("Line %d, col %d: Symbol %s already defined\n", c->node->line, c->node->col, c->node->token); sem_errs++; }
                else add_symbol(&method->symbols, c->node->token, vt, 0, c->node->line, c->node->col);
            }
            break;
        case N_If:
            c = n->children; if (c) c = c->next;
            if (c && c->node) { JType cond_t = infer_type(c->node, method, ct); if (cond_t != JT_BOOLEAN && cond_t != JT_UNDEF) { printf("Line %d, col %d: Incompatible type %s in if statement\n", c->node->line, c->node->col, jtype_to_string(cond_t)); sem_errs++; } }
            c = c && c->next ? c->next : NULL; if (c && c->node) check_statement(c->node, method, ct);
            c = c && c->next ? c->next : NULL; if (c && c->node) check_statement(c->node, method, ct);
            break;
        case N_While:
            c = n->children; if (c) c = c->next;
            if (c && c->node) { JType cond_t = infer_type(c->node, method, ct); if (cond_t != JT_BOOLEAN && cond_t != JT_UNDEF) { printf("Line %d, col %d: Incompatible type %s in while statement\n", c->node->line, c->node->col, jtype_to_string(cond_t)); sem_errs++; } }
            c = c && c->next ? c->next : NULL; if (c && c->node) check_statement(c->node, method, ct);
            break;
        case N_Return:
            c = n->children; if (c) c = c->next;
            if (c && c->node) {
                JType rt = infer_type(c->node, method, ct);
                if (rt == JT_VOID) { printf("Line %d, col %d: Incompatible type void in return statement\n", c->node->line, c->node->col); sem_errs++; }
                else if (rt != JT_UNDEF && method && !types_compatible(rt, method->return_type)) { printf("Line %d, col %d: Incompatible type %s in return statement\n", c->node->line, c->node->col, jtype_to_string(rt)); sem_errs++; }
            } else if (method && method->return_type != JT_VOID) { printf("Line %d, col %d: Incompatible type void in return statement\n", n->line, n->col); sem_errs++; }
            break;
        case N_Print:
            c = n->children; if (c) c = c->next;
            if (c && c->node) { JType at = infer_type(c->node, method, ct); if (at != JT_UNDEF && (at == JT_VOID || at == JT_STRING_ARRAY)) { printf("Line %d, col %d: Incompatible type %s in System.out.print statement\n", c->node->line, c->node->col, jtype_to_string(at)); sem_errs++; } }
            break;
        case N_Assign: case N_Call: case N_ParseArgs: infer_type(n, method, ct); break;
        case N_Block:
            c = n->children; if (c) c = c->next;
            for (; c && c->node; c = c->next) check_statement(c->node, method, ct);
            break;
        default: break;
    }
}

void check_and_annotate_ast(struct node *program, ClassTable *ct)
{
    if (!program || !ct) return;
    struct node_list *c = program->children; if (c) c = c->next;
    c = c && c->next ? c->next : NULL;
    MethodEntry *processed[256]; int n_processed = 0;
    for (; c && c->node; c = c->next) {
        if (c->node->category == N_MethodDecl) {
            struct node_list *md = c->node->children; if (md) md = md->next;
            if (md && md->node && md->node->category == N_MethodHeader) {
                JType rt; char *mn; JType *pts; int np; process_method_header(md->node, &rt, &mn, &pts, &np);
                MethodEntry *method = find_exact_method(ct->methods, mn, np, pts);
                int done = 0; if (method) for (int i = 0; i < n_processed; i++) if (processed[i] == method) { done = 1; break; }
                if (method && !done && md->next && md->next->node && md->next->node->category == N_MethodBody) {
                    if (n_processed < 256) processed[n_processed++] = method;
                    struct node_list *body = md->next->node->children; if (body) body = body->next;
                    for (; body && body->node; body = body->next) check_statement(body->node, method, ct);
                }
                if (pts) free(pts);
            }
        }
    }
}
