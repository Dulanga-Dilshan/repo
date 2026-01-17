#ifndef FNAME_H
#define FNAME_H

#include<stddef.h>
#include<stdbool.h>
#include "linkedlist.h"
#include "token.h"

#define MAX_FNAME 50
#define MAX_EXTENTION 20

#define VIDEO_FORMATS "mp4,mkv,avi,mov,wmv,flv,webm,3gp,m4v,ts,vob,mpg,mpeg"
#define AUDIO_FORMATS "mp3,wav,flac,acc,ogg,m4a,wma,alac,aiff,opus"
#define IMAGE_FORMATS "jpg,jpeg,png,gif,bmp,webp,tiff,tif,heic,svg"
#define DOCUMENT_FORMATS "pdf,doc,docx,txt,odt,md,xls,xlsx,ppt,pptx"

    typedef struct{
        char str[MAX_FNAME];
        size_t id;
    }f_name;

    typedef struct{
        char name[MAX_FNAME];
        char extention[MAX_EXTENTION];
    }file_name;

    void tostr(char* buff,int num);
    void fname_gen(linkedlist* tokenlist,linkedlist* fnamelist,bool z_padding,size_t limit);
    void strappend(char* destination,char* source);
    void getdata(token* tkn,char* buff,bool padding);
    int power(int num,int p);
    long int combinations(linkedlist* list);
    void file_rename(linkedlist* filenames,linkedlist* tokenlist,size_t zpadding);
    void display_fnames(linkedlist* fname_list);
    void remove_fnames(linkedlist* list,char* removings);
    void filter_exes(char* exestr,linkedlist* fnamelist,linkedlist* oldfnamelist);
    void print_files(linkedlist* list);

#endif