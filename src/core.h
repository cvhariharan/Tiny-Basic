#ifndef _CORE_H_
#define _CORE_H_

//here defining some structure for the program
struct Stack{
    struct Stack *prev;
    int value;
};

typedef struct Stack Stack;

#endif // _CORE_H_
