#ifndef _AST_H
#define _AST_H

/* Categorias da AST — prefixo N_ evita colisões com tokens yacc.
 * O array category_name[] em jucompiler.y mapeia → nomes do enunciado. */
enum category {
    N_Program, N_FieldDecl, N_VarDecl, N_MethodDecl,
    N_MethodHeader, N_MethodParams, N_ParamDecl, N_MethodBody,
    N_Block, N_If, N_While, N_Return, N_Call, N_Print, N_ParseArgs, N_Assign,
    N_Or, N_And, N_Eq, N_Ne, N_Lt, N_Gt, N_Le, N_Ge,
    N_Add, N_Sub, N_Mul, N_Div, N_Mod,
    N_Lshift, N_Rshift, N_Xor, N_Not, N_Minus, N_Plus, N_Length,
    N_Int, N_Double, N_Bool, N_StringArray, N_Void,
    N_Identifier, N_Natural, N_Decimal, N_BoolLit, N_StrLit
};

struct node {
    enum category category;
    char *token;
    struct node_list *children;
};

struct node_list {
    struct node *node;
    struct node_list *next;
};

struct node *newnode(enum category category, char *token);
void addchild(struct node *parent, struct node *child);

#endif
