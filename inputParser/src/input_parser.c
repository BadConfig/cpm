#include "../hdrs/API.h"

void inputParser__runBuild(str* p,str* f){
    cpmCompiller__buildApp(p,f);
}

void ip__run_init(str module_name){
    str dir;
    upckErr(pwd(),&dir);
    dir = strAdd(3,dir,s("/"),s(module_name.str));
    upckErr(makeDir(dir),NULL);

    str subdir = s(dir.str);
    subdir = strAdd(2,subdir,s("/test"));
    upckErr(makeDir(subdir),NULL);
    subdir = s(dir.str);
    subdir = strAdd(2,subdir,s("/doc"));
    upckErr(makeDir(subdir),NULL);
    subdir = s(dir.str);
    subdir = strAdd(2,subdir,s("/hdrs"));
    str hdrs    = strAdd(2,s(subdir.str),s("/API.h"));
    upckErr(makeDir(subdir),NULL);
    FILE* d     = fopen(hdrs.str,"a");
    fwrite((void*)"#include \"deps.h\"\n",1,18,d);
    fclose(d);
    hdrs        = strAdd(2,strUpDir(hdrs),s("/deps.h"));
    d           = fopen(hdrs.str,"a");
    fclose(d);
    hdrs        = strAdd(2,strUpDir(hdrs),s("/test.h"));
    d           = fopen(hdrs.str,"a");
    fclose(d);
    subdir = s(dir.str);
    subdir = strAdd(2,subdir,s("/build"));
    upckErr(makeDir(subdir),NULL);
    subdir = s(dir.str);
    subdir = strAdd(2,subdir,s("/src"));
    upckErr(makeDir(subdir),NULL);
    strDrop(hdrs);
    hdrs        = strAdd(4,s(subdir.str),s("/"),s(module_name.str),s(".c"));
    d           = fopen(hdrs.str,"a");
    fwrite((void*)"#include \"../hdrs/API.h\"\n",1,25,d);
    fclose(d);
    subdir = s(dir.str);
    subdir = strAdd(2,subdir,s("/res"));
    upckErr(makeDir(subdir),NULL);
    subdir = s(dir.str);
    subdir = strAdd(2,subdir,s("/mod.json"));
    d      = fopen(subdir.str,"a");
    str json = strAdd(3,s("{\n\"name\": \""),
            s(module_name.str),
            s("\",\n\"version\":\
\"0.0.0\",\n\"deps\":{\n\"dynamic\":[\n],\n\"static\":[\n]\n}\n}\n"));
    fwrite(json.str,1,json.size,d);
    strDrop(json);
    fclose(d);
    strDrop(dir);
    strDrop(subdir);
    strDrop(hdrs);
}

void parser(int argc,char* argv[]){
    if( argc >= 2 ){
        char* command = argv[1];
        if ( strCmp(s(command),s("init")) == 0 ){
            ip__run_init(s(argv[2]));
        } if (strCmp(s(command),s("build")) == 0 ){
            str flags = s(" ");
            str path = s(""); 
            for(int i = 0; i < argc; ++i){
                if ( strCmp(s(argv[i]),s("-p")) == 0 ){
                    path = s(argv[i+1]);
                    ++i;       
                }
                if ( strCmp(s(argv[i]),s("-f")) == 0 ){
                    flags = strAdd(3,flags,s(" "),s(argv[i+1]));
                            ++i;
                }
            }
            str *p,*f;
            f = &flags;
            if ( path.size == 0 ) p = NULL;
            else                  p = &path;
            inputParser__runBuild(p,f);
        } else if (!strCmp(s(command),s("refr"))){
            cpmCompiller__refresh();
        } else if (!strCmp(s(command),s("add"))){
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
