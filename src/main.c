#include<stdio.h>
#include "linkedlist.h"
#include "token.h"
#include "fname.h"
#include<string.h>
#include<stdlib.h>
#include<stddef.h>
#include "os.h"

void help();
void invalid_useage();
void check_flags(char* flag);


int main(int argc,char** argv){
    if(argc == 1) {
        invalid_useage();
    }

    size_t pattern_index=1,range_index=1,possible_combinations=0,MAX_POSSIBLE_COMBINATION_LIMIT=20000,exe_index;
    bool zpadding=false,rename=false,edit=false,extentions=false,img=false,vid=false,doc=false,audio=false;

    for(int i=1; i<argc; i++) {
        if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help();
        }else if(strcmp(argv[i],"-p")==0){
            pattern_index=++i;
        }else if(strcmp(argv[i],"-r")==0){
            range_index=++i;
        }else if(strcmp(argv[i],"-re")==0){
            rename=true;
        }else if(strcmp(argv[i],"-z")==0){
            zpadding=true;
        }else if(strcmp(argv[i],"-e")==0){
            edit=true;
        }else if(strcmp(argv[i],"-x")==0){
            extentions=true;
            exe_index=++i;
        }else if(strcmp(argv[i],"-img")==0 || strcmp(argv[i],"-images")==0){
            img=true;
        }else if(strcmp(argv[i],"-vid")==0 || strcmp(argv[i],"-video")==0){
            vid=true;
        }else if(strcmp(argv[i],"-doc")==0 || strcmp(argv[i],"-documents")==0){
            doc=true;
        }else if(strcmp(argv[i],"-audio")==0){
            audio=true;
        }else{
            check_flags(argv[i]);
        }
    }

    
    char path[MAX_FPATH];
    getparth(path);
    linkedlist token_list,fname,filename,refilenames;
    __init__(&token_list);
    __init__(&fname);
    __init__(&filename);
    __init__(&refilenames);

    if(pattern_index<1 || range_index<1){
        invalid_useage();
    }

    tokenize(argv[pattern_index],&token_list);
    setrange(argv[range_index],&token_list);

    possible_combinations=combinations(&token_list);
    if((possible_combinations>=MAX_POSSIBLE_COMBINATION_LIMIT) && (rename==false)){
        printf("%d possible folder combinations!\ncreate(y or n): ",possible_combinations);
        char ans;
        scanf("%c",&ans);
        if(!(ans=='y' || ans=='Y')){
            exit(0);
        }
    }

    fname_gen(&token_list,&fname,zpadding,possible_combinations);

    if(rename==true){
        getfilenames(path,&filename);
        getfilenames(path,&refilenames);
        
        if(extentions){
            filter_exes(argv[exe_index],&refilenames,&filename);
        }else if(img){
            filter_exes(IMAGE_FORMATS,&refilenames,&filename);
        }else if(doc){
            filter_exes(DOCUMENT_FORMATS,&refilenames,&filename);
        }else if(vid){
            filter_exes(VIDEO_FORMATS,&refilenames,&filename);
        }else if(audio){
            filter_exes(AUDIO_FORMATS,&refilenames,&filename);
        }
        file_rename(&refilenames,&token_list,zpadding);
        renamefiles(&filename,&refilenames,path);
    }else{
        if(edit==true){
            display_fnames(&fname);
            char removings[100];
            printf("IDs of the folder to remove(separeted by ,): ");
            fgets(removings,sizeof(removings),stdin);
            remove_fnames(&fname,removings);

        }
        create_dir(path,&fname);
    }

    clear(&token_list,free_in);
    clear(&fname,free_in);
    clear(&filename,free_in);
    clear(&refilenames,freenone);

    return 0;
}

void invalid_useage(){
    printf("Usage: repo -p (pattern) -r (range) (options)\n");
    printf("Use -h or --help for more information.\n");
    exit(1);
}

void help() {
    const char* help_message =
        "Usage:\n"
        "  repo (options) -p (pattern) -r (range)\n\n"
        "Description:\n"
        "  Folder/File pattern generator and renamer.\n\n"
        "Pattern Syntax:\n"
        "  ? - digit (0-9)\n"
        "  [ - lowercase letter (a-z)\n"
        "  ] - uppercase letter (A-Z)\n\n"
        "Range Format:\n"
        "  Each pattern placeholder must have a matching range.\n"
        "  Format: (char_type){start,end[,increment]}\n"
        "    - char_type: one of ?, <, or > (matching a placeholder)\n"
        "    - start/end: range values (e.g., 0-9, a-z, A-Z)\n"
        "    - increment: optional step value (default is 1)\n"
        "  Example: ?{0,9}, <{a,f,2}, >{A,D}\n\n"
        "Options:\n"
        "  -p (pattern)    Set the pattern for folder/file names\n"
        "  -r (range)      Define the range for each variable in the pattern\n"
        "  -z              Enable zero padding for digits\n"
        "  -re             Rename existing files using the pattern\n"
        "  -e              Edit names before creation\n"
        "  -h, --help      Show this help message\n\n"
        "-x {ext...}       Only include files with listed extensions\n"
        "-image,-img      Include only image files\n"
        "-video,-vid      Include only video files\n"
        "-audio          Include only audio files\n"
        "-document,-doc   Include only documents\n"
        "Examples:\n"
        "  repo -p file_? -r ?{0,4}\n"
        "    file_0.txt, file_1.txt, ..., file_4.txt\n\n"
        "  repo -z -p file_? -r ?{0,12}\n"
        "    file_00.txt, file_01.txt, ..., file_12.txt\n\n"
        "  repo -re -p img_<> -r <{a,b} >{A,B}\n"
        "    img_aA.jpg, img_aB.jpg, ..., img_bB.jpg\n";

    printf("%s", help_message);
    exit(0);
}

void check_flags(char* flag){
    char err[50]="Invalid flag [";
    strappend(err,flag);
    strappend(err,"] did you mean [");
    if(flag[0]=='-'){
        switch(flag[1]){
            case 'i':
                strappend(err,"-img] or [-image]\n");
                break;
            case 'v':
                strappend(err,"-vid] or [-video]\n");
                break;
            case 'a':
                strappend(err,"-audio]\n");
                break;
            case 'd':
                strappend(err,"-doc] or [-document]\n");
                break;

            default:
                err[0]='\0';
                strappend(err,"Invalid flag [");
                strappend(err,flag);
                strappend(err,"]\n");
        }
        printf("%s\n",err);
        invalid_useage();
    }
}


