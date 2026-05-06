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
%type <n> stmt_list var_decl
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
      LBRACE stmt_list RBRACE
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

stmt_list:
      /* vazio */   { $$ = newnode(N_MethodBody, NULL, yylloc.first_line, yylloc.first_column); }
    | stmt_list stmt
        {
            $$ = $1;
            if ($2) addchild($1, $2);
        }
    | stmt_list var_decl
        {
            struct node_list *c;
            $$ = $1;
            if ($2) {
                for (c = $2->children; c; c = c->next)
                    if (c->node) addchild($$, c->node);
            }
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
    if (mode == 4 && ast && syn_errs == 0) {
        class_table = build_symbol_tables(ast);
        if (class_table) {
            check_and_annotate_ast(ast, class_table);
            print_symbol_tables(class_table);
            printast(ast);
            free_class_table(class_table);
        }
    }
    return 0;
}
