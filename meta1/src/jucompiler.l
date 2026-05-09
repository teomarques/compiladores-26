/*
 * Autores:
 * Simão Tomás Botas Carvalho - 2021223055
 * Teodoro Marques - 2023211717
 */

%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Tokens independentes (não dependem do yacc) */
enum token_ids {
    BOOLLIT = 256,
    EQ, NE, GE, LE, LSHIFT, RSHIFT, ARROW, AND, OR, GT, LT,
    ASSIGN, NOT, PLUS, MINUS, STAR, DIV, MOD, XOR,
    COMMA, SEMICOLON, LPAR, RPAR, LBRACE, RBRACE, LSQ, RSQ,
    BOOL, CLASS, DOTLENGTH, DOUBLE, ELSE, IF, INT,
    PRINT, PARSEINT, PUBLIC, RETURN, STATIC, STRING, VOID, WHILE,
    IDENTIFIER, NATURAL, DECIMAL, STRLIT, RESERVED
};

/* Guarda a posição inicial do token */
#define YY_USER_ACTION { syn_line = lex_line; syn_column = lex_column; lex_column += yyleng; }

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
        string_has_error = 1;   /* buffer overflow tratado como erro */
    }
}

static void sb_append_two(char a, char b) {
    sb_append_char(a);
    sb_append_char(b);
}

static void print_simple_token(const char *token) {
    if (print_tokens)
        printf("%s\n", token);
}

static void print_valued_token(const char *token, const char *value) {
    if (print_tokens)
        printf("%s(%s)\n", token, value);
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

"/*" {
    comment_start_line = syn_line;
    comment_start_col = syn_column;
    BEGIN(COMMENT);
}

<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\r\n { lex_line++; lex_column = 1; }
<COMMENT>\n   { lex_line++; lex_column = 1; }
<COMMENT>\r   { lex_line++; lex_column = 1; }
<COMMENT>[^*/\r\n]+ { ; }
<COMMENT>[*/] { ; }
<COMMENT><<EOF>> {
    printf("Line %d, col %d: unterminated comment\n",
           comment_start_line, comment_start_col);
    lex_errs++;
    syn_line = lex_line;
    syn_column = lex_column;
    BEGIN(INITIAL);
    return 0;
}

"//"[^\r\n]* { ; }

"\"" {
    BEGIN(STR);
    string_start_line = syn_line;
    string_start_col = syn_column;
    buffer_pos = 0;
    string_has_error = 0;
}

<STR>\" {
    string_buffer[buffer_pos] = '\0';
    if (print_tokens && !string_has_error)
        printf("STRLIT(\"%s\")\n", string_buffer);
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
    sb_append_two('\\', yytext[1]);   /* preserva a sequência no token */
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

"true"|"false"   { print_valued_token("BOOLLIT", yytext); return BOOLLIT; }
"==" { print_simple_token("EQ"); return EQ; }
"!=" { print_simple_token("NE"); return NE; }
">=" { print_simple_token("GE"); return GE; }
"<=" { print_simple_token("LE"); return LE; }
"<<" { print_simple_token("LSHIFT"); return LSHIFT; }
">>" { print_simple_token("RSHIFT"); return RSHIFT; }
"->" { print_simple_token("ARROW"); return ARROW; }
"&&" { print_simple_token("AND"); return AND; }
"||" { print_simple_token("OR"); return OR; }
">"  { print_simple_token("GT"); return GT; }
"<"  { print_simple_token("LT"); return LT; }
"="  { print_simple_token("ASSIGN"); return ASSIGN; }
"!"  { print_simple_token("NOT"); return NOT; }
"+"  { print_simple_token("PLUS"); return PLUS; }
"-"  { print_simple_token("MINUS"); return MINUS; }
"*"  { print_simple_token("STAR"); return STAR; }
"/"  { print_simple_token("DIV"); return DIV; }
"%"  { print_simple_token("MOD"); return MOD; }
"^"  { print_simple_token("XOR"); return XOR; }
","  { print_simple_token("COMMA"); return COMMA; }
";"  { print_simple_token("SEMICOLON"); return SEMICOLON; }
"("  { print_simple_token("LPAR"); return LPAR; }
")"  { print_simple_token("RPAR"); return RPAR; }
"{"  { print_simple_token("LBRACE"); return LBRACE; }
"}"  { print_simple_token("RBRACE"); return RBRACE; }
"["  { print_simple_token("LSQ"); return LSQ; }
"]"  { print_simple_token("RSQ"); return RSQ; }

"boolean"          { print_simple_token("BOOL"); return BOOL; }
"class"            { print_simple_token("CLASS"); return CLASS; }
".length"          { print_simple_token("DOTLENGTH"); return DOTLENGTH; }
"double"           { print_simple_token("DOUBLE"); return DOUBLE; }
"else"             { print_simple_token("ELSE"); return ELSE; }
"if"               { print_simple_token("IF"); return IF; }
"int"              { print_simple_token("INT"); return INT; }
"System.out.print" { print_simple_token("PRINT"); return PRINT; }
"Integer.parseInt" { print_simple_token("PARSEINT"); return PARSEINT; }
"public"           { print_simple_token("PUBLIC"); return PUBLIC; }
"return"           { print_simple_token("RETURN"); return RETURN; }
"static"           { print_simple_token("STATIC"); return STATIC; }
"String"           { print_simple_token("STRING"); return STRING; }
"void"             { print_simple_token("VOID"); return VOID; }
"while"            { print_simple_token("WHILE"); return WHILE; }

"++"         { print_valued_token("RESERVED", yytext); return RESERVED; }
"--"         { print_valued_token("RESERVED", yytext); return RESERVED; }
"null"       { print_valued_token("RESERVED", yytext); return RESERVED; }
"Integer"    { print_valued_token("RESERVED", yytext); return RESERVED; }
"System"     { print_valued_token("RESERVED", yytext); return RESERVED; }
"abstract"   { print_valued_token("RESERVED", yytext); return RESERVED; }
"assert"     { print_valued_token("RESERVED", yytext); return RESERVED; }
"break"      { print_valued_token("RESERVED", yytext); return RESERVED; }
"byte"       { print_valued_token("RESERVED", yytext); return RESERVED; }
"case"       { print_valued_token("RESERVED", yytext); return RESERVED; }
"catch"      { print_valued_token("RESERVED", yytext); return RESERVED; }
"char"       { print_valued_token("RESERVED", yytext); return RESERVED; }
"const"      { print_valued_token("RESERVED", yytext); return RESERVED; }
"continue"   { print_valued_token("RESERVED", yytext); return RESERVED; }
"default"    { print_valued_token("RESERVED", yytext); return RESERVED; }
"do"         { print_valued_token("RESERVED", yytext); return RESERVED; }
"enum"       { print_valued_token("RESERVED", yytext); return RESERVED; }
"extends"    { print_valued_token("RESERVED", yytext); return RESERVED; }
"final"      { print_valued_token("RESERVED", yytext); return RESERVED; }
"finally"    { print_valued_token("RESERVED", yytext); return RESERVED; }
"float"      { print_valued_token("RESERVED", yytext); return RESERVED; }
"for"        { print_valued_token("RESERVED", yytext); return RESERVED; }
"goto"       { print_valued_token("RESERVED", yytext); return RESERVED; }
"implements" { print_valued_token("RESERVED", yytext); return RESERVED; }
"import"     { print_valued_token("RESERVED", yytext); return RESERVED; }
"instanceof" { print_valued_token("RESERVED", yytext); return RESERVED; }
"interface"  { print_valued_token("RESERVED", yytext); return RESERVED; }
"long"       { print_valued_token("RESERVED", yytext); return RESERVED; }
"native"     { print_valued_token("RESERVED", yytext); return RESERVED; }
"new"        { print_valued_token("RESERVED", yytext); return RESERVED; }
"package"    { print_valued_token("RESERVED", yytext); return RESERVED; }
"private"    { print_valued_token("RESERVED", yytext); return RESERVED; }
"protected"  { print_valued_token("RESERVED", yytext); return RESERVED; }
"short"      { print_valued_token("RESERVED", yytext); return RESERVED; }
"strictfp"   { print_valued_token("RESERVED", yytext); return RESERVED; }
"super"      { print_valued_token("RESERVED", yytext); return RESERVED; }
"switch"     { print_valued_token("RESERVED", yytext); return RESERVED; }
"synchronized" { print_valued_token("RESERVED", yytext); return RESERVED; }
"this"       { print_valued_token("RESERVED", yytext); return RESERVED; }
"throw"      { print_valued_token("RESERVED", yytext); return RESERVED; }
"throws"     { print_valued_token("RESERVED", yytext); return RESERVED; }
"transient"  { print_valued_token("RESERVED", yytext); return RESERVED; }
"try"        { print_valued_token("RESERVED", yytext); return RESERVED; }
"volatile"   { print_valued_token("RESERVED", yytext); return RESERVED; }

{digitseq}[eE][+-]?{digitseq}                         { print_valued_token("DECIMAL", yytext); return DECIMAL; }
{digitseq}?"."{digitseq}([eE][+-]?{digitseq})?         { print_valued_token("DECIMAL", yytext); return DECIMAL; }
{digitseq}"."([eE][+-]?{digitseq})?                    { print_valued_token("DECIMAL", yytext); return DECIMAL; }

{character}+({digit}|{character})*                    { print_valued_token("IDENTIFIER", yytext); return IDENTIFIER; }
0|{nonzero}({digit}|{underscore})*{digit}|{nonzero}    { print_valued_token("NATURAL", yytext); return NATURAL; }

. {
    lex_errs++;
    printf("Line %d, col %d: illegal character (%s)\n", syn_line, syn_column, yytext);
}

<INITIAL><<EOF>> {
    syn_line = lex_line;
    syn_column = lex_column;
    return 0;
}

%%

int main(int argc, char **argv) {
    if (argc >= 2 && strcmp(argv[1], "-l") == 0)
        print_tokens = 1;
    while (yylex())
        ;
    return 0;   /* sempre termina com sucesso, independentemente dos erros léxicos */
}
