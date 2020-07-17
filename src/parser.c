#define FUNCTION_CALL 200
#define FUNCTION_PROTO 205
#define LINES 100
#define STAKE_SIZE 1024

#include "lexer.h"
#include "symboltable.h"
#include "tokens.h"
#include "core.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void advance();

Token eat(int);

char *parseExprList();

int getRelational(int);
int parseLine();
int parseStatement();
int parseExpression();
int parseTerm();
int parseFactor();

// Global Variables
int tokenIndex = 0;
Token *tokArr;
char *mappings[LINES];
char *program[LINES];
Stack * stack=NULL;

// Program counter
int pc = 0;

int main(int argc, char *argv[]) {
    //char *stmts[MAX_STM];
    char *input, *filename;
    //FILE *fp;

    // tokArr = malloc(sizeof(Token) * MAX_TOKENS);
    input = malloc(sizeof(char) * MAX_STRLEN);
    // printf("Started");
    filename = (char *)malloc(MAX_STRLEN * sizeof(char));
    if (argc == 2) {
        filename = argv[1];
        FILE *fptr;
        if ((fptr = fopen(filename, "r")) == NULL) {
            printf("Error! opening file");
            // Program exits if file pointer returns NULL.
            exit(1);
        }

        // Read the file into program array
        int i = 0;
        while (fgets(input, 255, (FILE *)fptr)) {
            program[i] = (char *)malloc(sizeof(char) * MAX_STM);
            strcpy(program[i], input);
            Token *t = getTokens(program[i]);
            if (t[0].type == NUM) {
                mappings[i] = t[0].value;
            }
            i++;
        }

        while (pc < i) {
            tokenIndex = 0;
            tokArr = getTokens(program[pc]);

            pc++;

        
            parseLine();
            free(tokArr);
        }
        for (int j = 0; j < i; j++) {
            free(program[i]);
        }
        free(input);
        freeTable();
    }
    return 0;
}

// Push to stack
void push(int n) {
    Stack * tmp= (Stack *)malloc(sizeof(Stack));
    tmp->prev=stack;
    tmp->value = n;
    //and move to the new node
    stack = tmp;
}

// Pop from stack
int pop() {
    if(stack ==NULL)
    {
        return -1;
    }
    int n = stack->value;
    Stack * tmp = stack;
    
    stack=stack->prev;//return 1 step
    free(tmp);
    
    return n;
}

// Get the top value from the stack
int peek() {
    return stack->value;
}

int getLabelPos(char *label) {
    for (int i = 0; i < LINES; i++) {
        if (mappings[i] != NULL) {
            if (strcmp(mappings[i], label) == 0) {
                return i;
            }
        }
    }
    return -1;
}

// advance the tokencount
void advance() { tokenIndex++; }

Token eat(int type) {
    // Returns the type if correct
    if (tokArr[tokenIndex].type == type) {
        advance();
        return tokArr[tokenIndex - 1];
    } else {
        printf("Error: Could not parse\n");
        // exit(1);
    }
}

// Returns the relations token type
int getRelational(int token) {
    if (token == EQUALTO || token == GT || token == LT || token == NOTEQUALTO ||
        token == GEQUAL || token == LEQUAL) {
        return token;
    }
    return -1;
}

int parseLine() {
    if (tokArr[tokenIndex].type == NUM) {
        eat(NUM);
    }
    parseStatement();
}

int parseStatement() {
    switch (tokArr[tokenIndex].type) {
        case LET: {
            Token var;
            eat(LET);
            var = eat(ID);
            eat(ASSIGN);
            int val = parseExpression();
            insertVariable(var.value, val, NUM);
        }
            eat(ENTER);
            break;

        case PRINT:
            eat(PRINT);
            char *val = parseExprList();
            printf("\n%s", val);
            free(val);
            eat(ENTER);
            break;

        case IF: {
            int cond = 0;
            eat(IF);
            int op1 = parseExpression();
            int operator= eat(getRelational(tokArr[tokenIndex].type)).type;
            int op2 = parseExpression();
            switch (operator) {
                case EQUALTO:
                    if (op1 == op2) {
                        cond = 1;
                    }
                    break;
                case GT:
                    if (op1 > op2) {
                        cond = 1;
                    }
                    break;
                case LT:
                    if (op1 < op2) {
                        cond = 1;
                    }
                    break;
                case NOTEQUALTO:
                    if (op1 != op2) {
                        cond = 1;
                    }
                    break;
                case GEQUAL:
                    if (op1 >= op2) {
                        cond = 1;
                    }
                    break;
                case LEQUAL:
                    if (op1 <= op2) {
                        cond = 1;
                    }
                    break;
            }
            // printf("DEBUG COND: %d\n", cond);

            if (cond) {
                eat(THEN);
                parseStatement();
            }
            break;
        }

        case INPUT: {
            Token var;
            eat(INPUT);

            // var-list parser
            var = tokArr[tokenIndex];
            tokenIndex++;
            while (var.type != ENTER) {
                int t;
                Symbol *sVar = getSymbol(var.value);
                if (sVar != NULL) {
                    scanf("%d", &t);
                    sVar->value = t;
                } else {
                    printf("Variable Not Found!");
                }
                if (tokArr[tokenIndex].type != COMMA) {
                    break;
                }
                eat(COMMA);
                var = tokArr[tokenIndex];
                tokenIndex++;
            }
        }
            eat(ENTER);
            break;

        case GOTO: {
            eat(GOTO);
            int value = parseExpression();
            char label[5];
            sprintf(label, "%d", value);
            pc = getLabelPos(label);
            // printf("DEBUG: %d\n", pc);
        }
            eat(ENTER);
            break;

        case GOSUB: {
            push(pc);
            eat(GOSUB);
            int value = parseExpression();
            char label[5];
            sprintf(label, "%d", value);
            pc = getLabelPos(label);
        }
            eat(ENTER);
            break;

        case RETURN: {
            eat(RETURN);
            int jmp = pop();
            // printf("DEBUG: %d\n", jmp);
            if (jmp != -1) {
                // pc is incremented before being pushed to the stack
                pc = jmp;
            }
        }
            eat(ENTER);
            break;
        
        case REM:
            //Processing comments
            break;

        case END: {
            exit(0);
        }
    }
    // printf("PRINT\n");
    return 0;
}

char *parseExprList() {
    char *expr = (char *)malloc(sizeof(char) * MAX_INP);
    strcpy(expr, "");
    if (tokArr[tokenIndex].type == S_LITERAL) {
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

    if (tokArr[tokenIndex].type == COMMA) {
        eat(COMMA);
        strcat(expr, parseExprList());
    }

    return expr;
}

int parseExpression() {
    int op = parseTerm();
    switch (tokArr[tokenIndex].type) {
        case PLUS:
            eat(PLUS);
            op = op + parseTerm();
            // printf("DEBUG EXPR: %d\n", op);
            break;
        case SUB:
            eat(SUB);
            op = op - parseTerm();
            break;
    }
    return op;
}

int parseTerm() {
    int op = parseFactor();
    switch (tokArr[tokenIndex].type) {
        case MUL:
            eat(MUL);
            op = op * parseFactor();
            // printf("DEBUG TERM: %d\n", op);
            break;
        case DIV:
            eat(DIV);
            op = op / parseFactor();
            // printf("DEBUG TERM: %d\n", op);
            break;
    }
    return op;
}

int parseFactor() {
    switch (tokArr[tokenIndex].type) {
        case ID: {
            // printf("ID\n");
            Token var;
            var = eat(ID);
            return getSymbol(var.value)->value;
        } break;
        case NUM: {
            Token var;
            var = eat(NUM);
            // printf("FACTOR: %d\n", atoi(var.value));
            return atoi(var.value);
        }
            // default: error();
    }
    if (tokArr[tokenIndex].type == LEFTPAR) {
        int op = parseExpression();
        eat(RIGHTPAR);
        return op;
    }
}
