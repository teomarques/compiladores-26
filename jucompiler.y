/* START definitions section -- C code delimited by %{ ... %} and token declarations */

%{

#include <stdio.h>
#include "ast.h"
#include <stdlib.h>
#include <string.h>

/* Enable Bison runtime trace (yydebug). */
#define YYDEBUG 1

int yyparse(void);
int yylex(void);
void yyerror(char *);

struct node *ast;

%}

%union{
    char *lexeme;
    struct node *node;
}

/* Tokens devolvidos pelo lexer (jucompiler.l). */
%token BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT PUBLIC RETURN STATIC STRING VOID WHILE
%token EQ NE GE LE LSHIFT RSHIFT ARROW AND OR GT LT ASSIGN NOT
%token PLUS MINUS STAR DIV MOD XOR
%token COMMA SEMICOLON LPAR RPAR LBRACE RBRACE LSQ RSQ

/* Tokens herdados do exemplo "petit" (podem ser removidos quando substituíres a gramática). */
%token INTEGER THEN

%token<lexeme> IDENTIFIER NATURAL DECIMAL STRLIT BOOLLIT RESERVED

%type<node> program classdecl members member methoddecl type param_list_opt param_list param
%type<node> block stmt_list stmt
%type<node> expr arg_list_opt arg_list lvalue

%left OR
%left AND
%left EQ NE
%left GT LT GE LE
%left PLUS MINUS
%left STAR DIV

%nonassoc IFX
%nonassoc ELSE

/* START grammar rules section -- BNF grammar */

%%

program:
      classdecl
        {
            ast = $$ = $1;
        }
    ;

classdecl:
      CLASS IDENTIFIER LBRACE members RBRACE
        {
            $$ = newnode(Program, NULL);
            addchild($$, newnode(Identifier, $2));
            /* Flatten members under Program (no explicit ClassDecl/Members in output). */
            struct node_list *c = $4->children->next;
            while (c != NULL) {
                addchild($$, c->node);
                c = c->next;
            }
        }
    ;

members:
      /* empty */
        {
            $$ = newnode(Members, NULL);
        }
    | members member
        {
            $$ = $1;
            addchild($$, $2);
        }
    ;

member:
      methoddecl { $$ = $1; }
    ;

methoddecl:
      PUBLIC STATIC type IDENTIFIER LPAR param_list_opt RPAR block
        {
            $$ = newnode(MethodDecl, NULL);

            struct node *header = newnode(MethodHeader, NULL);
            addchild(header, $3);
            addchild(header, newnode(Identifier, $4));
            addchild(header, $6);
            addchild($$, header);

            struct node *body = newnode(MethodBody, NULL);
            addchild(body, $8);
            addchild($$, body);
        }
    ;

type:
      INT        { $$ = newnode(Integer, NULL); }
    | DOUBLE     { $$ = newnode(Double, NULL); }
    | BOOL       { $$ = newnode(BoolType, NULL); }
    | STRING     { $$ = newnode(StringType, NULL); }
    | VOID       { $$ = newnode(VoidType, NULL); }
    | STRING LSQ RSQ { $$ = newnode(StringType, NULL); }
    ;

param_list_opt:
      /* empty */
        {
            $$ = newnode(Parameters, NULL);
        }
    | param_list
        {
            $$ = $1;
        }
    ;

param_list:
      param
        {
            $$ = newnode(Parameters, NULL);
            addchild($$, $1);
        }
    | param_list COMMA param
        {
            $$ = $1;
            addchild($$, $3);
        }
    ;

param:
      type IDENTIFIER
        {
            $$ = newnode(Parameter, NULL);
            addchild($$, $1);
            addchild($$, newnode(Identifier, $2));
        }
    ;

block:
      LBRACE stmt_list RBRACE
        {
            $$ = newnode(Block, NULL);
            addchild($$, $2);
        }
    ;

stmt_list:
      /* empty */
        {
            $$ = newnode(Statements, NULL);
        }
    | stmt_list stmt
        {
            $$ = $1;
            addchild($$, $2);
        }
    ;

stmt:
      block
        { $$ = $1; }
    | type IDENTIFIER SEMICOLON
        {
            $$ = newnode(VarDecl, NULL);
            addchild($$, $1);
            addchild($$, newnode(Identifier, $2));
        }
    | lvalue ASSIGN expr SEMICOLON
        {
            $$ = newnode(Assign, NULL);
            addchild($$, $1);
            addchild($$, $3);
        }
    | PRINT LPAR expr RPAR SEMICOLON
        {
            $$ = newnode(PrintStmt, NULL);
            addchild($$, $3);
        }
    | RETURN expr SEMICOLON
        {
            $$ = newnode(ReturnStmt, NULL);
            addchild($$, $2);
        }
    | IF LPAR expr RPAR stmt %prec IFX
        {
            $$ = newnode(If, NULL);
            addchild($$, $3);
            addchild($$, $5);
        }
    | IF LPAR expr RPAR stmt ELSE stmt
        {
            $$ = newnode(If, NULL);
            addchild($$, $3);
            addchild($$, $5);
            addchild($$, $7);
        }
    ;

lvalue:
      IDENTIFIER
        { $$ = newnode(Identifier, $1); }
    | IDENTIFIER LSQ expr RSQ
        {
            $$ = newnode(Index, NULL);
            addchild($$, newnode(Identifier, $1));
            addchild($$, $3);
        }
    ;

expr:
      NATURAL
        { $$ = newnode(Natural, $1); }
    | DECIMAL
        { $$ = newnode(Decimal, $1); }
    | BOOLLIT
        { $$ = newnode(Identifier, $1); }
    | STRLIT
        { $$ = newnode(Identifier, $1); }
    | lvalue
        { $$ = $1; }
    | IDENTIFIER LPAR arg_list_opt RPAR
        {
            $$ = newnode(Call, NULL);
            addchild($$, newnode(Identifier, $1));
            addchild($$, $3);
        }
    | PARSEINT LPAR expr RPAR
        {
            $$ = newnode(Call, NULL);
            addchild($$, newnode(Identifier, "Integer.parseInt"));
            addchild($$, $3);
        }
    | expr EQ expr
        {
            $$ = newnode(Eq, NULL);
            addchild($$, $1);
            addchild($$, $3);
        }
    | expr PLUS expr
        {
            $$ = newnode(Add, NULL);
            addchild($$, $1);
            addchild($$, $3);
        }
    | expr MINUS expr
        {
            $$ = newnode(Sub, NULL);
            addchild($$, $1);
            addchild($$, $3);
        }
    | expr STAR expr
        {
            $$ = newnode(Mul, NULL);
            addchild($$, $1);
            addchild($$, $3);
        }
    | expr DIV expr
        {
            $$ = newnode(Div, NULL);
            addchild($$, $1);
            addchild($$, $3);
        }
    | LPAR expr RPAR
        { $$ = $2; }
    ;

arg_list_opt:
      /* empty */
        {
            $$ = newnode(Arguments, NULL);
        }
    | arg_list
        { $$ = $1; }
    ;

arg_list:
      expr
        {
            $$ = newnode(Arguments, NULL);
            addchild($$, $1);
        }
    | arg_list COMMA expr
        {
            $$ = $1;
            addchild($$, $3);
        }
    ;

%%

/* START subroutines section */

const char *category_name[] = {
    "Program",
    "Function",
    "MethodParams",
    "ParamDecl",
    "Arguments",
    "Int",
    "Double",
    "Identifier",
    "Natural",
    "Decimal",
    "Call",
    "If",
    "Add",
    "Sub",
    "Mul",
    "Div",
    "ClassDecl",
    "Members",
    "MethodDecl",
    "Block",
    "Statements",
    "VarDecl",
    "Assign",
    "ReturnStmt",
    "PrintStmt",
    "Eq",
    "Index",
    "StringType",
    "VoidType",
    "BoolType",
    "MethodHeader",
    "MethodBody"
};

void show(struct node *node, int depth) {
    if (node == NULL) return;

    for (int i = 0; i < depth; i++)
        printf("..");

    printf("%s", category_name[node->category]);
    if (node->token != NULL)
        printf("(%s)", node->token);
    printf("\n");

    struct node_list *child = node->children->next;
    while (child != NULL) {
        show(child->node, depth + 1);
        child = child->next;
    }
}

// all needed functions are collected in the .l and ast.* files

extern int syn_line;
extern int syn_column;
extern char *yytext;

void yyerror(char *msg) {
    /* Formato pedido na meta2 (aproximação mínima enquanto a gramática é substituída):
       Line X, col Y: syntax error: <token> */
    if (yytext == NULL) yytext = "";
    printf("Line %d, col %d: %s: %s\n", syn_line, syn_column, msg, yytext);
}

/* Program entry point.
 * For meta1-style usage, we run the lexer until EOF.
 * (lexical errors and optional token printing are handled in jucompiler.l) */
extern int print_tokens;
static int print_tree = 0;

typedef enum {
    MODE_PARSE_ERRORS,
    MODE_PARSE_TREE,
    MODE_LEX_ERRORS,
    MODE_LEX_TOKENS
} run_mode;

int main(int argc, char **argv) {
    run_mode mode = MODE_PARSE_ERRORS;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            mode = MODE_LEX_TOKENS;
        } else if (strcmp(argv[i], "-e1") == 0) {
            mode = MODE_LEX_ERRORS;
        } else if (strcmp(argv[i], "-t") == 0) {
            mode = MODE_PARSE_TREE;
        }
        /* Ignore unknown options/arguments: the judge invokes via stdin. */
    }

    if (mode == MODE_LEX_TOKENS || mode == MODE_LEX_ERRORS) {
        /* Compatibility with previous phase: lexer-only. */
        print_tokens = (mode == MODE_LEX_TOKENS) ? 1 : 0;
        while (yylex() != 0) {
            /* no-op */
        }
        return 0;
    }

    /* Default / -e2 / -t: run parser (which calls yylex). */
    print_tokens = 0;
    print_tree = (mode == MODE_PARSE_TREE) ? 1 : 0;
    if (yyparse() == 0 && print_tree && ast != NULL) {
        show(ast, 0);
    }

    return 0;
}
