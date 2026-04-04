/* START definitions section -- C code delimited by %{ ... %} and token declarations */

%{

#include <stdio.h>
#include "ast.h"
#include <string.h>

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

%type<node> program functions function parameters parameter arguments expression

%left LOW
%left '+' '-'
%left '*' '/'

/* START grammar rules section -- BNF grammar */

%%

program: functions                  { ast = $$ = $1; }
    ;

functions: function                 { $$ = newnode(Program, NULL);
                                      addchild($$, $1); }
    | functions function            { $$ = $1;
                                      addchild($$, $2); }
    ;

function: IDENTIFIER '(' parameters ')' '=' expression
                                    { $$ = newnode(Function, NULL);
                                      addchild($$, newnode(Identifier, $1));
                                      addchild($$, $3);
                                      addchild($$, $6); }
    ;

parameters: parameter               { $$ = newnode(Parameters, NULL);
                                      addchild($$, $1); }
    | parameters ',' parameter      { $$ = $1;
                                      addchild($$, $3); }
    ;

parameter: INTEGER IDENTIFIER       { $$ = newnode(Parameter, NULL);
                                      addchild($$, newnode(Integer, NULL));
                                      addchild($$, newnode(Identifier, $2)); }
    | DOUBLE IDENTIFIER             { $$ = newnode(Parameter, NULL);
                                      addchild($$, newnode(Double, NULL));
                                      addchild($$, newnode(Identifier, $2)); }
    ;

arguments: expression               { $$ = newnode(Arguments, NULL);
                                      addchild($$, $1); }
    | arguments ',' expression      { $$ = $1;
                                      addchild($$, $3); }
    ;

expression: IDENTIFIER              { $$ = newnode(Identifier, $1); }
    | NATURAL                       { $$ = newnode(Natural, $1); }
    | DECIMAL                       { $$ = newnode(Decimal, $1); }
    | IDENTIFIER '(' arguments ')'  { $$ = newnode(Call, NULL);
                                      addchild($$, newnode(Identifier, $1));
                                      addchild($$, $3); }
    | IF expression THEN expression ELSE expression  %prec LOW
                                    { $$ = newnode(If, NULL);
                                      addchild($$, $2);
                                      addchild($$, $4);
                                      addchild($$, $6); }
    | expression '+' expression     { $$ = newnode(Add, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    | expression '-' expression     { $$ = newnode(Sub, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    | expression '*' expression     { $$ = newnode(Mul, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    | expression '/' expression     { $$ = newnode(Div, NULL);
                                      addchild($$, $1);
                                      addchild($$, $3); }
    | '(' expression ')'            { $$ = $2; }  
    ;

%%

/* START subroutines section */

const char *category_name[] = {
    "Program",
    "Function",
    "Parameters",
    "Parameter",
    "Arguments",
    "Integer",
    "Double",
    "Identifier",
    "Natural",
    "Decimal",
    "Call",
    "If",
    "Add",
    "Sub",
    "Mul",
    "Div"
};

void show(struct node *node, int depth) {
    if (node == NULL) return;

    for (int i = 0; i < depth; i++)
        printf("__");

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

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s [-l|-e1]\n", prog);
}

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            print_tokens = 1;
        } else if (strcmp(argv[i], "-e1") == 0) {
            print_tokens = 0;
        } else {
            usage(argv[0]);
            return 1;
        }
    }

    while (yylex() != 0) {
        /* no-op */
    }

    return 0;
}
