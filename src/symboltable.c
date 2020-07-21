#define ST_SIZE 100

#include "symboltable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

//local declarations
//int getSymbolPos(char *var);

// SymbolTable
Symbol *st = NULL;

void insertVariable(char *var, int value, int type) {
    Symbol *tmp = getSymbol(var);
    if(tmp != NULL)
    {
        tmp->value = value;
        return;
    }

    Symbol *s = (Symbol *)malloc(sizeof(Symbol));

    s->variable = (char *)malloc(sizeof(char) * (strlen(var) + 1));
    // s.value = (char *)malloc(sizeof(char) * MAX_TOK_LEN);

    strcpy(s->variable, var);
    s->value = value;
    s->type = type;
    s->next = NULL;

    if(st == NULL)
    {
        st = s;
    }
    else
    {
        tmp = st;
        while(tmp->next)
        {
            tmp = tmp->next;
        }
        tmp->next = s;
    }
}

Symbol *getSymbol(char *var) {
    Symbol *tmp = st;

    while (tmp) {
        // printf("%d - %s\n", i, st[i].variable);
        if (*tmp->variable == *var) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}
/*
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
}*/

void freeTable() {
    //free(st);
    Symbol *tmp;

    while(st)
    {
        tmp = st->next;
        free(st);
        st = tmp;
    }
    st = NULL;
}
