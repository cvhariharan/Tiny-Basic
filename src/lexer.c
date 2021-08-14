#include "lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"

Token *getTokens(char *input) {
    Token *tokHead = NULL; //malloc(sizeof(Token));
    Token **tokPtr;// a var help to faster access
    Token * tmp;
    
    tokPtr= &tokHead; 
    
    char *token = malloc(sizeof(char) * MAX_TOK_LEN);
    int i;
    int slen = 0;
    //int tokenCount = 0;
    int retType;


    //If set, treat the input as code else it could be comment or string literal
    int isCode = 1;

    //Try to match tokens character by character
    for (i = 0; i < strlen(input); i++) {
        //Skip if whitespace
        if (isspace(input[i]) && isCode) {
            continue;
        }

        //If the first character of the token is alphabet or underscore
        if ((isalpha(input[i]) || isunderscore(input[i])) && isCode) {
            token[slen] = input[i];
            slen++;
            while (i < strlen(input)) {
                if (isalnum(input[i + 1]) || isunderscore(input[i + 1])) {
                    token[slen] = input[i + 1];
                    slen++;
                } else {
                    token[slen] = '\0';
                    slen = 0;
                    
                    retType = getType(token);
                    tmp = (Token *) malloc(sizeof(Token));
                    tmp->next = NULL;
                    
                    if (retType == -1) {
                        tmp->type = ID;
                    } else {
                        tmp->type = retType;
                    }
                    tmp->value = malloc(sizeof(char) * (strlen(token) + 1));
                    strcpy(tmp->value, token);
                    *tokPtr = tmp;
                    tokPtr = &(*tokPtr)->next;
                    
                    printf(" ");
                    strcpy(token, "");
                    break;
                }
                i++;
            }
        }

        //If the first character is a num, it must be a label
        if (isdigit(input[i]) && isCode) {
            token[slen] = input[i];
            slen++;
            while (i < strlen(input)) {
                if (isdigit(input[i + 1])) {
                    token[slen] = input[i + 1];
                    slen++;
                } else {
                    token[slen] = '\0';
                    slen = 0;
                    
                    tmp = (Token *) malloc(sizeof(Token));
                    tmp->next = NULL;
                    
                    tmp->type = NUM;
                    tmp->value = malloc(sizeof(char) * (strlen(token) + 1));
                    strcpy(tmp->value, token);
                    
                    *tokPtr = tmp;
                    tokPtr = &(*tokPtr)->next;
                    
                    printf(" ");
                    strcpy(token, "");
                    break;
                }
                i++;
            }
        }

        //String and character literals
        if (input[i] == '\"') {
            //To stop the characters inside quotes to be considered as tokens
            // isCode = !isCode;
            i++;
            char *literal = malloc(sizeof(char) * MAX_STRLEN);
            int literalLen = 0;
            while(input[i] != '\"') {
                literal[literalLen++] = input[i];
                i++;
            }
            
            tmp = (Token *) malloc(sizeof(Token));
            tmp->next = NULL;
            
            literal[literalLen] = '\0';
            tmp->type = S_LITERAL;
            tmp->value = malloc(sizeof(char) * (strlen(literal) + 1));
            strcpy(tmp->value, literal);
            
            *tokPtr = tmp;
            tokPtr = &(*tokPtr)->next;
        }
        // if (!isCode) {
        //     if (input[i] != '\\') {
        //         //Escape character
        //         literal[literalLen++] = input[i];
        //         literal[literalLen] = '\0';
        //     } else {
        //         if (input[i + 1] == '\'' || input[i + 1] == '\"') {
        //             // To handle escaped quotes, set isCode to true so that when the quote is encountered, it can be treated is first quote
        //             isCode = !isCode;
        //             continue;
        //         } else {
        //             // For handling \n, \t type of characters.
        //             i++;
        //         }
        //     }
        // }

        //If the first character is a special character
        if ((ispunct(input[i]) && (input[i] != '\'' && input[i] != '\"')) && isCode) {
            token[slen] = input[i];
            slen++;
            while (i < strlen(input)) {
                if ((ispunct(input[i + 1]) && (input[i + 1] != '\'' && input[i + 1] != '\"')) && isCode) {
                    token[slen] = input[i + 1];
                    slen++;
                } else {
                    token[slen] = '\0';
                    slen = 0;
                    tokPtr = findLongestToken(token, tokPtr);
                    break;
                }
                i++;
            }
        }
    }
    
    tmp = (Token *) malloc(sizeof(Token));
    tmp->next = NULL;
    
    tmp->type = ENTER;
    tmp->value = "CR";
    
    *tokPtr = tmp;
    
    free(token);
    return tokHead;
}

void freeTokens(Token *tok)
{
    Token *tmp;
    
    while(tok)
    {
        tmp = tok->next;
        free(tok->value);
        free(tok);
        
        tok = tmp;
    }
}

/** 
 * Method to find the longest valid token
 * from the given longest special characters token
 * tokCur is the last Token in the list, will help us to link the new nodes
 **/
Token **findLongestToken(char *token, Token **tokPtr) {
    Token *tmp;
    char *temp = malloc(sizeof(char) * MAX_TOK_LEN);
    char *longToken = malloc(sizeof(char) * MAX_TOK_LEN);
    int i, m = 0;
    for (i = 0; i < strlen(token); i++) {
        temp[m++] = token[i];
        temp[m] = '\0';
        if (isValidToken(temp)) {
            // printf("%s ", temp);
            // printf("%d ", getType(temp));
            strcpy(longToken, temp);
        } else {
            // printf("%s ", longToken);
            // printf("%d ", getType(longToken));
            tmp = (Token *) malloc(sizeof(Token));
            
            tmp->next = NULL;
            tmp->type = getType(longToken);
            tmp->value = malloc(sizeof(char) * (strlen(longToken) + 1 ));
            strcpy(tmp->value, longToken);
            
            *tokPtr = tmp;
            tokPtr = &(*tokPtr)->next;
            
            m = 0;
            //To consider the already considered character in the invalid token
            i--;
        }
    }
    // printf("%s ", longToken);
    // printf("%d ", getType(longToken));
    if( m!=0 )
    {
        tmp = (Token *) malloc(sizeof(Token));
            
        tmp->next = NULL;
        tmp->type = getType(longToken);
        tmp->value = malloc(sizeof(char) * (strlen(longToken) + 1 ));
        strcpy(tmp->value, longToken);
        *tokPtr = tmp;
        tokPtr = &(*tokPtr)->next;
    }
    free(longToken);
    free(temp);
    
    return tokPtr;
}

int isValidToken(char *token) {
    return getType(token) > 0;
}

int isunderscore(char c) {
    //Checks if a character is underscore
    return (int)c == 95;
}

int getType(char *token) {
    // Keywords
    if (0 == strcmp(token, "PRINT"))
        return PRINT;
    else if (0 == strcmp(token, "IF"))
        return IF;
    else if (0 == strcmp(token, "THEN"))
        return THEN;
    else if (0 == strcmp(token, "GOTO"))
        return GOTO;
    else if (0 == strcmp(token, "INPUT"))
        return INPUT;
    else if (0 == strcmp(token, "LET"))
        return LET;
    else if (0 == strcmp(token, "GOSUB"))
        return GOSUB;
    else if (0 == strcmp(token, "RETURN"))
        return RETURN;
    else if (0 == strcmp(token, "CLEAR"))
        return CLEAR;
    else if (0 == strcmp(token, "LIST"))
        return LIST;
    else if (0 == strcmp(token, "RUN"))
        return RUN;
    else if (0 == strcmp(token, "END"))
        return END;
    else if (0 == strcmp(token, "REM"))
        return REM;

    // Operators
    else if (0 == strcmp(token, "+"))
        return PLUS;
    else if (0 == strcmp(token, "-"))
        return SUB;
    else if (0 == strcmp(token, "*"))
        return MUL;
    else if (0 == strcmp(token, "/"))
        return DIV;
    else if (0 == strcmp(token, "%"))
        return MOD;
    else if (0 == strcmp(token, "++"))
        return INCR;
    else if (0 == strcmp(token, "--"))
        return DECR;

    // Assignment
    else if (0 == strcmp(token, "="))
        return ASSIGN;
    else if (0 == strcmp(token, "+="))
        return PLUS_ASSIGN;
    else if (0 == strcmp(token, "-="))
        return SUB_ASSIGN;
    else if (0 == strcmp(token, "*="))
        return MUL_ASSIGN;
    else if (0 == strcmp(token, "/="))
        return DIV_ASSIGN;
    else if (0 == strcmp(token, "%="))
        return MOD_ASSIGN;

    // Relational
    else if (0 == strcmp(token, "=="))
        return EQUALTO;
    else if (0 == strcmp(token, ">"))
        return GT;
    else if (0 == strcmp(token, "<"))
        return LT;
    else if (0 == strcmp(token, "!="))
        return NOTEQUALTO;
    else if (0 == strcmp(token, ">="))
        return GEQUAL;
    else if (0 == strcmp(token, "<="))
        return LEQUAL;

    // Miscellaneous
    else if (0 == strcmp(token, ","))
        return COMMA;

    // Logical
    else if (0 == strcmp(token, "&&"))
        return AND_L;
    else if (0 == strcmp(token, "||"))
        return OR_L;
    else if (0 == strcmp(token, "!"))
        return NOT_L;

    else
        return -1;
}
