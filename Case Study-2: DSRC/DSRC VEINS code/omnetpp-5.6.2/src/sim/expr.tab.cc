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


/* Substitute the variable and function names.  */
#define yyparse         expryyparse
#define yylex           expryylex
#define yyerror         expryyerror
#define yydebug         expryydebug
#define yynerrs         expryynerrs
#define yylval          expryylval
#define yychar          expryychar

/* First part of user prologue.  */
#line 47 "expr.y"


#include <cstdio>
#include <cstdlib>
#include "common/commonutil.h"
#include "expryydefs.h"

#define YYDEBUG 1           /* allow debugging */
#define YYDEBUGGING_ON 0    /* turn on/off debugging */

#if YYDEBUG != 0
#define YYERROR_VERBOSE     /* more detailed error messages */
#include <cstring>         /* YYVERBOSE needs it */
#endif

#define yyin expryyin
#define yyout expryyout
#define yyrestart expryyrestart
#define yy_scan_string expryy_scan_string
#define yy_delete_buffer expryy_delete_buffer
extern FILE *yyin;
extern FILE *yyout;
struct yy_buffer_state;
struct yy_buffer_state *yy_scan_string(const char *str);
void yy_delete_buffer(struct yy_buffer_state *);
void yyrestart(FILE *);
int yylex();
void yyerror (const char *s);

LineColumn xpos, xprevpos;


#include "common/stringutil.h"
#include "common/unitconversion.h"
#include "omnetpp/cdynamicexpression.h"
#include "omnetpp/cpar.h"
#include "omnetpp/cxmlelement.h"
#include "omnetpp/cexception.h"
#include "omnetpp/globals.h"
#include "omnetpp/cnedmathfunction.h"
#include "omnetpp/cnedfunction.h"
#include "omnetpp/nedsupport.h"

using namespace omnetpp;
using namespace omnetpp::common;
using namespace omnetpp::nedsupport;

static cDynamicExpression::Elem *e;

static char *expryyconcat(char *s1, char *s2, char *s3=nullptr)
{
    char *d = new char[strlen(s1)+strlen(s2)+strlen(s3?s3:"")+4];
    strcpy(d, s1);
    strcat(d, " ");
    strcat(d, s2);
    if (s3) {strcat(d, " "); strcat(d, s3);}
    delete [] s1; delete [] s2; delete [] s3;
    return d;
}

static void addFunction(const char *funcname, int argc)
{
    cNedMathFunction *f = cNedMathFunction::find(funcname, argc);
    if (f) {
        *e++ = f;
        return;
    }
    cNedFunction *nf = cNedFunction::find(funcname);
    if (nf) {
        if (argc < nf->getMinArgs() || (argc > nf->getMaxArgs() && !nf->hasVarArgs()))
            yyerror(opp_stringf("function '%s' does not accept %d arguments", nf->getSignature(), argc).c_str());
        (e++)->set(nf,argc);
        return;
    }
    yyerror(opp_stringf("function '%s()' (with %d args) not found (Define_NED_Function() or Define_NED_Math_Function() missing from C++ code?)", funcname, argc).c_str());
}

static double parseQuantity(const char *text, std::string& unit)
{
    try {
        // evaluate quantities like "5s 230ms"
        return UnitConversion::parseQuantity(text, unit);
    }
    catch (std::exception& e) {
        yyerror(e.what());
        return 0;
    }
}


#line 169 "expr.tab.cc"

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

#include "expr.tab.hh"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_DOUBLETYPE = 3,                 /* DOUBLETYPE  */
  YYSYMBOL_INTTYPE = 4,                    /* INTTYPE  */
  YYSYMBOL_STRINGTYPE = 5,                 /* STRINGTYPE  */
  YYSYMBOL_BOOLTYPE = 6,                   /* BOOLTYPE  */
  YYSYMBOL_XMLTYPE = 7,                    /* XMLTYPE  */
  YYSYMBOL_TRUE_ = 8,                      /* TRUE_  */
  YYSYMBOL_FALSE_ = 9,                     /* FALSE_  */
  YYSYMBOL_NAN_ = 10,                      /* NAN_  */
  YYSYMBOL_INF_ = 11,                      /* INF_  */
  YYSYMBOL_THIS_ = 12,                     /* THIS_  */
  YYSYMBOL_ASK_ = 13,                      /* ASK_  */
  YYSYMBOL_DEFAULT_ = 14,                  /* DEFAULT_  */
  YYSYMBOL_CONST_ = 15,                    /* CONST_  */
  YYSYMBOL_SIZEOF_ = 16,                   /* SIZEOF_  */
  YYSYMBOL_INDEX_ = 17,                    /* INDEX_  */
  YYSYMBOL_EXISTS = 18,                    /* EXISTS  */
  YYSYMBOL_TYPENAME = 19,                  /* TYPENAME  */
  YYSYMBOL_XMLDOC_ = 20,                   /* XMLDOC_  */
  YYSYMBOL_NAME = 21,                      /* NAME  */
  YYSYMBOL_INTCONSTANT = 22,               /* INTCONSTANT  */
  YYSYMBOL_REALCONSTANT = 23,              /* REALCONSTANT  */
  YYSYMBOL_STRINGCONSTANT = 24,            /* STRINGCONSTANT  */
  YYSYMBOL_EQ_ = 25,                       /* EQ_  */
  YYSYMBOL_NE_ = 26,                       /* NE_  */
  YYSYMBOL_GE_ = 27,                       /* GE_  */
  YYSYMBOL_LE_ = 28,                       /* LE_  */
  YYSYMBOL_AND_ = 29,                      /* AND_  */
  YYSYMBOL_OR_ = 30,                       /* OR_  */
  YYSYMBOL_XOR_ = 31,                      /* XOR_  */
  YYSYMBOL_NOT_ = 32,                      /* NOT_  */
  YYSYMBOL_BINAND_ = 33,                   /* BINAND_  */
  YYSYMBOL_BINOR_ = 34,                    /* BINOR_  */
  YYSYMBOL_BINXOR_ = 35,                   /* BINXOR_  */
  YYSYMBOL_BINCOMPL_ = 36,                 /* BINCOMPL_  */
  YYSYMBOL_SHIFTLEFT_ = 37,                /* SHIFTLEFT_  */
  YYSYMBOL_SHIFTRIGHT_ = 38,               /* SHIFTRIGHT_  */
  YYSYMBOL_INVALID_CHAR = 39,              /* INVALID_CHAR  */
  YYSYMBOL_40_ = 40,                       /* '?'  */
  YYSYMBOL_41_ = 41,                       /* ':'  */
  YYSYMBOL_42_ = 42,                       /* '>'  */
  YYSYMBOL_43_ = 43,                       /* '<'  */
  YYSYMBOL_44_ = 44,                       /* '+'  */
  YYSYMBOL_45_ = 45,                       /* '-'  */
  YYSYMBOL_46_ = 46,                       /* '*'  */
  YYSYMBOL_47_ = 47,                       /* '/'  */
  YYSYMBOL_48_ = 48,                       /* '%'  */
  YYSYMBOL_49_ = 49,                       /* '^'  */
  YYSYMBOL_UMIN_ = 50,                     /* UMIN_  */
  YYSYMBOL_51_ = 51,                       /* '('  */
  YYSYMBOL_52_ = 52,                       /* ')'  */
  YYSYMBOL_53_ = 53,                       /* ','  */
  YYSYMBOL_54_ = 54,                       /* '.'  */
  YYSYMBOL_55_ = 55,                       /* '['  */
  YYSYMBOL_56_ = 56,                       /* ']'  */
  YYSYMBOL_YYACCEPT = 57,                  /* $accept  */
  YYSYMBOL_expression = 58,                /* expression  */
  YYSYMBOL_expr = 59,                      /* expr  */
  YYSYMBOL_simple_expr = 60,               /* simple_expr  */
  YYSYMBOL_funcname = 61,                  /* funcname  */
  YYSYMBOL_identifier = 62,                /* identifier  */
  YYSYMBOL_special_expr = 63,              /* special_expr  */
  YYSYMBOL_literal = 64,                   /* literal  */
  YYSYMBOL_stringliteral = 65,             /* stringliteral  */
  YYSYMBOL_boolliteral = 66,               /* boolliteral  */
  YYSYMBOL_numliteral = 67,                /* numliteral  */
  YYSYMBOL_quantity = 68                   /* quantity  */
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
#define YYFINAL  56
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   801

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  12
/* YYNRULES -- Number of rules.  */
#define YYNRULES  83
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  175

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   295


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
       2,     2,     2,     2,     2,     2,     2,    48,     2,     2,
      51,    52,    46,    44,    53,    45,    54,    47,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    41,     2,
      43,     2,    42,    40,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    55,     2,    56,    49,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    50
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   141,   141,   142,   144,   146,   151,   152,   153,   156,
     158,   160,   162,   164,   166,   168,   177,   179,   181,   183,
     185,   187,   190,   192,   194,   197,   201,   203,   205,   208,
     211,   213,   215,   218,   220,   222,   225,   227,   229,   231,
     233,   235,   237,   239,   241,   243,   245,   250,   251,   252,
     256,   257,   259,   264,   266,   268,   270,   275,   277,   279,
     281,   283,   285,   287,   289,   294,   295,   296,   300,   305,
     307,   312,   314,   316,   318,   320,   336,   338,   340,   342,
     344,   346,   348,   350
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
  "\"end of file\"", "error", "\"invalid token\"", "DOUBLETYPE",
  "INTTYPE", "STRINGTYPE", "BOOLTYPE", "XMLTYPE", "TRUE_", "FALSE_",
  "NAN_", "INF_", "THIS_", "ASK_", "DEFAULT_", "CONST_", "SIZEOF_",
  "INDEX_", "EXISTS", "TYPENAME", "XMLDOC_", "NAME", "INTCONSTANT",
  "REALCONSTANT", "STRINGCONSTANT", "EQ_", "NE_", "GE_", "LE_", "AND_",
  "OR_", "XOR_", "NOT_", "BINAND_", "BINOR_", "BINXOR_", "BINCOMPL_",
  "SHIFTLEFT_", "SHIFTRIGHT_", "INVALID_CHAR", "'?'", "':'", "'>'", "'<'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "'^'", "UMIN_", "'('", "')'", "','",
  "'.'", "'['", "']'", "$accept", "expression", "expr", "simple_expr",
  "funcname", "identifier", "special_expr", "literal", "stringliteral",
  "boolliteral", "numliteral", "quantity", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-46)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-51)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     148,   -18,   -17,     6,   -46,   -46,   -46,     9,    40,    14,
     -46,    18,    29,    30,    31,    32,   -46,   -46,   -31,    63,
      64,   -46,   193,   193,   193,   193,    88,   702,   -46,    38,
     -46,   -46,   -46,   -46,   -46,   -46,    -4,   193,   193,   193,
     -46,   -46,    69,   193,   193,    -7,    41,    73,    74,   148,
     -46,   -46,   -46,   -46,   -46,   481,   -46,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,    55,    75,
      76,    77,    78,   509,   537,   565,   -46,   593,   621,    47,
     -23,   -46,    50,   -46,    36,   -46,   141,   141,   141,   141,
     752,   752,   752,    71,    71,    71,   -36,   -36,   677,   141,
     141,   -21,   -21,    54,    54,    54,    54,   -46,   220,   -46,
     -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,    83,   -46,
      84,   148,   -46,    56,   193,   -46,   193,    59,    61,    65,
     101,   727,   249,   -46,   -46,    70,   -46,   -46,   193,   102,
     278,    80,   -46,   193,   -46,   307,   -46,   193,   336,   -46,
     193,   365,   -46,   193,   394,   -46,   193,   423,   -46,   193,
     452,   -46,   193,   649,   -46
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,    52,    69,    70,    73,    74,     0,
       3,     4,     0,     0,    57,     0,    64,    51,    53,    71,
      72,    68,     0,     0,     0,     0,     0,     2,     6,     0,
      47,    48,    49,    65,    66,    67,    75,     0,     0,     0,
      82,    83,     0,     0,     0,     0,     0,     0,     0,     0,
      80,    81,    25,    29,    15,     0,     1,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,     0,     0,     0,
       0,    58,     0,    55,     0,     7,    16,    17,    19,    21,
      22,    23,    24,    26,    27,    28,    30,    31,     0,    18,
      20,     9,    10,    11,    12,    13,    14,    36,     0,    78,
      79,    76,    77,    34,    33,    35,     5,     8,     0,    60,
       0,     0,    59,     0,     0,    37,     0,     0,     0,     0,
       0,    32,     0,    61,    62,     0,    56,    38,     0,     0,
       0,     0,    39,     0,    63,     0,    40,     0,     0,    41,
       0,     0,    42,     0,     0,    43,     0,     0,    44,     0,
       0,    45,     0,     0,    46
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -46,   -45,   -22,   -46,   -46,   -46,   -46,   -46,   -46,   -46,
     -46,   -46
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      52,    53,    54,    55,    94,    89,    79,    80,    72,    73,
      74,    75,    76,    77,    90,    83,    84,    85,    81,    82,
     -50,    87,    88,    48,    49,    74,    75,    76,    77,   129,
      40,   130,   131,    37,    38,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   118,    39,     1,     2,
       3,    41,     4,     5,     6,     7,     8,     9,    42,    43,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      44,    45,    46,    47,    50,    51,   139,    22,    56,    78,
      86,    23,   133,    91,    92,    93,   119,   120,   121,   122,
      24,   128,   132,    77,   137,   138,    25,   117,    67,    68,
     140,   143,   141,   144,   142,    72,    73,    74,    75,    76,
      77,   145,   146,   151,   149,     0,   150,     0,     0,     0,
       0,   155,   154,     0,     0,   158,     0,     0,   161,     0,
       0,   164,     0,     0,   167,     0,     0,   170,     0,     0,
     173,     1,     2,     3,     0,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,     0,    64,    65,    66,     0,    67,    68,
      22,     0,     0,     0,    23,    72,    73,    74,    75,    76,
      77,     0,     0,    24,     0,     0,     1,     2,     3,    25,
       4,     5,     6,     7,     8,     9,     0,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,     0,
       0,     0,     0,     0,     0,    22,     0,     0,     0,    23,
       0,     0,     0,     0,     0,     0,     0,     0,    24,     0,
       0,     0,     0,     0,    25,    57,    58,    59,    60,    61,
      62,    63,     0,    64,    65,    66,     0,    67,    68,     0,
      69,     0,    70,    71,    72,    73,    74,    75,    76,    77,
       0,     0,   135,   136,    57,    58,    59,    60,    61,    62,
      63,     0,    64,    65,    66,     0,    67,    68,     0,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
       0,   147,   148,    57,    58,    59,    60,    61,    62,    63,
       0,    64,    65,    66,     0,    67,    68,     0,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,     0,
     152,   153,    57,    58,    59,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,     0,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,     0,   156,
     157,    57,    58,    59,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,     0,    69,     0,    70,    71,
      72,    73,    74,    75,    76,    77,     0,     0,   159,   160,
      57,    58,    59,    60,    61,    62,    63,     0,    64,    65,
      66,     0,    67,    68,     0,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,     0,   162,   163,    57,
      58,    59,    60,    61,    62,    63,     0,    64,    65,    66,
       0,    67,    68,     0,    69,     0,    70,    71,    72,    73,
      74,    75,    76,    77,     0,     0,   165,   166,    57,    58,
      59,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,     0,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,     0,   168,   169,    57,    58,    59,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,     0,    69,     0,    70,    71,    72,    73,    74,    75,
      76,    77,     0,     0,   171,   172,    57,    58,    59,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
       0,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,     0,    95,    57,    58,    59,    60,    61,    62,
      63,     0,    64,    65,    66,     0,    67,    68,     0,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
       0,   123,    57,    58,    59,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,     0,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,     0,   124,
      57,    58,    59,    60,    61,    62,    63,     0,    64,    65,
      66,     0,    67,    68,     0,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,     0,   125,    57,    58,
      59,    60,    61,    62,    63,     0,    64,    65,    66,     0,
      67,    68,     0,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,     0,   126,    57,    58,    59,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
       0,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,     0,   127,    57,    58,    59,    60,    61,    62,
      63,     0,    64,    65,    66,     0,    67,    68,     0,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
       0,   174,    57,    58,    59,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,     0,    69,   134,    70,
      71,    72,    73,    74,    75,    76,    77,    57,    58,    59,
      60,    61,    62,    63,     0,    64,    65,    66,     0,    67,
      68,     0,    69,     0,    70,    71,    72,    73,    74,    75,
      76,    77,    57,    58,    59,    60,    61,    62,    63,     0,
      64,    65,    66,     0,    67,    68,     0,     0,     0,    70,
      71,    72,    73,    74,    75,    76,    77,    57,    58,    59,
      60,     0,     0,     0,     0,    64,    65,    66,     0,    67,
      68,     0,     0,     0,    70,    71,    72,    73,    74,    75,
      76,    77
};

static const yytype_int16 yycheck[] =
{
      22,    23,    24,    25,    49,    12,    10,    11,    44,    45,
      46,    47,    48,    49,    21,    37,    38,    39,    22,    23,
      51,    43,    44,    54,    55,    46,    47,    48,    49,    52,
      21,    54,    55,    51,    51,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    51,     3,     4,
       5,    21,     7,     8,     9,    10,    11,    12,    54,    51,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      51,    51,    51,    51,    21,    21,   131,    32,     0,    51,
      21,    36,    56,    52,    21,    21,    21,    21,    21,    21,
      45,    54,    52,    49,    21,    21,    51,    52,    37,    38,
      54,    52,   134,    52,   136,    44,    45,    46,    47,    48,
      49,    56,    21,    21,    54,    -1,   148,    -1,    -1,    -1,
      -1,   153,    52,    -1,    -1,   157,    -1,    -1,   160,    -1,
      -1,   163,    -1,    -1,   166,    -1,    -1,   169,    -1,    -1,
     172,     3,     4,     5,    -1,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    33,    34,    35,    -1,    37,    38,
      32,    -1,    -1,    -1,    36,    44,    45,    46,    47,    48,
      49,    -1,    -1,    45,    -1,    -1,     3,     4,     5,    51,
       7,     8,     9,    10,    11,    12,    -1,    -1,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    -1,    -1,    51,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    35,    -1,    37,    38,    -1,
      40,    -1,    42,    43,    44,    45,    46,    47,    48,    49,
      -1,    -1,    52,    53,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    35,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      -1,    52,    53,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    35,    -1,    37,    38,    -1,    40,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    -1,    -1,
      52,    53,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    35,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    -1,    -1,    52,
      53,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    35,    -1,    37,    38,    -1,    40,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    -1,    -1,    52,    53,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      35,    -1,    37,    38,    -1,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    -1,    52,    53,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    35,
      -1,    37,    38,    -1,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    -1,    -1,    52,    53,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    35,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    -1,    -1,    52,    53,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    35,    -1,    37,
      38,    -1,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    -1,    -1,    52,    53,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    35,    -1,    37,    38,
      -1,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    -1,    52,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    35,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      -1,    52,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    35,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    -1,    -1,    52,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      35,    -1,    37,    38,    -1,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    -1,    -1,    52,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    35,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    -1,    -1,    52,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    35,    -1,    37,    38,
      -1,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    -1,    -1,    52,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    35,    -1,    37,    38,    -1,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    -1,
      -1,    52,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    35,    -1,    37,    38,    -1,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    35,    -1,    37,
      38,    -1,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    35,    -1,    37,    38,    -1,    -1,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    25,    26,    27,
      28,    -1,    -1,    -1,    -1,    33,    34,    35,    -1,    37,
      38,    -1,    -1,    -1,    42,    43,    44,    45,    46,    47,
      48,    49
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    32,    36,    45,    51,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    51,    51,    51,
      21,    21,    54,    51,    51,    51,    51,    51,    54,    55,
      21,    21,    59,    59,    59,    59,     0,    25,    26,    27,
      28,    29,    30,    31,    33,    34,    35,    37,    38,    40,
      42,    43,    44,    45,    46,    47,    48,    49,    51,    10,
      11,    22,    23,    59,    59,    59,    21,    59,    59,    12,
      21,    52,    21,    21,    58,    52,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    52,    59,    21,
      21,    21,    21,    52,    52,    52,    52,    52,    54,    52,
      54,    55,    52,    56,    41,    52,    53,    21,    21,    58,
      54,    59,    59,    52,    52,    56,    21,    52,    53,    54,
      59,    21,    52,    53,    52,    59,    52,    53,    59,    52,
      53,    59,    52,    53,    59,    52,    53,    59,    52,    53,
      59,    52,    53,    59,    52
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    58,    58,    58,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    60,    60,    60,
      61,    61,    61,    62,    62,    62,    62,    63,    63,    63,
      63,    63,    63,    63,    63,    64,    64,    64,    65,    66,
      66,    67,    67,    67,    67,    67,    68,    68,    68,    68,
      68,    68,    68,    68
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     4,     1,     3,     4,     3,
       3,     3,     3,     3,     3,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     3,     3,     3,     2,
       3,     3,     5,     4,     4,     4,     3,     4,     6,     8,
      10,    12,    14,    16,    18,    20,    22,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     6,     1,     3,     4,
       4,     6,     6,     9,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       2,     2,     2,     2
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
  case 3: /* expression: ASK_  */
#line 143 "expr.y"
                { yyerror("\"ask\" is not supported here"); }
#line 1460 "expr.tab.cc"
    break;

  case 4: /* expression: DEFAULT_  */
#line 145 "expr.y"
                { yyerror("\"default\" is not supported here"); }
#line 1466 "expr.tab.cc"
    break;

  case 5: /* expression: DEFAULT_ '(' expr ')'  */
#line 147 "expr.y"
                { yyerror("\"default()\" is not supported here"); }
#line 1472 "expr.tab.cc"
    break;

  case 8: /* expr: CONST_ '(' expr ')'  */
#line 154 "expr.y"
                { yyerror("const() is not supported here"); }
#line 1478 "expr.tab.cc"
    break;

  case 9: /* expr: expr '+' expr  */
#line 157 "expr.y"
                { *e++ = cDynamicExpression::ADD; }
#line 1484 "expr.tab.cc"
    break;

  case 10: /* expr: expr '-' expr  */
#line 159 "expr.y"
                { *e++ = cDynamicExpression::SUB; }
#line 1490 "expr.tab.cc"
    break;

  case 11: /* expr: expr '*' expr  */
#line 161 "expr.y"
                { *e++ = cDynamicExpression::MUL; }
#line 1496 "expr.tab.cc"
    break;

  case 12: /* expr: expr '/' expr  */
#line 163 "expr.y"
                { *e++ = cDynamicExpression::DIV; }
#line 1502 "expr.tab.cc"
    break;

  case 13: /* expr: expr '%' expr  */
#line 165 "expr.y"
                { *e++ = cDynamicExpression::MOD; }
#line 1508 "expr.tab.cc"
    break;

  case 14: /* expr: expr '^' expr  */
#line 167 "expr.y"
                { *e++ = cDynamicExpression::POW; }
#line 1514 "expr.tab.cc"
    break;

  case 15: /* expr: '-' expr  */
#line 170 "expr.y"
                {
                   cDynamicExpression::Elem& last = *(e-1);
                   if (last.isNumericConstant())
                       last.negate();
                   else
                       *e++ = cDynamicExpression::NEG;
                }
#line 1526 "expr.tab.cc"
    break;

  case 16: /* expr: expr EQ_ expr  */
#line 178 "expr.y"
                { *e++ = cDynamicExpression::EQ; }
#line 1532 "expr.tab.cc"
    break;

  case 17: /* expr: expr NE_ expr  */
#line 180 "expr.y"
                { *e++ = cDynamicExpression::NE; }
#line 1538 "expr.tab.cc"
    break;

  case 18: /* expr: expr '>' expr  */
#line 182 "expr.y"
                { *e++ = cDynamicExpression::GT; }
#line 1544 "expr.tab.cc"
    break;

  case 19: /* expr: expr GE_ expr  */
#line 184 "expr.y"
                { *e++ = cDynamicExpression::GE; }
#line 1550 "expr.tab.cc"
    break;

  case 20: /* expr: expr '<' expr  */
#line 186 "expr.y"
                { *e++ = cDynamicExpression::LT; }
#line 1556 "expr.tab.cc"
    break;

  case 21: /* expr: expr LE_ expr  */
#line 188 "expr.y"
                { *e++ = cDynamicExpression::LE; }
#line 1562 "expr.tab.cc"
    break;

  case 22: /* expr: expr AND_ expr  */
#line 191 "expr.y"
                { *e++ = cDynamicExpression::AND; }
#line 1568 "expr.tab.cc"
    break;

  case 23: /* expr: expr OR_ expr  */
#line 193 "expr.y"
                { *e++ = cDynamicExpression::OR; }
#line 1574 "expr.tab.cc"
    break;

  case 24: /* expr: expr XOR_ expr  */
#line 195 "expr.y"
                { *e++ = cDynamicExpression::XOR; }
#line 1580 "expr.tab.cc"
    break;

  case 25: /* expr: NOT_ expr  */
#line 199 "expr.y"
                { *e++ = cDynamicExpression::NOT; }
#line 1586 "expr.tab.cc"
    break;

  case 26: /* expr: expr BINAND_ expr  */
#line 202 "expr.y"
                { *e++ = cDynamicExpression::BIN_AND; }
#line 1592 "expr.tab.cc"
    break;

  case 27: /* expr: expr BINOR_ expr  */
#line 204 "expr.y"
                { *e++ = cDynamicExpression::BIN_OR; }
#line 1598 "expr.tab.cc"
    break;

  case 28: /* expr: expr BINXOR_ expr  */
#line 206 "expr.y"
                { *e++ = cDynamicExpression::BIN_XOR; }
#line 1604 "expr.tab.cc"
    break;

  case 29: /* expr: BINCOMPL_ expr  */
#line 210 "expr.y"
                { *e++ = cDynamicExpression::BIN_NOT; }
#line 1610 "expr.tab.cc"
    break;

  case 30: /* expr: expr SHIFTLEFT_ expr  */
#line 212 "expr.y"
                { *e++ = cDynamicExpression::LSHIFT; }
#line 1616 "expr.tab.cc"
    break;

  case 31: /* expr: expr SHIFTRIGHT_ expr  */
#line 214 "expr.y"
                { *e++ = cDynamicExpression::RSHIFT; }
#line 1622 "expr.tab.cc"
    break;

  case 32: /* expr: expr '?' expr ':' expr  */
#line 216 "expr.y"
                { *e++ = cDynamicExpression::IIF; }
#line 1628 "expr.tab.cc"
    break;

  case 33: /* expr: INTTYPE '(' expr ')'  */
#line 219 "expr.y"
                { addFunction("int",1); }
#line 1634 "expr.tab.cc"
    break;

  case 34: /* expr: DOUBLETYPE '(' expr ')'  */
#line 221 "expr.y"
                { addFunction("double",1); }
#line 1640 "expr.tab.cc"
    break;

  case 35: /* expr: STRINGTYPE '(' expr ')'  */
#line 223 "expr.y"
                { addFunction("string",1); }
#line 1646 "expr.tab.cc"
    break;

  case 36: /* expr: funcname '(' ')'  */
#line 226 "expr.y"
                { addFunction(yyvsp[-2],0); delete [] yyvsp[-2]; }
#line 1652 "expr.tab.cc"
    break;

  case 37: /* expr: funcname '(' expr ')'  */
#line 228 "expr.y"
                { addFunction(yyvsp[-3],1); delete [] yyvsp[-3]; }
#line 1658 "expr.tab.cc"
    break;

  case 38: /* expr: funcname '(' expr ',' expr ')'  */
#line 230 "expr.y"
                { addFunction(yyvsp[-5],2); delete [] yyvsp[-5]; }
#line 1664 "expr.tab.cc"
    break;

  case 39: /* expr: funcname '(' expr ',' expr ',' expr ')'  */
#line 232 "expr.y"
                { addFunction(yyvsp[-7],3); delete [] yyvsp[-7]; }
#line 1670 "expr.tab.cc"
    break;

  case 40: /* expr: funcname '(' expr ',' expr ',' expr ',' expr ')'  */
#line 234 "expr.y"
                { addFunction(yyvsp[-9],4); delete [] yyvsp[-9]; }
#line 1676 "expr.tab.cc"
    break;

  case 41: /* expr: funcname '(' expr ',' expr ',' expr ',' expr ',' expr ')'  */
#line 236 "expr.y"
                { addFunction(yyvsp[-11],5); delete [] yyvsp[-11]; }
#line 1682 "expr.tab.cc"
    break;

  case 42: /* expr: funcname '(' expr ',' expr ',' expr ',' expr ',' expr ',' expr ')'  */
#line 238 "expr.y"
                { addFunction(yyvsp[-13],6); delete [] yyvsp[-13]; }
#line 1688 "expr.tab.cc"
    break;

  case 43: /* expr: funcname '(' expr ',' expr ',' expr ',' expr ',' expr ',' expr ',' expr ')'  */
#line 240 "expr.y"
                { addFunction(yyvsp[-15],7); delete [] yyvsp[-15]; }
#line 1694 "expr.tab.cc"
    break;

  case 44: /* expr: funcname '(' expr ',' expr ',' expr ',' expr ',' expr ',' expr ',' expr ',' expr ')'  */
#line 242 "expr.y"
                { addFunction(yyvsp[-17],8); delete [] yyvsp[-17]; }
#line 1700 "expr.tab.cc"
    break;

  case 45: /* expr: funcname '(' expr ',' expr ',' expr ',' expr ',' expr ',' expr ',' expr ',' expr ',' expr ')'  */
#line 244 "expr.y"
                { addFunction(yyvsp[-19],9); delete [] yyvsp[-19]; }
#line 1706 "expr.tab.cc"
    break;

  case 46: /* expr: funcname '(' expr ',' expr ',' expr ',' expr ',' expr ',' expr ',' expr ',' expr ',' expr ',' expr ')'  */
#line 246 "expr.y"
                { addFunction(yyvsp[-21],10); delete [] yyvsp[-21]; }
#line 1712 "expr.tab.cc"
    break;

  case 51: /* funcname: XMLDOC_  */
#line 258 "expr.y"
                { yyval = common::opp_strdup("xmldoc"); }
#line 1718 "expr.tab.cc"
    break;

  case 52: /* funcname: XMLTYPE  */
#line 260 "expr.y"
                { yyval = common::opp_strdup("xml"); }
#line 1724 "expr.tab.cc"
    break;

  case 53: /* identifier: NAME  */
#line 265 "expr.y"
                { *e++ = new ParameterRef(yyvsp[0], true, false); delete [] yyvsp[0]; }
#line 1730 "expr.tab.cc"
    break;

  case 54: /* identifier: THIS_ '.' NAME  */
#line 267 "expr.y"
                { *e++ = new ParameterRef(yyvsp[0], false, true); delete [] yyvsp[0]; }
#line 1736 "expr.tab.cc"
    break;

  case 55: /* identifier: NAME '.' NAME  */
#line 269 "expr.y"
                { *e++ = new SiblingModuleParameterRef(yyvsp[-2], yyvsp[0], true, false); delete [] yyvsp[-2]; delete [] yyvsp[0]; }
#line 1742 "expr.tab.cc"
    break;

  case 56: /* identifier: NAME '[' expression ']' '.' NAME  */
#line 271 "expr.y"
                { *e++ = new SiblingModuleParameterRef(yyvsp[-5], yyvsp[0], true, true); delete [] yyvsp[-5]; delete [] yyvsp[0]; }
#line 1748 "expr.tab.cc"
    break;

  case 57: /* special_expr: INDEX_  */
#line 276 "expr.y"
                { *e++ = new ModuleIndex(); }
#line 1754 "expr.tab.cc"
    break;

  case 58: /* special_expr: INDEX_ '(' ')'  */
#line 278 "expr.y"
                { *e++ = new ModuleIndex(); }
#line 1760 "expr.tab.cc"
    break;

  case 59: /* special_expr: EXISTS '(' NAME ')'  */
#line 280 "expr.y"
                { *e++ = new Exists(yyvsp[-1], true); delete [] yyvsp[-1]; }
#line 1766 "expr.tab.cc"
    break;

  case 60: /* special_expr: SIZEOF_ '(' NAME ')'  */
#line 282 "expr.y"
                { *e++ = new Sizeof(yyvsp[-1], true, false); delete [] yyvsp[-1]; }
#line 1772 "expr.tab.cc"
    break;

  case 61: /* special_expr: SIZEOF_ '(' THIS_ '.' NAME ')'  */
#line 284 "expr.y"
                { *e++ = new Sizeof(yyvsp[-1], false, false); delete [] yyvsp[-1]; }
#line 1778 "expr.tab.cc"
    break;

  case 62: /* special_expr: SIZEOF_ '(' NAME '.' NAME ')'  */
#line 286 "expr.y"
                { delete [] yyvsp[-3]; delete [] yyvsp[-1]; yyerror("sizeof(submodule.gate) notation not supported here"); }
#line 1784 "expr.tab.cc"
    break;

  case 63: /* special_expr: SIZEOF_ '(' NAME '[' expression ']' '.' NAME ')'  */
#line 288 "expr.y"
                { delete [] yyvsp[-6]; delete [] yyvsp[-1]; yyerror("sizeof(submodule[index].gate) notation not supported here"); }
#line 1790 "expr.tab.cc"
    break;

  case 64: /* special_expr: TYPENAME  */
#line 290 "expr.y"
                { *e++ = new Typename(); }
#line 1796 "expr.tab.cc"
    break;

  case 68: /* stringliteral: STRINGCONSTANT  */
#line 301 "expr.y"
                { *e++ = opp_parsequotedstr(yyvsp[0]).c_str(); delete [] yyvsp[0]; }
#line 1802 "expr.tab.cc"
    break;

  case 69: /* boolliteral: TRUE_  */
#line 306 "expr.y"
                { *e++ = true; }
#line 1808 "expr.tab.cc"
    break;

  case 70: /* boolliteral: FALSE_  */
#line 308 "expr.y"
                { *e++ = false; }
#line 1814 "expr.tab.cc"
    break;

  case 71: /* numliteral: INTCONSTANT  */
#line 313 "expr.y"
                { *e++ = (intpar_t)opp_atoll(yyvsp[0]); delete [] yyvsp[0]; }
#line 1820 "expr.tab.cc"
    break;

  case 72: /* numliteral: REALCONSTANT  */
#line 315 "expr.y"
                { *e++ = opp_atof(yyvsp[0]); delete [] yyvsp[0]; }
#line 1826 "expr.tab.cc"
    break;

  case 73: /* numliteral: NAN_  */
#line 317 "expr.y"
                { *e++ = std::nan(""); }
#line 1832 "expr.tab.cc"
    break;

  case 74: /* numliteral: INF_  */
#line 319 "expr.y"
                { *e++ = 1/0.0; }
#line 1838 "expr.tab.cc"
    break;

  case 75: /* numliteral: quantity  */
#line 321 "expr.y"
                {
                  std::string unit;
                  double d = parseQuantity(yyvsp[0], unit);
                  bool isInteger = (d == floor(d)) && d >= std::numeric_limits<intpar_t>::min() && d <= std::numeric_limits<intpar_t>::max(); // note: it would be slightly better to try parsing it in integer in the first place
                  if (isInteger)
                      *e++ = (intpar_t)d;
                  else
                      *e++ = d;
                  if (!unit.empty())
                      (e-1)->setUnit(unit.c_str());
                  delete [] yyvsp[0];
                }
#line 1855 "expr.tab.cc"
    break;

  case 76: /* quantity: quantity INTCONSTANT NAME  */
#line 337 "expr.y"
                { yyval = expryyconcat(yyvsp[-2],yyvsp[-1],yyvsp[0]); }
#line 1861 "expr.tab.cc"
    break;

  case 77: /* quantity: quantity REALCONSTANT NAME  */
#line 339 "expr.y"
                { yyval = expryyconcat(yyvsp[-2],yyvsp[-1],yyvsp[0]); }
#line 1867 "expr.tab.cc"
    break;

  case 78: /* quantity: quantity NAN_ NAME  */
#line 341 "expr.y"
                { yyval = expryyconcat(yyvsp[-2],omnetpp::opp_strdup("nan"),yyvsp[0]); }
#line 1873 "expr.tab.cc"
    break;

  case 79: /* quantity: quantity INF_ NAME  */
#line 343 "expr.y"
                { yyval = expryyconcat(yyvsp[-2],omnetpp::opp_strdup("inf"),yyvsp[0]); }
#line 1879 "expr.tab.cc"
    break;

  case 80: /* quantity: INTCONSTANT NAME  */
#line 345 "expr.y"
                { yyval = expryyconcat(yyvsp[-1],yyvsp[0]); }
#line 1885 "expr.tab.cc"
    break;

  case 81: /* quantity: REALCONSTANT NAME  */
#line 347 "expr.y"
                { yyval = expryyconcat(yyvsp[-1],yyvsp[0]); }
#line 1891 "expr.tab.cc"
    break;

  case 82: /* quantity: NAN_ NAME  */
#line 349 "expr.y"
                { yyval = expryyconcat(omnetpp::opp_strdup("nan"),yyvsp[0]); }
#line 1897 "expr.tab.cc"
    break;

  case 83: /* quantity: INF_ NAME  */
#line 351 "expr.y"
                { yyval = expryyconcat(omnetpp::opp_strdup("inf"),yyvsp[0]); }
#line 1903 "expr.tab.cc"
    break;


#line 1907 "expr.tab.cc"

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

#line 354 "expr.y"


//----------------------------------------------------------------------

void doParseExpression(const char *nedtext, cDynamicExpression::Elem *&elems, int& nelems)
{
    NONREENTRANT_PARSER();

    elems = nullptr;
    nelems = 0;

    // reset the lexer
    xpos.co = 0;
    xpos.li = 1;
    xprevpos = xpos;

    yyin = nullptr;
    yyout = stderr; // not used anyway

    // alloc buffer
    struct yy_buffer_state *handle = yy_scan_string(nedtext);
    if (!handle)
        throw std::runtime_error("Parser is unable to allocate work memory");

    cDynamicExpression::Elem *v = new cDynamicExpression::Elem[100]; // overestimate for now; XXX danger of overrun
    e = v;

    // parse
    try
    {
        yyparse();
    }
    catch (std::exception& e)
    {
        yy_delete_buffer(handle);
        delete [] v;
        throw;
    }
    yy_delete_buffer(handle);

    // copy v[] and return
    nelems = e - v;
    elems = new cDynamicExpression::Elem[nelems];
    for (int i=0; i<nelems; i++)
        elems[i] = v[i];
    delete [] v;
}

void yyerror(const char *s)
{
    // chop newline
    char buf[250];
    strcpy(buf, s);
    if (buf[strlen(buf)-1] == '\n')
        buf[strlen(buf)-1] = '\0';

    throw std::runtime_error(buf);
}

