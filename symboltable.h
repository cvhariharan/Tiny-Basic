typedef struct Symbols{
    char *variable;
    int type;
    int value;
} Symbol;

Symbol *getSymbol(char *);
void insertVariable(char *, int, int);