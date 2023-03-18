/* A Bison parser, made from laygram.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

# define	OC	257
# define	CC	258
# define	OA	259
# define	CA	260
# define	OP	261
# define	CP	262
# define	NAME	263
# define	NUMBER	264
# define	INFINITY	265
# define	VERTICAL	266
# define	HORIZONTAL	267
# define	EQUAL	268
# define	DOLLAR	269
# define	PLUS	270
# define	MINUS	271
# define	TIMES	272
# define	DIVIDE	273
# define	PERCENTOF	274
# define	PERCENT	275
# define	WIDTH	276
# define	HEIGHT	277
# define	UMINUS	278
# define	UPLUS	279

#line 1 "laygram.y"

#include    <X11/Xlib.h>
#include    <X11/Xresource.h>
#include    <stdio.h>
#include    <X11/IntrinsicP.h>
#include    <X11/cursorfont.h>
#include    <X11/StringDefs.h>

#include    <X11/Xmu/Misc.h>
#include    <X11/Xmu/Converters.h>
#include    "LayoutP.h"

static LayoutPtr    *dest;


#line 17 "laygram.y"
#ifndef YYSTYPE
typedef union {
    int		    ival;
    XrmQuark	    qval;
    BoxPtr	    bval;
    BoxParamsPtr    pval;
    GlueRec	    gval;
    LayoutDirection lval;
    ExprPtr	    eval;
    Operator	    oval;
} LayYYstype;
# define YYSTYPE LayYYstype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		68
#define	YYFLAG		-32768
#define	YYNTBASE	26

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 279 ? LayYYtranslate[x] : 39)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const char LayYYtranslate[] =
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25
};

#if YYDEBUG
static const short LayYYprhs[] =
{
       0,     0,     2,     5,     8,    12,    14,    19,    22,    24,
      32,    33,    38,    39,    42,    43,    46,    47,    50,    52,
      54,    57,    60,    62,    65,    68,    72,    75,    77,    80,
      84,    88,    92,    96,   100,   103,   106,   108,   110
};
static const short LayYYrhs[] =
{
      28,     0,     9,    30,     0,    35,    31,     0,     9,    14,
      35,     0,    28,     0,    38,     3,    29,     4,     0,    27,
      29,     0,    27,     0,     5,    32,    33,    18,    32,    33,
       6,     0,     0,     5,    32,    33,     6,     0,     0,    16,
      34,     0,     0,    17,    34,     0,     0,    36,    11,     0,
      36,     0,    11,     0,    17,    36,     0,    16,    36,     0,
      36,     0,    22,     9,     0,    23,     9,     0,     7,    37,
       8,     0,    36,    21,     0,    10,     0,    15,     9,     0,
      37,    16,    37,     0,    37,    17,    37,     0,    37,    18,
      37,     0,    37,    19,    37,     0,    37,    20,    37,     0,
      17,    37,     0,    16,    37,     0,    36,     0,    12,     0,
      13,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short LayYYrline[] =
{
       0,    49,    52,    62,    72,    81,    86,   112,   117,   120,
     130,   141,   151,   162,   164,   167,   169,   172,   174,   176,
     179,   186,   188,   190,   195,   200,   202,   209,   214,   220,
     228,   230,   232,   234,   236,   243,   245,   247,   249
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const LayYYtname[] =
{
  "$", "error", "$undefined.", "OC", "CC", "OA", "CA", "OP", "CP", "NAME", 
  "NUMBER", "INFINITY", "VERTICAL", "HORIZONTAL", "EQUAL", "DOLLAR", 
  "PLUS", "MINUS", "TIMES", "DIVIDE", "PERCENTOF", "PERCENT", "WIDTH", 
  "HEIGHT", "UMINUS", "UPLUS", "layout", "box", "compositebox", "boxes", 
  "bothparams", "oneparams", "opStretch", "opShrink", "glue", 
  "signedExpr", "simpleExpr", "expr", "orientation", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short LayYYr1[] =
{
       0,    26,    27,    27,    27,    27,    28,    29,    29,    30,
      30,    31,    31,    32,    32,    33,    33,    34,    34,    34,
      35,    35,    35,    36,    36,    36,    36,    36,    36,    37,
      37,    37,    37,    37,    37,    37,    37,    38,    38
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short LayYYr2[] =
{
       0,     1,     2,     2,     3,     1,     4,     2,     1,     7,
       0,     4,     0,     2,     0,     2,     0,     2,     1,     1,
       2,     2,     1,     2,     2,     3,     2,     1,     2,     3,
       3,     3,     3,     3,     2,     2,     1,     1,     1
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short LayYYdefact[] =
{
       0,    37,    38,     1,     0,     0,     0,    10,    27,     0,
       0,     0,     0,     0,     8,     5,     0,    12,    22,     0,
       0,    36,     0,    14,     0,     2,    28,    21,    20,    23,
      24,     7,     6,    14,     3,    26,    35,    34,    25,     0,
       0,     0,     0,     0,     0,    16,     4,    16,    29,    30,
      31,    32,    33,    19,    13,    18,     0,     0,     0,    17,
      15,    14,    11,    16,     0,     9,     0,     0,     0
};

static const short LayYYdefgoto[] =
{
      66,    14,    15,    16,    25,    34,    45,    57,    54,    17,
      21,    22,     4
};

static const short LayYYpact[] =
{
      -2,-32768,-32768,-32768,     5,    21,    25,     7,-32768,     4,
      43,    43,     6,     9,    21,-32768,    10,    11,    24,    25,
      25,    24,    61,    36,    39,-32768,-32768,    24,    24,-32768,
  -32768,-32768,-32768,    36,-32768,-32768,-32768,-32768,-32768,    25,
      25,    25,    25,    25,    53,    40,-32768,    40,    52,    52,
  -32768,-32768,-32768,-32768,-32768,    -4,    53,    41,    67,-32768,
  -32768,    36,-32768,    40,    68,-32768,    27,    82,-32768
};

static const short LayYYpgoto[] =
{
  -32768,-32768,    83,    70,-32768,-32768,   -32,   -43,    29,    62,
      -5,   -17,-32768
};


#define	YYLAST		86


static const short LayYYtable[] =
{
      18,    47,    36,    37,    58,    27,    28,    59,     5,    18,
       1,     2,    23,    26,    32,    29,    33,    35,    30,    18,
      64,    24,    48,    49,    50,    51,    52,    67,     6,    63,
       7,     8,     6,     1,     2,     8,     9,    10,    11,    55,
       9,    19,    20,    12,    13,    35,     6,    12,    13,     8,
       6,    55,    44,     8,     9,    10,    11,    56,     9,    61,
       6,    12,    13,     8,    53,    12,    13,     0,     9,    38,
      41,    42,    43,    62,    65,    12,    13,    39,    40,    41,
      42,    43,    68,     3,    31,    60,    46
};

static const short LayYYcheck[] =
{
       5,    33,    19,    20,    47,    10,    11,    11,     3,    14,
      12,    13,     5,     9,     4,     9,     5,    21,     9,    24,
      63,    14,    39,    40,    41,    42,    43,     0,     7,    61,
       9,    10,     7,    12,    13,    10,    15,    16,    17,    44,
      15,    16,    17,    22,    23,    21,     7,    22,    23,    10,
       7,    56,    16,    10,    15,    16,    17,    17,    15,    18,
       7,    22,    23,    10,    11,    22,    23,    -1,    15,     8,
      18,    19,    20,     6,     6,    22,    23,    16,    17,    18,
      19,    20,     0,     0,    14,    56,    24
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with LayYY or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (LayYYoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (LayYYoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (LayYYoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union LayYYalloc
{
  short LayYYss;
  YYSTYPE LayYYvs;
# if YYLSP_NEEDED
  YYLTYPE LayYYls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union LayYYalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T LayYYi;		\
	  for (LayYYi = 0; LayYYi < (Count); LayYYi++)	\
	    (To)[LayYYi] = (From)[LayYYi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T LayYYnewbytes;						\
	YYCOPY (&LayYYptr->Stack, Stack, LayYYsize);				\
	Stack = &LayYYptr->Stack;						\
	LayYYnewbytes = LayYYstacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	LayYYptr += LayYYnewbytes / sizeof (*LayYYptr);				\
      }									\
    while (0)

#endif


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define LayYYerrok		(LayYYerrstatus = 0)
#define LayYYclearin	(LayYYchar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto LayYYacceptlab
#define YYABORT 	goto LayYYabortlab
#define YYERROR		goto LayYYerrlab1
/* Like YYERROR except do call LayYYerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto LayYYerrlab
#define YYRECOVERING()  (!!LayYYerrstatus)
#define YYBACKUP(Token, Value)					\
do								\
  if (LayYYchar == YYEMPTY && LayYYlen == 1)				\
    {								\
      LayYYchar = (Token);						\
      LayYYlval = (Value);						\
      LayYYchar1 = YYTRANSLATE (LayYYchar);				\
      YYPOPSTACK;						\
      goto LayYYbackup;						\
    }								\
  else								\
    { 								\
      LayYYerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `LayYYlex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		LayYYlex (&LayYYlval, &LayYYlloc, YYLEX_PARAM)
#  else
#   define YYLEX		LayYYlex (&LayYYlval, &LayYYlloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		LayYYlex (&LayYYlval, YYLEX_PARAM)
#  else
#   define YYLEX		LayYYlex (&LayYYlval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			LayYYlex ()
#endif /* !YYPURE */


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (LayYYdebug)					\
    YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int LayYYdebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

# ifndef LayYYstrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define LayYYstrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
LayYYstrlen (const char *LayYYstr)
#   else
LayYYstrlen (LayYYstr)
     const char *LayYYstr;
#   endif
{
  register const char *LayYYs = LayYYstr;

  while (*LayYYs++ != '\0')
    continue;

  return LayYYs - LayYYstr - 1;
}
#  endif
# endif

# ifndef LayYYstpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define LayYYstpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
LayYYstpcpy (char *LayYYdest, const char *LayYYsrc)
#   else
LayYYstpcpy (LayYYdest, LayYYsrc)
     char *LayYYdest;
     const char *LayYYsrc;
#   endif
{
  register char *LayYYd = LayYYdest;
  register const char *LayYYs = LayYYsrc;

  while ((*LayYYd++ = *LayYYs++) != '\0')
    continue;

  return LayYYd - 1;
}
#  endif
# endif
#endif

#line 315 "/usr/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into LayYYparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int LayYYparse (void *);
# else
int LayYYparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int LayYYchar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE LayYYlval;						\
							\
/* Number of parse errors so far.  */			\
int LayYYnerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE LayYYlloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
LayYYparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int LayYYstate;
  register int LayYYn;
  int LayYYresult;
  /* Number of tokens to shift before error messages enabled.  */
  int LayYYerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int LayYYchar1 = 0;

  /* Three stacks and their tools:
     `LayYYss': related to states,
     `LayYYvs': related to semantic values,
     `LayYYls': related to locations.

     Refer to the stacks thru separate pointers, to allow LayYYoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	LayYYssa[YYINITDEPTH];
  short *LayYYss = LayYYssa;
  register short *LayYYssp;

  /* The semantic value stack.  */
  YYSTYPE LayYYvsa[YYINITDEPTH];
  YYSTYPE *LayYYvs = LayYYvsa;
  register YYSTYPE *LayYYvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE LayYYlsa[YYINITDEPTH];
  YYLTYPE *LayYYls = LayYYlsa;
  YYLTYPE *LayYYlsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (LayYYvsp--, LayYYssp--, LayYYlsp--)
#else
# define YYPOPSTACK   (LayYYvsp--, LayYYssp--)
#endif

  YYSIZE_T LayYYstacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE LayYYval;
#if YYLSP_NEEDED
  YYLTYPE LayYYloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int LayYYlen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  LayYYstate = 0;
  LayYYerrstatus = 0;
  LayYYnerrs = 0;
  LayYYchar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  LayYYssp = LayYYss;
  LayYYvsp = LayYYvs;
#if YYLSP_NEEDED
  LayYYlsp = LayYYls;
#endif
  goto LayYYsetstate;

/*------------------------------------------------------------.
| LayYYnewstate -- Push a new state, which is found in LayYYstate.  |
`------------------------------------------------------------*/
 LayYYnewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  LayYYssp++;

 LayYYsetstate:
  *LayYYssp = LayYYstate;

  if (LayYYssp >= LayYYss + LayYYstacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T LayYYsize = LayYYssp - LayYYss + 1;

#ifdef LayYYoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *LayYYvs1 = LayYYvs;
	short *LayYYss1 = LayYYss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *LayYYls1 = LayYYls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if LayYYoverflow is a macro.  */
	LayYYoverflow ("parser stack overflow",
		    &LayYYss1, LayYYsize * sizeof (*LayYYssp),
		    &LayYYvs1, LayYYsize * sizeof (*LayYYvsp),
		    &LayYYls1, LayYYsize * sizeof (*LayYYlsp),
		    &LayYYstacksize);
	LayYYls = LayYYls1;
# else
	LayYYoverflow ("parser stack overflow",
		    &LayYYss1, LayYYsize * sizeof (*LayYYssp),
		    &LayYYvs1, LayYYsize * sizeof (*LayYYvsp),
		    &LayYYstacksize);
# endif
	LayYYss = LayYYss1;
	LayYYvs = LayYYvs1;
      }
#else /* no LayYYoverflow */
# ifndef YYSTACK_RELOCATE
      goto LayYYoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (LayYYstacksize >= YYMAXDEPTH)
	goto LayYYoverflowlab;
      LayYYstacksize *= 2;
      if (LayYYstacksize > YYMAXDEPTH)
	LayYYstacksize = YYMAXDEPTH;

      {
	short *LayYYss1 = LayYYss;
	union LayYYalloc *LayYYptr =
	  (union LayYYalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (LayYYstacksize));
	if (! LayYYptr)
	  goto LayYYoverflowlab;
	YYSTACK_RELOCATE (LayYYss);
	YYSTACK_RELOCATE (LayYYvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (LayYYls);
# endif
# undef YYSTACK_RELOCATE
	if (LayYYss1 != LayYYssa)
	  YYSTACK_FREE (LayYYss1);
      }
# endif
#endif /* no LayYYoverflow */

      LayYYssp = LayYYss + LayYYsize - 1;
      LayYYvsp = LayYYvs + LayYYsize - 1;
#if YYLSP_NEEDED
      LayYYlsp = LayYYls + LayYYsize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) LayYYstacksize));

      if (LayYYssp >= LayYYss + LayYYstacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", LayYYstate));

  goto LayYYbackup;


/*-----------.
| LayYYbackup.  |
`-----------*/
LayYYbackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* LayYYresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  LayYYn = LayYYpact[LayYYstate];
  if (LayYYn == YYFLAG)
    goto LayYYdefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* LayYYchar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (LayYYchar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      LayYYchar = YYLEX;
    }

  /* Convert token to internal form (in LayYYchar1) for indexing tables with */

  if (LayYYchar <= 0)		/* This means end of input. */
    {
      LayYYchar1 = 0;
      LayYYchar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      LayYYchar1 = YYTRANSLATE (LayYYchar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (LayYYdebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     LayYYchar, LayYYtname[LayYYchar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, LayYYchar, LayYYlval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  LayYYn += LayYYchar1;
  if (LayYYn < 0 || LayYYn > YYLAST || LayYYcheck[LayYYn] != LayYYchar1)
    goto LayYYdefault;

  LayYYn = LayYYtable[LayYYn];

  /* LayYYn is what to do for this token type in this state.
     Negative => reduce, -LayYYn is rule number.
     Positive => shift, LayYYn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (LayYYn < 0)
    {
      if (LayYYn == YYFLAG)
	goto LayYYerrlab;
      LayYYn = -LayYYn;
      goto LayYYreduce;
    }
  else if (LayYYn == 0)
    goto LayYYerrlab;

  if (LayYYn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      LayYYchar, LayYYtname[LayYYchar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (LayYYchar != YYEOF)
    LayYYchar = YYEMPTY;

  *++LayYYvsp = LayYYlval;
#if YYLSP_NEEDED
  *++LayYYlsp = LayYYlloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (LayYYerrstatus)
    LayYYerrstatus--;

  LayYYstate = LayYYn;
  goto LayYYnewstate;


/*-----------------------------------------------------------.
| LayYYdefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
LayYYdefault:
  LayYYn = LayYYdefact[LayYYstate];
  if (LayYYn == 0)
    goto LayYYerrlab;
  goto LayYYreduce;


/*-----------------------------.
| LayYYreduce -- Do a reduction.  |
`-----------------------------*/
LayYYreduce:
  /* LayYYn is the number of a rule to reduce with.  */
  LayYYlen = LayYYr2[LayYYn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  LayYYval = LayYYvsp[1-LayYYlen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  LayYYloc = LayYYlsp[1-LayYYlen];
  YYLLOC_DEFAULT (LayYYloc, (LayYYlsp - LayYYlen), LayYYlen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (LayYYdebug)
    {
      int LayYYi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 LayYYn, LayYYrline[LayYYn]);

      /* Print the symbols being reduced, and their result.  */
      for (LayYYi = LayYYprhs[LayYYn]; LayYYrhs[LayYYi] > 0; LayYYi++)
	YYFPRINTF (stderr, "%s ", LayYYtname[LayYYrhs[LayYYi]]);
      YYFPRINTF (stderr, " -> %s\n", LayYYtname[LayYYr1[LayYYn]]);
    }
#endif

  switch (LayYYn) {

case 1:
#line 50 "laygram.y"
{ *dest = LayYYvsp[0].bval; }
    break;
case 2:
#line 53 "laygram.y"
{
			BoxPtr	box = New(LBoxRec);
			box->nextSibling = 0;
			box->type = WidgetBox;
			box->params = *LayYYvsp[0].pval;
			Dispose (LayYYvsp[0].pval);
			box->u.widget.quark = LayYYvsp[-1].qval;
			LayYYval.bval = box;
		    }
    break;
case 3:
#line 63 "laygram.y"
{
			BoxPtr	box = New(LBoxRec);
			box->nextSibling = 0;
			box->type = GlueBox;
			box->params = *LayYYvsp[0].pval;
			Dispose (LayYYvsp[0].pval);
			box->u.glue.expr = LayYYvsp[-1].eval;
			LayYYval.bval = box;
		    }
    break;
case 4:
#line 73 "laygram.y"
{
			BoxPtr	box = New(LBoxRec);
			box->nextSibling = 0;
			box->type = VariableBox;
			box->u.variable.quark = LayYYvsp[-2].qval;
			box->u.variable.expr = LayYYvsp[0].eval;
			LayYYval.bval = box;
		    }
    break;
case 5:
#line 82 "laygram.y"
{
			LayYYval.bval = LayYYvsp[0].bval;
		    }
    break;
case 6:
#line 87 "laygram.y"
{
			BoxPtr	box = New(LBoxRec);
			BoxPtr	child;

			box->nextSibling = 0;
			box->parent = 0;
			box->type = BoxBox;
			box->u.box.dir = LayYYvsp[-3].lval;
			box->u.box.firstChild = LayYYvsp[-1].bval;
			for (child = LayYYvsp[-1].bval; child; child = child->nextSibling) 
			{
			    if (child->type == GlueBox) 
			    {
				child->params.stretch[!LayYYvsp[-3].lval].expr = 0;
				child->params.shrink[!LayYYvsp[-3].lval].expr = 0;
				child->params.stretch[!LayYYvsp[-3].lval].order = 100000;
				child->params.shrink[!LayYYvsp[-3].lval].order = 100000;
				child->params.stretch[!LayYYvsp[-3].lval].value = 1;
				child->params.shrink[!LayYYvsp[-3].lval].value = 1;
			    }
			    child->parent = box;
			}
			LayYYval.bval = box;
		    }
    break;
case 7:
#line 113 "laygram.y"
{ 
			LayYYvsp[-1].bval->nextSibling = LayYYvsp[0].bval;
			LayYYval.bval = LayYYvsp[-1].bval;
		    }
    break;
case 8:
#line 118 "laygram.y"
{	LayYYval.bval = LayYYvsp[0].bval; }
    break;
case 9:
#line 121 "laygram.y"
{	
			BoxParamsPtr	p = New(BoxParamsRec);
			
			p->stretch[LayoutHorizontal] = LayYYvsp[-5].gval;
			p->shrink[LayoutHorizontal] = LayYYvsp[-4].gval;
			p->stretch[LayoutVertical] = LayYYvsp[-2].gval;
			p->shrink[LayoutVertical] = LayYYvsp[-1].gval;
			LayYYval.pval = p;
		    }
    break;
case 10:
#line 131 "laygram.y"
{	
			BoxParamsPtr	p = New(BoxParamsRec);
			
			ZeroGlue (p->stretch[LayoutHorizontal]);
			ZeroGlue (p->shrink[LayoutHorizontal]);
			ZeroGlue (p->stretch[LayoutVertical]);
			ZeroGlue (p->shrink[LayoutVertical]);
			LayYYval.pval = p;
		    }
    break;
case 11:
#line 142 "laygram.y"
{	
			BoxParamsPtr	p = New(BoxParamsRec);
			
			p->stretch[LayoutHorizontal] = LayYYvsp[-2].gval;
			p->shrink[LayoutHorizontal] = LayYYvsp[-1].gval;
			p->stretch[LayoutVertical] = LayYYvsp[-2].gval;
			p->shrink[LayoutVertical] = LayYYvsp[-1].gval;
			LayYYval.pval = p;
		    }
    break;
case 12:
#line 152 "laygram.y"
{	
			BoxParamsPtr	p = New(BoxParamsRec);
			
			ZeroGlue (p->stretch[LayoutHorizontal]);
			ZeroGlue (p->shrink[LayoutHorizontal]);
			ZeroGlue (p->stretch[LayoutVertical]);
			ZeroGlue (p->shrink[LayoutVertical]);
			LayYYval.pval = p;
		    }
    break;
case 13:
#line 163 "laygram.y"
{ LayYYval.gval = LayYYvsp[0].gval; }
    break;
case 14:
#line 165 "laygram.y"
{ ZeroGlue (LayYYval.gval); }
    break;
case 15:
#line 168 "laygram.y"
{ LayYYval.gval = LayYYvsp[0].gval; }
    break;
case 16:
#line 170 "laygram.y"
{ ZeroGlue (LayYYval.gval); }
    break;
case 17:
#line 173 "laygram.y"
{ LayYYval.gval.order = LayYYvsp[0].ival; LayYYval.gval.expr = LayYYvsp[-1].eval; }
    break;
case 18:
#line 175 "laygram.y"
{ LayYYval.gval.order = 0; LayYYval.gval.expr = LayYYvsp[0].eval; }
    break;
case 19:
#line 177 "laygram.y"
{ LayYYval.gval.order = LayYYvsp[0].ival; LayYYval.gval.expr = 0; LayYYval.gval.value = 1; }
    break;
case 20:
#line 180 "laygram.y"
{
			LayYYval.eval = New(ExprRec);
			LayYYval.eval->type = Unary;
			LayYYval.eval->u.unary.op = LayYYvsp[-1].oval;
			LayYYval.eval->u.unary.down = LayYYvsp[0].eval;
		    }
    break;
case 21:
#line 187 "laygram.y"
{ LayYYval.eval = LayYYvsp[0].eval; }
    break;
case 23:
#line 191 "laygram.y"
{	LayYYval.eval = New(ExprRec);
			LayYYval.eval->type = Width;
			LayYYval.eval->u.width = LayYYvsp[0].qval;
		    }
    break;
case 24:
#line 196 "laygram.y"
{	LayYYval.eval = New(ExprRec);
			LayYYval.eval->type = Height;
			LayYYval.eval->u.height = LayYYvsp[0].qval;
		    }
    break;
case 25:
#line 201 "laygram.y"
{ LayYYval.eval = LayYYvsp[-1].eval; }
    break;
case 26:
#line 203 "laygram.y"
{
			LayYYval.eval = New(ExprRec);
			LayYYval.eval->type = Unary;
			LayYYval.eval->u.unary.op = LayYYvsp[0].oval;
			LayYYval.eval->u.unary.down = LayYYvsp[-1].eval;
		    }
    break;
case 27:
#line 210 "laygram.y"
{	LayYYval.eval = New(ExprRec);
			LayYYval.eval->type = Constant;
			LayYYval.eval->u.constant = LayYYvsp[0].ival;
		    }
    break;
case 28:
#line 215 "laygram.y"
{	LayYYval.eval = New(ExprRec);
			LayYYval.eval->type = Variable;
			LayYYval.eval->u.variable = LayYYvsp[0].qval;
		    }
    break;
case 29:
#line 221 "laygram.y"
{ binary: ;
			LayYYval.eval = New(ExprRec);
			LayYYval.eval->type = Binary;
			LayYYval.eval->u.binary.op = LayYYvsp[-1].oval;
			LayYYval.eval->u.binary.left = LayYYvsp[-2].eval;
			LayYYval.eval->u.binary.right = LayYYvsp[0].eval;
		    }
    break;
case 30:
#line 229 "laygram.y"
{ goto binary; }
    break;
case 31:
#line 231 "laygram.y"
{ goto binary; }
    break;
case 32:
#line 233 "laygram.y"
{ goto binary; }
    break;
case 33:
#line 235 "laygram.y"
{ goto binary; }
    break;
case 34:
#line 237 "laygram.y"
{ unary: ;
			LayYYval.eval = New(ExprRec);
			LayYYval.eval->type = Unary;
			LayYYval.eval->u.unary.op = LayYYvsp[-1].oval;
			LayYYval.eval->u.unary.down = LayYYvsp[0].eval;
		    }
    break;
case 35:
#line 244 "laygram.y"
{ LayYYval.eval = LayYYvsp[0].eval; }
    break;
case 37:
#line 248 "laygram.y"
{   LayYYval.lval = LayoutVertical; }
    break;
case 38:
#line 250 "laygram.y"
{   LayYYval.lval = LayoutHorizontal; }
    break;
}

#line 705 "/usr/share/bison/bison.simple"


  LayYYvsp -= LayYYlen;
  LayYYssp -= LayYYlen;
#if YYLSP_NEEDED
  LayYYlsp -= LayYYlen;
#endif

#if YYDEBUG
  if (LayYYdebug)
    {
      short *LayYYssp1 = LayYYss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (LayYYssp1 != LayYYssp)
	YYFPRINTF (stderr, " %d", *++LayYYssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++LayYYvsp = LayYYval;
#if YYLSP_NEEDED
  *++LayYYlsp = LayYYloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  LayYYn = LayYYr1[LayYYn];

  LayYYstate = LayYYpgoto[LayYYn - YYNTBASE] + *LayYYssp;
  if (LayYYstate >= 0 && LayYYstate <= YYLAST && LayYYcheck[LayYYstate] == *LayYYssp)
    LayYYstate = LayYYtable[LayYYstate];
  else
    LayYYstate = LayYYdefgoto[LayYYn - YYNTBASE];

  goto LayYYnewstate;


/*------------------------------------.
| LayYYerrlab -- here on detecting error |
`------------------------------------*/
LayYYerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!LayYYerrstatus)
    {
      ++LayYYnerrs;

#ifdef YYERROR_VERBOSE
      LayYYn = LayYYpact[LayYYstate];

      if (LayYYn > YYFLAG && LayYYn < YYLAST)
	{
	  YYSIZE_T LayYYsize = 0;
	  char *LayYYmsg;
	  int LayYYx, LayYYcount;

	  LayYYcount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (LayYYx = LayYYn < 0 ? -LayYYn : 0;
	       LayYYx < (int) (sizeof (LayYYtname) / sizeof (char *)); LayYYx++)
	    if (LayYYcheck[LayYYx + LayYYn] == LayYYx)
	      LayYYsize += LayYYstrlen (LayYYtname[LayYYx]) + 15, LayYYcount++;
	  LayYYsize += LayYYstrlen ("parse error, unexpected ") + 1;
	  LayYYsize += LayYYstrlen (LayYYtname[YYTRANSLATE (LayYYchar)]);
	  LayYYmsg = (char *) YYSTACK_ALLOC (LayYYsize);
	  if (LayYYmsg != 0)
	    {
	      char *LayYYp = LayYYstpcpy (LayYYmsg, "parse error, unexpected ");
	      LayYYp = LayYYstpcpy (LayYYp, LayYYtname[YYTRANSLATE (LayYYchar)]);

	      if (LayYYcount < 5)
		{
		  LayYYcount = 0;
		  for (LayYYx = LayYYn < 0 ? -LayYYn : 0;
		       LayYYx < (int) (sizeof (LayYYtname) / sizeof (char *));
		       LayYYx++)
		    if (LayYYcheck[LayYYx + LayYYn] == LayYYx)
		      {
			const char *LayYYq = ! LayYYcount ? ", expecting " : " or ";
			LayYYp = LayYYstpcpy (LayYYp, LayYYq);
			LayYYp = LayYYstpcpy (LayYYp, LayYYtname[LayYYx]);
			LayYYcount++;
		      }
		}
	      LayYYerror (LayYYmsg);
	      YYSTACK_FREE (LayYYmsg);
	    }
	  else
	    LayYYerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	LayYYerror ("parse error");
    }
  goto LayYYerrlab1;


/*--------------------------------------------------.
| LayYYerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
LayYYerrlab1:
  if (LayYYerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (LayYYchar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  LayYYchar, LayYYtname[LayYYchar1]));
      LayYYchar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  LayYYerrstatus = 3;		/* Each real token shifted decrements this */

  goto LayYYerrhandle;


/*-------------------------------------------------------------------.
| LayYYerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
LayYYerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  LayYYn = LayYYdefact[LayYYstate];
  if (LayYYn)
    goto LayYYdefault;
#endif


/*---------------------------------------------------------------.
| LayYYerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
LayYYerrpop:
  if (LayYYssp == LayYYss)
    YYABORT;
  LayYYvsp--;
  LayYYstate = *--LayYYssp;
#if YYLSP_NEEDED
  LayYYlsp--;
#endif

#if YYDEBUG
  if (LayYYdebug)
    {
      short *LayYYssp1 = LayYYss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (LayYYssp1 != LayYYssp)
	YYFPRINTF (stderr, " %d", *++LayYYssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| LayYYerrhandle.  |
`--------------*/
LayYYerrhandle:
  LayYYn = LayYYpact[LayYYstate];
  if (LayYYn == YYFLAG)
    goto LayYYerrdefault;

  LayYYn += YYTERROR;
  if (LayYYn < 0 || LayYYn > YYLAST || LayYYcheck[LayYYn] != YYTERROR)
    goto LayYYerrdefault;

  LayYYn = LayYYtable[LayYYn];
  if (LayYYn < 0)
    {
      if (LayYYn == YYFLAG)
	goto LayYYerrpop;
      LayYYn = -LayYYn;
      goto LayYYreduce;
    }
  else if (LayYYn == 0)
    goto LayYYerrpop;

  if (LayYYn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++LayYYvsp = LayYYlval;
#if YYLSP_NEEDED
  *++LayYYlsp = LayYYlloc;
#endif

  LayYYstate = LayYYn;
  goto LayYYnewstate;


/*-------------------------------------.
| LayYYacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
LayYYacceptlab:
  LayYYresult = 0;
  goto LayYYreturn;

/*-----------------------------------.
| LayYYabortlab -- YYABORT comes here.  |
`-----------------------------------*/
LayYYabortlab:
  LayYYresult = 1;
  goto LayYYreturn;

/*---------------------------------------------.
| LayYYoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
LayYYoverflowlab:
  LayYYerror ("parser stack overflow");
  LayYYresult = 2;
  /* Fall through.  */

LayYYreturn:
#ifndef LayYYoverflow
  if (LayYYss != LayYYssa)
    YYSTACK_FREE (LayYYss);
#endif
  return LayYYresult;
}
#line 252 "laygram.y"


int LayYYwrap ()
{
    return 1;
}

void LayYYsetdest (c)
    LayoutPtr	*c;
{
    dest = c;
}
