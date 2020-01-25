#include "../hdrs/API.h"

void inputParser__runBuild(str* p,str* f){
    cpmCompiller__buildApp(p,f);
}

void ip__run_init(str module_name){
    str dir = fileSystem__pwd();
    dir = aphinString__append(3,dir,aphinString__mkstr("/"),aphinString__mkstr(module_name.str));
    fileSystem__mkdir(dir);

    str subdir = aphinString__mkstr(dir.str);
    subdir = aphinString__append(2,subdir,aphinString__mkstr("/test"));
    fileSystem__mkdir(subdir);
    subdir = aphinString__mkstr(dir.str);
    subdir = aphinString__append(2,subdir,aphinString__mkstr("/doc"));
    fileSystem__mkdir(subdir);
    subdir = aphinString__mkstr(dir.str);
    subdir = aphinString__append(2,subdir,aphinString__mkstr("/hdrs"));
    str hdrs    = aphinString__append(2,aphinString__mkstr(subdir.str),
                                        aphinString__mkstr("/API.h"));
    fileSystem__mkdir(subdir);
    FILE* d     = fopen(hdrs.str,"a");
    fwrite((void*)"#include \"deps.h\"\n",1,18,d);
    fclose(d);
    hdrs        = aphinString__append(2,aphinString__upDir(hdrs),
                                        aphinString__mkstr("/deps.h"));
    d           = fopen(hdrs.str,"a");
    fclose(d);
    hdrs        = aphinString__append(2,aphinString__upDir(hdrs),
                                        aphinString__mkstr("/test.h"));
    d           = fopen(hdrs.str,"a");
    fclose(d);
    subdir = aphinString__mkstr(dir.str);
    subdir = aphinString__append(2,subdir,aphinString__mkstr("/build"));
    fileSystem__mkdir(subdir);
    subdir = aphinString__mkstr(dir.str);
    subdir = aphinString__append(2,subdir,aphinString__mkstr("/src"));
    fileSystem__mkdir(subdir);
    aphinString__drop(hdrs);
    hdrs        = aphinString__append(4,
                                        aphinString__mkstr(subdir.str),
                                        aphinString__mkstr("/"),
                                        aphinString__mkstr(module_name.str),
                                        aphinString__mkstr(".c"));
    d           = fopen(hdrs.str,"a");
    fwrite((void*)"#include \"../hdrs/API.h\"\n",1,25,d);
    fclose(d);
    subdir = aphinString__mkstr(dir.str);
    subdir = aphinString__append(2,subdir,aphinString__mkstr("/res"));
    fileSystem__mkdir(subdir);
    subdir = aphinString__mkstr(dir.str);
    subdir = aphinString__append(2,
                subdir,
                aphinString__mkstr("/mod.json"));
    d      = fopen(subdir.str,"a");
    str json = aphinString__append(3,aphinString__mkstr("{\n\"name\": \""),
            aphinString__mkstr(module_name.str),
            aphinString__mkstr("\",\n\"version\":\
\"0.0.0\",\n\"deps\":{\n\"dynamic\":[\n],\n\"static\":[\n]\n}\n}\n"));
    fwrite(json.str,1,json.size,d);
    aphinString__drop(json);
    fclose(d);
    aphinString__drop(dir);
    aphinString__drop(subdir);
    aphinString__drop(hdrs);
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
        } else if (!aphinString__cmpstr(aphinString__mkstr(command),aphinString__mkstr("refr"))){
            cpmCompiller__refresh();
        }
    }
}

void main(int argc,char* argv[]){
    parser(argc,argv);
    return;
}
