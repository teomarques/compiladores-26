/* 
 * Autores:
 * Simão Tomás Botas Carvalho - 2021223055
 * Teodoro Marques - 2023211717
 */

%{
   #include <stdio.h>
   #include <string.h>
   int line = 1;
   int column = 1;
   int print_tokens = 0;
   int comment_start_line = 1;
   int comment_start_col = 1;
   int string_start_line = 1;
   int string_start_col = 1;
   char string_buffer[10000];
   int buffer_pos = 0;
    int string_has_error = 0;
   
   void print_simple_token(const char *token) {
       if (print_tokens) {
           printf("%s\r\n", token);
       }
   }
   
   void print_valued_token(const char *token, const char *value) {
       if (print_tokens) {
           printf("%s(%s)\r\n", token, value);
       }
   }
%}

%option noyywrap

digit [0-9]
nonzero [1-9]
underscore _
character [a-zA-Z_$]
digitseq {digit}({digit}|{underscore})*{digit}|{digit}
%x COMMENT
%x STRING
%%


"/*" { BEGIN(COMMENT); comment_start_line = line; comment_start_col = column; column += 2; }

<COMMENT>"*/" { BEGIN(INITIAL); column += 2; }
<COMMENT>\r\n { line++; column = 1; }
<COMMENT>\n { line++; column = 1; }
<COMMENT>\r { line++; column = 1; }
<COMMENT>[^*/\r\n]+ { column += yyleng; }
<COMMENT>[*/] { column++; }
<COMMENT><<EOF>> { printf("Line %d, col %d: unterminated comment\r\n", comment_start_line, comment_start_col); yyterminate(); }

"//"[^\r\n]* { column += yyleng;}

"\"" { BEGIN(STRING); string_start_line = line; string_start_col = column; buffer_pos = 0; string_has_error = 0; column++;}

<STRING>\" { 
    string_buffer[buffer_pos] = '\0';
    if (print_tokens && !string_has_error) printf("STRLIT(\"%s\")\r\n", string_buffer);
    BEGIN(INITIAL);
    column++;
}

<STRING>\\[fnrt\\"] { 
    string_buffer[buffer_pos++] = '\\';
    string_buffer[buffer_pos++] = yytext[1];
    column += yyleng;
}

<STRING>\\\r\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\r\n", line, column);
    printf("Line %d, col %d: unterminated string literal\r\n", string_start_line, string_start_col);
    line++;
    column = 1;
    BEGIN(INITIAL);
}

<STRING>\\\n {
    printf("Line %d, col %d: invalid escape sequence (\\)\r\n", line, column);
    printf("Line %d, col %d: unterminated string literal\r\n", string_start_line, string_start_col);
    line++;
    column = 1;
    BEGIN(INITIAL);
}

<STRING>\\\r {
    printf("Line %d, col %d: invalid escape sequence (\\)\r\n", line, column);
    printf("Line %d, col %d: unterminated string literal\r\n", string_start_line, string_start_col);
    line++;
    column = 1;
    BEGIN(INITIAL);
}

<STRING>\\. { 
    printf("Line %d, col %d: invalid escape sequence (%s)\r\n", line, column, yytext);
    string_has_error = 1;
    column += yyleng;
}

<STRING>\r\n {
    printf("Line %d, col %d: unterminated string literal\r\n", string_start_line, string_start_col);
    line++;
    column = 1;
    BEGIN(INITIAL);
}

<STRING>[\n\r] { 
    printf("Line %d, col %d: unterminated string literal\r\n", string_start_line, string_start_col);
    line++;
    column = 1;
    BEGIN(INITIAL);
}

<STRING><<EOF>> { 
    printf("Line %d, col %d: unterminated string literal\r\n", string_start_line, string_start_col);
    yyterminate();
}

<STRING>. {
    string_buffer[buffer_pos++] = yytext[0];
    column++;
}


[ ] {column++; }
\f  {column += yyleng; }
\t  {column += yyleng; }

\r\n { line++; column = 1;}
\n   { line++; column = 1;}
\r   { line++; column = 1;}


"true"|"false" { print_valued_token("BOOLLIT", yytext); column += yyleng; }
"==" { print_simple_token("EQ"); column += yyleng; }
"!=" { print_simple_token("NE"); column += yyleng; }
">=" { print_simple_token("GE"); column += yyleng; }
"<=" { print_simple_token("LE"); column += yyleng; }
"<<" { print_simple_token("LSHIFT"); column += yyleng; }
">>" { print_simple_token("RSHIFT"); column += yyleng; }
"->" { print_simple_token("ARROW"); column += yyleng; }
"&&" { print_simple_token("AND"); column += yyleng; }
"||" { print_simple_token("OR"); column += yyleng; }
">" { print_simple_token("GT"); column += yyleng; }
"<" { print_simple_token("LT"); column += yyleng; }
"=" { print_simple_token("ASSIGN"); column += yyleng; }
"!" { print_simple_token("NOT"); column += yyleng; }
"+" { print_simple_token("PLUS"); column += yyleng; }
"-" { print_simple_token("MINUS"); column += yyleng; }
"*" { print_simple_token("STAR"); column += yyleng; }
"/" { print_simple_token("DIV"); column += yyleng; }
"%" { print_simple_token("MOD"); column += yyleng; }
"^" { print_simple_token("XOR"); column += yyleng; }
"," { print_simple_token("COMMA"); column += yyleng; }
";" { print_simple_token("SEMICOLON"); column += yyleng; }
"(" { print_simple_token("LPAR"); column += yyleng; }
")" { print_simple_token("RPAR"); column += yyleng; }
"{" { print_simple_token("LBRACE"); column += yyleng; }
"}" { print_simple_token("RBRACE"); column += yyleng; }
"[" { print_simple_token("LSQ"); column += yyleng; }
"]" { print_simple_token("RSQ"); column += yyleng; }




"boolean" { print_simple_token("BOOL"); column += yyleng; }
"class" { print_simple_token("CLASS"); column += yyleng; }
".length" { print_simple_token("DOTLENGTH"); column += yyleng; }
"double" { print_simple_token("DOUBLE"); column += yyleng; }
"else" { print_simple_token("ELSE"); column += yyleng; }
"if" { print_simple_token("IF"); column += yyleng; }
"int" { print_simple_token("INT"); column += yyleng; }
"System.out.print" { print_simple_token("PRINT"); column += yyleng; }
"Integer.parseInt" { print_simple_token("PARSEINT"); column += yyleng; }
"public" { print_simple_token("PUBLIC"); column += yyleng; }
"return" { print_simple_token("RETURN"); column += yyleng; }
"static" { print_simple_token("STATIC"); column += yyleng; }
"String" { print_simple_token("STRING"); column += yyleng; }
"void" { print_simple_token("VOID"); column += yyleng; }
"while" { print_simple_token("WHILE"); column += yyleng; }

"++" { print_valued_token("RESERVED", yytext); column += yyleng; }
"--" { print_valued_token("RESERVED", yytext); column += yyleng; }
"null" { print_valued_token("RESERVED", yytext); column += yyleng; }
"Integer" { print_valued_token("RESERVED", yytext); column += yyleng; }
"System" { print_valued_token("RESERVED", yytext); column += yyleng; }
"abstract" { print_valued_token("RESERVED", yytext); column += yyleng; }
"assert" { print_valued_token("RESERVED", yytext); column += yyleng; }
"break" { print_valued_token("RESERVED", yytext); column += yyleng; }
"byte" { print_valued_token("RESERVED", yytext); column += yyleng; }
"case" { print_valued_token("RESERVED", yytext); column += yyleng; }
"catch" { print_valued_token("RESERVED", yytext); column += yyleng; }
"char" { print_valued_token("RESERVED", yytext); column += yyleng; }
"const" { print_valued_token("RESERVED", yytext); column += yyleng; }
"continue" { print_valued_token("RESERVED", yytext); column += yyleng; }
"default" { print_valued_token("RESERVED", yytext); column += yyleng; }
"do" { print_valued_token("RESERVED", yytext); column += yyleng; }
"enum" { print_valued_token("RESERVED", yytext); column += yyleng; }
"extends" { print_valued_token("RESERVED", yytext); column += yyleng; }
"final" { print_valued_token("RESERVED", yytext); column += yyleng; }
"finally" { print_valued_token("RESERVED", yytext); column += yyleng; }
"float" { print_valued_token("RESERVED", yytext); column += yyleng; }
"for" { print_valued_token("RESERVED", yytext); column += yyleng; }
"goto" { print_valued_token("RESERVED", yytext); column += yyleng; }
"implements" { print_valued_token("RESERVED", yytext); column += yyleng; }
"import" { print_valued_token("RESERVED", yytext); column += yyleng; }
"instanceof" { print_valued_token("RESERVED", yytext); column += yyleng; }
"interface" { print_valued_token("RESERVED", yytext); column += yyleng; }
"long" { print_valued_token("RESERVED", yytext); column += yyleng; }
"native" { print_valued_token("RESERVED", yytext); column += yyleng; }
"new" { print_valued_token("RESERVED", yytext); column += yyleng; }
"package" { print_valued_token("RESERVED", yytext); column += yyleng; }
"private" { print_valued_token("RESERVED", yytext); column += yyleng; }
"protected" { print_valued_token("RESERVED", yytext); column += yyleng; }
"short" { print_valued_token("RESERVED", yytext); column += yyleng; }
"strictfp" { print_valued_token("RESERVED", yytext); column += yyleng; }
"super" { print_valued_token("RESERVED", yytext); column += yyleng; }
"switch" { print_valued_token("RESERVED", yytext); column += yyleng; }
"synchronized" { print_valued_token("RESERVED", yytext); column += yyleng; }
"this" { print_valued_token("RESERVED", yytext); column += yyleng; }
"throw" { print_valued_token("RESERVED", yytext); column += yyleng; }
"throws" { print_valued_token("RESERVED", yytext); column += yyleng; }
"transient" { print_valued_token("RESERVED", yytext); column += yyleng; }
"try" { print_valued_token("RESERVED", yytext); column += yyleng; }
"volatile" { print_valued_token("RESERVED", yytext); column += yyleng; }




{digitseq}[eE][+-]?{digitseq} { print_valued_token("DECIMAL", yytext); column += yyleng; }
{digitseq}?"."{digitseq}([eE][+-]?{digitseq})? { print_valued_token("DECIMAL", yytext); column += yyleng; }
{digitseq}"."([eE][+-]?{digitseq})? { print_valued_token("DECIMAL", yytext); column += yyleng; }

{character}+({digit}|{character})* { print_valued_token("IDENTIFIER", yytext); column += yyleng; }
0|{nonzero}({digit}|{underscore})*{digit}|{nonzero} { print_valued_token("NATURAL", yytext); column += yyleng; }


. { printf("Line %d, col %d: illegal character (%s)\r\n", line, column, yytext); column += yyleng; }

%%
extern int yylex();
int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "-l") == 0) {
            print_tokens = 1;
        } else if (strcmp(argv[1], "-e1") == 0) {
            print_tokens = 0;
        }
    } else {
        print_tokens = 0;
    }
    yylex();
    return 0;
}
