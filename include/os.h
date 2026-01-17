#ifndef OS_H
#define OS_H

#include "linkedlist.h"
#include<direct.h>
#include<stdio.h>
#include "fname.h"
#include <windows.h>

#define MAX_FPATH 512

    void create_dir(char* path,linkedlist *dirlist);
    void getfilenames(const char* parth,linkedlist* fnamelist);
    void addfile(char* fname,linkedlist* list);
    void renamefiles(linkedlist* oldname,linkedlist* newname,char* path);
    void getparth(char* path);

#endif
