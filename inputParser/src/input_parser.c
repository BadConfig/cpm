#include "../hdrs/API.h"

void inputParser__runBuild(str* p,str* f){
    cpmCompiller__buildApp(p,f);
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
    if( argc >= 2 ){
        char* command = argv[1];
        if ( aphinString__cmpstr(aphinString__mkstr(command),aphinString__mkstr("init")) == 0 ){
            ip__run_init(aphinString__mkstr(argv[2]));
        } if ( aphinString__cmpstr(aphinString__mkstr(command),aphinString__mkstr("build")) == 0 ){
            str flags = aphinString__mkstr(" ");
            str path = aphinString__mkstr(""); 
            for(int i = 0; i < argc; ++i){
                if ( aphinString__cmpstr(aphinString__mkstr(argv[i]),aphinString__mkstr("-p")) == 0 ){
                    path = aphinString__mkstr(argv[i+1]);
                    ++i;       
                }
                if ( aphinString__cmpstr(aphinString__mkstr(argv[i]),aphinString__mkstr("-f")) == 0 ){
                    flags = aphinString__append(3,
                            flags,
                            aphinString__mkstr(" "),
                            aphinString__mkstr(argv[i+1]));
                            ++i;
                }
            }
            str *p,*f;
            f = &flags;
            if ( path.size == 0 ) p = NULL;
            else                  p = &path;
            inputParser__runBuild(p,f);
        }
    }
}

void main(int argc,char* argv[]){
    parser(argc,argv);
    return;
}
