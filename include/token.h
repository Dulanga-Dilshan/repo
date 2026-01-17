#ifndef TOKEN_H
#define TOKEN_H
#include "linkedlist.h"

    typedef enum{
        INTEGER,
        CAPITAL,
        SIMPLE,
        CONSTANT
    }t_type;

    typedef struct{
        int start;
        int end;
    }t_range;

    typedef struct{
        int data;
        t_type token_type;
        t_range range;
        int iscarry;
        int padding;
        int increment;
    }token;

    void tokenize(char* str,linkedlist* list);
    void setrange(char* rangestr,linkedlist* list);
    int toint(char* str,size_t i,size_t end);



#endif

