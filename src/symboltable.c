#define ST_SIZE 100

#include "symboltable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

//local declarations
int getSymbolPos(char *var);

// SymbolTable
Symbol *st = NULL;
int stSize = 0;

void insertVariable(char *var, int value, int type) {
    if (st == NULL) {
        st = (Symbol *)malloc(sizeof(Symbol) * ST_SIZE);
    }
    Symbol s;
    s.variable = (char *)malloc(sizeof(char) * (strlen(var) + 1));
    // s.value = (char *)malloc(sizeof(char) * MAX_TOK_LEN);

    strcpy(s.variable, var);
    s.value = value;
    s.type = type;

    int pos = getSymbolPos(var);
    if(pos != -1) {
        free(st[pos].variable);
        st[pos] = s;
        return;
    }
    st[stSize] = s;
    // printf("%d - %s\n", stSize, st[stSize].variable);
    stSize++;
}

Symbol *getSymbol(char *var) {
    if (st == NULL) {
        return NULL;
    }
    for (int i = 0; i < stSize; i++) {
        // printf("%d - %s\n", i, st[i].variable);
        if (*(st[i].variable) == *var) {
            return &st[i];
        }
    }
    return NULL;
}

int getSymbolPos(char *var) {
    if (st == NULL) {
        return -1;
    }
    for (int i = 0; i < stSize; i++) {
        // printf("%d - %s\n", i, st[i].variable);
        if (*(st[i].variable) == *var) {
            return i;
        }
    }
    return -1;
}

void freeTable() {
    free(st);
}
