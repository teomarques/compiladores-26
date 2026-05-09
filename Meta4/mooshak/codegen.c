/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "codegen.h"

typedef enum {
    CG_Program, CG_FieldDecl, CG_VarDecl, CG_MethodDecl,
    CG_MethodHeader, CG_MethodParams, CG_ParamDecl, CG_MethodBody,
    CG_Block, CG_If, CG_While, CG_Return, CG_Call, CG_Print, CG_ParseArgs, CG_Assign,
    CG_Or, CG_And, CG_Eq, CG_Ne, CG_Lt, CG_Gt, CG_Le, CG_Ge,
    CG_Add, CG_Sub, CG_Mul, CG_Div, CG_Mod,
    CG_Lshift, CG_Rshift, CG_Xor, CG_Not, CG_Minus, CG_Plus, CG_Length,
    CG_Int, CG_Double, CG_Bool, CG_StringArray, CG_Void,
    CG_Identifier, CG_Natural, CG_Decimal, CG_BoolLit, CG_StrLit
} CGKind;

typedef struct CGNode {
    CGKind kind;
    char *token;
    const char *annotation;
    struct CGNode *child;
    struct CGNode *sibling;
} CGNode;

static CGNode *convert_node(struct node *n) {
    if (!n) return NULL;
    CGNode *cn = calloc(1, sizeof(CGNode));
    cn->token = n->token;
    cn->annotation = n->type_annot;
    
    switch (n->category) {
        case N_Program: cn->kind = CG_Program; break;
        case N_FieldDecl: cn->kind = CG_FieldDecl; break;
        case N_VarDecl: cn->kind = CG_VarDecl; break;
        case N_MethodDecl: cn->kind = CG_MethodDecl; break;
        case N_MethodHeader: cn->kind = CG_MethodHeader; break;
        case N_MethodParams: cn->kind = CG_MethodParams; break;
        case N_ParamDecl: cn->kind = CG_ParamDecl; break;
        case N_MethodBody: cn->kind = CG_MethodBody; break;
        case N_Block: cn->kind = CG_Block; break;
        case N_If: cn->kind = CG_If; break;
        case N_While: cn->kind = CG_While; break;
        case N_Return: cn->kind = CG_Return; break;
        case N_Call: cn->kind = CG_Call; break;
        case N_Print: cn->kind = CG_Print; break;
        case N_ParseArgs: cn->kind = CG_ParseArgs; break;
        case N_Assign: cn->kind = CG_Assign; break;
        case N_Or: cn->kind = CG_Or; break;
        case N_And: cn->kind = CG_And; break;
        case N_Eq: cn->kind = CG_Eq; break;
        case N_Ne: cn->kind = CG_Ne; break;
        case N_Lt: cn->kind = CG_Lt; break;
        case N_Gt: cn->kind = CG_Gt; break;
        case N_Le: cn->kind = CG_Le; break;
        case N_Ge: cn->kind = CG_Ge; break;
        case N_Add: cn->kind = CG_Add; break;
        case N_Sub: cn->kind = CG_Sub; break;
        case N_Mul: cn->kind = CG_Mul; break;
        case N_Div: cn->kind = CG_Div; break;
        case N_Mod: cn->kind = CG_Mod; break;
        case N_Lshift: cn->kind = CG_Lshift; break;
        case N_Rshift: cn->kind = CG_Rshift; break;
        case N_Xor: cn->kind = CG_Xor; break;
        case N_Not: cn->kind = CG_Not; break;
        case N_Minus: cn->kind = CG_Minus; break;
        case N_Plus: cn->kind = CG_Plus; break;
        case N_Length: cn->kind = CG_Length; break;
        case N_Int: cn->kind = CG_Int; break;
        case N_Double: cn->kind = CG_Double; break;
        case N_Bool: cn->kind = CG_Bool; break;
        case N_StringArray: cn->kind = CG_StringArray; break;
        case N_Void: cn->kind = CG_Void; break;
        case N_Identifier: cn->kind = CG_Identifier; break;
        case N_Natural: cn->kind = CG_Natural; break;
        case N_Decimal: cn->kind = CG_Decimal; break;
        case N_BoolLit: cn->kind = CG_BoolLit; break;
        case N_StrLit: cn->kind = CG_StrLit; break;
        default: cn->kind = CG_Program; break;
    }

    struct node_list *c = n->children;
    if (c) c = c->next;
    CGNode *last = NULL;
    for (; c; c = c->next) {
        if (!c->node) continue;
        CGNode *cc = convert_node(c->node);
        if (!cn->child) cn->child = cc;
        else last->sibling = cc;
        last = cc;
    }
    return cn;
}

static void free_cgnodes(CGNode *n) {
    if (!n) return;
    free_cgnodes(n->child);
    free_cgnodes(n->sibling);
    free(n);
}

static int temp_cnt = 0;
static int label_cnt = 0;
static int str_cnt = 0;
static int block_terminated = 0;
static int curr_label = -1;
static int g_is_main = 0;

static ClassTable *g_class_table = NULL;
static MethodEntry *g_cur_method = NULL;
static const char *g_cur_ret_type = NULL;

typedef struct { char *val; int len; char name[32]; } StrConst;
static StrConst str_consts[512];
static int n_str_consts = 0;

static int new_temp() { return temp_cnt++; }
static int new_label() { return label_cnt++; }
static void print_label(int l) { printf("L%d:\n", l); curr_label = l; }

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
    if (strcmp(juc, "boolean") == 0) return "false";
    return "0";
}

static void process_strlit(const char *val, char *out, int *out_len) {
    int i = 1, len = strlen(val), oi = 0;
    while (i < len - 1) {
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
        } else { out[oi++] = val[i++]; }
        (*out_len)++;
    }
    out[oi++] = '\\'; out[oi++] = '0'; out[oi++] = '0'; out[oi] = '\0'; (*out_len)++;
}

static const char *find_str_const(const char *val) {
    for (int i = 0; i < n_str_consts; i++) if (strcmp(str_consts[i].val, val) == 0) return str_consts[i].name;
    return "@.str.0";
}

static int find_str_len(const char *val) {
    for (int i = 0; i < n_str_consts; i++) if (strcmp(str_consts[i].val, val) == 0) return str_consts[i].len;
    return 1;
}

static void collect_strings(CGNode *n) {
    if (!n) return;
    if (n->kind == CG_StrLit && n->token) {
        int found = 0;
        for (int i = 0; i < n_str_consts; i++) if (strcmp(str_consts[i].val, n->token) == 0) { found = 1; break; }
        if (!found && n_str_consts < 512) {
            char esc[8192]; int bl = 0; process_strlit(n->token, esc, &bl);
            StrConst *sc = &str_consts[n_str_consts++];
            sc->val = strdup(n->token); sc->len = bl;
            snprintf(sc->name, sizeof(sc->name), "@.str.%d", str_cnt++);
            printf("%s = private unnamed_addr constant [%d x i8] c\"%s\"\n", sc->name, bl, esc);
        }
    }
    collect_strings(n->child); collect_strings(n->sibling);
}

static void mangle_method_name(const char *name, MethodEntry *m, char *out, int outlen) {
    if (!name) { out[0] = '\0'; return; }
    if (strcmp(name, "main") == 0 && m && m->n_params == 1 && m->param_types[0] == JT_STRING_ARRAY) {
        strncpy(out, "main", outlen-1); out[outlen-1] = '\0'; return;
    }
    char buf[512]; snprintf(buf, sizeof(buf), "%s__", name);
    if (m) {
        for (int i = 0; i < m->n_params; i++) {
            if (i > 0) strcat(buf, "__");
            const char *t = jtype_to_string(m->param_types[i]);
            if (strcmp(t, "String[]") == 0) strcat(buf, "StringArray");
            else strcat(buf, t);
        }
    }
    strncpy(out, buf, outlen-1); out[outlen-1] = '\0';
}

static int is_global_by_annotation(const char *name, const char *ann_type) {
    if (!g_class_table) return 0;
    if (g_cur_method) {
        for (Symbol *s = g_cur_method->symbols; s; s = s->next)
            if (s->name && strcmp(s->name, name) == 0) return 0;
    }
    for (Symbol *s = g_class_table->fields; s; s = s->next)
        if (s->name && strcmp(s->name, name) == 0) {
             const char *ft = jtype_to_string(s->type);
             if (ann_type && strcmp(ft, ann_type) == 0) return 1;
        }
    return 0;
}

static MethodEntry *resolve_overload(const char *name, const char *sig) {
    if (!sig || sig[0] != '(') return NULL;
    char s[256]; strncpy(s, sig + 1, 255); s[255] = '\0';
    char *end = strchr(s, ')'); if (end) *end = '\0';
    JType types[64]; int n = 0;
    if (s[0] != '\0') {
        char *p = strtok(s, ",");
        while (p && n < 64) {
            if (strcmp(p, "int") == 0) types[n++] = JT_INT;
            else if (strcmp(p, "double") == 0) types[n++] = JT_DOUBLE;
            else if (strcmp(p, "boolean") == 0) types[n++] = JT_BOOLEAN;
            else if (strcmp(p, "String[]") == 0) types[n++] = JT_STRING_ARRAY;
            p = strtok(NULL, ",");
        }
    }
    for (MethodEntry *m = g_class_table->methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n) {
            int same = 1;
            for (int i = 0; i < n; i++) if (m->param_types[i] != types[i]) { same = 0; break; }
            if (same) return m;
        }
    }
    return NULL;
}

static void emit_method(CGNode *md, MethodEntry *m);
static void emit_statement(CGNode *stmt);
static int emit_expr(CGNode *expr);

static int maybe_convert(int t, const char *from, const char *to) {
    if (!from || !to || strcmp(from, to) == 0) return t;
    if (strcmp(from, "int") == 0 && strcmp(to, "double") == 0) {
        int r = new_temp(); printf("  %%t%d = sitofp i32 %%t%d to double\n", r, t); return r;
    }
    return t;
}

static const char *cg_type_str(CGNode *n) {
    if (!n) return "int";
    switch (n->kind) {
        case CG_Int: return "int";
        case CG_Double: return "double";
        case CG_Bool: return "boolean";
        case CG_StringArray: return "String[]";
        case CG_Void: return "void";
        default: return n->annotation ? n->annotation : "int";
    }
}

void codegen(struct node *root, ClassTable *ct) {
    if (!root || !ct) return;
    CGNode *cgroot = convert_node(root); g_class_table = ct;
    printf("@.fmt.d = private unnamed_addr constant [3 x i8] c\"%%d\\00\"\n");
    printf("@.fmt.e = private unnamed_addr constant [6 x i8] c\"%%.16e\\00\"\n");
    printf("@.fmt.s = private unnamed_addr constant [3 x i8] c\"%%s\\00\"\n");
    printf("@.str.true = private unnamed_addr constant [5 x i8] c\"true\\00\"\n");
    printf("@.str.false = private unnamed_addr constant [6 x i8] c\"false\\00\"\n\n");
    collect_strings(cgroot); if (n_str_consts) printf("\n");
    CGNode *decl = cgroot->child ? cgroot->child->sibling : NULL;
    while (decl) {
        if (decl->kind == CG_FieldDecl) {
            CGNode *ft = decl->child, *fid = ft->sibling;
            while (fid) { printf("@%s = global %s %s\n", fid->token, llvm_type(cg_type_str(ft)), llvm_default(cg_type_str(ft))); fid = fid->sibling; }
        }
        decl = decl->sibling;
    }
    printf("\ndeclare i32 @printf(i8*, ...)\ndeclare i32 @atoi(i8*)\n\n");
    MethodEntry *m_ptr = g_class_table->methods;
    decl = cgroot->child ? cgroot->child->sibling : NULL;
    while (decl) {
        if (decl->kind == CG_MethodDecl) { emit_method(decl, m_ptr); if (m_ptr) m_ptr = m_ptr->next; printf("\n"); }
        decl = decl->sibling;
    }
    free_cgnodes(cgroot);
}

static void emit_method(CGNode *md, MethodEntry *m) {
    CGNode *h = md->child, *body = h->sibling, *rn = h->child, *mid = rn->sibling, *mp = mid->sibling;
    const char *fn = mid->token, *rj = m ? jtype_to_string(m->return_type) : cg_type_str(rn);
    char mangled[256]; mangle_method_name(fn, m, mangled, sizeof(mangled));
    g_is_main = (strcmp(mangled, "main") == 0);
    temp_cnt = 0; label_cnt = 0; block_terminated = 0; curr_label = -1; g_cur_method = m; g_cur_ret_type = rj;
    if (g_is_main) printf("define i32 @main(i32 %%argc, i8** %%argv) {\n");
    else {
        printf("define %s @%s(", llvm_type(rj), mangled);
        CGNode *p = mp->child; int f = 1;
        while (p) {
            CGNode *pt = p->child, *pid = pt->sibling;
            if (!f) printf(", ");
            if (pt->kind == CG_StringArray) printf("i32 %%argc.%s, i8** %%argv.%s", pid->token, pid->token);
            else printf("%s %%param.%s", llvm_type(cg_type_str(pt)), pid->token);
            f = 0; p = p->sibling;
        }
        printf(") {\n");
    }
    printf("entry:\n");
    CGNode *p = mp->child;
    while (p) {
        CGNode *pt = p->child, *pid = pt->sibling;
        if (pt->kind == CG_StringArray) {
            printf("  %%%s.argc = alloca i32\n  %%%s.argv = alloca i8**\n", pid->token, pid->token);
            if (g_is_main) printf("  store i32 %%argc, i32* %%%s.argc\n  store i8** %%argv, i8*** %%%s.argv\n", pid->token, pid->token);
            else printf("  store i32 %%argc.%s, i32* %%%s.argc\n  store i8** %%argv.%s, i8*** %%%s.argv\n", pid->token, pid->token, pid->token, pid->token);
        } else {
            const char *lt = llvm_type(cg_type_str(pt));
            printf("  %%%s = alloca %s\n", pid->token, lt);
            if (!g_is_main) printf("  store %s %%param.%s, %s* %%%s\n", lt, pid->token, lt, pid->token);
        }
        p = p->sibling;
    }
    printf("  %%sc_tmp = alloca i1\n");
    CGNode *s = body->child;
    while (s) {
        if (s->kind == CG_VarDecl) {
            CGNode *vt = s->child, *vid = vt->sibling;
            while (vid) { printf("  %%%s = alloca %s\n  store %s %s, %s* %%%s\n", vid->token, llvm_type(cg_type_str(vt)), llvm_type(cg_type_str(vt)), llvm_default(cg_type_str(vt)), llvm_type(cg_type_str(vt)), vid->token); vid = vid->sibling; }
        }
        s = s->sibling;
    }
    s = body->child;
    while (s) { if (s->kind != CG_VarDecl && !block_terminated) emit_statement(s); s = s->sibling; }
    if (!block_terminated) {
        if (g_is_main) printf("  ret i32 0\n");
        else if (strcmp(rj, "void") == 0) printf("  ret void\n");
        else printf("  ret %s %s\n", llvm_type(rj), llvm_default(rj));
    }
    printf("}\n");
}

static void emit_statement(CGNode *stmt) {
    if (!stmt) return;
    switch (stmt->kind) {
        case CG_If: {
            int c = emit_expr(stmt->child); int lt = new_label(), le = new_label(), ln = new_label();
            printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", c, lt, le);
            print_label(lt); block_terminated = 0; emit_statement(stmt->child->sibling);
            int then_term = block_terminated; if (!block_terminated) printf("  br label %%L%d\n", ln);
            print_label(le); block_terminated = 0;
            if (stmt->child->sibling->sibling && !(stmt->child->sibling->sibling->kind == CG_Block && !stmt->child->sibling->sibling->child))
                emit_statement(stmt->child->sibling->sibling);
            int else_term = block_terminated; if (!block_terminated) printf("  br label %%L%d\n", ln);
            if (!then_term || !else_term) { print_label(ln); block_terminated = 0; }
            else block_terminated = 1;
            break;
        }
        case CG_While: {
            int lc = new_label(), lb = new_label(), ln = new_label();
            printf("  br label %%L%d\n", lc); print_label(lc);
            int c = emit_expr(stmt->child); printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", c, lb, ln);
            print_label(lb); block_terminated = 0; emit_statement(stmt->child->sibling);
            if (!block_terminated) printf("  br label %%L%d\n", lc);
            print_label(ln); block_terminated = 0; break;
        }
        case CG_Return: {
            if (stmt->child) {
                int v = emit_expr(stmt->child); v = maybe_convert(v, stmt->child->annotation, g_cur_ret_type);
                if (g_is_main) printf("  ret i32 %%t%d\n", v);
                else printf("  ret %s %%t%d\n", llvm_type(g_cur_ret_type), v);
            } else {
                if (g_is_main) printf("  ret i32 0\n");
                else printf("  ret void\n");
            }
            block_terminated = 1; break;
        }
        case CG_Print: {
            CGNode *arg = stmt->child;
            if (arg->kind == CG_StrLit) {
                const char *sn = find_str_const(arg->token); int sl = find_str_len(arg->token);
                int p = new_temp(), f = new_temp(), d = new_temp();
                printf("  %%t%d = getelementptr [%d x i8], [%d x i8]* %s, i32 0, i32 0\n  %%t%d = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0\n  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, i8* %%t%d)\n", p, sl, sl, sn, f, d, f, p);
            } else {
                int v = emit_expr(arg); const char *at = arg->annotation ? arg->annotation : "int";
                if (strcmp(at, "boolean") == 0) {
                    int ltr = new_label(), lfa = new_label(), ln = new_label();
                    printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", v, ltr, lfa);
                    print_label(ltr); int pt = new_temp(), f1 = new_temp(), d1 = new_temp();
                    printf("  %%t%d = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0\n  %%t%d = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0\n  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, i8* %%t%d)\n  br label %%L%d\n", pt, f1, d1, f1, pt, ln);
                    print_label(lfa); int pf = new_temp(), f2 = new_temp(), d2 = new_temp();
                    printf("  %%t%d = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0\n  %%t%d = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0\n  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, i8* %%t%d)\n  br label %%L%d\n", pf, f2, d2, f2, pf, ln);
                    print_label(ln);
                } else if (strcmp(at, "double") == 0) {
                    int f = new_temp(), d = new_temp();
                    printf("  %%t%d = getelementptr [6 x i8], [6 x i8]* @.fmt.e, i32 0, i32 0\n  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, double %%t%d)\n", f, d, f, v);
                } else {
                    int f = new_temp(), d = new_temp();
                    printf("  %%t%d = getelementptr [3 x i8], [3 x i8]* @.fmt.d, i32 0, i32 0\n  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, i32 %%t%d)\n", f, d, f, v);
                }
            }
            break;
        }
        case CG_Block: { CGNode *c = stmt->child; while (c) { if (!block_terminated) emit_statement(c); c = c->sibling; } break; }
        case CG_Assign: case CG_Call: case CG_ParseArgs: emit_expr(stmt); break;
        default: break;
    }
}

static int emit_expr(CGNode *expr) {
    if (!expr) return 0;
    int r;
    switch (expr->kind) {
        case CG_Natural: {
            r = new_temp(); char cl[256]; int ci = 0; for (int i = 0; expr->token[i]; i++) if (expr->token[i] != '_') cl[ci++] = expr->token[i];
            cl[ci] = '\0'; printf("  %%t%d = add i32 0, %s\n", r, cl); return r;
        }
        case CG_Decimal: {
            r = new_temp(); char cl[512]; int ci = 0; for (int i = 0; expr->token[i]; i++) if (expr->token[i] != '_') cl[ci++] = expr->token[i];
            cl[ci] = '\0'; printf("  %%t%d = fadd double 0.0, %.17e\n", r, strtod(cl, NULL)); return r;
        }
        case CG_BoolLit: { r = new_temp(); printf("  %%t%d = add i1 0, %d\n", r, strcmp(expr->token, "true") == 0); return r; }
        case CG_Identifier: {
            const char *jt = expr->annotation ? expr->annotation : "int"; if (strcmp(jt, "String[]") == 0) return 0;
            r = new_temp(); if (is_global_by_annotation(expr->token, jt)) printf("  %%t%d = load %s, %s* @%s\n", r, llvm_type(jt), llvm_type(jt), expr->token);
            else printf("  %%t%d = load %s, %s* %%%s\n", r, llvm_type(jt), llvm_type(jt), expr->token);
            return r;
        }
        case CG_Add: case CG_Sub: case CG_Mul: case CG_Div: {
            int l = emit_expr(expr->child), rr = emit_expr(expr->child->sibling);
            const char *res = expr->annotation ? expr->annotation : "int";
            if (strcmp(res, "double") == 0) {
                l = maybe_convert(l, expr->child->annotation, "double"); rr = maybe_convert(rr, expr->child->sibling->annotation, "double");
                r = new_temp(); const char *op = (expr->kind == CG_Add) ? "fadd" : (expr->kind == CG_Sub) ? "fsub" : (expr->kind == CG_Mul) ? "fmul" : "fdiv";
                printf("  %%t%d = %s double %%t%d, %%t%d\n", r, op, l, rr);
            } else {
                r = new_temp(); const char *op = (expr->kind == CG_Add) ? "add" : (expr->kind == CG_Sub) ? "sub" : (expr->kind == CG_Mul) ? "mul" : "sdiv";
                printf("  %%t%d = %s i32 %%t%d, %%t%d\n", r, op, l, rr);
            }
            return r;
        }
        case CG_Mod: {
            int l = emit_expr(expr->child), rr = emit_expr(expr->child->sibling); r = new_temp();
            if (expr->annotation && strcmp(expr->annotation, "double") == 0) {
                l = maybe_convert(l, expr->child->annotation, "double"); rr = maybe_convert(rr, expr->child->sibling->annotation, "double");
                printf("  %%t%d = frem double %%t%d, %%t%d\n", r, l, rr);
            } else printf("  %%t%d = srem i32 %%t%d, %%t%d\n", r, l, rr);
            return r;
        }
        case CG_Lshift: case CG_Rshift: {
            int l = emit_expr(expr->child), rr = emit_expr(expr->child->sibling); r = new_temp();
            printf("  %%t%d = %s i32 %%t%d, %%t%d\n", r, (expr->kind == CG_Lshift) ? "shl" : "ashr", l, rr); return r;
        }
        case CG_Xor: {
            int l = emit_expr(expr->child), rr = emit_expr(expr->child->sibling); r = new_temp();
            const char *jt = expr->annotation ? expr->annotation : "int";
            printf("  %%t%d = xor %s %%t%d, %%t%d\n", r, llvm_type(jt), l, rr); return r;
        }
        case CG_And: {
            int lt = new_label(), lf = new_label(), ln = new_label();
            int l = emit_expr(expr->child);
            printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", l, lt, lf);
            print_label(lt); int rr = emit_expr(expr->child->sibling); int lb_rr = curr_label;
            printf("  br label %%L%d\n", ln);
            print_label(lf); printf("  br label %%L%d\n", ln);
            print_label(ln); r = new_temp();
            if (lb_rr == -1) printf("  %%t%d = phi i1 [ %%t%d, %%entry ], [ false, %%L%d ]\n", r, rr, lf);
            else printf("  %%t%d = phi i1 [ %%t%d, %%L%d ], [ false, %%L%d ]\n", r, rr, lb_rr, lf);
            return r;
        }
        case CG_Or: {
            int lt = new_label(), lf = new_label(), ln = new_label();
            int l = emit_expr(expr->child);
            printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", l, lt, lf);
            print_label(lf); int rr = emit_expr(expr->child->sibling); int lb_rr = curr_label;
            printf("  br label %%L%d\n", ln);
            print_label(lt); printf("  br label %%L%d\n", ln);
            print_label(ln); r = new_temp();
            if (lb_rr == -1) printf("  %%t%d = phi i1 [ %%t%d, %%entry ], [ true, %%L%d ]\n", r, rr, lt);
            else printf("  %%t%d = phi i1 [ %%t%d, %%L%d ], [ true, %%L%d ]\n", r, rr, lb_rr, lt);
            return r;
        }
        case CG_Eq: case CG_Ne: case CG_Lt: case CG_Gt: case CG_Le: case CG_Ge: {
            int l = emit_expr(expr->child), rr = emit_expr(expr->child->sibling);
            const char *lt = expr->child->annotation, *rt = expr->child->sibling->annotation;
            if ((lt && strcmp(lt, "double") == 0) || (rt && strcmp(rt, "double") == 0)) {
                l = maybe_convert(l, lt, "double"); rr = maybe_convert(rr, rt, "double");
                r = new_temp(); const char *op;
                switch (expr->kind) { case CG_Eq: op = "oeq"; break; case CG_Ne: op = "one"; break; case CG_Lt: op = "olt"; break; case CG_Gt: op = "ogt"; break; case CG_Le: op = "ole"; break; default: op = "oge"; break; }
                printf("  %%t%d = fcmp %s double %%t%d, %%t%d\n", r, op, l, rr);
            } else {
                r = new_temp(); const char *op; const char *ltp = (lt && strcmp(lt, "boolean") == 0) ? "i1" : "i32";
                switch (expr->kind) { case CG_Eq: op = "eq"; break; case CG_Ne: op = "ne"; break; case CG_Lt: op = "slt"; break; case CG_Gt: op = "sgt"; break; case CG_Le: op = "sle"; break; default: op = "sge"; break; }
                printf("  %%t%d = icmp %s %s %%t%d, %%t%d\n", r, op, ltp, l, rr);
            }
            return r;
        }
        case CG_Not: { int v = emit_expr(expr->child); r = new_temp(); printf("  %%t%d = xor i1 %%t%d, 1\n", r, v); return r; }
        case CG_Minus: {
            int v = emit_expr(expr->child); r = new_temp();
            if (expr->annotation && strcmp(expr->annotation, "double") == 0) printf("  %%t%d = fsub double -0.0, %%t%d\n", r, v);
            else printf("  %%t%d = sub i32 0, %%t%d\n", r, v);
            return r;
        }
        case CG_Plus: return emit_expr(expr->child);
        case CG_Length: {
            int ac = new_temp(); printf("  %%t%d = load i32, i32* %%%s.argc\n", ac, expr->child->token);
            r = new_temp(); printf("  %%t%d = sub i32 %%t%d, 1\n", r, ac); return r;
        }
        case CG_Assign: {
            int v = emit_expr(expr->child->sibling); const char *lt = expr->child->annotation ? expr->child->annotation : "int";
            v = maybe_convert(v, expr->child->sibling->annotation, lt);
            if (is_global_by_annotation(expr->child->token, lt)) printf("  store %s %%t%d, %s* @%s\n", llvm_type(lt), v, llvm_type(lt), expr->child->token);
            else printf("  store %s %%t%d, %s* %%%s\n", llvm_type(lt), v, llvm_type(lt), expr->child->token);
            return v;
        }
        case CG_Call: {
            CGNode *id = expr->child; MethodEntry *m = resolve_overload(id->token, id->annotation);
            char mangled[256]; mangle_method_name(id->token, m, mangled, sizeof(mangled));
            int ts[128]; const char *ltps[128]; int ncs = 0; CGNode *an = id->sibling; int fi = 0;
            while (an) {
                if (an->annotation && strcmp(an->annotation, "String[]") == 0) {
                    int ac = new_temp(); printf("  %%t%d = load i32, i32* %%%s.argc\n", ac, an->token); ts[ncs] = ac; ltps[ncs++] = "i32";
                    int av = new_temp(); printf("  %%t%d = load i8**, i8*** %%%s.argv\n", av, an->token); ts[ncs] = av; ltps[ncs++] = "i8**";
                } else {
                    int v = emit_expr(an); const char *fo = (m && fi < m->n_params) ? jtype_to_string(m->param_types[fi]) : NULL;
                    if (fo) v = maybe_convert(v, an->annotation, fo);
                    ts[ncs] = v; ltps[ncs++] = llvm_type(fo ? fo : an->annotation);
                }
                an = an->sibling; fi++;
            }
            const char *rj = expr->annotation ? expr->annotation : (m ? jtype_to_string(m->return_type) : "void");
            if (strcmp(rj, "void") == 0) { printf("  call void @%s(", mangled); for (int i = 0; i < ncs; i++) { if (i) printf(", "); printf("%s %%t%d", ltps[i], ts[i]); } printf(")\n"); return 0; }
            else { r = new_temp(); printf("  %%t%d = call %s @%s(", r, llvm_type(rj), mangled); for (int i = 0; i < ncs; i++) { if (i) printf(", "); printf("%s %%t%d", ltps[i], ts[i]); } printf(")\n"); return r; }
        }
        case CG_ParseArgs: {
            int iv = emit_expr(expr->child->sibling); int adj = new_temp(); printf("  %%t%d = add i32 %%t%d, 1\n", adj, iv);
            int ap = new_temp(); printf("  %%t%d = load i8**, i8*** %%%s.argv\n", ap, expr->child->token);
            int gep = new_temp(); printf("  %%t%d = getelementptr i8*, i8** %%t%d, i32 %%t%d\n", gep, ap, adj);
            int sp = new_temp(); printf("  %%t%d = load i8*, i8** %%t%d\n", sp, gep);
            r = new_temp(); printf("  %%t%d = call i32 @atoi(i8* %%t%d)\n", r, sp); return r;
        }
        default: return 0;
    }
}
