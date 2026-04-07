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
#define PASS_TOKEN() yylval.lexeme = strdup(yytext)

int lex_line = 1;
int lex_column = 1;
int syn_line = -1;
int syn_column = -1;

int print_tokens = 0;

extern int lex_error_count;

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

%option noyywrap

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
    lex_error_count++;
    /* Record current scanner position for parser EOF error reporting. */
    syn_line = lex_line;
    syn_column = lex_column;
    yyterminate();
}

"//"[^\r\n]* { ; }

"\"" { BEGIN(STR); string_start_line = syn_line; string_start_col = syn_column; buffer_pos = 0; string_has_error = 0; }

<STR>\" {
    size_t cap = sizeof(string_buffer);
    if (buffer_pos < 0) buffer_pos = 0;
    if ((size_t)buffer_pos >= cap) buffer_pos = (int)cap - 1;
    string_buffer[buffer_pos] = '\0';
    if (print_tokens && !string_has_error) printf("STRLIT(\"%s\")\n", string_buffer);

    if (string_has_error) {
        /* Discard invalid strings: keep lexical error reporting, but don't
           return STRLIT to the parser (matches meta2 expected behavior). */
        BEGIN(INITIAL);
    } else {
        /* For parser error reporting, STRLIT should point to the opening quote. */
        syn_line = string_start_line;
        syn_column = string_start_col;
        yylval.lexeme = strdup(string_buffer);
        BEGIN(INITIAL);
        return STRLIT;
    }
}

<STR>\\[fnrt\\\"] { 
    sb_append_two('\\', yytext[1]);
}

<STR>\\\r\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_error_count += 2;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_error_count += 2;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\\r {
    printf("Line %d, col %d: invalid escape sequence (\\)\n", syn_line, syn_column);
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_error_count += 2;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>\\. { 
    printf("Line %d, col %d: invalid escape sequence (%s)\n", syn_line, syn_column, yytext);
    string_has_error = 1;
    lex_error_count++;
}

<STR>\r\n {
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_error_count++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR>[\n\r] { 
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_error_count++;
    lex_line++;
    lex_column = 1;
    BEGIN(INITIAL);
}

<STR><<EOF>> { 
    printf("Line %d, col %d: unterminated string literal\n", string_start_line, string_start_col);
    lex_error_count++;
    yyterminate();
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
"String[]" {
    /* Parser wants a single token so plain 'String' errors at 'String'.
       Lexer-only mode (-l) historically prints STRING, LSQ, RSQ separately. */
    if (print_tokens) {
        print_simple_token("STRING");
        print_simple_token("LSQ");
        print_simple_token("RSQ");
    }
    return STRINGARRAY;
}
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

. {
    printf("Line %d, col %d: illegal character (%s)\n", syn_line, syn_column, yytext);
    lex_error_count++;
}

<<EOF>> { YY_USER_ACTION; return 0; }

%%

