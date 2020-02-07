#include "../hdrs/API.h"

void inputParser__runBuild(str* p,str* f){
    cpmCompiller__buildApp(p,f);
}

void ip__run_init(str module_name){
    str dir = pwd();
    dir = a(3,dir,s("/"),s(module_name.str));
    makeDir(dir);

    str subdir = s(dir.str);
    subdir = a(2,subdir,s("/test"));
    makeDir(subdir);
    subdir = s(dir.str);
    subdir = a(2,subdir,s("/doc"));
    makeDir(subdir);
    subdir = s(dir.str);
    subdir = a(2,subdir,s("/hdrs"));
    str hdrs    = a(2,s(subdir.str),s("/API.h"));
    makeDir(subdir);
    FILE* d     = fopen(hdrs.str,"a");
    fwrite((void*)"#include \"deps.h\"\n",1,18,d);
    fclose(d);
    hdrs        = a(2,aphinString__upDir(hdrs),s("/deps.h"));
    d           = fopen(hdrs.str,"a");
    fclose(d);
    hdrs        = a(2,aphinString__upDir(hdrs),s("/test.h"));
    d           = fopen(hdrs.str,"a");
    fclose(d);
    subdir = s(dir.str);
    subdir = a(2,subdir,s("/build"));
    makeDir(subdir);
    subdir = s(dir.str);
    subdir = a(2,subdir,s("/src"));
    makeDir(subdir);
    drop(hdrs);
    hdrs        = a(4,s(subdir.str),s("/"),s(module_name.str),s(".c"));
    d           = fopen(hdrs.str,"a");
    fwrite((void*)"#include \"../hdrs/API.h\"\n",1,25,d);
    fclose(d);
    subdir = s(dir.str);
    subdir = a(2,subdir,s("/res"));
    makeDir(subdir);
    subdir = s(dir.str);
    subdir = a(2,subdir,s("/mod.json"));
    d      = fopen(subdir.str,"a");
    str json = a(3,s("{\n\"name\": \""),
            s(module_name.str),
            s("\",\n\"version\":\
\"0.0.0\",\n\"deps\":{\n\"dynamic\":[\n],\n\"static\":[\n]\n}\n}\n"));
    fwrite(json.str,1,json.size,d);
    drop(json);
    fclose(d);
    drop(dir);
    drop(subdir);
    drop(hdrs);
}

void parser(int argc,char* argv[]){
    if( argc >= 2 ){
        char* command = argv[1];
        if ( cmp(s(command),s("init")) == 0 ){
            ip__run_init(s(argv[2]));
        } if ( cmp(s(command),s("build")) == 0 ){
            str flags = s(" ");
            str path = s(""); 
            for(int i = 0; i < argc; ++i){
                if ( cmp(s(argv[i]),s("-p")) == 0 ){
                    path = s(argv[i+1]);
                    ++i;       
                }
                if ( cmp(s(argv[i]),s("-f")) == 0 ){
                    flags = a(3,flags,s(" "),s(argv[i+1]));
                            ++i;
                }
            }
            str *p,*f;
            f = &flags;
            if ( path.size == 0 ) p = NULL;
            else                  p = &path;
            inputParser__runBuild(p,f);
        } else if (!cmp(s(command),s("refr"))){
            cpmCompiller__refresh();
        } else if (!cmp(s(command),s("add"))){
            if (argc > 2){
                remoteControll__clone(s(argv[2]));
            }
        }
    }
}

void main(int argc,char* argv[]){
    parser(argc,argv);
    return;
}
