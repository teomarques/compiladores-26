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
    VOID = 271,                    /* VOID  */
    WHILE = 272,                   /* WHILE  */
    EQ = 273,                      /* EQ  */
    NE = 274,                      /* NE  */
    GE = 275,                      /* GE  */
    LE = 276,                      /* LE  */
    LSHIFT = 277,                  /* LSHIFT  */
    RSHIFT = 278,                  /* RSHIFT  */
    ARROW = 279,                   /* ARROW  */
    AND = 280,                     /* AND  */
    OR = 281,                      /* OR  */
    GT = 282,                      /* GT  */
    LT = 283,                      /* LT  */
    ASSIGN = 284,                  /* ASSIGN  */
    NOT = 285,                     /* NOT  */
    PLUS = 286,                    /* PLUS  */
    MINUS = 287,                   /* MINUS  */
    STAR = 288,                    /* STAR  */
    DIV = 289,                     /* DIV  */
    MOD = 290,                     /* MOD  */
    XOR = 291,                     /* XOR  */
    COMMA = 292,                   /* COMMA  */
    SEMICOLON = 293,               /* SEMICOLON  */
    LPAR = 294,                    /* LPAR  */
    RPAR = 295,                    /* RPAR  */
    LBRACE = 296,                  /* LBRACE  */
    RBRACE = 297,                  /* RBRACE  */
    LSQ = 298,                     /* LSQ  */
    RSQ = 299,                     /* RSQ  */
    IDENTIFIER = 300,              /* IDENTIFIER  */
    NATURAL = 301,                 /* NATURAL  */
    DECIMAL = 302,                 /* DECIMAL  */
    STRLIT = 303,                  /* STRLIT  */
    BOOLLIT = 304,                 /* BOOLLIT  */
    RESERVED = 305,                /* RESERVED  */
    IFX = 306,                     /* IFX  */
    UPLUS = 307,                   /* UPLUS  */
    UMINUS = 308,                  /* UMINUS  */
    UNOT = 309                     /* UNOT  */
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
#define VOID 271
#define WHILE 272
#define EQ 273
#define NE 274
#define GE 275
#define LE 276
#define LSHIFT 277
#define RSHIFT 278
#define ARROW 279
#define AND 280
#define OR 281
#define GT 282
#define LT 283
#define ASSIGN 284
#define NOT 285
#define PLUS 286
#define MINUS 287
#define STAR 288
#define DIV 289
#define MOD 290
#define XOR 291
#define COMMA 292
#define SEMICOLON 293
#define LPAR 294
#define RPAR 295
#define LBRACE 296
#define RBRACE 297
#define LSQ 298
#define RSQ 299
#define IDENTIFIER 300
#define NATURAL 301
#define DECIMAL 302
#define STRLIT 303
#define BOOLLIT 304
#define RESERVED 305
#define IFX 306
#define UPLUS 307
#define UMINUS 308
#define UNOT 309

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 45 "jucompiler.y"

    char        *lexeme;
    struct node *n;

#line 180 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
