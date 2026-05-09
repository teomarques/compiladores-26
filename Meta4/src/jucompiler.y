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
