# allcode-meta4

Este ficheiro agrega o source code da Meta 4 (apenas ficheiros da própria meta, excluindo testes).

## ast.c

```c
/* ast.c - Meta 3 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static const char *node_names[] = {
    "Program", "FieldDecl", "VarDecl",
    "MethodDecl", "MethodHeader", "MethodParams", "ParamDecl", "MethodBody",
    "Block", "If", "While", "Return", "Call", "Print", "ParseArgs", "Assign",
    "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge",
    "Add", "Sub", "Mul", "Div", "Mod", "Lshift", "Rshift", "Xor",
    "Not", "Minus", "Plus", "Length",
    "Bool", "BoolLit", "Double", "Decimal", "Identifier", "Int",
    "Natural", "StrLit", "StringArray", "Void"
};

const char *node_type_name(NodeType type) { return node_names[type]; }

ASTNode *ast_new_node(NodeType type, char *value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) { fprintf(stderr, "Error: malloc failed\n"); exit(1); }
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->annotation = NULL;
    node->line = 0;
    node->col = 0;
    node->child = NULL;
    node->sibling = NULL;
    return node;
}

void ast_add_child(ASTNode *parent, ASTNode *child) {
    if (!parent || !child) return;
    if (!parent->child) { parent->child = child; }
    else {
        ASTNode *last = parent->child;
        while (last->sibling) last = last->sibling;
        last->sibling = child;
    }
}

ASTNode *ast_add_sibling(ASTNode *node, ASTNode *sibling) {
    if (!node) return sibling;
    if (!sibling) return node;
    ASTNode *last = node;
    while (last->sibling) last = last->sibling;
    last->sibling = sibling;
    return node;
}

void ast_print(ASTNode *node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("..");
    if (node->value)
        printf("%s(%s)", node_type_name(node->type), node->value);
    else
        printf("%s", node_type_name(node->type));
    if (node->annotation)
        printf(" - %s", node->annotation);
    printf("\n");
    ast_print(node->child, depth + 1);
    ast_print(node->sibling, depth);
}

void ast_free(ASTNode *node) {
    if (!node) return;
    ast_free(node->child);
    ast_free(node->sibling);
    if (node->value) free(node->value);
    if (node->annotation) free(node->annotation);
    free(node);
}

int ast_child_count(ASTNode *node) {
    if (!node) return 0;
    int count = 0;
    ASTNode *c = node->child;
    while (c) { count++; c = c->sibling; }
    return count;
}

```

## ast.h

```c
/* ast.h - Meta 3 */
#ifndef AST_H
#define AST_H

typedef enum {
    node_Program, node_FieldDecl, node_VarDecl,
    node_MethodDecl, node_MethodHeader, node_MethodParams, node_ParamDecl, node_MethodBody,
    node_Block, node_If, node_While, node_Return, node_Call, node_Print, node_ParseArgs, node_Assign,
    node_Or, node_And, node_Eq, node_Ne, node_Lt, node_Gt, node_Le, node_Ge,
    node_Add, node_Sub, node_Mul, node_Div, node_Mod, node_Lshift, node_Rshift, node_Xor,
    node_Not, node_Minus, node_Plus, node_Length,
    node_Bool, node_BoolLit, node_Double, node_Decimal, node_Identifier, node_Int,
    node_Natural, node_StrLit, node_StringArray, node_Void
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char *value;
    char *annotation;       /* type annotation for semantic analysis */
    int line, col;          /* source position for error reporting */
    struct ASTNode *child;
    struct ASTNode *sibling;
} ASTNode;

const char *node_type_name(NodeType type);
ASTNode *ast_new_node(NodeType type, char *value);
void ast_add_child(ASTNode *parent, ASTNode *child);
ASTNode *ast_add_sibling(ASTNode *node, ASTNode *sibling);
void ast_print(ASTNode *node, int depth);
void ast_free(ASTNode *node);
int ast_child_count(ASTNode *node);

#endif

```

## codegen.c

```c
/* codegen.c - LLVM IR Code Generation (Meta 4) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

/* ========== State ========== */
static int temp_cnt = 0;
static int label_cnt = 0;
static int str_cnt = 0;
static int block_terminated = 0;

static SymTable *all_tables = NULL;
static SymTable *class_table = NULL;
static SymTable *cur_method_table = NULL;
static const char *cur_ret_type = NULL;

/* Collected STRLIT constants */
typedef struct { char *name; char *llvm_str; int len; } StrConst;
static StrConst str_consts[256];
static int n_str_consts = 0;

/* ========== Helpers ========== */
static int new_temp(void) { return temp_cnt++; }
static int new_label(void) { return label_cnt++; }

static const char *llvm_type(const char *juc) {
    if (!juc) return "i32";
    if (strcmp(juc, "int") == 0) return "i32";
    if (strcmp(juc, "double") == 0) return "double";
    if (strcmp(juc, "boolean") == 0) return "i1";
    if (strcmp(juc, "void") == 0) return "void";
    if (strcmp(juc, "String[]") == 0) return "i8**";
    return "i32";
}

static const char *llvm_default(const char *juc) {
    if (!juc) return "0";
    if (strcmp(juc, "double") == 0) return "0.0";
    if (strcmp(juc, "boolean") == 0) return "0";
    return "0";
}

static const char *type_node_str(ASTNode *n) {
    if (!n) return "int";
    switch (n->type) {
        case node_Int: return "int";
        case node_Double: return "double";
        case node_Bool: return "boolean";
        case node_Void: return "void";
        case node_StringArray: return "String[]";
        default: return "int";
    }
}

/* Find method table by name and param_types */
static SymTable *find_method_table(const char *name, const char *ptypes) {
    char prefix[512];
    snprintf(prefix, sizeof(prefix), "Method %s%s Symbol Table", name, ptypes);
    SymTable *t = all_tables;
    while (t) {
        if (strcmp(t->name, prefix) == 0) return t;
        t = t->next;
    }
    return NULL;
}

/* Build param types string from MethodParams node */
static char *build_param_types(ASTNode *mp) {
    char buf[1024] = "(";
    ASTNode *p = mp ? mp->child : NULL;
    int first = 1;
    while (p) {
        if (p->type == node_ParamDecl) {
            if (!first) strcat(buf, ",");
            strcat(buf, type_node_str(p->child));
            first = 0;
        }
        p = p->sibling;
    }
    strcat(buf, ")");
    return strdup(buf);
}

/* Mangle a method name based on its param types.
   main(String[]) stays as "main" (LLVM entry point).
   Other methods: name + "__" + param types joined by "_".
   No params: name + "__". */
static void mangle_method_name(const char *fname, const char *ptypes,
                               char *out, int outlen) {
    /* main with String[] params is the LLVM entry point */
    if (strcmp(fname, "main") == 0 && strstr(ptypes, "String[]")) {
        strncpy(out, "main", outlen - 1);
        out[outlen - 1] = '\0';
        return;
    }

    char buf[512];
    int bi = snprintf(buf, sizeof(buf), "%s", fname);
    const char *p = ptypes + 1; /* skip '(' */

    if (*p == ')') {
        /* No params */
        bi += snprintf(buf + bi, sizeof(buf) - bi, "__");
    } else {
        while (*p && *p != ')') {
            const char *end = p;
            while (*end && *end != ',' && *end != ')') end++;
            int len = (int)(end - p);
            char type[64];
            if (len < 64) { strncpy(type, p, len); type[len] = '\0'; }
            else { strncpy(type, p, 63); type[63] = '\0'; }

            /* Sanitize: remove spaces and brackets */
            char stype[64]; int si = 0;
            for (int k = 0; type[k] && si < 62; k++) {
                if (type[k] == ' ' || type[k] == '[' || type[k] == ']') continue;
                stype[si++] = type[k];
            }
            stype[si] = '\0';

            bi += snprintf(buf + bi, sizeof(buf) - bi, "__%s", stype);
            if (*end == ',') end++;
            p = end;
        }
    }

    strncpy(out, buf, outlen - 1);
    out[outlen - 1] = '\0';
}

/* Check if Java-level arg types exactly match formal param types (no promotion) */
static int types_match_exact(const char *param_types,
                              const char **arg_types, int nargs) {
    const char *pt = param_types + 1; /* skip '(' */
    int i = 0;
    while (i < nargs && *pt && *pt != ')') {
        const char *end = pt;
        while (*end && *end != ',' && *end != ')') end++;
        int fl = (int)(end - pt);
        char formal[64];
        if (fl < 64) { strncpy(formal, pt, fl); formal[fl] = '\0'; }
        else { strncpy(formal, pt, 63); formal[63] = '\0'; }

        const char *actual = arg_types[i] ? arg_types[i] : "int";
        if (strcmp(formal, actual) != 0) return 0;
        i++;
        if (*end == ',') end++;
        pt = end;
    }
    return (i == nargs && (*pt == ')'));
}

/* Check if Java-level arg types match with int→double promotion allowed */
static int types_match_promo(const char *param_types,
                              const char **arg_types, int nargs) {
    const char *pt = param_types + 1;
    int i = 0;
    while (i < nargs && *pt && *pt != ')') {
        const char *end = pt;
        while (*end && *end != ',' && *end != ')') end++;
        int fl = (int)(end - pt);
        char formal[64];
        if (fl < 64) { strncpy(formal, pt, fl); formal[fl] = '\0'; }
        else { strncpy(formal, pt, 63); formal[63] = '\0'; }

        const char *actual = arg_types[i] ? arg_types[i] : "int";
        if (strcmp(formal, actual) != 0) {
            /* Allow int→double promotion only */
            if (!(strcmp(formal, "double") == 0 && strcmp(actual, "int") == 0))
                return 0;
        }
        i++;
        if (*end == ',') end++;
        pt = end;
    }
    return (i == nargs && (*pt == ')'));
}

/* Find the best overload for a call site given Java-level arg types.
   Prefers exact match over promoted match. */
static Symbol *find_overload(const char *fname,
                              const char **arg_types, int nargs) {
    Symbol *candidates[64];
    int ncand = symtab_find_methods(class_table, fname, candidates, 64);

    /* First pass: exact match */
    for (int i = 0; i < ncand; i++) {
        if (types_match_exact(candidates[i]->param_types, arg_types, nargs))
            return candidates[i];
    }
    /* Second pass: with int→double promotion */
    for (int i = 0; i < ncand; i++) {
        if (types_match_promo(candidates[i]->param_types, arg_types, nargs))
            return candidates[i];
    }
    return ncand > 0 ? candidates[0] : NULL;
}

/* Is this symbol a global variable?
   Returns false if the name is locally defined (local shadows global). */
static int is_global(const char *name) {
    /* If there's a local definition, it shadows the global */
    if (cur_method_table && symtab_lookup(cur_method_table, name)) return 0;
    Symbol *s = symtab_lookup(class_table, name);
    return (s && !s->param_types); /* in class table, not a method */
}

/* Resolve global/local using the annotation type from semantic analysis.
   If the method table has a local with a DIFFERENT type than ann_type, the
   semantic analyzer resolved this reference to the global (pre-declaration
   shadowing). Falls back to is_global() when types match. */
static int is_global_by_annotation(const char *name, const char *ann_type) {
    if (!cur_method_table || !ann_type) return is_global(name);
    Symbol *local = symtab_lookup(cur_method_table, name);
    if (!local) return is_global(name);
    if (local->is_param) return 0;
    if (strcmp(local->type, ann_type) != 0) {
        /* Local has different type than annotation → annotation resolved to global */
        Symbol *global = symtab_lookup(class_table, name);
        if (global && !global->param_types && strcmp(global->type, ann_type) == 0)
            return 1;
    }
    return 0;
}

/* ========== String Processing ========== */
static void process_strlit(const char *val, char *out, int *out_len) {
    int i = 1; /* skip opening quote */
    int len = strlen(val);
    int oi = 0;
    while (i < len - 1) { /* skip closing quote */
        if (val[i] == '\\' && i + 1 < len - 1) {
            switch (val[i+1]) {
                case 'n': out[oi++]='\\'; out[oi++]='0'; out[oi++]='A'; break;
                case 't': out[oi++]='\\'; out[oi++]='0'; out[oi++]='9'; break;
                case 'r': out[oi++]='\\'; out[oi++]='0'; out[oi++]='D'; break;
                case 'f': out[oi++]='\\'; out[oi++]='0'; out[oi++]='C'; break;
                case '\\': out[oi++]='\\'; out[oi++]='5'; out[oi++]='C'; break;
                case '"': out[oi++]='\\'; out[oi++]='2'; out[oi++]='2'; break;
                default: out[oi++] = val[i]; out[oi++] = val[i+1]; break;
            }
            i += 2;
            (*out_len)++;
        } else {
            out[oi++] = val[i++];
            (*out_len)++;
        }
    }
    out[oi++] = '\\'; out[oi++] = '0'; out[oi++] = '0'; /* null terminator */
    out[oi] = '\0';
    (*out_len)++; /* count null terminator */
}

static void collect_strings(ASTNode *node) {
    if (!node) return;
    if (node->type == node_StrLit && node->value) {
        int found = 0;
        for (int i = 0; i < n_str_consts; i++) {
            if (strcmp(str_consts[i].llvm_str, node->value) == 0) { found = 1; break; }
        }
        if (!found && n_str_consts < 256) {
            char llvm_str[4096];
            int byte_len = 0;
            process_strlit(node->value, llvm_str, &byte_len);
            char name[32];
            snprintf(name, sizeof(name), "@.str.%d", str_cnt++);
            str_consts[n_str_consts].name = strdup(name);
            str_consts[n_str_consts].llvm_str = strdup(node->value);
            str_consts[n_str_consts].len = byte_len;
            n_str_consts++;
            printf("%s = private constant [%d x i8] c\"%s\"\n", name, byte_len, llvm_str);
        }
    }
    collect_strings(node->child);
    collect_strings(node->sibling);
}

static const char *find_str_const(const char *val) {
    for (int i = 0; i < n_str_consts; i++)
        if (strcmp(str_consts[i].llvm_str, val) == 0) return str_consts[i].name;
    return "@.str.0";
}
static int find_str_len(const char *val) {
    for (int i = 0; i < n_str_consts; i++)
        if (strcmp(str_consts[i].llvm_str, val) == 0) return str_consts[i].len;
    return 1;
}

/* ========== Preamble & Globals ========== */
static void emit_preamble(void) {
    printf("@.fmt.d = private constant [3 x i8] c\"%%d\\00\"\n");
    printf("@.fmt.e = private constant [6 x i8] c\"%%.16e\\00\"\n");
    printf("@.fmt.s = private constant [3 x i8] c\"%%s\\00\"\n");
    printf("@.str.true = private constant [5 x i8] c\"true\\00\"\n");
    printf("@.str.false = private constant [6 x i8] c\"false\\00\"\n");
    printf("\n");
}

static void emit_extern_decls(void) {
    printf("declare i32 @printf(i8*, ...)\n");
    printf("declare i32 @atoi(i8*)\n");
    printf("\n");
}

static void emit_globals(ASTNode *root) {
    ASTNode *decl = root->child->sibling; /* skip class identifier */
    while (decl) {
        if (decl->type == node_FieldDecl) {
            ASTNode *ftype = decl->child;
            ASTNode *fid = ftype->sibling;
            const char *t = type_node_str(ftype);
            printf("@%s = global %s %s\n", fid->value, llvm_type(t), llvm_default(t));
        }
        decl = decl->sibling;
    }
    printf("\n");
}

/* ========== Forward declarations ========== */
static void emit_method(ASTNode *method_decl);
static void emit_statement(ASTNode *stmt);
static int emit_expr(ASTNode *expr);
static int maybe_convert(int t, const char *from, const char *to);

/* ========== Main codegen entry point ========== */
void codegen(ASTNode *root, SymTable *tables) {
    all_tables = tables;
    class_table = tables; /* first table is the class table */
    temp_cnt = 0;
    label_cnt = 0;

    emit_preamble();
    collect_strings(root);
    if (n_str_consts > 0) printf("\n");
    emit_globals(root);
    emit_extern_decls();

    ASTNode *decl = root->child->sibling;
    while (decl) {
        if (decl->type == node_MethodDecl) {
            emit_method(decl);
            printf("\n");
        }
        decl = decl->sibling;
    }
}

static void emit_method(ASTNode *md) {
    ASTNode *header = md->child;
    ASTNode *body = header->sibling;
    ASTNode *ret_type_node = header->child;
    ASTNode *method_id = ret_type_node->sibling;
    ASTNode *method_params = method_id->sibling;

    const char *fname = method_id->value;
    const char *ret_juc = type_node_str(ret_type_node);
    cur_ret_type = ret_juc;

    char *ptypes = build_param_types(method_params);
    cur_method_table = find_method_table(fname, ptypes);

    /* Get mangled LLVM name */
    char mangled[256];
    mangle_method_name(fname, ptypes, mangled, sizeof(mangled));

    int is_main = (strcmp(mangled, "main") == 0);
    temp_cnt = 0;
    block_terminated = 0;

    /* Function signature */
    if (is_main) {
        printf("define i32 @main(i32 %%argc, i8** %%argv) {\n");
    } else {
        printf("define %s @%s(", llvm_type(ret_juc), mangled);
        ASTNode *p = method_params->child;
        int first = 1;
        while (p) {
            if (p->type == node_ParamDecl) {
                ASTNode *pt = p->child;
                ASTNode *pid = pt->sibling;
                const char *pjuc = type_node_str(pt);
                if (!first) printf(", ");
                if (strcmp(pjuc, "String[]") == 0) {
                    printf("i32 %%argc%s, i8** %%argv%s", pid->value, pid->value);
                } else {
                    printf("%s %%param.%s", llvm_type(pjuc), pid->value);
                }
                first = 0;
            }
            p = p->sibling;
        }
        printf(") {\n");
    }

    printf("entry:\n");

    /* Alloca for all parameters */
    ASTNode *p = method_params->child;
    while (p) {
        if (p->type == node_ParamDecl) {
            ASTNode *pt = p->child;
            ASTNode *pid = pt->sibling;
            const char *pjuc = type_node_str(pt);
            if (strcmp(pjuc, "String[]") == 0) {
                printf("  %%%s.argc = alloca i32\n", pid->value);
                printf("  %%%s.argv = alloca i8**\n", pid->value);
                if (is_main) {
                    printf("  store i32 %%argc, i32* %%%s.argc\n", pid->value);
                    printf("  store i8** %%argv, i8*** %%%s.argv\n", pid->value);
                } else {
                    printf("  store i32 %%argc%s, i32* %%%s.argc\n", pid->value, pid->value);
                    printf("  store i8** %%argv%s, i8*** %%%s.argv\n", pid->value, pid->value);
                }
            } else if (!is_main) {
                const char *plt = llvm_type(pjuc);
                printf("  %%%s = alloca %s\n", pid->value, plt);
                printf("  store %s %%param.%s, %s* %%%s\n", plt, pid->value, plt, pid->value);
            }
        }
        p = p->sibling;
    }

    /* Alloca for local variables */
    ASTNode *stmt = body->child;
    while (stmt) {
        if (stmt->type == node_VarDecl) {
            ASTNode *vt = stmt->child;
            ASTNode *vid = vt->sibling;
            const char *vlt = llvm_type(type_node_str(vt));
            printf("  %%%s = alloca %s\n", vid->value, vlt);
            printf("  store %s %s, %s* %%%s\n", vlt, llvm_default(type_node_str(vt)), vlt, vid->value);
        }
        stmt = stmt->sibling;
    }

    /* Generate statements */
    stmt = body->child;
    while (stmt) {
        if (stmt->type != node_VarDecl && !block_terminated) {
            emit_statement(stmt);
        }
        stmt = stmt->sibling;
    }

    /* Default terminator */
    if (!block_terminated) {
        if (is_main) {
            printf("  ret i32 0\n");
        } else if (strcmp(ret_juc, "void") == 0) {
            printf("  ret void\n");
        } else if (strcmp(ret_juc, "double") == 0) {
            printf("  ret double 0.0\n");
        } else if (strcmp(ret_juc, "boolean") == 0) {
            printf("  ret i1 0\n");
        } else {
            printf("  ret i32 0\n");
        }
    }

    printf("}\n");
    free(ptypes);
}

static void emit_statement(ASTNode *stmt) {
    if (!stmt) return;

    switch (stmt->type) {
    case node_If: {
        ASTNode *cond = stmt->child;
        ASTNode *then_s = cond->sibling;
        ASTNode *else_s = then_s->sibling;
        int cval = emit_expr(cond);
        int lthen = new_label(), lelse = new_label(), lend = new_label();
        printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", cval, lthen, lelse);
        printf("L%d:\n", lthen);
        block_terminated = 0;
        emit_statement(then_s);
        if (!block_terminated) printf("  br label %%L%d\n", lend);
        printf("L%d:\n", lelse);
        block_terminated = 0;
        if (else_s && !(else_s->type == node_Block && !else_s->child))
            emit_statement(else_s);
        if (!block_terminated) printf("  br label %%L%d\n", lend);
        printf("L%d:\n", lend);
        block_terminated = 0;
        break;
    }
    case node_While: {
        ASTNode *cond = stmt->child;
        ASTNode *wbody = cond->sibling;
        int lcond = new_label(), lbody = new_label(), lend = new_label();
        printf("  br label %%L%d\n", lcond);
        printf("L%d:\n", lcond);
        int cval = emit_expr(cond);
        printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", cval, lbody, lend);
        printf("L%d:\n", lbody);
        block_terminated = 0;
        emit_statement(wbody);
        if (!block_terminated) printf("  br label %%L%d\n", lcond);
        printf("L%d:\n", lend);
        block_terminated = 0;
        break;
    }
    case node_Return: {
        if (stmt->child) {
            int val = emit_expr(stmt->child);
            val = maybe_convert(val, stmt->child->annotation, cur_ret_type);
            printf("  ret %s %%t%d\n", llvm_type(cur_ret_type), val);
        } else {
            printf("  ret void\n");
        }
        block_terminated = 1;
        break;
    }
    case node_Print: {
        ASTNode *arg = stmt->child;
        if (arg->type == node_StrLit) {
            const char *sname = find_str_const(arg->value);
            int slen = find_str_len(arg->value);
            int ptr = new_temp();
            printf("  %%t%d = getelementptr [%d x i8], [%d x i8]* %s, i32 0, i32 0\n",
                   ptr, slen, slen, sname);
            int fptr = new_temp();
            printf("  %%t%d = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0\n", fptr);
            int d = new_temp();
            printf("  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, i8* %%t%d)\n", d, fptr, ptr);
        } else {
            int val = emit_expr(arg);
            const char *atype = arg->annotation;
            if (atype && strcmp(atype, "boolean") == 0) {
                int ltr = new_label(), lfa = new_label(), lend = new_label();
                printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", val, ltr, lfa);
                printf("L%d:\n", ltr);
                int pt = new_temp();
                printf("  %%t%d = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0\n", pt);
                int fp1 = new_temp();
                printf("  %%t%d = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0\n", fp1);
                int d1 = new_temp();
                printf("  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, i8* %%t%d)\n", d1, fp1, pt);
                printf("  br label %%L%d\n", lend);
                printf("L%d:\n", lfa);
                int pf = new_temp();
                printf("  %%t%d = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0\n", pf);
                int fp2 = new_temp();
                printf("  %%t%d = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0\n", fp2);
                int d2 = new_temp();
                printf("  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, i8* %%t%d)\n", d2, fp2, pf);
                printf("  br label %%L%d\n", lend);
                printf("L%d:\n", lend);
            } else if (atype && strcmp(atype, "double") == 0) {
                int fptr = new_temp();
                printf("  %%t%d = getelementptr [6 x i8], [6 x i8]* @.fmt.e, i32 0, i32 0\n", fptr);
                int d = new_temp();
                printf("  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, double %%t%d)\n", d, fptr, val);
            } else {
                int fptr = new_temp();
                printf("  %%t%d = getelementptr [3 x i8], [3 x i8]* @.fmt.d, i32 0, i32 0\n", fptr);
                int d = new_temp();
                printf("  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, i32 %%t%d)\n", d, fptr, val);
            }
        }
        break;
    }
    case node_Assign:
    case node_Call:
    case node_ParseArgs:
        emit_expr(stmt);
        break;
    case node_Block: {
        ASTNode *s = stmt->child;
        while (s) {
            if (!block_terminated) emit_statement(s);
            s = s->sibling;
        }
        break;
    }
    default:
        break;
    }
}

static int maybe_convert(int t, const char *from, const char *to) {
    if (!from || !to) return t;
    if (strcmp(from, "int") == 0 && strcmp(to, "double") == 0) {
        int r = new_temp();
        printf("  %%t%d = sitofp i32 %%t%d to double\n", r, t);
        return r;
    }
    if (strcmp(from, "boolean") == 0 && strcmp(to, "int") == 0) {
        int r = new_temp();
        printf("  %%t%d = zext i1 %%t%d to i32\n", r, t);
        return r;
    }
    return t;
}

static int emit_expr(ASTNode *expr) {
    if (!expr) return 0;
    int r;

    switch (expr->type) {
    case node_Natural: {
        r = new_temp();
        char clean[256]; int ci = 0;
        for (int i = 0; expr->value[i]; i++)
            if (expr->value[i] != '_') clean[ci++] = expr->value[i];
        clean[ci] = '\0';
        printf("  %%t%d = add i32 0, %s\n", r, clean);
        return r;
    }
    case node_Decimal: {
        r = new_temp();
        char clean[512]; int ci = 0;
        for (int i = 0; expr->value[i]; i++)
            if (expr->value[i] != '_') clean[ci++] = expr->value[i];
        clean[ci] = '\0';
        double val = strtod(clean, NULL);
        printf("  %%t%d = fadd double 0.0, %.17e\n", r, val);
        return r;
    }
    case node_BoolLit: {
        r = new_temp();
        int v = (strcmp(expr->value, "true") == 0) ? 1 : 0;
        printf("  %%t%d = add i1 0, %d\n", r, v);
        return r;
    }
    case node_Identifier: {
        /* Use annotation from semantic analysis for correct type resolution
           (handles pre-declaration shadowing of globals by locals). */
        const char *juc_t = expr->annotation ? expr->annotation : "int";
        /* String[] identifiers are expanded at call sites */
        if (strcmp(juc_t, "String[]") == 0) return 0;
        r = new_temp();
        const char *t = llvm_type(juc_t);
        if (is_global_by_annotation(expr->value, juc_t))
            printf("  %%t%d = load %s, %s* @%s\n", r, t, t, expr->value);
        else
            printf("  %%t%d = load %s, %s* %%%s\n", r, t, t, expr->value);
        return r;
    }
    case node_Add: case node_Sub: case node_Mul: case node_Div: {
        int left = emit_expr(expr->child);
        int right = emit_expr(expr->child->sibling);
        const char *lt = expr->child->annotation;
        const char *rt = expr->child->sibling->annotation;
        const char *res = expr->annotation;
        if (res && strcmp(res, "double") == 0) {
            left = maybe_convert(left, lt, "double");
            right = maybe_convert(right, rt, "double");
            r = new_temp();
            const char *op;
            switch (expr->type) {
                case node_Add: op = "fadd"; break;
                case node_Sub: op = "fsub"; break;
                case node_Mul: op = "fmul"; break;
                default: op = "fdiv"; break;
            }
            printf("  %%t%d = %s double %%t%d, %%t%d\n", r, op, left, right);
        } else {
            r = new_temp();
            const char *op;
            switch (expr->type) {
                case node_Add: op = "add"; break;
                case node_Sub: op = "sub"; break;
                case node_Mul: op = "mul"; break;
                default: op = "sdiv"; break;
            }
            printf("  %%t%d = %s i32 %%t%d, %%t%d\n", r, op, left, right);
        }
        return r;
    }
    case node_Mod: {
        int left = emit_expr(expr->child);
        int right = emit_expr(expr->child->sibling);
        r = new_temp();
        if (expr->annotation && strcmp(expr->annotation, "double") == 0) {
            left = maybe_convert(left, expr->child->annotation, "double");
            right = maybe_convert(right, expr->child->sibling->annotation, "double");
            printf("  %%t%d = frem double %%t%d, %%t%d\n", r, left, right);
        } else {
            printf("  %%t%d = srem i32 %%t%d, %%t%d\n", r, left, right);
        }
        return r;
    }
    case node_Lshift: {
        int left = emit_expr(expr->child);
        int right = emit_expr(expr->child->sibling);
        r = new_temp();
        printf("  %%t%d = shl i32 %%t%d, %%t%d\n", r, left, right);
        return r;
    }
    case node_Rshift: {
        int left = emit_expr(expr->child);
        int right = emit_expr(expr->child->sibling);
        r = new_temp();
        printf("  %%t%d = ashr i32 %%t%d, %%t%d\n", r, left, right);
        return r;
    }
    case node_Xor: {
        int left = emit_expr(expr->child);
        int right = emit_expr(expr->child->sibling);
        r = new_temp();
        if (expr->annotation && strcmp(expr->annotation, "boolean") == 0)
            printf("  %%t%d = xor i1 %%t%d, %%t%d\n", r, left, right);
        else
            printf("  %%t%d = xor i32 %%t%d, %%t%d\n", r, left, right);
        return r;
    }
    /* Short-circuit AND: if left is false, skip right evaluation */
    case node_And: {
        int sc_slot = new_temp();
        printf("  %%t%d = alloca i1\n", sc_slot);
        printf("  store i1 0, i1* %%t%d\n", sc_slot);
        int lval = emit_expr(expr->child);
        int leval = new_label(), lend = new_label();
        printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", lval, leval, lend);
        printf("L%d:\n", leval);
        block_terminated = 0;
        int rval = emit_expr(expr->child->sibling);
        printf("  store i1 %%t%d, i1* %%t%d\n", rval, sc_slot);
        printf("  br label %%L%d\n", lend);
        printf("L%d:\n", lend);
        block_terminated = 0;
        r = new_temp();
        printf("  %%t%d = load i1, i1* %%t%d\n", r, sc_slot);
        return r;
    }
    /* Short-circuit OR: if left is true, skip right evaluation */
    case node_Or: {
        int sc_slot = new_temp();
        printf("  %%t%d = alloca i1\n", sc_slot);
        printf("  store i1 1, i1* %%t%d\n", sc_slot);
        int lval = emit_expr(expr->child);
        int lend = new_label(), leval = new_label();
        printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", lval, lend, leval);
        printf("L%d:\n", leval);
        block_terminated = 0;
        int rval = emit_expr(expr->child->sibling);
        printf("  store i1 %%t%d, i1* %%t%d\n", rval, sc_slot);
        printf("  br label %%L%d\n", lend);
        printf("L%d:\n", lend);
        block_terminated = 0;
        r = new_temp();
        printf("  %%t%d = load i1, i1* %%t%d\n", r, sc_slot);
        return r;
    }
    case node_Eq: case node_Ne: case node_Lt: case node_Gt:
    case node_Le: case node_Ge: {
        int left = emit_expr(expr->child);
        int right = emit_expr(expr->child->sibling);
        const char *lt = expr->child->annotation;
        const char *rt = expr->child->sibling->annotation;
        int use_float = (lt && strcmp(lt, "double") == 0) || (rt && strcmp(rt, "double") == 0);
        if (use_float) {
            left = maybe_convert(left, lt, "double");
            right = maybe_convert(right, rt, "double");
            r = new_temp();
            const char *op;
            switch (expr->type) {
                case node_Eq: op = "oeq"; break; case node_Ne: op = "one"; break;
                case node_Lt: op = "olt"; break; case node_Gt: op = "ogt"; break;
                case node_Le: op = "ole"; break; default: op = "oge"; break;
            }
            printf("  %%t%d = fcmp %s double %%t%d, %%t%d\n", r, op, left, right);
        } else if (lt && strcmp(lt, "boolean") == 0) {
            r = new_temp();
            const char *op = (expr->type == node_Eq) ? "eq" : "ne";
            printf("  %%t%d = icmp %s i1 %%t%d, %%t%d\n", r, op, left, right);
        } else {
            r = new_temp();
            const char *op;
            switch (expr->type) {
                case node_Eq: op = "eq"; break; case node_Ne: op = "ne"; break;
                case node_Lt: op = "slt"; break; case node_Gt: op = "sgt"; break;
                case node_Le: op = "sle"; break; default: op = "sge"; break;
            }
            printf("  %%t%d = icmp %s i32 %%t%d, %%t%d\n", r, op, left, right);
        }
        return r;
    }
    case node_Not: {
        int val = emit_expr(expr->child);
        r = new_temp();
        printf("  %%t%d = xor i1 %%t%d, 1\n", r, val);
        return r;
    }
    case node_Minus: {
        int val = emit_expr(expr->child);
        r = new_temp();
        if (expr->annotation && strcmp(expr->annotation, "double") == 0)
            printf("  %%t%d = fsub double -0.0, %%t%d\n", r, val);
        else
            printf("  %%t%d = sub i32 0, %%t%d\n", r, val);
        return r;
    }
    case node_Plus: {
        return emit_expr(expr->child);
    }
    /* NAME.length = NAME.argc - 1 */
    case node_Length: {
        ASTNode *id = expr->child;
        const char *aname = id->value;
        int argc_val = new_temp();
        printf("  %%t%d = load i32, i32* %%%s.argc\n", argc_val, aname);
        r = new_temp();
        printf("  %%t%d = sub i32 %%t%d, 1\n", r, argc_val);
        return r;
    }
    case node_Assign: {
        ASTNode *lhs = expr->child;
        ASTNode *rhs = lhs->sibling;
        int val = emit_expr(rhs);
        /* Use annotation from LHS (set by semantic analysis) for correct type.
           This handles pre-declaration shadowing: if lhs->annotation says "int"
           but the method has a local "double" with the same name, the semantic
           analysis resolved to the global field. */
        const char *lhs_type = lhs->annotation ? lhs->annotation : "int";
        val = maybe_convert(val, rhs->annotation, lhs_type);
        const char *t = llvm_type(lhs_type);
        if (is_global_by_annotation(lhs->value, lhs_type))
            printf("  store %s %%t%d, %s* @%s\n", t, val, t, lhs->value);
        else
            printf("  store %s %%t%d, %s* %%%s\n", t, val, t, lhs->value);
        return val;
    }
    case node_Call: {
        ASTNode *id = expr->child;
        const char *fname = id->value;

        /* Collect Java-level arg types for overload resolution */
        const char *juc_argtypes[64];
        int jnargs = 0;
        ASTNode *anode = id->sibling;
        while (anode) {
            juc_argtypes[jnargs++] = anode->annotation;
            anode = anode->sibling;
        }

        /* Find matching overload */
        Symbol *method = find_overload(fname, juc_argtypes, jnargs);

        /* Get mangled call target name */
        char call_mangled[256];
        if (method) {
            mangle_method_name(fname, method->param_types, call_mangled, sizeof(call_mangled));
        } else {
            strncpy(call_mangled, fname, sizeof(call_mangled) - 1);
            call_mangled[sizeof(call_mangled) - 1] = '\0';
        }

        /* Collect argument temps, expanding String[] to (argc, argv) */
        int args[128];
        const char *atypes[128]; /* LLVM type strings */
        int nargs = 0;

        anode = id->sibling;
        int formal_idx = 0;
        while (anode) {
            if (anode->annotation && strcmp(anode->annotation, "String[]") == 0) {
                /* Expand to two LLVM args */
                const char *arrname = anode->value;
                int argc_v = new_temp();
                printf("  %%t%d = load i32, i32* %%%s.argc\n", argc_v, arrname);
                args[nargs] = argc_v;
                atypes[nargs] = "i32";
                nargs++;
                int argv_v = new_temp();
                printf("  %%t%d = load i8**, i8*** %%%s.argv\n", argv_v, arrname);
                args[nargs] = argv_v;
                atypes[nargs] = "i8**";
                nargs++;
            } else {
                int val = emit_expr(anode);
                /* Apply int→double conversion if the formal param is double */
                const char *formal_type = NULL;
                if (method && method->param_types) {
                    const char *pt = method->param_types + 1;
                    int fi = 0;
                    while (*pt && *pt != ')') {
                        const char *end = pt;
                        while (*end && *end != ',' && *end != ')') end++;
                        if (fi == formal_idx) {
                            int fl = (int)(end - pt);
                            static char fmt_buf[64];
                            if (fl < 64) { strncpy(fmt_buf, pt, fl); fmt_buf[fl] = '\0'; }
                            formal_type = fmt_buf;
                            break;
                        }
                        fi++;
                        if (*end == ',') end++;
                        pt = end;
                    }
                }
                if (formal_type) {
                    val = maybe_convert(val, anode->annotation, formal_type);
                    atypes[nargs] = llvm_type(formal_type);
                } else {
                    atypes[nargs] = llvm_type(anode->annotation);
                }
                args[nargs] = val;
                nargs++;
            }
            formal_idx++;
            anode = anode->sibling;
        }

        /* Determine return LLVM type */
        const char *ret_juc = expr->annotation ? expr->annotation :
                              (method ? method->type : "int");
        const char *ret_llvm = llvm_type(ret_juc);

        /* Emit call */
        if (strcmp(ret_llvm, "void") == 0) {
            printf("  call void @%s(", call_mangled);
        } else {
            r = new_temp();
            printf("  %%t%d = call %s @%s(", r, ret_llvm, call_mangled);
        }
        for (int i = 0; i < nargs; i++) {
            if (i > 0) printf(", ");
            printf("%s %%t%d", atypes[i], args[i]);
        }
        printf(")\n");
        if (strcmp(ret_llvm, "void") == 0) return 0;
        return r;
    }
    /* Integer.parseInt(NAME[expr]) */
    case node_ParseArgs: {
        ASTNode *id = expr->child;
        ASTNode *idx = id->sibling;
        const char *aname = id->value;
        int idx_val = emit_expr(idx);
        int adj = new_temp();
        printf("  %%t%d = add i32 %%t%d, 1\n", adj, idx_val);
        int argv_ptr = new_temp();
        printf("  %%t%d = load i8**, i8*** %%%s.argv\n", argv_ptr, aname);
        int str_ptr_ptr = new_temp();
        printf("  %%t%d = getelementptr i8*, i8** %%t%d, i32 %%t%d\n",
               str_ptr_ptr, argv_ptr, adj);
        int str_ptr = new_temp();
        printf("  %%t%d = load i8*, i8** %%t%d\n", str_ptr, str_ptr_ptr);
        r = new_temp();
        printf("  %%t%d = call i32 @atoi(i8* %%t%d)\n", r, str_ptr);
        return r;
    }
    default:
        return 0;
    }
}

```

## codegen.h

```c
/* codegen.h */
#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "symtab.h"

/* Generate LLVM IR code from the annotated AST */
void codegen(ASTNode *root, SymTable *tables);

#endif

```

## jucompiler.l

```lex
/* Autor:
 * Nome: João Afonso dos Santos Simões - Número: 2022236316
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "y.tab.h"

int line = 1;
int col = 1;
int flag_l = 0;
int flag_e1 = 0;
int flag_t = 0;
int flag_e2 = 0;
int flag_s = 0;
int flag_e3 = 0;

int tok_line, tok_col;
char tok_text[8192] = "";

/* String state */
int str_line, str_col;
char str_buf[4096];
int str_len;
int str_has_error;

/* Comment state */
int comment_line, comment_col;

#define UPDATE_COL()  { tok_line = line; tok_col = col; col += yyleng; strncpy(tok_text, yytext, sizeof(tok_text)-1); tok_text[sizeof(tok_text)-1] = '\0'; }

/* Set yylloc for bison %locations */
#define SET_LOC() { yylloc.first_line = tok_line; yylloc.first_column = tok_col; \
                    yylloc.last_line = tok_line; yylloc.last_column = tok_col + yyleng - 1; }

#define LEX_ONLY_TOKEN(name, tok) { \
    UPDATE_COL(); \
    if (flag_l) { printf(name "\n"); } \
    if (!flag_l && !flag_e1) { SET_LOC(); return tok; } \
}

#define LEX_ONLY_TOKEN_VAL(name, tok) { \
    UPDATE_COL(); \
    if (flag_l) { printf(name "(%s)\n", yytext); } \
    if (!flag_l && !flag_e1) { yylval.str = strdup(yytext); SET_LOC(); return tok; } \
}

#define LEX_ONLY_RESERVED(tok) { \
    UPDATE_COL(); \
    if (flag_l) { printf("RESERVED(%s)\n", yytext); } \
    if (!flag_l && !flag_e1) { SET_LOC(); return tok; } \
}
%}



DIGIT       [0-9]
NONZERODIGIT [1-9]
LETTER      [a-zA-Z_$]
ALPHANUMID  [a-zA-Z0-9_$]
DIGITS      {DIGIT}({DIGIT}|_)*{DIGIT}|{DIGIT}
EXPONENT    [eE][+-]?{DIGITS}

%x COMMENT
%x STRSTATE

%%

"//"[^\r\n]*            { col += yyleng; }

"/*"                    { comment_line = line; comment_col = col; col += 2; BEGIN(COMMENT); }
<COMMENT>"*/"           { col += 2; BEGIN(INITIAL); }
<COMMENT>\n             { line++; col = 1; }
<COMMENT>\r\n           { line++; col = 1; }
<COMMENT>\r             { line++; col = 1; }
<COMMENT>.              { col++; }
<COMMENT><<EOF>>        { printf("Line %d, col %d: unterminated comment\n", comment_line, comment_col); tok_line = line; tok_col = col; tok_text[0] = '\0'; BEGIN(INITIAL); return 0; }

\"                      { str_line = line; str_col = col; col++; str_len = 0; str_has_error = 0; str_buf[0] = '"'; str_len = 1; BEGIN(STRSTATE); }
<STRSTATE>\"            {
                            col++;
                            str_buf[str_len++] = '"';
                            str_buf[str_len] = '\0';
                            tok_line = str_line; tok_col = str_col;
                            if (flag_l && !str_has_error) { printf("STRLIT(%s)\n", str_buf); }
                            if (!flag_l && !flag_e1 && !str_has_error) {
                                yylval.str = strdup(str_buf);
                                yylloc.first_line = str_line; yylloc.first_column = str_col;
                                yylloc.last_line = str_line; yylloc.last_column = col - 1;
                                BEGIN(INITIAL);
                                return STRLIT;
                            }
                            BEGIN(INITIAL);
                        }
<STRSTATE>\\f           { col += 2; str_buf[str_len++] = '\\'; str_buf[str_len++] = 'f'; }
<STRSTATE>\\n           { col += 2; str_buf[str_len++] = '\\'; str_buf[str_len++] = 'n'; }
<STRSTATE>\\r           { col += 2; str_buf[str_len++] = '\\'; str_buf[str_len++] = 'r'; }
<STRSTATE>\\t           { col += 2; str_buf[str_len++] = '\\'; str_buf[str_len++] = 't'; }
<STRSTATE>\\\\          { col += 2; str_buf[str_len++] = '\\'; str_buf[str_len++] = '\\'; }
<STRSTATE>\\\"          { col += 2; str_buf[str_len++] = '\\'; str_buf[str_len++] = '"'; }
<STRSTATE>\\[^\n\r]     { printf("Line %d, col %d: invalid escape sequence (%s)\n", line, col, yytext); col += 2; str_has_error = 1; str_buf[str_len++] = yytext[0]; str_buf[str_len++] = yytext[1]; }
<STRSTATE>\n            { printf("Line %d, col %d: unterminated string literal\n", str_line, str_col); line++; col = 1; BEGIN(INITIAL); }
<STRSTATE>\r\n          { printf("Line %d, col %d: unterminated string literal\n", str_line, str_col); line++; col = 1; BEGIN(INITIAL); }
<STRSTATE>\r            { printf("Line %d, col %d: unterminated string literal\n", str_line, str_col); line++; col = 1; BEGIN(INITIAL); }
<STRSTATE><<EOF>>       { printf("Line %d, col %d: unterminated string literal\n", str_line, str_col); tok_line = line; tok_col = col; tok_text[0] = '\0'; BEGIN(INITIAL); return 0; }
<STRSTATE>.             { col++; str_buf[str_len++] = yytext[0]; }

"boolean"               { LEX_ONLY_TOKEN("BOOL", BOOL); }
"class"                 { LEX_ONLY_TOKEN("CLASS", CLASS); }
"double"                { LEX_ONLY_TOKEN("DOUBLE", DOUBLE); }
"else"                  { LEX_ONLY_TOKEN("ELSE", ELSE); }
"if"                    { LEX_ONLY_TOKEN("IF", IF); }
"int"                   { LEX_ONLY_TOKEN("INT", INT); }
"public"                { LEX_ONLY_TOKEN("PUBLIC", PUBLIC); }
"return"                { LEX_ONLY_TOKEN("RETURN", RETURN); }
"static"                { LEX_ONLY_TOKEN("STATIC", STATIC); }
"void"                  { LEX_ONLY_TOKEN("VOID", VOID); }
"while"                 { LEX_ONLY_TOKEN("WHILE", WHILE); }
"String"                { LEX_ONLY_TOKEN("STRING", STRING); }

"System.out.print"      { LEX_ONLY_TOKEN("PRINT", PRINT); }
"Integer.parseInt"      { LEX_ONLY_TOKEN("PARSEINT", PARSEINT); }
".length"               { LEX_ONLY_TOKEN("DOTLENGTH", DOTLENGTH); }

"true"                  { LEX_ONLY_TOKEN_VAL("BOOLLIT", BOOLLIT); }
"false"                 { LEX_ONLY_TOKEN_VAL("BOOLLIT", BOOLLIT); }

"abstract"              { LEX_ONLY_RESERVED(RESERVED); }
"assert"                { LEX_ONLY_RESERVED(RESERVED); }
"break"                 { LEX_ONLY_RESERVED(RESERVED); }
"byte"                  { LEX_ONLY_RESERVED(RESERVED); }
"case"                  { LEX_ONLY_RESERVED(RESERVED); }
"catch"                 { LEX_ONLY_RESERVED(RESERVED); }
"char"                  { LEX_ONLY_RESERVED(RESERVED); }
"const"                 { LEX_ONLY_RESERVED(RESERVED); }
"continue"              { LEX_ONLY_RESERVED(RESERVED); }
"default"               { LEX_ONLY_RESERVED(RESERVED); }
"do"                    { LEX_ONLY_RESERVED(RESERVED); }
"enum"                  { LEX_ONLY_RESERVED(RESERVED); }
"extends"               { LEX_ONLY_RESERVED(RESERVED); }
"final"                 { LEX_ONLY_RESERVED(RESERVED); }
"finally"               { LEX_ONLY_RESERVED(RESERVED); }
"float"                 { LEX_ONLY_RESERVED(RESERVED); }
"for"                   { LEX_ONLY_RESERVED(RESERVED); }
"goto"                  { LEX_ONLY_RESERVED(RESERVED); }
"implements"            { LEX_ONLY_RESERVED(RESERVED); }
"import"                { LEX_ONLY_RESERVED(RESERVED); }
"instanceof"            { LEX_ONLY_RESERVED(RESERVED); }
"interface"             { LEX_ONLY_RESERVED(RESERVED); }
"long"                  { LEX_ONLY_RESERVED(RESERVED); }
"native"                { LEX_ONLY_RESERVED(RESERVED); }
"new"                   { LEX_ONLY_RESERVED(RESERVED); }
"package"               { LEX_ONLY_RESERVED(RESERVED); }
"private"               { LEX_ONLY_RESERVED(RESERVED); }
"protected"             { LEX_ONLY_RESERVED(RESERVED); }
"short"                 { LEX_ONLY_RESERVED(RESERVED); }
"strictfp"              { LEX_ONLY_RESERVED(RESERVED); }
"super"                 { LEX_ONLY_RESERVED(RESERVED); }
"switch"                { LEX_ONLY_RESERVED(RESERVED); }
"synchronized"          { LEX_ONLY_RESERVED(RESERVED); }
"this"                  { LEX_ONLY_RESERVED(RESERVED); }
"throw"                 { LEX_ONLY_RESERVED(RESERVED); }
"throws"                { LEX_ONLY_RESERVED(RESERVED); }
"transient"             { LEX_ONLY_RESERVED(RESERVED); }
"try"                   { LEX_ONLY_RESERVED(RESERVED); }
"volatile"              { LEX_ONLY_RESERVED(RESERVED); }
"null"                  { LEX_ONLY_RESERVED(RESERVED); }
"++"                    { LEX_ONLY_RESERVED(RESERVED); }
"--"                    { LEX_ONLY_RESERVED(RESERVED); }
"Integer"               { LEX_ONLY_RESERVED(RESERVED); }
"System"                { LEX_ONLY_RESERVED(RESERVED); }

"&&"                    { LEX_ONLY_TOKEN("AND", AND); }
"||"                    { LEX_ONLY_TOKEN("OR", OR); }
"=="                    { LEX_ONLY_TOKEN("EQ", EQ); }
"!="                    { LEX_ONLY_TOKEN("NE", NE); }
">="                    { LEX_ONLY_TOKEN("GE", GE); }
"<="                    { LEX_ONLY_TOKEN("LE", LE); }
"<<"                    { LEX_ONLY_TOKEN("LSHIFT", LSHIFT); }
">>"                    { LEX_ONLY_TOKEN("RSHIFT", RSHIFT); }
"->"                    { LEX_ONLY_TOKEN("ARROW", ARROW); }
"="                     { LEX_ONLY_TOKEN("ASSIGN", ASSIGN); }
"*"                     { LEX_ONLY_TOKEN("STAR", STAR); }
","                     { LEX_ONLY_TOKEN("COMMA", COMMA); }
"/"                     { LEX_ONLY_TOKEN("DIV", DIV); }
">"                     { LEX_ONLY_TOKEN("GT", GT); }
"<"                     { LEX_ONLY_TOKEN("LT", LT); }
"-"                     { LEX_ONLY_TOKEN("MINUS", MINUS); }
"%"                     { LEX_ONLY_TOKEN("MOD", MOD); }
"!"                     { LEX_ONLY_TOKEN("NOT", NOT); }
"+"                     { LEX_ONLY_TOKEN("PLUS", PLUS); }
"^"                     { LEX_ONLY_TOKEN("XOR", XOR); }
"{"                     { LEX_ONLY_TOKEN("LBRACE", LBRACE); }
"}"                     { LEX_ONLY_TOKEN("RBRACE", RBRACE); }
"("                     { LEX_ONLY_TOKEN("LPAR", LPAR); }
")"                     { LEX_ONLY_TOKEN("RPAR", RPAR); }
"["                     { LEX_ONLY_TOKEN("LSQ", LSQ); }
"]"                     { LEX_ONLY_TOKEN("RSQ", RSQ); }
";"                     { LEX_ONLY_TOKEN("SEMICOLON", SEMICOLON); }

{DIGITS}"."{DIGITS}?{EXPONENT}?     { LEX_ONLY_TOKEN_VAL("DECIMAL", DECIMAL); }
"."{DIGITS}{EXPONENT}?              { LEX_ONLY_TOKEN_VAL("DECIMAL", DECIMAL); }
{DIGITS}{EXPONENT}                  { LEX_ONLY_TOKEN_VAL("DECIMAL", DECIMAL); }

0|{NONZERODIGIT}({DIGIT}|_)*{DIGIT}|{NONZERODIGIT}    { LEX_ONLY_TOKEN_VAL("NATURAL", NATURAL); }

{LETTER}{ALPHANUMID}*   { LEX_ONLY_TOKEN_VAL("IDENTIFIER", IDENTIFIER); }

\n                      { line++; col = 1; }
\r\n                    { line++; col = 1; }
\r                      { line++; col = 1; }
" "                     { col++; }
\t                      { col++; }
\f                      { col++; }

.                       { printf("Line %d, col %d: illegal character (%s)\n", line, col, yytext); col++; tok_line = line; tok_col = col; tok_text[0] = '\0'; }

<<EOF>>                 { tok_line = line; tok_col = col; tok_text[0] = '\0'; return 0; }

%%

int yywrap(void) {
    return 1;
}

```

## jucompiler.y

```yacc
/* Autor:
 * Nome: João Afonso dos Santos Simões - Número: 2022236316
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantics.h"
#include "codegen.h"

extern int yylex(void);
extern char *yytext;
extern int line, col;
extern int tok_line, tok_col;
extern char tok_text[];

void yyerror(const char *s);

ASTNode *root = NULL;
int syntax_error = 0;

/* Flags */
extern int flag_l, flag_e1, flag_t, flag_e2, flag_s, flag_e3;

/* Helper: create node with position */
#define NEWNODE(t, v, l, c) ({ ASTNode *_n = ast_new_node(t, v); _n->line = l; _n->col = c; _n; })
%}

%union {
    char *str;
    struct ASTNode *node;
    struct { int line; int col; } pos;
}

/* Tokens with values */
%token <str> IDENTIFIER NATURAL DECIMAL STRLIT BOOLLIT

/* Keyword tokens - some need position */
%token BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT
%token PUBLIC RETURN STATIC STRING VOID WHILE RESERVED

/* Operator tokens */
%token AND ASSIGN STAR COMMA DIV EQ GE GT LBRACE LE LPAR LSQ
%token LT MINUS MOD NE NOT OR PLUS RBRACE RPAR RSQ SEMICOLON
%token ARROW LSHIFT RSHIFT XOR

/* AST node types */
%type <node> Program MethodDecl FieldDecl MethodHeader MethodBody
%type <node> FormalParams FormalParamsList
%type <node> Type VarDecl Statement StatementList
%type <node> Expr MethodInvocation Assignment ParseArgs
%type <node> CommaExprList CommaIdList
%type <node> DeclList BodyDeclList

/* Precedence and associativity (low to high) */
%right ASSIGN
%left OR
%left AND
%left XOR
%left EQ NE
%left LT GT LE GE
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD
%right UNARY
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

/* Enable location tracking */
%locations

%%

Program:
    CLASS IDENTIFIER LBRACE DeclList RBRACE
        {
            root = ast_new_node(node_Program, NULL);
            ASTNode *id = NEWNODE(node_Identifier, $2, @2.first_line, @2.first_column);
            ast_add_child(root, id);
            ast_add_child(root, $4);
            free($2);
        }
    ;

DeclList:
    /* empty */
        { $$ = NULL; }
    | DeclList MethodDecl
        { $$ = ast_add_sibling($1, $2); }
    | DeclList FieldDecl
        { $$ = ast_add_sibling($1, $2); }
    | DeclList SEMICOLON
        { $$ = $1; }
    ;

MethodDecl:
    PUBLIC STATIC MethodHeader MethodBody
        {
            $$ = ast_new_node(node_MethodDecl, NULL);
            ast_add_child($$, $3);
            ast_add_child($$, $4);
        }
    ;

FieldDecl:
    PUBLIC STATIC Type IDENTIFIER CommaIdList SEMICOLON
        {
            ASTNode *first = ast_new_node(node_FieldDecl, NULL);
            ast_add_child(first, $3);
            ASTNode *id = NEWNODE(node_Identifier, $4, @4.first_line, @4.first_column);
            ast_add_child(first, id);
            free($4);

            ASTNode *extra = $5;
            while (extra) {
                ASTNode *next = extra->sibling;
                extra->sibling = NULL;
                ASTNode *fd = ast_new_node(node_FieldDecl, NULL);
                ASTNode *type_clone = ast_new_node($3->type, $3->value);
                ast_add_child(fd, type_clone);
                ast_add_child(fd, extra);
                first = ast_add_sibling(first, fd);
                extra = next;
            }
            $$ = first;
        }
    | error SEMICOLON
        { $$ = NULL; }
    ;

CommaIdList:
    /* empty */
        { $$ = NULL; }
    | CommaIdList COMMA IDENTIFIER
        {
            ASTNode *id = NEWNODE(node_Identifier, $3, @3.first_line, @3.first_column);
            free($3);
            $$ = ast_add_sibling($1, id);
        }
    ;

Type:
    BOOL    { $$ = ast_new_node(node_Bool, NULL); }
    | INT   { $$ = ast_new_node(node_Int, NULL); }
    | DOUBLE { $$ = ast_new_node(node_Double, NULL); }
    ;

MethodHeader:
    Type IDENTIFIER LPAR FormalParams RPAR
        {
            $$ = ast_new_node(node_MethodHeader, NULL);
            ast_add_child($$, $1);
            ASTNode *id = NEWNODE(node_Identifier, $2, @2.first_line, @2.first_column);
            ast_add_child($$, id);
            free($2);
            ASTNode *params = ast_new_node(node_MethodParams, NULL);
            ast_add_child(params, $4);
            ast_add_child($$, params);
        }
    | VOID IDENTIFIER LPAR FormalParams RPAR
        {
            $$ = ast_new_node(node_MethodHeader, NULL);
            ast_add_child($$, ast_new_node(node_Void, NULL));
            ASTNode *id = NEWNODE(node_Identifier, $2, @2.first_line, @2.first_column);
            ast_add_child($$, id);
            free($2);
            ASTNode *params = ast_new_node(node_MethodParams, NULL);
            ast_add_child(params, $4);
            ast_add_child($$, params);
        }
    ;

FormalParams:
    /* empty */
        { $$ = NULL; }
    | Type IDENTIFIER FormalParamsList
        {
            ASTNode *pd = ast_new_node(node_ParamDecl, NULL);
            ast_add_child(pd, $1);
            ASTNode *id = NEWNODE(node_Identifier, $2, @2.first_line, @2.first_column);
            ast_add_child(pd, id);
            free($2);
            $$ = ast_add_sibling(pd, $3);
        }
    | STRING LSQ RSQ IDENTIFIER
        {
            ASTNode *pd = ast_new_node(node_ParamDecl, NULL);
            ast_add_child(pd, ast_new_node(node_StringArray, NULL));
            ASTNode *id = NEWNODE(node_Identifier, $4, @4.first_line, @4.first_column);
            ast_add_child(pd, id);
            free($4);
            $$ = pd;
        }
    ;

FormalParamsList:
    /* empty */
        { $$ = NULL; }
    | FormalParamsList COMMA Type IDENTIFIER
        {
            ASTNode *pd = ast_new_node(node_ParamDecl, NULL);
            ast_add_child(pd, $3);
            ASTNode *id = NEWNODE(node_Identifier, $4, @4.first_line, @4.first_column);
            ast_add_child(pd, id);
            free($4);
            $$ = ast_add_sibling($1, pd);
        }
    ;

MethodBody:
    LBRACE BodyDeclList RBRACE
        {
            $$ = ast_new_node(node_MethodBody, NULL);
            ast_add_child($$, $2);
        }
    ;

BodyDeclList:
    /* empty */
        { $$ = NULL; }
    | BodyDeclList Statement
        { $$ = ast_add_sibling($1, $2); }
    | BodyDeclList VarDecl
        { $$ = ast_add_sibling($1, $2); }
    ;

VarDecl:
    Type IDENTIFIER CommaIdList SEMICOLON
        {
            ASTNode *first = ast_new_node(node_VarDecl, NULL);
            ast_add_child(first, $1);
            ASTNode *id = NEWNODE(node_Identifier, $2, @2.first_line, @2.first_column);
            ast_add_child(first, id);
            free($2);

            ASTNode *extra = $3;
            while (extra) {
                ASTNode *next = extra->sibling;
                extra->sibling = NULL;
                ASTNode *vd = ast_new_node(node_VarDecl, NULL);
                ASTNode *type_clone = ast_new_node($1->type, $1->value);
                ast_add_child(vd, type_clone);
                ast_add_child(vd, extra);
                first = ast_add_sibling(first, vd);
                extra = next;
            }
            $$ = first;
        }
    ;

Statement:
    LBRACE StatementList RBRACE
        {
            int count = 0;
            ASTNode *tmp = $2;
            while (tmp) { count++; tmp = tmp->sibling; }
            if (count == 0) {
                $$ = NULL;
            } else if (count == 1) {
                $$ = $2;
            } else {
                $$ = ast_new_node(node_Block, NULL);
                ast_add_child($$, $2);
            }
        }
    | IF LPAR Expr RPAR Statement ELSE Statement
        {
            $$ = ast_new_node(node_If, NULL);
            ast_add_child($$, $3);
            ast_add_child($$, $5 ? $5 : ast_new_node(node_Block, NULL));
            ast_add_child($$, $7 ? $7 : ast_new_node(node_Block, NULL));
        }
    | IF LPAR Expr RPAR Statement %prec LOWER_THAN_ELSE
        {
            $$ = ast_new_node(node_If, NULL);
            ast_add_child($$, $3);
            ast_add_child($$, $5 ? $5 : ast_new_node(node_Block, NULL));
            ast_add_child($$, ast_new_node(node_Block, NULL));
        }
    | WHILE LPAR Expr RPAR Statement
        {
            $$ = ast_new_node(node_While, NULL);
            ast_add_child($$, $3);
            ast_add_child($$, $5 ? $5 : ast_new_node(node_Block, NULL));
        }
    | RETURN Expr SEMICOLON
        {
            $$ = NEWNODE(node_Return, NULL, @1.first_line, @1.first_column);
            ast_add_child($$, $2);
        }
    | RETURN SEMICOLON
        {
            $$ = NEWNODE(node_Return, NULL, @1.first_line, @1.first_column);
        }
    | MethodInvocation SEMICOLON
        { $$ = $1; }
    | Assignment SEMICOLON
        { $$ = $1; }
    | ParseArgs SEMICOLON
        { $$ = $1; }
    | SEMICOLON
        { $$ = NULL; }
    | PRINT LPAR Expr RPAR SEMICOLON
        {
            $$ = ast_new_node(node_Print, NULL);
            ast_add_child($$, $3);
        }
    | PRINT LPAR STRLIT RPAR SEMICOLON
        {
            $$ = ast_new_node(node_Print, NULL);
            ASTNode *s = NEWNODE(node_StrLit, $3, @3.first_line, @3.first_column);
            ast_add_child($$, s);
            free($3);
        }
    | error SEMICOLON
        { $$ = NULL; }
    ;

StatementList:
    /* empty */
        { $$ = NULL; }
    | StatementList Statement
        {
            if ($2 == NULL) { $$ = $1; }
            else { $$ = ast_add_sibling($1, $2); }
        }
    ;

MethodInvocation:
    IDENTIFIER LPAR CommaExprList RPAR
        {
            $$ = NEWNODE(node_Call, NULL, @1.first_line, @1.first_column);
            ASTNode *id = NEWNODE(node_Identifier, $1, @1.first_line, @1.first_column);
            ast_add_child($$, id);
            free($1);
            ast_add_child($$, $3);
        }
    | IDENTIFIER LPAR RPAR
        {
            $$ = NEWNODE(node_Call, NULL, @1.first_line, @1.first_column);
            ASTNode *id = NEWNODE(node_Identifier, $1, @1.first_line, @1.first_column);
            ast_add_child($$, id);
            free($1);
        }
    | IDENTIFIER LPAR error RPAR
        { $$ = NULL; free($1); }
    ;

Assignment:
    IDENTIFIER ASSIGN Expr
        {
            $$ = NEWNODE(node_Assign, NULL, @2.first_line, @2.first_column);
            ASTNode *id = NEWNODE(node_Identifier, $1, @1.first_line, @1.first_column);
            ast_add_child($$, id);
            free($1);
            ast_add_child($$, $3);
        }
    ;

ParseArgs:
    PARSEINT LPAR IDENTIFIER LSQ Expr RSQ RPAR
        {
            $$ = NEWNODE(node_ParseArgs, NULL, @1.first_line, @1.first_column);
            ASTNode *id = NEWNODE(node_Identifier, $3, @3.first_line, @3.first_column);
            ast_add_child($$, id);
            free($3);
            ast_add_child($$, $5);
        }
    | PARSEINT LPAR error RPAR
        { $$ = NULL; }
    ;

CommaExprList:
    Expr
        { $$ = $1; }
    | CommaExprList COMMA Expr
        { $$ = ast_add_sibling($1, $3); }
    ;

Expr:
    Expr PLUS Expr      { $$ = NEWNODE(node_Add, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr MINUS Expr   { $$ = NEWNODE(node_Sub, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr STAR Expr    { $$ = NEWNODE(node_Mul, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr DIV Expr     { $$ = NEWNODE(node_Div, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr MOD Expr     { $$ = NEWNODE(node_Mod, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr AND Expr     { $$ = NEWNODE(node_And, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr OR Expr      { $$ = NEWNODE(node_Or, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr XOR Expr     { $$ = NEWNODE(node_Xor, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr LSHIFT Expr  { $$ = NEWNODE(node_Lshift, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr RSHIFT Expr  { $$ = NEWNODE(node_Rshift, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr EQ Expr      { $$ = NEWNODE(node_Eq, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr NE Expr      { $$ = NEWNODE(node_Ne, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr GE Expr      { $$ = NEWNODE(node_Ge, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr GT Expr      { $$ = NEWNODE(node_Gt, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr LE Expr      { $$ = NEWNODE(node_Le, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }
    | Expr LT Expr      { $$ = NEWNODE(node_Lt, NULL, @2.first_line, @2.first_column); ast_add_child($$, $1); ast_add_child($$, $3); }

    | MINUS Expr %prec UNARY   { $$ = NEWNODE(node_Minus, NULL, @1.first_line, @1.first_column); ast_add_child($$, $2); }
    | NOT Expr %prec UNARY     { $$ = NEWNODE(node_Not, NULL, @1.first_line, @1.first_column); ast_add_child($$, $2); }
    | PLUS Expr %prec UNARY    { $$ = NEWNODE(node_Plus, NULL, @1.first_line, @1.first_column); ast_add_child($$, $2); }
    | LPAR Expr RPAR    { $$ = $2; }
    | LPAR error RPAR   { $$ = NULL; }
    | MethodInvocation  { $$ = $1; }
    | Assignment        { $$ = $1; }
    | ParseArgs         { $$ = $1; }
    | IDENTIFIER DOTLENGTH
        {
            $$ = NEWNODE(node_Length, NULL, @2.first_line, @2.first_column);
            ASTNode *id = NEWNODE(node_Identifier, $1, @1.first_line, @1.first_column);
            ast_add_child($$, id);
            free($1);
        }
    | IDENTIFIER
        {
            $$ = NEWNODE(node_Identifier, $1, @1.first_line, @1.first_column);
            free($1);
        }
    | NATURAL
        {
            $$ = NEWNODE(node_Natural, $1, @1.first_line, @1.first_column);
            free($1);
        }
    | DECIMAL
        {
            $$ = NEWNODE(node_Decimal, $1, @1.first_line, @1.first_column);
            free($1);
        }
    | BOOLLIT
        {
            $$ = NEWNODE(node_BoolLit, $1, @1.first_line, @1.first_column);
            free($1);
        }
    ;

%%

void yyerror(const char *s) {
    printf("Line %d, col %d: %s: %s\n", tok_line, tok_col, s, tok_text);
    syntax_error = 1;
}

int main(int argc, char *argv[]) {
    flag_l = 0; flag_e1 = 0; flag_t = 0; flag_e2 = 0; flag_s = 0; flag_e3 = 0;

    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) {
            flag_l = 1; yylex(); return 0;
        } else if (strcmp(argv[1], "-e1") == 0) {
            flag_e1 = 1; yylex(); return 0;
        } else if (strcmp(argv[1], "-t") == 0) {
            flag_t = 1;
        } else if (strcmp(argv[1], "-e2") == 0) {
            flag_e2 = 1;
        } else if (strcmp(argv[1], "-s") == 0) {
            flag_s = 1;
        } else if (strcmp(argv[1], "-e3") == 0) {
            flag_e3 = 1;
        }
    }

    yyparse();

    if (flag_t && !syntax_error && root) {
        ast_print(root, 0);
    }

    if ((flag_s || flag_e3 || (!flag_t && !flag_e2)) && !syntax_error && root) {
        int sem_errs = semantic_analysis(root);

        if (flag_s) {
            /* Print symbol tables */
            SymTable *t = get_symbol_tables();
            int first = 1;
            while (t) {
                if (!first) printf("\n");
                symtab_print(t);
                first = 0;
                t = t->next;
            }
            /* Print annotated AST */
            printf("\n");
            ast_print(root, 0);
        }

        /* Generate LLVM IR: no flags, no errors */
        if (!flag_s && !flag_e3 && sem_errs == 0) {
            codegen(root, get_symbol_tables());
        }

        semantic_free();
    }

    if (root) ast_free(root);
    return 0;
}

```

## semantics.c

```c
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

```

## semantics.h

```c
/* semantics.h */
#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "ast.h"
#include "symtab.h"

/* Run semantic analysis on the AST. Returns number of errors found. */
int semantic_analysis(ASTNode *root);

/* Get the global symbol table (for printing) */
SymTable *get_symbol_tables(void);

/* Free all semantic data */
void semantic_free(void);

#endif

```

## symtab.c

```c
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

```

## symtab.h

```c
/* symtab.h - Symbol Table */
#ifndef SYMTAB_H
#define SYMTAB_H

typedef struct Symbol {
    char *name;
    char *type;          /* "int", "double", "boolean", "void", "String []", "undef" */
    int is_param;        /* 1 if parameter */
    char *param_types;   /* for methods: "(int,double)" etc. NULL for variables */
    struct Symbol *next;
} Symbol;

typedef struct SymTable {
    char *name;          /* table name: "Class X" or "Method foo(int)" */
    Symbol *symbols;
    Symbol *last;        /* for maintaining insertion order */
    struct SymTable *next;
} SymTable;

/* Create a new symbol table with the given name */
SymTable *symtab_create(const char *name);

/* Add a symbol to a table. Returns 0 on success, 1 if already defined */
int symtab_add(SymTable *table, const char *name, const char *type, int is_param, const char *param_types);

/* Look up a symbol by name in a table */
Symbol *symtab_lookup(SymTable *table, const char *name);

/* Find all methods with a given name in a table */
int symtab_find_methods(SymTable *table, const char *name, Symbol **results, int max_results);

/* Print a symbol table */
void symtab_print(SymTable *table);

/* Free a symbol table */
void symtab_free(SymTable *table);

/* Free entire chain of symbol tables */
void symtab_free_all(SymTable *table);

#endif

```

