#include "../hdrs/remoteControll.h"

str get_url(str modName){
    str gitList = strAdd(2,s(getenv("HOME")),s("/.cpmModules/modList"));
    FILE* desk = fopen(gitList.str,"r");
    if (desk == NULL)   printf("error opening\n");
    fseek(desk,0,SEEK_END);
    size_t fileSize = ftell(desk);
    char* buff = (char*)malloc(fileSize);
    rewind(desk);
    size_t result = fread(buff,1,fileSize,desk);
    if (result != fileSize)     printf("error reading\n");
    char b[128];
    int j = 0;
    for(size_t i = 0; i < fileSize; ++i){
        b[j] = buff[i];
        if ( b[j] == ' ' ){
            b[j] = '\0';
            if ( strCmp(s(modName.str),s(b)) == 0 ){
                for( j = 0; buff[i] != '\n'; ++j,++i ){
                    b[j] = buff[i];
                }
                b[j] = '\0';
                return s(b);
            }
            j = -1;
        } 
        if ( buff[i] == '\n' ) j = -1;
        ++j;
    }
    str err = {0,NULL};
    return err;
}

void remoteControll__clone(str modName){
    str git_clone = strAdd(6,s("git clone "),get_url(modName),
            s(" "),s(getenv("HOME")),s("/.cpmModules/"),s(modName.str));
    system(git_clone.str);
}


