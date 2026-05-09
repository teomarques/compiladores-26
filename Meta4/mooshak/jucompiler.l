/* Autor:
 * Nome: João Afonso dos Santos Simões - Número: 2022236316
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "y.tab.h"

int line = 1;
int col = 1;
int flag_l = 0;
int flag_e1 = 0;
int flag_t = 0;
int flag_e2 = 0;
int flag_s = 0;
int flag_e3 = 0;

int tok_line, tok_col;
char tok_text[8192] = "";

/* String state */
int str_line, str_col;
char str_buf[4096];
int str_len;
int str_has_error;

/* Comment state */
int comment_line, comment_col;

#define UPDATE_COL()  { tok_line = line; tok_col = col; col += yyleng; strncpy(tok_text, yytext, sizeof(tok_text)-1); tok_text[sizeof(tok_text)-1] = '\0'; }

/* Set yylloc for bison %locations */
#define SET_LOC() { yylloc.first_line = tok_line; yylloc.first_column = tok_col; \
                    yylloc.last_line = tok_line; yylloc.last_column = tok_col + yyleng - 1; }

#define LEX_ONLY_TOKEN(name, tok) { \
    UPDATE_COL(); \
    if (flag_l) { printf(name "\n"); } \
    if (!flag_l && !flag_e1) { SET_LOC(); return tok; } \
}

#define LEX_ONLY_TOKEN_VAL(name, tok) { \
    UPDATE_COL(); \
    if (flag_l) { printf(name "(%s)\n", yytext); } \
    if (!flag_l && !flag_e1) { yylval.str = strdup(yytext); SET_LOC(); return tok; } \
}

#define LEX_ONLY_RESERVED(tok) { \
    UPDATE_COL(); \
    if (flag_l) { printf("RESERVED(%s)\n", yytext); } \
    if (!flag_l && !flag_e1) { SET_LOC(); return tok; } \
}
%}



DIGIT       [0-9]
NONZERODIGIT [1-9]
LETTER      [a-zA-Z_$]
ALPHANUMID  [a-zA-Z0-9_$]
DIGITS      {DIGIT}({DIGIT}|_)*{DIGIT}|{DIGIT}
EXPONENT    [eE][+-]?{DIGITS}

%x COMMENT
%x STRSTATE

%%

"//"[^\r\n]*            { col += yyleng; }

"/*"                    { comment_line = line; comment_col = col; col += 2; BEGIN(COMMENT); }
<COMMENT>"*/"           { col += 2; BEGIN(INITIAL); }
<COMMENT>\n             { line++; col = 1; }
<COMMENT>\r\n           { line++; col = 1; }
<COMMENT>\r             { line++; col = 1; }
<COMMENT>.              { col++; }
<COMMENT><<EOF>>        { printf("Line %d, col %d: unterminated comment\n", comment_line, comment_col); tok_line = line; tok_col = col; tok_text[0] = '\0'; BEGIN(INITIAL); return 0; }

\"                      { str_line = line; str_col = col; col++; str_len = 0; str_has_error = 0; str_buf[0] = '"'; str_len = 1; BEGIN(STRSTATE); }
<STRSTATE>\"            {
                            col++;
                            str_buf[str_len++] = '"';
                            str_buf[str_len] = '\0';
                            tok_line = str_line; tok_col = str_col;
                            if (flag_l && !str_has_error) { printf("STRLIT(%s)\n", str_buf); }
                            if (!flag_l && !flag_e1 && !str_has_error) {
                                yylval.str = strdup(str_buf);
                                yylloc.first_line = str_line; yylloc.first_column = str_col;
                                yylloc.last_line = str_line; yylloc.last_column = col - 1;
                                BEGIN(INITIAL);
                                return STRLIT;
                            }
                            BEGIN(INITIAL);
                        }
<STRSTATE>\\f           { col += 2; str_buf[str_len++] = '\\'; str_buf[str_len++] = 'f'; }
<STRSTATE>\\n           { col += 2; str_buf[str_len++] = '\\'; str_buf[str_len++] = 'n'; }
<STRSTATE>\\r           { col += 2; str_buf[str_len++] = '\\'; str_buf[str_len++] = 'r'; }
<STRSTATE>\\t           { col += 2; str_buf[str_len++] = '\\'; str_buf[str_len++] = 't'; }
<STRSTATE>\\\\          { col += 2; str_buf[str_len++] = '\\'; str_buf[str_len++] = '\\'; }
<STRSTATE>\\\"          { col += 2; str_buf[str_len++] = '\\'; str_buf[str_len++] = '"'; }
<STRSTATE>\\[^\n\r]     { printf("Line %d, col %d: invalid escape sequence (%s)\n", line, col, yytext); col += 2; str_has_error = 1; str_buf[str_len++] = yytext[0]; str_buf[str_len++] = yytext[1]; }
<STRSTATE>\n            { printf("Line %d, col %d: unterminated string literal\n", str_line, str_col); line++; col = 1; BEGIN(INITIAL); }
<STRSTATE>\r\n          { printf("Line %d, col %d: unterminated string literal\n", str_line, str_col); line++; col = 1; BEGIN(INITIAL); }
<STRSTATE>\r            { printf("Line %d, col %d: unterminated string literal\n", str_line, str_col); line++; col = 1; BEGIN(INITIAL); }
<STRSTATE><<EOF>>       { printf("Line %d, col %d: unterminated string literal\n", str_line, str_col); tok_line = line; tok_col = col; tok_text[0] = '\0'; BEGIN(INITIAL); return 0; }
<STRSTATE>.             { col++; str_buf[str_len++] = yytext[0]; }

"boolean"               { LEX_ONLY_TOKEN("BOOL", BOOL); }
"class"                 { LEX_ONLY_TOKEN("CLASS", CLASS); }
"double"                { LEX_ONLY_TOKEN("DOUBLE", DOUBLE); }
"else"                  { LEX_ONLY_TOKEN("ELSE", ELSE); }
"if"                    { LEX_ONLY_TOKEN("IF", IF); }
"int"                   { LEX_ONLY_TOKEN("INT", INT); }
"public"                { LEX_ONLY_TOKEN("PUBLIC", PUBLIC); }
"return"                { LEX_ONLY_TOKEN("RETURN", RETURN); }
"static"                { LEX_ONLY_TOKEN("STATIC", STATIC); }
"void"                  { LEX_ONLY_TOKEN("VOID", VOID); }
"while"                 { LEX_ONLY_TOKEN("WHILE", WHILE); }
"String"                { LEX_ONLY_TOKEN("STRING", STRING); }

"System.out.print"      { LEX_ONLY_TOKEN("PRINT", PRINT); }
"Integer.parseInt"      { LEX_ONLY_TOKEN("PARSEINT", PARSEINT); }
".length"               { LEX_ONLY_TOKEN("DOTLENGTH", DOTLENGTH); }

"true"                  { LEX_ONLY_TOKEN_VAL("BOOLLIT", BOOLLIT); }
"false"                 { LEX_ONLY_TOKEN_VAL("BOOLLIT", BOOLLIT); }

"abstract"              { LEX_ONLY_RESERVED(RESERVED); }
"assert"                { LEX_ONLY_RESERVED(RESERVED); }
"break"                 { LEX_ONLY_RESERVED(RESERVED); }
"byte"                  { LEX_ONLY_RESERVED(RESERVED); }
"case"                  { LEX_ONLY_RESERVED(RESERVED); }
"catch"                 { LEX_ONLY_RESERVED(RESERVED); }
"char"                  { LEX_ONLY_RESERVED(RESERVED); }
"const"                 { LEX_ONLY_RESERVED(RESERVED); }
"continue"              { LEX_ONLY_RESERVED(RESERVED); }
"default"               { LEX_ONLY_RESERVED(RESERVED); }
"do"                    { LEX_ONLY_RESERVED(RESERVED); }
"enum"                  { LEX_ONLY_RESERVED(RESERVED); }
"extends"               { LEX_ONLY_RESERVED(RESERVED); }
"final"                 { LEX_ONLY_RESERVED(RESERVED); }
"finally"               { LEX_ONLY_RESERVED(RESERVED); }
"float"                 { LEX_ONLY_RESERVED(RESERVED); }
"for"                   { LEX_ONLY_RESERVED(RESERVED); }
"goto"                  { LEX_ONLY_RESERVED(RESERVED); }
"implements"            { LEX_ONLY_RESERVED(RESERVED); }
"import"                { LEX_ONLY_RESERVED(RESERVED); }
"instanceof"            { LEX_ONLY_RESERVED(RESERVED); }
"interface"             { LEX_ONLY_RESERVED(RESERVED); }
"long"                  { LEX_ONLY_RESERVED(RESERVED); }
"native"                { LEX_ONLY_RESERVED(RESERVED); }
"new"                   { LEX_ONLY_RESERVED(RESERVED); }
"package"               { LEX_ONLY_RESERVED(RESERVED); }
"private"               { LEX_ONLY_RESERVED(RESERVED); }
"protected"             { LEX_ONLY_RESERVED(RESERVED); }
"short"                 { LEX_ONLY_RESERVED(RESERVED); }
"strictfp"              { LEX_ONLY_RESERVED(RESERVED); }
"super"                 { LEX_ONLY_RESERVED(RESERVED); }
"switch"                { LEX_ONLY_RESERVED(RESERVED); }
"synchronized"          { LEX_ONLY_RESERVED(RESERVED); }
"this"                  { LEX_ONLY_RESERVED(RESERVED); }
"throw"                 { LEX_ONLY_RESERVED(RESERVED); }
"throws"                { LEX_ONLY_RESERVED(RESERVED); }
"transient"             { LEX_ONLY_RESERVED(RESERVED); }
"try"                   { LEX_ONLY_RESERVED(RESERVED); }
"volatile"              { LEX_ONLY_RESERVED(RESERVED); }
"null"                  { LEX_ONLY_RESERVED(RESERVED); }
"++"                    { LEX_ONLY_RESERVED(RESERVED); }
"--"                    { LEX_ONLY_RESERVED(RESERVED); }
"Integer"               { LEX_ONLY_RESERVED(RESERVED); }
"System"                { LEX_ONLY_RESERVED(RESERVED); }

"&&"                    { LEX_ONLY_TOKEN("AND", AND); }
"||"                    { LEX_ONLY_TOKEN("OR", OR); }
"=="                    { LEX_ONLY_TOKEN("EQ", EQ); }
"!="                    { LEX_ONLY_TOKEN("NE", NE); }
">="                    { LEX_ONLY_TOKEN("GE", GE); }
"<="                    { LEX_ONLY_TOKEN("LE", LE); }
"<<"                    { LEX_ONLY_TOKEN("LSHIFT", LSHIFT); }
">>"                    { LEX_ONLY_TOKEN("RSHIFT", RSHIFT); }
"->"                    { LEX_ONLY_TOKEN("ARROW", ARROW); }
"="                     { LEX_ONLY_TOKEN("ASSIGN", ASSIGN); }
"*"                     { LEX_ONLY_TOKEN("STAR", STAR); }
","                     { LEX_ONLY_TOKEN("COMMA", COMMA); }
"/"                     { LEX_ONLY_TOKEN("DIV", DIV); }
">"                     { LEX_ONLY_TOKEN("GT", GT); }
"<"                     { LEX_ONLY_TOKEN("LT", LT); }
"-"                     { LEX_ONLY_TOKEN("MINUS", MINUS); }
"%"                     { LEX_ONLY_TOKEN("MOD", MOD); }
"!"                     { LEX_ONLY_TOKEN("NOT", NOT); }
"+"                     { LEX_ONLY_TOKEN("PLUS", PLUS); }
"^"                     { LEX_ONLY_TOKEN("XOR", XOR); }
"{"                     { LEX_ONLY_TOKEN("LBRACE", LBRACE); }
"}"                     { LEX_ONLY_TOKEN("RBRACE", RBRACE); }
"("                     { LEX_ONLY_TOKEN("LPAR", LPAR); }
")"                     { LEX_ONLY_TOKEN("RPAR", RPAR); }
"["                     { LEX_ONLY_TOKEN("LSQ", LSQ); }
"]"                     { LEX_ONLY_TOKEN("RSQ", RSQ); }
";"                     { LEX_ONLY_TOKEN("SEMICOLON", SEMICOLON); }

{DIGITS}"."{DIGITS}?{EXPONENT}?     { LEX_ONLY_TOKEN_VAL("DECIMAL", DECIMAL); }
"."{DIGITS}{EXPONENT}?              { LEX_ONLY_TOKEN_VAL("DECIMAL", DECIMAL); }
{DIGITS}{EXPONENT}                  { LEX_ONLY_TOKEN_VAL("DECIMAL", DECIMAL); }

0|{NONZERODIGIT}({DIGIT}|_)*{DIGIT}|{NONZERODIGIT}    { LEX_ONLY_TOKEN_VAL("NATURAL", NATURAL); }

{LETTER}{ALPHANUMID}*   { LEX_ONLY_TOKEN_VAL("IDENTIFIER", IDENTIFIER); }

\n                      { line++; col = 1; }
\r\n                    { line++; col = 1; }
\r                      { line++; col = 1; }
" "                     { col++; }
\t                      { col++; }
\f                      { col++; }

.                       { printf("Line %d, col %d: illegal character (%s)\n", line, col, yytext); col++; tok_line = line; tok_col = col; tok_text[0] = '\0'; }

<<EOF>>                 { tok_line = line; tok_col = col; tok_text[0] = '\0'; return 0; }

%%

int yywrap(void) {
    return 1;
}
