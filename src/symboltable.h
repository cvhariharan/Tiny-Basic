#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_

typedef struct Symbols{
    char *variable;
    int type;
    int value;
    struct Symbols * next;
} Symbol;

Symbol *getSymbol(char *);
void insertVariable(char *, int, int);
void freeTable();

#endif //_SYMBOLTABLE_H_
