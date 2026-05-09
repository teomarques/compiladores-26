/* codegen.c - LLVM IR generation for Meta3 AST (Meta4 features)
 * This file implements a code generator that walks the Meta3 AST (struct node)
 * and emits LLVM IR to stdout. It was written to integrate with the existing
 * Meta3 semantic data structures (ClassTable, MethodEntry, Symbol).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "codegen.h"

/* We transform the Meta3 AST (struct node + node_list children) into a small
 * internal tree of CGNode nodes that have convenient 'child' and 'sibling'
 * pointers and fields similar to the Meta4 generator. This keeps the core
 * generator code straightforward while not modifying the original AST. */

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
    char *token;         /* identifier name, literal text, etc. */
    const char *annotation; /* pointer into original annotation string */
    struct CGNode *child;
    struct CGNode *sibling;
} CGNode;

/* Convert Meta3 node->node_list children into CGNode child/sibling links. */
static CGNode *convert_node(struct node *n) {
    if (!n) return NULL;
    CGNode *cn = malloc(sizeof(CGNode));
    memset(cn, 0, sizeof(*cn));
    cn->token = n->token;
    cn->annotation = n->type_annot;
    /* map category */
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

    /* Convert children (node_list with sentinel). The grammar created a sentinel
     * first child in many nodes; we iterate all children and link converted nodes
     * as siblings. */
    struct node_list *c = n->children;
    CGNode *last = NULL;
    if (c) c = c->next; /* skip sentinel when present (common pattern in Meta3) */
    for (; c; c = c->next) {
        if (!c->node) continue;
        CGNode *cc = convert_node(c->node);
        if (!cn->child) cn->child = cc;
        else last->sibling = cc;
        last = cc;
    }
    return cn;
}

/* Free CGNode tree (not freeing token/annotation pointers since they point
 * to original AST memory managed elsewhere). */
static void free_cgnodes(CGNode *n) {
    if (!n) return;
    free_cgnodes(n->child);
    free_cgnodes(n->sibling);
    free(n);
}

/* --- The generator implementation below is adapted from the Meta4 generator
 * to operate on CGNode and the Meta3 ClassTable / MethodEntry structures.
 */

static int temp_cnt = 0;
static int label_cnt = 0;
static int str_cnt = 0;
static int block_terminated = 0;

static ClassTable *g_class_table = NULL;
static MethodEntry *g_method_entry = NULL;
static const char *cur_ret_type = NULL;

typedef struct { char *val; int len; char name[32]; } StrConst;
static StrConst str_consts[512];
static int n_str_consts = 0;

static int new_temp(void) { return temp_cnt++; }
static int new_label(void) { return label_cnt++; }

static const char *llvm_type_from_jtype_str(const char *s) {
    if (!s) return "i32";
    if (strcmp(s, "int") == 0) return "i32";
    if (strcmp(s, "double") == 0) return "double";
    if (strcmp(s, "boolean") == 0) return "i1";
    if (strcmp(s, "void") == 0) return "void";
    if (strcmp(s, "String[]") == 0) return "i8**";
    return "i32";
}

static const char *llvm_default_for(const char *s) {
    if (!s) return "0";
    if (strcmp(s, "double") == 0) return "0.0";
    return "0";
}

/* string escaping for LLVM constant emission */
static void process_strlit(const char *val, char *out, int *out_len) {
    int i = 1; /* skip '"' */
    int L = strlen(val);
    int oi = 0;
    while (i < L - 1) {
        if (val[i] == '\\' && i + 1 < L - 1) {
            char c = val[i+1];
            switch (c) {
                case 'n': out[oi++] = '\\'; out[oi++] = '0'; out[oi++] = 'A'; (*out_len)++; break;
                case 't': out[oi++] = '\\'; out[oi++] = '0'; out[oi++] = '9'; (*out_len)++; break;
                case 'r': out[oi++] = '\\'; out[oi++] = '0'; out[oi++] = 'D'; (*out_len)++; break;
                case 'f': out[oi++] = '\\'; out[oi++] = '0'; out[oi++] = 'C'; (*out_len)++; break;
                case '\\': out[oi++] = '\\'; out[oi++] = '5'; out[oi++] = 'C'; (*out_len)++; break;
                case '"': out[oi++] = '\\'; out[oi++] = '2'; out[oi++] = '2'; (*out_len)++; break;
                default:
                    out[oi++] = val[i]; (*out_len)++; out[oi++] = val[i+1]; (*out_len)++; break;
            }
            i += 2;
        } else {
            out[oi++] = val[i++]; (*out_len)++;
        }
    }
    out[oi++] = '\\'; out[oi++] = '0'; out[oi++] = '0'; (*out_len)++;
    out[oi] = '\0';
}

static const char *find_str_name(const char *val) {
    for (int i = 0; i < n_str_consts; i++) if (strcmp(str_consts[i].val, val) == 0) return str_consts[i].name;
    return "@.str.0";
}

/* Collect string literals and print their globals */
static void collect_strings_cg(CGNode *n) {
    if (!n) return;
    if (n->kind == CG_StrLit && n->token) {
        if (!find_str_name(n->token) && n_str_consts < 512) {
            int bytelen = 0;
            char esc[8192]; esc[0] = '\0';
            process_strlit(n->token, esc, &bytelen);
            StrConst *sc = &str_consts[n_str_consts];
            sc->val = strdup(n->token);
            sc->len = bytelen;
            snprintf(sc->name, sizeof(sc->name), "@.str.%d", str_cnt++);
            printf("%s = private unnamed_addr constant [%d x i8] c\"%s\"\n", sc->name, sc->len, esc);
            n_str_consts++;
        }
    }
    collect_strings_cg(n->child);
    collect_strings_cg(n->sibling);
}

/* Helper to find MethodEntry by name and param signature (string like (int,double)) */
static MethodEntry *find_methodentry_by_signature(ClassTable *ct, const char *name, const char *sig) {
    for (MethodEntry *m = ct->methods; m; m = m->next) {
        if (!m->name) continue;
        if (strcmp(m->name, name) != 0) continue;
        int plen = m->n_params;
        char buf[512]; buf[0] = '('; buf[1] = '\0';
        for (int i = 0; i < plen; i++) {
            if (i > 0) strcat(buf, ",");
            strcat(buf, jtype_to_string(m->param_types[i]));
        }
        strcat(buf, ")");
        if (strcmp(buf, sig) == 0) return m;
    }
    return NULL;
}

/* Mangle method name as requested by the specification */
static void mangle_method_name(const char *name, MethodEntry *m, char *out, int outlen) {
    if (!name) { out[0] = '\0'; return; }
    if (strcmp(name, "main") == 0 && m && m->n_params == 1 && m->param_types[0] == JT_STRING_ARRAY) {
        strncpy(out, "main", outlen-1); out[outlen-1] = '\0'; return;
    }
    char buf[512]; buf[0] = '\0'; strcat(buf, name);
    if (!m || m->n_params == 0) { strcat(buf, "__"); strncpy(out, buf, outlen-1); out[outlen-1] = '\0'; return; }
    for (int i = 0; i < m->n_params; i++) {
        char tbuf[64]; strcpy(tbuf, jtype_to_string(m->param_types[i]));
        char safe[64]; int si = 0;
        for (int k = 0; tbuf[k] && si < 62; k++) if (tbuf[k] != ' ' && tbuf[k] != '[' && tbuf[k] != ']') safe[si++] = tbuf[k];
        safe[si] = '\0';
        strcat(buf, "__"); strcat(buf, safe);
    }
    strncpy(out, buf, outlen-1); out[outlen-1] = '\0';
}

/* is this name a class-level field? Use class table lookup */
static int is_class_field(ClassTable *ct, const char *name) {
    if (!ct || !name) return 0;
    for (Symbol *s = ct->fields; s; s = s->next) if (s->name && strcmp(s->name, name) == 0) return 1;
    return 0;
}

/* Check whether a reference annotated with ann_type should be treated as global
 * (approximation based on class fields and method locals available in MethodEntry) */
static int is_global_by_annotation(const char *name, const char *ann_type) {
    if (!g_class_table) return 0;
    if (g_method_entry) {
        for (Symbol *s = g_method_entry->symbols; s; s = s->next) {
            if (s->name && strcmp(s->name, name) == 0) return 0;
        }
    }
    for (Symbol *s = g_class_table->fields; s; s = s->next) {
        if (s->name && strcmp(s->name, name) == 0) {
            if (!ann_type) return 1;
            if (strcmp(jtype_to_string(s->type), ann_type) == 0) return 1;
        }
    }
    return 0;
}

/* Forward declarations of codegen functions (emitters) */
static void emit_method(CGNode *md);
static void emit_statement(CGNode *stmt);
static int emit_expr(CGNode *expr);
static int maybe_convert(int t, const char *from, const char *to);

/* Top-level codegen entry point */
void codegen(struct node *root, ClassTable *ct) {
    if (!root || !ct) return;
    CGNode *cgroot = convert_node(root);
    g_class_table = ct;
    temp_cnt = label_cnt = str_cnt = 0;
    printf("@.fmt.d = private unnamed_addr constant [3 x i8] c\"%%d\\00\"\n");
    printf("@.fmt.e = private unnamed_addr constant [6 x i8] c\"%%.16e\\00\"\n");
    printf("@.fmt.s = private unnamed_addr constant [3 x i8] c\"%%s\\00\"\n");
    printf("@.str.true = private unnamed_addr constant [5 x i8] c\"true\\00\"\n");
    printf("@.str.false = private unnamed_addr constant [6 x i8] c\"false\\00\"\n\n");
    collect_strings_cg(cgroot);
    if (n_str_consts) printf("\n");
    CGNode *decl = cgroot->child ? cgroot->child->sibling : NULL;
    while (decl) {
        if (decl->kind == CG_FieldDecl) {
            CGNode *ft = decl->child; CGNode *fid = ft ? ft->sibling : NULL; const char *jstr = ft && ft->annotation ? ft->annotation : "int";
            printf("@%s = global %s %s\n", fid?fid->token:"_unknown", llvm_type_from_jtype_str(jstr), llvm_default_for(jstr));
        }
        decl = decl->sibling;
    }
    printf("\n");
    printf("declare i32 @printf(i8*, ...)\n");
    printf("declare i32 @atoi(i8*)\n\n");
    decl = cgroot->child ? cgroot->child->sibling : NULL;
    while (decl) {
        if (decl->kind == CG_MethodDecl) { emit_method(decl); printf("\n"); }
        decl = decl->sibling;
    }
    free_cgnodes(cgroot);
}

/* Helper: resolve overload using ClassTable and annotations */
static MethodEntry *resolve_overload(const char *name, const char **arg_annots, int nargs) {
    MethodEntry *exact = NULL; MethodEntry *promo = NULL;
    for (MethodEntry *m = g_class_table->methods; m; m = m->next) {
        if (!m->name || strcmp(m->name, name) != 0) continue;
        if (m->n_params != nargs) continue;
        int all_eq = 1; int all_promo_ok = 1;
        for (int i = 0; i < nargs; i++) {
            const char *formal = jtype_to_string(m->param_types[i]);
            const char *actual = arg_annots[i] ? arg_annots[i] : "int";
            if (strcmp(formal, actual) != 0) { all_eq = 0; if (!(strcmp(formal, "double")==0 && strcmp(actual, "int")==0)) { all_promo_ok = 0; break; } }
        }
        if (all_eq) return m;
        if (all_promo_ok && !promo) promo = m;
    }
    return promo;
}

static void mangle_from_method(MethodEntry *m, const char *name, char *out, int outlen) {
    if (!name) { out[0]='\0'; return; }
    if (strcmp(name, "main") == 0 && m && m->n_params==1 && m->param_types[0]==JT_STRING_ARRAY) { strncpy(out, "main", outlen-1); out[outlen-1]='\0'; return; }
    char buf[512]; strcpy(buf, name);
    if (!m || m->n_params==0) { strcat(buf, "__"); strncpy(out, buf, outlen-1); out[outlen-1]='\0'; return; }
    for (int i=0;i<m->n_params;i++) { char *t = (char*)jtype_to_string(m->param_types[i]); char safe[64]; int si=0; for (int k=0;t[k] && si<62;k++) if (t[k]!=' ' && t[k]!='[' && t[k]!=']') safe[si++]=t[k]; safe[si]='\0'; strcat(buf, "__"); strcat(buf, safe); }
    strncpy(out, buf, outlen-1); out[outlen-1]='\0';
}

static void emit_method(CGNode *md) {
    CGNode *header = md->child; CGNode *body = header ? header->sibling : NULL; CGNode *ret_type = header ? header->child : NULL; CGNode *method_id = ret_type ? ret_type->sibling : NULL; CGNode *method_params = method_id ? method_id->sibling : NULL;
    const char *fname = method_id ? method_id->token : "_anon";
    const char *ret_juc = ret_type && ret_type->annotation ? ret_type->annotation : "void";
    int nparams = 0; for (CGNode *p = method_params?method_params->child:NULL; p; p = p->sibling) if (p->kind==CG_ParamDecl) nparams++;
    MethodEntry *matched = NULL;
    for (MethodEntry *m = g_class_table->methods; m; m = m->next) if (m->name && strcmp(m->name, fname)==0 && m->n_params==nparams) { matched = m; break; }
    char mangled[256]; mangle_method_name(fname, matched, mangled, sizeof(mangled)); int is_main = strcmp(mangled, "main")==0;
    temp_cnt = 0; block_terminated = 0; cur_ret_type = ret_juc; g_method_entry = matched;
    if (is_main) printf("define i32 @main(i32 %%argc, i8** %%argv) {\n"); else { printf("define %s @%s(", llvm_type_from_jtype_str(ret_juc), mangled); int first=1; for (CGNode *p=method_params?method_params->child:NULL;p;p=p->sibling){ if (p->kind!=CG_ParamDecl) continue; CGNode *pt = p->child; CGNode *pid = pt?pt->sibling:NULL; const char *pj = pt && pt->annotation?pt->annotation:"int"; if (!first) printf(", "); if (strcmp(pj,"String[]")==0) printf("i32 %%argc.%s, i8** %%argv.%s", pid?pid->token:"arg", pid?pid->token:"arg"); else printf("%s %%param.%s", llvm_type_from_jtype_str(pj), pid?pid->token:"p"); first=0; } printf(") {\n"); }
    printf("entry:\n");
    for (CGNode *p = method_params?method_params->child:NULL; p; p = p->sibling) {
        if (p->kind!=CG_ParamDecl) continue; CGNode *pt=p->child; CGNode *pid=pt?pt->sibling:NULL; const char *pj = pt&&pt->annotation?pt->annotation:"int";
        if (strcmp(pj,"String[]")==0) { printf("  %%%s.argc = alloca i32\n", pid?pid->token:"arg"); printf("  %%%s.argv = alloca i8**\n", pid?pid->token:"arg"); if (is_main) { printf("  store i32 %%argc, i32* %%%s.argc\n", pid?pid->token:"arg"); printf("  store i8** %%argv, i8*** %%%s.argv\n", pid?pid->token:"arg"); } else { printf("  store i32 %%argc.%s, i32* %%%s.argc\n", pid?pid->token:"arg", pid?pid->token:"arg"); printf("  store i8** %%argv.%s, i8*** %%%s.argv\n", pid?pid->token:"arg", pid?pid->token:"arg"); } }
        else if (!is_main) { const char *ptt = llvm_type_from_jtype_str(pt?pt->annotation:NULL); printf("  %%%s = alloca %s\n", pid?pid->token:"p", ptt); printf("  store %s %%param.%s, %s* %%%s\n", ptt, pid?pid->token:"p", ptt, pid?pid->token:"p"); }
    }
    for (CGNode *s = body?body->child:NULL; s; s = s->sibling) { if (s->kind==CG_VarDecl) { CGNode *vt=s->child; CGNode *vid=vt?vt->sibling:NULL; const char *vlt = vt&&vt->annotation?vt->annotation:"int"; printf("  %%%s = alloca %s\n", vid?vid->token:"v", llvm_type_from_jtype_str(vlt)); printf("  store %s %s, %s* %%%s\n", llvm_type_from_jtype_str(vlt), llvm_default_for(vlt), llvm_type_from_jtype_str(vlt), vid?vid->token:"v"); } }
    for (CGNode *s = body?body->child:NULL; s; s = s->sibling) { if (s->kind==CG_VarDecl) continue; if (!block_terminated) emit_statement(s); }
    if (!block_terminated) { if (is_main) printf("  ret i32 0\n"); else if (strcmp(ret_juc,"void")==0) printf("  ret void\n"); else if (strcmp(ret_juc,"double")==0) printf("  ret double 0.0\n"); else if (strcmp(ret_juc,"boolean")==0) printf("  ret i1 0\n"); else printf("  ret i32 0\n"); }
    printf("}\n");
}

static int maybe_convert(int t, const char *from, const char *to) { if (!from||!to) return t; if (strcmp(from,"int")==0 && strcmp(to,"double")==0) { int r=new_temp(); printf("  %%t%d = sitofp i32 %%t%d to double\n", r, t); return r; } return t; }

static int emit_expr(CGNode *expr) {
    if (!expr) return 0; int r;
    switch (expr->kind) {
        case CG_Natural: { r=new_temp(); char clean[256]; int ci=0; for (int i=0; expr->token && expr->token[i]; i++) if (expr->token[i] != '_') clean[ci++] = expr->token[i]; clean[ci]='\0'; printf("  %%t%d = add i32 0, %s\n", r, clean); return r; }
        case CG_Decimal: { r=new_temp(); char clean[512]; int ci=0; for (int i=0; expr->token && expr->token[i]; i++) if (expr->token[i] != '_') clean[ci++] = expr->token[i]; clean[ci]='\0'; double val = strtod(clean, NULL); printf("  %%t%d = fadd double 0.0, %.17e\n", r, val); return r; }
        case CG_BoolLit: { r=new_temp(); int v=(expr->token && strcmp(expr->token,"true")==0)?1:0; printf("  %%t%d = add i1 0, %d\n", r, v); return r; }
        case CG_Identifier: { const char *juc_t = expr->annotation?expr->annotation:"int"; if (strcmp(juc_t,"String[]")==0) return 0; r=new_temp(); const char *t=llvm_type_from_jtype_str(juc_t); if (is_global_by_annotation(expr->token, expr->annotation)) printf("  %%t%d = load %s, %s* @%s\n", r, t, t, expr->token); else printf("  %%t%d = load %s, %s* %%%s\n", r, t, t, expr->token); return r; }
        case CG_Add: case CG_Sub: case CG_Mul: case CG_Div: { int left=emit_expr(expr->child); int right=emit_expr(expr->child->sibling); const char *res = expr->annotation; if (res && strcmp(res,"double")==0) { left=maybe_convert(left, expr->child->annotation, "double"); right=maybe_convert(right, expr->child->sibling->annotation, "double"); r=new_temp(); const char *op=(expr->kind==CG_Add)?"fadd":(expr->kind==CG_Sub)?"fsub":(expr->kind==CG_Mul)?"fmul":"fdiv"; printf("  %%t%d = %s double %%t%d, %%t%d\n", r, op, left, right); } else { r=new_temp(); const char *op=(expr->kind==CG_Add)?"add":(expr->kind==CG_Sub)?"sub":(expr->kind==CG_Mul)?"mul":"sdiv"; printf("  %%t%d = %s i32 %%t%d, %%t%d\n", r, op, left, right); } return r; }
        case CG_Mod: { int left=emit_expr(expr->child); int right=emit_expr(expr->child->sibling); r=new_temp(); if (expr->annotation && strcmp(expr->annotation,"double")==0) printf("  %%t%d = frem double %%t%d, %%t%d\n", r, left, right); else printf("  %%t%d = srem i32 %%t%d, %%t%d\n", r, left, right); return r; }
        case CG_Lshift: { int l=emit_expr(expr->child); int rr=emit_expr(expr->child->sibling); r=new_temp(); printf("  %%t%d = shl i32 %%t%d, %%t%d\n", r, l, rr); return r; }
        case CG_Rshift: { int l=emit_expr(expr->child); int rr=emit_expr(expr->child->sibling); r=new_temp(); printf("  %%t%d = ashr i32 %%t%d, %%t%d\n", r, l, rr); return r; }
        case CG_Xor: { int l=emit_expr(expr->child); int rr=emit_expr(expr->child->sibling); r=new_temp(); if (expr->annotation && strcmp(expr->annotation,"boolean")==0) printf("  %%t%d = xor i1 %%t%d, %%t%d\n", r, l, rr); else printf("  %%t%d = xor i32 %%t%d, %%t%d\n", r, l, rr); return r; }
        case CG_And: { int sc_slot=new_temp(); printf("  %%t%d = alloca i1\n", sc_slot); printf("  store i1 0, i1* %%t%d\n", sc_slot); int lval=emit_expr(expr->child); int leval=new_label(), lend=new_label(); printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", lval, leval, lend); printf("L%d:\n", leval); block_terminated=0; int rval=emit_expr(expr->child->sibling); printf("  store i1 %%t%d, i1* %%t%d\n", rval, sc_slot); printf("  br label %%L%d\n", lend); printf("L%d:\n", lend); block_terminated=0; r=new_temp(); printf("  %%t%d = load i1, i1* %%t%d\n", r, sc_slot); return r; }
        case CG_Or: { int sc_slot=new_temp(); printf("  %%t%d = alloca i1\n", sc_slot); printf("  store i1 1, i1* %%t%d\n", sc_slot); int lval=emit_expr(expr->child); int lend=new_label(), leval=new_label(); printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", lval, lend, leval); printf("L%d:\n", leval); block_terminated=0; int rval=emit_expr(expr->child->sibling); printf("  store i1 %%t%d, i1* %%t%d\n", rval, sc_slot); printf("  br label %%L%d\n", lend); printf("L%d:\n", lend); block_terminated=0; r=new_temp(); printf("  %%t%d = load i1, i1* %%t%d\n", r, sc_slot); return r; }
        case CG_Eq: case CG_Ne: case CG_Lt: case CG_Gt: case CG_Le: case CG_Ge: { int left=emit_expr(expr->child); int right=emit_expr(expr->child->sibling); const char *lt=expr->child->annotation; const char *rt=expr->child->sibling->annotation; int use_float=(lt&&strcmp(lt,"double")==0)||(rt&&strcmp(rt,"double")==0); if (use_float) { left=maybe_convert(left, lt, "double"); right=maybe_convert(right, rt, "double"); r=new_temp(); const char *op; switch(expr->kind){case CG_Eq:op="oeq";break;case CG_Ne:op="one";break;case CG_Lt:op="olt";break;case CG_Gt:op="ogt";break;case CG_Le:op="ole";break;default:op="oge";break;} printf("  %%t%d = fcmp %s double %%t%d, %%t%d\n", r, op, left, right); } else { r=new_temp(); const char *op; switch(expr->kind){case CG_Eq:op="eq";break;case CG_Ne:op="ne";break;case CG_Lt:op="slt";break;case CG_Gt:op="sgt";break;case CG_Le:op="sle";break;default:op="sge";break;} printf("  %%t%d = icmp %s i32 %%t%d, %%t%d\n", r, op, left, right); } return r; }
        case CG_Not: { int val=emit_expr(expr->child); r=new_temp(); printf("  %%t%d = xor i1 %%t%d, 1\n", r, val); return r; }
        case CG_Minus: { int val=emit_expr(expr->child); r=new_temp(); if (expr->annotation && strcmp(expr->annotation,"double")==0) printf("  %%t%d = fsub double -0.0, %%t%d\n", r, val); else printf("  %%t%d = sub i32 0, %%t%d\n", r, val); return r; }
        case CG_Plus: return emit_expr(expr->child);
        case CG_Length: { CGNode *id = expr->child; const char *aname = id->token; int argc_val=new_temp(); printf("  %%t%d = load i32, i32* %%%s.argc\n", argc_val, aname); r=new_temp(); printf("  %%t%d = sub i32 %%t%d, 1\n", r, argc_val); return r; }
        case CG_Assign: { CGNode *lhs=expr->child; CGNode *rhs=lhs->sibling; int val=emit_expr(rhs); const char *lhs_type=lhs->annotation?lhs->annotation:"int"; val=maybe_convert(val, rhs->annotation, lhs_type); const char *t=llvm_type_from_jtype_str(lhs_type); if (is_global_by_annotation(lhs->token, lhs->annotation)) printf("  store %s %%t%d, %s* @%s\n", t, val, t, lhs->token); else printf("  store %s %%t%d, %s* %%%s\n", t, val, t, lhs->token); return val; }
        case CG_Call: {
            CGNode *id = expr->child; const char *fname = id->token; const char *juc_argtypes[64]; int jnargs=0; for (CGNode *a=id->sibling; a; a=a->sibling) juc_argtypes[jnargs++]=a->annotation; MethodEntry *m = resolve_overload(fname, juc_argtypes, jnargs); char call_mangled[256]; if (m) mangle_from_method(m, fname, call_mangled, sizeof(call_mangled)); else { strncpy(call_mangled, fname, sizeof(call_mangled)-1); call_mangled[sizeof(call_mangled)-1]='\0'; } int args[128]; const char *atypes[128]; int nargs=0; CGNode *anode=id->sibling; int formal_idx=0; while (anode) { if (anode->annotation && strcmp(anode->annotation,"String[]")==0) { const char *arrname = anode->token; int argc_v=new_temp(); printf("  %%t%d = load i32, i32* %%%s.argc\n", argc_v, arrname); args[nargs]=argc_v; atypes[nargs]="i32"; nargs++; int argv_v=new_temp(); printf("  %%t%d = load i8**, i8*** %%%s.argv\n", argv_v, arrname); args[nargs]=argv_v; atypes[nargs]="i8**"; nargs++; } else { int val=emit_expr(anode); const char *formal_type=NULL; if (m && m->param_types) { if (formal_idx < m->n_params) formal_type = jtype_to_string(m->param_types[formal_idx]); } if (formal_type) { val = maybe_convert(val, anode->annotation, formal_type); atypes[nargs]=llvm_type_from_jtype_str(formal_type); } else atypes[nargs]=llvm_type_from_jtype_str(anode->annotation); args[nargs]=val; nargs++; } formal_idx++; anode=anode->sibling; }
            const char *ret_juc = expr->annotation ? expr->annotation : (m? jtype_to_string(m->return_type): "int"); const char *ret_llvm = llvm_type_from_jtype_str(ret_juc);
            if (strcmp(ret_llvm, "void")==0) { printf("  call void @%s(", call_mangled); for (int i=0;i<nargs;i++){ if (i) printf(", "); printf("%s %%t%d", atypes[i], args[i]); } printf(")\n"); return 0; } else { r=new_temp(); printf("  %%t%d = call %s @%s(", r, ret_llvm, call_mangled); for (int i=0;i<nargs;i++){ if (i) printf(", "); printf("%s %%t%d", atypes[i], args[i]); } printf(")\n"); return r; }
        }
        case CG_ParseArgs: { CGNode *id=expr->child; CGNode *idx=id->sibling; const char *aname=id->token; int idx_val=emit_expr(idx); int adj=new_temp(); printf("  %%t%d = add i32 %%t%d, 1\n", adj, idx_val); int argv_ptr=new_temp(); printf("  %%t%d = load i8**, i8*** %%%s.argv\n", argv_ptr, aname); int gep=new_temp(); printf("  %%t%d = getelementptr i8*, i8** %%t%d, i32 %%t%d\n", gep, argv_ptr, adj); int strp=new_temp(); printf("  %%t%d = load i8*, i8** %%t%d\n", strp, gep); r=new_temp(); printf("  %%t%d = call i32 @atoi(i8* %%t%d)\n", r, strp); return r; }
        default: return 0;
    }
}

static void emit_statement(CGNode *stmt) {
    if (!stmt) return;
    switch (stmt->kind) {
        case CG_If: { CGNode *cond=stmt->child; CGNode *then_s=cond?cond->sibling:NULL; CGNode *else_s=then_s?then_s->sibling:NULL; int cval=emit_expr(cond); int lthen=new_label(), lelse=new_label(), lend=new_label(); printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", cval, lthen, lelse); printf("L%d:\n", lthen); block_terminated=0; emit_statement(then_s); if (!block_terminated) printf("  br label %%L%d\n", lend); printf("L%d:\n", lelse); block_terminated=0; if (else_s && !(else_s->kind==CG_Block && !else_s->child)) emit_statement(else_s); if (!block_terminated) printf("  br label %%L%d\n", lend); printf("L%d:\n", lend); block_terminated=0; break; }
        case CG_While: { CGNode *cond=stmt->child; CGNode *wbody=cond?cond->sibling:NULL; int lcond=new_label(), lbody=new_label(), lend=new_label(); printf("  br label %%L%d\n", lcond); printf("L%d:\n", lcond); int cval=emit_expr(cond); printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", cval, lbody, lend); printf("L%d:\n", lbody); block_terminated=0; emit_statement(wbody); if (!block_terminated) printf("  br label %%L%d\n", lcond); printf("L%d:\n", lend); block_terminated=0; break; }
        case CG_Return: { if (stmt->child) { int val = emit_expr(stmt->child); val = maybe_convert(val, stmt->child->annotation, cur_ret_type); printf("  ret %s %%t%d\n", llvm_type_from_jtype_str(cur_ret_type), val); } else printf("  ret void\n"); block_terminated=1; break; }
        case CG_Print: { CGNode *arg=stmt->child; if (arg->kind==CG_StrLit) { const char *sname=find_str_name(arg->token); int slen=1; for (int i=0;i<n_str_consts;i++) if (strcmp(str_consts[i].val, arg->token)==0) slen=str_consts[i].len; int ptr=new_temp(); printf("  %%t%d = getelementptr [%d x i8], [%d x i8]* %s, i32 0, i32 0\n", ptr, slen, slen, sname); int fptr=new_temp(); printf("  %%t%d = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0\n", fptr); int d=new_temp(); printf("  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, i8* %%t%d)\n", d, fptr, ptr); } else { int val = emit_expr(arg); const char *atype = arg->annotation; if (atype && strcmp(atype,"boolean")==0) { int ltr=new_label(), lfa=new_label(), lend=new_label(); printf("  br i1 %%t%d, label %%L%d, label %%L%d\n", val, ltr, lfa); printf("L%d:\n", ltr); int pt=new_temp(); printf("  %%t%d = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0\n", pt); int fp1=new_temp(); printf("  %%t%d = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0\n", fp1); int d1=new_temp(); printf("  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, i8* %%t%d)\n", d1, fp1, pt); printf("  br label %%L%d\n", lend); printf("L%d:\n", lfa); int pf=new_temp(); printf("  %%t%d = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0\n", pf); int fp2=new_temp(); printf("  %%t%d = getelementptr [3 x i8], [3 x i8]* @.fmt.s, i32 0, i32 0\n", fp2); int d2=new_temp(); printf("  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, i8* %%t%d)\n", d2, fp2, pf); printf("  br label %%L%d\n", lend); printf("L%d:\n", lend); } else if (atype && strcmp(atype,"double")==0) { int fptr=new_temp(); printf("  %%t%d = getelementptr [6 x i8], [6 x i8]* @.fmt.e, i32 0, i32 0\n", fptr); int d=new_temp(); printf("  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, double %%t%d)\n", d, fptr, val); } else { int fptr=new_temp(); printf("  %%t%d = getelementptr [3 x i8], [3 x i8]* @.fmt.d, i32 0, i32 0\n", fptr); int d=new_temp(); printf("  %%t%d = call i32 (i8*, ...) @printf(i8* %%t%d, i32 %%t%d)\n", d, fptr, val); } } break; }
        case CG_Assign: case CG_Call: case CG_ParseArgs: emit_expr(stmt); break;
        case CG_Block: { for (CGNode *s = stmt->child; s; s = s->sibling) { if (!block_terminated) emit_statement(s); } break; }
        default: break;
    }
}
