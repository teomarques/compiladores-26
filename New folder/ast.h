#ifndef _AST_H
#define _AST_H

/* AST node categories required by the assignment spec.
 * Internal-only helpers (never printed / never attached to final AST) are prefixed with Tmp_. */
enum category {
    /* Root / declarations */
    Program,
    FieldDecl,
    VarDecl,
    MethodDecl,
    MethodHeader,
    MethodParams,
    ParamDecl,
    MethodBody,

    /* Statements */
    Block,
    If,
    While,
    Return,
    Print,
    Assign,

    /* Operators / expressions */
    Or,
    And,
    Eq,
    Ne,
    Lt,
    Gt,
    Le,
    Ge,
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Lshift,
    Rshift,
    Xor,
    Not,
    Minus,
    Plus,
    Length,
    Call,
    ParseArgs,

    /* Terminals */
    Bool,
    BoolLit,
    Double,
    Decimal,
    Identifier,
    Int,
    Natural,
    StrLit,
    StringArray,
    Void,

    /* Internal-only */
    Tmp_List
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
