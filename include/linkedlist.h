#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stddef.h>

    typedef struct node{
        size_t index;
        void* data;
        struct node* next;
        struct node* previous;
    }node;

    typedef struct{
        unsigned int size;
        node* headnode;
        node* tailnode;
        node* tempnode;
    }linkedlist;

    void insertend(linkedlist* list,void* data);
    void clear(linkedlist* list,void(*func)(void*));
    void* get(linkedlist* list,size_t index);
    void remove_front(linkedlist* list,void(*func)(void*));
    void removeat(linkedlist* list,unsigned int index,void(*func)(void*));
    void __init__(linkedlist* list);
    void free_in(void* data);
    void cpylist(linkedlist* oldlist,linkedlist* newlist);
    void freenone(void* data);
    void remove_end(linkedlist* list,void(*func)(void*));
    void set_index(linkedlist* list);


#endif
