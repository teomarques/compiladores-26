#define FIRST_CHILD(n) ((n) && (n)->children ? (n)->children->next : NULL)
/*
 * codegen.c - LLVM IR Code Generator for Juc Compiler - Meta 4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

int temp_counter = 1;
int label_counter = 1;
int string_counter = 0;

typedef struct str_entry {
    char *value;
    int id;
    struct str_entry *next;
} str_entry;

str_entry *string_constants = NULL;

typedef struct var_entry {
    char *name;
    char *llvm_type;
    int alloca_reg;
    struct var_entry *next;
} var_entry;

var_entry *local_vars = NULL;

int get_string_id(const char *str) {
    str_entry *curr = string_constants;
    while (curr != NULL) {
        if (strcmp(curr->value, str) == 0) return curr->id;
        curr = curr->next;
    }
    str_entry *new_str = malloc(sizeof(str_entry));
    new_str->value = strdup(str);
    new_str->id = string_counter++;
    new_str->next = string_constants;
    string_constants = new_str;
    return new_str->id;
}

void add_local_var(const char *name, const char *type, int reg) {
    var_entry *new_var = malloc(sizeof(var_entry));
    new_var->name = strdup(name);
    new_var->llvm_type = strdup(type);
    new_var->alloca_reg = reg;
    new_var->next = local_vars;
    local_vars = new_var;
}

var_entry *get_local_var(const char *name) {
    var_entry *curr = local_vars;
    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

void free_local_vars() {
    var_entry *curr = local_vars;
    while (curr != NULL) {
        var_entry *next = curr->next;
        free(curr->name);
        free(curr->llvm_type);
        free(curr);
        curr = next;
    }
    local_vars = NULL;
}

int new_temp() { return temp_counter++; }
int new_label() { return label_counter++; }

const char *get_llvm_type(const char *juc_type) {
    if (!juc_type || strcmp(juc_type, "undef") == 0) return "i32"; /* Fallback */
    if (strcmp(juc_type, "int") == 0) return "i32";
    if (strcmp(juc_type, "double") == 0) return "double";
    if (strcmp(juc_type, "boolean") == 0) return "i1";
    if (strcmp(juc_type, "void") == 0) return "void";
    if (strcmp(juc_type, "String") == 0) return "i8*";
    if (strcmp(juc_type, "String[]") == 0) return "i8**";
    return "i32"; /* Default fallback */
}

const char *get_llvm_type_from_jtype(JType type) {
    switch (type) {
        case JT_INT: return "i32";
        case JT_DOUBLE: return "double";
        case JT_BOOLEAN: return "i1";
        case JT_STRING: return "i8*";
        case JT_STRING_ARRAY: return "i8**";
        case JT_VOID: return "void";
        default: return "i32";
    }
}

void collect_strings(struct node *n) {
    if (!n) return;
    if (n->category == N_StrLit) {
        get_string_id(n->token);
    }
    struct node_list *c = FIRST_CHILD(n);
    while (c) {
        collect_strings(c->node);
        c = c->next;
    }
}

void emit_string_constants() {
    str_entry *curr = string_constants;
    while (curr != NULL) {
        /* Calculate length of string including null terminator, accounting for escaped chars like \n */
        int len = 0;
        char *str = curr->value;
        int i = 0;
        char formatted[1024] = {0};
        int f_idx = 0;
        
        /* Skip quotes */
        for (i = 1; str[i] != '\0' && str[i] != '\"'; i++) {
            if (str[i] == '\\') {
                i++;
                if (str[i] == 'n') { strcpy(&formatted[f_idx], "\\0A"); f_idx += 3; }
                else if (str[i] == 't') { strcpy(&formatted[f_idx], "\\09"); f_idx += 3; }
                else if (str[i] == 'r') { strcpy(&formatted[f_idx], "\\0D"); f_idx += 3; }
                else if (str[i] == '\"') { strcpy(&formatted[f_idx], "\\22"); f_idx += 3; }
                else if (str[i] == 'f') { strcpy(&formatted[f_idx], "\\0C"); f_idx += 3; }
                else if (str[i] == '\\') { strcpy(&formatted[f_idx], "\\5C"); f_idx += 3; }
                else { formatted[f_idx++] = str[i]; }
            } else {
                formatted[f_idx++] = str[i];
            }
            len++;
        }
        len++; /* null terminator */
        printf("@.str.%d = private unnamed_addr constant [%d x i8] c\"%s\\00\"\n", curr->id, len, formatted);
        curr = curr->next;
    }
    printf("@.str.true = private unnamed_addr constant [5 x i8] c\"true\\00\"\n");
    printf("@.str.false = private unnamed_addr constant [6 x i8] c\"false\\00\"\n");
    printf("@.str.fmt.int = private unnamed_addr constant [3 x i8] c\"%%d\\00\"\n");
    printf("@.str.fmt.double = private unnamed_addr constant [6 x i8] c\"%%.16e\\00\"\n");
    printf("@.str.fmt.str = private unnamed_addr constant [3 x i8] c\"%%s\\00\"\n");
}

void print_mangled_name_from_sig(const char *class_name, const char *method_name, const char *sig) {
    printf("@_%s_%s", class_name, method_name);
    if (!sig) return;
    for (int i = 0; sig[i]; i++) {
        if (sig[i] == '(' && sig[i+1] != ')') printf("_");
        else if (sig[i] == ',') printf("_");
        else if (sig[i] == '[') printf("Array");
        else if (sig[i] != '(' && sig[i] != ')' && sig[i] != ']') printf("%c", sig[i]);
    }
}

void print_mangled_name_from_mt(const char *class_name, const char *method_name, MethodEntry *mt) {
    printf("@_%s_%s", class_name, method_name);
    if (mt->n_params > 0) printf("_");
    for (int i = 0; i < mt->n_params; i++) {
        if (i > 0) printf("_");
        if (mt->param_types[i] == JT_INT) printf("int");
        else if (mt->param_types[i] == JT_DOUBLE) printf("double");
        else if (mt->param_types[i] == JT_BOOLEAN) printf("boolean");
        else if (mt->param_types[i] == JT_STRING_ARRAY) printf("StringArray");
        else if (mt->param_types[i] == JT_STRING) printf("String");
    }
}

int codegen_expression(struct node *expr, ClassTable *ct, MethodEntry *mt);
void codegen_statement(struct node *stmt, ClassTable *ct, MethodEntry *mt);

int codegen_expression(struct node *expr, ClassTable *ct, MethodEntry *mt) {
    if (!expr) return -1;

    switch (expr->category) {
        case N_Natural: {
            int t = new_temp();
            char *clean_tok = strdup(expr->token);
            char *p = clean_tok, *q = clean_tok;
            while (*p) { if (*p != '_') *q++ = *p; p++; }
            *q = '\0';
            printf("  %%%d = add i32 %s, 0\n", t, clean_tok);
            free(clean_tok);
            return t;
        }
        case N_Decimal: {
            int t = new_temp();
            /* Handle cases like 1.e0_1 -> 1.e01 */
            char *clean_tok = strdup(expr->token);
            char *p = clean_tok, *q = clean_tok;
            while (*p) { if (*p != '_') *q++ = *p; p++; }
            *q = '\0';
            double val = strtod(clean_tok, NULL);
            printf("  %%%d = fadd double %e, 0.0\n", t, val);
            free(clean_tok);
            return t;
        }
        case N_BoolLit: {
            int t = new_temp();
            int val = strcmp(expr->token, "true") == 0 ? 1 : 0;
            printf("  %%%d = add i1 %d, 0\n", t, val);
            return t;
        }
        case N_StrLit: {
            int id = get_string_id(expr->token);
            /* Calculate length again to emit correct type */
            int len = 0;
            char *str = expr->token;
            int i = 0;
            for (i = 1; str[i] != '\0' && str[i] != '\"'; i++) {
                if (str[i] == '\\') i++;
                len++;
            }
            len++;
            int t = new_temp();
            printf("  %%%d = getelementptr [%d x i8], [%d x i8]* @.str.%d, i32 0, i32 0\n", t, len, len, id);
            return t;
        }
        case N_Identifier: {
            if (strcmp(expr->token, "args") == 0 && strcmp(expr->type_annot, "String[]") == 0) {
                return -1; /* args is handled specially */
            }
            var_entry *v = get_local_var(expr->token);
            if (v) {
                int t = new_temp();
                printf("  %%%d = load %s, %s* %%%d\n", t, v->llvm_type, v->llvm_type, v->alloca_reg);
                return t;
            } else {
                Symbol *s = find_symbol(ct->fields, expr->token);
                if (s) {
                    const char *llvm_type = get_llvm_type_from_jtype(s->type);
                    int t = new_temp();
                    printf("  %%%d = load %s, %s* @_%s_%s\n", t, llvm_type, llvm_type, ct->name, expr->token);
                    return t;
                }
            }
            /* Variable not found in locals or fields; silently continue (no debug output) */
            return -1;
        }
        case N_Assign: {
            struct node *lhs = FIRST_CHILD(expr)->node;
            struct node *rhs = FIRST_CHILD(expr)->next->node;
            int t_rhs = codegen_expression(rhs, ct, mt);
            
            var_entry *v = get_local_var(lhs->token);
            if (v) {
                /* Cast if assigning int to double */
                const char *lhs_llvm_type = get_llvm_type(lhs->type_annot);
                const char *rhs_llvm_type = get_llvm_type(rhs->type_annot);
                int t_store = t_rhs;
                
                if (strcmp(lhs_llvm_type, "double") == 0 && strcmp(rhs_llvm_type, "i32") == 0) {
                    t_store = new_temp();
                    printf("  %%%d = sitofp i32 %%%d to double\n", t_store, t_rhs);
                }
                
                printf("  store %s %%%d, %s* %%%d\n", lhs_llvm_type, t_store, lhs_llvm_type, v->alloca_reg);
                return t_store;
            } else {
                Symbol *s = find_symbol(ct->fields, lhs->token);
                if (s) {
                    const char *lhs_llvm_type = get_llvm_type_from_jtype(s->type);
                    const char *rhs_llvm_type = get_llvm_type(rhs->type_annot);
                    int t_store = t_rhs;
                    
                    if (strcmp(lhs_llvm_type, "double") == 0 && strcmp(rhs_llvm_type, "i32") == 0) {
                        t_store = new_temp();
                        printf("  %%%d = sitofp i32 %%%d to double\n", t_store, t_rhs);
                    }
                    
                    printf("  store %s %%%d, %s* @_%s_%s\n", lhs_llvm_type, t_store, lhs_llvm_type, ct->name, lhs->token);
                    return t_store;
                }
            }
            return t_rhs;
        }
        case N_Add:
        case N_Sub:
        case N_Mul:
        case N_Div:
        case N_Mod: {
            struct node *lhs = FIRST_CHILD(expr)->node;
            struct node *rhs = FIRST_CHILD(expr)->next->node;
            int t1 = codegen_expression(lhs, ct, mt);
            int t2 = codegen_expression(rhs, ct, mt);
            
            int is_double = strcmp(expr->type_annot, "double") == 0;
            const char *ltype = get_llvm_type(lhs->type_annot);
            const char *rtype = get_llvm_type(rhs->type_annot);
            
            if (is_double && strcmp(ltype, "i32") == 0) {
                int t_new = new_temp();
                printf("  %%%d = sitofp i32 %%%d to double\n", t_new, t1);
                t1 = t_new;
            }
            if (is_double && strcmp(rtype, "i32") == 0) {
                int t_new = new_temp();
                printf("  %%%d = sitofp i32 %%%d to double\n", t_new, t2);
                t2 = t_new;
            }
            
            int res = new_temp();
            const char *op = "";
            if (is_double) {
                if (expr->category == N_Add) op = "fadd double";
                else if (expr->category == N_Sub) op = "fsub double";
                else if (expr->category == N_Mul) op = "fmul double";
                else if (expr->category == N_Div) op = "fdiv double";
                else if (expr->category == N_Mod) op = "frem double";
            } else {
                if (expr->category == N_Add) op = "add i32";
                else if (expr->category == N_Sub) op = "sub i32";
                else if (expr->category == N_Mul) op = "mul i32";
                else if (expr->category == N_Div) op = "sdiv i32";
                else if (expr->category == N_Mod) op = "srem i32";
            }
            printf("  %%%d = %s %%%d, %%%d\n", res, op, t1, t2);
            return res;
        }
        case N_Plus:
        case N_Minus: {
            struct node *child = FIRST_CHILD(expr)->node;
            int t1 = codegen_expression(child, ct, mt);
            if (expr->category == N_Plus) return t1;
            
            int res = new_temp();
            if (strcmp(expr->type_annot, "double") == 0) {
                printf("  %%%d = fsub double 0.0, %%%d\n", res, t1);
            } else {
                printf("  %%%d = sub i32 0, %%%d\n", res, t1);
            }
            return res;
        }
        case N_Eq:
        case N_Ne:
        case N_Lt:
        case N_Gt:
        case N_Le:
        case N_Ge: {
            struct node *lhs = FIRST_CHILD(expr)->node;
            struct node *rhs = FIRST_CHILD(expr)->next->node;
            int t1 = codegen_expression(lhs, ct, mt);
            int t2 = codegen_expression(rhs, ct, mt);
            
            const char *ltype = get_llvm_type(lhs->type_annot);
            const char *rtype = get_llvm_type(rhs->type_annot);
            int is_double = strcmp(ltype, "double") == 0 || strcmp(rtype, "double") == 0;
            
            if (is_double && strcmp(ltype, "i32") == 0) {
                int t_new = new_temp();
                printf("  %%%d = sitofp i32 %%%d to double\n", t_new, t1);
                t1 = t_new;
            }
            if (is_double && strcmp(rtype, "i32") == 0) {
                int t_new = new_temp();
                printf("  %%%d = sitofp i32 %%%d to double\n", t_new, t2);
                t2 = t_new;
            }
            
            int res = new_temp();
            const char *cond = "";
            if (is_double) {
                if (expr->category == N_Eq) cond = "oeq";
                else if (expr->category == N_Ne) cond = "one";
                else if (expr->category == N_Lt) cond = "olt";
                else if (expr->category == N_Gt) cond = "ogt";
                else if (expr->category == N_Le) cond = "ole";
                else if (expr->category == N_Ge) cond = "oge";
                printf("  %%%d = fcmp %s double %%%d, %%%d\n", res, cond, t1, t2);
            } else {
                if (expr->category == N_Eq) cond = "eq";
                else if (expr->category == N_Ne) cond = "ne";
                else if (expr->category == N_Lt) cond = "slt";
                else if (expr->category == N_Gt) cond = "sgt";
                else if (expr->category == N_Le) cond = "sle";
                else if (expr->category == N_Ge) cond = "sge";
                
                if (strcmp(ltype, "i1") == 0 && strcmp(rtype, "i1") == 0) {
                    printf("  %%%d = icmp %s i1 %%%d, %%%d\n", res, cond, t1, t2);
                } else {
                    printf("  %%%d = icmp %s i32 %%%d, %%%d\n", res, cond, t1, t2);
                }
            }
            return res;
        }
        case N_Not: {
            int t1 = codegen_expression(FIRST_CHILD(expr)->node, ct, mt);
            int res = new_temp();
            printf("  %%%d = xor i1 %%%d, 1\n", res, t1);
            return res;
        }
        case N_And:
        case N_Or: {
            struct node *lhs = FIRST_CHILD(expr)->node;
            struct node *rhs = FIRST_CHILD(expr)->next->node;
            
            int is_and = expr->category == N_And;
            int L1 = new_label(); /* eval right */
            int L2 = new_label(); /* end */
            
            /* Entry block for this expr is wherever we are now */
            int t_lhs = codegen_expression(lhs, ct, mt);
            
            /* Need to capture the label where t_lhs was computed for phi node */
            int label_lhs = new_label();
            printf("  br label %%L%d\n", label_lhs);
            printf("L%d:\n", label_lhs);
            
            if (is_and) {
                printf("  br i1 %%%d, label %%L%d, label %%L%d\n", t_lhs, L1, L2);
            } else {
                printf("  br i1 %%%d, label %%L%d, label %%L%d\n", t_lhs, L2, L1);
            }
            
            printf("L%d:\n", L1);
            int t_rhs = codegen_expression(rhs, ct, mt);
            int label_rhs = new_label();
            printf("  br label %%L%d\n", label_rhs);
            printf("L%d:\n", label_rhs);
            
            printf("  br label %%L%d\n", L2);
            
            printf("L%d:\n", L2);
            int res = new_temp();
            printf("  %%%d = phi i1 [ %%%d, %%L%d ], [ %%%d, %%L%d ]\n", res, t_lhs, label_lhs, t_rhs, label_rhs);
            return res;
        }
        case N_Call: {
            struct node *id_node = FIRST_CHILD(expr)->node;
            char *method_name = id_node->token;
            char *sig = id_node->type_annot; /* e.g. "(int,double)" */
            
            /* Evaluate arguments */
            int max_args = 256;
            int arg_temps[256];
            const char *arg_types[256];
            int n_args = 0;
            
            struct node_list *curr_arg = FIRST_CHILD(expr)->next;
            if (curr_arg && !curr_arg->node) curr_arg = curr_arg->next; /* Skip call_args sentinel */
            while (curr_arg && curr_arg->node) {
                arg_temps[n_args] = codegen_expression(curr_arg->node, ct, mt);
                arg_types[n_args] = get_llvm_type(curr_arg->node->type_annot);
                n_args++;
                curr_arg = curr_arg->next;
            }
            
            /* Match with method signature to cast arguments if needed */
            /* We need to parse the signature from type_annot to know if we need sitofp */
            char *sig_copy = strdup(sig);
            char *p = sig_copy + 1; /* Skip '(' */
            int param_idx = 0;
            char *token;
            while ((token = strtok(param_idx == 0 ? p : NULL, ",)")) != NULL) {
                const char *expected_type = get_llvm_type(token);
                if (strcmp(expected_type, "double") == 0 && strcmp(arg_types[param_idx], "i32") == 0) {
                    int t_cast = new_temp();
                    printf("  %%%d = sitofp i32 %%%d to double\n", t_cast, arg_temps[param_idx]);
                    arg_temps[param_idx] = t_cast;
                    arg_types[param_idx] = "double";
                }
                param_idx++;
            }
            free(sig_copy);
            
            const char *ret_type = get_llvm_type(expr->type_annot);
            int res = -1;
            if (strcmp(ret_type, "void") != 0) {
                res = new_temp();
                printf("  %%%d = call %s ", res, ret_type);
                print_mangled_name_from_sig(ct->name, method_name, sig);
                printf("(");
            } else {
                printf("  call void ");
                print_mangled_name_from_sig(ct->name, method_name, sig);
                printf("(");
            }
            
            for (int i = 0; i < n_args; i++) {
                if (i > 0) printf(", ");
                printf("%s %%%d", arg_types[i], arg_temps[i]);
            }
            printf(")\n");
            
            return res;
        }
        case N_ParseArgs: {
            /* Integer.parseInt(args[expr]) */
            struct node *idx_expr = FIRST_CHILD(expr)->next->node;
            int t_idx = codegen_expression(idx_expr, ct, mt);
            
            int t_inc = new_temp();
            printf("  %%%d = add i32 %%%d, 1\n", t_inc, t_idx);
            
            int t_argvptr = new_temp();
            printf("  %%%d = load i8**, i8*** @argv\n", t_argvptr);
            
            int t_strptr = new_temp();
            printf("  %%%d = getelementptr i8*, i8** %%%d, i32 %%%d\n", t_strptr, t_argvptr, t_inc);
            
            int t_str = new_temp();
            printf("  %%%d = load i8*, i8** %%%d\n", t_str, t_strptr);
            
            int res = new_temp();
            printf("  %%%d = call i32 @atoi(i8* %%%d)\n", res, t_str);
            return res;
        }
        case N_Length: {
            int t_argc = new_temp();
            printf("  %%%d = load i32, i32* @argc\n", t_argc);
            int res = new_temp();
            printf("  %%%d = sub i32 %%%d, 1\n", res, t_argc);
            return res;
        }
        default:
            return -1;
    }
}

void codegen_statement(struct node *stmt, ClassTable *ct, MethodEntry *mt) {
    if (!stmt) return;

    switch (stmt->category) {
        case N_Block: {
            struct node_list *c = FIRST_CHILD(stmt);
            while (c && c->node) {
                codegen_statement(c->node, ct, mt);
                c = c->next;
            }
            break;
        }
        case N_VarDecl: {
            struct node_list *c = FIRST_CHILD(stmt);
            const char *jtype = "undef";
            if (c && c->node) {
                if (c->node->category == N_Int) jtype = "int";
                else if (c->node->category == N_Double) jtype = "double";
                else if (c->node->category == N_Bool) jtype = "boolean";
                c = c->next;
            }
            const char *llvm_type = get_llvm_type(jtype);
            while (c && c->node) {
                char *name = c->node->token;
                int alloca_reg = new_temp();
                printf("  %%%d = alloca %s\n", alloca_reg, llvm_type);
                add_local_var(name, llvm_type, alloca_reg);
                c = c->next;
            }
            break;
        }
        case N_If: {
            struct node *cond = FIRST_CHILD(stmt)->node;
            struct node *then_stmt = FIRST_CHILD(stmt)->next->node;
            struct node *else_stmt = FIRST_CHILD(stmt)->next->next ? FIRST_CHILD(stmt)->next->next->node : NULL;
            
            int t_cond = codegen_expression(cond, ct, mt);
            
            int L_then = new_label();
            int L_else = new_label();
            int L_end = new_label();
            
            if (else_stmt) {
                printf("  br i1 %%%d, label %%L%d, label %%L%d\n", t_cond, L_then, L_else);
            } else {
                printf("  br i1 %%%d, label %%L%d, label %%L%d\n", t_cond, L_then, L_end);
            }
            
            printf("L%d:\n", L_then);
            codegen_statement(then_stmt, ct, mt);
            printf("  br label %%L%d\n", L_end);
            
            if (else_stmt) {
                printf("L%d:\n", L_else);
                codegen_statement(else_stmt, ct, mt);
                printf("  br label %%L%d\n", L_end);
            }
            
            printf("L%d:\n", L_end);
            break;
        }
        case N_While: {
            struct node *cond = FIRST_CHILD(stmt)->node;
            struct node *body = FIRST_CHILD(stmt)->next->node;
            
            int L_cond = new_label();
            int L_body = new_label();
            int L_end = new_label();
            
            printf("  br label %%L%d\n", L_cond);
            printf("L%d:\n", L_cond);
            
            int t_cond = codegen_expression(cond, ct, mt);
            printf("  br i1 %%%d, label %%L%d, label %%L%d\n", t_cond, L_body, L_end);
            
            printf("L%d:\n", L_body);
            codegen_statement(body, ct, mt);
            printf("  br label %%L%d\n", L_cond);
            
            printf("L%d:\n", L_end);
            break;
        }
        case N_Return: {
            struct node *ret_expr = FIRST_CHILD(stmt) ? FIRST_CHILD(stmt)->node : NULL;
            if (ret_expr) {
                int t = codegen_expression(ret_expr, ct, mt);
                const char *actual_llvm_type = get_llvm_type(ret_expr->type_annot);
                const char *expected_llvm_type = get_llvm_type_from_jtype(mt->return_type);
                
                int t_ret = t;
                if (strcmp(actual_llvm_type, "i32") == 0 && strcmp(expected_llvm_type, "double") == 0) {
                    t_ret = new_temp();
                    printf("  %%%d = sitofp i32 %%%d to double\n", t_ret, t);
                }
                
                printf("  ret %s %%%d\n", expected_llvm_type, t_ret);
            } else {
                printf("  ret void\n");
            }
            int L_dead = new_label();
            printf("L%d:\n", L_dead);
            break;
        }
        case N_Print: {
            struct node *arg = FIRST_CHILD(stmt)->node;
            if (arg->category == N_StrLit) {
                int id = get_string_id(arg->token);
                /* Need length to type correctly */
                int len = 0;
                char *str = arg->token;
                for (int i = 1; str[i] != '\0' && str[i] != '\"'; i++) {
                    if (str[i] == '\\') i++;
                    len++;
                }
                len++;
                int t_ptr = new_temp();
                printf("  %%%d = getelementptr [%d x i8], [%d x i8]* @.str.%d, i32 0, i32 0\n", t_ptr, len, len, id);
                int t_fmt = new_temp();
                printf("  %%%d = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0\n", t_fmt);
                printf("  %%%d = call i32 (i8*, ...) @printf(i8* %%%d, i8* %%%d)\n", new_temp(), t_fmt, t_ptr);
            } else {
                int t = codegen_expression(arg, ct, mt);
                const char *llvm_type = get_llvm_type(arg->type_annot);
                
                if (strcmp(llvm_type, "i32") == 0) {
                    int t_fmt = new_temp();
                    printf("  %%%d = getelementptr [3 x i8], [3 x i8]* @.str.fmt.int, i32 0, i32 0\n", t_fmt);
                    printf("  %%%d = call i32 (i8*, ...) @printf(i8* %%%d, i32 %%%d)\n", new_temp(), t_fmt, t);
                } else if (strcmp(llvm_type, "double") == 0) {
                    int t_fmt = new_temp();
                    printf("  %%%d = getelementptr [6 x i8], [6 x i8]* @.str.fmt.double, i32 0, i32 0\n", t_fmt);
                    printf("  %%%d = call i32 (i8*, ...) @printf(i8* %%%d, double %%%d)\n", new_temp(), t_fmt, t);
                } else if (strcmp(llvm_type, "i1") == 0) {
                    int t_true = new_temp();
                    int t_false = new_temp();
                    int t_str = new_temp();
                    printf("  %%%d = getelementptr [5 x i8], [5 x i8]* @.str.true, i32 0, i32 0\n", t_true);
                    printf("  %%%d = getelementptr [6 x i8], [6 x i8]* @.str.false, i32 0, i32 0\n", t_false);
                    printf("  %%%d = select i1 %%%d, i8* %%%d, i8* %%%d\n", t_str, t, t_true, t_false);
                    
                    int t_fmt = new_temp();
                    printf("  %%%d = getelementptr [3 x i8], [3 x i8]* @.str.fmt.str, i32 0, i32 0\n", t_fmt);
                    printf("  %%%d = call i32 (i8*, ...) @printf(i8* %%%d, i8* %%%d)\n", new_temp(), t_fmt, t_str);
                }
            }
            break;
        }
        case N_Assign:
        case N_Call:
        case N_ParseArgs:
            codegen_expression(stmt, ct, mt);
            break;
        default:
            break;
    }
}

void codegen_method(struct node *method_decl, ClassTable *ct) {
    struct node_list *md = FIRST_CHILD(method_decl);
    struct node *header = md->node;
    struct node *body = md->next ? md->next->node : NULL;
    
    struct node_list *hc = FIRST_CHILD(header);
    struct node *ret_node = hc->node;
    struct node *id_node = hc->next->node;
    struct node *params_node = hc->next->next->node;
    
    char *method_name = id_node->token;
    
    /* Determine jtype for return */
    JType ret_jtype = JT_UNDEF;
    if (ret_node->category == N_Int) ret_jtype = JT_INT;
    else if (ret_node->category == N_Double) ret_jtype = JT_DOUBLE;
    else if (ret_node->category == N_Bool) ret_jtype = JT_BOOLEAN;
    else if (ret_node->category == N_Void) ret_jtype = JT_VOID;
    
    /* Find MethodEntry to pass to expressions */
    JType *param_types = NULL;
    int n_params = 0;
    process_method_header(header, &ret_jtype, &method_name, &param_types, &n_params);
    MethodEntry *mt = find_exact_method(ct->methods, method_name, n_params, param_types);
    if (param_types) free(param_types);
    if (!mt) return; /* Should not happen unless syntax error */
    
    temp_counter = 1;
    label_counter = 1;
    free_local_vars();
    
    const char *ret_llvm_type = get_llvm_type_from_jtype(ret_jtype);
    
    printf("define %s ", ret_llvm_type);
    print_mangled_name_from_mt(ct->name, method_name, mt);
    printf("(");
    
    /* Print parameters */
    int is_main = (strcmp(method_name, "main") == 0 && n_params == 1 && mt->param_types[0] == JT_STRING_ARRAY);
    
    struct node_list *p = FIRST_CHILD(params_node);
    int p_idx = 0;
    while (p && p->node) {
        struct node *param_decl = p->node;
        struct node *ptype_node = FIRST_CHILD(param_decl)->node;
        
        if (!is_main) {
            if (p_idx > 0) printf(", ");
            const char *jtype = "undef";
            if (ptype_node->category == N_Int) jtype = "int";
            else if (ptype_node->category == N_Double) jtype = "double";
            else if (ptype_node->category == N_Bool) jtype = "boolean";
            
            printf("%s %%param_%d", get_llvm_type(jtype), p_idx);
        }
        p = p->next;
        p_idx++;
    }
    printf(") {\n");
    
    /* Allocate stack slots for params */
    p = FIRST_CHILD(params_node);
    p_idx = 0;
    while (p && p->node) {
        struct node *param_decl = p->node;
        struct node *ptype_node = FIRST_CHILD(param_decl)->node;
        struct node *pname_node = FIRST_CHILD(param_decl)->next->node;
        char *pname = pname_node->token;
        
        if (is_main && ptype_node->category == N_StringArray) {
            /* do nothing for String[] args, we intercept uses */
            p = p->next;
            p_idx++;
            continue;
        }
        
        const char *jtype = "undef";
        if (ptype_node->category == N_Int) jtype = "int";
        else if (ptype_node->category == N_Double) jtype = "double";
        else if (ptype_node->category == N_Bool) jtype = "boolean";
        
        const char *llvm_type = get_llvm_type(jtype);
        int alloca_reg = new_temp();
        printf("  %%%d = alloca %s\n", alloca_reg, llvm_type);
        add_local_var(pname, llvm_type, alloca_reg);
        
        /* Store incoming value */
        printf("  store %s %%param_%d, %s* %%%d\n", llvm_type, p_idx, llvm_type, alloca_reg);
        
        p = p->next;
        p_idx++;
    }
    
    /* Generate body */
    if (body) {
        struct node_list *stmt = FIRST_CHILD(body);
        while (stmt && stmt->node) {
            codegen_statement(stmt->node, ct, mt);
            stmt = stmt->next;
        }
    }
    
    /* Ensure termination */
    if (strcmp(ret_llvm_type, "void") == 0) {
        printf("  ret void\n");
    } else if (strcmp(ret_llvm_type, "i32") == 0) {
        printf("  ret i32 0\n");
    } else if (strcmp(ret_llvm_type, "double") == 0) {
        printf("  ret double 0.0\n");
    } else if (strcmp(ret_llvm_type, "i1") == 0) {
        printf("  ret i1 0\n");
    }
    
    printf("}\n\n");
    free_local_vars();
}

void codegen_program(struct node *program, ClassTable *ct) {
    if (!program || !ct) return;
    
    /* Collect strings */
    collect_strings(program);
    
    /* Globals */
    emit_string_constants();
    printf("declare i32 @printf(i8*, ...)\n");
    printf("declare i32 @atoi(i8*)\n");
    
    int has_main = 0;
    MethodEntry *m = ct->methods;
    while (m) {
        if (strcmp(m->name, "main") == 0 && m->n_params == 1 && m->param_types[0] == JT_STRING_ARRAY) {
            has_main = 1;
        }
        m = m->next;
    }
    
    Symbol *f = ct->fields;
    while (f) {
        const char *llvm_type = get_llvm_type_from_jtype(f->type);
        const char *init = "0";
        if (strcmp(llvm_type, "double") == 0) init = "0.0";
        else if (strcmp(llvm_type, "i1") == 0) init = "0";
        printf("@_%s_%s = global %s %s\n", ct->name, f->name, llvm_type, init);
        f = f->next;
    }
    
    if (has_main) {
        printf("@argc = global i32 0\n");
        printf("@argv = global i8** null\n\n");
    }
    
    /* Methods */
    struct node_list *c = FIRST_CHILD(program);
    if (c) c = c->next; /* skip ID */
    while (c && c->node) {
        if (c->node->category == N_MethodDecl) {
            codegen_method(c->node, ct);
        }
        c = c->next;
    }
    
    /* Entry main wrapper */
    if (has_main) {
        printf("define i32 @main(i32 %%argc, i8** %%argv) {\n");
        printf("  store i32 %%argc, i32* @argc\n");
        printf("  store i8** %%argv, i8*** @argv\n");
        printf("  call void @_%s_main_StringArray()\n", ct->name);
        printf("  ret i32 0\n");
        printf("}\n");
    }
}
