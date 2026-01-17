#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "linkedlist.h"
#include <stdbool.h>

void __init__(linkedlist* list){
    list->headnode=NULL;
    list->size=0;
    list->tailnode=NULL;
    list->tempnode=NULL;
}

void insertend(linkedlist* list,void* data){
    if(!list->headnode){
        list->headnode=(node*)malloc(sizeof(node));
        list->headnode->data=data;
        list->headnode->next=NULL;
        list->headnode->previous=NULL;
        list->tailnode=list->headnode;
        list->size=1;
        list->headnode->index=1;
        return;
    }

    list->tailnode->next=(node*)malloc(sizeof(node));
    list->tailnode->next->data=data;
    list->tailnode->next->index=list->tailnode->index+1;
    list->tailnode->next->previous=list->tailnode;
    list->tailnode=list->tailnode->next;
    list->tailnode->next=NULL;
    list->size++;
}

void free_in(void* data){
    free(data);
}

void clear(linkedlist* list,void(*func)(void*)){
    list->tempnode=list->headnode;
    node* tmp=list->headnode;
    while(list->tempnode){
        func(tmp->data);
        list->tempnode=list->tempnode->next;
        free(tmp);
        tmp=list->tempnode;
    }
    list->headnode=NULL;
    list->tailnode=NULL;
    list->tempnode=NULL;
    list->size=0;
}

void* get(linkedlist* list,size_t index){
    if(index<1 || index>list->size){
        puts("invalid index!");
        exit(EXIT_FAILURE);
    }
    if(list->tempnode==NULL){
        list->tempnode=list->headnode;
    }
    size_t current_index=list->tempnode->index;

    if(current_index<1 || current_index > list->size){
        current_index=1;
    }
      
    if(current_index<=index){
        for(int i=current_index;i<index;i++){
            list->tempnode=list->tempnode->next;
        }
    }else{
        for(int i=current_index;i>index;i--){
            list->tempnode=list->tempnode->previous;
        }
    }
    return list->tempnode->data;
}

void remove_end(linkedlist* list,void(*func)(void*)){
    if(list->headnode==NULL) return;
    func(list->tailnode->data);
    list->tailnode=list->tailnode->previous;
    free(list->tailnode->next);
    list->tailnode->next=NULL;
    list->size--;
    set_index(list);
}

void remove_front(linkedlist* list,void(*func)(void*)){
    if(list->headnode==NULL) return;
    func(list->headnode->data);
    list->headnode=list->headnode->next;
    free(list->headnode->previous);
    list->headnode->previous=NULL;
    list->size--;
    set_index(list);
}

void removeat(linkedlist* list,unsigned int index,void(*func)(void*)){
    if(list->headnode==NULL || index>list->size || index<=0) return;
    if(index==list->size){
        remove_end(list,func);
    }else if(index==1){
        remove_front(list,func);
    }else{
        list->tempnode=list->headnode->next;
        for(int i=2;i<=list->size-1;i++){
            if(index==i){
                list->tempnode->next->previous=list->tempnode->previous;
                list->tempnode->previous->next=list->tempnode->next;
                func(list->tempnode->data);
                free(list->tempnode);
                list->tempnode=NULL;
                break;
            }
            list->tempnode=list->tempnode->next;
        }
        list->size--;
    }
    set_index(list);
}

void cpylist(linkedlist* oldlist,linkedlist* newlist){
    if(oldlist==NULL || newlist==NULL){
        return;
    }
    void* data;
    for(int i=1;i<=oldlist->size;i++){
        data=get(oldlist,i);
        insertend(newlist,data);
    }
}

void freenone(void* data){
}

void set_index(linkedlist* list){
    list->tempnode=list->headnode;
    for(int i=1;i<=list->size;i++){
        list->tempnode->index=i;
        list->tempnode=list->tempnode->next;
    }
}