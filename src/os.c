#include "os.h"

void create_dir(char* path,linkedlist *dirlist){
    int len=dirlist->size;
    char fpath[MAX_FPATH];
    fpath[0]='\0';
    int count=0;
    f_name* dir;
    for(int i=1;i<=len;i++){
        dir=(f_name*)get(dirlist,i);
        strappend(fpath,path);
        strappend(fpath,"/");
        strappend(fpath,dir->str);
        if(_mkdir(fpath)!=0){
            count++;
        }
        fpath[0]='\0';
    }
    if(count>0){
        printf("created:%d skipped:%d\n",len-count,count);
    }
}

void addfile(char* fname,linkedlist* list){
    if (!fname || strlen(fname) == 0){
        fprintf(stderr, "Invalid file name\n");
        return;
    }

    file_name* file=(file_name*)malloc(sizeof(file_name));
    if(!file){
        fprintf(stderr,"malloc faild!\n");
        exit(EXIT_FAILURE);
    }
    
    char* last_dot = strrchr(fname, '.');
    if (last_dot != NULL) {
        size_t name_len = last_dot - fname;
        size_t ext_len = strlen(last_dot + 1);
        
        if (name_len > MAX_FNAME - 1) name_len = MAX_FNAME - 1;
        if (ext_len > MAX_EXTENTION - 1) ext_len = MAX_EXTENTION - 1;
        
        memcpy(file->name, fname, name_len);
        file->name[name_len] = '\0';
        memcpy(file->extention, last_dot + 1, ext_len);
        file->extention[ext_len] = '\0';
    } else {
        size_t name_len = strlen(fname);
        if (name_len > MAX_FNAME - 1) name_len = MAX_FNAME - 1;
        memcpy(file->name, fname, name_len);
        file->name[name_len] = '\0';
        file->extention[0] = '\0';
    }
    
    insertend(list,file);
}

void getfilenames(const char* parth,linkedlist* fnamelist){
    WIN32_FIND_DATA fdata;
    HANDLE handle;
    char fname[MAX_FNAME];
    size_t lenth=strlen(parth);
    char fparth[MAX_FPATH];
    fparth[0]='\0';
    if(lenth==0){
        strappend(fparth,"*.*");
    }else{
        strappend(fparth,(char*)parth);
        strappend(fparth,"\\*.*");
    }
    
    handle=FindFirstFile(fparth,&fdata);

    do{
        if(handle==INVALID_HANDLE_VALUE){
            printf("faill\n");
            exit(EXIT_FAILURE);
        }

        if(fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            continue;
        }

        if(strcmp(fdata.cFileName,".")==0 || strcmp(fdata.cFileName,"..")==0){
            continue;
        }

        memcpy(fname,fdata.cFileName,MAX_FNAME);
        if(strlen(fdata.cFileName)>=MAX_FNAME){
            fname[MAX_FNAME-1]='\0';
        }else{
            fname[strlen(fdata.cFileName)]='\0';
        }
        addfile(fname,fnamelist);
    }while(FindNextFile(handle,&fdata));

    FindClose(handle);
}


void renamefiles(linkedlist* oldname,linkedlist* newname,char* path){
    char new[MAX_FPATH+MAX_FNAME+MAX_EXTENTION];
    char old[MAX_FPATH+MAX_FNAME+MAX_EXTENTION];

    file_name *newfname,*oldfname;

    for(int i=1;i<=oldname->size;i++){
        new[0]='\0';
        old[0]='\0';
        oldfname=(file_name*)get(oldname,i);
        newfname=(file_name*)get(newname,i);

        strappend(new,path);
        strappend(old,path);
        strappend(new,"\\\0");
        strappend(old,"\\\0");
        strappend(new,newfname->name);
        strappend(old,oldfname->name);
        if(strlen(oldfname->extention)>0){
            strappend(new,".");
            strappend(old,".");
            strappend(new,newfname->extention);
            strappend(old,oldfname->extention);
        }
        rename(old,new);
    }
}

void getparth(char* path){
    char p[MAX_FPATH];
    if(_getcwd(p,MAX_FPATH)==NULL){
        printf("can't specify the path!");
        exit(EXIT_FAILURE);
    }

    size_t j=0,len=strlen(p);
    
    for(size_t i=0;i<len;i++){
        if(p[i]=='\\'){
            path[j++]='\\';
        }else{
            path[j++]=p[i];
        }
    }
    path[j]='\0';
}