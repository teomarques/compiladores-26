/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    BOOL = 258,                    /* BOOL  */
    CLASS = 259,                   /* CLASS  */
    DOTLENGTH = 260,               /* DOTLENGTH  */
    DOUBLE = 261,                  /* DOUBLE  */
    ELSE = 262,                    /* ELSE  */
    IF = 263,                      /* IF  */
    INT = 264,                     /* INT  */
    PRINT = 265,                   /* PRINT  */
    PARSEINT = 266,                /* PARSEINT  */
    PUBLIC = 267,                  /* PUBLIC  */
    RETURN = 268,                  /* RETURN  */
    STATIC = 269,                  /* STATIC  */
    STRING = 270,                  /* STRING  */
    STRINGARRAY = 271,             /* STRINGARRAY  */
    VOID = 272,                    /* VOID  */
    WHILE = 273,                   /* WHILE  */
    EQ = 274,                      /* EQ  */
    NE = 275,                      /* NE  */
    GE = 276,                      /* GE  */
    LE = 277,                      /* LE  */
    LSHIFT = 278,                  /* LSHIFT  */
    RSHIFT = 279,                  /* RSHIFT  */
    ARROW = 280,                   /* ARROW  */
    AND = 281,                     /* AND  */
    OR = 282,                      /* OR  */
    GT = 283,                      /* GT  */
    LT = 284,                      /* LT  */
    ASSIGN = 285,                  /* ASSIGN  */
    NOT = 286,                     /* NOT  */
    PLUS = 287,                    /* PLUS  */
    MINUS = 288,                   /* MINUS  */
    STAR = 289,                    /* STAR  */
    DIV = 290,                     /* DIV  */
    MOD = 291,                     /* MOD  */
    XOR = 292,                     /* XOR  */
    COMMA = 293,                   /* COMMA  */
    SEMICOLON = 294,               /* SEMICOLON  */
    LPAR = 295,                    /* LPAR  */
    RPAR = 296,                    /* RPAR  */
    LBRACE = 297,                  /* LBRACE  */
    RBRACE = 298,                  /* RBRACE  */
    LSQ = 299,                     /* LSQ  */
    RSQ = 300,                     /* RSQ  */
    INTEGER = 301,                 /* INTEGER  */
    THEN = 302,                    /* THEN  */
    IDENTIFIER = 303,              /* IDENTIFIER  */
    NATURAL = 304,                 /* NATURAL  */
    DECIMAL = 305,                 /* DECIMAL  */
    STRLIT = 306,                  /* STRLIT  */
    BOOLLIT = 307,                 /* BOOLLIT  */
    RESERVED = 308,                /* RESERVED  */
    UPLUS = 309,                   /* UPLUS  */
    UMINUS = 310,                  /* UMINUS  */
    IFX = 311                      /* IFX  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define BOOL 258
#define CLASS 259
#define DOTLENGTH 260
#define DOUBLE 261
#define ELSE 262
#define IF 263
#define INT 264
#define PRINT 265
#define PARSEINT 266
#define PUBLIC 267
#define RETURN 268
#define STATIC 269
#define STRING 270
#define STRINGARRAY 271
#define VOID 272
#define WHILE 273
#define EQ 274
#define NE 275
#define GE 276
#define LE 277
#define LSHIFT 278
#define RSHIFT 279
#define ARROW 280
#define AND 281
#define OR 282
#define GT 283
#define LT 284
#define ASSIGN 285
#define NOT 286
#define PLUS 287
#define MINUS 288
#define STAR 289
#define DIV 290
#define MOD 291
#define XOR 292
#define COMMA 293
#define SEMICOLON 294
#define LPAR 295
#define RPAR 296
#define LBRACE 297
#define RBRACE 298
#define LSQ 299
#define RSQ 300
#define INTEGER 301
#define THEN 302
#define IDENTIFIER 303
#define NATURAL 304
#define DECIMAL 305
#define STRLIT 306
#define BOOLLIT 307
#define RESERVED 308
#define UPLUS 309
#define UMINUS 310
#define IFX 311

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 32 "jucompiler.y"

    char *lexeme;
    struct node *node;

#line 184 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
