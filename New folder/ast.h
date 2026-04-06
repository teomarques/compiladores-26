#ifndef _AST_H
#define _AST_H

enum category {
    Program,
    Function,
    Parameters,
    Parameter,
    Arguments,
    Integer,
    Double,
    Identifier,
    Natural,
    Decimal,
    Call,
    If,
    Add,
    Sub,
    Mul,
    Div,
    /* Appended categories (keep existing numeric values stable). */
    ClassDecl,
    Members,
    MethodDecl,
    Block,
    Statements,
    VarDecl,
    Assign,
    ReturnStmt,
    PrintStmt,
    Eq,
    Index,
    StringType,
    VoidType,
    BoolType,
    MethodHeader,
    MethodBody
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
