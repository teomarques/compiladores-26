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

%type<node> program classdecl members member
%type<node> methoddecl methodparams_opt methodparams paramdecl
%type<node> fielddecl vardecl
%type<node> methodbody bodyelems
%type<node> block stmts stmts_opt stmt
%type<node> expr call_expr arg_list_opt arg_list lvalue
%type<node> type_nonvoid param_type

%left OR
%left AND
%left EQ NE
%left GT LT GE LE
%left XOR
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD
%left DOTLENGTH

%right UPLUS UMINUS
%right NOT

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
            /* Program children: Identifier + {FieldDecl|MethodDecl}. */
            struct node_list *c = $4->children->next;
            while (c != NULL) { addchild($$, c->node); c = c->next; }
        }
    ;

members:
      /* empty */
        {
            $$ = newnode(Tmp_List, NULL);
        }
    | members member
        {
            $$ = $1;
            addchild($$, $2);
        }
    ;

member:
      methoddecl { $$ = $1; }
    | fielddecl  { $$ = $1; }
    ;

fielddecl:
      type_nonvoid IDENTIFIER SEMICOLON
        {
            $$ = newnode(FieldDecl, NULL);
            addchild($$, $1);
            addchild($$, newnode(Identifier, $2));
        }
    | PUBLIC type_nonvoid IDENTIFIER SEMICOLON
        {
            $$ = newnode(FieldDecl, NULL);
            addchild($$, $2);
            addchild($$, newnode(Identifier, $3));
        }
    | PUBLIC STATIC type_nonvoid IDENTIFIER SEMICOLON
        {
            $$ = newnode(FieldDecl, NULL);
            addchild($$, $3);
            addchild($$, newnode(Identifier, $4));
        }
    | STATIC type_nonvoid IDENTIFIER SEMICOLON
        {
            $$ = newnode(FieldDecl, NULL);
            addchild($$, $2);
            addchild($$, newnode(Identifier, $3));
        }
    ;

methoddecl:
      PUBLIC STATIC type_nonvoid IDENTIFIER LPAR methodparams_opt RPAR methodbody
        {
            struct node *header = newnode(MethodHeader, NULL);
            addchild(header, $3);
            addchild(header, newnode(Identifier, $4));
            addchild(header, $6);

            $$ = newnode(MethodDecl, NULL);
            addchild($$, header);
            addchild($$, $8);
        }
    | PUBLIC STATIC VOID IDENTIFIER LPAR methodparams_opt RPAR methodbody
        {
            struct node *header = newnode(MethodHeader, NULL);
            addchild(header, newnode(Void, NULL));
            addchild(header, newnode(Identifier, $4));
            addchild(header, $6);

            $$ = newnode(MethodDecl, NULL);
            addchild($$, header);
            addchild($$, $8);
        }
    ;

methodparams_opt:
      /* empty */
        { $$ = newnode(MethodParams, NULL); }
    | methodparams
        { $$ = $1; }
    ;

methodparams:
      paramdecl
        {
            $$ = newnode(MethodParams, NULL);
            addchild($$, $1);
        }
    | methodparams COMMA paramdecl
        {
            $$ = $1;
            addchild($$, $3);
        }
    ;

paramdecl:
      param_type IDENTIFIER
        {
            $$ = newnode(ParamDecl, NULL);
            addchild($$, $1);
            addchild($$, newnode(Identifier, $2));
        }
    ;

type_nonvoid:
            INT         { $$ = newnode(Int, NULL); }
        | DOUBLE      { $$ = newnode(Double, NULL); }
        | BOOL        { $$ = newnode(Bool, NULL); }
        | STRING LSQ RSQ { $$ = newnode(StringArray, NULL); }
        ;

param_type:
    type_nonvoid { $$ = $1; }
        ;

methodbody:
      LBRACE bodyelems RBRACE
        {
            $$ = $2;
        }
    ;

bodyelems:
      /* empty */
        { $$ = newnode(MethodBody, NULL); }
    | bodyelems stmt
        { $$ = $1; addchild($$, $2); }
    ;

vardecl:
      type_nonvoid IDENTIFIER SEMICOLON
        {
            $$ = newnode(VarDecl, NULL);
            addchild($$, $1);
            addchild($$, newnode(Identifier, $2));
        }
    ;

block:
      LBRACE stmts_opt RBRACE
        {
            /* Avoid redundant Block with exactly 1 child. */
            int count = 0;
            struct node_list *c = $2->children->next;
            while (c != NULL) { count++; c = c->next; }

            if (count == 1) {
                $$ = $2->children->next->node;
            } else {
                $$ = newnode(Block, NULL);
                c = $2->children->next;
                while (c != NULL) { addchild($$, c->node); c = c->next; }
            }
        }
    ;

stmts_opt:
      /* empty */
        { $$ = newnode(Tmp_List, NULL); }
    | stmts
        { $$ = $1; }
    ;

stmts:
      stmt
        { $$ = newnode(Tmp_List, NULL); addchild($$, $1); }
    | stmts stmt
        { $$ = $1; addchild($$, $2); }
    ;

stmt:
      block
        { $$ = $1; }
    | vardecl
        { $$ = $1; }
    | lvalue ASSIGN expr SEMICOLON
        {
            $$ = newnode(Assign, NULL);
            addchild($$, $1);
            addchild($$, $3);
        }
    | PRINT LPAR expr RPAR SEMICOLON
        {
            $$ = newnode(Print, NULL);
            addchild($$, $3);
        }
    | RETURN SEMICOLON
        {
            $$ = newnode(Return, NULL);
        }
    | RETURN expr SEMICOLON
        {
            $$ = newnode(Return, NULL);
            addchild($$, $2);
        }
    | IF LPAR expr RPAR stmt %prec IFX
        {
            $$ = newnode(If, NULL);
            addchild($$, $3);
            addchild($$, $5);
            /* Mandatory else child: represent empty statement as empty Block. */
            addchild($$, newnode(Block, NULL));
        }
    | IF LPAR expr RPAR stmt ELSE stmt
        {
            $$ = newnode(If, NULL);
            addchild($$, $3);
            addchild($$, $5);
            addchild($$, $7);
        }
    | WHILE LPAR expr RPAR stmt
        {
            $$ = newnode(While, NULL);
            addchild($$, $3);
            addchild($$, $5);
        }
    | call_expr SEMICOLON
        { $$ = $1; }
    | SEMICOLON
        {
            /* Empty statement: represent as empty Block. */
            $$ = newnode(Block, NULL);
        }
    ;

lvalue:
      IDENTIFIER
        { $$ = newnode(Identifier, $1); }
    ;

expr:
      NATURAL
        { $$ = newnode(Natural, $1); }
    | DECIMAL
        { $$ = newnode(Decimal, $1); }
    | BOOLLIT
        { $$ = newnode(BoolLit, $1); }
    | STRLIT
        { $$ = newnode(StrLit, $1); }
    | IDENTIFIER
        { $$ = newnode(Identifier, $1); }
    | call_expr
        { $$ = $1; }
    | PARSEINT LPAR IDENTIFIER LSQ expr RSQ RPAR
        {
            $$ = newnode(ParseArgs, NULL);
            addchild($$, newnode(Identifier, $3));
            addchild($$, $5);
        }
    | expr OR expr
        { $$ = newnode(Or, NULL); addchild($$, $1); addchild($$, $3); }
    | expr AND expr
        { $$ = newnode(And, NULL); addchild($$, $1); addchild($$, $3); }
    | expr EQ expr
        {
            $$ = newnode(Eq, NULL);
            addchild($$, $1);
            addchild($$, $3);
        }
    | expr NE expr
        { $$ = newnode(Ne, NULL); addchild($$, $1); addchild($$, $3); }
    | expr LT expr
        { $$ = newnode(Lt, NULL); addchild($$, $1); addchild($$, $3); }
    | expr GT expr
        { $$ = newnode(Gt, NULL); addchild($$, $1); addchild($$, $3); }
    | expr LE expr
        { $$ = newnode(Le, NULL); addchild($$, $1); addchild($$, $3); }
    | expr GE expr
        { $$ = newnode(Ge, NULL); addchild($$, $1); addchild($$, $3); }
    | expr PLUS expr
        {
            $$ = newnode(Add, NULL); addchild($$, $1); addchild($$, $3);
        }
    | expr MINUS expr
        {
            $$ = newnode(Sub, NULL); addchild($$, $1); addchild($$, $3);
        }
    | expr STAR expr
        {
            $$ = newnode(Mul, NULL); addchild($$, $1); addchild($$, $3);
        }
    | expr DIV expr
        {
            $$ = newnode(Div, NULL); addchild($$, $1); addchild($$, $3);
        }
    | expr MOD expr
        { $$ = newnode(Mod, NULL); addchild($$, $1); addchild($$, $3); }
    | expr LSHIFT expr
        { $$ = newnode(Lshift, NULL); addchild($$, $1); addchild($$, $3); }
    | expr RSHIFT expr
        { $$ = newnode(Rshift, NULL); addchild($$, $1); addchild($$, $3); }
    | expr XOR expr
        { $$ = newnode(Xor, NULL); addchild($$, $1); addchild($$, $3); }
    | NOT expr
        { $$ = newnode(Not, NULL); addchild($$, $2); }
    | PLUS expr %prec UPLUS
        { $$ = newnode(Plus, NULL); addchild($$, $2); }
    | MINUS expr %prec UMINUS
        { $$ = newnode(Minus, NULL); addchild($$, $2); }
    | expr DOTLENGTH
        { $$ = newnode(Length, NULL); addchild($$, $1); }
    | LPAR expr RPAR
        { $$ = $2; }
    ;

call_expr:
      IDENTIFIER LPAR arg_list_opt RPAR
        {
            $$ = newnode(Call, NULL);
            addchild($$, newnode(Identifier, $1));
            struct node_list *c = $3->children->next;
            while (c != NULL) { addchild($$, c->node); c = c->next; }
        }
    ;

arg_list_opt:
      /* empty */
        {
            $$ = newnode(Tmp_List, NULL);
        }
    | arg_list
        { $$ = $1; }
    ;

arg_list:
      expr
        {
            $$ = newnode(Tmp_List, NULL);
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
    "FieldDecl",
    "VarDecl",
    "MethodDecl",
    "MethodHeader",
    "MethodParams",
    "ParamDecl",
    "MethodBody",

    "Block",
    "If",
    "While",
    "Return",
    "Print",
    "Assign",

    "Or",
    "And",
    "Eq",
    "Ne",
    "Lt",
    "Gt",
    "Le",
    "Ge",
    "Add",
    "Sub",
    "Mul",
    "Div",
    "Mod",
    "Lshift",
    "Rshift",
    "Xor",
    "Not",
    "Minus",
    "Plus",
    "Length",
    "Call",
    "ParseArgs",

    "Bool",
    "BoolLit",
    "Double",
    "Decimal",
    "Identifier",
    "Int",
    "Natural",
    "StrLit",
    "StringArray",
    "Void",

    "Tmp_List"
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
