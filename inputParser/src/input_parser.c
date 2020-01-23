#include "../hdrs/API.h"

void inputParser__runBuild(){
    cpmCompiller__build();
}

void ip__run_init(str module_name){
    str dir = fileSystem__pwd();
    dir = aphinString__append(3,dir,aphinString__mkstr("/"),module_name);
    fileSystem__mkdir(dir);

    str subdir = aphinString__mkstr(dir.str);
    subdir = aphinString__append(2,subdir,aphinString__mkstr("/test"));
    fileSystem__mkdir(subdir);
    subdir = aphinString__mkstr(dir.str);
    subdir = aphinString__append(2,subdir,aphinString__mkstr("/doc"));
    fileSystem__mkdir(subdir);
    subdir = aphinString__mkstr(dir.str);
    subdir = aphinString__append(2,subdir,aphinString__mkstr("/hdrs"));
    fileSystem__mkdir(subdir);
    subdir = aphinString__mkstr(dir.str);
    subdir = aphinString__append(2,subdir,aphinString__mkstr("/build"));
    fileSystem__mkdir(subdir);
    subdir = aphinString__mkstr(dir.str);
    subdir = aphinString__append(2,subdir,aphinString__mkstr("/src"));
    fileSystem__mkdir(subdir);
    subdir = aphinString__mkstr(dir.str);
    subdir = aphinString__append(2,subdir,aphinString__mkstr("/res"));
    fileSystem__mkdir(subdir);
    
}

void parser(int argc,char* argv[]){
    if( argc > 2 ){
        char* command = argv[1];
        if ( aphinString__cmpstr(aphinString__mkstr(command),aphinString__mkstr("init")) == 0 ){
            ip__run_init(aphinString__mkstr(argv[2]));
        } else if ( aphinString__cmpstr(aphinString__mkstr(command),aphinString__mkstr("build")) == 0 ){
            inputParser__runBuild();
        }
    }
}

void main(int argc,char* argv[]){
    parser(argc,argv);
    return;
}
