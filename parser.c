#define FUNCTION_CALL 200
#define FUNCTION_PROTO 205
#define LINES 100

#include "lexer.h"
#include "tokens.h"
#include "symboltable.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char *parseExprList();

// Global Variables
int tokenIndex = 0;
Token *tokArr;
char *mappings[LINES];
char *program[LINES];

//Program counter
int pc = 0;

int main(int argc, char *argv[])
{
  char *stmts[MAX_STM];
  char *input, *filename;
  FILE *fp;

  tokArr = malloc(sizeof(Token) * MAX_TOKENS);
  input = malloc(sizeof(char) * MAX_STRLEN);
  // printf("Started");
  // filename = (char *)malloc(MAX_STRLEN * sizeof(char));
  // printf("Valid: %d\n", isValidToken(";"));
  // if (argc == 2)
  // {
    // filename = argv[1];
    FILE *fptr;
    if ((fptr = fopen("t.tb", "r")) == NULL)
    {
      printf("Error! opening file");
      // Program exits if file pointer returns NULL.
      exit(1);
    }

    // Read the file into program array
    int i = 0;
    while (fgets(input, 255, (FILE *)fptr))
    {
      program[i] = (char *)malloc(sizeof(char) * MAX_STM);
      strcpy(program[i], input);
      tokenIndex = 0;
      tokArr = getTokens(program[i]);
      parseLine();
      // for (int j = 0; j < MAX_TOKENS; j++)
      // {
      //   if (tokArr[j].value != NULL)
      //   {
      //     printf("%s : %d\n", tokArr[j].value, tokArr[j].type);
      //   }
      // }
      i++;
      // printf("%d\n", i);
    }
  // }
  return 0;
}

// Read and execute the line referenced by the PC
// void next() {
//   tokArr = getTokens(program[pc]);
//   pc++;
//   parseLine();
// }

void advance()
{
  tokenIndex++;
}

Token eat(int type)
{
  //Returns the type if correct
  if (tokArr[tokenIndex].type == type)
  {
    advance();
    return tokArr[tokenIndex - 1];
  }
  else
  {
    printf("Error: Could not parse");
    // exit(1);
  }
}

// Returns the relations token type
int getRelational(int token)
{
  if (token == EQUALTO || token == GT || token == LT || token == NOTEQUALTO || token == GEQUAL || token == LEQUAL)
  {
    return token;
  }
  return -1;
}

int parseLine()
{
  if (tokArr[tokenIndex].type == NUM)
  {
    // mappings[pos] = (char *)malloc(sizeof(char) * MAX_TOK_LEN);
    mappings[pc] = eat(NUM).value;
  }
  parseStatement();
  // printf("Oth %s\n", tokArr[tokenIndex].value);
}

int parseStatement()
{
  switch (tokArr[tokenIndex].type)
  {
  case LET:
  {
    Token var;
    eat(LET);
    var = eat(ID);
    eat(ASSIGN);
    // printf("DEBUG1: %s\n", var.value);
    int val = parseExpression();
    // printf("Stored val: %d", val);
    // printf("DEBUG2: %d", val);
    // printf("Test\n"); 
    insertVariable(var.value, val, NUM);
    break;
  }
  

  case PRINT:
    eat(PRINT);
    
    printf("%s\n", parseExprList());
    break;

  case IF:
  {
    int cond = 0;
    eat(IF);
    int op1 = parseExpression();
    int operator= eat(getRelational(tokArr[tokenIndex].type)).type;
    int op2 = parseExpression();
    switch (operator)
    {
    case EQUALTO:
      if (op1 == op2)
      {
        cond = 1;
      }
      break;
    case GT:
      if (op1 > op2)
      {
        cond = 1;
      }
      break;
    case LT:
      if (op1 < op2)
      {
        cond = 1;
      }
      break;
    case NOTEQUALTO:
      if (op1 != op2)
      {
        cond = 1;
      }
      break;
    case GEQUAL:
      if (op1 >= op2)
      {
        cond = 1;
      }
      break;
    case LEQUAL:
      if (op1 <= op2)
      {
        cond = 1;
      }
      
    }
      break;
    if (cond)
    {
      eat(THEN);
      parseStatement();
    }
    break;
  }
  
  case INPUT:
  {
    Token var;
    eat(INPUT);

    // var-list parser
    var = tokArr[tokenIndex];
    tokenIndex++;
    while (var.type != ENTER)
    {
      int t;
      Symbol *sVar = getSymbol(var.value);
      if (sVar != NULL)
      {
        scanf("%d", &t);
        sVar->value = t;
      }
      else
      {
        printf("Variable Not Found!");
      }
      eat(COMMA);
      var = tokArr[tokenIndex];
      tokenIndex++;
    }
    break;
  }
  }
  eat(ENTER);
  // printf("PRINT\n");
  return 0;
}

char *parseExprList() {
  char *expr = (char *) malloc(sizeof(char) * MAX_INP);
  strcpy(expr, "");
//   printf("D: %s - %d\n", tokArr[tokenIndex].value, tokArr[tokenIndex].type);
  if(tokArr[tokenIndex].type == S_LITERAL) {
    // printf("Sl\n");
    strcat(expr, tokArr[tokenIndex].value);
    eat(S_LITERAL);
  } else {
    // printf("Exprs\n");
    int val = parseExpression();
    char value[5];
    sprintf(value, "%d", val);
    strcat(expr, value);
  }

  if(tokArr[tokenIndex].type == COMMA) {
    eat(COMMA);
  }
  if(tokArr[tokenIndex].type != ENTER) {
    strcat(expr, parseExprList());
  }
  return expr;
}

int parseExpression()
{
  int op = parseTerm();
  // printf("DEBUG8: %d\n", op);
  // if(isRelational(tokArr[tokenIndex].type)) {
  //   parseRelational();
  //   parseTerm();
  // }
  // else {
  switch (tokArr[tokenIndex].type)
  {
  case PLUS:
    eat(PLUS);
    op = op + parseTerm();
    break;
  case SUB:
    eat(SUB);
    op = op - parseTerm();
    break;
  }
  return op;
}

int parseTerm()
{
  int op = parseFactor();
  
  switch (tokArr[tokenIndex].type)
  {
  case MUL:
    eat(MUL);
    op = op * parseFactor();
  case DIV:
    eat(DIV);
    op = op / parseFactor();
  }
  return op;
}

int parseFactor()
{
  // printf("PF: \n");
  switch (tokArr[tokenIndex].type)
  {
  case ID:
  {
    // printf("ID\n");
    Token var;
    var = eat(ID);
    // printf("TEST\n");
    // printf("DEBUG: %d\n", getSymbol(var.value)->value);
    return getSymbol(var.value)->value;
  }
  break;
  case NUM: {
    Token var;
    var = eat(NUM);
    
    return atoi(var.value);
  }
    // default: error();
  }
  if (tokArr[tokenIndex].type == LEFTPAR)
  {
    int op = parseExpression();
    eat(RIGHTPAR);
    return op;
  }
}
