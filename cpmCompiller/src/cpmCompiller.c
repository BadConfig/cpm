#include "../hdrs/cpmCompiller.h"

void cpmCompiller__refresh(){
    str path = strAdd(2,detectModulePath(),s("/mod.json"));
    str deps = strAdd(2,detectModulePath(),s("/hdrs/deps.h"));
    str answ;
    moduleInfo inf = parseJsonInfo(path);
    str p;
    FILE* d = fopen(deps.str,"w");
    for(size_t i = 0; i < inf.dep.static_s; ++i ){
        p = getModuleFromSpot(
            strUpDir(detectModulePath()),
           readConfig(),
           s(inf.dep.static_d[i].str)); 
        answ = strAdd(3,s("#include \""),p,s("/hdrs/API.h\"\n"));
        fwrite(answ.str,1,answ.size,d); 
        strDrop(answ);
    }
    fclose(d);
    strDrop(path);
    strDrop(deps);
}

cpmConfig readConfig(){
    str home = strAdd(2,s(getenv("HOME")),s("/.cpmrc.json"));

    FILE* cpmrcDesk = fopen(home.str,"rb");

    fseek(cpmrcDesk,0,SEEK_END);
    size_t fileSize = ftell(cpmrcDesk);
    char* buff = (char*)malloc(fileSize);
    rewind(cpmrcDesk);

    size_t result = fread(buff,1,fileSize,cpmrcDesk);
    if (result != fileSize)     printf("error reading: %d\n",result);

    json_char* fileContent  = (json_char*)buff; 
    json_value* jsonVal     = json_parse(fileContent,fileSize);
    if (jsonVal == NULL) printf("jsonVal equal NULL\n");

    str name        = s(jsonVal->u.object.values[0].value->u.string.ptr);
    str email       = s(jsonVal->u.object.values[1].value->u.string.ptr); 
    size_t count    = jsonVal->u.object.values[2].value->u.array.length; 
    str* paths      = (str*)malloc(count);
    for(size_t i = 0; i < count; ++i)
        paths[i]    = s(jsonVal->u.object.values[2].value->u.array.values[i]->u.string.ptr);

    cpmConfig ret = {name,email,count,paths};
    return ret;
}

str detectModulePath(){
    str dir;
    upckErr(pwd(),&dir);
    str path    = strAdd(3,s(dir.str),s("/"),s("mod.json")); 
    FILE *desk  = fopen(path.str,"rb");
    while( desk == NULL && dir.str != NULL ){
        dir     = strUpDir(dir);
        path    = strAdd(3,s(dir.str),s("/"),s("mod.json")); 
        desk = fopen(path.str,"rb");
    }
    if( desk != NULL ) fclose(desk);
    return dir;
}

str getModuleFromSpot(str currentSpot, cpmConfig conf, str name){
    FILE*   module;
    str path = strAdd(4,s(currentSpot.str),s("/"),s(name.str),s("/mod.json"));
    if ( (module = fopen(path.str,"rw")) != NULL){
        path = strUpDir(path);
        fclose(module);
        return path;
    }
    for(size_t i = 0; i < conf.pathsCount; ++i){
        path = strAdd(4,s(conf.pathsToSpots[i].str),s("/"),s(name.str),s("/mod.json"));
        if ( (module = fopen(path.str,"rw")) != NULL){
            path = strUpDir(path);
            fclose(module);
            return path;
        }
    }
    str ret = {0,NULL};
    return ret;
}

moduleInfo parseJsonInfo(str path){
    FILE* desk = fopen(path.str,"rb");

    fseek(desk,0,SEEK_END);
    size_t fileSize = ftell(desk);
    char* buff = (char*)malloc(fileSize);
    rewind(desk);

    size_t result = fread(buff,1,fileSize,desk);
    if (result != fileSize)     printf("error reading: %d\n",result);

    json_char* fileContent  = (json_char*)buff; 
    json_value* jsonVal     = json_parse(fileContent,fileSize);


    if (jsonVal == NULL) printf("jsonVal equal NULL\n");

    moduleInfo t;
    t.name              = s(jsonVal->u.object.values[0].value->u.string.ptr);
    t.version           = s(jsonVal->u.object.values[1].value->u.string.ptr);
    t.path              = strUpDir(path);

    json_value* deps_d  = jsonVal->u.object.values[2].value->u.object.values[0].value;
    json_value* deps_s  = jsonVal->u.object.values[2].value->u.object.values[1].value;

    t.dep.dinamic_s     = deps_d->u.array.length;
    t.dep.static_s      = deps_s->u.array.length;
    t.dep.dynamic_d     = (str*)malloc(deps_d->u.array.length);
    t.dep.static_d      = (str*)malloc(deps_s->u.array.length);

    for(size_t i = 0; i < t.dep.static_s; ++i)
       t.dep.static_d[i] = s(deps_s->u.array.values[i]->u.string.ptr);
    for(size_t i = 0; i < t.dep.dinamic_s; ++i)
       t.dep.dynamic_d[i] = s(deps_d->u.array.values[i]->u.string.ptr);

    if ( t.dep.static_s != 0 )
    t.dep.static_d[0] = s(deps_s->u.array.values[0]->u.string.ptr);
    fclose(desk);
    free(buff);
    return t;
}
uint8_t cpmCompiller__buildApp(str* pathToMod,str* COMPILE_FLAGS){
    str linkString = cpmCompiller__build(pathToMod);
    if ( COMPILE_FLAGS != NULL )
        linkString = strAdd(3,linkString,s(" "),*COMPILE_FLAGS);
    system(linkString.str);
    return 1;
}
str cpmCompiller__build(str* pathToMod){

    str modPath;
    if ( pathToMod == NULL )    modPath = detectModulePath();   
    else                        modPath = *pathToMod;
    moduleInfo modInf   = parseJsonInfo(strAdd(3,s(modPath.str),s("/"),s("mod.json")));
    modInf.path         = s(modPath.str);
    str linkingStr      = strAdd(4,s("gcc -o "),s(modPath.str),s("/build/relise/"),s(modInf.name.str));
    str includedFiles   = s(" ");
    cpmCompiller__buildModule(modInf,&linkingStr,&includedFiles);
    return linkingStr;
}

uint8_t cpmCompiller__buildModule(moduleInfo mod, str* linkingStr,str* includedFiles){
    for(size_t i = 0; i < mod.dep.static_s; ++i){
        str depPath = getModuleFromSpot(
                strUpDir(detectModulePath()),
                readConfig(),
                s(mod.dep.static_d[i].str));
        depPath = strAdd(2,depPath,s("/mod.json"));
        cpmCompiller__buildModule(parseJsonInfo(depPath),linkingStr,includedFiles);
    } 
    mod.path = getModuleFromSpot(
            strUpDir(detectModulePath()),readConfig(),s(mod.name.str));
    str commandString = generateAssemblingStr(mod.path,linkingStr,includedFiles);
    system(commandString.str);
    strDrop(commandString);
}

str generateAssemblingStr(str modPath,str* linker,str* includedFiles){
    size_t          toComp  = 0;
    str             commOut = s(" -o "),
                    commIn  = s(" ");
    str             srcDir  = strAdd(2,s(modPath.str),s("/src/")),
                    objDir  = strAdd(2,s(modPath.str),s("/build/obj/")); 
    DIR*            d       = opendir(srcDir.str);
    struct stat     sourceinfo,
                    objInfo;
    struct dirent*  entry; 
    str             sourcesPath,
                    objectsPath;
    while (( entry = readdir(d)) != NULL){
        if (!strCmp(s(entry->d_name),s("."))) continue;
        if (!strCmp(s(entry->d_name),s(".."))) continue;

       sourcesPath = strAdd(2,
               s(srcDir.str),
               s(entry->d_name));
       objectsPath = strAdd(2,s(objDir.str),
               strChExt(s(entry->d_name),s(".o")));
       stat(sourcesPath.str,&sourceinfo);
       int f = stat(objectsPath.str,&objInfo);
        if (!strSubstr(s(entry->d_name),s(includedFiles->str)))
       *linker = strAdd(3,*linker,s(" "),s(objectsPath.str));
        *includedFiles = strAdd(3,*includedFiles,s(" "),s(sourcesPath.str));
       if ( f == -1 || objInfo.st_mtim.tv_sec < sourceinfo.st_mtim.tv_sec ){
           commIn   = strAdd(3,commIn,sourcesPath,s(" "));
           commOut  = strAdd(3,commOut,objectsPath,s(" "));
            ++toComp;
       } else {
           strDrop(objectsPath);
           strDrop(sourcesPath);
       }
    }
    closedir(d);
    return toComp > 0 ? strAdd(3,s("gcc -c"),commIn,commOut) : s(" ");
}

str generateSOStr(){

}


