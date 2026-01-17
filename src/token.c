#include "token.h"
#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

void tokenize(char* str,linkedlist* list){
    size_t i=0;
    token* temp=NULL;

    while(str[i]){
        temp=(token*)malloc(sizeof(token));
        if(!temp){
            fprintf(stderr,"malloc failed\n");
            exit(EXIT_FAILURE);
        }
        if(str[i]=='?'){
            temp->data=0;
            temp->iscarry=false;
            temp->range.start=0;
            temp->range.end=0;
            temp->token_type=INTEGER;
            temp->padding=0;
            temp->increment=1;
            insertend(list,temp);
            i++;
            continue;
        }else if(str[i]==']'){
            temp->token_type=CAPITAL;
            temp->data='A';
            temp->iscarry=false;
            temp->range.start='A';
            temp->range.end='Z';
            temp->padding=0;
            temp->increment=1;
            insertend(list,temp);
            i++;
            continue;
        }else if(str[i]=='['){
            temp->token_type=SIMPLE;
            temp->data='a';
            temp->iscarry=false;
            temp->range.start='a';
            temp->range.end='z';
            temp->padding=0;
            temp->increment=1;
            insertend(list,temp);
            i++;
            continue;

        }else if(strchr("\\/*'\"|",str[i])!= NULL){
            printf("invalid character - '%c' \n",str[i]);
            exit(EXIT_FAILURE);
        }else{
            temp->data=str[i];
            temp->iscarry=false;
            temp->token_type=CONSTANT;
            temp->range.start=0;
            temp->range.end=0;
            temp->padding=0;
            temp->increment=0;
            insertend(list,temp);
            i++;
        }
    }
}

void _setdata(linkedlist* list){
    token* tkn;
    for(int i=1;i<=list->size;i++){
        tkn=(token*)get(list,i);
        if(tkn->token_type!=CONSTANT){
            tkn->data=tkn->range.start;
        }
    }
}

void _checkrange(linkedlist* list){
    token* tkn;
    for(int i=1;i<=list->size;i++){
        tkn=(token*)get(list,i);
        if(tkn->token_type==INTEGER){
            if(tkn->range.start>tkn->range.end){
                int temp=tkn->range.start;
                tkn->range.start=tkn->range.end;
                tkn->range.end=temp;
            }else if(tkn->range.start==tkn->range.end){
                printf("unvalid range{%d,%d}for type ([)\n",tkn->range.start,tkn->range.end);
                exit(1);
            }
        }else if(tkn->token_type==SIMPLE){
            if((tkn->range.start>='a' && tkn->range.start<='z') &&((tkn->range.end>='a' && tkn->range.end<='z'))){
                if(tkn->range.start>tkn->range.end){
                    int temp=tkn->range.start;
                    tkn->range.start=tkn->range.end;
                    tkn->range.end=temp;
                }
            }else{
                printf("unvalid range{%c,%c}for type (])\n",tkn->range.start,tkn->range.end);
                exit(1);
            }
        }else if(tkn->token_type==CAPITAL){
            if((tkn->range.start>='A' && tkn->range.start<='Z') &&((tkn->range.end>='A' && tkn->range.end<='Z'))){
                if(tkn->range.start>tkn->range.end){
                    int temp=tkn->range.start;
                    tkn->range.start=tkn->range.end;
                    tkn->range.end=temp;
                }
            }else{
                printf("unvalid range{%c,%c}for type ([)\n",tkn->range.start,tkn->range.end);
                exit(1);
            }
        }
    }
}

int toint(char* str,size_t i,size_t end){
    int value=0;
    for(;i<end;i++){
        value*=10;
        value+=str[i]-'0';
    }
    return value;
}

void getrange(char* str,t_type type,size_t start_index,size_t end_index,int* start,int* end){
    for(int i=start_index;i<end_index;i++){       
        if(str[i]==','){
            if(type==INTEGER){
                *start=toint(str,start_index,i);
                *end=toint(str,i+1,end_index);
            }else{
                *start=(int)str[start_index];
                *end=(int)str[i+1];
            }
            return;
        }
    }
}

void setrange(char* rangestr,linkedlist* list){
    int t_index=1;
    int str_index=0;
    int i=0;
    token* tkn;
    int tkntype=0;
    
    while(rangestr[str_index]){
        if((rangestr[str_index]=='?') ||  (rangestr[str_index]==']') || (rangestr[str_index]=='[')){
            tkn=(token*)get(list,t_index);
            if(tkn==NULL){
                printf("missing range def for (%c) prameter in -p\n",rangestr[str_index]);
                exit(EXIT_FAILURE);
            }
            if(rangestr[str_index]=='?'){
                tkntype=INTEGER;
            }else if(rangestr[str_index]==']'){
                tkntype=CAPITAL;
            }else{
                tkntype=SIMPLE;
            }
            while(tkn->token_type!=tkntype){
                t_index++;
                tkn=get(list,t_index);
                if(tkn==NULL){
                    printf("invalid usage in -r \n");
                    exit(EXIT_FAILURE);
                }    
            }
            str_index++;
            i=str_index;
            if(rangestr[str_index]!='{'){
                printf("missing '{' after range parameter in -r arg\n");
                exit(EXIT_FAILURE);
            }
            int ccount=0;
            int cindex=0;
            while(rangestr[str_index]!='}'){
                if(rangestr[str_index]==','){
                    ccount++;
                    cindex=str_index;
                }
                str_index++;
            }
            if(ccount>1){
                getrange(rangestr,tkntype,i+1,cindex,&tkn->range.start,&tkn->range.end);
                tkn->increment=toint(rangestr,cindex+1,str_index);
            }else{
                getrange(rangestr,tkntype,i+1,str_index,&tkn->range.start,&tkn->range.end);
            }
            t_index++;
        }
        str_index++;   
    }
    _checkrange(list);
    _setdata(list);
}