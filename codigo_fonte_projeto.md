# Código Fonte do Projeto Juc

Este documento contém todo o código fonte do projeto, organizado pela linha do tempo de desenvolvimento (Meta 1 até Meta 3), com os respetivos caminhos dos ficheiros.

## Meta 1

### Ficheiro: `meta1/src/jucompiler.l`

```lex
/*
 * Autores:
 * Simão Tomás Botas Carvalho - 2021223055
 * Teodoro Marques - 2023211717
 */

%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Header gerado por yacc/bison a partir do jucompiler.y.
 * (o teu Makefile deve gerar y.tab.h) */
#include "y.tab.h"

/* Executado antes de cada ação do lexer:
 * guarda a posição inicial do token em syn_* e avança a coluna. */
#define YY_USER_ACTION { syn_line = lex_line; syn_column = lex_column; lex_column += yyleng; }

/* Para tokens com valor (IDENTIFIER, NATURAL, DECIMAL, BOOLLIT, STRLIT, etc.)
 * assume-se que o %union no yacc tem o campo 'lexeme'. */
#define PASS_TOKEN() do { \
    yylval.lexeme = malloc(strlen(yytext) + 1); \
    if (yylval.lexeme) strcpy(yylval.lexeme, yytext); \
} while(0)

int lex_line = 1;
int lex_column = 1;
int syn_line = -1;
int syn_column = -1;
int lex_errs = 0;

int print_tokens = 0;


int comment_start_line = 1;
int comment_start_col = 1;
int string_start_line = 1;
int string_start_col = 1;
char string_buffer[10000];
int buffer_pos = 0;
int string_has_error = 0;

static void sb_append_char(char c) {
    size_t cap = sizeof(string_buffer);
    if (buffer_pos < (int)cap - 1) {
        string_buffer[buffer_pos++] = c;
    } else {
        /* Truncate to avoid writing out of bounds. */
        string_has_error = 1;
    }
}

static void sb_append_two(char a, char b) {
    sb_append_char(a);
    sb_append_char(b);
}

static void print_simple_token(const char *token) {
    if (print_tokens) {
        printf("%s\n", token);
    }
}

static void print_valued_token(const char *token, const char *value) {
    if (print_tokens) {
        printf("%s(%s)\n", token, value);
    }
}

%}

%option noyywrap noinput nounput never-interactive

digit [0-9]
nonzero [1-9]
underscore _
character [a-zA-Z_$]
digitseq {digit}({digit}|{underscore})*{digit}|{digit}
%X COMMENT
%X STR
%%


"/*" { comment_start_line = syn_line; comment_start_col = syn_column; BEGIN(COMMENT); }

<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\r\n { lex_line++; lex_column = 1; }
<COMMENT>\n { lex_line++; lex_column = 1; }
<COMMENT>\r { lex_line++; lex_column = 1; }
<COMMENT>[^*/\r\n]+ { ; }
<COMMENT>[*/] { ; }
<COMMENT><<EOF>> {
    printf("Line %d, col %d: unterminated comment\n", comment_start_line, comment_start_col);
    syn_line = lex_line;
    syn_column = lex_column;
    BEGIN(INITIAL);
    return 0;
}

"//"[^\r\n]* { ; }

"\"" { BEGIN(STR); string_start_line = syn_line; string_start_col = syn_column; buffer_pos = 0; string_has_error = 0; }

<STR>\" {
    char *quoted;
    size_t len;
    size_t cap = sizeof(string_buffer);
    if (buffer_pos < 0) buffer_pos = 0;
    if ((size_t)buffer_pos >= cap) buffer_pos = (int)cap - 1;
    string_buffer[buffer_pos] = '\0';
    if (print_tokens && !string_has_error) printf("STRLIT(\"%s\")\n", string_buffer);
    if (string_has_error) {
        BEGIN(INITIAL);
        return yylex();
    }
    len = strlen(string_buffer);
    quoted = (char *)malloc(len + 3);
    if (quoted) {
        quoted[0] = '"';
        memcpy(quoted + 1, string_buffer, len);
        quoted[len + 1] = '"';
        quoted[len + 2] = '\0';
        yylval.lexeme = quoted;
    } else {
        yylval.lexeme = malloc(3);
        if (yylval.lexeme) strcpy(yylval.lexeme, "\"\"");
    }
    syn_line = string_start_line;
    syn_column = string_start_col;
    BEGIN(INITIAL);
    return STRLIT;
}

<STR>\\[fnrt\\\"] { 
    sb_append_two('\\', yytext[1]);
}

<STR>\\\r\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\\r {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\. { 
    printf("Line %d, col %d: invalid escape sequence (%s)\n", syn_line, syn_column, yytext);
    lex_errs++;
    string_has_error = 1;
}

<STR>\r\n|\r|\n {
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    syn_line = lex_line;
    syn_column = lex_column;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR><<EOF>> { 
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    syn_line = lex_line;
    syn_column = lex_column;
    BEGIN(INITIAL);
    return 0;
}

<STR>. {
    sb_append_char(yytext[0]);
}


[ \t\f]+ { ; }

\r\n { lex_line++; lex_column = 1; }
\n   { lex_line++; lex_column = 1; }
\r   { lex_line++; lex_column = 1; }


"true"|"false" { PASS_TOKEN(); print_valued_token("BOOLLIT", yytext); return BOOLLIT; }
"==" { print_simple_token("EQ"); return EQ; }
"!=" { print_simple_token("NE"); return NE; }
">=" { print_simple_token("GE"); return GE; }
"<=" { print_simple_token("LE"); return LE; }
"<<" { print_simple_token("LSHIFT"); return LSHIFT; }
">>" { print_simple_token("RSHIFT"); return RSHIFT; }
"->" { print_simple_token("ARROW"); return ARROW; }
"&&" { print_simple_token("AND"); return AND; }
"||" { print_simple_token("OR"); return OR; }
">" { print_simple_token("GT"); return GT; }
"<" { print_simple_token("LT"); return LT; }
"=" { print_simple_token("ASSIGN"); return ASSIGN; }
"!" { print_simple_token("NOT"); return NOT; }
"+" { print_simple_token("PLUS"); return PLUS; }
"-" { print_simple_token("MINUS"); return MINUS; }
"*" { print_simple_token("STAR"); return STAR; }
"/" { print_simple_token("DIV"); return DIV; }
"%" { print_simple_token("MOD"); return MOD; }
"^" { print_simple_token("XOR"); return XOR; }
"," { print_simple_token("COMMA"); return COMMA; }
";" { print_simple_token("SEMICOLON"); return SEMICOLON; }
"(" { print_simple_token("LPAR"); return LPAR; }
")" { print_simple_token("RPAR"); return RPAR; }
"{" { print_simple_token("LBRACE"); return LBRACE; }
"}" { print_simple_token("RBRACE"); return RBRACE; }
"[" { print_simple_token("LSQ"); return LSQ; }
"]" { print_simple_token("RSQ"); return RSQ; }



"boolean" { print_simple_token("BOOL"); return BOOL; }
"class" { print_simple_token("CLASS"); return CLASS; }
".length" { print_simple_token("DOTLENGTH"); return DOTLENGTH; }
"double" { print_simple_token("DOUBLE"); return DOUBLE; }
"else" { print_simple_token("ELSE"); return ELSE; }
"if" { print_simple_token("IF"); return IF; }
"int" { print_simple_token("INT"); return INT; }
"System.out.print" { print_simple_token("PRINT"); return PRINT; }
"Integer.parseInt" { print_simple_token("PARSEINT"); return PARSEINT; }
"public" { print_simple_token("PUBLIC"); return PUBLIC; }
"return" { print_simple_token("RETURN"); return RETURN; }
"static" { print_simple_token("STATIC"); return STATIC; }
"String" { print_simple_token("STRING"); return STRING; }
"void" { print_simple_token("VOID"); return VOID; }
"while" { print_simple_token("WHILE"); return WHILE; }

"++" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"--" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"null" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"Integer" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"System" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"abstract" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"assert" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"break" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"byte" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"case" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"catch" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"char" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"const" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"continue" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"default" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"do" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"enum" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"extends" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"final" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"finally" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"float" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"for" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"goto" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"implements" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"import" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"instanceof" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"interface" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"long" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"native" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"new" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"package" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"private" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"protected" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"short" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"strictfp" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"super" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"switch" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"synchronized" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"this" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"throw" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"throws" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"transient" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"try" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"volatile" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }


{digitseq}[eE][+-]?{digitseq} { PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL; }
{digitseq}?"."{digitseq}([eE][+-]?{digitseq})? { PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL; }
{digitseq}"."([eE][+-]?{digitseq})? { PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL; }

{character}+({digit}|{character})* { PASS_TOKEN(); print_valued_token("IDENTIFIER", yytext); return IDENTIFIER; }
0|{nonzero}({digit}|{underscore})*{digit}|{nonzero} { PASS_TOKEN(); print_valued_token("NATURAL", yytext); return NATURAL; }

. { lex_errs++; printf("Line %d, col %d: illegal character (%s)\n", syn_line, syn_column, yytext); }

<INITIAL><<EOF>> {
    syn_line = lex_line;
    syn_column = lex_column;
    return 0;
}

%%


```

## Meta 2

### Ficheiro: `meta2/src/ast.h`

```c
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
    int line, col;
    char *type_annot;  /* type annotation string (e.g., "int", "(int,double)", NULL) */
    struct node_list *children;
};

struct node_list {
    struct node *node;
    struct node_list *next;
};

struct node *newnode(enum category category, char *token, int line, int col);
void addchild(struct node *parent, struct node *child);

#endif

```

### Ficheiro: `meta2/src/ast.c`

```c
/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

/* Definido em jucompiler.y */
extern const char *category_name[];

struct node *newnode(enum category category, char *token, int line, int col)
{
    struct node *n = malloc(sizeof(struct node));
    n->category = category;
    n->token = token;
    n->line = line;
    n->col = col;
    n->type_annot = NULL;
    n->children = malloc(sizeof(struct node_list));
    n->children->node = NULL;
    n->children->next = NULL;
    return n;
}

void addchild(struct node *parent, struct node *child)
{
    struct node_list *c;
    struct node_list *nl = malloc(sizeof(struct node_list));
    nl->node = child;
    nl->next = NULL;
    c = parent->children;
    while (c->next) c = c->next;
    c->next = nl;
}

static void printast_rec(struct node *n, int depth)
{
    int i;
    struct node_list *c;
    if (!n) return;

    for (i = 0; i < depth; i++) printf("..");

    printf("%s", category_name[n->category]);
    if (n->token) printf("(%s)", n->token);
    if (n->type_annot) printf(" - %s", n->type_annot);
    printf("\n");

    for (c = n->children; c; c = c->next)
        printast_rec(c->node, depth + 1);
}

void printast(struct node *root)
{
    printast_rec(root, 0);
}

```

### Ficheiro: `meta2/src/jucompiler.l`

```lex
/*
 * Autores:
 * Simão Tomás Botas Carvalho - 2021223055
 * Teodoro Marques - 2023211717
 */

%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

/* Header gerado por yacc/bison a partir do jucompiler.y.
 * (o teu Makefile deve gerar y.tab.h) */
#include "y.tab.h"

/* Executado antes de cada ação do lexer:
 * guarda a posição inicial do token em syn_* e avança a coluna.
 * Também atualiza yylloc para rastrear localizações Yacc. */
#define YY_USER_ACTION { \
    yylloc.first_line = lex_line; \
    yylloc.first_column = lex_column; \
    syn_line = lex_line; \
    syn_column = lex_column; \
    lex_column += yyleng; \
    yylloc.last_line = lex_line; \
    yylloc.last_column = lex_column; \
}

/* Para tokens com valor (IDENTIFIER, NATURAL, DECIMAL, BOOLLIT, STRLIT, etc.)
 * assume-se que o %union no yacc tem o campo 'lexeme'. */
#define PASS_TOKEN() do { \
    yylval.lexeme = malloc(strlen(yytext) + 1); \
    if (yylval.lexeme) strcpy(yylval.lexeme, yytext); \
} while(0)

int lex_line = 1;
int lex_column = 1;
int syn_line = -1;
int syn_column = -1;
int lex_errs = 0;

int print_tokens = 0;


int comment_start_line = 1;
int comment_start_col = 1;
int string_start_line = 1;
int string_start_col = 1;
char string_buffer[10000];
int buffer_pos = 0;
int string_has_error = 0;

static void sb_append_char(char c) {
    size_t cap = sizeof(string_buffer);
    if (buffer_pos < (int)cap - 1) {
        string_buffer[buffer_pos++] = c;
    } else {
        /* Truncate to avoid writing out of bounds. */
        string_has_error = 1;
    }
}

static void sb_append_two(char a, char b) {
    sb_append_char(a);
    sb_append_char(b);
}

static void print_simple_token(const char *token) {
    if (print_tokens) {
        printf("%s\n", token);
    }
}

static void print_valued_token(const char *token, const char *value) {
    if (print_tokens) {
        printf("%s(%s)\n", token, value);
    }
}

static int check_natural_bounds(const char *text) {
    /* Check if integer literal is within int range (0 to 2147483647) */
    /* Remove underscores for comparison */
    char clean[100];
    int j = 0;
    for (int i = 0; text[i]; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
        }
    }
    clean[j] = '\0';

    /* Check if it's larger than INT_MAX (2147483647) */
    if (j > 10) return 0; /* More than 10 digits, definitely out of bounds */
    if (j == 10 && strcmp(clean, "2147483647") > 0) return 0;
    return 1;
}

static int check_decimal_bounds(const char *text) {
    /* Check if decimal literal is within valid double range */
    /* First, remove underscores (Java allows them, strtod doesn't) */
    char clean[1000];
    int j = 0;
    int has_digits = 0;
    for (int i = 0; text[i] && j < (int)sizeof(clean) - 1; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
            if (text[i] >= '0' && text[i] <= '9') has_digits = 1;
        }
    }
    clean[j] = '\0';

    char *endptr;
    double val = strtod(clean, &endptr);

    /* strtod returns HUGE_VAL on overflow */
    if (val == HUGE_VAL || val == -HUGE_VAL) return 0;

    /* Check for underflow: if we have significant digits but got 0, it underflowed */
    if (val == 0.0 && has_digits) {
        /* Need to check if original string was non-zero */
        /* If string contains non-zero digits and converted to 0, it's underflow */
        for (int i = 0; clean[i]; i++) {
            if (clean[i] >= '1' && clean[i] <= '9') return 0;  /* Non-zero digit found, underflowed */
        }
    }

    return 1;
}

%}

%option noyywrap noinput nounput never-interactive

digit [0-9]
nonzero [1-9]
underscore _
character [a-zA-Z_$]
digitseq {digit}({digit}|{underscore})*{digit}|{digit}
%X COMMENT
%X STR
%%


"/*" { comment_start_line = syn_line; comment_start_col = syn_column; BEGIN(COMMENT); }

<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\r\n { lex_line++; lex_column = 1; }
<COMMENT>\n { lex_line++; lex_column = 1; }
<COMMENT>\r { lex_line++; lex_column = 1; }
<COMMENT>[^*/\r\n]+ { ; }
<COMMENT>[*/] { ; }
<COMMENT><<EOF>> {
    printf("Line %d, col %d: unterminated comment\n", comment_start_line, comment_start_col);
    syn_line = lex_line;
    syn_column = lex_column;
    BEGIN(INITIAL);
    return 0;
}

"//"[^\r\n]* { ; }

"\"" { BEGIN(STR); string_start_line = syn_line; string_start_col = syn_column; buffer_pos = 0; string_has_error = 0; }

<STR>\" {
    char *quoted;
    size_t len;
    size_t cap = sizeof(string_buffer);
    if (buffer_pos < 0) buffer_pos = 0;
    if ((size_t)buffer_pos >= cap) buffer_pos = (int)cap - 1;
    string_buffer[buffer_pos] = '\0';
    if (print_tokens && !string_has_error) printf("STRLIT(\"%s\")\n", string_buffer);
    if (string_has_error) {
        BEGIN(INITIAL);
        return yylex();
    }
    len = strlen(string_buffer);
    quoted = (char *)malloc(len + 3);
    if (quoted) {
        quoted[0] = '"';
        memcpy(quoted + 1, string_buffer, len);
        quoted[len + 1] = '"';
        quoted[len + 2] = '\0';
        yylval.lexeme = quoted;
    } else {
        yylval.lexeme = malloc(3);
        if (yylval.lexeme) strcpy(yylval.lexeme, "\"\"");
    }
    syn_line = string_start_line;
    syn_column = string_start_col;
    BEGIN(INITIAL);
    return STRLIT;
}

<STR>\\[fnrt\\\"] { 
    sb_append_two('\\', yytext[1]);
}

<STR>\\\r\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\\r {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\. { 
    printf("Line %d, col %d: invalid escape sequence (%s)\n", syn_line, syn_column, yytext);
    lex_errs++;
    string_has_error = 1;
}

<STR>\r\n|\r|\n {
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    syn_line = lex_line;
    syn_column = lex_column;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR><<EOF>> { 
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    syn_line = lex_line;
    syn_column = lex_column;
    BEGIN(INITIAL);
    return 0;
}

<STR>. {
    sb_append_char(yytext[0]);
}


[ \t\f]+ { ; }

\r\n { lex_line++; lex_column = 1; }
\n   { lex_line++; lex_column = 1; }
\r   { lex_line++; lex_column = 1; }


"true"|"false" { PASS_TOKEN(); print_valued_token("BOOLLIT", yytext); return BOOLLIT; }
"==" { print_simple_token("EQ"); return EQ; }
"!=" { print_simple_token("NE"); return NE; }
">=" { print_simple_token("GE"); return GE; }
"<=" { print_simple_token("LE"); return LE; }
"<<" { print_simple_token("LSHIFT"); return LSHIFT; }
">>" { print_simple_token("RSHIFT"); return RSHIFT; }
"->" { print_simple_token("ARROW"); return ARROW; }
"&&" { print_simple_token("AND"); return AND; }
"||" { print_simple_token("OR"); return OR; }
">" { print_simple_token("GT"); return GT; }
"<" { print_simple_token("LT"); return LT; }
"=" { print_simple_token("ASSIGN"); return ASSIGN; }
"!" { print_simple_token("NOT"); return NOT; }
"+" { print_simple_token("PLUS"); return PLUS; }
"-" { print_simple_token("MINUS"); return MINUS; }
"*" { print_simple_token("STAR"); return STAR; }
"/" { print_simple_token("DIV"); return DIV; }
"%" { print_simple_token("MOD"); return MOD; }
"^" { print_simple_token("XOR"); return XOR; }
"," { print_simple_token("COMMA"); return COMMA; }
";" { print_simple_token("SEMICOLON"); return SEMICOLON; }
"(" { print_simple_token("LPAR"); return LPAR; }
")" { print_simple_token("RPAR"); return RPAR; }
"{" { print_simple_token("LBRACE"); return LBRACE; }
"}" { print_simple_token("RBRACE"); return RBRACE; }
"[" { print_simple_token("LSQ"); return LSQ; }
"]" { print_simple_token("RSQ"); return RSQ; }



"boolean" { print_simple_token("BOOL"); return BOOL; }
"class" { print_simple_token("CLASS"); return CLASS; }
".length" { print_simple_token("DOTLENGTH"); return DOTLENGTH; }
"double" { print_simple_token("DOUBLE"); return DOUBLE; }
"else" { print_simple_token("ELSE"); return ELSE; }
"if" { print_simple_token("IF"); return IF; }
"int" { print_simple_token("INT"); return INT; }
"System.out.print" { print_simple_token("PRINT"); return PRINT; }
"Integer.parseInt" { print_simple_token("PARSEINT"); return PARSEINT; }
"public" { print_simple_token("PUBLIC"); return PUBLIC; }
"return" { print_simple_token("RETURN"); return RETURN; }
"static" { print_simple_token("STATIC"); return STATIC; }
"String" { print_simple_token("STRING"); return STRING; }
"void" { print_simple_token("VOID"); return VOID; }
"while" { print_simple_token("WHILE"); return WHILE; }

"++" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"--" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"null" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"Integer" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"System" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"abstract" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"assert" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"break" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"byte" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"case" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"catch" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"char" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"const" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"continue" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"default" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"do" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"enum" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"extends" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"final" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"finally" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"float" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"for" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"goto" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"implements" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"import" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"instanceof" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"interface" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"long" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"native" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"new" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"package" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"private" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"protected" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"short" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"strictfp" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"super" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"switch" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"synchronized" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"this" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"throw" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"throws" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"transient" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"try" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"volatile" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }


{digitseq}[eE][+-]?{digitseq} {
    PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL;
}
{digitseq}?"."{digitseq}([eE][+-]?{digitseq})? {
    PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL;
}
{digitseq}"."([eE][+-]?{digitseq})? {
    PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL;
}

{character}+({digit}|{character})* { PASS_TOKEN(); print_valued_token("IDENTIFIER", yytext); return IDENTIFIER; }
0|{nonzero}({digit}|{underscore})*{digit}|{nonzero} {
    PASS_TOKEN(); print_valued_token("NATURAL", yytext); return NATURAL;
}

. { lex_errs++; printf("Line %d, col %d: illegal character (%s)\n", syn_line, syn_column, yytext); }

<INITIAL><<EOF>> {
    syn_line = lex_line;
    syn_column = lex_column;
    return 0;
}

%%


```

### Ficheiro: `meta2/src/jucompiler.y`

```yacc
/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 *
 * Meta 2 -- Analisador Sintático (Juc)
 */

%locations

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#define YYDEBUG 0

int  yylex(void);
int  yyparse(void);
void yyerror(const char *s);
void printast(struct node *root);

extern int  lex_line, lex_column;
extern int  syn_line, syn_column;
extern int  lex_errs;
extern char *yytext;
extern int  yychar;
extern int  print_tokens;

struct node *ast      = NULL;
int          syn_errs = 0;

static struct node *cur_type_node;
static struct node *cur_field_type;
static struct node *prog_root;
static struct node *vd_accum;
static char        *vd_first_id;
static char        *class_id;

static struct node *make_block(struct node *sl);
static struct node *type_node(enum category cat, int line, int col)
{
    return newnode(cat, NULL, line, col);
}
static void add_field(struct node *tn, char *id, int line, int col);
static void add_vardecl(struct node *acc, struct node *tn, char *id, int line, int col);

static struct node *err_cond_placeholder(void)
{
    return newnode(N_Block, NULL, 0, 0);
}
%}

%union {
    char        *lexeme;
    struct node *n;
}

%token BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT
       PUBLIC RETURN STATIC STRING VOID WHILE
%token EQ NE GE LE LSHIFT RSHIFT ARROW AND OR GT LT ASSIGN NOT
%token PLUS MINUS STAR DIV MOD XOR
%token COMMA SEMICOLON LPAR RPAR LBRACE RBRACE LSQ RSQ

%token <lexeme> IDENTIFIER NATURAL DECIMAL STRLIT BOOLLIT RESERVED

%type <n> class_members method_decl method_header method_body
%type <n> type formal_params fp_list
%type <n> stmt_list method_stmt_list var_decl
%type <n> stmt stmt_no_if
%type <n> expr print_arg opt_expr block_stmt op_expr
%type <n> call_args nonempty_call_args method_invocation assignment_expr parse_args_stmt

%nonassoc IFX
%nonassoc ELSE

%right ASSIGN
%left OR
%left AND
%left XOR
%left EQ NE
%left LT GT LE GE
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD

%nonassoc UPLUS UMINUS UNOT

%%

program:
      CLASS IDENTIFIER { class_id = $2; } LBRACE class_members RBRACE
        { ast = $5; }
    ;

class_members:
      /* vazio */
        {
            $$ = newnode(N_Program, NULL, @$.first_line, @$.first_column);
            addchild($$, newnode(N_Identifier, class_id, @$.first_line, @$.first_column));
            prog_root  = $$;
            vd_accum   = NULL;
        }
    | class_members PUBLIC STATIC field_decl
        { $$ = $1; }
    | class_members PUBLIC STATIC method_decl
        { $$ = $1; addchild($1, $4); }
    | class_members SEMICOLON
        { $$ = $1; }
    | class_members error SEMICOLON
        { $$ = $1; }
    ;

field_decl:
      type IDENTIFIER
        {
            cur_field_type = $1;
            add_field($1, $2, @2.first_line, @2.first_column);
        }
      field_id_list SEMICOLON
    | error SEMICOLON
        { }
    ;

field_id_list:
      /* vazio */
    | field_id_list COMMA IDENTIFIER
        {
            add_field(cur_field_type, $3, @3.first_line, @3.first_column);
        }
    ;

method_decl:
      method_header method_body
        {
            $$ = newnode(N_MethodDecl, NULL, @$.first_line, @$.first_column);
            addchild($$, $1);
            addchild($$, $2);
        }
    ;

method_header:
      type IDENTIFIER LPAR formal_params RPAR
        {
            $$ = newnode(N_MethodHeader, NULL, @$.first_line, @$.first_column);
            addchild($$, $1);
            addchild($$, newnode(N_Identifier, $2, @2.first_line, @2.first_column));
            addchild($$, $4);
        }
    | VOID IDENTIFIER LPAR formal_params RPAR
        {
            $$ = newnode(N_MethodHeader, NULL, @$.first_line, @$.first_column);
            addchild($$, type_node(N_Void, @1.first_line, @1.first_column));
            addchild($$, newnode(N_Identifier, $2, @2.first_line, @2.first_column));
            addchild($$, $4);
        }
    ;

formal_params:
      fp_list
    | STRING LSQ RSQ IDENTIFIER
        {
            struct node *pd = newnode(N_ParamDecl, NULL, @$.first_line, @$.first_column);
            addchild(pd, type_node(N_StringArray, @1.first_line, @1.first_column));
            addchild(pd, newnode(N_Identifier, $4, @4.first_line, @4.first_column));
            $$ = newnode(N_MethodParams, NULL, @$.first_line, @$.first_column);
            addchild($$, pd);
        }
    | /* epsilon */
        {
            $$ = newnode(N_MethodParams, NULL, yylloc.first_line, yylloc.first_column);
        }
    ;

fp_list:
      type IDENTIFIER
        {
            struct node *pd = newnode(N_ParamDecl, NULL, @$.first_line, @$.first_column);
            addchild(pd, $1);
            addchild(pd, newnode(N_Identifier, $2, @2.first_line, @2.first_column));
            $$ = newnode(N_MethodParams, NULL, @$.first_line, @$.first_column);
            addchild($$, pd);
        }
    | fp_list COMMA type IDENTIFIER
        {
            struct node *pd = newnode(N_ParamDecl, NULL, @3.first_line, @3.first_column);
            addchild(pd, $3);
            addchild(pd, newnode(N_Identifier, $4, @4.first_line, @4.first_column));
            $$ = $1;
            addchild($$, pd);
        }
    ;

type:
      INT    { $$ = type_node(N_Int,    @1.first_line, @1.first_column); }
    | DOUBLE { $$ = type_node(N_Double, @1.first_line, @1.first_column); }
    | BOOL   { $$ = type_node(N_Bool,   @1.first_line, @1.first_column); }
    ;

method_body:
      LBRACE method_stmt_list RBRACE
        {
            struct node_list *c;
            $$ = newnode(N_MethodBody, NULL, @$.first_line, @$.first_column);
            for (c = $2->children; c; c = c->next)
                if (c->node) addchild($$, c->node);
        }
    ;

method_stmt_list:
      /* vazio */   { $$ = newnode(N_MethodBody, NULL, yylloc.first_line, yylloc.first_column); }
    | method_stmt_list stmt
        {
            $$ = $1;
            if ($2) addchild($1, $2);
        }
    | method_stmt_list var_decl
        {
            struct node_list *c;
            $$ = $1;
            if ($2) {
                for (c = $2->children; c; c = c->next)
                    if (c->node) addchild($$, c->node);
            }
        }
    ;

var_decl:
      type IDENTIFIER
        {
            cur_type_node = $1;
            vd_first_id = $2;
            vd_accum = newnode(N_MethodBody, NULL, @$.first_line, @$.first_column);
            add_vardecl(vd_accum, cur_type_node, vd_first_id, @2.first_line, @2.first_column);
        }
      vd_id_list SEMICOLON
        { $$ = vd_accum; }
    ;

vd_id_list:
      /* vazio */
    | vd_id_list COMMA IDENTIFIER
        {
            add_vardecl(vd_accum, cur_type_node, $3, @3.first_line, @3.first_column);
        }
    ;

stmt_list:
      /* vazio */   { $$ = newnode(N_MethodBody, NULL, yylloc.first_line, yylloc.first_column); }
    | stmt_list stmt
        {
            $$ = $1;
            if ($2) addchild($1, $2);
        }
    ;

stmt:
      stmt_no_if
    | IF LPAR expr RPAR stmt  %prec IFX
        {
            $$ = newnode(N_If, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
            addchild($$, $5 ? $5 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
            addchild($$, newnode(N_Block, NULL, @$.first_line, @$.first_column));
        }
    | IF LPAR expr RPAR stmt ELSE stmt
        {
            $$ = newnode(N_If, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
            addchild($$, $5 ? $5 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
            addchild($$, $7 ? $7 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
        }
    ;

stmt_no_if:
    WHILE LPAR expr RPAR stmt
        {
            $$ = newnode(N_While, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
            addchild($$, $5 ? $5 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
        }
    | PRINT LPAR print_arg RPAR SEMICOLON
        {
            $$ = newnode(N_Print, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
        }
    | PRINT LPAR error RPAR SEMICOLON
        { $$ = NULL; }
    | PRINT LPAR error SEMICOLON
        { $$ = NULL; }
    | block_stmt
    | method_invocation SEMICOLON
        { $$ = $1; }
    | assignment_expr SEMICOLON
        { $$ = $1; }
    | parse_args_stmt SEMICOLON
        { $$ = $1; }
    | RETURN opt_expr SEMICOLON
        {
            $$ = newnode(N_Return, NULL, @1.first_line, @1.first_column);
            if ($2) addchild($$, $2);
        }
    | RETURN error SEMICOLON
        {
            $$ = newnode(N_Return, NULL, @1.first_line, @1.first_column);
        }
    | error SEMICOLON
        { $$ = NULL; }
    | SEMICOLON
        { $$ = NULL; }
    ;

block_stmt:
      LBRACE stmt_list RBRACE
        {
            struct node *blk = make_block($2);
            /* If block is empty (only sentinel child, no real content), skip it */
            if (blk && blk->category == N_Block && !blk->children->next) {
                $$ = NULL;
            } else {
                $$ = blk;
            }
        }
    ;

method_invocation:
      IDENTIFIER LPAR call_args RPAR
        {
            struct node_list *c;
            $$ = newnode(N_Call, NULL, @1.first_line, @1.first_column);
            addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column));
            for (c = $3->children; c; c = c->next) addchild($$, c->node);
        }
    | IDENTIFIER LPAR error RPAR
        { $$ = NULL; }
    ;

call_args:
      /* vazio */       { $$ = newnode(N_MethodBody, NULL, yylloc.first_line, yylloc.first_column); }
    | nonempty_call_args  { $$ = $1; }
    ;

nonempty_call_args:
      expr              { $$ = newnode(N_MethodBody, NULL, @$.first_line, @$.first_column); addchild($$, $1); }
    | nonempty_call_args COMMA expr
        { $$ = $1; addchild($$, $3); }
    ;

assignment_expr:
      IDENTIFIER ASSIGN expr
        {
            $$ = newnode(N_Assign, NULL, @2.first_line, @2.first_column);
            addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column));
            addchild($$, $3);
        }
    ;

parse_args_stmt:
      PARSEINT LPAR IDENTIFIER LSQ expr RSQ RPAR
        {
            $$ = newnode(N_ParseArgs, NULL, @1.first_line, @1.first_column);
            addchild($$, newnode(N_Identifier, $3, @3.first_line, @3.first_column));
            addchild($$, $5);
        }
    | PARSEINT LPAR IDENTIFIER LSQ error RSQ RPAR
        { $$ = NULL; }
    | PARSEINT LPAR IDENTIFIER LSQ error RPAR
        { $$ = NULL; }
    | PARSEINT LPAR error RPAR
        { $$ = NULL; }
    ;

opt_expr:
      /* vazio */ { $$ = NULL; }
    | expr        { $$ = $1; }
    ;

print_arg:
      expr   { $$ = $1; }
    | STRLIT { $$ = newnode(N_StrLit, $1, @1.first_line, @1.first_column); }
    ;

expr:
      assignment_expr
        { $$ = $1; }
    | op_expr
        { $$ = $1; }
    ;

op_expr:
      NATURAL
        { $$ = newnode(N_Natural, $1, @1.first_line, @1.first_column); }
    | DECIMAL
        { $$ = newnode(N_Decimal, $1, @1.first_line, @1.first_column); }
    | BOOLLIT
        { $$ = newnode(N_BoolLit, $1, @1.first_line, @1.first_column); }
    | LPAR expr RPAR  { $$ = $2; }
    | LPAR error RPAR { $$ = NULL; }
    | IDENTIFIER
        { $$ = newnode(N_Identifier, $1, @1.first_line, @1.first_column); }
    | IDENTIFIER DOTLENGTH
        { $$ = newnode(N_Length, NULL, @2.first_line, @2.first_column);
          addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column)); }
    | IDENTIFIER LPAR call_args RPAR
        {
            struct node_list *c;
            $$ = newnode(N_Call, NULL, @1.first_line, @1.first_column);
            addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column));
            for (c = $3->children; c; c = c->next) addchild($$, c->node);
        }
    | IDENTIFIER LPAR error RPAR
        { $$ = NULL; }
    | PARSEINT LPAR IDENTIFIER LSQ expr RSQ RPAR
        { $$ = newnode(N_ParseArgs, NULL, @1.first_line, @1.first_column);
          addchild($$, newnode(N_Identifier, $3, @3.first_line, @3.first_column));
          addchild($$, $5); }
    | PARSEINT LPAR IDENTIFIER LSQ error RSQ RPAR
        { $$ = NULL; }
    | PARSEINT LPAR error RPAR
        { $$ = NULL; }
    | MINUS op_expr %prec UMINUS
        { $$ = newnode(N_Minus, NULL, @1.first_line, @1.first_column); addchild($$, $2); }
    | PLUS  op_expr %prec UPLUS
        { $$ = newnode(N_Plus,  NULL, @1.first_line, @1.first_column); addchild($$, $2); }
    | NOT   op_expr %prec UNOT
        { $$ = newnode(N_Not,   NULL, @1.first_line, @1.first_column); addchild($$, $2); }
    | op_expr OR     op_expr  { $$ = newnode(N_Or,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr AND    op_expr  { $$ = newnode(N_And,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr XOR    op_expr  { $$ = newnode(N_Xor,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr EQ     op_expr  { $$ = newnode(N_Eq,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr NE     op_expr  { $$ = newnode(N_Ne,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr LT     op_expr  { $$ = newnode(N_Lt,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr GT     op_expr  { $$ = newnode(N_Gt,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr LE     op_expr  { $$ = newnode(N_Le,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr GE     op_expr  { $$ = newnode(N_Ge,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr LSHIFT op_expr  { $$ = newnode(N_Lshift, NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr RSHIFT op_expr  { $$ = newnode(N_Rshift, NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr PLUS   op_expr  { $$ = newnode(N_Add,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr MINUS  op_expr  { $$ = newnode(N_Sub,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr STAR   op_expr  { $$ = newnode(N_Mul,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr DIV    op_expr  { $$ = newnode(N_Div,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr MOD    op_expr  { $$ = newnode(N_Mod,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    ;

%%

static void add_field(struct node *tn, char *id, int line, int col)
{
    struct node *fd = newnode(N_FieldDecl, NULL, line, col);
    addchild(fd, tn);
    addchild(fd, newnode(N_Identifier, id, line, col));
    addchild(prog_root, fd);
}

static void add_vardecl(struct node *acc, struct node *tn, char *id, int line, int col)
{
    struct node *vd = newnode(N_VarDecl, NULL, line, col);
    addchild(vd, tn);
    addchild(vd, newnode(N_Identifier, id, line, col));
    if (acc)
        addchild(acc, vd);
}

const char *category_name[] = {
    "Program", "FieldDecl", "VarDecl", "MethodDecl",
    "MethodHeader", "MethodParams", "ParamDecl", "MethodBody",
    "Block", "If", "While", "Return", "Call", "Print", "ParseArgs", "Assign",
    "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge",
    "Add", "Sub", "Mul", "Div", "Mod",
    "Lshift", "Rshift", "Xor", "Not", "Minus", "Plus", "Length",
    "Int", "Double", "Bool", "StringArray", "Void",
    "Identifier", "Natural", "Decimal", "BoolLit", "StrLit"
};

static struct node *make_block(struct node *sl)
{
    int nc = 0;
    int saw_any = 0;
    struct node *single = NULL;
    struct node_list *c;
    struct node *blk;
    struct node_list *c2;

    if (!sl) return newnode(N_Block, NULL, 0, 0);

    for (c = sl->children; c; c = c->next) {
        if (c->node) {
            int is_empty_block = 0;
            struct node_list *cc;
            saw_any = 1;
            if (c->node->category == N_Block) {
                is_empty_block = 1;
                for (cc = c->node->children; cc; cc = cc->next)
                    if (cc->node) { is_empty_block = 0; break; }
            }
            if (is_empty_block) continue;
            nc++;
            if (nc == 1) single = c->node;
        }
    }
    if (nc == 0)
        return saw_any ? NULL : newnode(N_Block, NULL, 0, 0);
    if (nc == 1) return single;

    blk = newnode(N_Block, NULL, 0, 0);
    for (c2 = sl->children; c2; c2 = c2->next) {
        if (c2->node) {
            int is_empty_block = 0;
            struct node_list *cc;
            if (c2->node->category == N_Block) {
                is_empty_block = 1;
                for (cc = c2->node->children; cc; cc = cc->next)
                    if (cc->node) { is_empty_block = 0; break; }
            }
            if (!is_empty_block) addchild(blk, c2->node);
        }
    }
    return blk;
}

void yyerror(const char *msg)
{
    syn_errs++;
    if (yytext && yytext[0])
        if (yychar == STRLIT && yylval.lexeme)
            printf("Line %d, col %d: syntax error: %s\n",
                   syn_line, syn_column, yylval.lexeme);
        else
            printf("Line %d, col %d: syntax error: %s\n",
                   syn_line, syn_column, yytext);
    else
        printf("Line %d, col %d: syntax error: \n",
               syn_line, syn_column);
}

int main(int argc, char **argv)
{
    int i;
    int parse_status;
    int mode = 2;

    for (i = 1; i < argc; i++) {
        if      (!strcmp(argv[i], "-l"))  mode = 0;
        else if (!strcmp(argv[i], "-e1")) mode = 1;
        else if (!strcmp(argv[i], "-e2")) mode = 2;
        else if (!strcmp(argv[i], "-t"))  mode = 3;
    }
    if (mode == 0 || mode == 1) {
        print_tokens = (mode == 0);
        while (yylex() != 0) ;
        return 0;
    }
    print_tokens = 0;
    parse_status = yyparse();
    if (parse_status != 0 && lex_errs > 0) {
        while (yylex() != 0) ;
    }
    if (mode == 3 && ast && syn_errs == 0)
        printast(ast);
    return 0;
}

```

## Meta 3

### Ficheiro: `Meta3/src/ast.h`

```c
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
    int line, col;
    char *type_annot;  /* type annotation string (e.g., "int", "(int,double)", NULL) */
    struct node_list *children;
};

struct node_list {
    struct node *node;
    struct node_list *next;
};

struct node *newnode(enum category category, char *token, int line, int col);
void addchild(struct node *parent, struct node *child);

#endif

```

### Ficheiro: `Meta3/src/ast.c`

```c
/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

/* Definido em jucompiler.y */
extern const char *category_name[];

struct node *newnode(enum category category, char *token, int line, int col)
{
    struct node *n = malloc(sizeof(struct node));
    n->category = category;
    n->token = token;
    n->line = line;
    n->col = col;
    n->type_annot = NULL;
    n->children = malloc(sizeof(struct node_list));
    n->children->node = NULL;
    n->children->next = NULL;
    return n;
}

void addchild(struct node *parent, struct node *child)
{
    struct node_list *c;
    struct node_list *nl = malloc(sizeof(struct node_list));
    nl->node = child;
    nl->next = NULL;
    c = parent->children;
    while (c->next) c = c->next;
    c->next = nl;
}

static void printast_rec(struct node *n, int depth)
{
    int i;
    struct node_list *c;
    if (!n) return;

    for (i = 0; i < depth; i++) printf("..");

    printf("%s", category_name[n->category]);
    if (n->token) printf("(%s)", n->token);
    if (n->type_annot) printf(" - %s", n->type_annot);
    printf("\n");

    for (c = n->children; c; c = c->next)
        printast_rec(c->node, depth + 1);
}

void printast(struct node *root)
{
    printast_rec(root, 0);
}

```

### Ficheiro: `Meta3/src/jucompiler.l`

```lex
/*
 * Autores:
 * Simão Tomás Botas Carvalho - 2021223055
 * Teodoro Marques - 2023211717
 */

%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

/* Header gerado por yacc/bison a partir do jucompiler.y.
 * (o teu Makefile deve gerar y.tab.h) */
#include "y.tab.h"

/* Executado antes de cada ação do lexer:
 * guarda a posição inicial do token em syn_* e avança a coluna.
 * Também atualiza yylloc para rastrear localizações Yacc. */
#define YY_USER_ACTION { \
    yylloc.first_line = lex_line; \
    yylloc.first_column = lex_column; \
    syn_line = lex_line; \
    syn_column = lex_column; \
    lex_column += yyleng; \
    yylloc.last_line = lex_line; \
    yylloc.last_column = lex_column; \
}

/* Para tokens com valor (IDENTIFIER, NATURAL, DECIMAL, BOOLLIT, STRLIT, etc.)
 * assume-se que o %union no yacc tem o campo 'lexeme'. */
#define PASS_TOKEN() do { \
    yylval.lexeme = malloc(strlen(yytext) + 1); \
    if (yylval.lexeme) strcpy(yylval.lexeme, yytext); \
} while(0)

int lex_line = 1;
int lex_column = 1;
int syn_line = -1;
int syn_column = -1;
int lex_errs = 0;

int print_tokens = 0;


int comment_start_line = 1;
int comment_start_col = 1;
int string_start_line = 1;
int string_start_col = 1;
char string_buffer[10000];
int buffer_pos = 0;
int string_has_error = 0;

static void sb_append_char(char c) {
    size_t cap = sizeof(string_buffer);
    if (buffer_pos < (int)cap - 1) {
        string_buffer[buffer_pos++] = c;
    } else {
        /* Truncate to avoid writing out of bounds. */
        string_has_error = 1;
    }
}

static void sb_append_two(char a, char b) {
    sb_append_char(a);
    sb_append_char(b);
}

static void print_simple_token(const char *token) {
    if (print_tokens) {
        printf("%s\n", token);
    }
}

static void print_valued_token(const char *token, const char *value) {
    if (print_tokens) {
        printf("%s(%s)\n", token, value);
    }
}

static int check_natural_bounds(const char *text) {
    /* Check if integer literal is within int range (0 to 2147483647) */
    /* Remove underscores for comparison */
    char clean[100];
    int j = 0;
    for (int i = 0; text[i]; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
        }
    }
    clean[j] = '\0';

    /* Check if it's larger than INT_MAX (2147483647) */
    if (j > 10) return 0; /* More than 10 digits, definitely out of bounds */
    if (j == 10 && strcmp(clean, "2147483647") > 0) return 0;
    return 1;
}

static int check_decimal_bounds(const char *text) {
    /* Check if decimal literal is within valid double range */
    /* First, remove underscores (Java allows them, strtod doesn't) */
    char clean[1000];
    int j = 0;
    int has_digits = 0;
    for (int i = 0; text[i] && j < (int)sizeof(clean) - 1; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
            if (text[i] >= '0' && text[i] <= '9') has_digits = 1;
        }
    }
    clean[j] = '\0';

    char *endptr;
    double val = strtod(clean, &endptr);

    /* strtod returns HUGE_VAL on overflow */
    if (val == HUGE_VAL || val == -HUGE_VAL) return 0;

    /* Check for underflow: if we have significant digits but got 0, it underflowed */
    if (val == 0.0 && has_digits) {
        /* Need to check if original string was non-zero */
        /* If string contains non-zero digits and converted to 0, it's underflow */
        for (int i = 0; clean[i]; i++) {
            if (clean[i] >= '1' && clean[i] <= '9') return 0;  /* Non-zero digit found, underflowed */
        }
    }

    return 1;
}

%}

%option noyywrap noinput nounput never-interactive

digit [0-9]
nonzero [1-9]
underscore _
character [a-zA-Z_$]
digitseq {digit}({digit}|{underscore})*{digit}|{digit}
%X COMMENT
%X STR
%%


"/*" { comment_start_line = syn_line; comment_start_col = syn_column; BEGIN(COMMENT); }

<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\r\n { lex_line++; lex_column = 1; }
<COMMENT>\n { lex_line++; lex_column = 1; }
<COMMENT>\r { lex_line++; lex_column = 1; }
<COMMENT>[^*/\r\n]+ { ; }
<COMMENT>[*/] { ; }
<COMMENT><<EOF>> {
    printf("Line %d, col %d: unterminated comment\n", comment_start_line, comment_start_col);
    syn_line = lex_line;
    syn_column = lex_column;
    BEGIN(INITIAL);
    return 0;
}

"//"[^\r\n]* { ; }

"\"" { BEGIN(STR); string_start_line = syn_line; string_start_col = syn_column; buffer_pos = 0; string_has_error = 0; }

<STR>\" {
    char *quoted;
    size_t len;
    size_t cap = sizeof(string_buffer);
    if (buffer_pos < 0) buffer_pos = 0;
    if ((size_t)buffer_pos >= cap) buffer_pos = (int)cap - 1;
    string_buffer[buffer_pos] = '\0';
    if (print_tokens && !string_has_error) printf("STRLIT(\"%s\")\n", string_buffer);
    if (string_has_error) {
        BEGIN(INITIAL);
        return yylex();
    }
    len = strlen(string_buffer);
    quoted = (char *)malloc(len + 3);
    if (quoted) {
        quoted[0] = '"';
        memcpy(quoted + 1, string_buffer, len);
        quoted[len + 1] = '"';
        quoted[len + 2] = '\0';
        yylval.lexeme = quoted;
    } else {
        yylval.lexeme = malloc(3);
        if (yylval.lexeme) strcpy(yylval.lexeme, "\"\"");
    }
    syn_line = string_start_line;
    syn_column = string_start_col;
    BEGIN(INITIAL);
    return STRLIT;
}

<STR>\\[fnrt\\\"] { 
    sb_append_two('\\', yytext[1]);
}

<STR>\\\r\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\\r {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\. { 
    printf("Line %d, col %d: invalid escape sequence (%s)\n", syn_line, syn_column, yytext);
    lex_errs++;
    string_has_error = 1;
}

<STR>\r\n|\r|\n {
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    syn_line = lex_line;
    syn_column = lex_column;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR><<EOF>> { 
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    syn_line = lex_line;
    syn_column = lex_column;
    BEGIN(INITIAL);
    return 0;
}

<STR>. {
    sb_append_char(yytext[0]);
}


[ \t\f]+ { ; }

\r\n { lex_line++; lex_column = 1; }
\n   { lex_line++; lex_column = 1; }
\r   { lex_line++; lex_column = 1; }


"true"|"false" { PASS_TOKEN(); print_valued_token("BOOLLIT", yytext); return BOOLLIT; }
"==" { print_simple_token("EQ"); return EQ; }
"!=" { print_simple_token("NE"); return NE; }
">=" { print_simple_token("GE"); return GE; }
"<=" { print_simple_token("LE"); return LE; }
"<<" { print_simple_token("LSHIFT"); return LSHIFT; }
">>" { print_simple_token("RSHIFT"); return RSHIFT; }
"->" { print_simple_token("ARROW"); return ARROW; }
"&&" { print_simple_token("AND"); return AND; }
"||" { print_simple_token("OR"); return OR; }
">" { print_simple_token("GT"); return GT; }
"<" { print_simple_token("LT"); return LT; }
"=" { print_simple_token("ASSIGN"); return ASSIGN; }
"!" { print_simple_token("NOT"); return NOT; }
"+" { print_simple_token("PLUS"); return PLUS; }
"-" { print_simple_token("MINUS"); return MINUS; }
"*" { print_simple_token("STAR"); return STAR; }
"/" { print_simple_token("DIV"); return DIV; }
"%" { print_simple_token("MOD"); return MOD; }
"^" { print_simple_token("XOR"); return XOR; }
"," { print_simple_token("COMMA"); return COMMA; }
";" { print_simple_token("SEMICOLON"); return SEMICOLON; }
"(" { print_simple_token("LPAR"); return LPAR; }
")" { print_simple_token("RPAR"); return RPAR; }
"{" { print_simple_token("LBRACE"); return LBRACE; }
"}" { print_simple_token("RBRACE"); return RBRACE; }
"[" { print_simple_token("LSQ"); return LSQ; }
"]" { print_simple_token("RSQ"); return RSQ; }



"boolean" { print_simple_token("BOOL"); return BOOL; }
"class" { print_simple_token("CLASS"); return CLASS; }
".length" { print_simple_token("DOTLENGTH"); return DOTLENGTH; }
"double" { print_simple_token("DOUBLE"); return DOUBLE; }
"else" { print_simple_token("ELSE"); return ELSE; }
"if" { print_simple_token("IF"); return IF; }
"int" { print_simple_token("INT"); return INT; }
"System.out.print" { print_simple_token("PRINT"); return PRINT; }
"Integer.parseInt" { print_simple_token("PARSEINT"); return PARSEINT; }
"public" { print_simple_token("PUBLIC"); return PUBLIC; }
"return" { print_simple_token("RETURN"); return RETURN; }
"static" { print_simple_token("STATIC"); return STATIC; }
"String" { print_simple_token("STRING"); return STRING; }
"void" { print_simple_token("VOID"); return VOID; }
"while" { print_simple_token("WHILE"); return WHILE; }

"++" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"--" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"null" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"Integer" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"System" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"abstract" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"assert" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"break" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"byte" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"case" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"catch" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"char" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"const" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"continue" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"default" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"do" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"enum" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"extends" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"final" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"finally" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"float" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"for" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"goto" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"implements" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"import" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"instanceof" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"interface" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"long" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"native" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"new" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"package" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"private" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"protected" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"short" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"strictfp" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"super" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"switch" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"synchronized" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"this" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"throw" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"throws" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"transient" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"try" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"volatile" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }


{digitseq}[eE][+-]?{digitseq} {
    PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL;
}
{digitseq}?"."{digitseq}([eE][+-]?{digitseq})? {
    PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL;
}
{digitseq}"."([eE][+-]?{digitseq})? {
    PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL;
}

{character}+({digit}|{character})* { PASS_TOKEN(); print_valued_token("IDENTIFIER", yytext); return IDENTIFIER; }
0|{nonzero}({digit}|{underscore})*{digit}|{nonzero} {
    PASS_TOKEN(); print_valued_token("NATURAL", yytext); return NATURAL;
}

. { lex_errs++; printf("Line %d, col %d: illegal character (%s)\n", syn_line, syn_column, yytext); }

<INITIAL><<EOF>> {
    syn_line = lex_line;
    syn_column = lex_column;
    return 0;
}

%%


```

### Ficheiro: `Meta3/src/jucompiler.y`

```yacc
/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 *
 * Meta 3 -- Semantic Analysis (Phase 1: Symbol Table)
 */

%locations

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic.h"

#define YYDEBUG 0

int  yylex(void);
int  yyparse(void);
void yyerror(const char *s);
void printast(struct node *root);

extern int  lex_line, lex_column;
extern int  syn_line, syn_column;
extern int  lex_errs;
extern char *yytext;
extern int  yychar;
extern int  print_tokens;

struct node *ast      = NULL;
int          syn_errs = 0;
ClassTable  *class_table = NULL;

static struct node *cur_type_node;
static struct node *cur_field_type;
static struct node *prog_root;
static struct node *vd_accum;
static char        *vd_first_id;
static char        *class_id;

static struct node *make_block(struct node *sl);
static struct node *type_node(enum category cat, int line, int col)
{
    return newnode(cat, NULL, line, col);
}
static void add_field(struct node *tn, char *id, int line, int col);
static void add_vardecl(struct node *acc, struct node *tn, char *id, int line, int col);

static struct node *err_cond_placeholder(void)
{
    return newnode(N_Block, NULL, 0, 0);
}
%}

%union {
    char        *lexeme;
    struct node *n;
}

%token BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT
       PUBLIC RETURN STATIC STRING VOID WHILE
%token EQ NE GE LE LSHIFT RSHIFT ARROW AND OR GT LT ASSIGN NOT
%token PLUS MINUS STAR DIV MOD XOR
%token COMMA SEMICOLON LPAR RPAR LBRACE RBRACE LSQ RSQ

%token <lexeme> IDENTIFIER NATURAL DECIMAL STRLIT BOOLLIT RESERVED

%type <n> class_members method_decl method_header method_body
%type <n> type formal_params fp_list
%type <n> stmt_list method_stmt_list var_decl
%type <n> stmt stmt_no_if
%type <n> expr print_arg opt_expr block_stmt op_expr
%type <n> call_args nonempty_call_args method_invocation assignment_expr parse_args_stmt

%nonassoc IFX
%nonassoc ELSE

%right ASSIGN
%left OR
%left AND
%left XOR
%left EQ NE
%left LT GT LE GE
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD

%nonassoc UPLUS UMINUS UNOT

%%

program:
      CLASS IDENTIFIER { class_id = $2; } LBRACE class_members RBRACE
        { ast = $5; }
    ;

class_members:
      /* vazio */
        {
            $$ = newnode(N_Program, NULL, @$.first_line, @$.first_column);
            addchild($$, newnode(N_Identifier, class_id, @$.first_line, @$.first_column));
            prog_root  = $$;
            vd_accum   = NULL;
        }
    | class_members PUBLIC STATIC field_decl
        { $$ = $1; }
    | class_members PUBLIC STATIC method_decl
        { $$ = $1; addchild($1, $4); }
    | class_members SEMICOLON
        { $$ = $1; }
    | class_members error SEMICOLON
        { $$ = $1; }
    ;

field_decl:
      type IDENTIFIER
        {
            cur_field_type = $1;
            add_field($1, $2, @2.first_line, @2.first_column);
        }
      field_id_list SEMICOLON
    | error SEMICOLON
        { }
    ;

field_id_list:
      /* vazio */
    | field_id_list COMMA IDENTIFIER
        {
            add_field(cur_field_type, $3, @3.first_line, @3.first_column);
        }
    ;

method_decl:
      method_header method_body
        {
            $$ = newnode(N_MethodDecl, NULL, @$.first_line, @$.first_column);
            addchild($$, $1);
            addchild($$, $2);
        }
    ;

method_header:
      type IDENTIFIER LPAR formal_params RPAR
        {
            $$ = newnode(N_MethodHeader, NULL, @$.first_line, @$.first_column);
            addchild($$, $1);
            addchild($$, newnode(N_Identifier, $2, @2.first_line, @2.first_column));
            addchild($$, $4);
        }
    | VOID IDENTIFIER LPAR formal_params RPAR
        {
            $$ = newnode(N_MethodHeader, NULL, @$.first_line, @$.first_column);
            addchild($$, type_node(N_Void, @1.first_line, @1.first_column));
            addchild($$, newnode(N_Identifier, $2, @2.first_line, @2.first_column));
            addchild($$, $4);
        }
    ;

formal_params:
      fp_list
    | STRING LSQ RSQ IDENTIFIER
        {
            struct node *pd = newnode(N_ParamDecl, NULL, @$.first_line, @$.first_column);
            addchild(pd, type_node(N_StringArray, @1.first_line, @1.first_column));
            addchild(pd, newnode(N_Identifier, $4, @4.first_line, @4.first_column));
            $$ = newnode(N_MethodParams, NULL, @$.first_line, @$.first_column);
            addchild($$, pd);
        }
    | /* epsilon */
        {
            $$ = newnode(N_MethodParams, NULL, yylloc.first_line, yylloc.first_column);
        }
    ;

fp_list:
      type IDENTIFIER
        {
            struct node *pd = newnode(N_ParamDecl, NULL, @$.first_line, @$.first_column);
            addchild(pd, $1);
            addchild(pd, newnode(N_Identifier, $2, @2.first_line, @2.first_column));
            $$ = newnode(N_MethodParams, NULL, @$.first_line, @$.first_column);
            addchild($$, pd);
        }
    | fp_list COMMA type IDENTIFIER
        {
            struct node *pd = newnode(N_ParamDecl, NULL, @3.first_line, @3.first_column);
            addchild(pd, $3);
            addchild(pd, newnode(N_Identifier, $4, @4.first_line, @4.first_column));
            $$ = $1;
            addchild($$, pd);
        }
    ;

type:
      INT    { $$ = type_node(N_Int,    @1.first_line, @1.first_column); }
    | DOUBLE { $$ = type_node(N_Double, @1.first_line, @1.first_column); }
    | BOOL   { $$ = type_node(N_Bool,   @1.first_line, @1.first_column); }
    ;

method_body:
      LBRACE method_stmt_list RBRACE
        {
            struct node_list *c;
            $$ = newnode(N_MethodBody, NULL, @$.first_line, @$.first_column);
            for (c = $2->children; c; c = c->next)
                if (c->node) addchild($$, c->node);
        }
    ;

var_decl:
      type IDENTIFIER
        {
            cur_type_node = $1;
            vd_first_id = $2;
            vd_accum = newnode(N_MethodBody, NULL, @$.first_line, @$.first_column);
            add_vardecl(vd_accum, cur_type_node, vd_first_id, @2.first_line, @2.first_column);
        }
      vd_id_list SEMICOLON
        { $$ = vd_accum; }
    ;

vd_id_list:
      /* vazio */
    | vd_id_list COMMA IDENTIFIER
        {
            add_vardecl(vd_accum, cur_type_node, $3, @3.first_line, @3.first_column);
        }
    ;

method_stmt_list:
      /* vazio */   { $$ = newnode(N_MethodBody, NULL, yylloc.first_line, yylloc.first_column); }
    | method_stmt_list stmt
        {
            $$ = $1;
            if ($2) addchild($1, $2);
        }
    | method_stmt_list var_decl
        {
            struct node_list *c;
            $$ = $1;
            if ($2) {
                for (c = $2->children; c; c = c->next)
                    if (c->node) addchild($$, c->node);
            }
        }
    ;

stmt_list:
      /* vazio */   { $$ = newnode(N_MethodBody, NULL, yylloc.first_line, yylloc.first_column); }
    | stmt_list stmt
        {
            $$ = $1;
            if ($2) addchild($1, $2);
        }
    ;

stmt:
      stmt_no_if
    | IF LPAR expr RPAR stmt  %prec IFX
        {
            $$ = newnode(N_If, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
            addchild($$, $5 ? $5 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
            addchild($$, newnode(N_Block, NULL, @$.first_line, @$.first_column));
        }
    | IF LPAR expr RPAR stmt ELSE stmt
        {
            $$ = newnode(N_If, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
            addchild($$, $5 ? $5 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
            addchild($$, $7 ? $7 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
        }
    ;

stmt_no_if:
    WHILE LPAR expr RPAR stmt
        {
            $$ = newnode(N_While, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
            addchild($$, $5 ? $5 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
        }
    | PRINT LPAR print_arg RPAR SEMICOLON
        {
            $$ = newnode(N_Print, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
        }
    | PRINT LPAR error RPAR SEMICOLON
        { $$ = NULL; }
    | PRINT LPAR error SEMICOLON
        { $$ = NULL; }
    | block_stmt
    | method_invocation SEMICOLON
        { $$ = $1; }
    | assignment_expr SEMICOLON
        { $$ = $1; }
    | parse_args_stmt SEMICOLON
        { $$ = $1; }
    | RETURN opt_expr SEMICOLON
        {
            $$ = newnode(N_Return, NULL, @1.first_line, @1.first_column);
            if ($2) addchild($$, $2);
        }
    | RETURN error SEMICOLON
        {
            $$ = newnode(N_Return, NULL, @1.first_line, @1.first_column);
        }
    | error SEMICOLON
        { $$ = NULL; }
    | SEMICOLON
        { $$ = NULL; }
    ;

block_stmt:
      LBRACE stmt_list RBRACE
        {
            struct node *blk = make_block($2);
            /* If block is empty (only sentinel child, no real content), skip it */
            if (blk && blk->category == N_Block && !blk->children->next) {
                $$ = NULL;
            } else {
                $$ = blk;
            }
        }
    ;

method_invocation:
      IDENTIFIER LPAR call_args RPAR
        {
            struct node_list *c;
            $$ = newnode(N_Call, NULL, @1.first_line, @1.first_column);
            addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column));
            for (c = $3->children; c; c = c->next) addchild($$, c->node);
        }
    | IDENTIFIER LPAR error RPAR
        { $$ = NULL; }
    ;

call_args:
      /* vazio */       { $$ = newnode(N_MethodBody, NULL, yylloc.first_line, yylloc.first_column); }
    | nonempty_call_args  { $$ = $1; }
    ;

nonempty_call_args:
      expr              { $$ = newnode(N_MethodBody, NULL, @$.first_line, @$.first_column); addchild($$, $1); }
    | nonempty_call_args COMMA expr
        { $$ = $1; addchild($$, $3); }
    ;

assignment_expr:
      IDENTIFIER ASSIGN expr
        {
            $$ = newnode(N_Assign, NULL, @2.first_line, @2.first_column);
            addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column));
            addchild($$, $3);
        }
    ;

parse_args_stmt:
      PARSEINT LPAR IDENTIFIER LSQ expr RSQ RPAR
        {
            $$ = newnode(N_ParseArgs, NULL, @1.first_line, @1.first_column);
            addchild($$, newnode(N_Identifier, $3, @3.first_line, @3.first_column));
            addchild($$, $5);
        }
    | PARSEINT LPAR IDENTIFIER LSQ error RSQ RPAR
        { $$ = NULL; }
    | PARSEINT LPAR IDENTIFIER LSQ error RPAR
        { $$ = NULL; }
    | PARSEINT LPAR error RPAR
        { $$ = NULL; }
    ;

opt_expr:
      /* vazio */ { $$ = NULL; }
    | expr        { $$ = $1; }
    ;

print_arg:
      expr   { $$ = $1; }
    | STRLIT { $$ = newnode(N_StrLit, $1, @1.first_line, @1.first_column); }
    ;

expr:
      assignment_expr
        { $$ = $1; }
    | op_expr
        { $$ = $1; }
    ;

op_expr:
      NATURAL
        { $$ = newnode(N_Natural, $1, @1.first_line, @1.first_column); }
    | DECIMAL
        { $$ = newnode(N_Decimal, $1, @1.first_line, @1.first_column); }
    | BOOLLIT
        { $$ = newnode(N_BoolLit, $1, @1.first_line, @1.first_column); }
    | LPAR expr RPAR  { $$ = $2; }
    | LPAR error RPAR { $$ = NULL; }
    | IDENTIFIER
        { $$ = newnode(N_Identifier, $1, @1.first_line, @1.first_column); }
    | IDENTIFIER DOTLENGTH
        { $$ = newnode(N_Length, NULL, @2.first_line, @2.first_column);
          addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column)); }
    | IDENTIFIER LPAR call_args RPAR
        {
            struct node_list *c;
            $$ = newnode(N_Call, NULL, @1.first_line, @1.first_column);
            addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column));
            for (c = $3->children; c; c = c->next) addchild($$, c->node);
        }
    | IDENTIFIER LPAR error RPAR
        { $$ = NULL; }
    | PARSEINT LPAR IDENTIFIER LSQ expr RSQ RPAR
        { $$ = newnode(N_ParseArgs, NULL, @1.first_line, @1.first_column);
          addchild($$, newnode(N_Identifier, $3, @3.first_line, @3.first_column));
          addchild($$, $5); }
    | PARSEINT LPAR IDENTIFIER LSQ error RSQ RPAR
        { $$ = NULL; }
    | PARSEINT LPAR error RPAR
        { $$ = NULL; }
    | MINUS op_expr %prec UMINUS
        { $$ = newnode(N_Minus, NULL, @1.first_line, @1.first_column); addchild($$, $2); }
    | PLUS  op_expr %prec UPLUS
        { $$ = newnode(N_Plus,  NULL, @1.first_line, @1.first_column); addchild($$, $2); }
    | NOT   op_expr %prec UNOT
        { $$ = newnode(N_Not,   NULL, @1.first_line, @1.first_column); addchild($$, $2); }
    | op_expr OR     op_expr  { $$ = newnode(N_Or,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr AND    op_expr  { $$ = newnode(N_And,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr XOR    op_expr  { $$ = newnode(N_Xor,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr EQ     op_expr  { $$ = newnode(N_Eq,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr NE     op_expr  { $$ = newnode(N_Ne,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr LT     op_expr  { $$ = newnode(N_Lt,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr GT     op_expr  { $$ = newnode(N_Gt,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr LE     op_expr  { $$ = newnode(N_Le,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr GE     op_expr  { $$ = newnode(N_Ge,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr LSHIFT op_expr  { $$ = newnode(N_Lshift, NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr RSHIFT op_expr  { $$ = newnode(N_Rshift, NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr PLUS   op_expr  { $$ = newnode(N_Add,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr MINUS  op_expr  { $$ = newnode(N_Sub,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr STAR   op_expr  { $$ = newnode(N_Mul,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr DIV    op_expr  { $$ = newnode(N_Div,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr MOD    op_expr  { $$ = newnode(N_Mod,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    ;

%%

static void add_field(struct node *tn, char *id, int line, int col)
{
    struct node *fd = newnode(N_FieldDecl, NULL, line, col);
    addchild(fd, tn);
    addchild(fd, newnode(N_Identifier, id, line, col));
    addchild(prog_root, fd);
}

static void add_vardecl(struct node *acc, struct node *tn, char *id, int line, int col)
{
    struct node *vd = newnode(N_VarDecl, NULL, line, col);
    addchild(vd, tn);
    addchild(vd, newnode(N_Identifier, id, line, col));
    if (acc)
        addchild(acc, vd);
}

const char *category_name[] = {
    "Program", "FieldDecl", "VarDecl", "MethodDecl",
    "MethodHeader", "MethodParams", "ParamDecl", "MethodBody",
    "Block", "If", "While", "Return", "Call", "Print", "ParseArgs", "Assign",
    "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge",
    "Add", "Sub", "Mul", "Div", "Mod",
    "Lshift", "Rshift", "Xor", "Not", "Minus", "Plus", "Length",
    "Int", "Double", "Bool", "StringArray", "Void",
    "Identifier", "Natural", "Decimal", "BoolLit", "StrLit"
};

static struct node *make_block(struct node *sl)
{
    int nc = 0;
    int saw_any = 0;
    struct node *single = NULL;
    struct node_list *c;
    struct node *blk;
    struct node_list *c2;

    if (!sl) return newnode(N_Block, NULL, 0, 0);

    for (c = sl->children; c; c = c->next) {
        if (c->node) {
            int is_empty_block = 0;
            struct node_list *cc;
            saw_any = 1;
            if (c->node->category == N_Block) {
                is_empty_block = 1;
                for (cc = c->node->children; cc; cc = cc->next)
                    if (cc->node) { is_empty_block = 0; break; }
            }
            if (is_empty_block) continue;
            nc++;
            if (nc == 1) single = c->node;
        }
    }
    if (nc == 0)
        return saw_any ? NULL : newnode(N_Block, NULL, 0, 0);
    if (nc == 1) return single;

    blk = newnode(N_Block, NULL, 0, 0);
    for (c2 = sl->children; c2; c2 = c2->next) {
        if (c2->node) {
            int is_empty_block = 0;
            struct node_list *cc;
            if (c2->node->category == N_Block) {
                is_empty_block = 1;
                for (cc = c2->node->children; cc; cc = cc->next)
                    if (cc->node) { is_empty_block = 0; break; }
            }
            if (!is_empty_block) addchild(blk, c2->node);
        }
    }
    return blk;
}

void yyerror(const char *msg)
{
    syn_errs++;
    if (yytext && yytext[0])
        if (yychar == STRLIT && yylval.lexeme)
            printf("Line %d, col %d: syntax error: %s\n",
                   syn_line, syn_column, yylval.lexeme);
        else
            printf("Line %d, col %d: syntax error: %s\n",
                   syn_line, syn_column, yytext);
    else
        printf("Line %d, col %d: syntax error: \n",
               syn_line, syn_column);
}

int main(int argc, char **argv)
{
    int i;
    int parse_status;
    int mode = 2;

    for (i = 1; i < argc; i++) {
        if      (!strcmp(argv[i], "-l"))  mode = 0;
        else if (!strcmp(argv[i], "-e1")) mode = 1;
        else if (!strcmp(argv[i], "-e2")) mode = 2;
        else if (!strcmp(argv[i], "-t"))  mode = 3;
        else if (!strcmp(argv[i], "-s"))  mode = 4;
    }
    if (mode == 0 || mode == 1) {
        print_tokens = (mode == 0);
        while (yylex() != 0) ;
        return 0;
    }
    print_tokens = 0;
    parse_status = yyparse();
    if (parse_status != 0 && lex_errs > 0) {
        while (yylex() != 0) ;
    }
    if (mode == 3 && ast && syn_errs == 0)
        printast(ast);
    if ((mode == 2 || mode == 4) && ast && syn_errs == 0) {
        class_table = build_symbol_tables(ast);
        if (class_table) {
            check_and_annotate_ast(ast, class_table);
            if (mode == 4) {
                print_symbol_tables(class_table);
                printast(ast);
            }
            free_class_table(class_table);
        }
    }
    return 0;
}

```

### Ficheiro: `Meta3/src/semantic.h`

```c
/*
 * Semantic Analyzer Header - Meta 3
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "ast.h"

typedef enum {
    JT_INT, JT_DOUBLE, JT_BOOLEAN, JT_VOID, JT_STRING_ARRAY, JT_STRING, JT_UNDEF
} JType;

typedef struct Symbol {
    char *name;
    JType type;
    int is_param;
    int line, col;
    struct Symbol *next;
} Symbol;

typedef struct MethodEntry {
    char *name;
    JType return_type;
    JType *param_types;
    int n_params;
    Symbol *symbols;
    int line, col;
    struct MethodEntry *next;
} MethodEntry;

/* Ordered list node for class-level entries (fields and methods mixed) */
typedef enum { CE_FIELD, CE_METHOD } ClassEntryKind;
typedef struct ClassEntryNode {
    ClassEntryKind kind;
    union {
        Symbol *field;
        MethodEntry *method;
    };
    struct ClassEntryNode *next;
} ClassEntryNode;

typedef struct {
    char *name;
    Symbol *fields;
    MethodEntry *methods;
    ClassEntryNode *entries;  /* Fields and methods in declaration order */
} ClassTable;

ClassTable *build_symbol_tables(struct node *program);
void print_symbol_tables(ClassTable *ct);
void check_and_annotate_ast(struct node *program, ClassTable *ct);
void free_class_table(ClassTable *ct);

const char *jtype_to_string(JType t);
JType node_to_jtype(struct node *type_node);

#endif

```

### Ficheiro: `Meta3/src/semantic.c`

```c
/*
 * Semantic Analyzer Implementation - Meta 3
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "semantic.h"

extern const char *category_name[];

static int check_natural_bounds(const char *text) {
    /* Check if integer literal is within int range (0 to 2147483647) */
    /* Remove underscores for comparison */
    char clean[32];
    int j = 0;
    for (int i = 0; text[i] && j < 20; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
        }
    }
    clean[j] = '\0';

    /* Check if it's larger than INT_MAX (2147483647) */
    if (j > 10) return 0; /* More than 10 digits, definitely out of bounds */
    if (j == 10 && strcmp(clean, "2147483647") > 0) return 0;
    return 1;
}

static int check_decimal_bounds(const char *text) {
    /* Check if decimal literal is within valid double range */
    /* First, remove underscores (Java allows them, strtod doesn't) */
    char clean[1000];
    int j = 0;
    int has_digits = 0;
    for (int i = 0; text[i] && j < (int)sizeof(clean) - 1; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
            if (text[i] >= '0' && text[i] <= '9') has_digits = 1;
        }
    }
    clean[j] = '\0';

    char *endptr;
    double val = strtod(clean, &endptr);

    /* strtod returns HUGE_VAL on overflow */
    if (val == HUGE_VAL || val == -HUGE_VAL) return 0;

    /* Check for underflow: if we have significant digits but got 0, it underflowed */
    if (val == 0.0 && has_digits) {
        /* Need to check if original string was non-zero */
        /* If string contains non-zero digits and converted to 0, it's underflow */
        for (int i = 0; clean[i]; i++) {
            if (clean[i] >= '1' && clean[i] <= '9') return 0;  /* Non-zero digit found, underflowed */
        }
    }

    return 1;
}

const char *jtype_to_string(JType t)
{
    switch (t) {
        case JT_INT: return "int";
        case JT_DOUBLE: return "double";
        case JT_BOOLEAN: return "boolean";
        case JT_VOID: return "void";
        case JT_STRING_ARRAY: return "String[]";
        case JT_STRING: return "String";
        case JT_UNDEF: return "undef";
    }
    return "unknown";
}

JType node_to_jtype(struct node *type_node)
{
    if (!type_node) return JT_UNDEF;

    switch (type_node->category) {
        case N_Int: return JT_INT;
        case N_Double: return JT_DOUBLE;
        case N_Bool: return JT_BOOLEAN;
        case N_Void: return JT_VOID;
        case N_StringArray: return JT_STRING_ARRAY;
        default: return JT_UNDEF;
    }
}

static int is_reserved_id(const char *name)
{
    return (strcmp(name, "_") == 0) || (strcmp(name, "$") == 0);
}

static Symbol *find_symbol(Symbol *symbols, const char *name)
{
    for (Symbol *s = symbols; s; s = s->next) {
        if (strcmp(s->name, name) == 0) return s;
    }
    return NULL;
}

static void add_symbol(Symbol **symbols, const char *name, JType type, int is_param, int line, int col)
{
    Symbol *s = malloc(sizeof(Symbol));
    s->name = malloc(strlen(name) + 1);
    strcpy(s->name, name);
    s->type = type;
    s->is_param = is_param;
    s->line = line;
    s->col = col;
    s->next = NULL;

    if (!*symbols) {
        *symbols = s;
    } else {
        Symbol *tail = *symbols;
        while (tail->next) tail = tail->next;
        tail->next = s;
    }
}

static int types_compatible(JType actual, JType expected); /* Forward declaration */

static MethodEntry *find_method_by_signature(MethodEntry *methods, const char *name, int n_params)
{
    for (MethodEntry *m = methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n_params) return m;
    }
    return NULL;
}

static MethodEntry *find_exact_method(MethodEntry *methods, const char *name, int n_params,
                                       JType *arg_types)
{
    /* Find a method with exact type match (all types must be equal) */
    for (MethodEntry *m = methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n_params) {
            int exact = 1;
            for (int i = 0; i < n_params; i++) {
                if (arg_types[i] != m->param_types[i]) {
                    exact = 0;
                    break;
                }
            }
            if (exact) return m;
        }
    }
    return NULL;
}

static MethodEntry *find_compatible_method(MethodEntry *methods, const char *name, int n_params,
                                           JType *arg_types, int *out_count)
{
    /* Find all methods with matching name and parameter count whose types are compatible */
    MethodEntry *first_match = NULL;
    int match_count = 0;

    for (MethodEntry *m = methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n_params) {
            /* Check if all parameter types are compatible */
            int compatible = 1;
            for (int i = 0; i < n_params; i++) {
                if (!types_compatible(arg_types[i], m->param_types[i])) {
                    compatible = 0;
                    break;
                }
            }
            if (compatible) {
                match_count++;
                if (!first_match) first_match = m;
            }
        }
    }

    *out_count = match_count;
    return first_match;
}

static void add_class_entry(ClassTable *ct, ClassEntryKind kind, Symbol *field, MethodEntry *method)
{
    ClassEntryNode *en = malloc(sizeof(ClassEntryNode));
    en->kind = kind;
    if (kind == CE_FIELD) en->field = field;
    else en->method = method;
    en->next = NULL;
    if (!ct->entries) {
        ct->entries = en;
    } else {
        ClassEntryNode *tail = ct->entries;
        while (tail->next) tail = tail->next;
        tail->next = en;
    }
}

static MethodEntry *add_method(MethodEntry **methods, const char *name, JType return_type,
                                JType *param_types, int n_params, int line, int col)
{
    MethodEntry *m = malloc(sizeof(MethodEntry));
    m->name = malloc(strlen(name) + 1);
    strcpy(m->name, name);
    m->return_type = return_type;
    m->n_params = n_params;
    m->param_types = NULL;
    if (n_params > 0) {
        m->param_types = malloc(n_params * sizeof(JType));
        memcpy(m->param_types, param_types, n_params * sizeof(JType));
    }
    m->symbols = NULL;
    m->line = line;
    m->col = col;
    m->next = NULL;

    if (!*methods) {
        *methods = m;
    } else {
        MethodEntry *tail = *methods;
        while (tail->next) tail = tail->next;
        tail->next = m;
    }

    return m;
}

static JType extract_param_type(struct node *param_node)
{
    if (!param_node) return JT_UNDEF;
    if (param_node->category == N_StringArray) return JT_STRING_ARRAY;
    return node_to_jtype(param_node);
}

static void process_method_header(struct node *header, JType *ret_type,
                                   char **method_name, JType **param_types, int *n_params)
{
    *ret_type = JT_UNDEF;
    *method_name = NULL;
    *param_types = NULL;
    *n_params = 0;

    if (!header || header->category != N_MethodHeader) return;

    struct node_list *c = header->children;
    if (c) c = c->next;  /* Skip sentinel */
    if (!c || !c->node) return;

    /* First child: return type */
    *ret_type = node_to_jtype(c->node);
    if (c->node->category == N_Void) *ret_type = JT_VOID;

    /* Second child: identifier (method name) */
    c = c->next;
    if (c && c->node && c->node->category == N_Identifier) {
        *method_name = c->node->token;
    }

    /* Third child: MethodParams */
    c = c->next;
    if (c && c->node && c->node->category == N_MethodParams) {
        struct node_list *pc = c->node->children;
        if (pc) pc = pc->next;  /* Skip sentinel */
        int count = 0;
        for (struct node_list *tmp = pc; tmp && tmp->node; tmp = tmp->next) {
            if (tmp->node->category == N_ParamDecl) count++;
        }

        if (count > 0) {
            *param_types = malloc(count * sizeof(JType));
            int idx = 0;
            for (pc = c->node->children; pc; pc = pc->next) {
                if (pc->node && pc->node->category == N_ParamDecl) {
                    struct node_list *pd = pc->node->children;
                    if (pd) pd = pd->next;  /* Skip sentinel */
                    if (pd && pd->node) {
                        (*param_types)[idx++] = extract_param_type(pd->node);
                    }
                }
            }
        }
        *n_params = count;
    }
}

ClassTable *build_symbol_tables(struct node *program)
{
    if (!program || program->category != N_Program) return NULL;

    ClassTable *ct = malloc(sizeof(ClassTable));
    ct->name = NULL;
    ct->fields = NULL;
    ct->methods = NULL;
    ct->entries = NULL;

    /* First child is class Identifier (skip sentinel) */
    struct node_list *c = program->children;
    if (c) c = c->next;  /* Skip sentinel */
    if (c && c->node && c->node->category == N_Identifier) {
        if (c->node->token) {
            ct->name = malloc(strlen(c->node->token) + 1);
            strcpy(ct->name, c->node->token);
        }
    }

    /* Process FieldDecl and MethodDecl */
    for (c = c->next; c; c = c->next) {
        if (!c->node) {
            continue;
        }

        if (c->node->category == N_FieldDecl) {
            struct node_list *fd = c->node->children;
            if (fd) fd = fd->next;  /* Skip sentinel */

            /* Get type from first child */
            JType type = JT_UNDEF;
            if (fd && fd->node) {
                type = node_to_jtype(fd->node);
                fd = fd->next;  /* Move to first identifier */
            }

            /* Process all identifiers in this field declaration (handles int a, b, c;) */
            for (; fd && fd->node; fd = fd->next) {
                const char *id = fd->node->token;
                int line = fd->node->line;
                int col = fd->node->col;

                if (is_reserved_id(id)) {
                    printf("Line %d, col %d: Symbol %s is reserved\n", line, col, id);
                } else if (find_symbol(ct->fields, id)) {
                    printf("Line %d, col %d: Symbol %s already defined\n", line, col, id);
                } else {
                    add_symbol(&ct->fields, id, type, 0, line, col);
                    add_class_entry(ct, CE_FIELD, find_symbol(ct->fields, id), NULL);
                }
            }
        }
        else if (c->node->category == N_MethodDecl) {
            struct node_list *md = c->node->children;
            if (md) md = md->next;  /* Skip sentinel */
            if (md && md->node && md->node->category == N_MethodHeader) {
                JType ret_type;
                char *method_name;
                JType *param_types;
                int n_params;

                process_method_header(md->node, &ret_type, &method_name, &param_types, &n_params);

                if (method_name) {
                    int method_line = md->node->line;
                    int method_col = md->node->col;
                    struct node_list *hdr_c = md->node->children;
                    if (hdr_c) hdr_c = hdr_c->next;  /* Skip sentinel */
                    if (hdr_c) hdr_c = hdr_c->next;  /* Skip return type */
                    if (hdr_c && hdr_c->node && hdr_c->node->category == N_Identifier) {
                        method_line = hdr_c->node->line;
                        method_col = hdr_c->node->col;
                    }

                    /* Check for reserved method name (but don't report yet) */
                    int reserved_method = is_reserved_id(method_name);

                    /* Check for exact duplicate: same name AND same parameter types */
                    MethodEntry *existing = find_method_by_signature(ct->methods, method_name, n_params);
                    int is_duplicate = 0;
                    if (existing && n_params == existing->n_params) {
                        int types_match = 1;
                        for (int i = 0; i < n_params; i++) {
                            if (param_types[i] != existing->param_types[i]) {
                                types_match = 0;
                                break;
                            }
                        }
                        is_duplicate = types_match;
                    }

                    MethodEntry *method = NULL;
                    if (!reserved_method && !is_duplicate) {
                        method = add_method(&ct->methods, method_name, ret_type, param_types, n_params,
                                            method_line, method_col);
                        if (method) add_class_entry(ct, CE_METHOD, NULL, method);
                    }

                    /* Process parameters to check for errors FIRST (before reporting method errors) */
                    if (method) {
                            add_symbol(&method->symbols, "return", ret_type, 0, method_line, method_col);

                            struct node_list *params = md->node->children;
                            params = params->next;  /* Skip sentinel */
                            params = params->next;  /* Skip return type */
                            params = params->next;  /* Skip method name identifier */
                            if (params && params->node && params->node->category == N_MethodParams) {
                                struct node_list *pc = params->node->children;
                                if (pc) pc = pc->next;  /* Skip sentinel */
                                for (; pc; pc = pc->next) {
                                    if (pc->node && pc->node->category == N_ParamDecl) {
                                        struct node_list *pd = pc->node->children;
                                        if (pd) pd = pd->next;  /* Skip sentinel */
                                        if (pd && pd->node && pd->next && pd->next->node) {
                                            JType ptype = extract_param_type(pd->node);
                                            const char *pname = pd->next->node->token;
                                            int pline = pd->next->node->line;
                                            int pcol = pd->next->node->col;

                                            if (is_reserved_id(pname)) {
                                                printf("Line %d, col %d: Symbol %s is reserved\n",
                                                       pline, pcol, pname);
                                            } else if (find_symbol(method->symbols, pname)) {
                                                printf("Line %d, col %d: Symbol %s already defined\n",
                                                       pline, pcol, pname);
                                            } else {
                                                add_symbol(&method->symbols, pname, ptype, 1,
                                                           pline, pcol);
                                            }
                                        }
                                    }
                                }
                            }
                        } else if (reserved_method || is_duplicate) {
                            /* Process params with temp list to detect duplicate param names even if method is reserved/duplicate */
                            Symbol *temp_symbols = NULL;
                            add_symbol(&temp_symbols, "return", ret_type, 0, method_line, method_col);

                            struct node_list *params = md->node->children;
                            params = params->next;  /* Skip sentinel */
                            params = params->next;  /* Skip return type */
                            params = params->next;  /* Skip method name identifier */
                            if (params && params->node && params->node->category == N_MethodParams) {
                                struct node_list *pc = params->node->children;
                                if (pc) pc = pc->next;  /* Skip sentinel */
                                for (; pc; pc = pc->next) {
                                    if (pc->node && pc->node->category == N_ParamDecl) {
                                        struct node_list *pd = pc->node->children;
                                        if (pd) pd = pd->next;  /* Skip sentinel */
                                        if (pd && pd->node && pd->next && pd->next->node) {
                                            JType ptype = extract_param_type(pd->node);
                                            const char *pname = pd->next->node->token;
                                            int pline = pd->next->node->line;
                                            int pcol = pd->next->node->col;
                                            if (is_reserved_id(pname)) {
                                                printf("Line %d, col %d: Symbol %s is reserved\n", pline, pcol, pname);
                                            } else if (find_symbol(temp_symbols, pname)) {
                                                printf("Line %d, col %d: Symbol %s already defined\n", pline, pcol, pname);
                                            } else {
                                                add_symbol(&temp_symbols, pname, ptype, 1, pline, pcol);
                                            }
                                        }
                                    }
                                }
                            }

                            /* Free temp symbols */
                            while (temp_symbols) {
                                Symbol *tmp = temp_symbols;
                                temp_symbols = temp_symbols->next;
                                free(tmp->name); free(tmp);
                            }

                            /* Report method errors (reserved first, then duplicate only if not reserved) */
                            if (reserved_method) {
                                printf("Line %d, col %d: Symbol %s is reserved\n",
                                       method_line, method_col, method_name);
                            } else if (is_duplicate) {
                                printf("Line %d, col %d: Symbol %s(", method_line, method_col, method_name);
                                for (int i = 0; i < n_params; i++) {
                                    if (i > 0) printf(",");
                                    printf("%s", jtype_to_string(param_types[i]));
                                }
                                printf(") already defined\n");
                            }
                        }

                    if (param_types) free(param_types);
                }
            }
        }
    }

    return ct;
}

void print_symbol_tables(ClassTable *ct)
{
    if (!ct || !ct->name) return;

    printf("===== Class %s Symbol Table =====\n", ct->name);
    for (ClassEntryNode *e = ct->entries; e; e = e->next) {
        if (e->kind == CE_FIELD) {
            printf("%s\t\t%s\n", e->field->name, jtype_to_string(e->field->type));
        } else {
            MethodEntry *m = e->method;
            printf("%s\t(", m->name);
            for (int i = 0; i < m->n_params; i++) {
                if (i > 0) printf(",");
                printf("%s", jtype_to_string(m->param_types[i]));
            }
            printf(")\t%s\n", jtype_to_string(m->return_type));
        }
    }

    printf("\n");

    for (MethodEntry *m = ct->methods; m; m = m->next) {
        printf("===== Method %s(", m->name);
        for (int i = 0; i < m->n_params; i++) {
            if (i > 0) printf(",");
            printf("%s", jtype_to_string(m->param_types[i]));
        }
        printf(") Symbol Table =====\n");

        for (Symbol *s = m->symbols; s; s = s->next) {
            printf("%s\t\t%s", s->name, jtype_to_string(s->type));
            if (s->is_param) printf("\tparam");
            printf("\n");
        }

        printf("\n");
    }
}

void free_class_table(ClassTable *ct)
{
    if (!ct) return;

    Symbol *f = ct->fields;
    while (f) {
        Symbol *tmp = f;
        f = f->next;
        free(tmp->name);
        free(tmp);
    }

    MethodEntry *m = ct->methods;
    while (m) {
        MethodEntry *tmp = m;
        m = m->next;

        Symbol *s = tmp->symbols;
        while (s) {
            Symbol *stmp = s;
            s = s->next;
            free(stmp->name);
            free(stmp);
        }

        if (tmp->param_types) free(tmp->param_types);
        free(tmp->name);
        free(tmp);
    }

    ClassEntryNode *en = ct->entries;
    while (en) {
        ClassEntryNode *tmp = en;
        en = en->next;
        free(tmp);
    }

    if (ct->name) free(ct->name);
    free(ct);
}

static Symbol *lookup_symbol(const char *name, MethodEntry *method, ClassTable *ct)
{
    if (!method || !ct) return NULL;
    if (find_symbol(method->symbols, name)) return find_symbol(method->symbols, name);
    if (find_symbol(ct->fields, name)) return find_symbol(ct->fields, name);
    return NULL;
}

static int types_compatible(JType actual, JType expected)
{
    if (actual == expected) return 1;
    if (actual == JT_INT && expected == JT_DOUBLE) return 1;
    return 0;
}

static JType infer_type(struct node *n, MethodEntry *method, ClassTable *ct);

static JType check_binary_op(struct node *n, MethodEntry *method, ClassTable *ct)
{
    struct node_list *c = n->children;
    if (c) c = c->next;

    struct node *left = c && c->node ? c->node : NULL;
    struct node *right = c && c->next && c->next->node ? c->next->node : NULL;

    JType left_type = left ? infer_type(left, method, ct) : JT_UNDEF;
    JType right_type = right ? infer_type(right, method, ct) : JT_UNDEF;

    char left_str[32], right_str[32];
    strcpy(left_str, jtype_to_string(left_type));
    strcpy(right_str, jtype_to_string(right_type));

    switch (n->category) {
        case N_Add: case N_Sub: case N_Mul: case N_Div: case N_Mod:
            if ((left_type == JT_INT || left_type == JT_DOUBLE) &&
                (right_type == JT_INT || right_type == JT_DOUBLE)) {
                return (left_type == JT_DOUBLE || right_type == JT_DOUBLE) ? JT_DOUBLE : JT_INT;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_Add ? "+" :
                            n->category == N_Sub ? "-" :
                            n->category == N_Mul ? "*" :
                            n->category == N_Div ? "/" : "%", left_str, right_str);
            return JT_UNDEF;

        case N_Lt: case N_Gt: case N_Le: case N_Ge:
            if ((left_type == JT_INT || left_type == JT_DOUBLE) &&
                (right_type == JT_INT || right_type == JT_DOUBLE)) {
                return JT_BOOLEAN;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_Lt ? "<" :
                            n->category == N_Gt ? ">" :
                            n->category == N_Le ? "<=" : ">=", left_str, right_str);
            return JT_BOOLEAN;

        case N_Eq: case N_Ne: {
            /* == and != only work on primitive types (int, double, boolean), not arrays */
            int valid = 0;
            if (left_type != JT_UNDEF && right_type != JT_UNDEF &&
                left_type != JT_VOID && right_type != JT_VOID &&
                left_type != JT_STRING_ARRAY && right_type != JT_STRING_ARRAY) {
                /* Check if types are compatible (same or widening) */
                if (left_type == right_type) {
                    valid = 1;
                } else if ((left_type == JT_INT && right_type == JT_DOUBLE) ||
                           (left_type == JT_DOUBLE && right_type == JT_INT)) {
                    valid = 1;
                }
            }
            if (!valid) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                       n->line, n->col, n->category == N_Eq ? "==" : "!=", left_str, right_str);
            }
            return JT_BOOLEAN;
        }

        case N_And: case N_Or:
            if (left_type == JT_BOOLEAN && right_type == JT_BOOLEAN) {
                return JT_BOOLEAN;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_And ? "&&" : "||", left_str, right_str);
            return JT_BOOLEAN;

        case N_Lshift: case N_Rshift:
            if (left_type == JT_INT && right_type == JT_INT) {
                return JT_INT;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_Lshift ? "<<" : ">>", left_str, right_str);
            return JT_UNDEF;

        case N_Xor:
            /* XOR works on both int and boolean types */
            if (left_type == JT_INT && right_type == JT_INT) {
                return JT_INT;
            }
            if (left_type == JT_BOOLEAN && right_type == JT_BOOLEAN) {
                return JT_BOOLEAN;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, "^", left_str, right_str);
            return JT_UNDEF;

        default:
            return JT_UNDEF;
    }
}

static JType check_unary_op(struct node *n, MethodEntry *method, ClassTable *ct)
{
    struct node_list *c = n->children;
    if (c) c = c->next;
    struct node *operand = c && c->node ? c->node : NULL;

    JType op_type = operand ? infer_type(operand, method, ct) : JT_UNDEF;
    char op_str[32];
    strcpy(op_str, jtype_to_string(op_type));

    switch (n->category) {
        case N_Not:
            if (op_type == JT_BOOLEAN) return JT_BOOLEAN;
            printf("Line %d, col %d: Operator ! cannot be applied to type %s\n",
                   n->line, n->col, op_str);
            return JT_BOOLEAN;

        case N_Minus: case N_Plus:
            if (op_type == JT_INT || op_type == JT_DOUBLE) return op_type;
            printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",
                   n->line, n->col, n->category == N_Minus ? "-" : "+", op_str);
            return JT_UNDEF;

        case N_Length:
            if (op_type == JT_STRING_ARRAY) return JT_INT;
            printf("Line %d, col %d: Operator .length cannot be applied to type %s\n",
                   n->line, n->col, op_str);
            return JT_INT;

        default:
            return JT_UNDEF;
    }
}

static JType infer_type(struct node *n, MethodEntry *method, ClassTable *ct)
{
    if (!n) return JT_UNDEF;

    switch (n->category) {
        case N_Natural: {
            /* Check bounds for natural number */
            if (n->token && !check_natural_bounds(n->token)) {
                printf("Line %d, col %d: Number %s out of bounds\n", n->line, n->col, n->token);
            }
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "int");
            return JT_INT;
        }

        case N_Decimal: {
            /* Check bounds for decimal number */
            if (n->token && !check_decimal_bounds(n->token)) {
                printf("Line %d, col %d: Number %s out of bounds\n", n->line, n->col, n->token);
            }
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "double");
            return JT_DOUBLE;
        }

        case N_BoolLit:
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "boolean");
            return JT_BOOLEAN;

        case N_StrLit:
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "String");
            return JT_STRING;

        case N_Identifier: {
            if (is_reserved_id(n->token)) {
                printf("Line %d, col %d: Symbol %s is reserved\n", n->line, n->col, n->token);
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                return JT_UNDEF;
            }
            Symbol *s = lookup_symbol(n->token, method, ct);
            if (s) {
                n->type_annot = malloc(16);
                strcpy(n->type_annot, jtype_to_string(s->type));
                return s->type;
            } else {
                printf("Line %d, col %d: Cannot find symbol %s\n", n->line, n->col, n->token);
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                return JT_UNDEF;
            }
        }

        case N_Add: case N_Sub: case N_Mul: case N_Div: case N_Mod:
        case N_Lt: case N_Gt: case N_Le: case N_Ge:
        case N_Eq: case N_Ne: case N_And: case N_Or:
        case N_Lshift: case N_Rshift: case N_Xor: {
            JType result = check_binary_op(n, method, ct);
            n->type_annot = malloc(16);
            strcpy(n->type_annot, jtype_to_string(result));
            return result;
        }

        case N_Not: case N_Minus: case N_Plus: case N_Length: {
            JType result = check_unary_op(n, method, ct);
            n->type_annot = malloc(16);
            strcpy(n->type_annot, jtype_to_string(result));
            return result;
        }

        case N_Call: {
            struct node_list *c = n->children;
            if (c) c = c->next;  /* Skip sentinel */

            struct node *method_id = c && c->node ? c->node : NULL;
            char *method_name = method_id ? method_id->token : NULL;

            /* Check for reserved method name */
            if (method_name && is_reserved_id(method_name)) {
                printf("Line %d, col %d: Symbol %s is reserved\n",
                       n->line, n->col, method_name);
                if (method_id) {
                    method_id->type_annot = malloc(8);
                    strcpy(method_id->type_annot, "undef");
                }
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                /* Still need to infer types of arguments for annotation */
                c = c && c->next ? c->next : NULL;
                if (c && !c->node) c = c->next;
                for (struct node_list *args = c; args; args = args->next) {
                    if (args->node) infer_type(args->node, method, ct);
                }
                return JT_UNDEF;
            }

            c = c && c->next ? c->next : NULL;
            if (c && !c->node) c = c->next;  /* Skip NULL from call_args sentinel if present */

            int n_args = 0;
            JType *arg_types = NULL;

            /* Count arguments (skip NULL nodes from call_args sentinel) */
            for (struct node_list *tmp = c; tmp; tmp = tmp->next) {
                if (tmp->node) n_args++;
            }

            if (n_args > 0) {
                arg_types = malloc(n_args * sizeof(JType));
                int idx = 0;
                for (struct node_list *args = c; args; args = args->next) {
                    if (args->node) arg_types[idx++] = infer_type(args->node, method, ct);
                }
            }

            /* First try exact match (all types must be exactly equal) across ALL methods */
            MethodEntry *called = NULL;
            if (n_args >= 0) {
                called = find_exact_method(ct->methods, method_name, n_args, arg_types);
            }

            /* If no exact match, find all compatible methods */
            int ambiguous = 0;
            if (!called) {
                int match_count = 0;
                called = find_compatible_method(ct->methods, method_name, n_args, arg_types, &match_count);

                /* Check for ambiguity */
                if (match_count > 1) {
                    printf("Line %d, col %d: Reference to method %s(", n->line, n->col, method_name ? method_name : "");
                    for (int i = 0; i < n_args; i++) {
                        if (i > 0) printf(",");
                        printf("%s", jtype_to_string(arg_types[i]));
                    }
                    printf(") is ambiguous\n");
                    ambiguous = 1;
                    called = NULL;
                } else if (match_count == 0) {
                    called = NULL;
                }
            }

            if (called) {
                if (method_id) {
                    /* Format method signature for identifier annotation (dynamic allocation) */
                    int sig_len = 2; /* "(" + ")" */
                    for (int i = 0; i < called->n_params; i++) {
                        if (i > 0) sig_len++; /* comma */
                        sig_len += (int)strlen(jtype_to_string(called->param_types[i]));
                    }
                    char *sig = malloc(sig_len + 1);
                    sig[0] = '(';
                    sig[1] = '\0';
                    for (int i = 0; i < called->n_params; i++) {
                        if (i > 0) strcat(sig, ",");
                        strcat(sig, jtype_to_string(called->param_types[i]));
                    }
                    strcat(sig, ")");
                    method_id->type_annot = sig;
                }
                n->type_annot = malloc(16);
                strcpy(n->type_annot, jtype_to_string(called->return_type));
                if (arg_types) free(arg_types);
                return called->return_type;
            } else if (!ambiguous) {
                printf("Line %d, col %d: Cannot find symbol %s(", n->line, n->col, method_name ? method_name : "");
                for (int i = 0; i < n_args; i++) {
                    if (i > 0) printf(",");
                    printf("%s", jtype_to_string(arg_types[i]));
                }
                printf(")\n");
                if (method_id) {
                    method_id->type_annot = malloc(8);
                    strcpy(method_id->type_annot, "undef");
                }
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                if (arg_types) free(arg_types);
                return JT_UNDEF;
            } else {
                /* Ambiguous call - set undef type but don't print error (already printed) */
                if (method_id) {
                    method_id->type_annot = malloc(8);
                    strcpy(method_id->type_annot, "undef");
                }
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                if (arg_types) free(arg_types);
                return JT_UNDEF;
            }
        }

        case N_ParseArgs: {
            struct node_list *c = n->children;
            if (c) c = c->next;

            JType arg1_type = JT_UNDEF;
            JType arg2_type = JT_UNDEF;

            if (c && c->node) {
                arg1_type = infer_type(c->node, method, ct);
            }
            c = c && c->next ? c->next : NULL;
            if (c && c->node) {
                arg2_type = infer_type(c->node, method, ct);
            }

            /* Check argument types: first should be String[], second should be int */
            if (arg1_type != JT_STRING_ARRAY || arg2_type != JT_INT) {
                printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n",
                       n->line, n->col, jtype_to_string(arg1_type), jtype_to_string(arg2_type));
            }

            n->type_annot = malloc(8);
            strcpy(n->type_annot, "int");
            return JT_INT;
        }

        case N_Assign: {
            struct node_list *c = n->children;
            if (c) c = c->next;
            struct node *lhs = c && c->node ? c->node : NULL;
            struct node *rhs = c && c->next && c->next->node ? c->next->node : NULL;

            JType lhs_type = lhs ? infer_type(lhs, method, ct) : JT_UNDEF;
            JType rhs_type = rhs ? infer_type(rhs, method, ct) : JT_UNDEF;

            /* Check if lhs is a String[] parameter (arrays cannot be reassigned) */
            int is_param_assign = 0;
            if (lhs && lhs->category == N_Identifier) {
                Symbol *lhs_sym = lookup_symbol(lhs->token, method, ct);
                if (lhs_sym && lhs_sym->is_param && lhs_sym->type == JT_STRING_ARRAY) {
                    is_param_assign = 1;
                    printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n",
                           n->line, n->col, jtype_to_string(lhs_type), jtype_to_string(rhs_type));
                }
            }

            /* Report error if either operand is undef or types are incompatible */
            if (!is_param_assign && (lhs_type == JT_UNDEF || rhs_type == JT_UNDEF || !types_compatible(rhs_type, lhs_type))) {
                printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n",
                       n->line, n->col, jtype_to_string(lhs_type), jtype_to_string(rhs_type));
            }

            n->type_annot = malloc(16);
            strcpy(n->type_annot, jtype_to_string(lhs_type));
            return lhs_type;
        }

        default:
            return JT_UNDEF;
    }
}

static void check_statement(struct node *n, MethodEntry *method, ClassTable *ct)
{
    if (!n) return;

    struct node_list *c;
    switch (n->category) {
        case N_VarDecl: {
            /* Add variable to method's symbol table when encountered */
            c = n->children;
            if (c) c = c->next;  /* Skip sentinel */

            /* Get the type from first child */
            JType vtype = JT_UNDEF;
            if (c && c->node) {
                vtype = node_to_jtype(c->node);
                c = c->next;  /* Move to first identifier */
            }

            /* Process all identifiers in this declaration (handles int a, b, c; ) */
            for (; c && c->node; c = c->next) {
                const char *vname = c->node->token;
                int vline = c->node->line;
                int vcol = c->node->col;

                if (is_reserved_id(vname)) {
                    printf("Line %d, col %d: Symbol %s is reserved\n",
                           vline, vcol, vname);
                } else if (find_symbol(method->symbols, vname)) {
                    printf("Line %d, col %d: Symbol %s already defined\n",
                           vline, vcol, vname);
                } else {
                    add_symbol(&method->symbols, vname, vtype, 0, vline, vcol);
                }
            }
            break;
        }

        case N_If: {
            c = n->children;
            if (c) c = c->next;

            /* Track condition type error to report after body processing */
            JType cond_type = JT_BOOLEAN;
            int cond_line = 0, cond_col = 0;
            int has_type_error = 0;

            if (c && c->node) {
                cond_type = infer_type(c->node, method, ct);
                if (cond_type != JT_BOOLEAN) {
                    cond_line = c->node->line;
                    cond_col = c->node->col;
                    has_type_error = 1;
                }
            }

            /* Process then block */
            c = c && c->next ? c->next : NULL;
            if (c && c->node) check_statement(c->node, method, ct);

            /* Process else block */
            c = c && c->next ? c->next : NULL;
            if (c && c->node) check_statement(c->node, method, ct);

            /* Report type error after processing blocks */
            if (has_type_error) {
                printf("Line %d, col %d: Incompatible type %s in if statement\n",
                       cond_line, cond_col, jtype_to_string(cond_type));
            }
            break;
        }

        case N_While: {
            c = n->children;
            if (c) c = c->next;

            /* Track condition type error to report after body processing */
            JType cond_type = JT_BOOLEAN;
            int cond_line = 0, cond_col = 0;
            int has_type_error = 0;

            if (c && c->node) {
                cond_type = infer_type(c->node, method, ct);
                if (cond_type != JT_BOOLEAN) {
                    cond_line = c->node->line;
                    cond_col = c->node->col;
                    has_type_error = 1;
                }
            }

            /* Process body */
            c = c && c->next ? c->next : NULL;
            if (c && c->node) check_statement(c->node, method, ct);

            /* Report type error after processing body */
            if (has_type_error) {
                printf("Line %d, col %d: Incompatible type %s in while statement\n",
                       cond_line, cond_col, jtype_to_string(cond_type));
            }
            break;
        }

        case N_Return:
            c = n->children;
            if (c) c = c->next;
            if (c && c->node) {
                JType ret_type = infer_type(c->node, method, ct);
                if (ret_type == JT_VOID) {
                    /* Returning a void expression is always an error */
                    printf("Line %d, col %d: Incompatible type void in return statement\n",
                           c->node->line, c->node->col);
                } else if (method && !types_compatible(ret_type, method->return_type)) {
                    printf("Line %d, col %d: Incompatible type %s in return statement\n",
                           c->node->line, c->node->col, jtype_to_string(ret_type));
                }
            } else {
                /* No return value (void return) */
                if (method && method->return_type != JT_VOID) {
                    printf("Line %d, col %d: Incompatible type void in return statement\n",
                           n->line, n->col);
                }
            }
            break;

        case N_Print:
            c = n->children;
            if (c) c = c->next;
            if (c && c->node) {
                JType arg_type = infer_type(c->node, method, ct);
                if (arg_type == JT_UNDEF || arg_type == JT_VOID || arg_type == JT_STRING_ARRAY) {
                    printf("Line %d, col %d: Incompatible type %s in System.out.print statement\n",
                           c->node->line, c->node->col, jtype_to_string(arg_type));
                }
            }
            break;

        case N_Assign:
        case N_Call:
        case N_ParseArgs:
            infer_type(n, method, ct);
            break;

        case N_Block:
            c = n->children;
            if (c) c = c->next;
            for (; c && c->node; c = c->next) {
                check_statement(c->node, method, ct);
            }
            break;

        default:
            break;
    }
}

void check_and_annotate_ast(struct node *program, ClassTable *ct)
{
    if (!program || !ct) return;

    struct node_list *c = program->children;
    if (c) c = c->next;
    c = c && c->next ? c->next : NULL;

    /* Track which MethodEntry instances have been processed to skip duplicates */
    MethodEntry *processed[256];
    int n_processed = 0;

    for (; c && c->node; c = c->next) {
        if (c->node->category == N_MethodDecl) {
            struct node_list *md = c->node->children;
            if (md) md = md->next;
            if (md && md->node && md->node->category == N_MethodHeader) {
                JType ret_type;
                char *method_name;
                JType *param_types;
                int n_params;

                process_method_header(md->node, &ret_type, &method_name, &param_types, &n_params);
                MethodEntry *method = find_exact_method(ct->methods, method_name, n_params, param_types);

                /* Check if this method was already processed (duplicate) */
                int already_processed = 0;
                if (method) {
                    for (int i = 0; i < n_processed; i++) {
                        if (processed[i] == method) { already_processed = 1; break; }
                    }
                }

                if (method && !already_processed && md->next && md->next->node &&
                    md->next->node->category == N_MethodBody) {
                    if (n_processed < 256) processed[n_processed++] = method;
                    struct node_list *body = md->next->node->children;
                    if (body) body = body->next;  /* Skip sentinel */
                    for (; body && body->node; body = body->next) {
                        check_statement(body->node, method, ct);
                    }
                }

                if (param_types) free(param_types);
            }
        }
    }
}

```

## Atualização automática (Meta 3 e Meta 4)

Secção gerada automaticamente a partir dos ficheiros atuais em `Meta3/src/` e `Meta4/src/`.

### Meta 3 (estado atual)

#### Ficheiro: `Meta3/src/ast.h`

```c
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
    int line, col;
    char *type_annot;  /* type annotation string (e.g., "int", "(int,double)", NULL) */
    struct node_list *children;
};

struct node_list {
    struct node *node;
    struct node_list *next;
};

struct node *newnode(enum category category, char *token, int line, int col);
void addchild(struct node *parent, struct node *child);

#endif

```

#### Ficheiro: `Meta3/src/ast.c`

```c
/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

/* Definido em jucompiler.y */
extern const char *category_name[];

struct node *newnode(enum category category, char *token, int line, int col)
{
    struct node *n = malloc(sizeof(struct node));
    n->category = category;
    n->token = token;
    n->line = line;
    n->col = col;
    n->type_annot = NULL;
    n->children = malloc(sizeof(struct node_list));
    n->children->node = NULL;
    n->children->next = NULL;
    return n;
}

void addchild(struct node *parent, struct node *child)
{
    struct node_list *c;
    struct node_list *nl = malloc(sizeof(struct node_list));
    nl->node = child;
    nl->next = NULL;
    c = parent->children;
    while (c->next) c = c->next;
    c->next = nl;
}

static void printast_rec(struct node *n, int depth)
{
    int i;
    struct node_list *c;
    if (!n) return;

    for (i = 0; i < depth; i++) printf("..");

    printf("%s", category_name[n->category]);
    if (n->token) printf("(%s)", n->token);
    if (n->type_annot) printf(" - %s", n->type_annot);
    printf("\n");

    for (c = n->children; c; c = c->next)
        printast_rec(c->node, depth + 1);
}

void printast(struct node *root)
{
    printast_rec(root, 0);
}

```

#### Ficheiro: `Meta3/src/semantic.h`

```c
/*
 * Semantic Analyzer Header - Meta 3
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "ast.h"

typedef enum {
    JT_INT, JT_DOUBLE, JT_BOOLEAN, JT_VOID, JT_STRING_ARRAY, JT_STRING, JT_UNDEF
} JType;

typedef struct Symbol {
    char *name;
    JType type;
    int is_param;
    int line, col;
    struct Symbol *next;
} Symbol;

typedef struct MethodEntry {
    char *name;
    JType return_type;
    JType *param_types;
    int n_params;
    Symbol *symbols;
    int line, col;
    struct MethodEntry *next;
} MethodEntry;

/* Ordered list node for class-level entries (fields and methods mixed) */
typedef enum { CE_FIELD, CE_METHOD } ClassEntryKind;
typedef struct ClassEntryNode {
    ClassEntryKind kind;
    union {
        Symbol *field;
        MethodEntry *method;
    };
    struct ClassEntryNode *next;
} ClassEntryNode;

typedef struct {
    char *name;
    Symbol *fields;
    MethodEntry *methods;
    ClassEntryNode *entries;  /* Fields and methods in declaration order */
} ClassTable;

ClassTable *build_symbol_tables(struct node *program);
void print_symbol_tables(ClassTable *ct);
void check_and_annotate_ast(struct node *program, ClassTable *ct);
void free_class_table(ClassTable *ct);

const char *jtype_to_string(JType t);
JType node_to_jtype(struct node *type_node);

#endif

```

#### Ficheiro: `Meta3/src/semantic.c`

```c
/*
 * Semantic Analyzer Implementation - Meta 3
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "semantic.h"

extern const char *category_name[];

static int check_natural_bounds(const char *text) {
    /* Check if integer literal is within int range (0 to 2147483647) */
    /* Remove underscores for comparison */
    char clean[32];
    int j = 0;
    for (int i = 0; text[i] && j < 20; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
        }
    }
    clean[j] = '\0';

    /* Check if it's larger than INT_MAX (2147483647) */
    if (j > 10) return 0; /* More than 10 digits, definitely out of bounds */
    if (j == 10 && strcmp(clean, "2147483647") > 0) return 0;
    return 1;
}

static int check_decimal_bounds(const char *text) {
    /* Check if decimal literal is within valid double range */
    /* First, remove underscores (Java allows them, strtod doesn't) */
    char clean[1000];
    int j = 0;
    int has_digits = 0;
    for (int i = 0; text[i] && j < (int)sizeof(clean) - 1; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
            if (text[i] >= '0' && text[i] <= '9') has_digits = 1;
        }
    }
    clean[j] = '\0';

    char *endptr;
    double val = strtod(clean, &endptr);

    /* strtod returns HUGE_VAL on overflow */
    if (val == HUGE_VAL || val == -HUGE_VAL) return 0;

    /* Check for underflow: if we have significant digits but got 0, it underflowed */
    if (val == 0.0 && has_digits) {
        /* Need to check if original string was non-zero */
        /* If string contains non-zero digits and converted to 0, it's underflow */
        for (int i = 0; clean[i]; i++) {
            if (clean[i] >= '1' && clean[i] <= '9') return 0;  /* Non-zero digit found, underflowed */
        }
    }

    return 1;
}

const char *jtype_to_string(JType t)
{
    switch (t) {
        case JT_INT: return "int";
        case JT_DOUBLE: return "double";
        case JT_BOOLEAN: return "boolean";
        case JT_VOID: return "void";
        case JT_STRING_ARRAY: return "String[]";
        case JT_STRING: return "String";
        case JT_UNDEF: return "undef";
    }
    return "unknown";
}

JType node_to_jtype(struct node *type_node)
{
    if (!type_node) return JT_UNDEF;

    switch (type_node->category) {
        case N_Int: return JT_INT;
        case N_Double: return JT_DOUBLE;
        case N_Bool: return JT_BOOLEAN;
        case N_Void: return JT_VOID;
        case N_StringArray: return JT_STRING_ARRAY;
        default: return JT_UNDEF;
    }
}

static int is_reserved_id(const char *name)
{
    return (strcmp(name, "_") == 0);
}

static Symbol *find_symbol(Symbol *symbols, const char *name)
{
    for (Symbol *s = symbols; s; s = s->next) {
        if (strcmp(s->name, name) == 0) return s;
    }
    return NULL;
}

static void add_symbol(Symbol **symbols, const char *name, JType type, int is_param, int line, int col)
{
    Symbol *s = malloc(sizeof(Symbol));
    s->name = malloc(strlen(name) + 1);
    strcpy(s->name, name);
    s->type = type;
    s->is_param = is_param;
    s->line = line;
    s->col = col;
    s->next = NULL;

    if (!*symbols) {
        *symbols = s;
    } else {
        Symbol *tail = *symbols;
        while (tail->next) tail = tail->next;
        tail->next = s;
    }
}

static int types_compatible(JType actual, JType expected); /* Forward declaration */

static MethodEntry *find_method_by_signature(MethodEntry *methods, const char *name, int n_params)
{
    for (MethodEntry *m = methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n_params) return m;
    }
    return NULL;
}

static MethodEntry *find_exact_method(MethodEntry *methods, const char *name, int n_params,
                                       JType *arg_types)
{
    /* Find a method with exact type match (all types must be equal) */
    for (MethodEntry *m = methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n_params) {
            int exact = 1;
            for (int i = 0; i < n_params; i++) {
                if (arg_types[i] != m->param_types[i]) {
                    exact = 0;
                    break;
                }
            }
            if (exact) return m;
        }
    }
    return NULL;
}

static MethodEntry *find_compatible_method(MethodEntry *methods, const char *name, int n_params,
                                           JType *arg_types, int *out_count)
{
    /* Find all methods with matching name and parameter count whose types are compatible */
    MethodEntry *first_match = NULL;
    int match_count = 0;

    for (MethodEntry *m = methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n_params) {
            /* Check if all parameter types are compatible */
            int compatible = 1;
            for (int i = 0; i < n_params; i++) {
                if (!types_compatible(arg_types[i], m->param_types[i])) {
                    compatible = 0;
                    break;
                }
            }
            if (compatible) {
                match_count++;
                if (!first_match) first_match = m;
            }
        }
    }

    *out_count = match_count;
    return first_match;
}

static void add_class_entry(ClassTable *ct, ClassEntryKind kind, Symbol *field, MethodEntry *method)
{
    ClassEntryNode *en = malloc(sizeof(ClassEntryNode));
    en->kind = kind;
    if (kind == CE_FIELD) en->field = field;
    else en->method = method;
    en->next = NULL;
    if (!ct->entries) {
        ct->entries = en;
    } else {
        ClassEntryNode *tail = ct->entries;
        while (tail->next) tail = tail->next;
        tail->next = en;
    }
}

static MethodEntry *add_method(MethodEntry **methods, const char *name, JType return_type,
                                JType *param_types, int n_params, int line, int col)
{
    MethodEntry *m = malloc(sizeof(MethodEntry));
    m->name = malloc(strlen(name) + 1);
    strcpy(m->name, name);
    m->return_type = return_type;
    m->n_params = n_params;
    m->param_types = NULL;
    if (n_params > 0) {
        m->param_types = malloc(n_params * sizeof(JType));
        memcpy(m->param_types, param_types, n_params * sizeof(JType));
    }
    m->symbols = NULL;
    m->line = line;
    m->col = col;
    m->next = NULL;

    if (!*methods) {
        *methods = m;
    } else {
        MethodEntry *tail = *methods;
        while (tail->next) tail = tail->next;
        tail->next = m;
    }

    return m;
}

static JType extract_param_type(struct node *param_node)
{
    if (!param_node) return JT_UNDEF;
    if (param_node->category == N_StringArray) return JT_STRING_ARRAY;
    return node_to_jtype(param_node);
}

static void process_method_header(struct node *header, JType *ret_type,
                                   char **method_name, JType **param_types, int *n_params)
{
    *ret_type = JT_UNDEF;
    *method_name = NULL;
    *param_types = NULL;
    *n_params = 0;

    if (!header || header->category != N_MethodHeader) return;

    struct node_list *c = header->children;
    if (c) c = c->next;  /* Skip sentinel */
    if (!c || !c->node) return;

    /* First child: return type */
    *ret_type = node_to_jtype(c->node);
    if (c->node->category == N_Void) *ret_type = JT_VOID;

    /* Second child: identifier (method name) */
    c = c->next;
    if (c && c->node && c->node->category == N_Identifier) {
        *method_name = c->node->token;
    }

    /* Third child: MethodParams */
    c = c->next;
    if (c && c->node && c->node->category == N_MethodParams) {
        struct node_list *pc = c->node->children;
        if (pc) pc = pc->next;  /* Skip sentinel */
        int count = 0;
        for (struct node_list *tmp = pc; tmp && tmp->node; tmp = tmp->next) {
            if (tmp->node->category == N_ParamDecl) count++;
        }

        if (count > 0) {
            *param_types = malloc(count * sizeof(JType));
            int idx = 0;
            for (pc = c->node->children; pc; pc = pc->next) {
                if (pc->node && pc->node->category == N_ParamDecl) {
                    struct node_list *pd = pc->node->children;
                    if (pd) pd = pd->next;  /* Skip sentinel */
                    if (pd && pd->node) {
                        (*param_types)[idx++] = extract_param_type(pd->node);
                    }
                }
            }
        }
        *n_params = count;
    }
}

ClassTable *build_symbol_tables(struct node *program)
{
    if (!program || program->category != N_Program) return NULL;

    ClassTable *ct = malloc(sizeof(ClassTable));
    ct->name = NULL;
    ct->fields = NULL;
    ct->methods = NULL;
    ct->entries = NULL;

    /* First child is class Identifier (skip sentinel) */
    struct node_list *c = program->children;
    if (c) c = c->next;  /* Skip sentinel */
    if (c && c->node && c->node->category == N_Identifier) {
        if (c->node->token) {
            ct->name = malloc(strlen(c->node->token) + 1);
            strcpy(ct->name, c->node->token);
        }
    }

    /* Process FieldDecl and MethodDecl */
    for (c = c->next; c; c = c->next) {
        if (!c->node) {
            continue;
        }

        if (c->node->category == N_FieldDecl) {
            struct node_list *fd = c->node->children;
            if (fd) fd = fd->next;  /* Skip sentinel */

            /* Get type from first child */
            JType type = JT_UNDEF;
            if (fd && fd->node) {
                type = node_to_jtype(fd->node);
                fd = fd->next;  /* Move to first identifier */
            }

            /* Process all identifiers in this field declaration (handles int a, b, c;) */
            for (; fd && fd->node; fd = fd->next) {
                const char *id = fd->node->token;
                int line = fd->node->line;
                int col = fd->node->col;

                if (is_reserved_id(id)) {
                    printf("Line %d, col %d: Symbol %s is reserved\n", line, col, id);
                } else if (find_symbol(ct->fields, id)) {
                    printf("Line %d, col %d: Symbol %s already defined\n", line, col, id);
                } else {
                    add_symbol(&ct->fields, id, type, 0, line, col);
                    add_class_entry(ct, CE_FIELD, find_symbol(ct->fields, id), NULL);
                }
            }
        }
        else if (c->node->category == N_MethodDecl) {
            struct node_list *md = c->node->children;
            if (md) md = md->next;  /* Skip sentinel */
            if (md && md->node && md->node->category == N_MethodHeader) {
                JType ret_type;
                char *method_name;
                JType *param_types;
                int n_params;

                process_method_header(md->node, &ret_type, &method_name, &param_types, &n_params);

                if (method_name) {
                    int method_line = md->node->line;
                    int method_col = md->node->col;
                    struct node_list *hdr_c = md->node->children;
                    if (hdr_c) hdr_c = hdr_c->next;  /* Skip sentinel */
                    if (hdr_c) hdr_c = hdr_c->next;  /* Skip return type */
                    if (hdr_c && hdr_c->node && hdr_c->node->category == N_Identifier) {
                        method_line = hdr_c->node->line;
                        method_col = hdr_c->node->col;
                    }

                    /* Check for reserved method name (but don't report yet) */
                    int reserved_method = is_reserved_id(method_name);

                    /* Check for exact duplicate: same name AND same parameter types */
                    MethodEntry *existing = find_method_by_signature(ct->methods, method_name, n_params);
                    int is_duplicate = 0;
                    if (existing && n_params == existing->n_params) {
                        int types_match = 1;
                        for (int i = 0; i < n_params; i++) {
                            if (param_types[i] != existing->param_types[i]) {
                                types_match = 0;
                                break;
                            }
                        }
                        is_duplicate = types_match;
                    }

                    MethodEntry *method = NULL;
                    if (!reserved_method && !is_duplicate) {
                        method = add_method(&ct->methods, method_name, ret_type, param_types, n_params,
                                            method_line, method_col);
                        if (method) add_class_entry(ct, CE_METHOD, NULL, method);
                    }

                    /* Process parameters to check for errors FIRST (before reporting method errors) */
                    if (method) {
                            add_symbol(&method->symbols, "return", ret_type, 0, method_line, method_col);

                            struct node_list *params = md->node->children;
                            params = params->next;  /* Skip sentinel */
                            params = params->next;  /* Skip return type */
                            params = params->next;  /* Skip method name identifier */
                            if (params && params->node && params->node->category == N_MethodParams) {
                                struct node_list *pc = params->node->children;
                                if (pc) pc = pc->next;  /* Skip sentinel */
                                for (; pc; pc = pc->next) {
                                    if (pc->node && pc->node->category == N_ParamDecl) {
                                        struct node_list *pd = pc->node->children;
                                        if (pd) pd = pd->next;  /* Skip sentinel */
                                        if (pd && pd->node && pd->next && pd->next->node) {
                                            JType ptype = extract_param_type(pd->node);
                                            const char *pname = pd->next->node->token;
                                            int pline = pd->next->node->line;
                                            int pcol = pd->next->node->col;

                                            if (is_reserved_id(pname)) {
                                                printf("Line %d, col %d: Symbol %s is reserved\n",
                                                       pline, pcol, pname);
                                            } else if (find_symbol(method->symbols, pname)) {
                                                printf("Line %d, col %d: Symbol %s already defined\n",
                                                       pline, pcol, pname);
                                            } else {
                                                add_symbol(&method->symbols, pname, ptype, 1,
                                                           pline, pcol);
                                            }
                                        }
                                    }
                                }
                            }
                        } else if (reserved_method || is_duplicate) {
                            /* Process params with temp list to detect duplicate param names even if method is reserved/duplicate */
                            Symbol *temp_symbols = NULL;
                            add_symbol(&temp_symbols, "return", ret_type, 0, method_line, method_col);

                            struct node_list *params = md->node->children;
                            params = params->next;  /* Skip sentinel */
                            params = params->next;  /* Skip return type */
                            params = params->next;  /* Skip method name identifier */
                            if (params && params->node && params->node->category == N_MethodParams) {
                                struct node_list *pc = params->node->children;
                                if (pc) pc = pc->next;  /* Skip sentinel */
                                for (; pc; pc = pc->next) {
                                    if (pc->node && pc->node->category == N_ParamDecl) {
                                        struct node_list *pd = pc->node->children;
                                        if (pd) pd = pd->next;  /* Skip sentinel */
                                        if (pd && pd->node && pd->next && pd->next->node) {
                                            JType ptype = extract_param_type(pd->node);
                                            const char *pname = pd->next->node->token;
                                            int pline = pd->next->node->line;
                                            int pcol = pd->next->node->col;
                                            if (is_reserved_id(pname)) {
                                                printf("Line %d, col %d: Symbol %s is reserved\n", pline, pcol, pname);
                                            } else if (find_symbol(temp_symbols, pname)) {
                                                printf("Line %d, col %d: Symbol %s already defined\n", pline, pcol, pname);
                                            } else {
                                                add_symbol(&temp_symbols, pname, ptype, 1, pline, pcol);
                                            }
                                        }
                                    }
                                }
                            }

                            /* Free temp symbols */
                            while (temp_symbols) {
                                Symbol *tmp = temp_symbols;
                                temp_symbols = temp_symbols->next;
                                free(tmp->name); free(tmp);
                            }

                            /* Report method errors (reserved first, then duplicate only if not reserved) */
                            if (reserved_method) {
                                printf("Line %d, col %d: Symbol %s is reserved\n",
                                       method_line, method_col, method_name);
                            } else if (is_duplicate) {
                                /* Specification: duplicate method message must show only the identifier */
                                printf("Line %d, col %d: Symbol %s already defined\n",
                                       method_line, method_col, method_name);
                            }
                        }

                    if (param_types) free(param_types);
                }
            }
        }
    }

    return ct;
}

void print_symbol_tables(ClassTable *ct)
{
    if (!ct || !ct->name) return;

    printf("===== Class %s Symbol Table =====\n", ct->name);
    for (ClassEntryNode *e = ct->entries; e; e = e->next) {
        if (e->kind == CE_FIELD) {
            printf("%s\t\t%s\n", e->field->name, jtype_to_string(e->field->type));
        } else {
            MethodEntry *m = e->method;
            printf("%s\t(", m->name);
            for (int i = 0; i < m->n_params; i++) {
                if (i > 0) printf(",");
                printf("%s", jtype_to_string(m->param_types[i]));
            }
            printf(")\t%s\n", jtype_to_string(m->return_type));
        }
    }

    printf("\n");

    for (MethodEntry *m = ct->methods; m; m = m->next) {
        printf("===== Method %s(", m->name);
        for (int i = 0; i < m->n_params; i++) {
            if (i > 0) printf(",");
            printf("%s", jtype_to_string(m->param_types[i]));
        }
        printf(") Symbol Table =====\n");

        for (Symbol *s = m->symbols; s; s = s->next) {
            printf("%s\t\t%s", s->name, jtype_to_string(s->type));
            if (s->is_param) printf("\tparam");
            printf("\n");
        }

        printf("\n");
    }
}

void free_class_table(ClassTable *ct)
{
    if (!ct) return;

    Symbol *f = ct->fields;
    while (f) {
        Symbol *tmp = f;
        f = f->next;
        free(tmp->name);
        free(tmp);
    }

    MethodEntry *m = ct->methods;
    while (m) {
        MethodEntry *tmp = m;
        m = m->next;

        Symbol *s = tmp->symbols;
        while (s) {
            Symbol *stmp = s;
            s = s->next;
            free(stmp->name);
            free(stmp);
        }

        if (tmp->param_types) free(tmp->param_types);
        free(tmp->name);
        free(tmp);
    }

    ClassEntryNode *en = ct->entries;
    while (en) {
        ClassEntryNode *tmp = en;
        en = en->next;
        free(tmp);
    }

    if (ct->name) free(ct->name);
    free(ct);
}

static Symbol *lookup_symbol(const char *name, MethodEntry *method, ClassTable *ct)
{
    if (!method || !ct) return NULL;
    if (find_symbol(method->symbols, name)) return find_symbol(method->symbols, name);
    if (find_symbol(ct->fields, name)) return find_symbol(ct->fields, name);
    return NULL;
}

static int types_compatible(JType actual, JType expected)
{
    if (actual == expected) return 1;
    if (actual == JT_INT && expected == JT_DOUBLE) return 1;
    return 0;
}

static JType infer_type(struct node *n, MethodEntry *method, ClassTable *ct);

static JType check_binary_op(struct node *n, MethodEntry *method, ClassTable *ct)
{
    struct node_list *c = n->children;
    if (c) c = c->next;

    struct node *left = c && c->node ? c->node : NULL;
    struct node *right = c && c->next && c->next->node ? c->next->node : NULL;

    JType left_type = left ? infer_type(left, method, ct) : JT_UNDEF;
    JType right_type = right ? infer_type(right, method, ct) : JT_UNDEF;

    char left_str[32], right_str[32];
    strcpy(left_str, jtype_to_string(left_type));
    strcpy(right_str, jtype_to_string(right_type));

    switch (n->category) {
        case N_Add: case N_Sub: case N_Mul: case N_Div: case N_Mod:
            if ((left_type == JT_INT || left_type == JT_DOUBLE) &&
                (right_type == JT_INT || right_type == JT_DOUBLE)) {
                return (left_type == JT_DOUBLE || right_type == JT_DOUBLE) ? JT_DOUBLE : JT_INT;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_Add ? "+" :
                            n->category == N_Sub ? "-" :
                            n->category == N_Mul ? "*" :
                            n->category == N_Div ? "/" : "%", left_str, right_str);
            return JT_UNDEF;

        case N_Lt: case N_Gt: case N_Le: case N_Ge:
            if ((left_type == JT_INT || left_type == JT_DOUBLE) &&
                (right_type == JT_INT || right_type == JT_DOUBLE)) {
                return JT_BOOLEAN;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_Lt ? "<" :
                            n->category == N_Gt ? ">" :
                            n->category == N_Le ? "<=" : ">=", left_str, right_str);
            return JT_BOOLEAN;

        case N_Eq: case N_Ne: {
            /* == and != only work on primitive types (int, double, boolean), not arrays */
            int valid = 0;
            if (left_type != JT_UNDEF && right_type != JT_UNDEF &&
                left_type != JT_VOID && right_type != JT_VOID &&
                left_type != JT_STRING_ARRAY && right_type != JT_STRING_ARRAY) {
                /* Check if types are compatible (same or widening) */
                if (left_type == right_type) {
                    valid = 1;
                } else if ((left_type == JT_INT && right_type == JT_DOUBLE) ||
                           (left_type == JT_DOUBLE && right_type == JT_INT)) {
                    valid = 1;
                }
            }
            if (!valid) {
                printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                       n->line, n->col, n->category == N_Eq ? "==" : "!=", left_str, right_str);
            }
            return JT_BOOLEAN;
        }

        case N_And: case N_Or:
            if (left_type == JT_BOOLEAN && right_type == JT_BOOLEAN) {
                return JT_BOOLEAN;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_And ? "&&" : "||", left_str, right_str);
            return JT_BOOLEAN;

        case N_Lshift: case N_Rshift:
            if (left_type == JT_INT && right_type == JT_INT) {
                return JT_INT;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_Lshift ? "<<" : ">>", left_str, right_str);
            return JT_UNDEF;

        case N_Xor:
            /* XOR works on both int and boolean types */
            if (left_type == JT_INT && right_type == JT_INT) {
                return JT_INT;
            }
            if (left_type == JT_BOOLEAN && right_type == JT_BOOLEAN) {
                return JT_BOOLEAN;
            }
            printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, "^", left_str, right_str);
            return JT_UNDEF;

        default:
            return JT_UNDEF;
    }
}

static JType check_unary_op(struct node *n, MethodEntry *method, ClassTable *ct)
{
    struct node_list *c = n->children;
    if (c) c = c->next;
    struct node *operand = c && c->node ? c->node : NULL;

    JType op_type = operand ? infer_type(operand, method, ct) : JT_UNDEF;
    char op_str[32];
    strcpy(op_str, jtype_to_string(op_type));

    switch (n->category) {
        case N_Not:
            if (op_type == JT_BOOLEAN) return JT_BOOLEAN;
            printf("Line %d, col %d: Operator ! cannot be applied to type %s\n",
                   n->line, n->col, op_str);
            return JT_BOOLEAN;

        case N_Minus: case N_Plus:
            if (op_type == JT_INT || op_type == JT_DOUBLE) return op_type;
            printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",
                   n->line, n->col, n->category == N_Minus ? "-" : "+", op_str);
            return JT_UNDEF;

        case N_Length:
            if (op_type == JT_STRING_ARRAY) return JT_INT;
            printf("Line %d, col %d: Operator .length cannot be applied to type %s\n",
                   n->line, n->col, op_str);
            return JT_INT;

        default:
            return JT_UNDEF;
    }
}

static JType infer_type(struct node *n, MethodEntry *method, ClassTable *ct)
{
    if (!n) return JT_UNDEF;

    switch (n->category) {
        case N_Natural: {
            /* Check bounds for natural number */
            if (n->token && !check_natural_bounds(n->token)) {
                printf("Line %d, col %d: Number %s out of bounds\n", n->line, n->col, n->token);
            }
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "int");
            return JT_INT;
        }

        case N_Decimal: {
            /* Check bounds for decimal number */
            if (n->token && !check_decimal_bounds(n->token)) {
                printf("Line %d, col %d: Number %s out of bounds\n", n->line, n->col, n->token);
            }
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "double");
            return JT_DOUBLE;
        }

        case N_BoolLit:
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "boolean");
            return JT_BOOLEAN;

        case N_StrLit:
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "String");
            return JT_STRING;

        case N_Identifier: {
            if (is_reserved_id(n->token)) {
                printf("Line %d, col %d: Symbol %s is reserved\n", n->line, n->col, n->token);
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                return JT_UNDEF;
            }
            Symbol *s = lookup_symbol(n->token, method, ct);
            if (s) {
                n->type_annot = malloc(16);
                strcpy(n->type_annot, jtype_to_string(s->type));
                return s->type;
            } else {
                printf("Line %d, col %d: Cannot find symbol %s\n", n->line, n->col, n->token);
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                return JT_UNDEF;
            }
        }

        case N_Add: case N_Sub: case N_Mul: case N_Div: case N_Mod:
        case N_Lt: case N_Gt: case N_Le: case N_Ge:
        case N_Eq: case N_Ne: case N_And: case N_Or:
        case N_Lshift: case N_Rshift: case N_Xor: {
            JType result = check_binary_op(n, method, ct);
            n->type_annot = malloc(16);
            strcpy(n->type_annot, jtype_to_string(result));
            return result;
        }

        case N_Not: case N_Minus: case N_Plus: case N_Length: {
            JType result = check_unary_op(n, method, ct);
            n->type_annot = malloc(16);
            strcpy(n->type_annot, jtype_to_string(result));
            return result;
        }

        case N_Call: {
            struct node_list *c = n->children;
            if (c) c = c->next;  /* Skip sentinel */

            struct node *method_id = c && c->node ? c->node : NULL;
            char *method_name = method_id ? method_id->token : NULL;

            /* Check for reserved method name */
            if (method_name && is_reserved_id(method_name)) {
                printf("Line %d, col %d: Symbol %s is reserved\n",
                       n->line, n->col, method_name);
                if (method_id) {
                    method_id->type_annot = malloc(8);
                    strcpy(method_id->type_annot, "undef");
                }
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                /* Still need to infer types of arguments for annotation */
                c = c && c->next ? c->next : NULL;
                if (c && !c->node) c = c->next;
                for (struct node_list *args = c; args; args = args->next) {
                    if (args->node) infer_type(args->node, method, ct);
                }
                return JT_UNDEF;
            }

            c = c && c->next ? c->next : NULL;
            if (c && !c->node) c = c->next;  /* Skip NULL from call_args sentinel if present */

            int n_args = 0;
            JType *arg_types = NULL;

            /* Count arguments (skip NULL nodes from call_args sentinel) */
            for (struct node_list *tmp = c; tmp; tmp = tmp->next) {
                if (tmp->node) n_args++;
            }

            if (n_args > 0) {
                arg_types = malloc(n_args * sizeof(JType));
                int idx = 0;
                for (struct node_list *args = c; args; args = args->next) {
                    if (args->node) arg_types[idx++] = infer_type(args->node, method, ct);
                }
            }

            /* First try exact match (all types must be exactly equal) across ALL methods */
            MethodEntry *called = NULL;
            if (n_args >= 0) {
                called = find_exact_method(ct->methods, method_name, n_args, arg_types);
            }

            /* If no exact match, find all compatible methods */
            int ambiguous = 0;
            if (!called) {
                int match_count = 0;
                called = find_compatible_method(ct->methods, method_name, n_args, arg_types, &match_count);

                /* Check for ambiguity */
                if (match_count > 1) {
                    printf("Line %d, col %d: Reference to method %s(", n->line, n->col, method_name ? method_name : "");
                    for (int i = 0; i < n_args; i++) {
                        if (i > 0) printf(",");
                        printf("%s", jtype_to_string(arg_types[i]));
                    }
                    printf(") is ambiguous\n");
                    ambiguous = 1;
                    called = NULL;
                } else if (match_count == 0) {
                    called = NULL;
                }
            }

            if (called) {
                if (method_id) {
                    /* Format method signature for identifier annotation (dynamic allocation) */
                    int sig_len = 2; /* "(" + ")" */
                    for (int i = 0; i < called->n_params; i++) {
                        if (i > 0) sig_len++; /* comma */
                        sig_len += (int)strlen(jtype_to_string(called->param_types[i]));
                    }
                    char *sig = malloc(sig_len + 1);
                    sig[0] = '(';
                    sig[1] = '\0';
                    for (int i = 0; i < called->n_params; i++) {
                        if (i > 0) strcat(sig, ",");
                        strcat(sig, jtype_to_string(called->param_types[i]));
                    }
                    strcat(sig, ")");
                    method_id->type_annot = sig;
                }
                n->type_annot = malloc(16);
                strcpy(n->type_annot, jtype_to_string(called->return_type));
                if (arg_types) free(arg_types);
                return called->return_type;
            } else if (!ambiguous) {
                printf("Line %d, col %d: Cannot find symbol %s(", n->line, n->col, method_name ? method_name : "");
                for (int i = 0; i < n_args; i++) {
                    if (i > 0) printf(",");
                    printf("%s", jtype_to_string(arg_types[i]));
                }
                printf(")\n");
                if (method_id) {
                    method_id->type_annot = malloc(8);
                    strcpy(method_id->type_annot, "undef");
                }
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                if (arg_types) free(arg_types);
                return JT_UNDEF;
            } else {
                /* Ambiguous call - set undef type but don't print error (already printed) */
                if (method_id) {
                    method_id->type_annot = malloc(8);
                    strcpy(method_id->type_annot, "undef");
                }
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                if (arg_types) free(arg_types);
                return JT_UNDEF;
            }
        }

        case N_ParseArgs: {
            struct node_list *c = n->children;
            if (c) c = c->next;

            JType arg1_type = JT_UNDEF;
            JType arg2_type = JT_UNDEF;

            if (c && c->node) {
                arg1_type = infer_type(c->node, method, ct);
            }
            c = c && c->next ? c->next : NULL;
            if (c && c->node) {
                arg2_type = infer_type(c->node, method, ct);
            }

            /* Check argument types: first should be String[], second should be int */
            if (arg1_type != JT_STRING_ARRAY || arg2_type != JT_INT) {
                printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n",
                       n->line, n->col, jtype_to_string(arg1_type), jtype_to_string(arg2_type));
            }

            n->type_annot = malloc(8);
            strcpy(n->type_annot, "int");
            return JT_INT;
        }

        case N_Assign: {
            struct node_list *c = n->children;
            if (c) c = c->next;
            struct node *lhs = c && c->node ? c->node : NULL;
            struct node *rhs = c && c->next && c->next->node ? c->next->node : NULL;

            JType lhs_type = lhs ? infer_type(lhs, method, ct) : JT_UNDEF;
            JType rhs_type = rhs ? infer_type(rhs, method, ct) : JT_UNDEF;

            /* Check if lhs is a String[] parameter (arrays cannot be reassigned) */
            int is_param_assign = 0;
            if (lhs && lhs->category == N_Identifier) {
                Symbol *lhs_sym = lookup_symbol(lhs->token, method, ct);
                if (lhs_sym && lhs_sym->is_param && lhs_sym->type == JT_STRING_ARRAY) {
                    is_param_assign = 1;
                    printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n",
                           n->line, n->col, jtype_to_string(lhs_type), jtype_to_string(rhs_type));
                }
            }

            /* Report error if either operand is undef or types are incompatible */
            if (!is_param_assign && (lhs_type == JT_UNDEF || rhs_type == JT_UNDEF || !types_compatible(rhs_type, lhs_type))) {
                printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n",
                       n->line, n->col, jtype_to_string(lhs_type), jtype_to_string(rhs_type));
            }

            n->type_annot = malloc(16);
            strcpy(n->type_annot, jtype_to_string(lhs_type));
            return lhs_type;
        }

        default:
            return JT_UNDEF;
    }
}

static void check_statement(struct node *n, MethodEntry *method, ClassTable *ct)
{
    if (!n) return;

    struct node_list *c;
    switch (n->category) {
        case N_VarDecl: {
            /* Add variable to method's symbol table when encountered */
            c = n->children;
            if (c) c = c->next;  /* Skip sentinel */

            /* Get the type from first child */
            JType vtype = JT_UNDEF;
            if (c && c->node) {
                vtype = node_to_jtype(c->node);
                c = c->next;  /* Move to first identifier */
            }

            /* Process all identifiers in this declaration (handles int a, b, c; ) */
            for (; c && c->node; c = c->next) {
                const char *vname = c->node->token;
                int vline = c->node->line;
                int vcol = c->node->col;

                if (is_reserved_id(vname)) {
                    printf("Line %d, col %d: Symbol %s is reserved\n",
                           vline, vcol, vname);
                } else if (find_symbol(method->symbols, vname)) {
                    printf("Line %d, col %d: Symbol %s already defined\n",
                           vline, vcol, vname);
                } else {
                    add_symbol(&method->symbols, vname, vtype, 0, vline, vcol);
                }
            }
            break;
        }

        case N_If: {
            c = n->children;
            if (c) c = c->next;

            /* Track condition type error to report after body processing */
            JType cond_type = JT_BOOLEAN;
            int cond_line = 0, cond_col = 0;
            int has_type_error = 0;

            if (c && c->node) {
                cond_type = infer_type(c->node, method, ct);
                if (cond_type != JT_BOOLEAN) {
                    cond_line = c->node->line;
                    cond_col = c->node->col;
                    has_type_error = 1;
                }
            }

            /* Process then block */
            c = c && c->next ? c->next : NULL;
            if (c && c->node) check_statement(c->node, method, ct);

            /* Process else block */
            c = c && c->next ? c->next : NULL;
            if (c && c->node) check_statement(c->node, method, ct);

            /* Report type error after processing blocks */
            if (has_type_error) {
                printf("Line %d, col %d: Incompatible type %s in if statement\n",
                       cond_line, cond_col, jtype_to_string(cond_type));
            }
            break;
        }

        case N_While: {
            c = n->children;
            if (c) c = c->next;

            /* Track condition type error to report after body processing */
            JType cond_type = JT_BOOLEAN;
            int cond_line = 0, cond_col = 0;
            int has_type_error = 0;

            if (c && c->node) {
                cond_type = infer_type(c->node, method, ct);
                if (cond_type != JT_BOOLEAN) {
                    cond_line = c->node->line;
                    cond_col = c->node->col;
                    has_type_error = 1;
                }
            }

            /* Process body */
            c = c && c->next ? c->next : NULL;
            if (c && c->node) check_statement(c->node, method, ct);

            /* Report type error after processing body */
            if (has_type_error) {
                printf("Line %d, col %d: Incompatible type %s in while statement\n",
                       cond_line, cond_col, jtype_to_string(cond_type));
            }
            break;
        }

        case N_Return:
            c = n->children;
            if (c) c = c->next;
            if (c && c->node) {
                JType ret_type = infer_type(c->node, method, ct);
                if (ret_type == JT_VOID) {
                    /* Returning a void expression is always an error */
                    printf("Line %d, col %d: Incompatible type void in return statement\n",
                           c->node->line, c->node->col);
                } else if (method && !types_compatible(ret_type, method->return_type)) {
                    printf("Line %d, col %d: Incompatible type %s in return statement\n",
                           c->node->line, c->node->col, jtype_to_string(ret_type));
                }
            } else {
                /* No return value (void return) */
                if (method && method->return_type != JT_VOID) {
                    printf("Line %d, col %d: Incompatible type void in return statement\n",
                           n->line, n->col);
                }
            }
            break;

        case N_Print:
            c = n->children;
            if (c) c = c->next;
            if (c && c->node) {
                JType arg_type = infer_type(c->node, method, ct);
                if (arg_type == JT_UNDEF || arg_type == JT_VOID || arg_type == JT_STRING_ARRAY) {
                    printf("Line %d, col %d: Incompatible type %s in System.out.print statement\n",
                           c->node->line, c->node->col, jtype_to_string(arg_type));
                }
            }
            break;

        case N_Assign:
        case N_Call:
        case N_ParseArgs:
            infer_type(n, method, ct);
            break;

        case N_Block:
            c = n->children;
            if (c) c = c->next;
            for (; c && c->node; c = c->next) {
                check_statement(c->node, method, ct);
            }
            break;

        default:
            break;
    }
}

void check_and_annotate_ast(struct node *program, ClassTable *ct)
{
    if (!program || !ct) return;

    struct node_list *c = program->children;
    if (c) c = c->next;
    c = c && c->next ? c->next : NULL;

    /* Track which MethodEntry instances have been processed to skip duplicates */
    MethodEntry *processed[256];
    int n_processed = 0;

    for (; c && c->node; c = c->next) {
        if (c->node->category == N_MethodDecl) {
            struct node_list *md = c->node->children;
            if (md) md = md->next;
            if (md && md->node && md->node->category == N_MethodHeader) {
                JType ret_type;
                char *method_name;
                JType *param_types;
                int n_params;

                process_method_header(md->node, &ret_type, &method_name, &param_types, &n_params);
                MethodEntry *method = find_exact_method(ct->methods, method_name, n_params, param_types);

                /* Check if this method was already processed (duplicate) */
                int already_processed = 0;
                if (method) {
                    for (int i = 0; i < n_processed; i++) {
                        if (processed[i] == method) { already_processed = 1; break; }
                    }
                }

                if (method && !already_processed && md->next && md->next->node &&
                    md->next->node->category == N_MethodBody) {
                    if (n_processed < 256) processed[n_processed++] = method;
                    struct node_list *body = md->next->node->children;
                    if (body) body = body->next;  /* Skip sentinel */
                    for (; body && body->node; body = body->next) {
                        check_statement(body->node, method, ct);
                    }
                }

                if (param_types) free(param_types);
            }
        }
    }
}

```

#### Ficheiro: `Meta3/src/jucompiler.l`

```lex
/*
 * Autores:
 * Simão Tomás Botas Carvalho - 2021223055
 * Teodoro Marques - 2023211717
 */

%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

/* Header gerado por yacc/bison a partir do jucompiler.y.
 * (o teu Makefile deve gerar y.tab.h) */
#include "y.tab.h"

/* Executado antes de cada ação do lexer:
 * guarda a posição inicial do token em syn_* e avança a coluna.
 * Também atualiza yylloc para rastrear localizações Yacc. */
#define YY_USER_ACTION { \
    yylloc.first_line = lex_line; \
    yylloc.first_column = lex_column; \
    syn_line = lex_line; \
    syn_column = lex_column; \
    lex_column += yyleng; \
    yylloc.last_line = lex_line; \
    yylloc.last_column = lex_column; \
}

/* Para tokens com valor (IDENTIFIER, NATURAL, DECIMAL, BOOLLIT, STRLIT, etc.)
 * assume-se que o %union no yacc tem o campo 'lexeme'. */
#define PASS_TOKEN() do { \
    yylval.lexeme = malloc(strlen(yytext) + 1); \
    if (yylval.lexeme) strcpy(yylval.lexeme, yytext); \
} while(0)

int lex_line = 1;
int lex_column = 1;
int syn_line = -1;
int syn_column = -1;
int lex_errs = 0;

int print_tokens = 0;


int comment_start_line = 1;
int comment_start_col = 1;
int string_start_line = 1;
int string_start_col = 1;
char string_buffer[10000];
int buffer_pos = 0;
int string_has_error = 0;

static void sb_append_char(char c) {
    size_t cap = sizeof(string_buffer);
    if (buffer_pos < (int)cap - 1) {
        string_buffer[buffer_pos++] = c;
    } else {
        /* Truncate to avoid writing out of bounds. */
        string_has_error = 1;
    }
}

static void sb_append_two(char a, char b) {
    sb_append_char(a);
    sb_append_char(b);
}

static void print_simple_token(const char *token) {
    if (print_tokens) {
        printf("%s\n", token);
    }
}

static void print_valued_token(const char *token, const char *value) {
    if (print_tokens) {
        printf("%s(%s)\n", token, value);
    }
}

static int check_natural_bounds(const char *text) {
    /* Check if integer literal is within int range (0 to 2147483647) */
    /* Remove underscores for comparison */
    char clean[100];
    int j = 0;
    for (int i = 0; text[i]; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
        }
    }
    clean[j] = '\0';

    /* Check if it's larger than INT_MAX (2147483647) */
    if (j > 10) return 0; /* More than 10 digits, definitely out of bounds */
    if (j == 10 && strcmp(clean, "2147483647") > 0) return 0;
    return 1;
}

static int check_decimal_bounds(const char *text) {
    /* Check if decimal literal is within valid double range */
    /* First, remove underscores (Java allows them, strtod doesn't) */
    char clean[1000];
    int j = 0;
    int has_digits = 0;
    for (int i = 0; text[i] && j < (int)sizeof(clean) - 1; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
            if (text[i] >= '0' && text[i] <= '9') has_digits = 1;
        }
    }
    clean[j] = '\0';

    char *endptr;
    double val = strtod(clean, &endptr);

    /* strtod returns HUGE_VAL on overflow */
    if (val == HUGE_VAL || val == -HUGE_VAL) return 0;

    /* Check for underflow: if we have significant digits but got 0, it underflowed */
    if (val == 0.0 && has_digits) {
        /* Need to check if original string was non-zero */
        /* If string contains non-zero digits and converted to 0, it's underflow */
        for (int i = 0; clean[i]; i++) {
            if (clean[i] >= '1' && clean[i] <= '9') return 0;  /* Non-zero digit found, underflowed */
        }
    }

    return 1;
}

%}

%option noyywrap noinput nounput never-interactive

digit [0-9]
nonzero [1-9]
underscore _
character [a-zA-Z_$]
digitseq {digit}({digit}|{underscore})*{digit}|{digit}
%X COMMENT
%X STR
%%


"/*" { comment_start_line = syn_line; comment_start_col = syn_column; BEGIN(COMMENT); }

<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\r\n { lex_line++; lex_column = 1; }
<COMMENT>\n { lex_line++; lex_column = 1; }
<COMMENT>\r { lex_line++; lex_column = 1; }
<COMMENT>[^*/\r\n]+ { ; }
<COMMENT>[*/] { ; }
<COMMENT><<EOF>> {
    printf("Line %d, col %d: unterminated comment\n", comment_start_line, comment_start_col);
    syn_line = lex_line;
    syn_column = lex_column;
    BEGIN(INITIAL);
    return 0;
}

"//"[^\r\n]* { ; }

"\"" { BEGIN(STR); string_start_line = syn_line; string_start_col = syn_column; buffer_pos = 0; string_has_error = 0; }

<STR>\" {
    char *quoted;
    size_t len;
    size_t cap = sizeof(string_buffer);
    if (buffer_pos < 0) buffer_pos = 0;
    if ((size_t)buffer_pos >= cap) buffer_pos = (int)cap - 1;
    string_buffer[buffer_pos] = '\0';
    if (print_tokens && !string_has_error) printf("STRLIT(\"%s\")\n", string_buffer);
    if (string_has_error) {
        BEGIN(INITIAL);
        return yylex();
    }
    len = strlen(string_buffer);
    quoted = (char *)malloc(len + 3);
    if (quoted) {
        quoted[0] = '"';
        memcpy(quoted + 1, string_buffer, len);
        quoted[len + 1] = '"';
        quoted[len + 2] = '\0';
        yylval.lexeme = quoted;
    } else {
        yylval.lexeme = malloc(3);
        if (yylval.lexeme) strcpy(yylval.lexeme, "\"\"");
    }
    syn_line = string_start_line;
    syn_column = string_start_col;
    BEGIN(INITIAL);
    return STRLIT;
}

<STR>\\[fnrt\\\"] { 
    sb_append_two('\\', yytext[1]);
}

<STR>\\\r\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\\r {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\. { 
    printf("Line %d, col %d: invalid escape sequence (%s)\n", syn_line, syn_column, yytext);
    lex_errs++;
    string_has_error = 1;
}

<STR>\r\n|\r|\n {
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    syn_line = lex_line;
    syn_column = lex_column;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR><<EOF>> { 
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    syn_line = lex_line;
    syn_column = lex_column;
    BEGIN(INITIAL);
    return 0;
}

<STR>. {
    sb_append_char(yytext[0]);
}


[ \t\f]+ { ; }

\r\n { lex_line++; lex_column = 1; }
\n   { lex_line++; lex_column = 1; }
\r   { lex_line++; lex_column = 1; }


"true"|"false" { PASS_TOKEN(); print_valued_token("BOOLLIT", yytext); return BOOLLIT; }
"==" { print_simple_token("EQ"); return EQ; }
"!=" { print_simple_token("NE"); return NE; }
">=" { print_simple_token("GE"); return GE; }
"<=" { print_simple_token("LE"); return LE; }
"<<" { print_simple_token("LSHIFT"); return LSHIFT; }
">>" { print_simple_token("RSHIFT"); return RSHIFT; }
"->" { print_simple_token("ARROW"); return ARROW; }
"&&" { print_simple_token("AND"); return AND; }
"||" { print_simple_token("OR"); return OR; }
">" { print_simple_token("GT"); return GT; }
"<" { print_simple_token("LT"); return LT; }
"=" { print_simple_token("ASSIGN"); return ASSIGN; }
"!" { print_simple_token("NOT"); return NOT; }
"+" { print_simple_token("PLUS"); return PLUS; }
"-" { print_simple_token("MINUS"); return MINUS; }
"*" { print_simple_token("STAR"); return STAR; }
"/" { print_simple_token("DIV"); return DIV; }
"%" { print_simple_token("MOD"); return MOD; }
"^" { print_simple_token("XOR"); return XOR; }
"," { print_simple_token("COMMA"); return COMMA; }
";" { print_simple_token("SEMICOLON"); return SEMICOLON; }
"(" { print_simple_token("LPAR"); return LPAR; }
")" { print_simple_token("RPAR"); return RPAR; }
"{" { print_simple_token("LBRACE"); return LBRACE; }
"}" { print_simple_token("RBRACE"); return RBRACE; }
"[" { print_simple_token("LSQ"); return LSQ; }
"]" { print_simple_token("RSQ"); return RSQ; }



"boolean" { print_simple_token("BOOL"); return BOOL; }
"class" { print_simple_token("CLASS"); return CLASS; }
".length" { print_simple_token("DOTLENGTH"); return DOTLENGTH; }
"double" { print_simple_token("DOUBLE"); return DOUBLE; }
"else" { print_simple_token("ELSE"); return ELSE; }
"if" { print_simple_token("IF"); return IF; }
"int" { print_simple_token("INT"); return INT; }
"System.out.print" { print_simple_token("PRINT"); return PRINT; }
"Integer.parseInt" { print_simple_token("PARSEINT"); return PARSEINT; }
"public" { print_simple_token("PUBLIC"); return PUBLIC; }
"return" { print_simple_token("RETURN"); return RETURN; }
"static" { print_simple_token("STATIC"); return STATIC; }
"String" { print_simple_token("STRING"); return STRING; }
"void" { print_simple_token("VOID"); return VOID; }
"while" { print_simple_token("WHILE"); return WHILE; }

"++" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"--" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"null" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"Integer" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"System" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"abstract" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"assert" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"break" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"byte" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"case" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"catch" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"char" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"const" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"continue" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"default" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"do" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"enum" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"extends" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"final" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"finally" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"float" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"for" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"goto" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"implements" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"import" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"instanceof" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"interface" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"long" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"native" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"new" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"package" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"private" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"protected" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"short" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"strictfp" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"super" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"switch" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"synchronized" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"this" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"throw" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"throws" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"transient" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"try" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"volatile" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }


{digitseq}[eE][+-]?{digitseq} {
    PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL;
}
{digitseq}?"."{digitseq}([eE][+-]?{digitseq})? {
    PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL;
}
{digitseq}"."([eE][+-]?{digitseq})? {
    PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL;
}

{character}+({digit}|{character})* { PASS_TOKEN(); print_valued_token("IDENTIFIER", yytext); return IDENTIFIER; }
0|{nonzero}({digit}|{underscore})*{digit}|{nonzero} {
    PASS_TOKEN(); print_valued_token("NATURAL", yytext); return NATURAL;
}

. { lex_errs++; printf("Line %d, col %d: illegal character (%s)\n", syn_line, syn_column, yytext); }

<INITIAL><<EOF>> {
    syn_line = lex_line;
    syn_column = lex_column;
    return 0;
}

%%


```

#### Ficheiro: `Meta3/src/jucompiler.y`

```c
/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 *
 * Meta 3 -- Semantic Analysis (Phase 1: Symbol Table)
 */

%locations

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic.h"

#define YYDEBUG 0

int  yylex(void);
int  yyparse(void);
void yyerror(const char *s);
void printast(struct node *root);

extern int  lex_line, lex_column;
extern int  syn_line, syn_column;
extern int  lex_errs;
extern char *yytext;
extern int  yychar;
extern int  print_tokens;

struct node *ast      = NULL;
int          syn_errs = 0;
ClassTable  *class_table = NULL;

static struct node *cur_type_node;
static struct node *cur_field_type;
static struct node *prog_root;
static struct node *vd_accum;
static char        *vd_first_id;
static char        *class_id;

static struct node *make_block(struct node *sl);
static struct node *type_node(enum category cat, int line, int col)
{
    return newnode(cat, NULL, line, col);
}
static void add_field(struct node *tn, char *id, int line, int col);
static void add_vardecl(struct node *acc, struct node *tn, char *id, int line, int col);

static struct node *err_cond_placeholder(void)
{
    return newnode(N_Block, NULL, 0, 0);
}
%}

%union {
    char        *lexeme;
    struct node *n;
}

%token BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT
       PUBLIC RETURN STATIC STRING VOID WHILE
%token EQ NE GE LE LSHIFT RSHIFT ARROW AND OR GT LT ASSIGN NOT
%token PLUS MINUS STAR DIV MOD XOR
%token COMMA SEMICOLON LPAR RPAR LBRACE RBRACE LSQ RSQ

%token <lexeme> IDENTIFIER NATURAL DECIMAL STRLIT BOOLLIT RESERVED

%type <n> class_members method_decl method_header method_body
%type <n> type formal_params fp_list
%type <n> stmt_list method_stmt_list var_decl
%type <n> stmt stmt_no_if
%type <n> expr print_arg opt_expr block_stmt op_expr
%type <n> call_args nonempty_call_args method_invocation assignment_expr parse_args_stmt

%nonassoc IFX
%nonassoc ELSE

%right ASSIGN
%left OR
%left AND
%left XOR
%left EQ NE
%left LT GT LE GE
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD

%nonassoc UPLUS UMINUS UNOT

%%

program:
      CLASS IDENTIFIER { class_id = $2; } LBRACE class_members RBRACE
        { ast = $5; }
    ;

class_members:
      /* vazio */
        {
            $$ = newnode(N_Program, NULL, @$.first_line, @$.first_column);
            addchild($$, newnode(N_Identifier, class_id, @$.first_line, @$.first_column));
            prog_root  = $$;
            vd_accum   = NULL;
        }
    | class_members PUBLIC STATIC field_decl
        { $$ = $1; }
    | class_members PUBLIC STATIC method_decl
        { $$ = $1; addchild($1, $4); }
    | class_members SEMICOLON
        { $$ = $1; }
    | class_members error SEMICOLON
        { $$ = $1; }
    ;

field_decl:
      type IDENTIFIER
        {
            cur_field_type = $1;
            add_field($1, $2, @2.first_line, @2.first_column);
        }
      field_id_list SEMICOLON
    | error SEMICOLON
        { }
    ;

field_id_list:
      /* vazio */
    | field_id_list COMMA IDENTIFIER
        {
            add_field(cur_field_type, $3, @3.first_line, @3.first_column);
        }
    ;

method_decl:
      method_header method_body
        {
            $$ = newnode(N_MethodDecl, NULL, @$.first_line, @$.first_column);
            addchild($$, $1);
            addchild($$, $2);
        }
    ;

method_header:
      type IDENTIFIER LPAR formal_params RPAR
        {
            $$ = newnode(N_MethodHeader, NULL, @$.first_line, @$.first_column);
            addchild($$, $1);
            addchild($$, newnode(N_Identifier, $2, @2.first_line, @2.first_column));
            addchild($$, $4);
        }
    | VOID IDENTIFIER LPAR formal_params RPAR
        {
            $$ = newnode(N_MethodHeader, NULL, @$.first_line, @$.first_column);
            addchild($$, type_node(N_Void, @1.first_line, @1.first_column));
            addchild($$, newnode(N_Identifier, $2, @2.first_line, @2.first_column));
            addchild($$, $4);
        }
    ;

formal_params:
      fp_list
    | STRING LSQ RSQ IDENTIFIER
        {
            struct node *pd = newnode(N_ParamDecl, NULL, @$.first_line, @$.first_column);
            addchild(pd, type_node(N_StringArray, @1.first_line, @1.first_column));
            addchild(pd, newnode(N_Identifier, $4, @4.first_line, @4.first_column));
            $$ = newnode(N_MethodParams, NULL, @$.first_line, @$.first_column);
            addchild($$, pd);
        }
    | /* epsilon */
        {
            $$ = newnode(N_MethodParams, NULL, yylloc.first_line, yylloc.first_column);
        }
    ;

fp_list:
      type IDENTIFIER
        {
            struct node *pd = newnode(N_ParamDecl, NULL, @$.first_line, @$.first_column);
            addchild(pd, $1);
            addchild(pd, newnode(N_Identifier, $2, @2.first_line, @2.first_column));
            $$ = newnode(N_MethodParams, NULL, @$.first_line, @$.first_column);
            addchild($$, pd);
        }
    | fp_list COMMA type IDENTIFIER
        {
            struct node *pd = newnode(N_ParamDecl, NULL, @3.first_line, @3.first_column);
            addchild(pd, $3);
            addchild(pd, newnode(N_Identifier, $4, @4.first_line, @4.first_column));
            $$ = $1;
            addchild($$, pd);
        }
    ;

type:
      INT    { $$ = type_node(N_Int,    @1.first_line, @1.first_column); }
    | DOUBLE { $$ = type_node(N_Double, @1.first_line, @1.first_column); }
    | BOOL   { $$ = type_node(N_Bool,   @1.first_line, @1.first_column); }
    ;

method_body:
      LBRACE method_stmt_list RBRACE
        {
            struct node_list *c;
            $$ = newnode(N_MethodBody, NULL, @$.first_line, @$.first_column);
            for (c = $2->children; c; c = c->next)
                if (c->node) addchild($$, c->node);
        }
    ;

var_decl:
      type IDENTIFIER
        {
            cur_type_node = $1;
            vd_first_id = $2;
            vd_accum = newnode(N_MethodBody, NULL, @$.first_line, @$.first_column);
            add_vardecl(vd_accum, cur_type_node, vd_first_id, @2.first_line, @2.first_column);
        }
      vd_id_list SEMICOLON
        { $$ = vd_accum; }
    ;

vd_id_list:
      /* vazio */
    | vd_id_list COMMA IDENTIFIER
        {
            add_vardecl(vd_accum, cur_type_node, $3, @3.first_line, @3.first_column);
        }
    ;

method_stmt_list:
      /* vazio */   { $$ = newnode(N_MethodBody, NULL, yylloc.first_line, yylloc.first_column); }
    | method_stmt_list stmt
        {
            $$ = $1;
            if ($2) addchild($1, $2);
        }
    | method_stmt_list var_decl
        {
            struct node_list *c;
            $$ = $1;
            if ($2) {
                for (c = $2->children; c; c = c->next)
                    if (c->node) addchild($$, c->node);
            }
        }
    ;

stmt_list:
      /* vazio */   { $$ = newnode(N_MethodBody, NULL, yylloc.first_line, yylloc.first_column); }
    | stmt_list stmt
        {
            $$ = $1;
            if ($2) addchild($1, $2);
        }
    ;

stmt:
      stmt_no_if
    | IF LPAR expr RPAR stmt  %prec IFX
        {
            $$ = newnode(N_If, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
            addchild($$, $5 ? $5 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
            addchild($$, newnode(N_Block, NULL, @$.first_line, @$.first_column));
        }
    | IF LPAR expr RPAR stmt ELSE stmt
        {
            $$ = newnode(N_If, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
            addchild($$, $5 ? $5 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
            addchild($$, $7 ? $7 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
        }
    ;

stmt_no_if:
    WHILE LPAR expr RPAR stmt
        {
            $$ = newnode(N_While, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
            addchild($$, $5 ? $5 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
        }
    | PRINT LPAR print_arg RPAR SEMICOLON
        {
            $$ = newnode(N_Print, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
        }
    | PRINT LPAR error RPAR SEMICOLON
        { $$ = NULL; }
    | PRINT LPAR error SEMICOLON
        { $$ = NULL; }
    | block_stmt
    | method_invocation SEMICOLON
        { $$ = $1; }
    | assignment_expr SEMICOLON
        { $$ = $1; }
    | parse_args_stmt SEMICOLON
        { $$ = $1; }
    | RETURN opt_expr SEMICOLON
        {
            $$ = newnode(N_Return, NULL, @1.first_line, @1.first_column);
            if ($2) addchild($$, $2);
        }
    | RETURN error SEMICOLON
        {
            $$ = newnode(N_Return, NULL, @1.first_line, @1.first_column);
        }
    | error SEMICOLON
        { $$ = NULL; }
    | SEMICOLON
        { $$ = NULL; }
    ;

block_stmt:
      LBRACE stmt_list RBRACE
        {
            struct node *blk = make_block($2);
            /* If block is empty (only sentinel child, no real content), skip it */
            if (blk && blk->category == N_Block && !blk->children->next) {
                $$ = NULL;
            } else {
                $$ = blk;
            }
        }
    ;

method_invocation:
      IDENTIFIER LPAR call_args RPAR
        {
            struct node_list *c;
            $$ = newnode(N_Call, NULL, @1.first_line, @1.first_column);
            addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column));
            for (c = $3->children; c; c = c->next) addchild($$, c->node);
        }
    | IDENTIFIER LPAR error RPAR
        { $$ = NULL; }
    ;

call_args:
      /* vazio */       { $$ = newnode(N_MethodBody, NULL, yylloc.first_line, yylloc.first_column); }
    | nonempty_call_args  { $$ = $1; }
    ;

nonempty_call_args:
      expr              { $$ = newnode(N_MethodBody, NULL, @$.first_line, @$.first_column); addchild($$, $1); }
    | nonempty_call_args COMMA expr
        { $$ = $1; addchild($$, $3); }
    ;

assignment_expr:
      IDENTIFIER ASSIGN expr
        {
            $$ = newnode(N_Assign, NULL, @2.first_line, @2.first_column);
            addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column));
            addchild($$, $3);
        }
    ;

parse_args_stmt:
      PARSEINT LPAR IDENTIFIER LSQ expr RSQ RPAR
        {
            $$ = newnode(N_ParseArgs, NULL, @1.first_line, @1.first_column);
            addchild($$, newnode(N_Identifier, $3, @3.first_line, @3.first_column));
            addchild($$, $5);
        }
    | PARSEINT LPAR IDENTIFIER LSQ error RSQ RPAR
        { $$ = NULL; }
    | PARSEINT LPAR IDENTIFIER LSQ error RPAR
        { $$ = NULL; }
    | PARSEINT LPAR error RPAR
        { $$ = NULL; }
    ;

opt_expr:
      /* vazio */ { $$ = NULL; }
    | expr        { $$ = $1; }
    ;

print_arg:
      expr   { $$ = $1; }
    | STRLIT { $$ = newnode(N_StrLit, $1, @1.first_line, @1.first_column); }
    ;

expr:
      assignment_expr
        { $$ = $1; }
    | op_expr
        { $$ = $1; }
    ;

op_expr:
      NATURAL
        { $$ = newnode(N_Natural, $1, @1.first_line, @1.first_column); }
    | DECIMAL
        { $$ = newnode(N_Decimal, $1, @1.first_line, @1.first_column); }
    | BOOLLIT
        { $$ = newnode(N_BoolLit, $1, @1.first_line, @1.first_column); }
    | LPAR expr RPAR  { $$ = $2; }
    | LPAR error RPAR { $$ = NULL; }
    | IDENTIFIER
        { $$ = newnode(N_Identifier, $1, @1.first_line, @1.first_column); }
    | IDENTIFIER DOTLENGTH
        { $$ = newnode(N_Length, NULL, @2.first_line, @2.first_column);
          addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column)); }
    | IDENTIFIER LPAR call_args RPAR
        {
            struct node_list *c;
            $$ = newnode(N_Call, NULL, @1.first_line, @1.first_column);
            addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column));
            for (c = $3->children; c; c = c->next) addchild($$, c->node);
        }
    | IDENTIFIER LPAR error RPAR
        { $$ = NULL; }
    | PARSEINT LPAR IDENTIFIER LSQ expr RSQ RPAR
        { $$ = newnode(N_ParseArgs, NULL, @1.first_line, @1.first_column);
          addchild($$, newnode(N_Identifier, $3, @3.first_line, @3.first_column));
          addchild($$, $5); }
    | PARSEINT LPAR IDENTIFIER LSQ error RSQ RPAR
        { $$ = NULL; }
    | PARSEINT LPAR error RPAR
        { $$ = NULL; }
    | MINUS op_expr %prec UMINUS
        { $$ = newnode(N_Minus, NULL, @1.first_line, @1.first_column); addchild($$, $2); }
    | PLUS  op_expr %prec UPLUS
        { $$ = newnode(N_Plus,  NULL, @1.first_line, @1.first_column); addchild($$, $2); }
    | NOT   op_expr %prec UNOT
        { $$ = newnode(N_Not,   NULL, @1.first_line, @1.first_column); addchild($$, $2); }
    | op_expr OR     op_expr  { $$ = newnode(N_Or,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr AND    op_expr  { $$ = newnode(N_And,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr XOR    op_expr  { $$ = newnode(N_Xor,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr EQ     op_expr  { $$ = newnode(N_Eq,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr NE     op_expr  { $$ = newnode(N_Ne,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr LT     op_expr  { $$ = newnode(N_Lt,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr GT     op_expr  { $$ = newnode(N_Gt,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr LE     op_expr  { $$ = newnode(N_Le,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr GE     op_expr  { $$ = newnode(N_Ge,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr LSHIFT op_expr  { $$ = newnode(N_Lshift, NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr RSHIFT op_expr  { $$ = newnode(N_Rshift, NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr PLUS   op_expr  { $$ = newnode(N_Add,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr MINUS  op_expr  { $$ = newnode(N_Sub,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr STAR   op_expr  { $$ = newnode(N_Mul,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr DIV    op_expr  { $$ = newnode(N_Div,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr MOD    op_expr  { $$ = newnode(N_Mod,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    ;

%%

static void add_field(struct node *tn, char *id, int line, int col)
{
    struct node *fd = newnode(N_FieldDecl, NULL, line, col);
    addchild(fd, tn);
    addchild(fd, newnode(N_Identifier, id, line, col));
    addchild(prog_root, fd);
}

static void add_vardecl(struct node *acc, struct node *tn, char *id, int line, int col)
{
    struct node *vd = newnode(N_VarDecl, NULL, line, col);
    addchild(vd, tn);
    addchild(vd, newnode(N_Identifier, id, line, col));
    if (acc)
        addchild(acc, vd);
}

const char *category_name[] = {
    "Program", "FieldDecl", "VarDecl", "MethodDecl",
    "MethodHeader", "MethodParams", "ParamDecl", "MethodBody",
    "Block", "If", "While", "Return", "Call", "Print", "ParseArgs", "Assign",
    "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge",
    "Add", "Sub", "Mul", "Div", "Mod",
    "Lshift", "Rshift", "Xor", "Not", "Minus", "Plus", "Length",
    "Int", "Double", "Bool", "StringArray", "Void",
    "Identifier", "Natural", "Decimal", "BoolLit", "StrLit"
};

static struct node *make_block(struct node *sl)
{
    int nc = 0;
    int saw_any = 0;
    struct node *single = NULL;
    struct node_list *c;
    struct node *blk;
    struct node_list *c2;

    if (!sl) return newnode(N_Block, NULL, 0, 0);

    for (c = sl->children; c; c = c->next) {
        if (c->node) {
            int is_empty_block = 0;
            struct node_list *cc;
            saw_any = 1;
            if (c->node->category == N_Block) {
                is_empty_block = 1;
                for (cc = c->node->children; cc; cc = cc->next)
                    if (cc->node) { is_empty_block = 0; break; }
            }
            if (is_empty_block) continue;
            nc++;
            if (nc == 1) single = c->node;
        }
    }
    if (nc == 0)
        return saw_any ? NULL : newnode(N_Block, NULL, 0, 0);
    if (nc == 1) return single;

    blk = newnode(N_Block, NULL, 0, 0);
    for (c2 = sl->children; c2; c2 = c2->next) {
        if (c2->node) {
            int is_empty_block = 0;
            struct node_list *cc;
            if (c2->node->category == N_Block) {
                is_empty_block = 1;
                for (cc = c2->node->children; cc; cc = cc->next)
                    if (cc->node) { is_empty_block = 0; break; }
            }
            if (!is_empty_block) addchild(blk, c2->node);
        }
    }
    return blk;
}

void yyerror(const char *msg)
{
    syn_errs++;
    if (yytext && yytext[0])
        if (yychar == STRLIT && yylval.lexeme)
            printf("Line %d, col %d: syntax error: %s\n",
                   syn_line, syn_column, yylval.lexeme);
        else
            printf("Line %d, col %d: syntax error: %s\n",
                   syn_line, syn_column, yytext);
    else
        printf("Line %d, col %d: syntax error: \n",
               syn_line, syn_column);
}

int main(int argc, char **argv)
{
    int i;
    int parse_status;
    int mode = 4;

    for (i = 1; i < argc; i++) {
        if      (!strcmp(argv[i], "-l"))  mode = 0;
        else if (!strcmp(argv[i], "-e1")) mode = 1;
        else if (!strcmp(argv[i], "-e2")) mode = 2;
        else if (!strcmp(argv[i], "-t"))  mode = 3;
        else if (!strcmp(argv[i], "-s"))  mode = 4;
    }
    if (mode == 0 || mode == 1) {
        print_tokens = (mode == 0);
        while (yylex() != 0) ;
        return 0;
    }
    print_tokens = 0;
    parse_status = yyparse();
    if (parse_status != 0 && lex_errs > 0) {
        while (yylex() != 0) ;
    }
    if (mode == 3 && ast && syn_errs == 0)
        printast(ast);
    if ((mode == 2 || mode == 4) && ast && syn_errs == 0) {
        class_table = build_symbol_tables(ast);
        if (class_table) {
            check_and_annotate_ast(ast, class_table);
            if (mode == 4) {
                print_symbol_tables(class_table);
                printast(ast);
            }
            free_class_table(class_table);
        }
    }
    return 0;
}

```

### Meta 4 (estado atual)

#### Ficheiro: `Meta4/src/ast.h`

```c
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
    int line, col;
    char *type_annot;  /* type annotation string (e.g., "int", "(int,double)", NULL) */
    struct node_list *children;
};

struct node_list {
    struct node *node;
    struct node_list *next;
};

struct node *newnode(enum category category, char *token, int line, int col);
void addchild(struct node *parent, struct node *child);

#endif

```

#### Ficheiro: `Meta4/src/ast.c`

```c
/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

/* Definido em jucompiler.y */
extern const char *category_name[];

struct node *newnode(enum category category, char *token, int line, int col)
{
    struct node *n = malloc(sizeof(struct node));
    n->category = category;
    n->token = token;
    n->line = line;
    n->col = col;
    n->type_annot = NULL;
    n->children = malloc(sizeof(struct node_list));
    n->children->node = NULL;
    n->children->next = NULL;
    return n;
}

void addchild(struct node *parent, struct node *child)
{
    struct node_list *c;
    struct node_list *nl = malloc(sizeof(struct node_list));
    nl->node = child;
    nl->next = NULL;
    c = parent->children;
    while (c->next) c = c->next;
    c->next = nl;
}

static void printast_rec(struct node *n, int depth)
{
    int i;
    struct node_list *c;
    if (!n) return;

    for (i = 0; i < depth; i++) printf("..");

    printf("%s", category_name[n->category]);
    if (n->token) printf("(%s)", n->token);
    if (n->type_annot) printf(" - %s", n->type_annot);
    printf("\n");

    for (c = n->children; c; c = c->next)
        printast_rec(c->node, depth + 1);
}

void printast(struct node *root)
{
    printast_rec(root, 0);
}

```

#### Ficheiro: `Meta4/src/semantic.h`

```c
/*
 * Semantic Analyzer Header - Meta 3
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include "ast.h"

typedef enum {
    JT_INT, JT_DOUBLE, JT_BOOLEAN, JT_VOID, JT_STRING_ARRAY, JT_STRING, JT_UNDEF
} JType;

typedef struct Symbol {
    char *name;
    JType type;
    int is_param;
    int line, col;
    struct Symbol *next;
} Symbol;

typedef struct MethodEntry {
    char *name;
    JType return_type;
    JType *param_types;
    int n_params;
    Symbol *symbols;
    int line, col;
    struct MethodEntry *next;
} MethodEntry;

/* Ordered list node for class-level entries (fields and methods mixed) */
typedef enum { CE_FIELD, CE_METHOD } ClassEntryKind;
typedef struct ClassEntryNode {
    ClassEntryKind kind;
    union {
        Symbol *field;
        MethodEntry *method;
    };
    struct ClassEntryNode *next;
} ClassEntryNode;

typedef struct {
    char *name;
    Symbol *fields;
    MethodEntry *methods;
    ClassEntryNode *entries;  /* Fields and methods in declaration order */
    int semantic_errors;
} ClassTable;

ClassTable *build_symbol_tables(struct node *program);
void print_symbol_tables(ClassTable *ct);
void check_and_annotate_ast(struct node *program, ClassTable *ct);
void free_class_table(ClassTable *ct);

const char *jtype_to_string(JType t);
JType node_to_jtype(struct node *type_node);
void process_method_header(struct node *header, JType *ret_type, char **method_name, JType **param_types, int *n_params);
MethodEntry *find_exact_method(MethodEntry *methods, const char *name, int n_params, JType *param_types);
Symbol *find_symbol(Symbol *symbols, const char *name);

#endif

```

#### Ficheiro: `Meta4/src/semantic.c`

```c
int semantic_errors = 0;
/*
 * Semantic Analyzer Implementation - Meta 3
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "semantic.h"

extern const char *category_name[];

static int check_natural_bounds(const char *text) {
    /* Check if integer literal is within int range (0 to 2147483647) */
    /* Remove underscores for comparison */
    char clean[32];
    int j = 0;
    for (int i = 0; text[i] && j < 20; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
        }
    }
    clean[j] = '\0';

    /* Check if it's larger than INT_MAX (2147483647) */
    if (j > 10) return 0; /* More than 10 digits, definitely out of bounds */
    if (j == 10 && strcmp(clean, "2147483647") > 0) return 0;
    return 1;
}

static int check_decimal_bounds(const char *text) {
    /* Check if decimal literal is within valid double range */
    /* First, remove underscores (Java allows them, strtod doesn't) */
    char clean[1000];
    int j = 0;
    int has_digits = 0;
    for (int i = 0; text[i] && j < (int)sizeof(clean) - 1; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
            if (text[i] >= '0' && text[i] <= '9') has_digits = 1;
        }
    }
    clean[j] = '\0';

    char *endptr;
    double val = strtod(clean, &endptr);

    /* strtod returns HUGE_VAL on overflow */
    if (val == HUGE_VAL || val == -HUGE_VAL) return 0;

    /* Check for underflow: if we have significant digits but got 0, it underflowed */
    if (val == 0.0 && has_digits) {
        /* Need to check if original string was non-zero */
        /* If string contains non-zero digits and converted to 0, it's underflow */
        for (int i = 0; clean[i]; i++) {
            if (clean[i] == 'e' || clean[i] == 'E') break;
            if (clean[i] >= '1' && clean[i] <= '9') return 0;  /* Non-zero digit found in mantissa, underflowed */
        }
    }

    return 1;
}

const char *jtype_to_string(JType t)
{
    switch (t) {
        case JT_INT: return "int";
        case JT_DOUBLE: return "double";
        case JT_BOOLEAN: return "boolean";
        case JT_VOID: return "void";
        case JT_STRING_ARRAY: return "String[]";
        case JT_STRING: return "String";
        case JT_UNDEF: return "undef";
    }
    return "unknown";
}

JType node_to_jtype(struct node *type_node)
{
    if (!type_node) return JT_UNDEF;

    switch (type_node->category) {
        case N_Int: return JT_INT;
        case N_Double: return JT_DOUBLE;
        case N_Bool: return JT_BOOLEAN;
        case N_Void: return JT_VOID;
        case N_StringArray: return JT_STRING_ARRAY;
        default: return JT_UNDEF;
    }
}

static int is_reserved_id(const char *name)
{
    return (strcmp(name, "_") == 0);
}

Symbol *find_symbol(Symbol *symbols, const char *name)
{
    for (Symbol *s = symbols; s; s = s->next) {
        if (strcmp(s->name, name) == 0) return s;
    }
    return NULL;
}

static void add_symbol(Symbol **symbols, const char *name, JType type, int is_param, int line, int col)
{
    Symbol *s = malloc(sizeof(Symbol));
    s->name = malloc(strlen(name) + 1);
    strcpy(s->name, name);
    s->type = type;
    s->is_param = is_param;
    s->line = line;
    s->col = col;
    s->next = NULL;

    if (!*symbols) {
        *symbols = s;
    } else {
        Symbol *tail = *symbols;
        while (tail->next) tail = tail->next;
        tail->next = s;
    }
}

static int types_compatible(JType actual, JType expected); /* Forward declaration */

static MethodEntry *find_method_by_signature(MethodEntry *methods, const char *name, int n_params)
{
    for (MethodEntry *m = methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n_params) return m;
    }
    return NULL;
}

MethodEntry *find_exact_method(MethodEntry *methods, const char *name, int n_params,
                                       JType *arg_types)
{
    /* Find a method with exact type match (all types must be equal) */
    for (MethodEntry *m = methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n_params) {
            int exact = 1;
            for (int i = 0; i < n_params; i++) {
                if (arg_types[i] != m->param_types[i]) {
                    exact = 0;
                    break;
                }
            }
            if (exact) return m;
        }
    }
    return NULL;
}

static MethodEntry *find_compatible_method(MethodEntry *methods, const char *name, int n_params,
                                           JType *arg_types, int *out_count)
{
    /* Find all methods with matching name and parameter count whose types are compatible */
    MethodEntry *first_match = NULL;
    int match_count = 0;

    for (MethodEntry *m = methods; m; m = m->next) {
        if (strcmp(m->name, name) == 0 && m->n_params == n_params) {
            /* Check if all parameter types are compatible */
            int compatible = 1;
            for (int i = 0; i < n_params; i++) {
                if (!types_compatible(arg_types[i], m->param_types[i])) {
                    compatible = 0;
                    break;
                }
            }
            if (compatible) {
                match_count++;
                if (!first_match) first_match = m;
            }
        }
    }

    *out_count = match_count;
    return first_match;
}

static void add_class_entry(ClassTable *ct, ClassEntryKind kind, Symbol *field, MethodEntry *method)
{
    ClassEntryNode *en = malloc(sizeof(ClassEntryNode));
    en->kind = kind;
    if (kind == CE_FIELD) en->field = field;
    else en->method = method;
    en->next = NULL;
    if (!ct->entries) {
        ct->entries = en;
    } else {
        ClassEntryNode *tail = ct->entries;
        while (tail->next) tail = tail->next;
        tail->next = en;
    }
}

static MethodEntry *add_method(MethodEntry **methods, const char *name, JType return_type,
                                JType *param_types, int n_params, int line, int col)
{
    MethodEntry *m = malloc(sizeof(MethodEntry));
    m->name = malloc(strlen(name) + 1);
    strcpy(m->name, name);
    m->return_type = return_type;
    m->n_params = n_params;
    m->param_types = NULL;
    if (n_params > 0) {
        m->param_types = malloc(n_params * sizeof(JType));
        memcpy(m->param_types, param_types, n_params * sizeof(JType));
    }
    m->symbols = NULL;
    m->line = line;
    m->col = col;
    m->next = NULL;

    if (!*methods) {
        *methods = m;
    } else {
        MethodEntry *tail = *methods;
        while (tail->next) tail = tail->next;
        tail->next = m;
    }

    return m;
}

static JType extract_param_type(struct node *param_node)
{
    if (!param_node) return JT_UNDEF;
    if (param_node->category == N_StringArray) return JT_STRING_ARRAY;
    return node_to_jtype(param_node);
}

void process_method_header(struct node *header, JType *ret_type,
                                   char **method_name, JType **param_types, int *n_params)
{
    *ret_type = JT_UNDEF;
    *method_name = NULL;
    *param_types = NULL;
    *n_params = 0;

    if (!header || header->category != N_MethodHeader) return;

    struct node_list *c = header->children;
    if (c) c = c->next;  /* Skip sentinel */
    if (!c || !c->node) return;

    /* First child: return type */
    *ret_type = node_to_jtype(c->node);
    if (c->node->category == N_Void) *ret_type = JT_VOID;

    /* Second child: identifier (method name) */
    c = c->next;
    if (c && c->node && c->node->category == N_Identifier) {
        *method_name = c->node->token;
    }

    /* Third child: MethodParams */
    c = c->next;
    if (c && c->node && c->node->category == N_MethodParams) {
        struct node_list *pc = c->node->children;
        if (pc) pc = pc->next;  /* Skip sentinel */
        int count = 0;
        for (struct node_list *tmp = pc; tmp && tmp->node; tmp = tmp->next) {
            if (tmp->node->category == N_ParamDecl) count++;
        }

        if (count > 0) {
            *param_types = malloc(count * sizeof(JType));
            int idx = 0;
            for (pc = c->node->children; pc; pc = pc->next) {
                if (pc->node && pc->node->category == N_ParamDecl) {
                    struct node_list *pd = pc->node->children;
                    if (pd) pd = pd->next;  /* Skip sentinel */
                    if (pd && pd->node) {
                        (*param_types)[idx++] = extract_param_type(pd->node);
                    }
                }
            }
        }
        *n_params = count;
    }
}

ClassTable *build_symbol_tables(struct node *program)
{
    if (!program || program->category != N_Program) return NULL;

    ClassTable *ct = malloc(sizeof(ClassTable));
    ct->name = NULL;
    ct->fields = NULL;
    ct->methods = NULL;
    ct->entries = NULL;
    ct->semantic_errors = 0;

    /* First child is class Identifier (skip sentinel) */
    struct node_list *c = program->children;
    if (c) c = c->next;  /* Skip sentinel */
    if (c && c->node && c->node->category == N_Identifier) {
        if (c->node->token) {
            ct->name = malloc(strlen(c->node->token) + 1);
            strcpy(ct->name, c->node->token);
        }
    }

    /* Process FieldDecl and MethodDecl */
    for (c = c->next; c; c = c->next) {
        if (!c->node) {
            continue;
        }

        if (c->node->category == N_FieldDecl) {
            struct node_list *fd = c->node->children;
            if (fd) fd = fd->next;  /* Skip sentinel */

            /* Get type from first child */
            JType type = JT_UNDEF;
            if (fd && fd->node) {
                type = node_to_jtype(fd->node);
                fd = fd->next;  /* Move to first identifier */
            }

            /* Process all identifiers in this field declaration (handles int a, b, c;) */
            for (; fd && fd->node; fd = fd->next) {
                const char *id = fd->node->token;
                int line = fd->node->line;
                int col = fd->node->col;

                if (is_reserved_id(id)) {
                    (semantic_errors++, printf("Line %d, col %d: Symbol %s is reserved\n", line, col, id));
                } else if (find_symbol(ct->fields, id)) {
                    (semantic_errors++, printf("Line %d, col %d: Symbol %s already defined\n", line, col, id));
                } else {
                    add_symbol(&ct->fields, id, type, 0, line, col);
                    add_class_entry(ct, CE_FIELD, find_symbol(ct->fields, id), NULL);
                }
            }
        }
        else if (c->node->category == N_MethodDecl) {
            struct node_list *md = c->node->children;
            if (md) md = md->next;  /* Skip sentinel */
            if (md && md->node && md->node->category == N_MethodHeader) {
                JType ret_type;
                char *method_name;
                JType *param_types;
                int n_params;

                process_method_header(md->node, &ret_type, &method_name, &param_types, &n_params);

                if (method_name) {
                    int method_line = md->node->line;
                    int method_col = md->node->col;
                    struct node_list *hdr_c = md->node->children;
                    if (hdr_c) hdr_c = hdr_c->next;  /* Skip sentinel */
                    if (hdr_c) hdr_c = hdr_c->next;  /* Skip return type */
                    if (hdr_c && hdr_c->node && hdr_c->node->category == N_Identifier) {
                        method_line = hdr_c->node->line;
                        method_col = hdr_c->node->col;
                    }

                    /* Check for reserved method name (but don't report yet) */
                    int reserved_method = is_reserved_id(method_name);

                    /* Check for exact duplicate: same name AND same parameter types */
                    MethodEntry *existing = find_method_by_signature(ct->methods, method_name, n_params);
                    int is_duplicate = 0;
                    if (existing && n_params == existing->n_params) {
                        int types_match = 1;
                        for (int i = 0; i < n_params; i++) {
                            if (param_types[i] != existing->param_types[i]) {
                                types_match = 0;
                                break;
                            }
                        }
                        is_duplicate = types_match;
                    }

                    MethodEntry *method = NULL;
                    if (!reserved_method && !is_duplicate) {
                        method = add_method(&ct->methods, method_name, ret_type, param_types, n_params,
                                            method_line, method_col);
                        if (method) add_class_entry(ct, CE_METHOD, NULL, method);
                    }

                    /* Process parameters to check for errors FIRST (before reporting method errors) */
                    if (method) {
                            add_symbol(&method->symbols, "return", ret_type, 0, method_line, method_col);

                            struct node_list *params = md->node->children;
                            params = params->next;  /* Skip sentinel */
                            params = params->next;  /* Skip return type */
                            params = params->next;  /* Skip method name identifier */
                            if (params && params->node && params->node->category == N_MethodParams) {
                                struct node_list *pc = params->node->children;
                                if (pc) pc = pc->next;  /* Skip sentinel */
                                for (; pc; pc = pc->next) {
                                    if (pc->node && pc->node->category == N_ParamDecl) {
                                        struct node_list *pd = pc->node->children;
                                        if (pd) pd = pd->next;  /* Skip sentinel */
                                        if (pd && pd->node && pd->next && pd->next->node) {
                                            JType ptype = extract_param_type(pd->node);
                                            const char *pname = pd->next->node->token;
                                            int pline = pd->next->node->line;
                                            int pcol = pd->next->node->col;

                                            if (is_reserved_id(pname)) {
                                                (semantic_errors++, printf("Line %d, col %d: Symbol %s is reserved\n",
                                                       pline, pcol, pname));
                                            } else if (find_symbol(method->symbols, pname)) {
                                                (semantic_errors++, printf("Line %d, col %d: Symbol %s already defined\n",
                                                       pline, pcol, pname));
                                            } else {
                                                add_symbol(&method->symbols, pname, ptype, 1,
                                                           pline, pcol);
                                            }
                                        }
                                    }
                                }
                            }
                        } else if (reserved_method || is_duplicate) {
                            /* Process params with temp list to detect duplicate param names even if method is reserved/duplicate */
                            Symbol *temp_symbols = NULL;
                            add_symbol(&temp_symbols, "return", ret_type, 0, method_line, method_col);

                            struct node_list *params = md->node->children;
                            params = params->next;  /* Skip sentinel */
                            params = params->next;  /* Skip return type */
                            params = params->next;  /* Skip method name identifier */
                            if (params && params->node && params->node->category == N_MethodParams) {
                                struct node_list *pc = params->node->children;
                                if (pc) pc = pc->next;  /* Skip sentinel */
                                for (; pc; pc = pc->next) {
                                    if (pc->node && pc->node->category == N_ParamDecl) {
                                        struct node_list *pd = pc->node->children;
                                        if (pd) pd = pd->next;  /* Skip sentinel */
                                        if (pd && pd->node && pd->next && pd->next->node) {
                                            JType ptype = extract_param_type(pd->node);
                                            const char *pname = pd->next->node->token;
                                            int pline = pd->next->node->line;
                                            int pcol = pd->next->node->col;
                                            if (is_reserved_id(pname)) {
                                                (semantic_errors++, printf("Line %d, col %d: Symbol %s is reserved\n", pline, pcol, pname));
                                            } else if (find_symbol(temp_symbols, pname)) {
                                                (semantic_errors++, printf("Line %d, col %d: Symbol %s already defined\n", pline, pcol, pname));
                                            } else {
                                                add_symbol(&temp_symbols, pname, ptype, 1, pline, pcol);
                                            }
                                        }
                                    }
                                }
                            }

                            /* Free temp symbols */
                            while (temp_symbols) {
                                Symbol *tmp = temp_symbols;
                                temp_symbols = temp_symbols->next;
                                free(tmp->name); free(tmp);
                            }

                            /* Report method errors (reserved first, then duplicate only if not reserved) */
                            if (reserved_method) {
                                (semantic_errors++, printf("Line %d, col %d: Symbol %s is reserved\n",
                                       method_line, method_col, method_name));
                            } else if (is_duplicate) {
                                /* Specification: duplicate method message must show only the identifier */
                                (semantic_errors++, printf("Line %d, col %d: Symbol %s already defined\n",
                                       method_line, method_col, method_name));
                            }
                        }

                    if (param_types) free(param_types);
                }
            }
        }
    }

    return ct;
}

void print_symbol_tables(ClassTable *ct)
{
    if (!ct || !ct->name) return;

    printf("===== Class %s Symbol Table =====\n", ct->name);
    for (ClassEntryNode *e = ct->entries; e; e = e->next) {
        if (e->kind == CE_FIELD) {
            printf("%s\t\t%s\n", e->field->name, jtype_to_string(e->field->type));
        } else {
            MethodEntry *m = e->method;
            printf("%s\t(", m->name);
            for (int i = 0; i < m->n_params; i++) {
                if (i > 0) printf(",");
                printf("%s", jtype_to_string(m->param_types[i]));
            }
            printf(")\t%s\n", jtype_to_string(m->return_type));
        }
    }

    printf("\n");

    for (MethodEntry *m = ct->methods; m; m = m->next) {
        printf("===== Method %s(", m->name);
        for (int i = 0; i < m->n_params; i++) {
            if (i > 0) printf(",");
            printf("%s", jtype_to_string(m->param_types[i]));
        }
        printf(") Symbol Table =====\n");

        for (Symbol *s = m->symbols; s; s = s->next) {
            printf("%s\t\t%s", s->name, jtype_to_string(s->type));
            if (s->is_param) printf("\tparam");
            printf("\n");
        }

        printf("\n");
    }
}

void free_class_table(ClassTable *ct)
{
    if (!ct) return;

    Symbol *f = ct->fields;
    while (f) {
        Symbol *tmp = f;
        f = f->next;
        free(tmp->name);
        free(tmp);
    }

    MethodEntry *m = ct->methods;
    while (m) {
        MethodEntry *tmp = m;
        m = m->next;

        Symbol *s = tmp->symbols;
        while (s) {
            Symbol *stmp = s;
            s = s->next;
            free(stmp->name);
            free(stmp);
        }

        if (tmp->param_types) free(tmp->param_types);
        free(tmp->name);
        free(tmp);
    }

    ClassEntryNode *en = ct->entries;
    while (en) {
        ClassEntryNode *tmp = en;
        en = en->next;
        free(tmp);
    }

    if (ct->name) free(ct->name);
    free(ct);
}

static Symbol *lookup_symbol(const char *name, MethodEntry *method, ClassTable *ct)
{
    if (!method || !ct) return NULL;
    if (find_symbol(method->symbols, name)) return find_symbol(method->symbols, name);
    if (find_symbol(ct->fields, name)) return find_symbol(ct->fields, name);
    return NULL;
}

static int types_compatible(JType actual, JType expected)
{
    if (actual == expected) return 1;
    if (actual == JT_INT && expected == JT_DOUBLE) return 1;
    return 0;
}

static JType infer_type(struct node *n, MethodEntry *method, ClassTable *ct);

static JType check_binary_op(struct node *n, MethodEntry *method, ClassTable *ct)
{
    struct node_list *c = n->children;
    if (c) c = c->next;

    struct node *left = c && c->node ? c->node : NULL;
    struct node *right = c && c->next && c->next->node ? c->next->node : NULL;

    JType left_type = left ? infer_type(left, method, ct) : JT_UNDEF;
    JType right_type = right ? infer_type(right, method, ct) : JT_UNDEF;

    char left_str[32], right_str[32];
    strcpy(left_str, jtype_to_string(left_type));
    strcpy(right_str, jtype_to_string(right_type));

    switch (n->category) {
        case N_Add: case N_Sub: case N_Mul: case N_Div: case N_Mod:
            if ((left_type == JT_INT || left_type == JT_DOUBLE) &&
                (right_type == JT_INT || right_type == JT_DOUBLE)) {
                return (left_type == JT_DOUBLE || right_type == JT_DOUBLE) ? JT_DOUBLE : JT_INT;
            }
            (semantic_errors++, printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_Add ? "+" :
                            n->category == N_Sub ? "-" :
                            n->category == N_Mul ? "*" :
                            n->category == N_Div ? "/" : "%", left_str, right_str));
            return JT_UNDEF;

        case N_Lt: case N_Gt: case N_Le: case N_Ge:
            if ((left_type == JT_INT || left_type == JT_DOUBLE) &&
                (right_type == JT_INT || right_type == JT_DOUBLE)) {
                return JT_BOOLEAN;
            }
            (semantic_errors++, printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_Lt ? "<" :
                            n->category == N_Gt ? ">" :
                            n->category == N_Le ? "<=" : ">=", left_str, right_str));
            return JT_BOOLEAN;

        case N_Eq: case N_Ne: {
            /* == and != only work on primitive types (int, double, boolean), not arrays */
            int valid = 0;
            if (left_type != JT_UNDEF && right_type != JT_UNDEF &&
                left_type != JT_VOID && right_type != JT_VOID &&
                left_type != JT_STRING_ARRAY && right_type != JT_STRING_ARRAY) {
                /* Check if types are compatible (same or widening) */
                if (left_type == right_type) {
                    valid = 1;
                } else if ((left_type == JT_INT && right_type == JT_DOUBLE) ||
                           (left_type == JT_DOUBLE && right_type == JT_INT)) {
                    valid = 1;
                }
            }
            if (!valid) {
                (semantic_errors++, printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                       n->line, n->col, n->category == N_Eq ? "==" : "!=", left_str, right_str));
            }
            return JT_BOOLEAN;
        }

        case N_And: case N_Or:
            if (left_type == JT_BOOLEAN && right_type == JT_BOOLEAN) {
                return JT_BOOLEAN;
            }
            (semantic_errors++, printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_And ? "&&" : "||", left_str, right_str));
            return JT_BOOLEAN;

        case N_Lshift: case N_Rshift:
            if (left_type == JT_INT && right_type == JT_INT) {
                return JT_INT;
            }
            (semantic_errors++, printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, n->category == N_Lshift ? "<<" : ">>", left_str, right_str));
            return JT_UNDEF;

        case N_Xor:
            /* XOR works on both int and boolean types */
            if (left_type == JT_INT && right_type == JT_INT) {
                return JT_INT;
            }
            if (left_type == JT_BOOLEAN && right_type == JT_BOOLEAN) {
                return JT_BOOLEAN;
            }
            (semantic_errors++, printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
                   n->line, n->col, "^", left_str, right_str));
            return JT_UNDEF;

        default:
            return JT_UNDEF;
    }
}

static JType check_unary_op(struct node *n, MethodEntry *method, ClassTable *ct)
{
    struct node_list *c = n->children;
    if (c) c = c->next;
    struct node *operand = c && c->node ? c->node : NULL;

    JType op_type = operand ? infer_type(operand, method, ct) : JT_UNDEF;
    char op_str[32];
    strcpy(op_str, jtype_to_string(op_type));

    switch (n->category) {
        case N_Not:
            if (op_type == JT_BOOLEAN) return JT_BOOLEAN;
            (semantic_errors++, printf("Line %d, col %d: Operator ! cannot be applied to type %s\n",
                   n->line, n->col, op_str));
            return JT_BOOLEAN;

        case N_Minus: case N_Plus:
            if (op_type == JT_INT || op_type == JT_DOUBLE) return op_type;
            (semantic_errors++, printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",
                   n->line, n->col, n->category == N_Minus ? "-" : "+", op_str));
            return JT_UNDEF;

        case N_Length:
            if (op_type == JT_STRING_ARRAY) return JT_INT;
            (semantic_errors++, printf("Line %d, col %d: Operator .length cannot be applied to type %s\n",
                   n->line, n->col, op_str));
            return JT_INT;

        default:
            return JT_UNDEF;
    }
}

static JType infer_type(struct node *n, MethodEntry *method, ClassTable *ct)
{
    if (!n) return JT_UNDEF;

    switch (n->category) {
        case N_Natural: {
            /* Check bounds for natural number */
            if (n->token && !check_natural_bounds(n->token)) {
                (semantic_errors++, printf("Line %d, col %d: Number %s out of bounds\n", n->line, n->col, n->token));
            }
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "int");
            return JT_INT;
        }

        case N_Decimal: {
            /* Check bounds for decimal number */
            if (n->token && !check_decimal_bounds(n->token)) {
                (semantic_errors++, printf("Line %d, col %d: Number %s out of bounds\n", n->line, n->col, n->token));
            }
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "double");
            return JT_DOUBLE;
        }

        case N_BoolLit:
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "boolean");
            return JT_BOOLEAN;

        case N_StrLit:
            n->type_annot = malloc(8);
            strcpy(n->type_annot, "String");
            return JT_STRING;

        case N_Identifier: {
            if (is_reserved_id(n->token)) {
                (semantic_errors++, printf("Line %d, col %d: Symbol %s is reserved\n", n->line, n->col, n->token));
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                return JT_UNDEF;
            }
            Symbol *s = lookup_symbol(n->token, method, ct);
            if (s) {
                n->type_annot = malloc(16);
                strcpy(n->type_annot, jtype_to_string(s->type));
                return s->type;
            } else {
                (semantic_errors++, printf("Line %d, col %d: Cannot find symbol %s\n", n->line, n->col, n->token));
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                return JT_UNDEF;
            }
        }

        case N_Add: case N_Sub: case N_Mul: case N_Div: case N_Mod:
        case N_Lt: case N_Gt: case N_Le: case N_Ge:
        case N_Eq: case N_Ne: case N_And: case N_Or:
        case N_Lshift: case N_Rshift: case N_Xor: {
            JType result = check_binary_op(n, method, ct);
            n->type_annot = malloc(16);
            strcpy(n->type_annot, jtype_to_string(result));
            return result;
        }

        case N_Not: case N_Minus: case N_Plus: case N_Length: {
            JType result = check_unary_op(n, method, ct);
            n->type_annot = malloc(16);
            strcpy(n->type_annot, jtype_to_string(result));
            return result;
        }

        case N_Call: {
            struct node_list *c = n->children;
            if (c) c = c->next;  /* Skip sentinel */

            struct node *method_id = c && c->node ? c->node : NULL;
            char *method_name = method_id ? method_id->token : NULL;

            /* Check for reserved method name */
            if (method_name && is_reserved_id(method_name)) {
                (semantic_errors++, printf("Line %d, col %d: Symbol %s is reserved\n",
                       n->line, n->col, method_name));
                if (method_id) {
                    method_id->type_annot = malloc(8);
                    strcpy(method_id->type_annot, "undef");
                }
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                /* Still need to infer types of arguments for annotation */
                c = c && c->next ? c->next : NULL;
                if (c && !c->node) c = c->next;
                for (struct node_list *args = c; args; args = args->next) {
                    if (args->node) infer_type(args->node, method, ct);
                }
                return JT_UNDEF;
            }

            c = c && c->next ? c->next : NULL;
            if (c && !c->node) c = c->next;  /* Skip NULL from call_args sentinel if present */

            int n_args = 0;
            JType *arg_types = NULL;

            /* Count arguments (skip NULL nodes from call_args sentinel) */
            for (struct node_list *tmp = c; tmp; tmp = tmp->next) {
                if (tmp->node) n_args++;
            }

            if (n_args > 0) {
                arg_types = malloc(n_args * sizeof(JType));
                int idx = 0;
                for (struct node_list *args = c; args; args = args->next) {
                    if (args->node) arg_types[idx++] = infer_type(args->node, method, ct);
                }
            }

            /* First try exact match (all types must be exactly equal) across ALL methods */
            MethodEntry *called = NULL;
            if (n_args >= 0) {
                called = find_exact_method(ct->methods, method_name, n_args, arg_types);
            }

            /* If no exact match, find all compatible methods */
            int ambiguous = 0;
            if (!called) {
                int match_count = 0;
                called = find_compatible_method(ct->methods, method_name, n_args, arg_types, &match_count);

                /* Check for ambiguity */
                if (match_count > 1) {
                    (semantic_errors++, printf("Line %d, col %d: Reference to method %s(", n->line, n->col, method_name ? method_name : ""));
                    for (int i = 0; i < n_args; i++) {
                        if (i > 0) printf(",");
                        printf("%s", jtype_to_string(arg_types[i]));
                    }
                    printf(") is ambiguous\n");
                    ambiguous = 1;
                    called = NULL;
                } else if (match_count == 0) {
                    called = NULL;
                }
            }

            if (called) {
                if (method_id) {
                    /* Format method signature for identifier annotation (dynamic allocation) */
                    int sig_len = 2; /* "(" + ")" */
                    for (int i = 0; i < called->n_params; i++) {
                        if (i > 0) sig_len++; /* comma */
                        sig_len += (int)strlen(jtype_to_string(called->param_types[i]));
                    }
                    char *sig = malloc(sig_len + 1);
                    sig[0] = '(';
                    sig[1] = '\0';
                    for (int i = 0; i < called->n_params; i++) {
                        if (i > 0) strcat(sig, ",");
                        strcat(sig, jtype_to_string(called->param_types[i]));
                    }
                    strcat(sig, ")");
                    method_id->type_annot = sig;
                }
                n->type_annot = malloc(16);
                strcpy(n->type_annot, jtype_to_string(called->return_type));
                if (arg_types) free(arg_types);
                return called->return_type;
            } else if (!ambiguous) {
                (semantic_errors++, printf("Line %d, col %d: Cannot find symbol %s(", n->line, n->col, method_name ? method_name : ""));
                for (int i = 0; i < n_args; i++) {
                    if (i > 0) printf(",");
                    printf("%s", jtype_to_string(arg_types[i]));
                }
                printf(")\n");
                if (method_id) {
                    method_id->type_annot = malloc(8);
                    strcpy(method_id->type_annot, "undef");
                }
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                if (arg_types) free(arg_types);
                return JT_UNDEF;
            } else {
                /* Ambiguous call - set undef type but don't print error (already printed) */
                if (method_id) {
                    method_id->type_annot = malloc(8);
                    strcpy(method_id->type_annot, "undef");
                }
                n->type_annot = malloc(8);
                strcpy(n->type_annot, "undef");
                if (arg_types) free(arg_types);
                return JT_UNDEF;
            }
        }

        case N_ParseArgs: {
            struct node_list *c = n->children;
            if (c) c = c->next;

            JType arg1_type = JT_UNDEF;
            JType arg2_type = JT_UNDEF;

            if (c && c->node) {
                arg1_type = infer_type(c->node, method, ct);
            }
            c = c && c->next ? c->next : NULL;
            if (c && c->node) {
                arg2_type = infer_type(c->node, method, ct);
            }

            /* Check argument types: first should be String[], second should be int */
            if (arg1_type != JT_STRING_ARRAY || arg2_type != JT_INT) {
                (semantic_errors++, printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n",
                       n->line, n->col, jtype_to_string(arg1_type), jtype_to_string(arg2_type)));
            }

            n->type_annot = malloc(8);
            strcpy(n->type_annot, "int");
            return JT_INT;
        }

        case N_Assign: {
            struct node_list *c = n->children;
            if (c) c = c->next;
            struct node *lhs = c && c->node ? c->node : NULL;
            struct node *rhs = c && c->next && c->next->node ? c->next->node : NULL;

            JType lhs_type = lhs ? infer_type(lhs, method, ct) : JT_UNDEF;
            JType rhs_type = rhs ? infer_type(rhs, method, ct) : JT_UNDEF;

            /* Check if lhs is a String[] parameter (arrays cannot be reassigned) */
            int is_param_assign = 0;
            if (lhs && lhs->category == N_Identifier) {
                Symbol *lhs_sym = lookup_symbol(lhs->token, method, ct);
                if (lhs_sym && lhs_sym->is_param && lhs_sym->type == JT_STRING_ARRAY) {
                    is_param_assign = 1;
                    (semantic_errors++, printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n",
                           n->line, n->col, jtype_to_string(lhs_type), jtype_to_string(rhs_type)));
                }
            }

            /* Report error if either operand is undef or types are incompatible */
            if (!is_param_assign && (lhs_type == JT_UNDEF || rhs_type == JT_UNDEF || !types_compatible(rhs_type, lhs_type))) {
                (semantic_errors++, printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n",
                       n->line, n->col, jtype_to_string(lhs_type), jtype_to_string(rhs_type)));
            }

            n->type_annot = malloc(16);
            strcpy(n->type_annot, jtype_to_string(lhs_type));
            return lhs_type;
        }

        default:
            return JT_UNDEF;
    }
}

static void check_statement(struct node *n, MethodEntry *method, ClassTable *ct)
{
    if (!n) return;

    struct node_list *c;
    switch (n->category) {
        case N_VarDecl: {
            /* Add variable to method's symbol table when encountered */
            c = n->children;
            if (c) c = c->next;  /* Skip sentinel */

            /* Get the type from first child */
            JType vtype = JT_UNDEF;
            if (c && c->node) {
                vtype = node_to_jtype(c->node);
                c = c->next;  /* Move to first identifier */
            }

            /* Process all identifiers in this declaration (handles int a, b, c; ) */
            for (; c && c->node; c = c->next) {
                const char *vname = c->node->token;
                int vline = c->node->line;
                int vcol = c->node->col;

                if (is_reserved_id(vname)) {
                    (semantic_errors++, printf("Line %d, col %d: Symbol %s is reserved\n",
                           vline, vcol, vname));
                } else if (find_symbol(method->symbols, vname)) {
                    (semantic_errors++, printf("Line %d, col %d: Symbol %s already defined\n",
                           vline, vcol, vname));
                } else {
                    add_symbol(&method->symbols, vname, vtype, 0, vline, vcol);
                }
            }
            break;
        }

        case N_If: {
            c = n->children;
            if (c) c = c->next;

            /* Track condition type error to report after body processing */
            JType cond_type = JT_BOOLEAN;
            int cond_line = 0, cond_col = 0;
            int has_type_error = 0;

            if (c && c->node) {
                cond_type = infer_type(c->node, method, ct);
                if (cond_type != JT_BOOLEAN) {
                    cond_line = c->node->line;
                    cond_col = c->node->col;
                    has_type_error = 1;
                }
            }

            /* Process then block */
            c = c && c->next ? c->next : NULL;
            if (c && c->node) check_statement(c->node, method, ct);

            /* Process else block */
            c = c && c->next ? c->next : NULL;
            if (c && c->node) check_statement(c->node, method, ct);

            /* Report type error after processing blocks */
            if (has_type_error) {
                (semantic_errors++, printf("Line %d, col %d: Incompatible type %s in if statement\n",
                       cond_line, cond_col, jtype_to_string(cond_type)));
            }
            break;
        }

        case N_While: {
            c = n->children;
            if (c) c = c->next;

            /* Track condition type error to report after body processing */
            JType cond_type = JT_BOOLEAN;
            int cond_line = 0, cond_col = 0;
            int has_type_error = 0;

            if (c && c->node) {
                cond_type = infer_type(c->node, method, ct);
                if (cond_type != JT_BOOLEAN) {
                    cond_line = c->node->line;
                    cond_col = c->node->col;
                    has_type_error = 1;
                }
            }

            /* Process body */
            c = c && c->next ? c->next : NULL;
            if (c && c->node) check_statement(c->node, method, ct);

            /* Report type error after processing body */
            if (has_type_error) {
                (semantic_errors++, printf("Line %d, col %d: Incompatible type %s in while statement\n",
                       cond_line, cond_col, jtype_to_string(cond_type)));
            }
            break;
        }

        case N_Return:
            c = n->children;
            if (c) c = c->next;
            if (c && c->node) {
                JType ret_type = infer_type(c->node, method, ct);
                if (ret_type == JT_VOID) {
                    /* Returning a void expression is always an error */
                    (semantic_errors++, printf("Line %d, col %d: Incompatible type void in return statement\n",
                           c->node->line, c->node->col));
                } else if (method && !types_compatible(ret_type, method->return_type)) {
                    (semantic_errors++, printf("Line %d, col %d: Incompatible type %s in return statement\n",
                           c->node->line, c->node->col, jtype_to_string(ret_type)));
                }
            } else {
                /* No return value (void return) */
                if (method && method->return_type != JT_VOID) {
                    (semantic_errors++, printf("Line %d, col %d: Incompatible type void in return statement\n",
                           n->line, n->col));
                }
            }
            break;

        case N_Print:
            c = n->children;
            if (c) c = c->next;
            if (c && c->node) {
                JType arg_type = infer_type(c->node, method, ct);
                if (arg_type == JT_UNDEF || arg_type == JT_VOID || arg_type == JT_STRING_ARRAY) {
                    (semantic_errors++, printf("Line %d, col %d: Incompatible type %s in System.out.print statement\n",
                           c->node->line, c->node->col, jtype_to_string(arg_type)));
                }
            }
            break;

        case N_Assign:
        case N_Call:
        case N_ParseArgs:
            infer_type(n, method, ct);
            break;

        case N_Block:
            c = n->children;
            if (c) c = c->next;
            for (; c && c->node; c = c->next) {
                check_statement(c->node, method, ct);
            }
            break;

        default:
            break;
    }
}

void check_and_annotate_ast(struct node *program, ClassTable *ct)
{
    if (!program || !ct) return;

    struct node_list *c = program->children;
    if (c) c = c->next;
    c = c && c->next ? c->next : NULL;

    /* Track which MethodEntry instances have been processed to skip duplicates */
    MethodEntry *processed[256];
    int n_processed = 0;

    for (; c && c->node; c = c->next) {
        if (c->node->category == N_MethodDecl) {
            struct node_list *md = c->node->children;
            if (md) md = md->next;
            if (md && md->node && md->node->category == N_MethodHeader) {
                JType ret_type;
                char *method_name;
                JType *param_types;
                int n_params;

                process_method_header(md->node, &ret_type, &method_name, &param_types, &n_params);
                MethodEntry *method = find_exact_method(ct->methods, method_name, n_params, param_types);

                /* Check if this method was already processed (duplicate) */
                int already_processed = 0;
                if (method) {
                    for (int i = 0; i < n_processed; i++) {
                        if (processed[i] == method) { already_processed = 1; break; }
                    }
                }

                if (method && !already_processed && md->next && md->next->node &&
                    md->next->node->category == N_MethodBody) {
                    if (n_processed < 256) processed[n_processed++] = method;
                    struct node_list *body = md->next->node->children;
                    if (body) body = body->next;  /* Skip sentinel */
                    for (; body && body->node; body = body->next) {
                        check_statement(body->node, method, ct);
                    }
                }

                if (param_types) free(param_types);
            }
        }
    }
    ct->semantic_errors = semantic_errors;
}

```

#### Ficheiro: `Meta4/src/codegen.h`

```c
/*
 * codegen.h - LLVM IR Code Generator for Juc Compiler - Meta 4
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques - 2023211717
 */

#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "ast.h"
#include "semantic.h"

/* Main entry point for code generation */
void codegen_program(struct node *program, ClassTable *ct);

#endif /* _CODEGEN_H */

```

#### Ficheiro: `Meta4/src/codegen.c`

```c
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

```

#### Ficheiro: `Meta4/src/jucompiler.l`

```lex
/*
 * Autores:
 * Simão Tomás Botas Carvalho - 2021223055
 * Teodoro Marques - 2023211717
 */

%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

/* Header gerado por yacc/bison a partir do jucompiler.y.
 * (o teu Makefile deve gerar y.tab.h) */
#include "y.tab.h"

/* Executado antes de cada ação do lexer:
 * guarda a posição inicial do token em syn_* e avança a coluna.
 * Também atualiza yylloc para rastrear localizações Yacc. */
#define YY_USER_ACTION { \
    yylloc.first_line = lex_line; \
    yylloc.first_column = lex_column; \
    syn_line = lex_line; \
    syn_column = lex_column; \
    lex_column += yyleng; \
    yylloc.last_line = lex_line; \
    yylloc.last_column = lex_column; \
}

/* Para tokens com valor (IDENTIFIER, NATURAL, DECIMAL, BOOLLIT, STRLIT, etc.)
 * assume-se que o %union no yacc tem o campo 'lexeme'. */
#define PASS_TOKEN() do { \
    yylval.lexeme = malloc(strlen(yytext) + 1); \
    if (yylval.lexeme) strcpy(yylval.lexeme, yytext); \
} while(0)

int lex_line = 1;
int lex_column = 1;
int syn_line = -1;
int syn_column = -1;
int lex_errs = 0;

int print_tokens = 0;


int comment_start_line = 1;
int comment_start_col = 1;
int string_start_line = 1;
int string_start_col = 1;
char string_buffer[10000];
int buffer_pos = 0;
int string_has_error = 0;

static void sb_append_char(char c) {
    size_t cap = sizeof(string_buffer);
    if (buffer_pos < (int)cap - 1) {
        string_buffer[buffer_pos++] = c;
    } else {
        /* Truncate to avoid writing out of bounds. */
        string_has_error = 1;
    }
}

static void sb_append_two(char a, char b) {
    sb_append_char(a);
    sb_append_char(b);
}

static void print_simple_token(const char *token) {
    if (print_tokens) {
        printf("%s\n", token);
    }
}

static void print_valued_token(const char *token, const char *value) {
    if (print_tokens) {
        printf("%s(%s)\n", token, value);
    }
}

static int check_natural_bounds(const char *text) {
    /* Check if integer literal is within int range (0 to 2147483647) */
    /* Remove underscores for comparison */
    char clean[100];
    int j = 0;
    for (int i = 0; text[i]; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
        }
    }
    clean[j] = '\0';

    /* Check if it's larger than INT_MAX (2147483647) */
    if (j > 10) return 0; /* More than 10 digits, definitely out of bounds */
    if (j == 10 && strcmp(clean, "2147483647") > 0) return 0;
    return 1;
}

static int check_decimal_bounds(const char *text) {
    /* Check if decimal literal is within valid double range */
    /* First, remove underscores (Java allows them, strtod doesn't) */
    char clean[1000];
    int j = 0;
    int has_digits = 0;
    for (int i = 0; text[i] && j < (int)sizeof(clean) - 1; i++) {
        if (text[i] != '_') {
            clean[j++] = text[i];
            if (text[i] >= '0' && text[i] <= '9') has_digits = 1;
        }
    }
    clean[j] = '\0';

    char *endptr;
    double val = strtod(clean, &endptr);

    /* strtod returns HUGE_VAL on overflow */
    if (val == HUGE_VAL || val == -HUGE_VAL) return 0;

    /* Check for underflow: if we have significant digits but got 0, it underflowed */
    if (val == 0.0 && has_digits) {
        /* Need to check if original string was non-zero */
        /* If string contains non-zero digits and converted to 0, it's underflow */
        for (int i = 0; clean[i]; i++) {
            if (clean[i] >= '1' && clean[i] <= '9') return 0;  /* Non-zero digit found, underflowed */
        }
    }

    return 1;
}

%}

%option noyywrap noinput nounput never-interactive

digit [0-9]
nonzero [1-9]
underscore _
character [a-zA-Z_$]
digitseq {digit}({digit}|{underscore})*{digit}|{digit}
%X COMMENT
%X STR
%%


"/*" { comment_start_line = syn_line; comment_start_col = syn_column; BEGIN(COMMENT); }

<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\r\n { lex_line++; lex_column = 1; }
<COMMENT>\n { lex_line++; lex_column = 1; }
<COMMENT>\r { lex_line++; lex_column = 1; }
<COMMENT>[^*/\r\n]+ { ; }
<COMMENT>[*/] { ; }
<COMMENT><<EOF>> {
    printf("Line %d, col %d: unterminated comment\n", comment_start_line, comment_start_col);
    syn_line = lex_line;
    syn_column = lex_column;
    BEGIN(INITIAL);
    return 0;
}

"//"[^\r\n]* { ; }

"\"" { BEGIN(STR); string_start_line = syn_line; string_start_col = syn_column; buffer_pos = 0; string_has_error = 0; }

<STR>\" {
    char *quoted;
    size_t len;
    size_t cap = sizeof(string_buffer);
    if (buffer_pos < 0) buffer_pos = 0;
    if ((size_t)buffer_pos >= cap) buffer_pos = (int)cap - 1;
    string_buffer[buffer_pos] = '\0';
    if (print_tokens && !string_has_error) printf("STRLIT(\"%s\")\n", string_buffer);
    if (string_has_error) {
        BEGIN(INITIAL);
        return yylex();
    }
    len = strlen(string_buffer);
    quoted = (char *)malloc(len + 3);
    if (quoted) {
        quoted[0] = '"';
        memcpy(quoted + 1, string_buffer, len);
        quoted[len + 1] = '"';
        quoted[len + 2] = '\0';
        yylval.lexeme = quoted;
    } else {
        yylval.lexeme = malloc(3);
        if (yylval.lexeme) strcpy(yylval.lexeme, "\"\"");
    }
    syn_line = string_start_line;
    syn_column = string_start_col;
    BEGIN(INITIAL);
    return STRLIT;
}

<STR>\\[fnrt\\\"] { 
    sb_append_two('\\', yytext[1]);
}

<STR>\\\r\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\\r {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\. { 
    printf("Line %d, col %d: invalid escape sequence (%s)\n", syn_line, syn_column, yytext);
    lex_errs++;
    string_has_error = 1;
}

<STR>\r\n|\r|\n {
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    syn_line = lex_line;
    syn_column = lex_column;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR><<EOF>> { 
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_errs++;
    syn_line = lex_line;
    syn_column = lex_column;
    BEGIN(INITIAL);
    return 0;
}

<STR>. {
    sb_append_char(yytext[0]);
}


[ \t\f]+ { ; }

\r\n { lex_line++; lex_column = 1; }
\n   { lex_line++; lex_column = 1; }
\r   { lex_line++; lex_column = 1; }


"true"|"false" { PASS_TOKEN(); print_valued_token("BOOLLIT", yytext); return BOOLLIT; }
"==" { print_simple_token("EQ"); return EQ; }
"!=" { print_simple_token("NE"); return NE; }
">=" { print_simple_token("GE"); return GE; }
"<=" { print_simple_token("LE"); return LE; }
"<<" { print_simple_token("LSHIFT"); return LSHIFT; }
">>" { print_simple_token("RSHIFT"); return RSHIFT; }
"->" { print_simple_token("ARROW"); return ARROW; }
"&&" { print_simple_token("AND"); return AND; }
"||" { print_simple_token("OR"); return OR; }
">" { print_simple_token("GT"); return GT; }
"<" { print_simple_token("LT"); return LT; }
"=" { print_simple_token("ASSIGN"); return ASSIGN; }
"!" { print_simple_token("NOT"); return NOT; }
"+" { print_simple_token("PLUS"); return PLUS; }
"-" { print_simple_token("MINUS"); return MINUS; }
"*" { print_simple_token("STAR"); return STAR; }
"/" { print_simple_token("DIV"); return DIV; }
"%" { print_simple_token("MOD"); return MOD; }
"^" { print_simple_token("XOR"); return XOR; }
"," { print_simple_token("COMMA"); return COMMA; }
";" { print_simple_token("SEMICOLON"); return SEMICOLON; }
"(" { print_simple_token("LPAR"); return LPAR; }
")" { print_simple_token("RPAR"); return RPAR; }
"{" { print_simple_token("LBRACE"); return LBRACE; }
"}" { print_simple_token("RBRACE"); return RBRACE; }
"[" { print_simple_token("LSQ"); return LSQ; }
"]" { print_simple_token("RSQ"); return RSQ; }



"boolean" { print_simple_token("BOOL"); return BOOL; }
"class" { print_simple_token("CLASS"); return CLASS; }
".length" { print_simple_token("DOTLENGTH"); return DOTLENGTH; }
"double" { print_simple_token("DOUBLE"); return DOUBLE; }
"else" { print_simple_token("ELSE"); return ELSE; }
"if" { print_simple_token("IF"); return IF; }
"int" { print_simple_token("INT"); return INT; }
"System.out.print" { print_simple_token("PRINT"); return PRINT; }
"Integer.parseInt" { print_simple_token("PARSEINT"); return PARSEINT; }
"public" { print_simple_token("PUBLIC"); return PUBLIC; }
"return" { print_simple_token("RETURN"); return RETURN; }
"static" { print_simple_token("STATIC"); return STATIC; }
"String" { print_simple_token("STRING"); return STRING; }
"void" { print_simple_token("VOID"); return VOID; }
"while" { print_simple_token("WHILE"); return WHILE; }

"++" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"--" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"null" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"Integer" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"System" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"abstract" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"assert" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"break" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"byte" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"case" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"catch" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"char" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"const" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"continue" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"default" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"do" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"enum" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"extends" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"final" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"finally" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"float" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"for" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"goto" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"implements" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"import" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"instanceof" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"interface" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"long" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"native" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"new" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"package" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"private" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"protected" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"short" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"strictfp" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"super" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"switch" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"synchronized" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"this" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"throw" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"throws" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"transient" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"try" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }
"volatile" { PASS_TOKEN(); print_valued_token("RESERVED", yytext); return RESERVED; }


{digitseq}[eE][+-]?{digitseq} {
    PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL;
}
{digitseq}?"."{digitseq}([eE][+-]?{digitseq})? {
    PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL;
}
{digitseq}"."([eE][+-]?{digitseq})? {
    PASS_TOKEN(); print_valued_token("DECIMAL", yytext); return DECIMAL;
}

{character}+({digit}|{character})* { PASS_TOKEN(); print_valued_token("IDENTIFIER", yytext); return IDENTIFIER; }
0|{nonzero}({digit}|{underscore})*{digit}|{nonzero} {
    PASS_TOKEN(); print_valued_token("NATURAL", yytext); return NATURAL;
}

. { lex_errs++; printf("Line %d, col %d: illegal character (%s)\n", syn_line, syn_column, yytext); }

<INITIAL><<EOF>> {
    syn_line = lex_line;
    syn_column = lex_column;
    return 0;
}

%%


```

#### Ficheiro: `Meta4/src/jucompiler.y`

```c
/*
 * Autores:
 *   Simão Tomás Botas Carvalho - 2021223055
 *   Teodoro Marques          - 2023211717
 *
 * Meta 3 -- Semantic Analysis (Phase 1: Symbol Table)
 */

%locations

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic.h"
#include "codegen.h"

#define YYDEBUG 0

int  yylex(void);
int  yyparse(void);
void yyerror(const char *s);
void printast(struct node *root);

extern int  lex_line, lex_column;
extern int  syn_line, syn_column;
extern int  lex_errs;
extern char *yytext;
extern int  yychar;
extern int  print_tokens;

struct node *ast      = NULL;
int          syn_errs = 0;
ClassTable  *class_table = NULL;

static struct node *cur_type_node;
static struct node *cur_field_type;
static struct node *prog_root;
static struct node *vd_accum;
static char        *vd_first_id;
static char        *class_id;

static struct node *make_block(struct node *sl);
static struct node *type_node(enum category cat, int line, int col)
{
    return newnode(cat, NULL, line, col);
}
static void add_field(struct node *tn, char *id, int line, int col);
static void add_vardecl(struct node *acc, struct node *tn, char *id, int line, int col);

static struct node *err_cond_placeholder(void)
{
    return newnode(N_Block, NULL, 0, 0);
}
%}

%union {
    char        *lexeme;
    struct node *n;
}

%token BOOL CLASS DOTLENGTH DOUBLE ELSE IF INT PRINT PARSEINT
       PUBLIC RETURN STATIC STRING VOID WHILE
%token EQ NE GE LE LSHIFT RSHIFT ARROW AND OR GT LT ASSIGN NOT
%token PLUS MINUS STAR DIV MOD XOR
%token COMMA SEMICOLON LPAR RPAR LBRACE RBRACE LSQ RSQ

%token <lexeme> IDENTIFIER NATURAL DECIMAL STRLIT BOOLLIT RESERVED

%type <n> class_members method_decl method_header method_body
%type <n> type formal_params fp_list
%type <n> stmt_list method_stmt_list var_decl
%type <n> stmt stmt_no_if
%type <n> expr print_arg opt_expr block_stmt op_expr
%type <n> call_args nonempty_call_args method_invocation assignment_expr parse_args_stmt

%nonassoc IFX
%nonassoc ELSE

%right ASSIGN
%left OR
%left AND
%left XOR
%left EQ NE
%left LT GT LE GE
%left LSHIFT RSHIFT
%left PLUS MINUS
%left STAR DIV MOD

%nonassoc UPLUS UMINUS UNOT

%%

program:
      CLASS IDENTIFIER { class_id = $2; } LBRACE class_members RBRACE
        { ast = $5; }
    ;

class_members:
      /* vazio */
        {
            $$ = newnode(N_Program, NULL, @$.first_line, @$.first_column);
            addchild($$, newnode(N_Identifier, class_id, @$.first_line, @$.first_column));
            prog_root  = $$;
            vd_accum   = NULL;
        }
    | class_members PUBLIC STATIC field_decl
        { $$ = $1; }
    | class_members PUBLIC STATIC method_decl
        { $$ = $1; addchild($1, $4); }
    | class_members SEMICOLON
        { $$ = $1; }
    | class_members error SEMICOLON
        { $$ = $1; }
    ;

field_decl:
      type IDENTIFIER
        {
            cur_field_type = $1;
            add_field($1, $2, @2.first_line, @2.first_column);
        }
      field_id_list SEMICOLON
    | error SEMICOLON
        { }
    ;

field_id_list:
      /* vazio */
    | field_id_list COMMA IDENTIFIER
        {
            add_field(cur_field_type, $3, @3.first_line, @3.first_column);
        }
    ;

method_decl:
      method_header method_body
        {
            $$ = newnode(N_MethodDecl, NULL, @$.first_line, @$.first_column);
            addchild($$, $1);
            addchild($$, $2);
        }
    ;

method_header:
      type IDENTIFIER LPAR formal_params RPAR
        {
            $$ = newnode(N_MethodHeader, NULL, @$.first_line, @$.first_column);
            addchild($$, $1);
            addchild($$, newnode(N_Identifier, $2, @2.first_line, @2.first_column));
            addchild($$, $4);
        }
    | VOID IDENTIFIER LPAR formal_params RPAR
        {
            $$ = newnode(N_MethodHeader, NULL, @$.first_line, @$.first_column);
            addchild($$, type_node(N_Void, @1.first_line, @1.first_column));
            addchild($$, newnode(N_Identifier, $2, @2.first_line, @2.first_column));
            addchild($$, $4);
        }
    ;

formal_params:
      fp_list
    | STRING LSQ RSQ IDENTIFIER
        {
            struct node *pd = newnode(N_ParamDecl, NULL, @$.first_line, @$.first_column);
            addchild(pd, type_node(N_StringArray, @1.first_line, @1.first_column));
            addchild(pd, newnode(N_Identifier, $4, @4.first_line, @4.first_column));
            $$ = newnode(N_MethodParams, NULL, @$.first_line, @$.first_column);
            addchild($$, pd);
        }
    | /* epsilon */
        {
            $$ = newnode(N_MethodParams, NULL, yylloc.first_line, yylloc.first_column);
        }
    ;

fp_list:
      type IDENTIFIER
        {
            struct node *pd = newnode(N_ParamDecl, NULL, @$.first_line, @$.first_column);
            addchild(pd, $1);
            addchild(pd, newnode(N_Identifier, $2, @2.first_line, @2.first_column));
            $$ = newnode(N_MethodParams, NULL, @$.first_line, @$.first_column);
            addchild($$, pd);
        }
    | fp_list COMMA type IDENTIFIER
        {
            struct node *pd = newnode(N_ParamDecl, NULL, @3.first_line, @3.first_column);
            addchild(pd, $3);
            addchild(pd, newnode(N_Identifier, $4, @4.first_line, @4.first_column));
            $$ = $1;
            addchild($$, pd);
        }
    ;

type:
      INT    { $$ = type_node(N_Int,    @1.first_line, @1.first_column); }
    | DOUBLE { $$ = type_node(N_Double, @1.first_line, @1.first_column); }
    | BOOL   { $$ = type_node(N_Bool,   @1.first_line, @1.first_column); }
    ;

method_body:
      LBRACE method_stmt_list RBRACE
        {
            struct node_list *c;
            $$ = newnode(N_MethodBody, NULL, @$.first_line, @$.first_column);
            for (c = $2->children; c; c = c->next)
                if (c->node) addchild($$, c->node);
        }
    ;

var_decl:
      type IDENTIFIER
        {
            cur_type_node = $1;
            vd_first_id = $2;
            vd_accum = newnode(N_MethodBody, NULL, @$.first_line, @$.first_column);
            add_vardecl(vd_accum, cur_type_node, vd_first_id, @2.first_line, @2.first_column);
        }
      vd_id_list SEMICOLON
        { $$ = vd_accum; }
    ;

vd_id_list:
      /* vazio */
    | vd_id_list COMMA IDENTIFIER
        {
            add_vardecl(vd_accum, cur_type_node, $3, @3.first_line, @3.first_column);
        }
    ;

method_stmt_list:
      /* vazio */   { $$ = newnode(N_MethodBody, NULL, yylloc.first_line, yylloc.first_column); }
    | method_stmt_list stmt
        {
            $$ = $1;
            if ($2) addchild($1, $2);
        }
    | method_stmt_list var_decl
        {
            struct node_list *c;
            $$ = $1;
            if ($2) {
                for (c = $2->children; c; c = c->next)
                    if (c->node) addchild($$, c->node);
            }
        }
    ;

stmt_list:
      /* vazio */   { $$ = newnode(N_MethodBody, NULL, yylloc.first_line, yylloc.first_column); }
    | stmt_list stmt
        {
            $$ = $1;
            if ($2) addchild($1, $2);
        }
    ;

stmt:
      stmt_no_if
    | IF LPAR expr RPAR stmt  %prec IFX
        {
            $$ = newnode(N_If, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
            addchild($$, $5 ? $5 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
            addchild($$, newnode(N_Block, NULL, @$.first_line, @$.first_column));
        }
    | IF LPAR expr RPAR stmt ELSE stmt
        {
            $$ = newnode(N_If, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
            addchild($$, $5 ? $5 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
            addchild($$, $7 ? $7 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
        }
    ;

stmt_no_if:
    WHILE LPAR expr RPAR stmt
        {
            $$ = newnode(N_While, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
            addchild($$, $5 ? $5 : newnode(N_Block, NULL, @$.first_line, @$.first_column));
        }
    | PRINT LPAR print_arg RPAR SEMICOLON
        {
            $$ = newnode(N_Print, NULL, @1.first_line, @1.first_column);
            addchild($$, $3);
        }
    | PRINT LPAR error RPAR SEMICOLON
        { $$ = NULL; }
    | PRINT LPAR error SEMICOLON
        { $$ = NULL; }
    | block_stmt
    | method_invocation SEMICOLON
        { $$ = $1; }
    | assignment_expr SEMICOLON
        { $$ = $1; }
    | parse_args_stmt SEMICOLON
        { $$ = $1; }
    | RETURN opt_expr SEMICOLON
        {
            $$ = newnode(N_Return, NULL, @1.first_line, @1.first_column);
            if ($2) addchild($$, $2);
        }
    | RETURN error SEMICOLON
        {
            $$ = newnode(N_Return, NULL, @1.first_line, @1.first_column);
        }
    | error SEMICOLON
        { $$ = NULL; }
    | SEMICOLON
        { $$ = NULL; }
    ;

block_stmt:
      LBRACE stmt_list RBRACE
        {
            struct node *blk = make_block($2);
            /* If block is empty (only sentinel child, no real content), skip it */
            if (blk && blk->category == N_Block && !blk->children->next) {
                $$ = NULL;
            } else {
                $$ = blk;
            }
        }
    ;

method_invocation:
      IDENTIFIER LPAR call_args RPAR
        {
            struct node_list *c;
            $$ = newnode(N_Call, NULL, @1.first_line, @1.first_column);
            addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column));
            for (c = $3->children; c; c = c->next) addchild($$, c->node);
        }
    | IDENTIFIER LPAR error RPAR
        { $$ = NULL; }
    ;

call_args:
      /* vazio */       { $$ = newnode(N_MethodBody, NULL, yylloc.first_line, yylloc.first_column); }
    | nonempty_call_args  { $$ = $1; }
    ;

nonempty_call_args:
      expr              { $$ = newnode(N_MethodBody, NULL, @$.first_line, @$.first_column); addchild($$, $1); }
    | nonempty_call_args COMMA expr
        { $$ = $1; addchild($$, $3); }
    ;

assignment_expr:
      IDENTIFIER ASSIGN expr
        {
            $$ = newnode(N_Assign, NULL, @2.first_line, @2.first_column);
            addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column));
            addchild($$, $3);
        }
    ;

parse_args_stmt:
      PARSEINT LPAR IDENTIFIER LSQ expr RSQ RPAR
        {
            $$ = newnode(N_ParseArgs, NULL, @1.first_line, @1.first_column);
            addchild($$, newnode(N_Identifier, $3, @3.first_line, @3.first_column));
            addchild($$, $5);
        }
    | PARSEINT LPAR IDENTIFIER LSQ error RSQ RPAR
        { $$ = NULL; }
    | PARSEINT LPAR IDENTIFIER LSQ error RPAR
        { $$ = NULL; }
    | PARSEINT LPAR error RPAR
        { $$ = NULL; }
    ;

opt_expr:
      /* vazio */ { $$ = NULL; }
    | expr        { $$ = $1; }
    ;

print_arg:
      expr   { $$ = $1; }
    | STRLIT { $$ = newnode(N_StrLit, $1, @1.first_line, @1.first_column); }
    ;

expr:
      assignment_expr
        { $$ = $1; }
    | op_expr
        { $$ = $1; }
    ;

op_expr:
      NATURAL
        { $$ = newnode(N_Natural, $1, @1.first_line, @1.first_column); }
    | DECIMAL
        { $$ = newnode(N_Decimal, $1, @1.first_line, @1.first_column); }
    | BOOLLIT
        { $$ = newnode(N_BoolLit, $1, @1.first_line, @1.first_column); }
    | LPAR expr RPAR  { $$ = $2; }
    | LPAR error RPAR { $$ = NULL; }
    | IDENTIFIER
        { $$ = newnode(N_Identifier, $1, @1.first_line, @1.first_column); }
    | IDENTIFIER DOTLENGTH
        { $$ = newnode(N_Length, NULL, @2.first_line, @2.first_column);
          addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column)); }
    | IDENTIFIER LPAR call_args RPAR
        {
            struct node_list *c;
            $$ = newnode(N_Call, NULL, @1.first_line, @1.first_column);
            addchild($$, newnode(N_Identifier, $1, @1.first_line, @1.first_column));
            for (c = $3->children; c; c = c->next) addchild($$, c->node);
        }
    | IDENTIFIER LPAR error RPAR
        { $$ = NULL; }
    | PARSEINT LPAR IDENTIFIER LSQ expr RSQ RPAR
        { $$ = newnode(N_ParseArgs, NULL, @1.first_line, @1.first_column);
          addchild($$, newnode(N_Identifier, $3, @3.first_line, @3.first_column));
          addchild($$, $5); }
    | PARSEINT LPAR IDENTIFIER LSQ error RSQ RPAR
        { $$ = NULL; }
    | PARSEINT LPAR error RPAR
        { $$ = NULL; }
    | MINUS op_expr %prec UMINUS
        { $$ = newnode(N_Minus, NULL, @1.first_line, @1.first_column); addchild($$, $2); }
    | PLUS  op_expr %prec UPLUS
        { $$ = newnode(N_Plus,  NULL, @1.first_line, @1.first_column); addchild($$, $2); }
    | NOT   op_expr %prec UNOT
        { $$ = newnode(N_Not,   NULL, @1.first_line, @1.first_column); addchild($$, $2); }
    | op_expr OR     op_expr  { $$ = newnode(N_Or,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr AND    op_expr  { $$ = newnode(N_And,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr XOR    op_expr  { $$ = newnode(N_Xor,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr EQ     op_expr  { $$ = newnode(N_Eq,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr NE     op_expr  { $$ = newnode(N_Ne,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr LT     op_expr  { $$ = newnode(N_Lt,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr GT     op_expr  { $$ = newnode(N_Gt,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr LE     op_expr  { $$ = newnode(N_Le,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr GE     op_expr  { $$ = newnode(N_Ge,     NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr LSHIFT op_expr  { $$ = newnode(N_Lshift, NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr RSHIFT op_expr  { $$ = newnode(N_Rshift, NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr PLUS   op_expr  { $$ = newnode(N_Add,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr MINUS  op_expr  { $$ = newnode(N_Sub,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr STAR   op_expr  { $$ = newnode(N_Mul,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr DIV    op_expr  { $$ = newnode(N_Div,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    | op_expr MOD    op_expr  { $$ = newnode(N_Mod,    NULL, @2.first_line, @2.first_column); addchild($$, $1); addchild($$, $3); }
    ;

%%

static void add_field(struct node *tn, char *id, int line, int col)
{
    struct node *fd = newnode(N_FieldDecl, NULL, line, col);
    addchild(fd, tn);
    addchild(fd, newnode(N_Identifier, id, line, col));
    addchild(prog_root, fd);
}

static void add_vardecl(struct node *acc, struct node *tn, char *id, int line, int col)
{
    struct node *vd = newnode(N_VarDecl, NULL, line, col);
    addchild(vd, tn);
    addchild(vd, newnode(N_Identifier, id, line, col));
    if (acc)
        addchild(acc, vd);
}

const char *category_name[] = {
    "Program", "FieldDecl", "VarDecl", "MethodDecl",
    "MethodHeader", "MethodParams", "ParamDecl", "MethodBody",
    "Block", "If", "While", "Return", "Call", "Print", "ParseArgs", "Assign",
    "Or", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge",
    "Add", "Sub", "Mul", "Div", "Mod",
    "Lshift", "Rshift", "Xor", "Not", "Minus", "Plus", "Length",
    "Int", "Double", "Bool", "StringArray", "Void",
    "Identifier", "Natural", "Decimal", "BoolLit", "StrLit"
};

static struct node *make_block(struct node *sl)
{
    int nc = 0;
    int saw_any = 0;
    struct node *single = NULL;
    struct node_list *c;
    struct node *blk;
    struct node_list *c2;

    if (!sl) return newnode(N_Block, NULL, 0, 0);

    for (c = sl->children; c; c = c->next) {
        if (c->node) {
            int is_empty_block = 0;
            struct node_list *cc;
            saw_any = 1;
            if (c->node->category == N_Block) {
                is_empty_block = 1;
                for (cc = c->node->children; cc; cc = cc->next)
                    if (cc->node) { is_empty_block = 0; break; }
            }
            if (is_empty_block) continue;
            nc++;
            if (nc == 1) single = c->node;
        }
    }
    if (nc == 0)
        return saw_any ? NULL : newnode(N_Block, NULL, 0, 0);
    if (nc == 1) return single;

    blk = newnode(N_Block, NULL, 0, 0);
    for (c2 = sl->children; c2; c2 = c2->next) {
        if (c2->node) {
            int is_empty_block = 0;
            struct node_list *cc;
            if (c2->node->category == N_Block) {
                is_empty_block = 1;
                for (cc = c2->node->children; cc; cc = cc->next)
                    if (cc->node) { is_empty_block = 0; break; }
            }
            if (!is_empty_block) addchild(blk, c2->node);
        }
    }
    return blk;
}

void yyerror(const char *msg)
{
    syn_errs++;
    if (yytext && yytext[0])
        if (yychar == STRLIT && yylval.lexeme)
            printf("Line %d, col %d: syntax error: %s\n",
                   syn_line, syn_column, yylval.lexeme);
        else
            printf("Line %d, col %d: syntax error: %s\n",
                   syn_line, syn_column, yytext);
    else
        printf("Line %d, col %d: syntax error: \n",
               syn_line, syn_column);
}

int main(int argc, char **argv)
{
    int i;
    int parse_status;
    int mode = 6;

    for (i = 1; i < argc; i++) {
        if      (!strcmp(argv[i], "-l"))  mode = 0;
        else if (!strcmp(argv[i], "-e1")) mode = 1;
        else if (!strcmp(argv[i], "-e2")) mode = 2;
        else if (!strcmp(argv[i], "-t"))  mode = 3;
        else if (!strcmp(argv[i], "-s"))  mode = 4;
        else if (!strcmp(argv[i], "-e3")) mode = 5;
    }
    if (mode == 0 || mode == 1) {
        print_tokens = (mode == 0);
        while (yylex() != 0) ;
        return 0;
    }
    print_tokens = 0;
    parse_status = yyparse();
    if (parse_status != 0 && lex_errs > 0) {
        while (yylex() != 0) ;
    }
    if (mode == 3 && ast && syn_errs == 0)
        printast(ast);
    if ((mode == 2 || mode == 4 || mode == 5 || mode == 6) && ast && syn_errs == 0) {
        class_table = build_symbol_tables(ast);
        if (class_table) {
            check_and_annotate_ast(ast, class_table);
            if (mode == 4) {
                print_symbol_tables(class_table);
                printast(ast);
            } else if (mode == 6 && class_table->semantic_errors == 0) {
                codegen_program(ast, class_table);
            }
            free_class_table(class_table);
        }
    }
    return 0;
}

```
