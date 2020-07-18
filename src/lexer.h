#ifndef _LEXER_H_
#define _LEXER_H_

#define MAX_STM 200
#define MAX_INP 10000
#define MAX_STRLEN 300
#define MAX_DEFS 100
#define MAX_TOK_LEN 15
#define MAX_TOKENS 10000

#define DEF 100
#define INC 101

// #include "uthash.h"

typedef struct Tokens{
  int type;
  char *value;
} Token;

typedef struct Directives{
  char *directive;
  int type;
} Directive;

// typedef struct Symbols{
//   char *identifier;
//   int type;
//   void *value;
//   UT_hash_handle hh;
// } SymbolTable;
 
Directive *directives(char *);
int getType(char *);
Token *getTokens(char *);
int isunderscore(char);
int isValidToken(char *);
void findLongestToken ( char *token, Token* tokArr, int* tokenCount );

#endif //_LEXER_H_
