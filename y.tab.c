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
static int syntax_error_count = 0;
static int in_method_header = 0;

static struct node *dup_type_node(struct node *t) {
    if (t == NULL) return NULL;
    return newnode(t->category, NULL);
}


#line 96 "y.tab.c"

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
#line 28 "jucompiler.y"

    char *lexeme;
    struct node *node;

#line 264 "y.tab.c"

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
  YYSYMBOL_63_1 = 63,                      /* $@1  */
  YYSYMBOL_64_2 = 64,                      /* $@2  */
  YYSYMBOL_65_3 = 65,                      /* $@3  */
  YYSYMBOL_66_4 = 66,                      /* $@4  */
  YYSYMBOL_methodparams_opt = 67,          /* methodparams_opt  */
  YYSYMBOL_methodparams = 68,              /* methodparams  */
  YYSYMBOL_paramdecl = 69,                 /* paramdecl  */
  YYSYMBOL_type_nonvoid = 70,              /* type_nonvoid  */
  YYSYMBOL_param_type = 71,                /* param_type  */
  YYSYMBOL_methodbody = 72,                /* methodbody  */
  YYSYMBOL_bodyelems = 73,                 /* bodyelems  */
  YYSYMBOL_vardecl = 74,                   /* vardecl  */
  YYSYMBOL_vardecl_ids = 75,               /* vardecl_ids  */
  YYSYMBOL_block = 76,                     /* block  */
  YYSYMBOL_stmts_opt = 77,                 /* stmts_opt  */
  YYSYMBOL_stmts = 78,                     /* stmts  */
  YYSYMBOL_stmt = 79,                      /* stmt  */
  YYSYMBOL_lvalue = 80,                    /* lvalue  */
  YYSYMBOL_expr = 81,                      /* expr  */
  YYSYMBOL_call_expr = 82,                 /* call_expr  */
  YYSYMBOL_arg_list_opt = 83,              /* arg_list_opt  */
  YYSYMBOL_arg_list = 84                   /* arg_list  */
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
#define YYLAST   554

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  89
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  175

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
       0,    73,    73,    80,    95,    98,   113,   114,   115,   119,
     125,   137,   149,   161,   176,   176,   176,   187,   187,   187,
     202,   203,   208,   213,   221,   230,   231,   232,   233,   237,
     241,   249,   250,   265,   281,   286,   294,   313,   314,   319,
     331,   346,   348,   350,   356,   362,   367,   371,   376,   384,
     391,   397,   399,   407,   412,   414,   416,   418,   420,   422,
     424,   430,   436,   438,   440,   446,   448,   450,   452,   454,
     456,   460,   464,   468,   472,   474,   476,   478,   480,   482,
     484,   486,   488,   490,   499,   506,   516,   519,   524,   529
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
  "members", "member", "fielddecl", "methoddecl", "$@1", "$@2", "$@3",
  "$@4", "methodparams_opt", "methodparams", "paramdecl", "type_nonvoid",
  "param_type", "methodbody", "bodyelems", "vardecl", "vardecl_ids",
  "block", "stmts_opt", "stmts", "stmt", "lvalue", "expr", "call_expr",
  "arg_list_opt", "arg_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-75)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-87)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       0,   -36,    16,   -75,   -23,   -75,   -75,   216,   -17,   -75,
     -75,   -75,    86,   167,   -18,   -75,   -75,   -75,   -75,   -75,
     -20,   -75,    67,   -20,   -20,   -15,   -75,   -35,   -16,   -14,
      30,    34,   -75,   -12,   -75,   -75,    42,    40,   -75,   -75,
     -75,    55,    57,   -75,   167,   167,    13,    50,   -75,   -75,
      32,    58,   -75,   167,   -75,   -75,    65,   -75,    65,   -75,
     -75,   -75,   130,    61,    68,    75,   218,    76,   -75,   143,
     -75,    77,   -20,   -75,   -75,   -75,    88,    81,   -75,   231,
     231,    93,   231,   231,   231,   -75,    79,    77,   -75,   -75,
     -75,   -75,   394,   -75,   231,    83,   185,   -75,    73,    48,
     231,   -75,   298,   322,    14,   -75,   -75,   -75,    94,   346,
     -75,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   -75,   370,   -75,
     -75,    95,   442,   101,   105,   -75,   418,   198,   110,   117,
     107,   -75,   -75,   499,   499,   518,   518,    25,    25,   480,
     461,   518,   518,    31,    31,   154,   154,   154,   132,   198,
     -75,   -75,   231,   -75,   155,   -75,   -75,   231,   -75,   442,
     198,   266,   -75,   121,   -75
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     0,     1,     4,     0,     0,    27,
      26,    25,     0,     0,     0,     8,     3,     5,     7,     6,
       0,     9,     0,     0,     0,     0,    34,     0,     0,     0,
       0,     0,    28,     0,    10,    17,    34,     0,    11,    13,
      35,     0,     0,    12,    20,    20,     0,    21,    22,    29,
       0,     0,    18,     0,    24,    15,     0,    23,     0,    31,
      19,    16,     0,     0,     0,     0,     0,     0,    52,     0,
      30,    53,     0,    42,    41,    32,     0,     0,    43,     0,
       0,     0,     0,     0,     0,    46,     0,    58,    54,    55,
      57,    56,     0,    59,     0,     0,     0,    39,     0,     0,
       0,    51,     0,     0,     0,    78,    79,    80,     0,     0,
      81,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,     0,    36,
      40,     0,    88,     0,    87,    33,     0,     0,     0,     0,
       0,    83,    82,    64,    65,    69,    68,    75,    76,    63,
      62,    67,    66,    70,    71,    72,    73,    74,    77,     0,
      85,    84,     0,    44,    48,    45,    61,     0,    50,    89,
       0,     0,    49,     0,    60
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,   124,   -75,   122,    10,   -75,   116,   -75,   -75,   -10,
     -75,   -75,   -75,   -68,   -75,   -74,   -62,   -75,   -75
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     7,    17,    18,    19,    42,    58,    41,
      56,    46,    47,    48,    72,    50,    60,    62,    73,    27,
      74,    95,    96,    75,    76,    92,    93,   133,   134
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      77,    97,    33,    34,     1,   102,   103,    77,   105,   106,
     107,     4,   109,    30,    31,   139,     5,    20,     6,    37,
     128,    21,    23,    24,   132,    25,   136,    26,   130,    32,
     110,    35,    29,    36,    77,    40,   110,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    52,    49,    49,   121,   122,   123,   124,
     125,   140,    99,    49,   123,   124,   125,    33,    38,   164,
       9,    33,    39,    10,   131,    77,    11,    33,    43,    54,
     108,   -14,    14,    28,    81,    33,   135,    53,   169,     9,
      81,   168,    10,   171,    44,    11,    45,    77,    55,    78,
      22,    14,   172,    82,    83,    84,    59,    79,    77,    82,
      83,    84,    86,   -86,    80,    94,    98,   100,    86,   101,
      87,    88,    89,    90,    91,   129,    87,    88,    89,    90,
      91,    63,   104,     9,   141,   160,    10,   110,    64,    11,
      65,   161,   162,    66,    63,    14,     9,    67,   165,    10,
     167,    64,    11,    65,   115,   116,    66,   166,    14,   110,
      67,   174,   170,   121,   122,   123,   124,   125,    68,    51,
       9,    69,    70,    10,    61,    57,    11,    71,     0,     0,
       0,    68,    14,     0,    69,   -37,    63,     0,     9,     0,
      71,    10,     0,    64,    11,    65,     0,     0,    66,    63,
      14,     9,    67,     0,    10,     0,    64,    11,    65,     0,
       0,    66,     0,    14,     0,    67,     0,     8,     0,     9,
       0,     0,    10,    68,     0,    11,    69,   -38,    12,    81,
      13,    14,    71,     0,     0,     0,    68,     0,     0,    69,
       0,     0,    81,     0,     0,    71,     0,     0,    82,    83,
      84,     0,     0,     0,    15,     0,    85,    86,    16,     0,
       0,    82,    83,    84,     0,    87,    88,    89,    90,    91,
      86,   110,     0,     0,     0,     0,     0,     0,    87,    88,
      89,    90,    91,     0,   111,   112,   113,   114,   115,   116,
       0,   117,   118,   119,   120,     0,     0,   121,   122,   123,
     124,   125,   126,   110,     0,     0,     0,     0,     0,     0,
     173,     0,     0,     0,     0,     0,   111,   112,   113,   114,
     115,   116,     0,   117,   118,   119,   120,   110,     0,   121,
     122,   123,   124,   125,   126,     0,     0,     0,   137,     0,
     111,   112,   113,   114,   115,   116,     0,   117,   118,   119,
     120,   110,     0,   121,   122,   123,   124,   125,   126,     0,
       0,     0,   138,     0,   111,   112,   113,   114,   115,   116,
       0,   117,   118,   119,   120,   110,     0,   121,   122,   123,
     124,   125,   126,     0,     0,     0,   142,     0,   111,   112,
     113,   114,   115,   116,     0,   117,   118,   119,   120,   110,
       0,   121,   122,   123,   124,   125,   126,     0,     0,     0,
     159,     0,   111,   112,   113,   114,   115,   116,     0,   117,
     118,   119,   120,   110,     0,   121,   122,   123,   124,   125,
     126,     0,   127,     0,     0,     0,   111,   112,   113,   114,
     115,   116,     0,   117,   118,   119,   120,   110,     0,   121,
     122,   123,   124,   125,   126,     0,   163,     0,     0,     0,
     111,   112,   113,   114,   115,   116,   110,   117,   118,   119,
     120,     0,     0,   121,   122,   123,   124,   125,   126,   111,
     112,   113,   114,   115,   116,   110,   117,     0,   119,   120,
       0,     0,   121,   122,   123,   124,   125,   126,   111,   112,
     113,   114,   115,   116,   110,     0,     0,   119,   120,     0,
       0,   121,   122,   123,   124,   125,   126,     0,     0,   113,
     114,   115,   116,   110,     0,     0,   119,   120,     0,     0,
     121,   122,   123,   124,   125,   126,     0,     0,     0,     0,
     115,   116,     0,     0,     0,     0,     0,     0,     0,   121,
     122,   123,   124,   125,   126
};

static const yytype_int16 yycheck[] =
{
      62,    69,    37,    38,     4,    79,    80,    69,    82,    83,
      84,    47,    86,    23,    24,     1,     0,     7,    41,    29,
      94,    38,    12,    13,    98,    43,   100,    47,    96,    44,
       5,    47,    22,    47,    96,    47,     5,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,    40,    44,    45,    31,    32,    33,    34,
      35,    47,    72,    53,    33,    34,    35,    37,    38,   137,
       3,    37,    38,     6,     1,   137,     9,    37,    38,    47,
       1,    39,    15,    16,    11,    37,    38,    37,   162,     3,
      11,   159,     6,   167,    39,     9,    39,   159,    40,    38,
      14,    15,   170,    30,    31,    32,    41,    39,   170,    30,
      31,    32,    39,    40,    39,    39,    39,    29,    39,    38,
      47,    48,    49,    50,    51,    42,    47,    48,    49,    50,
      51,     1,    39,     3,    40,    40,     6,     5,     8,     9,
      10,    40,    37,    13,     1,    15,     3,    17,    38,     6,
      43,     8,     9,    10,    22,    23,    13,    40,    15,     5,
      17,    40,     7,    31,    32,    33,    34,    35,    38,    45,
       3,    41,    42,     6,    58,    53,     9,    47,    -1,    -1,
      -1,    38,    15,    -1,    41,    42,     1,    -1,     3,    -1,
      47,     6,    -1,     8,     9,    10,    -1,    -1,    13,     1,
      15,     3,    17,    -1,     6,    -1,     8,     9,    10,    -1,
      -1,    13,    -1,    15,    -1,    17,    -1,     1,    -1,     3,
      -1,    -1,     6,    38,    -1,     9,    41,    42,    12,    11,
      14,    15,    47,    -1,    -1,    -1,    38,    -1,    -1,    41,
      -1,    -1,    11,    -1,    -1,    47,    -1,    -1,    30,    31,
      32,    -1,    -1,    -1,    38,    -1,    38,    39,    42,    -1,
      -1,    30,    31,    32,    -1,    47,    48,    49,    50,    51,
      39,     5,    -1,    -1,    -1,    -1,    -1,    -1,    47,    48,
      49,    50,    51,    -1,    18,    19,    20,    21,    22,    23,
      -1,    25,    26,    27,    28,    -1,    -1,    31,    32,    33,
      34,    35,    36,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    -1,    25,    26,    27,    28,     5,    -1,    31,
      32,    33,    34,    35,    36,    -1,    -1,    -1,    40,    -1,
      18,    19,    20,    21,    22,    23,    -1,    25,    26,    27,
      28,     5,    -1,    31,    32,    33,    34,    35,    36,    -1,
      -1,    -1,    40,    -1,    18,    19,    20,    21,    22,    23,
      -1,    25,    26,    27,    28,     5,    -1,    31,    32,    33,
      34,    35,    36,    -1,    -1,    -1,    40,    -1,    18,    19,
      20,    21,    22,    23,    -1,    25,    26,    27,    28,     5,
      -1,    31,    32,    33,    34,    35,    36,    -1,    -1,    -1,
      40,    -1,    18,    19,    20,    21,    22,    23,    -1,    25,
      26,    27,    28,     5,    -1,    31,    32,    33,    34,    35,
      36,    -1,    38,    -1,    -1,    -1,    18,    19,    20,    21,
      22,    23,    -1,    25,    26,    27,    28,     5,    -1,    31,
      32,    33,    34,    35,    36,    -1,    38,    -1,    -1,    -1,
      18,    19,    20,    21,    22,    23,     5,    25,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    36,    18,
      19,    20,    21,    22,    23,     5,    25,    -1,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    36,    18,    19,
      20,    21,    22,    23,     5,    -1,    -1,    27,    28,    -1,
      -1,    31,    32,    33,    34,    35,    36,    -1,    -1,    20,
      21,    22,    23,     5,    -1,    -1,    27,    28,    -1,    -1,
      31,    32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,
      22,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    33,    34,    35,    36
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    57,    58,    47,     0,    41,    59,     1,     3,
       6,     9,    12,    14,    15,    38,    42,    60,    61,    62,
      70,    38,    14,    70,    70,    43,    47,    75,    16,    70,
      75,    75,    44,    37,    38,    47,    47,    75,    38,    38,
      47,    65,    63,    38,    39,    39,    67,    68,    69,    70,
      71,    67,    40,    37,    47,    40,    66,    69,    64,    41,
      72,    72,    73,     1,     8,    10,    13,    17,    38,    41,
      42,    47,    70,    74,    76,    79,    80,    82,    38,    39,
      39,    11,    30,    31,    32,    38,    39,    47,    48,    49,
      50,    51,    81,    82,    39,    77,    78,    79,    39,    75,
      29,    38,    81,    81,    39,    81,    81,    81,     1,    81,
       5,    18,    19,    20,    21,    22,    23,    25,    26,    27,
      28,    31,    32,    33,    34,    35,    36,    38,    81,    42,
      79,     1,    81,    83,    84,    38,    81,    40,    40,     1,
      47,    40,    40,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    40,
      40,    40,    37,    38,    79,    38,    40,    43,    79,    81,
       7,    81,    79,    44,    40
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    56,    57,    58,    59,    59,    60,    60,    60,    61,
      61,    61,    61,    61,    63,    64,    62,    65,    66,    62,
      67,    67,    68,    68,    69,    70,    70,    70,    70,    71,
      72,    73,    73,    74,    75,    75,    76,    77,    77,    78,
      78,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    80,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    82,    82,    83,    83,    84,    84
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     5,     0,     2,     1,     1,     1,     2,
       3,     4,     5,     4,     0,     0,    10,     0,     0,    10,
       0,     1,     1,     3,     2,     1,     1,     1,     3,     1,
       3,     0,     2,     3,     1,     3,     3,     0,     1,     1,
       2,     1,     1,     2,     4,     5,     2,     3,     5,     7,
       5,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       7,     4,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     3,     3,     4,     4,     0,     1,     1,     3
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
#line 74 "jucompiler.y"
        {
            ast = (yyval.node) = (yyvsp[0].node);
        }
#line 1521 "y.tab.c"
    break;

  case 3: /* classdecl: CLASS IDENTIFIER LBRACE members RBRACE  */
#line 81 "jucompiler.y"
        {
            (yyval.node) = newnode(Program, NULL);
            addchild((yyval.node), newnode(Identifier, (yyvsp[-3].lexeme)));
            /* Program children: Identifier + {FieldDecl|MethodDecl}. */
            struct node_list *c = (yyvsp[-1].node)->children->next;
            while (c != NULL) {
                if (c->node != NULL) addchild((yyval.node), c->node);
                c = c->next;
            }
        }
#line 1536 "y.tab.c"
    break;

  case 4: /* members: %empty  */
#line 95 "jucompiler.y"
        {
            (yyval.node) = newnode(Tmp_List, NULL);
        }
#line 1544 "y.tab.c"
    break;

  case 5: /* members: members member  */
#line 99 "jucompiler.y"
        {
            (yyval.node) = (yyvsp[-1].node);
            if ((yyvsp[0].node) == NULL) {
                /* skip */
            } else if ((yyvsp[0].node)->category == Tmp_List) {
                struct node_list *c = (yyvsp[0].node)->children->next;
                while (c != NULL) { addchild((yyval.node), c->node); c = c->next; }
            } else {
                addchild((yyval.node), (yyvsp[0].node));
            }
        }
#line 1560 "y.tab.c"
    break;

  case 6: /* member: methoddecl  */
#line 113 "jucompiler.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1566 "y.tab.c"
    break;

  case 7: /* member: fielddecl  */
#line 114 "jucompiler.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1572 "y.tab.c"
    break;

  case 8: /* member: SEMICOLON  */
#line 115 "jucompiler.y"
                     { (yyval.node) = NULL; }
#line 1578 "y.tab.c"
    break;

  case 9: /* fielddecl: error SEMICOLON  */
#line 120 "jucompiler.y"
                {
                        /* Local recovery (spec): FieldDecl -> error ';' */
                        (yyval.node) = NULL;
                        yyerrok;
                }
#line 1588 "y.tab.c"
    break;

  case 10: /* fielddecl: type_nonvoid vardecl_ids SEMICOLON  */
#line 126 "jucompiler.y"
        {
            (yyval.node) = newnode(Tmp_List, NULL);
            struct node_list *c = (yyvsp[-1].node)->children->next;
            while (c != NULL) {
                struct node *fd = newnode(FieldDecl, NULL);
                addchild(fd, dup_type_node((yyvsp[-2].node)));
                addchild(fd, c->node);
                addchild((yyval.node), fd);
                c = c->next;
            }
        }
#line 1604 "y.tab.c"
    break;

  case 11: /* fielddecl: PUBLIC type_nonvoid vardecl_ids SEMICOLON  */
#line 138 "jucompiler.y"
        {
            (yyval.node) = newnode(Tmp_List, NULL);
            struct node_list *c = (yyvsp[-1].node)->children->next;
            while (c != NULL) {
                struct node *fd = newnode(FieldDecl, NULL);
                addchild(fd, dup_type_node((yyvsp[-2].node)));
                addchild(fd, c->node);
                addchild((yyval.node), fd);
                c = c->next;
            }
        }
#line 1620 "y.tab.c"
    break;

  case 12: /* fielddecl: PUBLIC STATIC type_nonvoid vardecl_ids SEMICOLON  */
#line 150 "jucompiler.y"
        {
            (yyval.node) = newnode(Tmp_List, NULL);
            struct node_list *c = (yyvsp[-1].node)->children->next;
            while (c != NULL) {
                struct node *fd = newnode(FieldDecl, NULL);
                addchild(fd, dup_type_node((yyvsp[-2].node)));
                addchild(fd, c->node);
                addchild((yyval.node), fd);
                c = c->next;
            }
        }
#line 1636 "y.tab.c"
    break;

  case 13: /* fielddecl: STATIC type_nonvoid vardecl_ids SEMICOLON  */
#line 162 "jucompiler.y"
        {
            (yyval.node) = newnode(Tmp_List, NULL);
            struct node_list *c = (yyvsp[-1].node)->children->next;
            while (c != NULL) {
                struct node *fd = newnode(FieldDecl, NULL);
                addchild(fd, dup_type_node((yyvsp[-2].node)));
                addchild(fd, c->node);
                addchild((yyval.node), fd);
                c = c->next;
            }
        }
#line 1652 "y.tab.c"
    break;

  case 14: /* $@1: %empty  */
#line 176 "jucompiler.y"
                                            { in_method_header = 1; }
#line 1658 "y.tab.c"
    break;

  case 15: /* $@2: %empty  */
#line 176 "jucompiler.y"
                                                                                                 { in_method_header = 0; }
#line 1664 "y.tab.c"
    break;

  case 16: /* methoddecl: PUBLIC STATIC type_nonvoid IDENTIFIER $@1 LPAR methodparams_opt RPAR $@2 methodbody  */
#line 177 "jucompiler.y"
        {
            struct node *header = newnode(MethodHeader, NULL);
            addchild(header, (yyvsp[-7].node));
            addchild(header, newnode(Identifier, (yyvsp[-6].lexeme)));
                        addchild(header, (yyvsp[-3].node));

            (yyval.node) = newnode(MethodDecl, NULL);
            addchild((yyval.node), header);
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1679 "y.tab.c"
    break;

  case 17: /* $@3: %empty  */
#line 187 "jucompiler.y"
                                    { in_method_header = 1; }
#line 1685 "y.tab.c"
    break;

  case 18: /* $@4: %empty  */
#line 187 "jucompiler.y"
                                                                                         { in_method_header = 0; }
#line 1691 "y.tab.c"
    break;

  case 19: /* methoddecl: PUBLIC STATIC VOID IDENTIFIER $@3 LPAR methodparams_opt RPAR $@4 methodbody  */
#line 188 "jucompiler.y"
        {
            struct node *header = newnode(MethodHeader, NULL);
            addchild(header, newnode(Void, NULL));
            addchild(header, newnode(Identifier, (yyvsp[-6].lexeme)));
            addchild(header, (yyvsp[-3].node));

            (yyval.node) = newnode(MethodDecl, NULL);
            addchild((yyval.node), header);
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1706 "y.tab.c"
    break;

  case 20: /* methodparams_opt: %empty  */
#line 202 "jucompiler.y"
        { (yyval.node) = newnode(MethodParams, NULL); }
#line 1712 "y.tab.c"
    break;

  case 21: /* methodparams_opt: methodparams  */
#line 204 "jucompiler.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1718 "y.tab.c"
    break;

  case 22: /* methodparams: paramdecl  */
#line 209 "jucompiler.y"
        {
            (yyval.node) = newnode(MethodParams, NULL);
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1727 "y.tab.c"
    break;

  case 23: /* methodparams: methodparams COMMA paramdecl  */
#line 214 "jucompiler.y"
        {
            (yyval.node) = (yyvsp[-2].node);
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1736 "y.tab.c"
    break;

  case 24: /* paramdecl: param_type IDENTIFIER  */
#line 222 "jucompiler.y"
        {
            (yyval.node) = newnode(ParamDecl, NULL);
            addchild((yyval.node), (yyvsp[-1].node));
            addchild((yyval.node), newnode(Identifier, (yyvsp[0].lexeme)));
        }
#line 1746 "y.tab.c"
    break;

  case 25: /* type_nonvoid: INT  */
#line 230 "jucompiler.y"
                        { (yyval.node) = newnode(Int, NULL); }
#line 1752 "y.tab.c"
    break;

  case 26: /* type_nonvoid: DOUBLE  */
#line 231 "jucompiler.y"
                      { (yyval.node) = newnode(Double, NULL); }
#line 1758 "y.tab.c"
    break;

  case 27: /* type_nonvoid: BOOL  */
#line 232 "jucompiler.y"
                      { (yyval.node) = newnode(Bool, NULL); }
#line 1764 "y.tab.c"
    break;

  case 28: /* type_nonvoid: STRING LSQ RSQ  */
#line 233 "jucompiler.y"
                         { (yyval.node) = newnode(StringArray, NULL); }
#line 1770 "y.tab.c"
    break;

  case 29: /* param_type: type_nonvoid  */
#line 237 "jucompiler.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1776 "y.tab.c"
    break;

  case 30: /* methodbody: LBRACE bodyelems RBRACE  */
#line 242 "jucompiler.y"
        {
            (yyval.node) = (yyvsp[-1].node);
        }
#line 1784 "y.tab.c"
    break;

  case 31: /* bodyelems: %empty  */
#line 249 "jucompiler.y"
        { (yyval.node) = newnode(MethodBody, NULL); }
#line 1790 "y.tab.c"
    break;

  case 32: /* bodyelems: bodyelems stmt  */
#line 251 "jucompiler.y"
        {
            (yyval.node) = (yyvsp[-1].node);
            if ((yyvsp[0].node) == NULL) {
                /* skip */
            } else if ((yyvsp[0].node)->category == Tmp_List) {
                struct node_list *c = (yyvsp[0].node)->children->next;
                while (c != NULL) { addchild((yyval.node), c->node); c = c->next; }
            } else {
                addchild((yyval.node), (yyvsp[0].node));
            }
        }
#line 1806 "y.tab.c"
    break;

  case 33: /* vardecl: type_nonvoid vardecl_ids SEMICOLON  */
#line 266 "jucompiler.y"
        {
            /* Build one VarDecl per identifier (supports: int a,b,c;). */
            (yyval.node) = newnode(Tmp_List, NULL);
            struct node_list *c = (yyvsp[-1].node)->children->next;
            while (c != NULL) {
                struct node *vd = newnode(VarDecl, NULL);
                addchild(vd, dup_type_node((yyvsp[-2].node)));
                addchild(vd, c->node);
                addchild((yyval.node), vd);
                c = c->next;
            }
        }
#line 1823 "y.tab.c"
    break;

  case 34: /* vardecl_ids: IDENTIFIER  */
#line 282 "jucompiler.y"
        {
            (yyval.node) = newnode(Tmp_List, NULL);
            addchild((yyval.node), newnode(Identifier, (yyvsp[0].lexeme)));
        }
#line 1832 "y.tab.c"
    break;

  case 35: /* vardecl_ids: vardecl_ids COMMA IDENTIFIER  */
#line 287 "jucompiler.y"
        {
            (yyval.node) = (yyvsp[-2].node);
            addchild((yyval.node), newnode(Identifier, (yyvsp[0].lexeme)));
        }
#line 1841 "y.tab.c"
    break;

  case 36: /* block: LBRACE stmts_opt RBRACE  */
#line 295 "jucompiler.y"
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
#line 1860 "y.tab.c"
    break;

  case 37: /* stmts_opt: %empty  */
#line 313 "jucompiler.y"
        { (yyval.node) = newnode(Tmp_List, NULL); }
#line 1866 "y.tab.c"
    break;

  case 38: /* stmts_opt: stmts  */
#line 315 "jucompiler.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1872 "y.tab.c"
    break;

  case 39: /* stmts: stmt  */
#line 320 "jucompiler.y"
        {
            (yyval.node) = newnode(Tmp_List, NULL);
            if ((yyvsp[0].node) == NULL) {
                /* skip */
            } else if ((yyvsp[0].node)->category == Tmp_List) {
                struct node_list *c = (yyvsp[0].node)->children->next;
                while (c != NULL) { addchild((yyval.node), c->node); c = c->next; }
            } else {
                addchild((yyval.node), (yyvsp[0].node));
            }
        }
#line 1888 "y.tab.c"
    break;

  case 40: /* stmts: stmts stmt  */
#line 332 "jucompiler.y"
        {
            (yyval.node) = (yyvsp[-1].node);
            if ((yyvsp[0].node) == NULL) {
                /* skip */
            } else if ((yyvsp[0].node)->category == Tmp_List) {
                struct node_list *c = (yyvsp[0].node)->children->next;
                while (c != NULL) { addchild((yyval.node), c->node); c = c->next; }
            } else {
                addchild((yyval.node), (yyvsp[0].node));
            }
        }
#line 1904 "y.tab.c"
    break;

  case 41: /* stmt: block  */
#line 347 "jucompiler.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1910 "y.tab.c"
    break;

  case 42: /* stmt: vardecl  */
#line 349 "jucompiler.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1916 "y.tab.c"
    break;

  case 43: /* stmt: error SEMICOLON  */
#line 351 "jucompiler.y"
        {
            /* Local recovery (spec): Statement -> error ';' */
            (yyval.node) = NULL;
            yyerrok;
        }
#line 1926 "y.tab.c"
    break;

  case 44: /* stmt: lvalue ASSIGN expr SEMICOLON  */
#line 357 "jucompiler.y"
        {
            (yyval.node) = newnode(Assign, NULL);
            addchild((yyval.node), (yyvsp[-3].node));
            addchild((yyval.node), (yyvsp[-1].node));
        }
#line 1936 "y.tab.c"
    break;

  case 45: /* stmt: PRINT LPAR expr RPAR SEMICOLON  */
#line 363 "jucompiler.y"
        {
            (yyval.node) = newnode(Print, NULL);
            addchild((yyval.node), (yyvsp[-2].node));
        }
#line 1945 "y.tab.c"
    break;

  case 46: /* stmt: RETURN SEMICOLON  */
#line 368 "jucompiler.y"
        {
            (yyval.node) = newnode(Return, NULL);
        }
#line 1953 "y.tab.c"
    break;

  case 47: /* stmt: RETURN expr SEMICOLON  */
#line 372 "jucompiler.y"
        {
            (yyval.node) = newnode(Return, NULL);
            addchild((yyval.node), (yyvsp[-1].node));
        }
#line 1962 "y.tab.c"
    break;

  case 48: /* stmt: IF LPAR expr RPAR stmt  */
#line 377 "jucompiler.y"
        {
            (yyval.node) = newnode(If, NULL);
            addchild((yyval.node), (yyvsp[-2].node));
            addchild((yyval.node), (yyvsp[0].node));
            /* Mandatory else child: represent empty statement as empty Block. */
            addchild((yyval.node), newnode(Block, NULL));
        }
#line 1974 "y.tab.c"
    break;

  case 49: /* stmt: IF LPAR expr RPAR stmt ELSE stmt  */
#line 385 "jucompiler.y"
        {
            (yyval.node) = newnode(If, NULL);
            addchild((yyval.node), (yyvsp[-4].node));
            addchild((yyval.node), (yyvsp[-2].node));
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1985 "y.tab.c"
    break;

  case 50: /* stmt: WHILE LPAR expr RPAR stmt  */
#line 392 "jucompiler.y"
        {
            (yyval.node) = newnode(While, NULL);
            addchild((yyval.node), (yyvsp[-2].node));
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 1995 "y.tab.c"
    break;

  case 51: /* stmt: call_expr SEMICOLON  */
#line 398 "jucompiler.y"
        { (yyval.node) = (yyvsp[-1].node); }
#line 2001 "y.tab.c"
    break;

  case 52: /* stmt: SEMICOLON  */
#line 400 "jucompiler.y"
        {
            /* Empty statement: represent as empty Block. */
            (yyval.node) = newnode(Block, NULL);
        }
#line 2010 "y.tab.c"
    break;

  case 53: /* lvalue: IDENTIFIER  */
#line 408 "jucompiler.y"
        { (yyval.node) = newnode(Identifier, (yyvsp[0].lexeme)); }
#line 2016 "y.tab.c"
    break;

  case 54: /* expr: NATURAL  */
#line 413 "jucompiler.y"
        { (yyval.node) = newnode(Natural, (yyvsp[0].lexeme)); }
#line 2022 "y.tab.c"
    break;

  case 55: /* expr: DECIMAL  */
#line 415 "jucompiler.y"
        { (yyval.node) = newnode(Decimal, (yyvsp[0].lexeme)); }
#line 2028 "y.tab.c"
    break;

  case 56: /* expr: BOOLLIT  */
#line 417 "jucompiler.y"
        { (yyval.node) = newnode(BoolLit, (yyvsp[0].lexeme)); }
#line 2034 "y.tab.c"
    break;

  case 57: /* expr: STRLIT  */
#line 419 "jucompiler.y"
        { (yyval.node) = newnode(StrLit, (yyvsp[0].lexeme)); }
#line 2040 "y.tab.c"
    break;

  case 58: /* expr: IDENTIFIER  */
#line 421 "jucompiler.y"
        { (yyval.node) = newnode(Identifier, (yyvsp[0].lexeme)); }
#line 2046 "y.tab.c"
    break;

  case 59: /* expr: call_expr  */
#line 423 "jucompiler.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 2052 "y.tab.c"
    break;

  case 60: /* expr: PARSEINT LPAR IDENTIFIER LSQ expr RSQ RPAR  */
#line 425 "jucompiler.y"
        {
            (yyval.node) = newnode(ParseArgs, NULL);
            addchild((yyval.node), newnode(Identifier, (yyvsp[-4].lexeme)));
            addchild((yyval.node), (yyvsp[-2].node));
        }
#line 2062 "y.tab.c"
    break;

  case 61: /* expr: PARSEINT LPAR error RPAR  */
#line 431 "jucompiler.y"
        {
            /* Local recovery (spec): ParseArgs -> PARSEINT '(' error ')' */
            (yyval.node) = NULL;
            yyerrok;
        }
#line 2072 "y.tab.c"
    break;

  case 62: /* expr: expr OR expr  */
#line 437 "jucompiler.y"
        { (yyval.node) = newnode(Or, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 2078 "y.tab.c"
    break;

  case 63: /* expr: expr AND expr  */
#line 439 "jucompiler.y"
        { (yyval.node) = newnode(And, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 2084 "y.tab.c"
    break;

  case 64: /* expr: expr EQ expr  */
#line 441 "jucompiler.y"
        {
            (yyval.node) = newnode(Eq, NULL);
            addchild((yyval.node), (yyvsp[-2].node));
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 2094 "y.tab.c"
    break;

  case 65: /* expr: expr NE expr  */
#line 447 "jucompiler.y"
        { (yyval.node) = newnode(Ne, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 2100 "y.tab.c"
    break;

  case 66: /* expr: expr LT expr  */
#line 449 "jucompiler.y"
        { (yyval.node) = newnode(Lt, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 2106 "y.tab.c"
    break;

  case 67: /* expr: expr GT expr  */
#line 451 "jucompiler.y"
        { (yyval.node) = newnode(Gt, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 2112 "y.tab.c"
    break;

  case 68: /* expr: expr LE expr  */
#line 453 "jucompiler.y"
        { (yyval.node) = newnode(Le, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 2118 "y.tab.c"
    break;

  case 69: /* expr: expr GE expr  */
#line 455 "jucompiler.y"
        { (yyval.node) = newnode(Ge, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 2124 "y.tab.c"
    break;

  case 70: /* expr: expr PLUS expr  */
#line 457 "jucompiler.y"
        {
            (yyval.node) = newnode(Add, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node));
        }
#line 2132 "y.tab.c"
    break;

  case 71: /* expr: expr MINUS expr  */
#line 461 "jucompiler.y"
        {
            (yyval.node) = newnode(Sub, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node));
        }
#line 2140 "y.tab.c"
    break;

  case 72: /* expr: expr STAR expr  */
#line 465 "jucompiler.y"
        {
            (yyval.node) = newnode(Mul, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node));
        }
#line 2148 "y.tab.c"
    break;

  case 73: /* expr: expr DIV expr  */
#line 469 "jucompiler.y"
        {
            (yyval.node) = newnode(Div, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node));
        }
#line 2156 "y.tab.c"
    break;

  case 74: /* expr: expr MOD expr  */
#line 473 "jucompiler.y"
        { (yyval.node) = newnode(Mod, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 2162 "y.tab.c"
    break;

  case 75: /* expr: expr LSHIFT expr  */
#line 475 "jucompiler.y"
        { (yyval.node) = newnode(Lshift, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 2168 "y.tab.c"
    break;

  case 76: /* expr: expr RSHIFT expr  */
#line 477 "jucompiler.y"
        { (yyval.node) = newnode(Rshift, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 2174 "y.tab.c"
    break;

  case 77: /* expr: expr XOR expr  */
#line 479 "jucompiler.y"
        { (yyval.node) = newnode(Xor, NULL); addchild((yyval.node), (yyvsp[-2].node)); addchild((yyval.node), (yyvsp[0].node)); }
#line 2180 "y.tab.c"
    break;

  case 78: /* expr: NOT expr  */
#line 481 "jucompiler.y"
        { (yyval.node) = newnode(Not, NULL); addchild((yyval.node), (yyvsp[0].node)); }
#line 2186 "y.tab.c"
    break;

  case 79: /* expr: PLUS expr  */
#line 483 "jucompiler.y"
        { (yyval.node) = newnode(Plus, NULL); addchild((yyval.node), (yyvsp[0].node)); }
#line 2192 "y.tab.c"
    break;

  case 80: /* expr: MINUS expr  */
#line 485 "jucompiler.y"
        { (yyval.node) = newnode(Minus, NULL); addchild((yyval.node), (yyvsp[0].node)); }
#line 2198 "y.tab.c"
    break;

  case 81: /* expr: expr DOTLENGTH  */
#line 487 "jucompiler.y"
        { (yyval.node) = newnode(Length, NULL); addchild((yyval.node), (yyvsp[-1].node)); }
#line 2204 "y.tab.c"
    break;

  case 82: /* expr: LPAR expr RPAR  */
#line 489 "jucompiler.y"
        { (yyval.node) = (yyvsp[-1].node); }
#line 2210 "y.tab.c"
    break;

  case 83: /* expr: LPAR error RPAR  */
#line 491 "jucompiler.y"
        {
            /* Local recovery (spec): Expr -> '(' error ')' */
            (yyval.node) = NULL;
            yyerrok;
        }
#line 2220 "y.tab.c"
    break;

  case 84: /* call_expr: IDENTIFIER LPAR arg_list_opt RPAR  */
#line 500 "jucompiler.y"
        {
            (yyval.node) = newnode(Call, NULL);
            addchild((yyval.node), newnode(Identifier, (yyvsp[-3].lexeme)));
            struct node_list *c = (yyvsp[-1].node)->children->next;
            while (c != NULL) { addchild((yyval.node), c->node); c = c->next; }
        }
#line 2231 "y.tab.c"
    break;

  case 85: /* call_expr: IDENTIFIER LPAR error RPAR  */
#line 507 "jucompiler.y"
        {
            /* Local recovery (spec): MethodInvocation -> IDENTIFIER '(' error ')' */
            (yyval.node) = NULL;
            yyerrok;
        }
#line 2241 "y.tab.c"
    break;

  case 86: /* arg_list_opt: %empty  */
#line 516 "jucompiler.y"
        {
            (yyval.node) = newnode(Tmp_List, NULL);
        }
#line 2249 "y.tab.c"
    break;

  case 87: /* arg_list_opt: arg_list  */
#line 520 "jucompiler.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 2255 "y.tab.c"
    break;

  case 88: /* arg_list: expr  */
#line 525 "jucompiler.y"
        {
            (yyval.node) = newnode(Tmp_List, NULL);
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 2264 "y.tab.c"
    break;

  case 89: /* arg_list: arg_list COMMA expr  */
#line 530 "jucompiler.y"
        {
            (yyval.node) = (yyvsp[-2].node);
            addchild((yyval.node), (yyvsp[0].node));
        }
#line 2273 "y.tab.c"
    break;


#line 2277 "y.tab.c"

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

#line 536 "jucompiler.y"


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
    syntax_error_count++;
    printf("Line %d, col %d: %s: %s\n", syn_line, syn_column, msg, yytext);

    /* Avoid cascading errors: parameter list / method header errors are fatal. */
    if (in_method_header) {
        exit(0);
    }
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
    syntax_error_count = 0;
    in_method_header = 0;

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
    if (yyparse() == 0 && syntax_error_count == 0 && print_tree && ast != NULL) {
        show(ast, 0);
    }

    return 0;
}
