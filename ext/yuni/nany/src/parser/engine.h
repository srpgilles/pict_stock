/*
Kessels engine for GOLD.



See the "readme.txt" file.



Jeroen C. Kessels
Internet Engineer
http://www.kessels.com/

Copyright:   Jeroen C. Kessels
Date:        22 march 2006
Version:     1.8

*/



/* Return values of the Parse() function. */
#define PARSEACCEPT             0   /* Input parsed, no errors. */
#define PARSELEXICALERROR       1   /* Input could not be tokenized. */
#define PARSETOKENERROR         2   /* Input is an invalid token. */
#define PARSESYNTAXERROR        3   /* Input does not match any rule. */
#define PARSECOMMENTERROR       4   /* A comment was started but not finished. */
#define PARSEMEMORYERROR        5   /* Insufficient memory. */


/* SymbolStruct types (defined by GOLD). */
#define SYMBOLNONTERMINAL       0
#define SYMBOLTERMINAL          1
#define SYMBOLWHITESPACE        2
#define SYMBOLEOF               3
#define SYMBOLCOMMENTSTART      4
#define SYMBOLCOMMENTEND        5
#define SYMBOLCOMMENTLINE       6
#define SYMBOLERROR             7


/* ActionStruct types (defined by GOLD). */
#define ACTIONSHIFT             1
#define ACTIONREDUCE            2
#define ACTIONGOTO              3
#define ACTIONACCEPT            4


/* CaseSensitive values (defined by GOLD). */
#define False 0
#define True  1


/* Grammar table and sub-tables. */
struct SymbolStruct {              /* Grammar.SymbolArray[] */
  short Kind;                         /* 0...7, See SYMBOL defines. */
  const wchar_t *Name;                /* String with name of symbol. */
  };
struct DfaEdgeStruct {             /* Grammar.DfaArray[].Edges[] */
  int TargetState;                    /* Index into Grammar.DfaArray[]. */
  int CharCount;                      /* Number of characters in the charset. */
  wchar_t *CharacterSet;              /* String with characters. */
  };
struct DfaStateStruct {            /* Grammar.DfaArray[] */
  int AcceptSymbol;                   /* -1 (Terminal), or index into Grammar.SymbolArray[]. */
  int EdgeCount;                      /* Number of items in Edges[] array. */
  struct DfaEdgeStruct *Edges;        /* Array of DfaEdgeStruct. */
  };
struct RuleStruct {                /* Grammar.RuleArray[] */
  int Head;                           /* Index into Grammar.SymbolArray[]. */
  int SymbolsCount;                   /* Number of items in Symbols[] array. */
  int *Symbols;                       /* Array of indexes into Grammar.SymbolArray[]. */
  const wchar_t* Description;         /* String with BNF of the rule. */
  };
struct ActionStruct {              /* Grammar.LalrArray[].Actions[] */
  int Entry;                          /* Index into Grammar.SymbolArray[]. */
  short Action;                       /* 1...4, see ACTION defines. */
  int Target;                         /* If Action=SHIFT then index into Grammar.LalrArray[]. */
                                      /* If Action=REDUCE then index into Grammar.RuleArray[]. */
                                      /* If Action=GOTO then index into Grammar.LalrArray[]. */
  };
struct LalrStateStruct {           /* Grammar.LalrArray[] */
  int ActionCount;                    /* Number of items in Actions[] array. */
  struct ActionStruct *Actions;       /* Array of ActionStruct. */
  };
struct GrammarStruct {             /* Grammar */
  char CaseSensitive;                 /* 'True' or 'False'. */
  int InitialSymbol;                  /* Index into Grammar.SymbolArray[]. */
  int InitialDfaState;                /* Index into Grammar.DfaArray[]. */
  int InitialLalrState;               /* Index into Grammar.LalrArray[]. */
  int SymbolCount;                    /* Number of items in Grammar.SymbolArray[]. */
  struct SymbolStruct *SymbolArray;
  int RuleCount;                      /* Number of items in Grammar.RuleArray[]. */
  struct RuleStruct *RuleArray;
  int DfaStateCount;                  /* Number of items in Grammar.DfaArray[]. */
  struct DfaStateStruct *DfaArray;
  int LalrStateCount;                 /* Number of items in Grammar.LalrArray[]. */
  struct LalrStateStruct *LalrArray;
  };
extern struct GrammarStruct Grammar;

/* Output from the parser. */
struct TokenStruct {
  int ReductionRule;                  /* Index into Grammar.RuleArray[]. */
  struct TokenStruct **Tokens;        /* Array of reduction Tokens. */
  int Symbol;                         /* Index into Grammar.SymbolArray[]. */
  wchar_t *Data;                      /* String with data from the input. */
  long Line;                          /* Line number in the input. */
  long Column;                        /* Column in the input. */
  };


/* Exported functions. */
int Parse(
  wchar_t *InputBuf,                  /* Pointer to the input data. */
  long InputSize,                     /* Number of characters in the input. */
  int TrimReductions,                 /* 0 = don't trim, 1 = trim reductions. */
  struct TokenStruct **Token);        /* Output, the first Token. */
void DeleteTokens(struct TokenStruct *Token);
wchar_t *RetrieveToken(
  wchar_t *InputBuf,                  /* The input data. */
  long InputSize,                     /* Size of the input data. */
  long *InputHere,                    /* Index into input data. */
  long *Line,                         /* Current line number. */
  long *Column,                       /* Current column number. */
  int *Symbol);
