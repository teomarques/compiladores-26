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
