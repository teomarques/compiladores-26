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
#line 9 "jucompiler.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#define YYDEBUG 1

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

/* Globais para FieldDecl e VarDecl multi-ids */
static struct node *cur_type_node;
static struct node *cur_field_type;
static struct node *prog_root;
static struct node *vd_accum;
static char        *vd_first_id;
static char        *class_id;

static struct node *make_block(struct node *sl);
static void add_field(struct node *tn, char *id);
static void add_vardecl(struct node *acc, struct node *tn, char *id);

#line 107 "y.tab.c"

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

#line 273 "y.tab.c"

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
  YYSYMBOL_IDENTIFIER = 45,                /* IDENTIFIER  */
  YYSYMBOL_NATURAL = 46,                   /* NATURAL  */
  YYSYMBOL_DECIMAL = 47,                   /* DECIMAL  */
  YYSYMBOL_STRLIT = 48,                    /* STRLIT  */
  YYSYMBOL_BOOLLIT = 49,                   /* BOOLLIT  */
  YYSYMBOL_RESERVED = 50,                  /* RESERVED  */
  YYSYMBOL_IFX = 51,                       /* IFX  */
  YYSYMBOL_UPLUS = 52,                     /* UPLUS  */
  YYSYMBOL_UMINUS = 53,                    /* UMINUS  */
  YYSYMBOL_UNOT = 54,                      /* UNOT  */
  YYSYMBOL_YYACCEPT = 55,                  /* $accept  */
  YYSYMBOL_program = 56,                   /* program  */
  YYSYMBOL_57_1 = 57,                      /* $@1  */
  YYSYMBOL_class_members = 58,             /* class_members  */
  YYSYMBOL_field_decl = 59,                /* field_decl  */
  YYSYMBOL_60_2 = 60,                      /* $@2  */
  YYSYMBOL_field_id_list = 61,             /* field_id_list  */
  YYSYMBOL_method_decl = 62,               /* method_decl  */
  YYSYMBOL_method_header = 63,             /* method_header  */
  YYSYMBOL_formal_params = 64,             /* formal_params  */
  YYSYMBOL_fp_list = 65,                   /* fp_list  */
  YYSYMBOL_type = 66,                      /* type  */
  YYSYMBOL_method_body = 67,               /* method_body  */
  YYSYMBOL_var_decl = 68,                  /* var_decl  */
  YYSYMBOL_69_3 = 69,                      /* $@3  */
  YYSYMBOL_vd_id_list = 70,                /* vd_id_list  */
  YYSYMBOL_stmt_list = 71,                 /* stmt_list  */
  YYSYMBOL_stmt = 72,                      /* stmt  */
  YYSYMBOL_stmt_no_if = 73,                /* stmt_no_if  */
  YYSYMBOL_block_stmt = 74,                /* block_stmt  */
  YYSYMBOL_method_invocation = 75,         /* method_invocation  */
  YYSYMBOL_call_args = 76,                 /* call_args  */
  YYSYMBOL_nonempty_call_args = 77,        /* nonempty_call_args  */
  YYSYMBOL_assignment_expr = 78,           /* assignment_expr  */
  YYSYMBOL_parse_args_stmt = 79,           /* parse_args_stmt  */
  YYSYMBOL_opt_expr = 80,                  /* opt_expr  */
  YYSYMBOL_print_arg = 81,                 /* print_arg  */
  YYSYMBOL_expr = 82,                      /* expr  */
  YYSYMBOL_op_expr = 83                    /* op_expr  */
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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   372

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  92
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  189

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   309


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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    89,    89,    89,    95,   101,   103,   105,   107,   120,
     119,   127,   131,   133,   143,   155,   162,   175,   176,   185,
     191,   199,   213,   214,   215,   222,   234,   233,   244,   246,
     256,   257,   262,   279,   280,   287,   298,   304,   309,   310,
     312,   314,   316,   321,   323,   329,   339,   346,   351,   352,
     356,   357,   365,   377,   384,   386,   394,   395,   399,   400,
     407,   409,   414,   416,   418,   421,   422,   424,   426,   431,
     439,   442,   447,   449,   453,   455,   457,   461,   462,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476
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
  "SEMICOLON", "LPAR", "RPAR", "LBRACE", "RBRACE", "LSQ", "RSQ",
  "IDENTIFIER", "NATURAL", "DECIMAL", "STRLIT", "BOOLLIT", "RESERVED",
  "IFX", "UPLUS", "UMINUS", "UNOT", "$accept", "program", "$@1",
  "class_members", "field_decl", "$@2", "field_id_list", "method_decl",
  "method_header", "formal_params", "fp_list", "type", "method_body",
  "var_decl", "$@3", "vd_id_list", "stmt_list", "stmt", "stmt_no_if",
  "block_stmt", "method_invocation", "call_args", "nonempty_call_args",
  "assignment_expr", "parse_args_stmt", "opt_expr", "print_arg", "expr",
  "op_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-57)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-49)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       8,   -40,     7,   -57,   -57,   -21,   -57,   101,   -22,    15,
     -57,   -57,   -57,    12,    34,   -57,   -57,   -57,   -19,   -57,
     -57,     4,    28,   -57,    35,   -57,   -57,    36,    16,   178,
      16,   -57,    42,    38,    51,    44,    54,    55,    56,    57,
     220,    58,   -57,   -57,   -57,   -25,    61,   -57,   -57,   -57,
     -57,    65,    82,    83,    88,    -1,    79,   -57,    84,   -57,
     -57,   220,   179,     2,    94,   232,   232,   232,    80,     3,
     -57,   -57,   -57,   -57,    96,   -57,   264,   220,   191,   220,
      37,   -57,   -57,   -57,   -57,   -57,    90,   -57,    95,    99,
     108,   -57,   109,   -57,   110,    98,    26,     1,   -57,   -57,
     -57,   114,   118,   -57,    69,   -57,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   119,   -57,   -57,   120,   124,   145,   -57,   -57,
     -57,   -57,   -57,   204,   147,   -57,   106,   143,   103,   -57,
     -57,   153,   156,   337,   337,   134,   134,   140,   140,   302,
     283,   134,   134,     0,     0,   -57,   -57,   -57,   321,   204,
     -57,   -57,   220,     6,   196,   -57,   154,   162,   -57,   131,
     -57,   -57,   -57,   -57,   168,   -57,   204,   167,   182,   186,
     190,   -57,   -57,   -57,   -57,   195,   197,   -57,   -57
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     2,     1,     0,     4,     0,     0,     0,
       7,     3,     8,     0,     0,    24,    23,    22,     0,     5,
       6,     0,     0,    11,     0,    30,    14,     9,    19,     0,
      19,    12,     0,     0,    17,     0,     0,     0,     0,     0,
      56,     0,    44,    30,    25,     0,     0,    32,    31,    33,
      38,     0,     0,     0,     0,     0,     0,    16,     0,    20,
      43,     0,     0,     0,     0,     0,     0,     0,     0,    67,
      62,    63,    64,    60,     0,    57,    61,     0,     0,     0,
       0,    26,    39,    40,    41,    15,     0,    10,     0,     0,
       0,    59,     0,    58,     0,     0,     0,    67,    76,    75,
      74,     0,     0,    68,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    45,    52,     0,     0,    49,    50,    28,
      13,    18,    21,     0,     0,    55,     0,     0,     0,    66,
      65,     0,     0,    80,    81,    85,    84,    86,    87,    78,
      77,    83,    82,    88,    89,    90,    91,    92,    79,     0,
      47,    46,     0,     0,    34,    37,     0,     0,    73,     0,
      70,    69,    36,    51,     0,    27,     0,     0,     0,     0,
       0,    29,    35,    54,    53,     0,     0,    72,    71
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -57,   -57,   -57,   -57,   -57,   -57,   -57,   -57,   -57,   208,
     -57,   -12,   -57,   -57,   -57,   -57,   198,   -54,   -57,   -57,
     -57,   135,   -57,   -29,   -57,   -57,   -57,   -38,   -56
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     5,     7,    19,    31,    55,    20,    21,    33,
      34,    35,    26,    47,   129,   163,    29,    48,    49,    50,
      51,   126,   127,    73,    53,    74,    92,   128,    76
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      52,    22,    75,    94,    79,     3,   103,     4,   103,    98,
      99,   100,     1,    14,    80,    15,    12,    46,    16,    15,
       6,    17,    16,    90,    93,    17,    24,   137,    18,    13,
     102,    32,    79,   118,   119,   120,    86,    87,   125,   122,
     104,   124,   104,   174,   175,    25,    89,    95,    64,    52,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    46,    65,    66,    67,
     141,   138,    23,    27,    28,    30,    68,   -48,    57,   164,
      64,   101,    69,    70,    71,    56,    72,    15,    58,    59,
      16,    64,    60,    17,    61,    62,    63,    77,   167,    65,
      66,    67,     8,    82,    52,   172,    81,   166,    68,   -48,
      65,    66,    67,     9,    69,    70,    71,    64,    72,    68,
      83,    84,   182,    88,   173,    69,    70,    71,    85,    72,
      52,   180,   179,    96,   105,   130,    65,    66,    67,    10,
     131,   136,    64,    11,   132,    68,   169,    52,   133,   134,
     135,    69,    70,    71,   139,    72,   110,   111,   140,   159,
     160,    65,    66,    67,   161,   116,   117,   118,   119,   120,
      68,   116,   117,   118,   119,   120,    69,    70,    71,    36,
      72,    15,   162,   168,    16,   165,    37,    17,    38,    39,
      64,    40,    36,   170,    15,    41,   171,    16,   177,    37,
      17,    38,    39,   176,    40,    36,   178,   183,    41,    65,
      66,    67,    37,   181,    38,    39,    42,    40,    68,    43,
      44,    41,   184,    45,    69,    70,    71,    91,    72,    42,
     185,    64,    43,   123,   186,   187,    45,   188,    54,   142,
       0,    78,    42,    64,     0,    43,     0,     0,     0,    45,
      65,    66,    67,     0,     0,     0,     0,     0,     0,    68,
       0,     0,    65,    66,    67,    69,    70,    71,     0,    72,
       0,    68,     0,     0,     0,     0,     0,    97,    70,    71,
       0,    72,   106,   107,   108,   109,   110,   111,     0,   112,
     113,   114,   115,     0,     0,   116,   117,   118,   119,   120,
     121,   106,   107,   108,   109,   110,   111,     0,   112,     0,
     114,   115,     0,     0,   116,   117,   118,   119,   120,   121,
     106,   107,   108,   109,   110,   111,     0,     0,     0,   114,
     115,     0,     0,   116,   117,   118,   119,   120,   121,   106,
     107,   108,   109,   110,   111,     0,     0,     0,   114,   115,
       0,     0,   116,   117,   118,   119,   120,   108,   109,   110,
     111,     0,     0,     0,   114,   115,     0,     0,   116,   117,
     118,   119,   120
};

static const yytype_int16 yycheck[] =
{
      29,    13,    40,     1,    29,    45,     5,     0,     5,    65,
      66,    67,     4,     1,    39,     3,    38,    29,     6,     3,
      41,     9,     6,    61,    62,     9,    45,     1,    16,    14,
      68,    15,    29,    33,    34,    35,    37,    38,     1,    77,
      39,    79,    39,    37,    38,    41,    58,    45,    11,    78,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    78,    30,    31,    32,
       1,    45,    38,    45,    39,    39,    39,    40,    40,   133,
      11,     1,    45,    46,    47,    43,    49,     3,    37,    45,
       6,    11,    38,     9,    39,    39,    39,    39,   136,    30,
      31,    32,     1,    38,   133,   159,    45,     1,    39,    40,
      30,    31,    32,    12,    45,    46,    47,    11,    49,    39,
      38,    38,   176,    44,   162,    45,    46,    47,    40,    49,
     159,   169,     1,    39,    38,    45,    30,    31,    32,    38,
      45,    43,    11,    42,    45,    39,    43,   176,    40,    40,
      40,    45,    46,    47,    40,    49,    22,    23,    40,    40,
      40,    30,    31,    32,    40,    31,    32,    33,    34,    35,
      39,    31,    32,    33,    34,    35,    45,    46,    47,     1,
      49,     3,    37,    40,     6,    38,     8,     9,    10,    11,
      11,    13,     1,    40,     3,    17,    40,     6,    44,     8,
       9,    10,    11,     7,    13,     1,    44,    40,    17,    30,
      31,    32,     8,    45,    10,    11,    38,    13,    39,    41,
      42,    17,    40,    45,    45,    46,    47,    48,    49,    38,
      44,    11,    41,    42,    44,    40,    45,    40,    30,   104,
      -1,    43,    38,    11,    -1,    41,    -1,    -1,    -1,    45,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    30,    31,    32,    45,    46,    47,    -1,    49,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      -1,    49,    18,    19,    20,    21,    22,    23,    -1,    25,
      26,    27,    28,    -1,    -1,    31,    32,    33,    34,    35,
      36,    18,    19,    20,    21,    22,    23,    -1,    25,    -1,
      27,    28,    -1,    -1,    31,    32,    33,    34,    35,    36,
      18,    19,    20,    21,    22,    23,    -1,    -1,    -1,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    36,    18,
      19,    20,    21,    22,    23,    -1,    -1,    -1,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    20,    21,    22,
      23,    -1,    -1,    -1,    27,    28,    -1,    -1,    31,    32,
      33,    34,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,    56,    45,     0,    57,    41,    58,     1,    12,
      38,    42,    38,    14,     1,     3,     6,     9,    16,    59,
      62,    63,    66,    38,    45,    41,    67,    45,    39,    71,
      39,    60,    15,    64,    65,    66,     1,     8,    10,    11,
      13,    17,    38,    41,    42,    45,    66,    68,    72,    73,
      74,    75,    78,    79,    64,    61,    43,    40,    37,    45,
      38,    39,    39,    39,    11,    30,    31,    32,    39,    45,
      46,    47,    49,    78,    80,    82,    83,    39,    71,    29,
      39,    45,    38,    38,    38,    40,    37,    38,    44,    66,
      82,    48,    81,    82,     1,    45,    39,    45,    83,    83,
      83,     1,    82,     5,    39,    38,    18,    19,    20,    21,
      22,    23,    25,    26,    27,    28,    31,    32,    33,    34,
      35,    36,    82,    42,    82,     1,    76,    77,    82,    69,
      45,    45,    45,    40,    40,    40,    43,     1,    45,    40,
      40,     1,    76,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    40,
      40,    40,    37,    70,    72,    38,     1,    82,    40,    43,
      40,    40,    72,    82,    37,    38,     7,    44,    44,     1,
      82,    45,    72,    40,    40,    44,    44,    40,    40
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    55,    57,    56,    58,    58,    58,    58,    58,    60,
      59,    59,    61,    61,    62,    63,    63,    64,    64,    64,
      65,    65,    66,    66,    66,    67,    69,    68,    70,    70,
      71,    71,    71,    72,    72,    72,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    74,    75,    75,    76,    76,
      77,    77,    78,    79,    79,    79,    80,    80,    81,    81,
      82,    82,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     6,     0,     4,     4,     2,     3,     0,
       5,     2,     0,     3,     2,     5,     5,     1,     4,     0,
       2,     4,     1,     1,     1,     3,     0,     5,     0,     3,
       0,     2,     2,     1,     5,     7,     5,     5,     1,     2,
       2,     2,     3,     2,     1,     3,     4,     4,     0,     1,
       1,     3,     3,     7,     7,     4,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     1,     2,     4,
       4,     7,     7,     4,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3
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
  case 2: /* $@1: %empty  */
#line 89 "jucompiler.y"
                       { class_id = (yyvsp[0].lexeme); }
#line 1497 "y.tab.c"
    break;

  case 3: /* program: CLASS IDENTIFIER $@1 LBRACE class_members RBRACE  */
#line 90 "jucompiler.y"
        { ast = (yyvsp[-1].n); }
#line 1503 "y.tab.c"
    break;

  case 4: /* class_members: %empty  */
#line 95 "jucompiler.y"
        {
            (yyval.n) = newnode(N_Program, NULL);
            addchild((yyval.n), newnode(N_Identifier, class_id));
            prog_root  = (yyval.n);
            vd_accum   = NULL;
        }
#line 1514 "y.tab.c"
    break;

  case 5: /* class_members: class_members PUBLIC STATIC field_decl  */
#line 102 "jucompiler.y"
        { (yyval.n) = (yyvsp[-3].n); }
#line 1520 "y.tab.c"
    break;

  case 6: /* class_members: class_members PUBLIC STATIC method_decl  */
#line 104 "jucompiler.y"
        { (yyval.n) = (yyvsp[-3].n); addchild((yyvsp[-3].n), (yyvsp[0].n)); }
#line 1526 "y.tab.c"
    break;

  case 7: /* class_members: class_members SEMICOLON  */
#line 106 "jucompiler.y"
        { (yyval.n) = (yyvsp[-1].n); }
#line 1532 "y.tab.c"
    break;

  case 8: /* class_members: class_members error SEMICOLON  */
#line 108 "jucompiler.y"
        { (yyval.n) = (yyvsp[-2].n); }
#line 1538 "y.tab.c"
    break;

  case 9: /* $@2: %empty  */
#line 120 "jucompiler.y"
        {
            /* $<n>0 is unused — we store in global so recursive
               rule can retrieve it. */
            cur_field_type = (yyvsp[-1].n);
            add_field((yyvsp[-1].n), (yyvsp[0].lexeme));
        }
#line 1549 "y.tab.c"
    break;

  case 11: /* field_decl: error SEMICOLON  */
#line 128 "jucompiler.y"
                { }
#line 1555 "y.tab.c"
    break;

  case 13: /* field_id_list: field_id_list COMMA IDENTIFIER  */
#line 134 "jucompiler.y"
        {
            add_field(cur_field_type, (yyvsp[0].lexeme));
        }
#line 1563 "y.tab.c"
    break;

  case 14: /* method_decl: method_header method_body  */
#line 144 "jucompiler.y"
        {
            (yyval.n) = newnode(N_MethodDecl, NULL);
            addchild((yyval.n), (yyvsp[-1].n));
            addchild((yyval.n), (yyvsp[0].n));
        }
#line 1573 "y.tab.c"
    break;

  case 15: /* method_header: type IDENTIFIER LPAR formal_params RPAR  */
#line 156 "jucompiler.y"
        {
            (yyval.n) = newnode(N_MethodHeader, NULL);
            addchild((yyval.n), (yyvsp[-4].n));
            addchild((yyval.n), newnode(N_Identifier, (yyvsp[-3].lexeme)));
            addchild((yyval.n), (yyvsp[-1].n));
        }
#line 1584 "y.tab.c"
    break;

  case 16: /* method_header: VOID IDENTIFIER LPAR formal_params RPAR  */
#line 163 "jucompiler.y"
        {
            (yyval.n) = newnode(N_MethodHeader, NULL);
            addchild((yyval.n), newnode(N_Void, NULL));
            addchild((yyval.n), newnode(N_Identifier, (yyvsp[-3].lexeme)));
            addchild((yyval.n), (yyvsp[-1].n));
        }
#line 1595 "y.tab.c"
    break;

  case 18: /* formal_params: STRING LSQ RSQ IDENTIFIER  */
#line 177 "jucompiler.y"
        {
            struct node *pd = newnode(N_ParamDecl, NULL);
            addchild(pd, newnode(N_StringArray, NULL));
            addchild(pd, newnode(N_Identifier, (yyvsp[0].lexeme)));
            (yyval.n) = newnode(N_MethodParams, NULL);
            addchild((yyval.n), pd);
        }
#line 1607 "y.tab.c"
    break;

  case 19: /* formal_params: %empty  */
#line 185 "jucompiler.y"
        {
            (yyval.n) = newnode(N_MethodParams, NULL);
        }
#line 1615 "y.tab.c"
    break;

  case 20: /* fp_list: type IDENTIFIER  */
#line 192 "jucompiler.y"
        {
            struct node *pd = newnode(N_ParamDecl, NULL);
            addchild(pd, (yyvsp[-1].n));
            addchild(pd, newnode(N_Identifier, (yyvsp[0].lexeme)));
            (yyval.n) = newnode(N_MethodParams, NULL);
            addchild((yyval.n), pd);
        }
#line 1627 "y.tab.c"
    break;

  case 21: /* fp_list: fp_list COMMA type IDENTIFIER  */
#line 200 "jucompiler.y"
        {
            struct node *pd = newnode(N_ParamDecl, NULL);
            addchild(pd, (yyvsp[-1].n));
            addchild(pd, newnode(N_Identifier, (yyvsp[0].lexeme)));
            (yyval.n) = (yyvsp[-3].n);
            addchild((yyval.n), pd);
        }
#line 1639 "y.tab.c"
    break;

  case 22: /* type: INT  */
#line 213 "jucompiler.y"
             { (yyval.n) = newnode(N_Int,    NULL); }
#line 1645 "y.tab.c"
    break;

  case 23: /* type: DOUBLE  */
#line 214 "jucompiler.y"
             { (yyval.n) = newnode(N_Double, NULL); }
#line 1651 "y.tab.c"
    break;

  case 24: /* type: BOOL  */
#line 215 "jucompiler.y"
             { (yyval.n) = newnode(N_Bool,   NULL); }
#line 1657 "y.tab.c"
    break;

  case 25: /* method_body: LBRACE stmt_list RBRACE  */
#line 223 "jucompiler.y"
        {
            struct node_list *c;
            (yyval.n) = newnode(N_MethodBody, NULL);
            for (c = (yyvsp[-1].n)->children; c; c = c->next)
                if (c->node) addchild((yyval.n), c->node);
        }
#line 1668 "y.tab.c"
    break;

  case 26: /* $@3: %empty  */
#line 234 "jucompiler.y"
        {
            cur_type_node = (yyvsp[-1].n);
            vd_first_id = (yyvsp[0].lexeme);
            vd_accum = newnode(N_MethodBody, NULL);
            add_vardecl(vd_accum, cur_type_node, vd_first_id);
        }
#line 1679 "y.tab.c"
    break;

  case 27: /* var_decl: type IDENTIFIER $@3 vd_id_list SEMICOLON  */
#line 241 "jucompiler.y"
        { (yyval.n) = vd_accum; }
#line 1685 "y.tab.c"
    break;

  case 29: /* vd_id_list: vd_id_list COMMA IDENTIFIER  */
#line 247 "jucompiler.y"
        {
            add_vardecl(vd_accum, cur_type_node, (yyvsp[0].lexeme));
        }
#line 1693 "y.tab.c"
    break;

  case 30: /* stmt_list: %empty  */
#line 256 "jucompiler.y"
                    { (yyval.n) = newnode(N_MethodBody, NULL); }
#line 1699 "y.tab.c"
    break;

  case 31: /* stmt_list: stmt_list stmt  */
#line 258 "jucompiler.y"
        {
            (yyval.n) = (yyvsp[-1].n);
            if ((yyvsp[0].n)) addchild((yyvsp[-1].n), (yyvsp[0].n));
        }
#line 1708 "y.tab.c"
    break;

  case 32: /* stmt_list: stmt_list var_decl  */
#line 263 "jucompiler.y"
        {
            struct node_list *c;
            (yyval.n) = (yyvsp[-1].n);
            if ((yyvsp[0].n)) {
                for (c = (yyvsp[0].n)->children; c; c = c->next)
                    if (c->node) addchild((yyval.n), c->node);
            }
        }
#line 1721 "y.tab.c"
    break;

  case 34: /* stmt: IF LPAR expr RPAR stmt  */
#line 281 "jucompiler.y"
        {
            (yyval.n) = newnode(N_If, NULL);
            addchild((yyval.n), (yyvsp[-2].n));
            addchild((yyval.n), (yyvsp[0].n) ? (yyvsp[0].n) : newnode(N_Block, NULL));
            addchild((yyval.n), newnode(N_Block, NULL));
        }
#line 1732 "y.tab.c"
    break;

  case 35: /* stmt: IF LPAR expr RPAR stmt ELSE stmt  */
#line 288 "jucompiler.y"
        {
            (yyval.n) = newnode(N_If, NULL);
            addchild((yyval.n), (yyvsp[-4].n));
            addchild((yyval.n), (yyvsp[-2].n) ? (yyvsp[-2].n) : newnode(N_Block, NULL));
            addchild((yyval.n), (yyvsp[0].n) ? (yyvsp[0].n) : newnode(N_Block, NULL));
        }
#line 1743 "y.tab.c"
    break;

  case 36: /* stmt_no_if: WHILE LPAR expr RPAR stmt  */
#line 299 "jucompiler.y"
        {
            (yyval.n) = newnode(N_While, NULL);
            addchild((yyval.n), (yyvsp[-2].n));
            addchild((yyval.n), (yyvsp[0].n) ? (yyvsp[0].n) : newnode(N_Block, NULL));
        }
#line 1753 "y.tab.c"
    break;

  case 37: /* stmt_no_if: PRINT LPAR print_arg RPAR SEMICOLON  */
#line 305 "jucompiler.y"
        {
            (yyval.n) = newnode(N_Print, NULL);
            addchild((yyval.n), (yyvsp[-2].n));
        }
#line 1762 "y.tab.c"
    break;

  case 39: /* stmt_no_if: method_invocation SEMICOLON  */
#line 311 "jucompiler.y"
        { (yyval.n) = (yyvsp[-1].n); }
#line 1768 "y.tab.c"
    break;

  case 40: /* stmt_no_if: assignment_expr SEMICOLON  */
#line 313 "jucompiler.y"
        { (yyval.n) = (yyvsp[-1].n); }
#line 1774 "y.tab.c"
    break;

  case 41: /* stmt_no_if: parse_args_stmt SEMICOLON  */
#line 315 "jucompiler.y"
        { (yyval.n) = (yyvsp[-1].n); }
#line 1780 "y.tab.c"
    break;

  case 42: /* stmt_no_if: RETURN opt_expr SEMICOLON  */
#line 317 "jucompiler.y"
        {
            (yyval.n) = newnode(N_Return, NULL);
            if ((yyvsp[-1].n)) addchild((yyval.n), (yyvsp[-1].n));
        }
#line 1789 "y.tab.c"
    break;

  case 43: /* stmt_no_if: error SEMICOLON  */
#line 322 "jucompiler.y"
        { (yyval.n) = NULL; }
#line 1795 "y.tab.c"
    break;

  case 44: /* stmt_no_if: SEMICOLON  */
#line 324 "jucompiler.y"
        { (yyval.n) = NULL; }
#line 1801 "y.tab.c"
    break;

  case 45: /* block_stmt: LBRACE stmt_list RBRACE  */
#line 330 "jucompiler.y"
        {
            (yyval.n) = make_block((yyvsp[-1].n));
        }
#line 1809 "y.tab.c"
    break;

  case 46: /* method_invocation: IDENTIFIER LPAR call_args RPAR  */
#line 340 "jucompiler.y"
        {
                        struct node_list *c;
            (yyval.n) = newnode(N_Call, NULL);
            addchild((yyval.n), newnode(N_Identifier, (yyvsp[-3].lexeme)));
            for (c = (yyvsp[-1].n)->children; c; c = c->next) addchild((yyval.n), c->node);
        }
#line 1820 "y.tab.c"
    break;

  case 47: /* method_invocation: IDENTIFIER LPAR error RPAR  */
#line 347 "jucompiler.y"
        { (yyval.n) = NULL; }
#line 1826 "y.tab.c"
    break;

  case 48: /* call_args: %empty  */
#line 351 "jucompiler.y"
                        { (yyval.n) = newnode(N_MethodBody, NULL); }
#line 1832 "y.tab.c"
    break;

  case 49: /* call_args: nonempty_call_args  */
#line 352 "jucompiler.y"
                          { (yyval.n) = (yyvsp[0].n); }
#line 1838 "y.tab.c"
    break;

  case 50: /* nonempty_call_args: expr  */
#line 356 "jucompiler.y"
                        { (yyval.n) = newnode(N_MethodBody, NULL); addchild((yyval.n), (yyvsp[0].n)); }
#line 1844 "y.tab.c"
    break;

  case 51: /* nonempty_call_args: nonempty_call_args COMMA expr  */
#line 358 "jucompiler.y"
        { (yyval.n) = (yyvsp[-2].n); addchild((yyval.n), (yyvsp[0].n)); }
#line 1850 "y.tab.c"
    break;

  case 52: /* assignment_expr: IDENTIFIER ASSIGN expr  */
#line 366 "jucompiler.y"
        {
            (yyval.n) = newnode(N_Assign, NULL);
            addchild((yyval.n), newnode(N_Identifier, (yyvsp[-2].lexeme)));
            addchild((yyval.n), (yyvsp[0].n));
        }
#line 1860 "y.tab.c"
    break;

  case 53: /* parse_args_stmt: PARSEINT LPAR IDENTIFIER LSQ expr RSQ RPAR  */
#line 378 "jucompiler.y"
        {
            (yyval.n) = newnode(N_ParseArgs, NULL);
            addchild((yyval.n), newnode(N_Identifier, (yyvsp[-4].lexeme)));
            addchild((yyval.n), (yyvsp[-2].n));
        }
#line 1870 "y.tab.c"
    break;

  case 54: /* parse_args_stmt: PARSEINT LPAR IDENTIFIER LSQ error RSQ RPAR  */
#line 385 "jucompiler.y"
        { (yyval.n) = NULL; }
#line 1876 "y.tab.c"
    break;

  case 55: /* parse_args_stmt: PARSEINT LPAR error RPAR  */
#line 387 "jucompiler.y"
        { (yyval.n) = NULL; }
#line 1882 "y.tab.c"
    break;

  case 56: /* opt_expr: %empty  */
#line 394 "jucompiler.y"
                  { (yyval.n) = NULL; }
#line 1888 "y.tab.c"
    break;

  case 57: /* opt_expr: expr  */
#line 395 "jucompiler.y"
                  { (yyval.n) = (yyvsp[0].n); }
#line 1894 "y.tab.c"
    break;

  case 58: /* print_arg: expr  */
#line 399 "jucompiler.y"
             { (yyval.n) = (yyvsp[0].n); }
#line 1900 "y.tab.c"
    break;

  case 59: /* print_arg: STRLIT  */
#line 400 "jucompiler.y"
             { (yyval.n) = newnode(N_StrLit, (yyvsp[0].lexeme)); }
#line 1906 "y.tab.c"
    break;

  case 60: /* expr: assignment_expr  */
#line 408 "jucompiler.y"
        { (yyval.n) = (yyvsp[0].n); }
#line 1912 "y.tab.c"
    break;

  case 61: /* expr: op_expr  */
#line 410 "jucompiler.y"
        { (yyval.n) = (yyvsp[0].n); }
#line 1918 "y.tab.c"
    break;

  case 62: /* op_expr: NATURAL  */
#line 415 "jucompiler.y"
        { (yyval.n) = newnode(N_Natural, (yyvsp[0].lexeme)); }
#line 1924 "y.tab.c"
    break;

  case 63: /* op_expr: DECIMAL  */
#line 417 "jucompiler.y"
        { (yyval.n) = newnode(N_Decimal, (yyvsp[0].lexeme)); }
#line 1930 "y.tab.c"
    break;

  case 64: /* op_expr: BOOLLIT  */
#line 419 "jucompiler.y"
        { (yyval.n) = newnode(N_BoolLit, (yyvsp[0].lexeme)); }
#line 1936 "y.tab.c"
    break;

  case 65: /* op_expr: LPAR expr RPAR  */
#line 421 "jucompiler.y"
                      { (yyval.n) = (yyvsp[-1].n); }
#line 1942 "y.tab.c"
    break;

  case 66: /* op_expr: LPAR error RPAR  */
#line 422 "jucompiler.y"
                      { (yyval.n) = NULL; }
#line 1948 "y.tab.c"
    break;

  case 67: /* op_expr: IDENTIFIER  */
#line 425 "jucompiler.y"
        { (yyval.n) = newnode(N_Identifier, (yyvsp[0].lexeme)); }
#line 1954 "y.tab.c"
    break;

  case 68: /* op_expr: IDENTIFIER DOTLENGTH  */
#line 427 "jucompiler.y"
        { (yyval.n) = newnode(N_Length, NULL);
          addchild((yyval.n), newnode(N_Identifier, (yyvsp[-1].lexeme))); }
#line 1961 "y.tab.c"
    break;

  case 69: /* op_expr: IDENTIFIER LPAR call_args RPAR  */
#line 432 "jucompiler.y"
        {
            struct node_list *c;
            (yyval.n) = newnode(N_Call, NULL);
            addchild((yyval.n), newnode(N_Identifier, (yyvsp[-3].lexeme)));
            for (c = (yyvsp[-1].n)->children; c; c = c->next) addchild((yyval.n), c->node);
        }
#line 1972 "y.tab.c"
    break;

  case 70: /* op_expr: IDENTIFIER LPAR error RPAR  */
#line 440 "jucompiler.y"
        { (yyval.n) = NULL; }
#line 1978 "y.tab.c"
    break;

  case 71: /* op_expr: PARSEINT LPAR IDENTIFIER LSQ expr RSQ RPAR  */
#line 443 "jucompiler.y"
        { (yyval.n) = newnode(N_ParseArgs, NULL);
          addchild((yyval.n), newnode(N_Identifier, (yyvsp[-4].lexeme)));
          addchild((yyval.n), (yyvsp[-2].n)); }
#line 1986 "y.tab.c"
    break;

  case 72: /* op_expr: PARSEINT LPAR IDENTIFIER LSQ error RSQ RPAR  */
#line 448 "jucompiler.y"
        { (yyval.n) = NULL; }
#line 1992 "y.tab.c"
    break;

  case 73: /* op_expr: PARSEINT LPAR error RPAR  */
#line 450 "jucompiler.y"
        { (yyval.n) = NULL; }
#line 1998 "y.tab.c"
    break;

  case 74: /* op_expr: MINUS op_expr  */
#line 454 "jucompiler.y"
        { (yyval.n) = newnode(N_Minus, NULL); addchild((yyval.n), (yyvsp[0].n)); }
#line 2004 "y.tab.c"
    break;

  case 75: /* op_expr: PLUS op_expr  */
#line 456 "jucompiler.y"
        { (yyval.n) = newnode(N_Plus,  NULL); addchild((yyval.n), (yyvsp[0].n)); }
#line 2010 "y.tab.c"
    break;

  case 76: /* op_expr: NOT op_expr  */
#line 458 "jucompiler.y"
        { (yyval.n) = newnode(N_Not,   NULL); addchild((yyval.n), (yyvsp[0].n)); }
#line 2016 "y.tab.c"
    break;

  case 77: /* op_expr: op_expr OR op_expr  */
#line 461 "jucompiler.y"
                              { (yyval.n) = newnode(N_Or,     NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2022 "y.tab.c"
    break;

  case 78: /* op_expr: op_expr AND op_expr  */
#line 462 "jucompiler.y"
                              { (yyval.n) = newnode(N_And,    NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2028 "y.tab.c"
    break;

  case 79: /* op_expr: op_expr XOR op_expr  */
#line 463 "jucompiler.y"
                              { (yyval.n) = newnode(N_Xor,    NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2034 "y.tab.c"
    break;

  case 80: /* op_expr: op_expr EQ op_expr  */
#line 464 "jucompiler.y"
                              { (yyval.n) = newnode(N_Eq,     NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2040 "y.tab.c"
    break;

  case 81: /* op_expr: op_expr NE op_expr  */
#line 465 "jucompiler.y"
                              { (yyval.n) = newnode(N_Ne,     NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2046 "y.tab.c"
    break;

  case 82: /* op_expr: op_expr LT op_expr  */
#line 466 "jucompiler.y"
                              { (yyval.n) = newnode(N_Lt,     NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2052 "y.tab.c"
    break;

  case 83: /* op_expr: op_expr GT op_expr  */
#line 467 "jucompiler.y"
                              { (yyval.n) = newnode(N_Gt,     NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2058 "y.tab.c"
    break;

  case 84: /* op_expr: op_expr LE op_expr  */
#line 468 "jucompiler.y"
                              { (yyval.n) = newnode(N_Le,     NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2064 "y.tab.c"
    break;

  case 85: /* op_expr: op_expr GE op_expr  */
#line 469 "jucompiler.y"
                              { (yyval.n) = newnode(N_Ge,     NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2070 "y.tab.c"
    break;

  case 86: /* op_expr: op_expr LSHIFT op_expr  */
#line 470 "jucompiler.y"
                              { (yyval.n) = newnode(N_Lshift, NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2076 "y.tab.c"
    break;

  case 87: /* op_expr: op_expr RSHIFT op_expr  */
#line 471 "jucompiler.y"
                              { (yyval.n) = newnode(N_Rshift, NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2082 "y.tab.c"
    break;

  case 88: /* op_expr: op_expr PLUS op_expr  */
#line 472 "jucompiler.y"
                              { (yyval.n) = newnode(N_Add,    NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2088 "y.tab.c"
    break;

  case 89: /* op_expr: op_expr MINUS op_expr  */
#line 473 "jucompiler.y"
                              { (yyval.n) = newnode(N_Sub,    NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2094 "y.tab.c"
    break;

  case 90: /* op_expr: op_expr STAR op_expr  */
#line 474 "jucompiler.y"
                              { (yyval.n) = newnode(N_Mul,    NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2100 "y.tab.c"
    break;

  case 91: /* op_expr: op_expr DIV op_expr  */
#line 475 "jucompiler.y"
                              { (yyval.n) = newnode(N_Div,    NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2106 "y.tab.c"
    break;

  case 92: /* op_expr: op_expr MOD op_expr  */
#line 476 "jucompiler.y"
                              { (yyval.n) = newnode(N_Mod,    NULL); addchild((yyval.n), (yyvsp[-2].n)); addchild((yyval.n), (yyvsp[0].n)); }
#line 2112 "y.tab.c"
    break;


#line 2116 "y.tab.c"

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

#line 479 "jucompiler.y"


/* ============================================================
 * Helper functions
 * ============================================================ */

/* Cria FieldDecl(type, id) e adiciona ao prog_root */
static void add_field(struct node *tn, char *id)
{
    struct node *fd = newnode(N_FieldDecl, NULL);
    addchild(fd, tn);
    addchild(fd, newnode(N_Identifier, id));
    addchild(prog_root, fd);
}

/* Cria VarDecl(type, id) e adiciona ao vd_accum */
static void add_vardecl(struct node *acc, struct node *tn, char *id)
{
    struct node *vd = newnode(N_VarDecl, NULL);
    addchild(vd, tn);
    addchild(vd, newnode(N_Identifier, id));
    if (acc)
        addchild(acc, vd);
}

/* ============================================================
 * NOMES DE CATEGORIAS
 * ============================================================ */
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

/* ============================================================
 * make_block
 * ============================================================ */
static struct node *make_block(struct node *sl)
{
    int nc = 0;
    int saw_any = 0;
    struct node *single = NULL;
    struct node_list *c;
    struct node *blk;
    struct node_list *c2;

    if (!sl) return newnode(N_Block, NULL);

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
        return saw_any ? NULL : newnode(N_Block, NULL);
    if (nc == 1) return single;

    blk = newnode(N_Block, NULL);
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

/* ============================================================
 * ERROR HANDLING
 * ============================================================ */
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

/* ============================================================
 * MAIN
 * ============================================================ */
int main(int argc, char **argv)
{
    int i;
    int parse_status;
    int mode = 2; /* 0: LEX, 1: E1, 2: PARSE, 3: TREE */

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
