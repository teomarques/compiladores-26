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
