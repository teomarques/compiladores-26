/* semantics.c - Semantic Analysis */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <errno.h>
#include "semantics.h"

static SymTable *tables = NULL;      /* linked list of all symbol tables */
static SymTable *tables_last = NULL;
static SymTable *class_table = NULL; /* pointer to the class table */
static SymTable *current_method = NULL;
static SymTable *active_locals = NULL; /* vars visible so far in current method */
static int sem_errors = 0;

/* Helper: get the type string for a type node */
static const char *type_node_to_str(ASTNode *node) {
    if (!node) return "undef";
    switch (node->type) {
        case node_Int: return "int";
        case node_Double: return "double";
        case node_Bool: return "boolean";
        case node_Void: return "void";
        case node_StringArray: return "String[]";
        default: return "undef";
    }
}

/* Helper: add a table to the chain */
static void add_table(SymTable *t) {
    if (!tables) { tables = t; tables_last = t; }
    else { tables_last->next = t; tables_last = t; }
}

/* Helper: build param types string like "(int,double)" from MethodParams node */
static char *build_param_types(ASTNode *method_params) {
    char buf[1024] = "(";
    ASTNode *param = method_params ? method_params->child : NULL;
    int first = 1;
    while (param) {
        if (param->type == node_ParamDecl) {
            ASTNode *ptype = param->child;
            if (!first) strcat(buf, ",");
            strcat(buf, type_node_to_str(ptype));
            first = 0;
        }
        param = param->sibling;
    }
    strcat(buf, ")");
    return strdup(buf);
}

/* Helper: count params in a Call node (children after the identifier) */
static int count_call_args(ASTNode *call) {
    int count = 0;
    ASTNode *c = call->child;
    if (c) c = c->sibling; /* skip function identifier */
    while (c) { count++; c = c->sibling; }
    return count;
}

/* Helper: count params in a param_types string */
static int count_param_types(const char *pt) {
    if (!pt || strcmp(pt, "()") == 0) return 0;
    int count = 1;
    for (const char *p = pt; *p; p++)
        if (*p == ',') count++;
    return count;
}

/* Helper: parse i-th param type from "(int,double)" */
static char *get_param_type(const char *pt, int index) {
    if (!pt) return NULL;
    const char *start = pt + 1; /* skip '(' */
    int i = 0;
    while (*start && *start != ')') {
        const char *end = start;
        while (*end && *end != ',' && *end != ')') end++;
        if (i == index) {
            int len = end - start;
            char *result = (char *)malloc(len + 1);
            strncpy(result, start, len);
            result[len] = '\0';
            return result;
        }
        if (*end == ',') end++;
        start = end;
        i++;
    }
    return NULL;
}

/* Helper: check if type1 is compatible with type2 (type1 can be assigned/passed as type2) */
static int types_compatible(const char *actual, const char *formal) {
    if (!actual || !formal) return 0;
    if (strcmp(actual, "undef") == 0 || strcmp(formal, "undef") == 0) return 1;
    if (strcmp(actual, formal) == 0) return 1;
    /* int can be widened to double */
    if (strcmp(actual, "int") == 0 && strcmp(formal, "double") == 0) return 1;
    return 0;
}

/* Helper: is numeric type */
static int is_numeric(const char *type) {
    return type && (strcmp(type, "int") == 0 || strcmp(type, "double") == 0);
}

/* Forward declaration */
static const char *check_expr(ASTNode *node);

/* Phase 1: Build symbol tables */
static void build_tables(ASTNode *root) {
    if (!root || root->type != node_Program) return;

    /* Get class name */
    ASTNode *class_id = root->child; /* Identifier(ClassName) */
    char tname[256];
    snprintf(tname, sizeof(tname), "Class %s Symbol Table", class_id->value);
    class_table = symtab_create(tname);
    add_table(class_table);

    /* Process declarations */
    ASTNode *decl = class_id->sibling;
    while (decl) {
        if (decl->type == node_FieldDecl) {
            ASTNode *ftype = decl->child;
            ASTNode *fid = ftype->sibling;
            const char *tstr = type_node_to_str(ftype);

            if (strcmp(fid->value, "_") == 0) {
                printf("Line %d, col %d: Symbol _ is reserved\n", fid->line, fid->col);
                sem_errors++;
            } else if (symtab_add(class_table, fid->value, tstr, 0, NULL) != 0) {
                printf("Line %d, col %d: Symbol %s already defined\n", fid->line, fid->col, fid->value);
                sem_errors++;
            }
        }
        else if (decl->type == node_MethodDecl) {
            ASTNode *header = decl->child;
            ASTNode *ret_type_node = header->child;
            ASTNode *method_id = ret_type_node->sibling;
            ASTNode *method_params = method_id->sibling;

            const char *ret_type = type_node_to_str(ret_type_node);
            char *param_types = build_param_types(method_params);

            /* Add method to class table; skip duplicate */
            if (symtab_add(class_table, method_id->value, ret_type, 0, param_types) != 0) {
                printf("Line %d, col %d: Symbol %s%s already defined\n",
                       method_id->line, method_id->col, method_id->value, param_types);
                sem_errors++;
                decl->line = -1;
                free(param_types);
                decl = decl->sibling;
                continue;
            }

            /* Create method symbol table */
            char mtname[512];
            snprintf(mtname, sizeof(mtname), "Method %s%s Symbol Table", method_id->value, param_types);
            SymTable *method_table = symtab_create(mtname);
            add_table(method_table);

            /* Add return symbol */
            symtab_add(method_table, "return", ret_type, 0, NULL);

            /* Add parameters */
            ASTNode *param = method_params->child;
            while (param) {
                if (param->type == node_ParamDecl) {
                    ASTNode *ptype = param->child;
                    ASTNode *pid = ptype->sibling;
                    const char *ptstr = type_node_to_str(ptype);

                    if (strcmp(pid->value, "_") == 0) {
                        printf("Line %d, col %d: Symbol _ is reserved\n", pid->line, pid->col);
                        sem_errors++;
                    } else if (symtab_add(method_table, pid->value, ptstr, 1, NULL) != 0) {
                        printf("Line %d, col %d: Symbol %s already defined\n", pid->line, pid->col, pid->value);
                        sem_errors++;
                    }
                }
                param = param->sibling;
            }

            free(param_types);
        }
        decl = decl->sibling;
    }
}

/* Find the method table for a given method (by name and param_types) */
static SymTable *find_method_table(const char *method_name, const char *param_types) {
    char prefix[512];
    snprintf(prefix, sizeof(prefix), "Method %s%s Symbol Table", method_name, param_types);
    SymTable *t = tables;
    while (t) {
        if (strcmp(t->name, prefix) == 0) return t;
        t = t->next;
    }
    return NULL;
}

/* Resolve a method call. Returns the return type or "undef" */
static const char *resolve_method(ASTNode *call_node) {
    ASTNode *id_node = call_node->child;
    const char *name = id_node->value;
    int nargs = count_call_args(call_node);

    /* Collect argument types */
    const char *arg_types[64];
    ASTNode *arg = id_node->sibling;
    int ai = 0;
    while (arg && ai < 64) {
        arg_types[ai++] = check_expr(arg);
        arg = arg->sibling;
    }

    /* Find all methods with this name */
    Symbol *candidates[64];
    int ncand = symtab_find_methods(class_table, name, candidates, 64);

    if (ncand == 0) {
        char argbuf[512] = "(";
        for (int i = 0; i < nargs; i++) {
            if (i > 0) strcat(argbuf, ",");
            strcat(argbuf, arg_types[i] ? arg_types[i] : "undef");
        }
        strcat(argbuf, ")");
        printf("Line %d, col %d: Cannot find symbol %s%s\n", id_node->line, id_node->col, name, argbuf);
        sem_errors++;
        id_node->annotation = strdup("undef");
        return "undef";
    }

    /* Step 1: exact match */
    for (int i = 0; i < ncand; i++) {
        int nparams = count_param_types(candidates[i]->param_types);
        if (nparams != nargs) continue;
        int exact = 1;
        for (int j = 0; j < nargs; j++) {
            char *pt = get_param_type(candidates[i]->param_types, j);
            if (!pt || strcmp(arg_types[j], pt) != 0) exact = 0;
            free(pt);
            if (!exact) break;
        }
        if (exact) {
            id_node->annotation = strdup(candidates[i]->param_types);
            return candidates[i]->type;
        }
    }

    /* Step 2: compatible match */
    Symbol *compatible[64];
    int ncompat = 0;
    for (int i = 0; i < ncand; i++) {
        int nparams = count_param_types(candidates[i]->param_types);
        if (nparams != nargs) continue;
        int compat = 1;
        for (int j = 0; j < nargs; j++) {
            char *pt = get_param_type(candidates[i]->param_types, j);
            if (!pt || !types_compatible(arg_types[j], pt)) compat = 0;
            free(pt);
            if (!compat) break;
        }
        if (compat) compatible[ncompat++] = candidates[i];
    }

    if (ncompat == 1) {
        id_node->annotation = strdup(compatible[0]->param_types);
        return compatible[0]->type;
    } else if (ncompat > 1) {
        char sig[512];
        snprintf(sig, sizeof(sig), "%s(", name);
        for (int i = 0; i < nargs; i++) {
            if (i > 0) strcat(sig, ",");
            strcat(sig, arg_types[i] ? arg_types[i] : "undef");
        }
        strcat(sig, ")");
        printf("Line %d, col %d: Reference to method %s is ambiguous\n",
               call_node->child->line, call_node->child->col, sig);
        sem_errors++;
        id_node->annotation = strdup("undef");
        return "undef";
    } else {
        char argbuf[512] = "(";
        for (int i = 0; i < nargs; i++) {
            if (i > 0) strcat(argbuf, ",");
            strcat(argbuf, arg_types[i] ? arg_types[i] : "undef");
        }
        strcat(argbuf, ")");
        printf("Line %d, col %d: Cannot find symbol %s%s\n", id_node->line, id_node->col, name, argbuf);
        sem_errors++;
        id_node->annotation = strdup("undef");
        return "undef";
    }
}

/* Look up an identifier: active_locals (vars declared so far) + params/return in current_method + class table */
static Symbol *lookup_symbol(const char *name) {
    if (active_locals) {
        Symbol *s = symtab_lookup(active_locals, name);
        if (s && !s->param_types) return s;
    }
    if (current_method) {
        Symbol *s = current_method->symbols;
        while (s) {
            if (strcmp(s->name, name) == 0 && s->is_param) return s;
            if (strcmp(s->name, name) == 0 && strcmp(s->name, "return") == 0) return s;
            s = s->next;
        }
    }
    Symbol *s = class_table->symbols;
    while (s) {
        if (strcmp(s->name, name) == 0 && !s->param_types) return s;
        s = s->next;
    }
    return NULL;
}

/* Phase 2: Type check expressions. Returns the type string. */
static const char *check_expr(ASTNode *node) {
    if (!node) return "undef";

    const char *result_type = "undef";

    switch (node->type) {
    case node_Natural: {
        /* Check bounds: must be <= 2147483647. 2147483648 is ALWAYS out of bounds */
        errno = 0;
        /* Remove underscores for parsing */
        char clean[256];
        int ci = 0;
        for (int i = 0; node->value[i]; i++)
            if (node->value[i] != '_') clean[ci++] = node->value[i];
        clean[ci] = '\0';
        long long val = strtoll(clean, NULL, 10);
        if (val > 2147483647 || errno == ERANGE) {
            printf("Line %d, col %d: Number %s out of bounds\n", node->line, node->col, node->value);
            sem_errors++;
        }
        result_type = "int";
        break;
    }
    case node_Decimal: {
        /* Check bounds */
        char clean[512];
        int ci = 0;
        for (int i = 0; node->value[i]; i++)
            if (node->value[i] != '_') clean[ci++] = node->value[i];
        clean[ci] = '\0';
        errno = 0;
        double val = strtod(clean, NULL);
        if (errno == ERANGE && val != 0.0) {
            printf("Line %d, col %d: Number %s out of bounds\n", node->line, node->col, node->value);
            sem_errors++;
        }
        result_type = "double";
        break;
    }
    case node_BoolLit:
        result_type = "boolean";
        break;

    case node_StrLit:
        result_type = "String";
        break;

    case node_Identifier: {
        Symbol *s = lookup_symbol(node->value);
        if (!s) {
            printf("Line %d, col %d: Cannot find symbol %s\n", node->line, node->col, node->value);
            sem_errors++;
            result_type = "undef";
        } else {
            result_type = s->type;
        }
        break;
    }

    case node_Add: case node_Sub: case node_Mul: case node_Div: {
        const char *left = check_expr(node->child);
        const char *right = check_expr(node->child->sibling);
        static const char *ops[] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                                     NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                                     "+","-","*","/"};
        const char *op = ops[node->type];
        if (is_numeric(left) && is_numeric(right)) {
            if (strcmp(left, "double") == 0 || strcmp(right, "double") == 0)
                result_type = "double";
            else
                result_type = "int";
        } else {
            if (strcmp(left, "undef") != 0 || strcmp(right, "undef") != 0) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                       node->line, node->col, op, left, right);
                sem_errors++;
            }
            result_type = "undef";
        }
        break;
    }

    case node_Mod: {
        const char *left = check_expr(node->child);
        const char *right = check_expr(node->child->sibling);
        if (is_numeric(left) && is_numeric(right)) {
            if (strcmp(left, "double") == 0 || strcmp(right, "double") == 0)
                result_type = "double";
            else
                result_type = "int";
        } else {
            if (strcmp(left, "undef") != 0 || strcmp(right, "undef") != 0) {
                printf("Line %d, col %d: Operator %% cannot be applied to types %s, %s\n",
                       node->line, node->col, left, right);
                sem_errors++;
            }
            result_type = "undef";
        }
        break;
    }
    case node_Lshift: case node_Rshift: case node_Xor: {
        const char *left = check_expr(node->child);
        const char *right = check_expr(node->child->sibling);
        const char *op;
        switch (node->type) {
            case node_Lshift: op = "<<"; break;
            case node_Rshift: op = ">>"; break;
            case node_Xor: op = "^"; break;
            default: op = "?";
        }
        if (strcmp(left, "int") == 0 && strcmp(right, "int") == 0) {
            result_type = "int";
        } else if (node->type == node_Xor &&
                   strcmp(left, "boolean") == 0 && strcmp(right, "boolean") == 0) {
            result_type = "boolean";
        } else {
            if (strcmp(left, "undef") != 0 || strcmp(right, "undef") != 0) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                       node->line, node->col, op, left, right);
                sem_errors++;
            }
            result_type = "undef";
        }
        break;
    }

    case node_And: case node_Or: {
        const char *left = check_expr(node->child);
        const char *right = check_expr(node->child->sibling);
        const char *op = (node->type == node_And) ? "&&" : "||";
        if (strcmp(left, "boolean") == 0 && strcmp(right, "boolean") == 0) {
            result_type = "boolean";
        } else {
            if (strcmp(left, "undef") != 0 || strcmp(right, "undef") != 0) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                       node->line, node->col, op, left, right);
                sem_errors++;
            }
            result_type = "undef";
        }
        break;
    }

    case node_Eq: case node_Ne: {
        const char *left = check_expr(node->child);
        const char *right = check_expr(node->child->sibling);
        const char *op = (node->type == node_Eq) ? "==" : "!=";
        if ((is_numeric(left) && is_numeric(right)) ||
            (strcmp(left, "boolean") == 0 && strcmp(right, "boolean") == 0)) {
            result_type = "boolean";
        } else {
            if (strcmp(left, "undef") != 0 || strcmp(right, "undef") != 0) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                       node->line, node->col, op, left, right);
                sem_errors++;
            }
            result_type = "undef";
        }
        break;
    }

    case node_Lt: case node_Gt: case node_Le: case node_Ge: {
        const char *left = check_expr(node->child);
        const char *right = check_expr(node->child->sibling);
        const char *op;
        switch (node->type) {
            case node_Lt: op = "<"; break;
            case node_Gt: op = ">"; break;
            case node_Le: op = "<="; break;
            case node_Ge: op = ">="; break;
            default: op = "?";
        }
        if (is_numeric(left) && is_numeric(right)) {
            result_type = "boolean";
        } else {
            if (strcmp(left, "undef") != 0 || strcmp(right, "undef") != 0) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                       node->line, node->col, op, left, right);
                sem_errors++;
            }
            result_type = "undef";
        }
        break;
    }

    case node_Not: {
        const char *operand = check_expr(node->child);
        if (strcmp(operand, "boolean") == 0) {
            result_type = "boolean";
        } else {
            if (strcmp(operand, "undef") != 0) {
                printf("Line %d, col %d: Operator ! cannot be applied to type %s\n",
                       node->line, node->col, operand);
                sem_errors++;
            }
            result_type = "undef";
        }
        break;
    }

    case node_Minus: case node_Plus: {
        const char *operand = check_expr(node->child);
        const char *op = (node->type == node_Minus) ? "-" : "+";
        if (is_numeric(operand)) {
            result_type = operand;
        } else {
            if (strcmp(operand, "undef") != 0) {
                printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",
                       node->line, node->col, op, operand);
                sem_errors++;
            }
            result_type = "undef";
        }
        break;
    }

    case node_Length: {
        const char *operand = check_expr(node->child);
        if (strcmp(operand, "String[]") == 0) {
            result_type = "int";
        } else {
            if (strcmp(operand, "undef") != 0) {
                printf("Line %d, col %d: Operator .length cannot be applied to type %s\n",
                       node->line, node->col, operand);
                sem_errors++;
            }
            result_type = "undef";
        }
        break;
    }

    case node_Assign: {
        const char *left = check_expr(node->child);
        const char *right = check_expr(node->child->sibling);
        if (types_compatible(right, left) && strcmp(left, "undef") != 0 &&
            strcmp(left, "String[]") != 0 && strcmp(left, "void") != 0) {
            if (strcmp(left, "double") == 0 && strcmp(right, "int") == 0)
                result_type = "double";
            else
                result_type = left;
        } else {
            if (strcmp(left, "undef") != 0 || strcmp(right, "undef") != 0) {
                printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n",
                       node->line, node->col, left, right);
                sem_errors++;
            }
            result_type = "undef";
        }
        break;
    }

    case node_Call:
        result_type = resolve_method(node);
        break;

    case node_ParseArgs: {
        /* Check that the identifier is String[] */
        const char *id_type = check_expr(node->child);
        /* Check that the index expression is int */
        const char *idx_type = check_expr(node->child->sibling);
        if (strcmp(id_type, "String[]") != 0 && strcmp(id_type, "undef") != 0) {
            printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to type %s\n",
                   node->child->line, node->child->col, id_type);
            sem_errors++;
        }
        if (strcmp(idx_type, "int") != 0 && strcmp(idx_type, "undef") != 0) {
            printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to type %s\n",
                   node->child->sibling->line, node->child->sibling->col, idx_type);
            sem_errors++;
        }
        result_type = "int";
        break;
    }

    default:
        return "undef";
    }

    node->annotation = strdup(result_type);
    return result_type;
}

/* Phase 2: Check statements */
static void check_statement(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
    case node_If: {
        const char *cond_type = check_expr(node->child);
        if (strcmp(cond_type, "boolean") != 0 && strcmp(cond_type, "undef") != 0) {
            printf("Line %d, col %d: Incompatible type %s in if statement\n",
                   node->child->line, node->child->col, cond_type);
            sem_errors++;
        }
        check_statement(node->child->sibling);
        if (node->child->sibling->sibling)
            check_statement(node->child->sibling->sibling);
        break;
    }
    case node_While: {
        const char *cond_type = check_expr(node->child);
        if (strcmp(cond_type, "boolean") != 0 && strcmp(cond_type, "undef") != 0) {
            printf("Line %d, col %d: Incompatible type %s in while statement\n",
                   node->child->line, node->child->col, cond_type);
            sem_errors++;
        }
        check_statement(node->child->sibling);
        break;
    }
    case node_Return: {
        Symbol *ret = symtab_lookup(current_method, "return");
        if (node->child) {
            const char *expr_type = check_expr(node->child);
            if (ret && !types_compatible(expr_type, ret->type) && strcmp(expr_type, "undef") != 0) {
                printf("Line %d, col %d: Incompatible type %s in return statement\n",
                       node->child->line, node->child->col, expr_type);
                sem_errors++;
            }
        } else {
            /* return; without expression */
            if (ret && strcmp(ret->type, "void") != 0) {
                printf("Line %d, col %d: Incompatible type void in return statement\n",
                       node->line, node->col);
                sem_errors++;
            }
        }
        break;
    }
    case node_Print: {
        ASTNode *arg = node->child;
        if (arg->type != node_StrLit) {
            const char *arg_type = check_expr(arg);
            if (strcmp(arg_type, "int") != 0 && strcmp(arg_type, "double") != 0 &&
                strcmp(arg_type, "boolean") != 0 &&
                strcmp(arg_type, "undef") != 0) {
                printf("Line %d, col %d: Incompatible type %s in System.out.print statement\n",
                       arg->line, arg->col, arg_type);
                sem_errors++;
            }
        }
        break;
    }
    case node_Block: {
        ASTNode *s = node->child;
        while (s) { check_statement(s); s = s->sibling; }
        break;
    }
    case node_Assign: case node_Call: case node_ParseArgs:
        check_expr(node);
        break;
    default:
        break;
    }
}

/* Phase 2: Process each method body */
static void check_methods(ASTNode *root) {
    ASTNode *decl = root->child->sibling; /* skip class identifier */
    while (decl) {
        if (decl->type == node_MethodDecl) {
            if (decl->line == -1) { decl = decl->sibling; continue; }

            ASTNode *header = decl->child;
            ASTNode *body = header->sibling;
            ASTNode *ret_type_node = header->child;
            ASTNode *method_id = ret_type_node->sibling;
            ASTNode *method_params = method_id->sibling;

            char *param_types = build_param_types(method_params);
            current_method = find_method_table(method_id->value, param_types);
            free(param_types);

            if (active_locals) { symtab_free(active_locals); }
            active_locals = symtab_create("locals");

            /* Process statements in order; add VarDecl to active_locals when encountered */
            ASTNode *stmt = body->child;
            while (stmt) {
                if (stmt->type == node_VarDecl) {
                    ASTNode *vtype = stmt->child;
                    ASTNode *vid = vtype->sibling;
                    const char *vtstr = type_node_to_str(vtype);
                    if (strcmp(vid->value, "_") == 0) {
                        printf("Line %d, col %d: Symbol _ is reserved\n", vid->line, vid->col);
                        sem_errors++;
                    } else {
                        if (current_method && symtab_add(current_method, vid->value, vtstr, 0, NULL) != 0) {
                            printf("Line %d, col %d: Symbol %s already defined\n", vid->line, vid->col, vid->value);
                            sem_errors++;
                        } else {
                            symtab_add(active_locals, vid->value, vtstr, 0, NULL);
                        }
                    }
                } else {
                    check_statement(stmt);
                }
                stmt = stmt->sibling;
            }
        }
        decl = decl->sibling;
    }
    if (active_locals) { symtab_free(active_locals); active_locals = NULL; }
}

int semantic_analysis(ASTNode *root) {
    sem_errors = 0;
    build_tables(root);
    check_methods(root);
    return sem_errors;
}

SymTable *get_symbol_tables(void) {
    return tables;
}

void semantic_free(void) {
    symtab_free_all(tables);
    tables = NULL;
    tables_last = NULL;
    class_table = NULL;
    current_method = NULL;
    if (active_locals) { symtab_free(active_locals); active_locals = NULL; }
}
