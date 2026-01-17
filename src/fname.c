#include<stdlib.h>
#include "fname.h"
#include<stdio.h>
#include<string.h>

int intlen(int num){
    int lenth=0;
    while(num>0){
        num /= 10;
        lenth++;
    }
    return lenth;
}

long int combinations(linkedlist* list){
    int len=list->size;
    token* tkn;
    long int tot=1;
    for(int i=1;i<=len;i++){
        tkn=get(list,i);
        if(tkn->token_type!=CONSTANT){
            tot*=(tkn->range.end - tkn->range.start +1)/tkn->increment;
        }
    }
    return tot;
}

int power(int num,int p){
    int tot=1;
    for(int i=1;i<=p;i++){
        tot*=num;
    }
    return tot;
}

void tostr(char* buff,int num){
    int len=intlen(num);
    int pow=power(10,len-1);
    int ans=0;
    int i=0;

    for(;i<len-1;i++){
        ans=num/pow;
        buff[i]=(char)(ans+'0');
        ans*=pow;
        num-=ans;
        pow/=10;
    }
    ans=num/pow;
    buff[i]=(char)(ans+'0');
    buff[i+1]='\0';
}

int calc_fnlen(linkedlist* list){
    token* t;
    size_t lenth=0;
    for(int i=1;i<=list->size;i++){
        t=get(list,i);
        if(t->token_type==INTEGER){
            lenth+=t->padding+intlen(t->data);
        }else{
            lenth++;
        }
        
    }
    return lenth;
}

void calc_padding(linkedlist* list){
    token* t;
    for(int i=1;i<=list->size;i++){
        t=get(list,i);
        if(t->token_type==INTEGER){
            t->padding=intlen(t->range.end);
        }
    }
}

void strappend(char* destination,char* source){
    int start=strlen(destination);
    int size=strlen(source);
    memcpy((char*)destination+start,source,size);
    destination[start+size]='\0';
}

void getdata(token* tkn,char* buff,bool padding){
    int i=0;
    if(tkn->token_type==INTEGER){       
        char data[intlen(tkn->data)+tkn->padding+1];
        for(;i<tkn->padding-(intlen(tkn->data)) && padding==true;i++){
            data[i]='0';
        }
        tostr(data+i,tkn->data);
        strappend(buff,data);
        return;
    }
    char data[2];
    data[0]=(char)tkn->data;
    data[1]='\0';
    strappend(buff,data);
}

void updatetokens(linkedlist* list,int* carry){
    token* tkn;
    int lenth=list->size;
    for(int i=lenth;i>0;i--){
        tkn=get(list,i);
        if(tkn->token_type!=CONSTANT){
            if(tkn->data+tkn->increment<=tkn->range.end){
                tkn->data+=tkn->increment;
                *carry=0;
                break;
            }else{
                *carry=(tkn->data+tkn->increment)-tkn->range.end;
                tkn->data=tkn->range.start;
            }
        }
    }
}


void fname_gen(linkedlist* tokenlist,linkedlist* fnamelist,bool z_padding,size_t limit){
    calc_padding(tokenlist);
    int fnamelen=calc_fnlen(tokenlist);
    char buff[fnamelen+1];
    buff[0]='\0';
    token* tkn;
    f_name* fname;
    int headtkncarry=0;
    size_t count=1;
    while(headtkncarry==0 && count<=limit){     
        for(int i=1;i<=tokenlist->size;i++){
            tkn=get(tokenlist,i);
            getdata(tkn,buff,z_padding);  
        }
        fname=(f_name*)malloc(sizeof(f_name));
        fname->id=count++;
        memcpy(fname->str,buff,MAX_FNAME);
        insertend(fnamelist,fname);
        updatetokens(tokenlist,&headtkncarry);
        buff[0]='\0';
    }
}

void clearbuff(char* buff,size_t len){
    for(int i=0;i<len;i++){
        buff[i]='\0';
    }
}

void file_rename(linkedlist* filenames,linkedlist* tokenlist,size_t zpadding){
    linkedlist newnames;
    __init__(&newnames);
    fname_gen(tokenlist,&newnames,zpadding,filenames->size); 

    

    file_name* fname;
    f_name* newname;
    for(int i=1;i<=newnames.size;i++){
        fname=(file_name*)get(filenames,i);
        newname=(f_name*)get(&newnames,i);

        clearbuff(fname->name,MAX_FNAME);
        memcpy(fname->name,newname->str,strlen(newname->str)*sizeof(char));
    }
}

void display_fnames(linkedlist* fname_list){
    f_name* fname;
    for(size_t i=1;i<=fname_list->size;i++){
        fname=(f_name*)get(fname_list,i);
        printf("ID: %d - Folder Name: %s\n",fname->id,fname->str);
    }
}

void remove_fnames(linkedlist* list,char* removings){
    size_t len=strlen(removings),j=0,count=0,index=0;
    char buff[10];
    buff[0]='\0';
    for(size_t i=0;i<len;i++){
        if(removings[i]==',' || i==len-1){
            memcpy(buff,(char*)(removings+j),i-j);
            buff[i-j]='\0';
            index=toint(buff,0,strlen(buff));
            removeat(list,index-count,free_in);
            count++;
            j=i+1;
        }

    }
}

int count_com(char* str,size_t len){
    size_t count=0;
    for(size_t i=0;i<len;i++){
        if(str[i]==','){
            count+=1;
        }
    }
    return count;
}

void print_files(linkedlist* list){
    file_name* f;
    printf("%d size\n",list->size);

    for(size_t i=1;i<=list->size;i++){
        f=(file_name*)get(list,i);

        printf("%s.%s\n",f->name,f->extention);
    }
}

void filter_exes(char* exestr,linkedlist* fnamelist,linkedlist* oldfnamelist){
    size_t indexs[fnamelist->size];
    size_t len=strlen(exestr),index_count=0,j=0;
    size_t c_count=count_com(exestr,len);
    linkedlist* exe_list=(linkedlist*)malloc(sizeof(linkedlist));
    file_name* f;
    bool remove=true;
    __init__(exe_list);
    if(exe_list==NULL){
        printf("malloc faild!\n");
        exit(EXIT_FAILURE);
    }
    char *buff,*data;
    if(c_count>0){
        for(size_t i=0;i<len;i++){
            if(exestr[i]==',' || i==len-1){
                buff=(char*)malloc(sizeof(char)*MAX_EXTENTION);
                if(i==len-1) i++;
                memcpy(buff,(char*)(exestr+(j*sizeof(char))),i-j);
                buff[i-j]='\0';
                insertend(exe_list,buff);
                j=++i;
            }
        }
    }else{
        buff=(char*)malloc(sizeof(char)*MAX_EXTENTION);
        memcpy(buff,exestr,len);
        buff[len+1]='\0';
        insertend(exe_list,buff);
    }

    for(size_t i=1;i<=fnamelist->size;i++){
        f=(file_name*)get(fnamelist,i);
        for(size_t j=1;j<=exe_list->size;j++){
            data=(char*)get(exe_list,j);
            if(strcmp(f->extention,data)==0){ 
                indexs[index_count++]=i;
            }
        }
    }

    for(int i=fnamelist->size;i>0;i--){
        for(j=0;j<index_count;j++){
            if(i==indexs[j]){
                remove=false;
            }
        }
        if(remove){
            removeat(fnamelist,i,free_in);
            removeat(oldfnamelist,i,free_in);
        }
        remove=true;
    }
    clear(exe_list,free_in);
    free(exe_list);
}