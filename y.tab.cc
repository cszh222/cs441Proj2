/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "calc3.y"

#include <stdbool.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <cstring>
#include "calc3.h"
#include "symbol_table.h"
#include "pstcode.h"
#include "apm.h"

/* prototypes */
nodeType *opr(int oper, int nops, ...);
nodeType *id(char* i);
nodeType *integer(int value);
nodeType *doub(float value);
nodeType *newId(const char* i);
nodeType *declareProc(nodeType *p);
nodeType *declareDoub(nodeType *p);
nodeType *declareInt(nodeType *p);
void freeNode(nodeType *p);
int ex(nodeType *p);
int yylex(void);
bool checkOpError(int op1, int op2);
bool errorFound;
bool declaringNewVar;
int errToken;

std::string typeOpError = "Operating between int and float values";

int progBlkLvl;
int lineno;
PstackCode myPStack;

void yyerror(std::string s);
/*int sym[26]; */                   /* symbol table */


/* Line 268 of yacc.c  */
#line 112 "y.tab.cc"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER = 258,
     VARIABLE = 259,
     DOUBLE = 260,
     WHILE = 261,
     IF = 262,
     PRINT = 263,
     DO = 264,
     REPEAT = 265,
     UNTIL = 266,
     INT = 267,
     DOUB = 268,
     PROCEDURE = 269,
     FOR = 270,
     TO = 271,
     STEP = 272,
     CALL = 273,
     BG = 274,
     ED = 275,
     IFX = 276,
     ELSE = 277,
     NE = 278,
     EQ = 279,
     LE = 280,
     GE = 281,
     UMINUS = 282
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 41 "calc3.y"

    int iValue;                 /* integer value */
    float fValue;               /*float value*/
    char* sVariable;            /* identifier name */
    nodeType *nPtr;             /* node pointer */



/* Line 293 of yacc.c  */
#line 184 "y.tab.cc"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 196 "y.tab.cc"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   406

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNRULES -- Number of states.  */
#define YYNSTATES  145

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   282

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      40,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      38,    39,    31,    29,    41,    30,     2,    32,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    34,
      24,    37,    23,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    35,     2,    36,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    25,    26,
      27,    28,    33
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    11,    12,    18,    21,
      23,    26,    30,    35,    41,    47,    55,    59,    62,    69,
      76,    88,    89,    94,    95,   100,   101,   107,   108,   113,
     114,   120,   121,   128,   130,   131,   136,   141,   145,   148,
     151,   152,   157,   158,   163,   169,   173,   177,   179,   180,
     185,   187,   190,   192,   194,   196,   199,   203,   207,   211,
     215,   219,   223,   227,   231,   235,   239,   243,   244
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      43,     0,    -1,    44,    -1,    44,    45,    -1,    -1,    34,
      -1,    -1,    55,    46,    35,    62,    36,    -1,    56,    34,
      -1,    53,    -1,    63,    34,    -1,     8,    63,    34,    -1,
       4,    37,    63,    34,    -1,     6,    38,    63,    39,    45,
      -1,     7,    38,    63,    39,    45,    -1,     7,    38,    63,
      39,    45,    22,    45,    -1,    35,    62,    36,    -1,    57,
      34,    -1,     9,    45,     6,    38,    63,    39,    -1,    10,
      45,    11,    38,    63,    39,    -1,    15,    38,     4,    37,
      63,    17,    63,    16,    63,    39,    45,    -1,    -1,    56,
      47,     1,    40,    -1,    -1,    63,    48,     1,    40,    -1,
      -1,    35,    62,    49,     1,    40,    -1,    -1,    57,    50,
       1,    40,    -1,    -1,     8,    63,    51,     1,    40,    -1,
      -1,     4,    37,    63,    52,     1,    40,    -1,    40,    -1,
      -1,    54,    19,    62,    20,    -1,    14,     4,    38,    39,
      -1,     4,    38,    39,    -1,    12,    60,    -1,    13,    60,
      -1,    -1,    12,    58,     1,    40,    -1,    -1,    13,    59,
       1,    40,    -1,     4,    37,    63,    41,    60,    -1,     4,
      37,    63,    -1,     4,    41,    60,    -1,     4,    -1,    -1,
       4,    61,     1,    40,    -1,    45,    -1,    62,    45,    -1,
       3,    -1,     5,    -1,     4,    -1,    30,    63,    -1,    63,
      29,    63,    -1,    63,    30,    63,    -1,    63,    31,    63,
      -1,    63,    32,    63,    -1,    63,    24,    63,    -1,    63,
      23,    63,    -1,    63,    28,    63,    -1,    63,    27,    63,
      -1,    63,    25,    63,    -1,    63,    26,    63,    -1,    38,
      63,    39,    -1,    -1,    38,    64,     1,    39,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    65,    65,    69,    70,    74,    75,    75,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    92,    92,    93,    93,    94,    94,    95,    95,    96,
      96,    97,    97,    98,   101,   101,   105,   108,   112,   113,
     114,   114,   115,   115,   118,   119,   120,   121,   122,   122,
     126,   127,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   146
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTEGER", "VARIABLE", "DOUBLE", "WHILE",
  "IF", "PRINT", "DO", "REPEAT", "UNTIL", "INT", "DOUB", "PROCEDURE",
  "FOR", "TO", "STEP", "CALL", "BG", "ED", "IFX", "ELSE", "'>'", "'<'",
  "NE", "EQ", "LE", "GE", "'+'", "'-'", "'*'", "'/'", "UMINUS", "';'",
  "'{'", "'}'", "'='", "'('", "')'", "'\\n'", "','", "$accept", "program",
  "function", "stmt", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6", "$@7",
  "blk", "$@8", "procedure", "call", "declaration", "$@9", "$@10",
  "variable", "$@11", "stmt_list", "expr", "$@12", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    62,    60,   278,   279,   280,   281,    43,
      45,    42,    47,   282,    59,   123,   125,    61,    40,    41,
      10,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    43,    44,    44,    45,    46,    45,    45,    45,
      45,    45,    45,    45,    45,    45,    45,    45,    45,    45,
      45,    47,    45,    48,    45,    49,    45,    50,    45,    51,
      45,    52,    45,    45,    54,    53,    55,    56,    57,    57,
      58,    57,    59,    57,    60,    60,    60,    60,    61,    60,
      62,    62,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    64,    63
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     0,     5,     2,     1,
       2,     3,     4,     5,     5,     7,     3,     2,     6,     6,
      11,     0,     4,     0,     4,     0,     5,     0,     4,     0,
       5,     0,     6,     1,     0,     4,     4,     3,     2,     2,
       0,     4,     0,     4,     5,     3,     3,     1,     0,     4,
       1,     2,     1,     1,     1,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     0,     4
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     2,     1,    52,    54,    53,     0,     0,     0,
      34,    34,    40,    42,     0,     0,     0,     5,    34,    67,
      33,     3,     9,     0,     6,    21,    27,    23,     0,     0,
       0,     0,    54,    29,     0,     0,    47,     0,    38,     0,
      39,     0,     0,    55,    50,    25,     0,     0,    34,     0,
       8,     0,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,     0,    31,    37,     0,     0,
      11,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    16,    51,     0,    66,     0,    34,    34,     0,     0,
      61,    60,    64,    65,    63,    62,    56,    57,    58,    59,
       0,    12,     0,    34,    34,     0,     0,     0,    45,    46,
       0,    41,    43,    36,     0,     0,    68,    35,    34,    22,
      28,    24,     0,    13,    14,    30,     0,     0,     0,    49,
       0,    26,     7,    32,    34,    18,    19,    44,     0,    15,
       0,     0,     0,    34,    20
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    44,    49,    51,    65,    83,    53,    71,
     102,    22,    23,    24,    25,    26,    37,    39,    38,    76,
      45,    27,    47
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -48
static const yytype_int16 yypact[] =
{
     -48,    10,   153,   -48,   -48,   -23,   -48,   -26,   -15,     0,
     242,   242,    20,    20,    22,    -9,     0,   -48,   242,     0,
     -48,   -48,   -48,    12,   -48,    -1,     1,   348,     0,    -7,
       0,     0,   -48,   360,    28,    25,   -30,    36,   -48,    38,
     -48,     3,    40,   -48,   -48,   115,    68,    41,   242,    11,
     -48,    44,   -48,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -48,    47,   372,   -48,   150,   260,
     -48,    48,    23,    24,     0,    20,    62,    26,    27,    29,
      13,   -48,   -48,    64,   -48,    30,   191,   242,    42,    43,
     -12,   -12,   -12,   -12,   -12,   -12,   -10,   -10,   -48,   -48,
      45,   -48,    69,   242,   242,    65,     0,     0,    49,   -48,
      66,   -48,   -48,   -48,     0,    70,   -48,   -48,   229,   -48,
     -48,   -48,    72,   -48,    67,   -48,   277,   294,    20,   -48,
     338,   -48,   -48,   -48,   242,   -48,   -48,   -48,     0,   -48,
     311,     0,   321,   242,   -48
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -48,   -48,   -48,    -2,   -48,   -48,   -48,   -48,   -48,   -48,
     -48,   -48,   -48,   -48,   -48,   -48,   -48,   -48,   -11,   -48,
     -47,    -3,   -48
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -35
static const yytype_int16 yytable[] =
{
      21,    86,    40,     4,    32,     6,    33,    74,    34,    35,
       3,    75,    30,    43,    28,    29,    46,    60,    61,    62,
      63,    62,    63,    31,    36,    66,    41,    68,    69,    42,
      16,    48,    67,    50,    72,    52,    73,    77,    19,    78,
     118,    79,    85,    82,    80,    88,    87,    89,   100,   105,
     114,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   106,   107,   110,   109,   115,   111,   112,   113,   116,
     122,   108,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,   119,   120,    82,   121,     0,     0,     0,   134,
     128,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,   123,   124,   126,   127,   125,   129,    84,     0,     0,
     131,   130,   133,     0,     0,     0,    82,   137,     4,     5,
       6,     7,     8,     9,    10,    11,     0,    12,    13,    14,
      15,     0,   139,     0,   -34,   140,     0,     0,   142,     0,
       0,   144,     0,     0,     0,    16,     0,     0,     0,    17,
      18,    81,     0,    19,     0,    20,     4,     5,     6,     7,
       8,     9,    10,    11,     0,    12,    13,    14,    15,     0,
       0,     0,   -34,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    16,     0,     0,     0,    17,    18,   103,
       0,    19,     0,    20,     4,     5,     6,     7,     8,     9,
      10,    11,     0,    12,    13,    14,    15,     0,     0,     0,
       0,   117,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    16,     0,     0,     0,    17,    18,     0,     0,    19,
       0,    20,     4,     5,     6,     7,     8,     9,    10,    11,
       0,    12,    13,    14,    15,     4,     5,     6,     7,     8,
       9,    10,    11,     0,    12,    13,    14,    15,     0,    16,
       0,     0,     0,    17,    18,   132,     0,    19,     0,    20,
       0,     0,    16,     0,     0,     0,    17,    18,     0,     0,
      19,     0,    20,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,     0,     0,     0,     0,     0,     0,   104,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
       0,     0,     0,     0,     0,     0,   135,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,   141,     0,     0,
       0,     0,     0,   136,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,     0,   138,     0,     0,     0,     0,
     143,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,     0,    64,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,     0,    70,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,     0,   101
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-48))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,    48,    13,     3,     4,     5,     9,    37,    10,    11,
       0,    41,    38,    16,    37,    38,    19,    29,    30,    31,
      32,    31,    32,    38,     4,    28,     4,    30,    31,    38,
      30,    19,    39,    34,     6,    34,    11,     1,    38,     1,
      87,    38,     1,    45,     4,     1,    35,     1,     1,     1,
      37,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    38,    38,     1,    75,     1,    40,    40,    39,    39,
       1,    74,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    40,    40,    86,    40,    -1,    -1,    -1,    22,
      41,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,   103,   104,   106,   107,    40,    40,    39,    -1,    -1,
      40,   114,    40,    -1,    -1,    -1,   118,   128,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    12,    13,    14,
      15,    -1,   134,    -1,    19,   138,    -1,    -1,   141,    -1,
      -1,   143,    -1,    -1,    -1,    30,    -1,    -1,    -1,    34,
      35,    36,    -1,    38,    -1,    40,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    12,    13,    14,    15,    -1,
      -1,    -1,    19,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    30,    -1,    -1,    -1,    34,    35,    39,
      -1,    38,    -1,    40,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    12,    13,    14,    15,    -1,    -1,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    34,    35,    -1,    -1,    38,
      -1,    40,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    12,    13,    14,    15,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    12,    13,    14,    15,    -1,    30,
      -1,    -1,    -1,    34,    35,    36,    -1,    38,    -1,    40,
      -1,    -1,    30,    -1,    -1,    -1,    34,    35,    -1,    -1,
      38,    -1,    40,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    16,    -1,    -1,
      -1,    -1,    -1,    39,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    -1,    17,    -1,    -1,    -1,    -1,
      39,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    -1,    34,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    -1,    34,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    34
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    43,    44,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    12,    13,    14,    15,    30,    34,    35,    38,
      40,    45,    53,    54,    55,    56,    57,    63,    37,    38,
      38,    38,     4,    63,    45,    45,     4,    58,    60,    59,
      60,     4,    38,    63,    45,    62,    63,    64,    19,    46,
      34,    47,    34,    50,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    34,    48,    63,    39,    63,    63,
      34,    51,     6,    11,    37,    41,    61,     1,     1,    38,
       4,    36,    45,    49,    39,     1,    62,    35,     1,     1,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
       1,    34,    52,    39,    39,     1,    38,    38,    63,    60,
       1,    40,    40,    39,    37,     1,    39,    20,    62,    40,
      40,    40,     1,    45,    45,    40,    63,    63,    41,    40,
      63,    40,    36,    40,    22,    39,    39,    60,    17,    45,
      63,    16,    63,    39,    45
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 65 "calc3.y"
    {}
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 69 "calc3.y"
    { ex((yyvsp[(2) - (2)].nPtr)); freeNode((yyvsp[(2) - (2)].nPtr));}
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 74 "calc3.y"
    { (yyval.nPtr) = opr(';', 2, NULL, NULL); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 75 "calc3.y"
    {pushSymbolTable(); progBlkLvl= getCurrentLevel();}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 77 "calc3.y"
    { (yyval.nPtr) = opr(PROCEDURE, 2, (yyvsp[(1) - (5)].nPtr), (yyvsp[(4) - (5)].nPtr)); progBlkLvl--;}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 78 "calc3.y"
    { (yyval.nPtr) = opr(CALL, 1, (yyvsp[(1) - (2)].nPtr));}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 79 "calc3.y"
    { (yyval.nPtr) = (yyvsp[(1) - (1)].nPtr);}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 80 "calc3.y"
    { (yyval.nPtr) = (yyvsp[(1) - (2)].nPtr);}
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 81 "calc3.y"
    { (yyval.nPtr) = opr(PRINT, 1, (yyvsp[(2) - (3)].nPtr)); }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 82 "calc3.y"
    { (yyval.nPtr) = opr('=', 2, id((yyvsp[(1) - (4)].sVariable)), (yyvsp[(3) - (4)].nPtr)); }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 83 "calc3.y"
    { (yyval.nPtr) = opr(WHILE, 2, (yyvsp[(3) - (5)].nPtr), (yyvsp[(5) - (5)].nPtr)); }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 84 "calc3.y"
    { (yyval.nPtr) = opr(IF, 2, (yyvsp[(3) - (5)].nPtr), (yyvsp[(5) - (5)].nPtr)); }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 85 "calc3.y"
    { (yyval.nPtr) = opr(IF, 3, (yyvsp[(3) - (7)].nPtr), (yyvsp[(5) - (7)].nPtr), (yyvsp[(7) - (7)].nPtr)); }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 86 "calc3.y"
    { (yyval.nPtr) = (yyvsp[(2) - (3)].nPtr);}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 87 "calc3.y"
    { (yyval.nPtr) = (yyvsp[(1) - (2)].nPtr);}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 88 "calc3.y"
    { (yyval.nPtr) = opr(DO, 2, (yyvsp[(2) - (6)].nPtr), (yyvsp[(5) - (6)].nPtr));}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 89 "calc3.y"
    { (yyval.nPtr) = opr(REPEAT, 2, (yyvsp[(2) - (6)].nPtr), (yyvsp[(5) - (6)].nPtr));}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 91 "calc3.y"
    { (yyval.nPtr) = opr(FOR, 5, id((yyvsp[(3) - (11)].sVariable)), (yyvsp[(5) - (11)].nPtr), (yyvsp[(7) - (11)].nPtr), (yyvsp[(9) - (11)].nPtr), (yyvsp[(11) - (11)].nPtr)); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 92 "calc3.y"
    { errToken = yychar;}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 93 "calc3.y"
    { errToken = yychar;}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 94 "calc3.y"
    { errToken = yychar;}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 95 "calc3.y"
    { errToken = yychar;}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 96 "calc3.y"
    { errToken = yychar;}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 97 "calc3.y"
    { errToken = yychar;}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 98 "calc3.y"
    {lineno++;}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 101 "calc3.y"
    {pushSymbolTable(); progBlkLvl = getCurrentLevel();}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 103 "calc3.y"
    {(yyval.nPtr) = opr(BG, 1, (yyvsp[(3) - (4)].nPtr)); progBlkLvl--;}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 105 "calc3.y"
    {(yyval.nPtr) = declareProc(newId((yyvsp[(2) - (4)].sVariable)));}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 108 "calc3.y"
    {(yyval.nPtr) = id((yyvsp[(1) - (3)].sVariable));}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 112 "calc3.y"
    {(yyval.nPtr) = opr(INT, 1, declareInt((yyvsp[(2) - (2)].nPtr)));}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 113 "calc3.y"
    {(yyval.nPtr) = opr(DOUB, 1, declareDoub((yyvsp[(2) - (2)].nPtr)));}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 114 "calc3.y"
    { errToken = yychar;}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 115 "calc3.y"
    { errToken = yychar;}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 118 "calc3.y"
    {(yyval.nPtr) = opr(',', 2, opr('=', 2, newId((yyvsp[(1) - (5)].sVariable)), (yyvsp[(3) - (5)].nPtr)), (yyvsp[(5) - (5)].nPtr));}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 119 "calc3.y"
    {(yyval.nPtr) = opr('=', 2, newId((yyvsp[(1) - (3)].sVariable)), (yyvsp[(3) - (3)].nPtr));}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 120 "calc3.y"
    {(yyval.nPtr) = opr(',', 2, newId((yyvsp[(1) - (3)].sVariable)), (yyvsp[(3) - (3)].nPtr));}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 121 "calc3.y"
    {(yyval.nPtr) = newId((yyvsp[(1) - (1)].sVariable));}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 122 "calc3.y"
    { errToken = yychar;}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 126 "calc3.y"
    { (yyval.nPtr) = (yyvsp[(1) - (1)].nPtr); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 127 "calc3.y"
    { (yyval.nPtr) = opr(';', 2, (yyvsp[(1) - (2)].nPtr), (yyvsp[(2) - (2)].nPtr)); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 131 "calc3.y"
    { (yyval.nPtr) = integer((yyvsp[(1) - (1)].iValue)); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 132 "calc3.y"
    { (yyval.nPtr) = doub((yyvsp[(1) - (1)].fValue));  }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 133 "calc3.y"
    { (yyval.nPtr) = id((yyvsp[(1) - (1)].sVariable)); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 134 "calc3.y"
    { (yyval.nPtr) = opr(UMINUS, 1, (yyvsp[(2) - (2)].nPtr)); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 135 "calc3.y"
    { (yyval.nPtr) = opr('+', 2, (yyvsp[(1) - (3)].nPtr), (yyvsp[(3) - (3)].nPtr)); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 136 "calc3.y"
    { (yyval.nPtr) = opr('-', 2, (yyvsp[(1) - (3)].nPtr), (yyvsp[(3) - (3)].nPtr)); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 137 "calc3.y"
    { (yyval.nPtr) = opr('*', 2, (yyvsp[(1) - (3)].nPtr), (yyvsp[(3) - (3)].nPtr)); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 138 "calc3.y"
    { (yyval.nPtr) = opr('/', 2, (yyvsp[(1) - (3)].nPtr), (yyvsp[(3) - (3)].nPtr)); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 139 "calc3.y"
    { (yyval.nPtr) = opr('<', 2, (yyvsp[(1) - (3)].nPtr), (yyvsp[(3) - (3)].nPtr)); }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 140 "calc3.y"
    { (yyval.nPtr) = opr('>', 2, (yyvsp[(1) - (3)].nPtr), (yyvsp[(3) - (3)].nPtr)); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 141 "calc3.y"
    { (yyval.nPtr) = opr(GE, 2, (yyvsp[(1) - (3)].nPtr), (yyvsp[(3) - (3)].nPtr)); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 142 "calc3.y"
    { (yyval.nPtr) = opr(LE, 2, (yyvsp[(1) - (3)].nPtr), (yyvsp[(3) - (3)].nPtr)); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 143 "calc3.y"
    { (yyval.nPtr) = opr(NE, 2, (yyvsp[(1) - (3)].nPtr), (yyvsp[(3) - (3)].nPtr)); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 144 "calc3.y"
    { (yyval.nPtr) = opr(EQ, 2, (yyvsp[(1) - (3)].nPtr), (yyvsp[(3) - (3)].nPtr)); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 145 "calc3.y"
    { (yyval.nPtr) = (yyvsp[(2) - (3)].nPtr); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 146 "calc3.y"
    { errToken = yychar;}
    break;



/* Line 1806 of yacc.c  */
#line 1984 "y.tab.cc"
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 149 "calc3.y"


#define SIZEOF_NODETYPE ((char *)&p->con - (char *)p)


nodeType *declareProc(nodeType *p){
    symbol_entry *entry = p->id.i;
    entry->type = TYPE_PROC;
    return p;
    
}

nodeType *integer(int value) {
    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(conNodeType);
    if ((p = (nodeType*) malloc(nodeSize)) == NULL)
        yyerror("out of memory");
    
    /* copy information */
    p->type = typeInt;
    p->con.value = value;
    return p;
}

nodeType *doub(float value){
    nodeType *p;
    size_t nodeSize;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(floatNodeType);
    if ((p = (nodeType*) malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeFloat;
    p->fl.value = value;

    return p;
}

nodeType *id(char* i){
    symbol_entry* entry;
    if((entry=getSymbolEntry(i)) == NULL)
        yyerror("missing declaration for identifier");

    if(entry->blk_level < progBlkLvl && entry->type != TYPE_PROC ){
        symbol_entry *new_entry;
        new_entry = newId(entry->name)->id.i;
        new_entry->type = entry->type;
        if(entry->type == TYPE_INT)
            new_entry->iVal = entry->iVal;
        else if(entry->type == TYPE_INT)
            new_entry->fVal = entry->fVal;
        
        new_entry->relentry = entry;
        new_entry->scoped = true;
        new_entry->prev_type = entry->type;
        entry = new_entry;
    }

    if(progBlkLvl < getCurrentLevel())
        entry->refetch = true;

    nodeType *p;
    size_t nodeSize;

    nodeSize = SIZEOF_NODETYPE + sizeof(idNodeType);
    if ((p = (nodeType*)malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = typeId;
    p->id.i = entry;

    return p;
}

nodeType *newId(const char* i){
    symbol_entry* newEntry = NULL;
    nodeType *p;
    size_t nodeSize;

    //printSymbolTable();
    if(getSymbolEntry(i)!=0){
        if(getSymbolEntry(i)->blk_level == progBlkLvl && !getSymbolEntry(i)->scoped){
            char error[100]; /*assuming noone will declare variables that nears 100 character*/
            snprintf(error, 100, "Variable already declared \'%s\'", i);
            yyerror(std::string(error));
        }
        else if(getSymbolEntry(i)->blk_level == progBlkLvl && getSymbolEntry(i)->scoped){
            newEntry = getSymbolEntry(i);
        }
    }
    if(newEntry == NULL){
        newEntry = (symbol_entry*)malloc(sizeof(symbol_entry));
        newEntry->name = strdup(i);
        newEntry->size = 1; //this is used to count the number of symbols
        newEntry->relentry = NULL;
        newEntry->prev_type = -1;
        addSymbol(newEntry, lineno);
    }

    newEntry->refetch = false;
    newEntry->scoped = false;

    nodeSize = SIZEOF_NODETYPE + sizeof(idNodeType);
    if ((p = (nodeType*)malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = typeId;
    p->id.i = getSymbolEntry(i);

    return p;
}

nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    size_t nodeSize;
    int i;

    /* allocate node */
    nodeSize = SIZEOF_NODETYPE + sizeof(oprNodeType) +
        (nops - 1) * sizeof(nodeType*);
    if ((p = (nodeType*)malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

/* setting type of declared variables to INT*/
nodeType *declareInt(nodeType *p){
    switch(p->type){
        case typeId: {
            symbol_entry *entry = p->id.i;
            entry->type = TYPE_INT;
            return p;
        }
        case typeOpr:
            switch(p->opr.oper){
                case ',': 
                    declareInt(p->opr.op[0]);
                    declareInt(p->opr.op[1]);
                    return p;
                case '=': 
                    declareInt(p->opr.op[0]);
                    return p;
        }
    }
}

/* setting type of declared variables to DOUB*/
nodeType *declareDoub(nodeType *p){
    switch(p->type){
        case typeId: {
            symbol_entry *entry = p->id.i;
            entry->type = TYPE_FLOAT;
            return p;
        }
        case typeOpr:
            switch(p->opr.oper){
                case ',': 
                    declareDoub(p->opr.op[0]);
                    declareDoub(p->opr.op[1]);
                    return p;
                case '=': 
                    declareDoub(p->opr.op[0]);
                    return p;
        }
    }
}

void freeNode(nodeType *p) {
    int i;
    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
    }
    free (p);
}

bool checkOpError(int op1, int op2){
    if(op1 == TYPE_INT|| op1 == typeInt)
        return !(op2 == TYPE_INT || op2 == typeInt);
    if(op1 == TYPE_FLOAT || op1 == typeFloat)
        return !(op2 == TYPE_FLOAT || op2 == typeFloat);
}

void yyerror(std::string s) {
    std::string tokenName;
    if(errToken != -1 && errToken != YYEMPTY && s=="syntax error"){
        switch(errToken){
        case INTEGER: tokenName = "CONSTANT"; break;
        case DOUBLE: tokenName = "FLOAT"; break;
        case VARIABLE: tokenName = "IDENTIFIER"; break;
        case WHILE: tokenName = "WHILE"; break;
        case IF: tokenName = "IF"; break;
        case PRINT: tokenName = "PRINT"; break;
        case DO: tokenName = "DO"; break;
        case REPEAT: tokenName = "REPEAT"; break;
        case UNTIL: tokenName = "UNTIL"; break;
        case INT: tokenName = "INTTYPE"; break;
        case DOUB: tokenName = "FLOATTYPE"; break;
        case PROCEDURE: tokenName = "PROCEDURE"; break;
        case FOR: tokenName = "FOR"; break;
        case TO: tokenName = "TO"; break;
        case BG: tokenName = "BEGINBLOCK"; break;
        case ED: tokenName = "ENDBLOCK"; break;
        case IFX: tokenName = "IF"; break;
        case ELSE: tokenName = "ELSE"; break;
        case GE: tokenName = ">="; break;
        case LE: tokenName = "<="; break;
        case EQ: tokenName = "=="; break;
        case NE: tokenName = "!="; break;
        case UMINUS: tokenName = "-"; break;
        case '+': tokenName = "+"; break;
        case '-': tokenName = "-"; break;
        case '*': tokenName = "*"; break;
        case '/': tokenName = "/"; break;
        case '>': tokenName = ">"; break;
        case '<': tokenName = "<"; break;
        case '=': tokenName = "="; break;
        }   
        errToken = -1;
        printf("syntax error, unexpected %s near line %d\n", tokenName.c_str(), lineno);
        fflush(stdout);     
    }
    else{
        printf("%s near line %d\n", s.c_str(),lineno);
        fflush(stdout); 
    }
    errorFound = true;    
}

int main(void) {
    pushSymbolTable();
    lineno = 1;
    progBlkLvl = getCurrentLevel();
    errorFound = false;
    declaringNewVar = false;
    myPStack.begin_prog();
    yyparse();
    myPStack.end_prog(getCurrentSymbolTableSize());
    if(!errorFound)
        myPStack.write("calc_out.apm", 1);
    return 0;
}

int ex(nodeType *p) {
    if (!p) return 0;
    switch(p->type) {
    case typeInt:       {
                            myPStack.add(I_CONSTANT);
                            myPStack.add(p->con.value);
                            
                            return typeInt;
                        }
    case typeFloat:     {
                            myPStack.add(R_CONSTANT);
                            myPStack.add(p->fl.value);
                            
                            return typeFloat;
                        }
    case typeId:        {
                        if(p->id.i->relentry != NULL){
                            symbol_entry *entry = p->id.i->relentry;
                            if(entry->type == TYPE_INT){
                                myPStack.add(I_VARIABLE);
                                myPStack.add(0);
                                myPStack.add(p->id.i->offset);
                                
                                myPStack.add(I_VARIABLE);
                                myPStack.add(p->id.i->blk_level-entry->blk_level);
                                myPStack.add(entry->offset);                   

                                myPStack.add(I_VALUE);
                                myPStack.add(I_ASSIGN);
                                myPStack.add(1);
                            }
                            else if(entry->type == TYPE_FLOAT){
                                myPStack.add(R_VARIABLE);
                                myPStack.add(0);
                                myPStack.add(p->id.i->offset);
                                
                                myPStack.add(R_VARIABLE);
                                myPStack.add(p->id.i->blk_level-entry->blk_level);
                                myPStack.add(entry->offset);                   

                                myPStack.add(R_VALUE);
                                myPStack.add(R_ASSIGN);
                                myPStack.add(1);
                            }
                            p->id.i->relentry = NULL;
                        }

                        symbol_entry *curVar;
                        if(p->id.i->refetch) 
                            curVar = getSymbolEntry(p->id.i->name);
                        else
                            curVar = p->id.i;

                        int varType;
                        if(curVar->prev_type != -1)
                            varType = curVar->prev_type;
                        else
                            varType = curVar->type;
                        
                        if (varType == TYPE_INT){

                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(curVar->offset);
			                myPStack.add(I_VALUE);
                 
                            return TYPE_INT;
                        }
                        else if (varType == TYPE_FLOAT){
                                                      
                            myPStack.add(R_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(curVar->offset);
                            myPStack.add(R_VALUE);

                            return TYPE_FLOAT;
                        }
                        }
    case typeOpr:
        switch(p->opr.oper) {
        case FOR:       {   symbol_entry *entry = p->opr.op[0]->id.i;
                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(entry->offset);
                            
                            ex(p->opr.op[1]);
                            
                            myPStack.add(I_ASSIGN);
                            myPStack.add(1);

                            int initpos = myPStack.pos();

                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(entry->offset);
                            myPStack.add(I_VALUE);

                            ex(p->opr.op[3]);

                            myPStack.add(I_EQUAL);
                            myPStack.add(I_JMP_IF_TRUE);
                            int comparepos = myPStack.pos();
                            myPStack.add(0);

                            ex(p->opr.op[4]);

                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(entry->offset);

                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(entry->offset);
                            myPStack.add(I_VALUE);

                            ex(p->opr.op[2]);

                            myPStack.add(I_ADD);
                            myPStack.add(I_ASSIGN);
                            myPStack.add(1);

                            myPStack.add(I_JMP);
                            myPStack.add(initpos);

                            myPStack.at(comparepos) = myPStack.pos();  

                            ex(p->opr.op[4]);

                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(entry->offset);

                            myPStack.add(I_VARIABLE);
                            myPStack.add(0);
                            myPStack.add(entry->offset);
                            myPStack.add(I_VALUE);

                            ex(p->opr.op[2]);

                            myPStack.add(I_ADD);
                            myPStack.add(I_ASSIGN);
                            myPStack.add(1);

                            return 0;                          

                        }
        case PROCEDURE: {
                        symbol_entry* entry = p->opr.op[0]->id.i;
                        myPStack.add(I_JMP);
                        myPStack.add(0);
                        int jmpPos = myPStack.pos()-1;

                        entry->pAddr = myPStack.pos();

                        myPStack.begin_proc();
                        int patch_loc = myPStack.pos()-2;
                        ex(p->opr.op[1]);
                        myPStack.end_proc(patch_loc, getCurrentSymbolTableSize());

                        myPStack.at(jmpPos) = myPStack.pos();

                        popSymbolTable();

                        return 0;
                        }
        case CALL:      {
                        symbol_entry* entry = p->opr.op[0]->id.i;
                        myPStack.add(I_CALL);
                        myPStack.add(entry->blk_level);
                        myPStack.add(entry->pAddr);

                        return 0;
                        }
        case BG:        {
                        myPStack.add(I_JMP);
                        myPStack.add(0);
                        int jmpPos =  myPStack.pos()-1;
                        myPStack.begin_proc();
                        int patch_loc = myPStack.pos()-2;
                        ex(p->opr.op[0]);
                        myPStack.end_proc(patch_loc, getCurrentSymbolTableSize());
                        
                        myPStack.at(jmpPos) = myPStack.pos();
                        myPStack.add(I_CALL);
                        myPStack.add(0);
                        myPStack.add(patch_loc-1);
                        popSymbolTable();

                        return 0;
                        }
        case WHILE:     {
                        ex(p->opr.op[0]);   
                        int jmpAddr = myPStack.pos();                     
                        myPStack.add(I_JMP_IF_FALSE);
                        myPStack.add(0);
                        int initAddr = myPStack.pos();
                        ex(p->opr.op[1]);
                        ex(p->opr.op[0]);
                        myPStack.add(I_JMP);
                        myPStack.add(jmpAddr);
                        myPStack.at(initAddr-1) = myPStack.pos();

                        return 0;
                        }
        case DO:        {
                        int jmpAddr = myPStack.pos();
                        ex(p->opr.op[0]);                        
                        ex(p->opr.op[1]);
                        myPStack.add(I_JMP_IF_TRUE);
                        myPStack.add(jmpAddr);
                        }

                        return 0;
        case REPEAT:    {
                        int jmpAddr = myPStack.pos();
                        ex(p->opr.op[0]);                        
                        ex(p->opr.op[1]);
                        myPStack.add(I_JMP_IF_FALSE);
                        myPStack.add(jmpAddr);
                        
                        return 0;
                        }
        case INT:       declaringNewVar = true;
                        if(p->opr.op[0]->type == typeOpr)   
                            ex(p->opr.op[0]);
                        else
                            p->opr.op[0]->id.i->prev_type = -1;
                        declaringNewVar = false;  
                        return 0;
        case DOUB:      declaringNewVar = true;
                        if(p->opr.op[0]->type == typeOpr)
                            ex(p->opr.op[0]); 
                        else
                            p->opr.op[0]->id.i->prev_type = -1;
                        declaringNewVar = false;  
                        return 0;
        case ',':       if(p->opr.op[0]->type == typeOpr)
                            ex(p->opr.op[0]);
                        else
                            p->opr.op[0]->id.i->prev_type = -1;
                        if(p->opr.op[1]->type == typeOpr)
                            ex(p->opr.op[1]);
                        else
                            p->opr.op[1]->id.i->prev_type = -1;
                        return 0;
        case '=':       {
                         if(declaringNewVar)
                            p->opr.op[0]->id.i->prev_type = -1;
                         
                         int valType;
                         if(p->opr.op[0]->id.i->prev_type != -1)
                            valType = p->opr.op[0]->id.i->prev_type;
                         else
                           valType =  p->opr.op[0]->id.i->type;

			             if (valType == TYPE_INT){
                            myPStack.add(I_VARIABLE);
			                myPStack.add(0);
			                myPStack.add(p->opr.op[0]->id.i->offset);
			             
			                int valueType = ex(p->opr.op[1]);
                            if(valueType != TYPE_INT && valueType != typeInt)
                                yyerror("Assigning float into an int variable");
                                                      
                            myPStack.add(I_ASSIGN);
                            myPStack.add(1);
                           
                            p->opr.op[0]->id.i->relentry = NULL;
                            return TYPE_INT;
                        }
                        else if (valType == TYPE_FLOAT){
                            myPStack.add(R_VARIABLE);
			                myPStack.add(0);
			                myPStack.add(p->opr.op[0]->id.i->offset);
                            
                            int valueType = ex(p->opr.op[1]);
                            if(valueType != TYPE_FLOAT && valueType != typeFloat)
                                yyerror("Assigning int into an float variable");                           
                            
                            myPStack.add(R_ASSIGN);
                            myPStack.add(1);
                            
                            p->opr.op[0]->id.i->relentry = NULL;
                            return TYPE_FLOAT;
                        }
                        }
        case IF:        {                        
                        ex(p->opr.op[0]);
                        
                        myPStack.add(I_JMP_IF_FALSE);
                        myPStack.add(0);
                        
                        int initAddr = myPStack.pos();
                        //fprintf(stderr, "%d", initAddr);
                        ex(p->opr.op[1]);                        
                        
                        myPStack.add(I_JMP);
                        myPStack.add(0);
                        int nextAddr = myPStack.pos();

                        myPStack.at(initAddr-1) = myPStack.pos();
                        if (p->opr.nops > 2)
                            ex(p->opr.op[2]);

                        myPStack.at(nextAddr-1) = myPStack.pos();

                        return 0;
                        }
        case PRINT:     { 
                        int valueType = ex(p->opr.op[0]); 
                                             
                        if(valueType == TYPE_INT || valueType == typeInt)
                            myPStack.add(I_WRITE);
                        else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                            myPStack.add(R_WRITE);                                                 
                        
                        myPStack.add(1);
                        
                        return 0;
                        }
        case ';':       ex(p->opr.op[0]); return ex(p->opr.op[1]);
        case UMINUS:    {
                        int valueType = ex(p->opr.op[0]); 
                     
                        if(valueType == TYPE_INT || valueType == typeInt)
                            myPStack.add(I_MINUS);
                        else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                            myPStack.add(R_MINUS);                                                 
                        return valueType;
                        }
        case '+':       {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]);  

                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);                     
                                                               
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_ADD);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_ADD);  

                         return valueType;
                        }
        case '-':       {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                            
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_SUBTRACT);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_SUBTRACT);  

                         return valueType;
                         }
        case '*':       {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                            
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_MULTIPLY);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_MULTIPLY);  

                         return valueType;
                         }
        case '/':       {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                            
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_DIVIDE);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_DIVIDE);  

                         return valueType;
                         }
        case '<':       {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                            
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_LESS);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_GREATER);  

                         return 0;
                         }
        case '>':       {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                           
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_GREATER);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_GREATER);  

                         return 0;
                         }
        case GE:        {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                            
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_EQUAL);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_EQUAL);  

                         ex(p->opr.op[0]);
                         ex(p->opr.op[1]);

                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_GREATER);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_GREATER);
                         
                         myPStack.add(I_OR);
                         return 0;
                        }
        case LE:        {    
                        int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);                    
                         
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_EQUAL);
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_EQUAL);  

                         ex(p->opr.op[0]);
                         ex(p->opr.op[1]);

                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_LESS);

                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_LESS);

                         myPStack.add(I_OR);
                         return 0;
                        }
        case NE:        {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                     
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_EQUAL);                         
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_EQUAL);  

                         myPStack.add(I_NOT);
                         return 0;                         
                        }
        case EQ:        {
                         int valueType = ex(p->opr.op[0]);
                         int valueType2 = ex(p->opr.op[1]); 
         
                         if(checkOpError(valueType, valueType2))
                            yyerror(typeOpError);
                            
                         if(valueType == TYPE_INT || valueType == typeInt)
                             myPStack.add(I_EQUAL);                         
                         else if (valueType == TYPE_FLOAT || valueType == typeFloat)
                             myPStack.add(R_EQUAL);  
                         return 0;
                         }		
        }
    }
    return 0;
}
