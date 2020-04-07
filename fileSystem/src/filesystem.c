#include "../hdrs/API.h"
err pwd(){  
    char buff[1024];
    getcwd(buff,sizeof(buff));
    Return(s(buff)); 
}

void pwdError(err e,void* d){
    printf("<!>:error geting present working directory from env var\n");
    PANIC;
}

void openFileError(err e,void* d){
    printf("<!>:can't open openFileError\n");
    PANIC;
}



void readingFileError(err e,void* d){
    printf("can't read the whole file\n");
    PANIC;
}
err readFile(str path){
    FILE* d = fopen(path.str,"rb");
    if( d == NULL) Error(openFileError);
    fseek(d,0,SEEK_END);
    size_t fileSize = ftell(d);
    char* buff = (char*)malloc(fileSize);
    rewind(d);
    size_t result = fread(buff,1,fileSize,d);
    if (result != fileSize) Error(readingFileError);
    Return(strn(buff,fileSize));
}
err makeDir(str path){

    if ( !system(NULL) ){
        Error(noCommandInterpreter);
    }
    str command = strAdd(2,s(" mkdir "),path);
    system(command.str);
    strDrop(command);
    Return(NULL);
}

void noCommandInterpreter(err e, void* d){
    printf("there is no intepreter found...\n\
    we require using system()\n");
    PANIC;
}


