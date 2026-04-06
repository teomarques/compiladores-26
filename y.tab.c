/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 3 "jucompiler.y"


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


#line 89 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
    INTEGER = 300,                 /* INTEGER  */
    THEN = 301,                    /* THEN  */
    IDENTIFIER = 302,              /* IDENTIFIER  */
    NATURAL = 303,                 /* NATURAL  */
    DECIMAL = 304,                 /* DECIMAL  */
    STRLIT = 305,                  /* STRLIT  */
    BOOLLIT = 306,                 /* BOOLLIT  */
    RESERVED = 307,                /* RESERVED  */
    UPLUS = 308,                   /* UPLUS  */
    UMINUS = 309,                  /* UMINUS  */
    IFX = 310                      /* IFX  */
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
#define INTEGER 300
#define THEN 301
#define IDENTIFIER 302
#define NATURAL 303
#define DECIMAL 304
#define STRLIT 305
#define BOOLLIT 306
#define RESERVED 307
#define UPLUS 308
#define UMINUS 309
#define IFX 310

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 21 "jucompiler.y"

    char *lexeme;
    struct node *node;

#line 257 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_BOOL = 3,                       /* BOOL  */
  YYSYMBOL_CLASS = 4,                      /* CLASS  */
  YYSYMBOL_DOTLENGTH = 5,                  /* DOTLENGTH  */
  YYSYMBOL_DOUBLE = 6,                     /* DOUBLE  */
  YYSYMBOL_ELSE = 7,                       /* ELSE  */
  YYSYMBOL_IF = 8,                         /* IF  */
  YYSYMBOL_INT = 9,                        /* INT  */
  YYSYMBOL_PRINT = 10,                     /* PRINT  */
  YYSYMBOL_PARSEINT = 11,                  /* PARSEINT  */
  YYSYMBOL_PUBLIC = 12,                    /* PUBLIC  */
  YYSYMBOL_RETURN = 13,                    /* RETURN  */
  YYSYMBOL_STATIC = 14,                    /* STATIC  */
  YYSYMBOL_STRING = 15,                    /* STRING  */
  YYSYMBOL_VOID = 16,                      /* VOID  */
  YYSYMBOL_WHILE = 17,                     /* WHILE  */
  YYSYMBOL_EQ = 18,                        /* EQ  */
  YYSYMBOL_NE = 19,                        /* NE  */
  YYSYMBOL_GE = 20,                        /* GE  */
  YYSYMBOL_LE = 21,                        /* LE  */
  YYSYMBOL_LSHIFT = 22,                    /* LSHIFT  */
  YYSYMBOL_RSHIFT = 23,                    /* RSHIFT  */
  YYSYMBOL_ARROW = 24,                     /* ARROW  */
  YYSYMBOL_AND = 25,                       /* AND  */
  YYSYMBOL_OR = 26,                        /* OR  */
  YYSYMBOL_GT = 27,                        /* GT  */
  YYSYMBOL_LT = 28,                        /* LT  */
  YYSYMBOL_ASSIGN = 29,                    /* ASSIGN  */
  YYSYMBOL_NOT = 30,                       /* NOT  */
  YYSYMBOL_PLUS = 31,                      /* PLUS  */
  YYSYMBOL_MINUS = 32,                     /* MINUS  */
  YYSYMBOL_STAR = 33,                      /* STAR  */
  YYSYMBOL_DIV = 34,                       /* DIV  */
  YYSYMBOL_MOD = 35,                       /* MOD  */
  YYSYMBOL_XOR = 36,                       /* XOR  */
  YYSYMBOL_COMMA = 37,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 38,                 /* SEMICOLON  */
  YYSYMBOL_LPAR = 39,                      /* LPAR  */
  YYSYMBOL_RPAR = 40,                      /* RPAR  */
  YYSYMBOL_LBRACE = 41,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 42,                    /* RBRACE  */
  YYSYMBOL_LSQ = 43,                       /* LSQ  */
  YYSYMBOL_RSQ = 44,                       /* RSQ  */
  YYSYMBOL_INTEGER = 45,                   /* INTEGER  */
  YYSYMBOL_THEN = 46,                      /* THEN  */
  YYSYMBOL_IDENTIFIER = 47,                /* IDENTIFIER  */
  YYSYMBOL_NATURAL = 48,                   /* NATURAL  */
  YYSYMBOL_DECIMAL = 49,                   /* DECIMAL  */
  YYSYMBOL_STRLIT = 50,                    /* STRLIT  */
  YYSYMBOL_BOOLLIT = 51,                   /* BOOLLIT  */
  YYSYMBOL_RESERVED = 52,                  /* RESERVED  */
  YYSYMBOL_UPLUS = 53,                     /* UPLUS  */
  YYSYMBOL_UMINUS = 54,                    /* UMINUS  */
  YYSYMBOL_IFX = 55,                       /* IFX  */
  YYSYMBOL_YYACCEPT = 56,                  /* $accept  */
  YYSYMBOL_program = 57,                   /* program  */
  YYSYMBOL_classdecl = 58,                 /* classdecl  */
  YYSYMBOL_members = 59,                   /* members  */
  YYSYMBOL_member = 60,                    /* member  */
  YYSYMBOL_fielddecl = 61,                 /* fielddecl  */
  YYSYMBOL_methoddecl = 62,                /* methoddecl  */
  YYSYMBOL_methodparams_opt = 63,          /* methodparams_opt  */
  YYSYMBOL_methodparams = 64,              /* methodparams  */
  YYSYMBOL_paramdecl = 65,                 /* paramdecl  */
  YYSYMBOL_type_nonvoid = 66,              /* type_nonvoid  */
  YYSYMBOL_param_type = 67,                /* param_type  */
  YYSYMBOL_methodbody = 68,                /* methodbody  */
  YYSYMBOL_bodyelems = 69,                 /* bodyelems  */
  YYSYMBOL_vardecl = 70,                   /* vardecl  */
  YYSYMBOL_block = 71,                     /* block  */
  YYSYMBOL_stmts_opt = 72,                 /* stmts_opt  */
  YYSYMBOL_stmts = 73,                     /* stmts  */
  YYSYMBOL_stmt = 74,                      /* stmt  */
  YYSYMBOL_lvalue = 75,                    /* lvalue  */
  YYSYMBOL_expr = 76,                      /* expr  */
  YYSYMBOL_call_expr = 77,                 /* call_expr  */
  YYSYMBOL_arg_list_opt = 78,              /* arg_list_opt  */
  YYSYMBOL_arg_list = 79                   /* arg_list  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   444

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  77
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  156

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   310


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    66,    66,    73,    85,    88,    96,    97,   101,   107,
     113,   119,   128,   139,   154,   155,   160,   165,   173,   182,
     183,   184,   185,   189,   193,   201,   202,   207,   216,   235,
     236,   241,   243,   248,   250,   252,   258,   263,   267,   272,
     280,   287,   293,   295,   303,   308,   310,   312,   314,   316,
     318,   320,   326,   328,   330,   336,   338,   340,   342,   344,
     346,   350,   354,   358,   362,   364,   366,   368,   370,   372,
     374,   376,   378,   383,   394,   397,   402,   407
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "BOOL", "CLASS",
  "DOTLENGTH", "DOUBLE", "ELSE", "IF", "INT", "PRINT", "PARSEINT",
  "PUBLIC", "RETURN", "STATIC", "STRING", "VOID", "WHILE", "EQ", "NE",
  "GE", "LE", "LSHIFT", "RSHIFT", "ARROW", "AND", "OR", "GT", "LT",
  "ASSIGN", "NOT", "PLUS", "MINUS", "STAR", "DIV", "MOD", "XOR", "COMMA",
  "SEMICOLON", "LPAR", "RPAR", "LBRACE", "RBRACE", "LSQ", "RSQ", "INTEGER",
  "THEN", "IDENTIFIER", "NATURAL", "DECIMAL", "STRLIT", "BOOLLIT",
  "RESERVED", "UPLUS", "UMINUS", "IFX", "$accept", "program", "classdecl",
  "members", "member", "fielddecl", "methoddecl", "methodparams_opt",
  "methodparams", "paramdecl", "type_nonvoid", "param_type", "methodbody",
  "bodyelems", "vardecl", "block", "stmts_opt", "stmts", "stmt", "lvalue",
  "expr", "call_expr", "arg_list_opt", "arg_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-60)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       5,   -43,    15,   -60,   -23,   -60,   -60,   154,   -60,   -60,
     -60,    10,   103,   -22,   -60,   -60,   -60,   -60,   -16,    14,
     -11,   -10,    12,    16,    17,    21,    19,    25,   -60,   -60,
      20,   -36,   -60,   -60,   103,   -60,   103,    32,    37,   -60,
     -60,    31,    39,    40,   103,   -60,    40,   -60,   -60,   -60,
     -60,    52,    48,    49,    72,    53,   -60,    67,   -60,    58,
      51,   -60,   -60,   -60,    78,    75,    85,    85,    87,    85,
      85,    85,   -60,    85,    58,   -60,   -60,   -60,   -60,   279,
     -60,    85,    95,    67,   -60,    85,   106,    85,   -60,   183,
     207,   102,   -60,   -60,   -60,   231,   -60,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,   -60,   255,   -60,   -60,   327,   110,   121,
     -60,   303,    67,   123,   116,   -60,   384,   384,   403,   403,
      96,    96,   365,   346,   403,   403,     0,     0,   157,   157,
     157,   409,    67,   -60,    85,   -60,   158,   -60,    85,   -60,
     327,    67,   120,   -60,   127,   -60
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     0,     1,     4,     0,    21,    20,
      19,     0,     0,     0,     3,     5,     7,     6,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    22,     8,
       0,     0,     9,    11,    14,    10,    14,     0,    15,    16,
      23,     0,     0,     0,     0,    18,     0,    25,    13,    17,
      12,     0,     0,     0,     0,     0,    43,    29,    24,    44,
       0,    34,    33,    26,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,    49,    45,    46,    48,    47,     0,
      50,     0,     0,    30,    31,    74,     0,     0,    42,     0,
       0,     0,    68,    69,    70,     0,    71,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    38,     0,    28,    32,    76,     0,    75,
      27,     0,     0,     0,     0,    72,    54,    55,    59,    58,
      65,    66,    53,    52,    57,    56,    60,    61,    62,    63,
      64,    67,     0,    73,     0,    35,    39,    36,     0,    41,
      77,     0,     0,    40,     0,    51
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -60,   -60,   -60,   -60,   -60,   -60,   -60,   135,   -60,   128,
     163,   -60,   130,   -60,   -60,   -60,   -60,   -60,   -56,   -60,
     -59,   -51,   -60,   -60
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     3,     7,    15,    16,    17,    37,    38,    39,
      60,    41,    48,    51,    61,    62,    82,    83,    63,    64,
      79,    80,   118,   119
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      65,    84,    35,    36,     4,    96,    65,    89,    90,     1,
      92,    93,    94,     8,    95,     5,     9,     8,     6,    10,
       9,    22,   114,    10,    19,    13,   117,   116,   121,    13,
      24,    23,    65,   109,   110,   111,    26,    27,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,    29,     8,    28,    32,     9,    34,
      52,    10,    53,    33,    30,    54,   146,    13,    31,    55,
       8,    65,    43,     9,    44,    52,    10,    53,    45,    46,
      54,    47,    13,    68,    55,   150,   149,    66,    67,   152,
      56,    65,    81,    57,    58,   153,    68,    85,    86,    59,
      65,    96,    69,    70,    71,    56,     8,    87,    57,     9,
      72,    73,    10,    88,    59,    69,    70,    71,    13,    74,
      75,    76,    77,    78,    73,    96,    91,   107,   108,   109,
     110,   111,    74,    75,    76,    77,    78,   115,    97,    98,
      99,   100,   101,   102,   120,   103,   104,   105,   106,   124,
     143,   107,   108,   109,   110,   111,   112,     8,   144,   148,
       9,   147,    96,    10,   154,   151,    11,   155,    12,    13,
      18,    42,    49,     0,    20,    21,    50,     0,     0,     0,
       0,     0,    25,     0,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,    14,    40,     0,    40,
       0,    97,    98,    99,   100,   101,   102,    40,   103,   104,
     105,   106,    96,     0,   107,   108,   109,   110,   111,   112,
       0,     0,     0,   122,     0,    97,    98,    99,   100,   101,
     102,     0,   103,   104,   105,   106,    96,     0,   107,   108,
     109,   110,   111,   112,     0,     0,     0,   123,     0,    97,
      98,    99,   100,   101,   102,     0,   103,   104,   105,   106,
      96,     0,   107,   108,   109,   110,   111,   112,     0,     0,
       0,   125,     0,    97,    98,    99,   100,   101,   102,     0,
     103,   104,   105,   106,    96,     0,   107,   108,   109,   110,
     111,   112,     0,     0,     0,   142,     0,    97,    98,    99,
     100,   101,   102,     0,   103,   104,   105,   106,    96,     0,
     107,   108,   109,   110,   111,   112,     0,   113,     0,     0,
       0,    97,    98,    99,   100,   101,   102,     0,   103,   104,
     105,   106,    96,     0,   107,   108,   109,   110,   111,   112,
       0,   145,     0,     0,     0,    97,    98,    99,   100,   101,
     102,    96,   103,   104,   105,   106,     0,     0,   107,   108,
     109,   110,   111,   112,    97,    98,    99,   100,   101,   102,
      96,   103,     0,   105,   106,     0,     0,   107,   108,   109,
     110,   111,   112,    97,    98,    99,   100,   101,   102,    96,
       0,     0,   105,   106,     0,     0,   107,   108,   109,   110,
     111,   112,     0,     0,    99,   100,   101,   102,    96,     0,
       0,   105,   106,     0,    96,   107,   108,   109,   110,   111,
     112,     0,     0,     0,     0,   101,   102,     0,     0,     0,
       0,   101,   102,     0,   107,   108,   109,   110,   111,   112,
     107,   108,   109,   110,   111
};

static const yytype_int16 yycheck[] =
{
      51,    57,    38,    39,    47,     5,    57,    66,    67,     4,
      69,    70,    71,     3,    73,     0,     6,     3,    41,     9,
       6,    43,    81,     9,    14,    15,    85,    83,    87,    15,
      16,    47,    83,    33,    34,    35,    47,    47,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    38,     3,    44,    38,     6,    39,
       8,     9,    10,    38,    47,    13,   122,    15,    47,    17,
       3,   122,    40,     6,    37,     8,     9,    10,    47,    40,
      13,    41,    15,    11,    17,   144,   142,    39,    39,   148,
      38,   142,    39,    41,    42,   151,    11,    39,    47,    47,
     151,     5,    30,    31,    32,    38,     3,    29,    41,     6,
      38,    39,     9,    38,    47,    30,    31,    32,    15,    47,
      48,    49,    50,    51,    39,     5,    39,    31,    32,    33,
      34,    35,    47,    48,    49,    50,    51,    42,    18,    19,
      20,    21,    22,    23,    38,    25,    26,    27,    28,    47,
      40,    31,    32,    33,    34,    35,    36,     3,    37,    43,
       6,    38,     5,     9,    44,     7,    12,    40,    14,    15,
       7,    36,    44,    -1,    11,    12,    46,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    34,    -1,    36,
      -1,    18,    19,    20,    21,    22,    23,    44,    25,    26,
      27,    28,     5,    -1,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    40,    -1,    18,    19,    20,    21,    22,
      23,    -1,    25,    26,    27,    28,     5,    -1,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    40,    -1,    18,
      19,    20,    21,    22,    23,    -1,    25,    26,    27,    28,
       5,    -1,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    40,    -1,    18,    19,    20,    21,    22,    23,    -1,
      25,    26,    27,    28,     5,    -1,    31,    32,    33,    34,
      35,    36,    -1,    -1,    -1,    40,    -1,    18,    19,    20,
      21,    22,    23,    -1,    25,    26,    27,    28,     5,    -1,
      31,    32,    33,    34,    35,    36,    -1,    38,    -1,    -1,
      -1,    18,    19,    20,    21,    22,    23,    -1,    25,    26,
      27,    28,     5,    -1,    31,    32,    33,    34,    35,    36,
      -1,    38,    -1,    -1,    -1,    18,    19,    20,    21,    22,
      23,     5,    25,    26,    27,    28,    -1,    -1,    31,    32,
      33,    34,    35,    36,    18,    19,    20,    21,    22,    23,
       5,    25,    -1,    27,    28,    -1,    -1,    31,    32,    33,
      34,    35,    36,    18,    19,    20,    21,    22,    23,     5,
      -1,    -1,    27,    28,    -1,    -1,    31,    32,    33,    34,
      35,    36,    -1,    -1,    20,    21,    22,    23,     5,    -1,
      -1,    27,    28,    -1,     5,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,
      -1,    22,    23,    -1,    31,    32,    33,    34,    35,    36,
      31,    32,    33,    34,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    57,    58,    47,     0,    41,    59,     3,     6,
       9,    12,    14,    15,    42,    60,    61,    62,    66,    14,
      66,    66,    43,    47,    16,    66,    47,    47,    44,    38,
      47,    47,    38,    38,    39,    38,    39,    63,    64,    65,
      66,    67,    63,    40,    37,    47,    40,    41,    68,    65,
      68,    69,     8,    10,    13,    17,    38,    41,    42,    47,
      66,    70,    71,    74,    75,    77,    39,    39,    11,    30,
      31,    32,    38,    39,    47,    48,    49,    50,    51,    76,
      77,    39,    72,    73,    74,    39,    47,    29,    38,    76,
      76,    39,    76,    76,    76,    76,     5,    18,    19,    20,
      21,    22,    23,    25,    26,    27,    28,    31,    32,    33,
      34,    35,    36,    38,    76,    42,    74,    76,    78,    79,
      38,    76,    40,    40,    47,    40,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    40,    40,    37,    38,    74,    38,    43,    74,
      76,     7,    76,    74,    44,    40
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    56,    57,    58,    59,    59,    60,    60,    61,    61,
      61,    61,    62,    62,    63,    63,    64,    64,    65,    66,
      66,    66,    66,    67,    68,    69,    69,    70,    71,    72,
      72,    73,    73,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    75,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      76,    76,    76,    77,    78,    78,    79,    79
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     5,     0,     2,     1,     1,     3,     4,
       5,     4,     8,     8,     0,     1,     1,     3,     2,     1,
       1,     1,     3,     1,     3,     0,     2,     3,     3,     0,
       1,     1,     2,     1,     1,     4,     5,     2,     3,     5,
       7,     5,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     7,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     3,     4,     0,     1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: classdecl  */
#line 67 "jucompiler.y"
        {
            ast = (yyval.node) = (yyvsp[0].node);
        }
#line 1477 "y.tab.c"
    break;

  case 3: /* classdecl: CLASS IDENTIFIER LBRACE members RBRACE  */
#line 74 "jucompiler.y"
        {
            (yyval.node) = newnode(Program, NULL);
            addchild((yyval.node), newnode(Identifier, (yyvsp[-3].lexeme)));
            /* Program children: Identifier + {FieldDecl|MethodDecl}. */
            struct node_list *c = (yyvsp[-1].node)->children->next;
            while (c != NULL) { addchild((yyval.node), c->node); c = c->next; }
        }
#line 1489 "y.tab.c"
    break;

  case 4: /* members: %empty  */
#line 85 "jucompiler.y"
        {
            (yyval.node) = newnode(Tmp_List, NULL);
        }
#line 1497 "y.tab.c"
    break;

  case 5: /* members: members member  */
#line 89 "jucompiler.y"
        {
            (yyval.node) = (yyvsp[-1].node);
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1506 "y.tab.c"
    break;

  case 6: /* member: methoddecl  */
#line 96 "jucompiler.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1512 "y.tab.c"
    break;

  case 7: /* member: fielddecl  */
#line 97 "jucompiler.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1518 "y.tab.c"
    break;

  case 8: /* fielddecl: type_nonvoid IDENTIFIER SEMICOLON  */
#line 102 "jucompiler.y"
        {
            (yyval.node) = newnode(FieldDecl, NULL);
            addchild((yyval.node), (yyvsp[-2].node));
            addchild((yyval.node), newnode(Identifier, (yyvsp[-1].lexeme)));
        }
#line 1528 "y.tab.c"
    break;

  case 9: /* fielddecl: PUBLIC type_nonvoid IDENTIFIER SEMICOLON  */
#line 108 "jucompiler.y"
        {
            (yyval.node) = newnode(FieldDecl, NULL);
            addchild((yyval.node), (yyvsp[-2].node));
            addchild((yyval.node), newnode(Identifier, (yyvsp[-1].lexeme)));
        }
#line 1538 "y.tab.c"
    break;

  case 10: /* fielddecl: PUBLIC STATIC type_nonvoid IDENTIFIER SEMICOLON  */
#line 114 "jucompiler.y"
        {
            (yyval.node) = newnode(FieldDecl, NULL);
            addchild((yyval.node), (yyvsp[-2].node));
            addchild((yyval.node), newnode(Identifier, (yyvsp[-1].lexeme)));
        }
#line 1548 "y.tab.c"
    break;

  case 11: /* fielddecl: STATIC type_nonvoid IDENTIFIER SEMICOLON  */
#line 120 "jucompiler.y"
        {
            (yyval.node) = newnode(FieldDecl, NULL);
            addchild((yyval.node), (yyvsp[-2].node));
            addchild((yyval.node), newnode(Identifier, (yyvsp[-1].lexeme)));
        }
#line 1558 "y.tab.c"
    break;

  case 12: /* methoddecl: PUBLIC STATIC type_nonvoid IDENTIFIER LPAR methodparams_opt RPAR methodbody  */
#line 129 "jucompiler.y"
        {
            struct node *header = newnode(MethodHeader, NULL);
            addchild(header, (yyvsp[-5].node));
            addchild(header, newnode(Identifier, (yyvsp[-4].lexeme)));
            addchild(header, (yyvsp[-2].node));

            (yyval.node) = newnode(MethodDecl, NULL);
            addchild((yyval.node), header);
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1573 "y.tab.c"
    break;

  case 13: /* methoddecl: PUBLIC STATIC VOID IDENTIFIER LPAR methodparams_opt RPAR methodbody  */
#line 140 "jucompiler.y"
        {
            struct node *header = newnode(MethodHeader, NULL);
            addchild(header, newnode(Void, NULL));
            addchild(header, newnode(Identifier, (yyvsp[-4].lexeme)));
            addchild(header, (yyvsp[-2].node));

            (yyval.node) = newnode(MethodDecl, NULL);
            addchild((yyval.node), header);
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1588 "y.tab.c"
    break;

  case 14: /* methodparams_opt: %empty  */
#line 154 "jucompiler.y"
        { (yyval.node) = newnode(MethodParams, NULL); }
#line 1594 "y.tab.c"
    break;

  case 15: /* methodparams_opt: methodparams  */
#line 156 "jucompiler.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1600 "y.tab.c"
    break;

  case 16: /* methodparams: paramdecl  */
#line 161 "jucompiler.y"
        {
            (yyval.node) = newnode(MethodParams, NULL);
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1609 "y.tab.c"
    break;

  case 17: /* methodparams: methodparams COMMA paramdecl  */
#line 166 "jucompiler.y"
        {
            (yyval.node) = (yyvsp[-2].node);
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1618 "y.tab.c"
    break;

  case 18: /* paramdecl: param_type IDENTIFIER  */
#line 174 "jucompiler.y"
        {
            (yyval.node) = newnode(ParamDecl, NULL);
            addchild((yyval.node), (yyvsp[-1].node));
            addchild((yyval.node), newnode(Identifier, (yyvsp[0].lexeme)));
        }
#line 1628 "y.tab.c"
    break;

  case 19: /* type_nonvoid: INT  */
#line 182 "jucompiler.y"
                        { (yyval.node) = newnode(Int, NULL); }
#line 1634 "y.tab.c"
    break;

  case 20: /* type_nonvoid: DOUBLE  */
#line 183 "jucompiler.y"
                      { (yyval.node) = newnode(Double, NULL); }
#line 1640 "y.tab.c"
    break;

  case 21: /* type_nonvoid: BOOL  */
#line 184 "jucompiler.y"
                      { (yyval.node) = newnode(Bool, NULL); }
#line 1646 "y.tab.c"
    break;

  case 22: /* type_nonvoid: STRING LSQ RSQ  */
#line 185 "jucompiler.y"
                         { (yyval.node) = newnode(StringArray, NULL); }
#line 1652 "y.tab.c"
    break;

  case 23: /* param_type: type_nonvoid  */
#line 189 "jucompiler.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1658 "y.tab.c"
    break;

  case 24: /* methodbody: LBRACE bodyelems RBRACE  */
#line 194 "jucompiler.y"
        {
            (yyval.node) = (yyvsp[-1].node);
        }
#line 1666 "y.tab.c"
    break;

  case 25: /* bodyelems: %empty  */
#line 201 "jucompiler.y"
        { (yyval.node) = newnode(MethodBody, NULL); }
#line 1672 "y.tab.c"
    break;

  case 26: /* bodyelems: bodyelems stmt  */
#line 203 "jucompiler.y"
        { (yyval.node) = (yyvsp[-1].node); addchild((yyval.node), (yyvsp[0].node)); }
#line 1678 "y.tab.c"
    break;

  case 27: /* vardecl: type_nonvoid IDENTIFIER SEMICOLON  */
#line 208 "jucompiler.y"
        {
            (yyval.node) = newnode(VarDecl, NULL);
            addchild((yyval.node), (yyvsp[-2].node));
            addchild((yyval.node), newnode(Identifier, (yyvsp[-1].lexeme)));
        }
#line 1688 "y.tab.c"
    break;

  case 28: /* block: LBRACE stmts_opt RBRACE  */
#line 217 "jucompiler.y"
        {
            /* Avoid redundant Block with exactly 1 child. */
            int count = 0;
            struct node_list *c = (yyvsp[-1].node)->children->next;
            while (c != NULL) { count++; c = c->next; }

            if (count == 1) {
                (yyval.node) = (yyvsp[-1].node)->children->next->node;
            } else {
                (yyval.node) = newnode(Block, NULL);
                c = (yyvsp[-1].node)->children->next;
                while (c != NULL) { addchild((yyval.node), c->node); c = c->next; }
            }
        }
#line 1707 "y.tab.c"
    break;

  case 29: /* stmts_opt: %empty  */
#line 235 "jucompiler.y"
        { (yyval.node) = newnode(Tmp_List, NULL); }
#line 1713 "y.tab.c"
    break;

  case 30: /* stmts_opt: stmts  */
#line 237 "jucompiler.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1719 "y.tab.c"
    break;

  case 31: /* stmts: stmt  */
#line 242 "jucompiler.y"
        { (yyval.node) = newnode(Tmp_List, NULL); addchild((yyval.node), (yyvsp[0].node)); }
#line 1725 "y.tab.c"
    break;

  case 32: /* stmts: stmts stmt  */
#line 244 "jucompiler.y"
        { (yyval.node) = (yyvsp[-1].node); addchild((yyval.node), (yyvsp[0].node)); }
#line 1731 "y.tab.c"
    break;

  case 33: /* stmt: block  */
#line 249 "jucompiler.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1737 "y.tab.c"
    break;

  case 34: /* stmt: vardecl  */
#line 251 "jucompiler.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1743 "y.tab.c"
    break;

  case 35: /* stmt: lvalue ASSIGN expr SEMICOLON  */
#line 253 "jucompiler.y"
        {
            (yyval.node) = newnode(Assign, NULL);
            addchild((yyval.node), (yyvsp[-3].node));
            addchild((yyval.node), (yyvsp[-1].node));
        }
#line 1753 "y.tab.c"
    break;

  case 36: /* stmt: PRINT LPAR expr RPAR SEMICOLON  */
#line 259 "jucompiler.y"
        {
            (yyval.node) = newnode(Print, NULL);
            addchild((yyval.node), (yyvsp[-2].node));
        }
#line 1762 "y.tab.c"
    break;

  case 37: /* stmt: RETURN SEMICOLON  */
#line 264 "jucompiler.y"
        {
            (yyval.node) = newnode(Return, NULL);
        }
#line 1770 "y.tab.c"
    break;

  case 38: /* stmt: RETURN expr SEMICOLON  */
#line 268 "jucompiler.y"
        {
            (yyval.node) = newnode(Return, NULL);
            addchild((yyval.node), (yyvsp[-1].node));
        }
#line 1779 "y.tab.c"
    break;

  case 39: /* stmt: IF LPAR expr RPAR stmt  */
#line 273 "jucompiler.y"
        {
            (yyval.node) = newnode(If, NULL);
            addchild((yyval.node), (yyvsp[-2].node));
            addchild((yyval.node), (yyvsp[0].node));
            /* Mandatory else child: represent empty statement as empty Block. */
            addchild((yyval.node), newnode(Block, NULL));
        }
#line 1791 "y.tab.c"
    break;

  case 40: /* stmt: IF LPAR expr RPAR stmt ELSE stmt  */
#line 281 "jucompiler.y"
        {
            (yyval.node) = newnode(If, NULL);
            addchild((yyval.node), (yyvsp[-4].node));
            addchild((yyval.node), (yyvsp[-2].node));
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1802 "y.tab.c"
    break;

  case 41: /* stmt: WHILE LPAR expr RPAR stmt  */
#line 288 "jucompiler.y"
        {
            (yyval.node) = newnode(While, NULL);
            addchild((yyval.node), (yyvsp[-2].node));
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1812 "y.tab.c"
    break;

  case 42: /* stmt: call_expr SEMICOLON  */
#line 294 "jucompiler.y"
        { (yyval.node) = (yyvsp[-1].node); }
#line 1818 "y.tab.c"
    break;

  case 43: /* stmt: SEMICOLON  */
#line 296 "jucompiler.y"
        {
            /* Empty statement: represent as empty Block. */
            (yyval.node) = newnode(Block, NULL);
        }
#line 1827 "y.tab.c"
    break;

  case 44: /* lvalue: IDENTIFIER  */
#line 304 "jucompiler.y"
        { (yyval.node) = newnode(Identifier, (yyvsp[0].lexeme)); }
#line 1833 "y.tab.c"
    break;

  case 45: /* expr: NATURAL  */
#line 309 "jucompiler.y"
        { (yyval.node) = newnode(Natural, (yyvsp[0].lexeme)); }
#line 1839 "y.tab.c"
    break;

  case 46: /* expr: DECIMAL  */
#line 311 "jucompiler.y"
        { (yyval.node) = newnode(Decimal, (yyvsp[0].lexeme)); }
#line 1845 "y.tab.c"
    break;

  case 47: /* expr: BOOLLIT  */
#line 313 "jucompiler.y"
        { (yyval.node) = newnode(BoolLit, (yyvsp[0].lexeme)); }
#line 1851 "y.tab.c"
    break;

  case 48: /* expr: STRLIT  */
#line 315 "jucompiler.y"
        { (yyval.node) = newnode(StrLit, (yyvsp[0].lexeme)); }
#line 1857 "y.tab.c"
    break;

  case 49: /* expr: IDENTIFIER  */
#line 317 "jucompiler.y"
        { (yyval.node) = newnode(Identifier, (yyvsp[0].lexeme)); }
#line 1863 "y.tab.c"
    break;

  case 50: /* expr: call_expr  */
#line 319 "jucompiler.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1869 "y.tab.c"
    break;

  case 51: /* expr: PARSEINT LPAR IDENTIFIER LSQ expr RSQ RPAR  */
#line 321 "jucompiler.y"
        {
            (yyval.node) = newnode(ParseArgs, NULL);
            addchild((yyval.node), newnode(Identifier, (yyvsp[-4].lexeme)));
            addchild((yyval.node), (yyvsp[-2].node));
        }
#line 1879 "y.tab.c"
    break;

  case 52: /* expr: expr OR expr  */
#line 327 "jucompiler.y"
        { (yyval.node) = newnode(Or, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1885 "y.tab.c"
    break;

  case 53: /* expr: expr AND expr  */
#line 329 "jucompiler.y"
        { (yyval.node) = newnode(And, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1891 "y.tab.c"
    break;

  case 54: /* expr: expr EQ expr  */
#line 331 "jucompiler.y"
        {
            (yyval.node) = newnode(Eq, NULL);
            addchild((yyval.node), (yyvsp[-2].node));
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1901 "y.tab.c"
    break;

  case 55: /* expr: expr NE expr  */
#line 337 "jucompiler.y"
        { (yyval.node) = newnode(Ne, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1907 "y.tab.c"
    break;

  case 56: /* expr: expr LT expr  */
#line 339 "jucompiler.y"
        { (yyval.node) = newnode(Lt, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1913 "y.tab.c"
    break;

  case 57: /* expr: expr GT expr  */
#line 341 "jucompiler.y"
        { (yyval.node) = newnode(Gt, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1919 "y.tab.c"
    break;

  case 58: /* expr: expr LE expr  */
#line 343 "jucompiler.y"
        { (yyval.node) = newnode(Le, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1925 "y.tab.c"
    break;

  case 59: /* expr: expr GE expr  */
#line 345 "jucompiler.y"
        { (yyval.node) = newnode(Ge, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1931 "y.tab.c"
    break;

  case 60: /* expr: expr PLUS expr  */
#line 347 "jucompiler.y"
        {
            (yyval.node) = newnode(Add, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node));
        }
#line 1939 "y.tab.c"
    break;

  case 61: /* expr: expr MINUS expr  */
#line 351 "jucompiler.y"
        {
            (yyval.node) = newnode(Sub, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node));
        }
#line 1947 "y.tab.c"
    break;

  case 62: /* expr: expr STAR expr  */
#line 355 "jucompiler.y"
        {
            (yyval.node) = newnode(Mul, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node));
        }
#line 1955 "y.tab.c"
    break;

  case 63: /* expr: expr DIV expr  */
#line 359 "jucompiler.y"
        {
            (yyval.node) = newnode(Div, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node));
        }
#line 1963 "y.tab.c"
    break;

  case 64: /* expr: expr MOD expr  */
#line 363 "jucompiler.y"
        { (yyval.node) = newnode(Mod, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1969 "y.tab.c"
    break;

  case 65: /* expr: expr LSHIFT expr  */
#line 365 "jucompiler.y"
        { (yyval.node) = newnode(Lshift, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1975 "y.tab.c"
    break;

  case 66: /* expr: expr RSHIFT expr  */
#line 367 "jucompiler.y"
        { (yyval.node) = newnode(Rshift, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1981 "y.tab.c"
    break;

  case 67: /* expr: expr XOR expr  */
#line 369 "jucompiler.y"
        { (yyval.node) = newnode(Xor, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 1987 "y.tab.c"
    break;

  case 68: /* expr: NOT expr  */
#line 371 "jucompiler.y"
        { (yyval.node) = newnode(Not, NULL); addchild((yyval.node), (yyvsp[0].node)); }
#line 1993 "y.tab.c"
    break;

  case 69: /* expr: PLUS expr  */
#line 373 "jucompiler.y"
        { (yyval.node) = newnode(Plus, NULL); addchild((yyval.node), (yyvsp[0].node)); }
#line 1999 "y.tab.c"
    break;

  case 70: /* expr: MINUS expr  */
#line 375 "jucompiler.y"
        { (yyval.node) = newnode(Minus, NULL); addchild((yyval.node), (yyvsp[0].node)); }
#line 2005 "y.tab.c"
    break;

  case 71: /* expr: expr DOTLENGTH  */
#line 377 "jucompiler.y"
        { (yyval.node) = newnode(Length, NULL); addchild((yyval.node), (yyvsp[-1].node)); }
#line 2011 "y.tab.c"
    break;

  case 72: /* expr: LPAR expr RPAR  */
#line 379 "jucompiler.y"
        { (yyval.node) = (yyvsp[-1].node); }
#line 2017 "y.tab.c"
    break;

  case 73: /* call_expr: IDENTIFIER LPAR arg_list_opt RPAR  */
#line 384 "jucompiler.y"
        {
            (yyval.node) = newnode(Call, NULL);
            addchild((yyval.node), newnode(Identifier, (yyvsp[-3].lexeme)));
            struct node_list *c = (yyvsp[-1].node)->children->next;
            while (c != NULL) { addchild((yyval.node), c->node); c = c->next; }
        }
#line 2028 "y.tab.c"
    break;

  case 74: /* arg_list_opt: %empty  */
#line 394 "jucompiler.y"
        {
            (yyval.node) = newnode(Tmp_List, NULL);
        }
#line 2036 "y.tab.c"
    break;

  case 75: /* arg_list_opt: arg_list  */
#line 398 "jucompiler.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 2042 "y.tab.c"
    break;

  case 76: /* arg_list: expr  */
#line 403 "jucompiler.y"
        {
            (yyval.node) = newnode(Tmp_List, NULL);
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 2051 "y.tab.c"
    break;

  case 77: /* arg_list: arg_list COMMA expr  */
#line 408 "jucompiler.y"
        {
            (yyval.node) = (yyvsp[-2].node);
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 2060 "y.tab.c"
    break;


#line 2064 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 414 "jucompiler.y"


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
