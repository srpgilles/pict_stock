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



#include <stdio.h>                  /* fprintf() */
#include <stdlib.h>                 /* malloc(), free() */
#include <wchar.h>                  /* wchar_t */

#include "engine.h"                 /* Defines all the data structs. */




/* FIFO (first in first out) stack of Tokens. */
struct TokenStackStruct {
  struct TokenStruct *Token;
  int LalrState;                      /* Index into Grammar.LalrArray[]. */
  struct TokenStackStruct *NextToken; /* Pointer to next item. */
  };




/********* DFA Tokenizer *********************************************************/




/* Read 'Length' characters from the InputBuf and return as a
   malloc'ed string. The string is terminated by appending a zero,
   although the routine is binary-safe.
   Return NULL if out of memory. */
wchar_t *ReadString(
    wchar_t *InputBuf,                  /* The input data. */
    long InputSize,                     /* Size of the input data. */
    long *InputHere,                    /* Index into input data. */
    long *Line,                         /* Current line number. */
    long *Column,                       /* Current column number. */
    long Length) {                      /* Number of characters to copy. */
  wchar_t *String;
  long i;

  String = (wchar_t *)malloc((Length + 1) * sizeof(wchar_t));
  if (String == NULL) return(NULL);
  for (i = 0; i < Length; i++) {
    if (*InputHere < InputSize) {
        if (InputBuf[*InputHere] == '\r') {
          if ((*InputHere + 1 < InputSize) &&
              (InputBuf[*InputHere + 1] != '\n')) {
            *Line = *Line + 1;
            *Column = 0;
            }
          }
        if (InputBuf[*InputHere] == '\n') {
          *Line = *Line + 1;
          *Column = 0;
          }
        String[i] = InputBuf[*InputHere];
        *InputHere = *InputHere + 1;
        *Column = *Column + 1;
      } else {
        String[i] = 0;
        }
    }
  String[i] = 0;
  return(String);
  }




/* Search for a character in a characterset. Return 1 if found,
   0 if not found. */
int FindChar(wchar_t ThisChar, wchar_t *CharacterSet, long Count) {
  long Here;
  long Interval;

  /* Use wcschr() for charactersets with a length of up to 11
     characters. */
  if (Count < 11) {
    if (wcschr(CharacterSet,ThisChar) != NULL) return(1);
    return(0);
    }

  /* Binary search the characterset for the character. This method is
     possible because GOLD always produces sorted charactersets.
     Measurements show that although the code is more complex, this
     binary search is faster than wcschr() for charactersets longer
     than 11 characters. At 100 characters it's 4 times faster. */
  Interval = 32768;
  while (Interval > Count) Interval = (Interval >> 1);
  Here = Interval - 1;
  Interval = (Interval >> 1);
  while (Interval > 0) {
    if (CharacterSet[Here] == ThisChar) return(1);
    if (CharacterSet[Here] > ThisChar) {
        Here = Here - Interval;
      } else {
        while (Here + Interval >= Count) {
          Interval = (Interval >> 1);
          if (Interval == 0) return(0);
          }
        Here = Here + Interval;
        }
    Interval = (Interval >> 1);
    }
  if (CharacterSet[Here] == ThisChar) return(1);
  return(0);
  }





/* Parse a symbol from the InputBuf.
   Symbols are lexical entities of 1 or more characters. The parser
   uses the Grammar.DfaArray[] compiled by GOLD, which is basically
   a tree of charactersets. The parser takes characters from the
   input and descends the tree by looking for the character in the
   characterset. At the last branch of the tree is an index into the
   Grammar.SymbolsArray[], also compiled by GOLD.
   Return the index into Grammar.SymbolsArray[], and a pointer to a
   malloc'ed string with characters from the input.
   Symbol 0 is 'SYMBOLEOF' (string is NULL)
   Symbol 1 is 'SYMBOLERROR' (string is 1 character).
   */
wchar_t *RetrieveToken(
    wchar_t *InputBuf,                  /* The input data. */
    long InputSize,                     /* Size of the input data. */
    long *InputHere,                    /* Index into input data. */
    long *Line,                         /* Current line number. */
    long *Column,                       /* Current column number. */
    int *Symbol) {
  int DfaIndex;       /* Index into Grammar.DfaArray[]. */
  long Length;        /* Number of processed characters from Data->InputBuf. */
  int AcceptIndex;    /* Longest found symbol so far. */
  long AcceptLength;  /* Length of longest found symbol. */
  long i;

  /* Sanity check (no input). */
  if ((InputBuf == NULL) || (InputBuf[0] == 0)) {
    *Symbol = 0;
    return(NULL);
    }

  /* If there are no more characters in the input then return SYMBOLEOF
     and NULL. */
  if (*InputHere >= InputSize) {
    *Symbol = 0;
    return(NULL);
    }

  /* Compare characters from the input with the DFA charactersets until
     not found. */
  DfaIndex = Grammar.InitialDfaState;
  Length = 0;
  AcceptLength = 0;
  AcceptIndex = -1;
  while (*InputHere + Length < InputSize) {
    /* If this is a valid symbol-terminal then save it. We know the
       input matches the symbol, but there may be a longer symbol that
       matches so we have to keep scanning. */
    if (Grammar.DfaArray[DfaIndex].AcceptSymbol >= 0) {
      AcceptIndex = DfaIndex;
      AcceptLength = Length;
      }

    /* Walk through the edges and scan the characterset of each edge for
       the current character. */
    for (i = 0; i < Grammar.DfaArray[DfaIndex].EdgeCount; i++) {
      if (FindChar(InputBuf[*InputHere + Length],
        Grammar.DfaArray[DfaIndex].Edges[i].CharacterSet,
        Grammar.DfaArray[DfaIndex].Edges[i].CharCount) == 1) break;
      }

    /* If not found then exit the loop. */
    if (i >= Grammar.DfaArray[DfaIndex].EdgeCount) break;

    /* Jump to the TargetState, which points to another set of DFA edges
       describing the next character. */
    DfaIndex = Grammar.DfaArray[DfaIndex].Edges[i].TargetState;

    /* Increment the Length, we have handled the character. */
    Length = Length + 1;
    }

  /* If the DFA is a terminal then return the Symbol, and Length characters
     from the input. */
  if (Grammar.DfaArray[DfaIndex].AcceptSymbol >= 0) {
    *Symbol = Grammar.DfaArray[DfaIndex].AcceptSymbol;
    return(ReadString(InputBuf,InputSize,InputHere,Line,Column,Length));
    }

  /* If we found a shorter terminal before, then return that Symbol, and
     it's characters. */
  if (AcceptIndex >= 0) {
    *Symbol = Grammar.DfaArray[AcceptIndex].AcceptSymbol;
    return(ReadString(InputBuf,InputSize,InputHere,Line,Column,AcceptLength));
    }

  /* Return SYMBOLERROR and a string with 1 character from the input. */
  *Symbol = 1;
  return(ReadString(InputBuf,InputSize,InputHere,Line,Column,1));
  }




/********* LALR Tokenizer ********************************************************/




/* LALR state machine. Depending on the Token->Symbol the machine will
   change it's state and perform actions, such as reduce the TokenStack and
   iteratively call itself. */
#define LALRMEMORYERROR   0
#define LALRSYNTAXERROR   1
#define LALRACCEPT        2
#define LALRSHIFT         3
#define LALRGOTO          4
int ParseToken(
    int *LalrState,
    struct TokenStackStruct **TokenStack,
    struct TokenStackStruct *InputToken,
    int TrimReductions) {
  struct TokenStackStruct *PopToken;
  struct TokenStackStruct *Reduction;
  int Action;
  int Rule;
  int i;

  /* Find the Token->Symbol in the LALR table. */
  Action = 0;
  while (Action < Grammar.LalrArray[*LalrState].ActionCount) {
    if (Grammar.LalrArray[*LalrState].Actions[Action].Entry == InputToken->Token->Symbol) {
      break;
      }
    Action++;
    }

  /* If not found then exit with SYNTAXERROR. The Token is not allowed in this
     context. */
  if (Action >= Grammar.LalrArray[*LalrState].ActionCount) {
    return(LALRSYNTAXERROR);
    }

  /* ACTIONACCEPT: exit. We're finished parsing the input. */
  if (Grammar.LalrArray[*LalrState].Actions[Action].Action == ACTIONACCEPT) {
    return(LALRACCEPT);
    }

  /* ACTIONSHIFT: switch the LALR state and return. We're ready to accept
     the next token. */
  if (Grammar.LalrArray[*LalrState].Actions[Action].Action == ACTIONSHIFT) {
    *LalrState = Grammar.LalrArray[*LalrState].Actions[Action].Target;
    return(LALRSHIFT);
    }

  /* ACTIONGOTO: switch the LALR state and return. We're ready to accept
     the next token.
     Note: In my implementation SHIFT and GOTO do the exact same thing. As far
     as I can tell GOTO only happens just after a reduction. Perhaps GOLD makes
     the difference to allow the program to perform special actions, which my
     implementation does not need. */
  if (Grammar.LalrArray[*LalrState].Actions[Action].Action == ACTIONGOTO) {
    *LalrState = Grammar.LalrArray[*LalrState].Actions[Action].Target;
    return(LALRGOTO);
    }

  /* ACTIONREDUCE:
     Create a new Reduction according to the Rule that is specified by the action.
     - Create a new Reduction in the ReductionArray.
     - Pop tokens from the TokenStack and add them to the Reduction.
     - Push a new token on the TokenStack for the Reduction.
     - Iterate.
     */
  Rule = Grammar.LalrArray[*LalrState].Actions[Action].Target;

  /* If TrimReductions is active, and the Rule contains a single non-terminal,
     then eleminate the unneeded reduction by modifying the Rule on the stack
     into this Rule.
     */
  if ((TrimReductions != 0) &&
      (Grammar.RuleArray[Rule].SymbolsCount == 1) &&
      (Grammar.SymbolArray[Grammar.RuleArray[Rule].Symbols[0]].Kind == SYMBOLNONTERMINAL)) {

    /* Pop the Rule from the TokenStack. */
    PopToken = *TokenStack;
    *TokenStack = PopToken->NextToken;

    /* Rewind the LALR state. */
    *LalrState = PopToken->LalrState;

    /* Change the Token into the Rule. */
    PopToken->Token->Symbol = Grammar.RuleArray[Rule].Head;

    /* Feed the Token to the LALR state machine. */
    ParseToken(LalrState,TokenStack,PopToken,TrimReductions);

    /* Push the modified Token back onto the TokenStack. */
    PopToken->NextToken = *TokenStack;
    *TokenStack = PopToken;

    /* Save the new LALR state in the input token. */
    InputToken->LalrState = *LalrState;

    /* Feed the input Token to the LALR state machine and exit. */
    return(ParseToken(LalrState,TokenStack,InputToken,TrimReductions));
    }

  /* Allocate and initialize memory for the Reduction. */
  Reduction = (struct TokenStackStruct *)malloc(sizeof(struct TokenStackStruct));
  if (Reduction == NULL) return(LALRMEMORYERROR);
  Reduction->Token = (struct TokenStruct *)malloc(sizeof(struct TokenStruct));
  if (Reduction->Token == NULL) {
    free(Reduction);
    return(LALRMEMORYERROR);
    }
  Reduction->Token->ReductionRule = Rule;
  Reduction->Token->Tokens = (struct TokenStruct **)malloc(
    sizeof(struct TokenStruct *) * Grammar.RuleArray[Rule].SymbolsCount);
  if (Reduction->Token->Tokens == NULL) {
    free(Reduction->Token);
    free(Reduction);
    return(LALRMEMORYERROR);
    }
  Reduction->Token->Symbol = Grammar.RuleArray[Rule].Head;
  Reduction->Token->Data = NULL;
  Reduction->Token->Line = InputToken->Token->Line;
  Reduction->Token->Column = InputToken->Token->Column;
  Reduction->LalrState = *LalrState;
  Reduction->NextToken = NULL;

  /* Reduce tokens from the TokenStack by moving them to the Reduction.
     The Lalr state will be rewound to the state it was for the first
     symbol of the rule. */
  for (i = Grammar.RuleArray[Rule].SymbolsCount; i > 0; i--) {
    PopToken = *TokenStack;
    *TokenStack = PopToken->NextToken;
    PopToken->NextToken = NULL;
    Reduction->Token->Tokens[i - 1] = PopToken->Token;
    *LalrState = PopToken->LalrState;
    Reduction->LalrState = PopToken->LalrState;
    Reduction->Token->Line = PopToken->Token->Line;
    Reduction->Token->Column = PopToken->Token->Column;
    free(PopToken);
    }

  /* Call the LALR state machine with the Symbol of the Rule. */
  ParseToken(LalrState,TokenStack,Reduction,TrimReductions);

  /* Push new Token on the TokenStack for the Reduction. */
  Reduction->NextToken = *TokenStack;
  *TokenStack = Reduction;

  /* Save the current LALR state in the InputToken. We need this to be
     able to rewind the state when reducing. */
  InputToken->LalrState = *LalrState;

  /* Call the LALR state machine with the InputToken. The state has
     changed because of the reduction, so we must accept the token
     again. */
  return(ParseToken(LalrState,TokenStack,InputToken,TrimReductions));
  }




/********* Exported functions ****************************************************/




/* Delete a Token from memory, including all the reductions and data. */
void DeleteTokens(struct TokenStruct *Token) {
  int i;

  if (Token == NULL) return;
  if (Token->Data != NULL) free(Token->Data);
  if (Token->ReductionRule >= 0) {
    for (i = 0; i < Grammar.RuleArray[Token->ReductionRule].SymbolsCount; i++) {
      DeleteTokens(Token->Tokens[i]);
      }
    free(Token->Tokens);
    }
  }




void ParseCleanup(
    struct TokenStackStruct *Top,
    struct TokenStackStruct *New,
    struct TokenStruct **FirstToken) {
  struct TokenStackStruct *OldTop;

  *FirstToken = NULL;
  if (Top != NULL) {
    *FirstToken = Top->Token;
    OldTop = Top;
    Top = Top->NextToken;
    free(OldTop);
    }
  if (New != NULL) {
    DeleteTokens(*FirstToken);
    *FirstToken = New->Token;
    free(New);
    }
  while (Top != NULL) {
    DeleteTokens(Top->Token);
    OldTop = Top;
    Top = Top->NextToken;
    free(OldTop);
    }
  }




/* Parse the input data.
   Returns a pointer to a ParserData struct, NULL if insufficient memory.
   The Data->Result value will be one of these values:
        PARSEACCEPT             Input parsed, no errors.
        PARSELEXICALERROR       Input could not be tokenized.
        PARSETOKENERROR         Input is an invalid token.
        PARSESYNTAXERROR        Input does not match any rule.
        PARSECOMMENTERROR       A comment was started but not finished.
        PARSEMEMORYERROR        Insufficient memory.
   */
int Parse(
    wchar_t *InputBuf,                   /* Pointer to the input data. */
    long InputSize,                      /* Number of characters in the input. */
    int TrimReductions,                  /* 0 = don't trim, 1 = trim reductions. */
    struct TokenStruct **FirstToken) {   /* Output token. */
  int LalrState;                         /* Index into Grammar.LalrArray[]. */
  struct TokenStackStruct *TokenStack;   /* Stack of Tokens. */
  struct TokenStackStruct *Work;     /* Current token. */
  long InputHere;                         /* Index into input. */
  long Line;                              /* Line number. */
  long Column;                            /* Column number. */
  int CommentLevel;                      /* Used when skipping comments, nested comment count. */
  int Result;                            /* Result from ParseToken(). */

  /* Initialize variables. */
  LalrState = Grammar.InitialLalrState;
  TokenStack = NULL;
  InputHere = 0;
  Line = 1;
  Column = 1;
  CommentLevel = 0;
  *FirstToken = NULL;

  /* Sanity check. */
  if ((InputBuf == NULL) || (*InputBuf == 0) || (InputSize == 0)) {
    return(PARSEACCEPT);
    }

  /* Accept tokens until finished. */
  while (1) {

    /* Create a new Token. Exit if out of memory. */
    Work = (struct TokenStackStruct *)malloc(sizeof(struct TokenStackStruct));
    if (Work == NULL) {
      ParseCleanup(TokenStack,NULL,FirstToken);
      return(PARSEMEMORYERROR);
      }
    Work->LalrState = LalrState;
    Work->NextToken = NULL;
    Work->Token = (struct TokenStruct *)malloc(sizeof(struct TokenStruct));
    if (Work->Token == NULL) {
      free(Work);
      ParseCleanup(TokenStack,NULL,FirstToken);
      return(PARSEMEMORYERROR);
      }
    Work->Token->ReductionRule = -1;
    Work->Token->Tokens = NULL;
    Work->Token->Line = Line;
    Work->Token->Column = Column;

    /* Call the DFA tokenizer and parse a token from the input. */
    Work->Token->Data = RetrieveToken(InputBuf,InputSize,&InputHere,&Line,
      &Column,&Work->Token->Symbol);
    if ((Work->Token->Data == NULL) && (Work->Token->Symbol != 0)) {
      ParseCleanup(TokenStack,Work,FirstToken);
      return(PARSEMEMORYERROR);
      }

    /* If we are inside a comment then ignore everything except the end
       of the comment, or the start of a nested comment. */
    if (CommentLevel > 0) {
      /* Begin of nested comment: */
      if (Grammar.SymbolArray[Work->Token->Symbol].Kind == SYMBOLCOMMENTSTART) {
        /* Push the Token on the TokenStack to keep track of line+column. */
        Work->NextToken = TokenStack;
        TokenStack = Work;

        CommentLevel = CommentLevel + 1;
        continue;
        }

      /* End of comment: */
      if (Grammar.SymbolArray[Work->Token->Symbol].Kind == SYMBOLCOMMENTEND) {
        /* Delete the Token. */
        if (Work->Token->Data != NULL) free(Work->Token->Data);
        free(Work->Token);
        free(Work);

        /* Pop the comment-start Token from the TokenStack and delete
           that as well. */
        Work = TokenStack;
        TokenStack = Work->NextToken;
        if (Work->Token->Data != NULL) free(Work->Token->Data);
        free(Work->Token);
        free(Work);

        CommentLevel = CommentLevel - 1;
        continue;
        }

      /* End of file: Error exit. A comment was started but not finished. */
      if (Grammar.SymbolArray[Work->Token->Symbol].Kind == SYMBOLEOF) {
        if (Work->Token->Data != NULL) free(Work->Token->Data);
        free(Work->Token);
        free(Work);
        ParseCleanup(TokenStack,NULL,FirstToken);
        return(PARSECOMMENTERROR);
        }

      /* Any other Token: delete and loop. */
      if (Work->Token->Data != NULL) free(Work->Token->Data);
      free(Work->Token);
      free(Work);

      continue;
      }

    /* If the token is the start of a comment then increment the
       CommentLevel and loop. The routine will keep reading tokens
       until the end of the comment. */
    if (Grammar.SymbolArray[Work->Token->Symbol].Kind == SYMBOLCOMMENTSTART) {

      /* Push the Token on the TokenStack to keep track of line+column. */
      Work->NextToken = TokenStack;
      TokenStack = Work;

      CommentLevel = CommentLevel + 1;
      continue;
      }

    /* If the token is the start of a linecomment then skip the rest
       of the line. */
    if (Grammar.SymbolArray[Work->Token->Symbol].Kind == SYMBOLCOMMENTLINE) {
      if (Work->Token->Data != NULL) free(Work->Token->Data);
      free(Work->Token);
      free(Work);
      while ((InputHere < InputSize) &&
             (InputBuf[InputHere] != '\r') &&
             (InputBuf[InputHere] != '\n')) {
        InputHere = InputHere + 1;
        }
      if ((InputHere < InputSize) &&
          (InputBuf[InputHere] == '\r')) {
        InputHere = InputHere + 1;
        }
      if ((InputHere < InputSize) &&
          (InputBuf[InputHere] == '\n')) {
        InputHere = InputHere + 1;
        }
      Line = Line + 1;
      Column = 1;
      continue;
      }

    /* If parse error then exit. */
    if (Grammar.SymbolArray[Work->Token->Symbol].Kind == SYMBOLERROR) {
      ParseCleanup(TokenStack,Work,FirstToken);
      return(PARSELEXICALERROR);
      }

    /* Ignore whitespace. */
    if (Grammar.SymbolArray[Work->Token->Symbol].Kind == SYMBOLWHITESPACE) {
      if (Work->Token->Data != NULL) free(Work->Token->Data);
      free(Work->Token);
      free(Work);
      continue;
      }

    /* The tokenizer should never return a non-terminal symbol. */
    if (Grammar.SymbolArray[Work->Token->Symbol].Kind == SYMBOLNONTERMINAL) {
      ParseCleanup(TokenStack,Work,FirstToken);
      return(PARSETOKENERROR);
      }

    /* Feed the Symbol to the LALR state machine. It can do several
       things, such as wind back and iteratively call itself. */
    Result = ParseToken(&LalrState,&TokenStack,Work,TrimReductions);

    /* If out of memory then exit. */
    if (Result == LALRMEMORYERROR) {
      ParseCleanup(TokenStack,Work,FirstToken);
      return(PARSEMEMORYERROR);
      }

    /* If syntax error then exit. */
    if (Result == LALRSYNTAXERROR) {
      /* Return LALR state in the Token->Symbol. */
      Work->Token->Symbol = LalrState;
      ParseCleanup(TokenStack,Work,FirstToken);
      return(PARSESYNTAXERROR);
      }

    /* Exit if the LALR state machine says it has reached it's exit. */
    if (Result == LALRACCEPT) {
      if (Grammar.SymbolArray[Work->Token->Symbol].Kind == SYMBOLEOF) {
        if (Work->Token->Data != NULL) free(Work->Token->Data);
        free(Work->Token);
        free(Work);
        }
      ParseCleanup(TokenStack,NULL,FirstToken);
      return(PARSEACCEPT);
      }

    /* Push the token onto the TokenStack. */
    Work->NextToken = TokenStack;
    TokenStack = Work;
    }

  /* Should never get here. */
  }
