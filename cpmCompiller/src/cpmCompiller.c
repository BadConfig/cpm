#include "../hdrs/cpmCompiller.h"

cpmConfig readConfig(){
    str home = aphinString__append(
            2,
            aphinString__mkstr(getenv("HOME")),
            aphinString__mkstr("/.cpmrc.json") 
            );

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

    str name        = aphinString__mkstr(jsonVal->u.object.values[0].value->u.string.ptr);
    str email       = aphinString__mkstr(jsonVal->u.object.values[1].value->u.string.ptr); 
    size_t count    = jsonVal->u.object.values[2].value->u.array.length; 
    str* paths      = (str*)malloc(count);
    for(size_t i = 0; i < count; ++i)
        paths[i]    = aphinString__mkstr(
                jsonVal->u.object.values[2].value->u.array.values[i]->u.string.ptr);

    cpmConfig ret = {name,email,count,paths};
    return ret;
}

str detectModulePath(){
    str dir     = fileSystem__pwd();
    str path    = aphinString__append(3,
                aphinString__mkstr(dir.str),
                aphinString__mkstr("/"),
                aphinString__mkstr("mod.json")); 
    FILE *desk  = fopen(path.str,"rb");
    while( desk == NULL && dir.str != NULL ){
        dir     = aphinString__upDir(dir);
        path    = aphinString__append(3,
                aphinString__mkstr(dir.str),
                aphinString__mkstr("/"),
                aphinString__mkstr("mod.json")); 
        desk = fopen(path.str,"rb");
    }
    if( desk != NULL ) fclose(desk);
    return dir;
}

str getModuleFromSpot(str currentSpot, cpmConfig conf, str name){
    FILE*   module;
    str path = aphinString__append(4,
                        aphinString__mkstr(currentSpot.str),
                        aphinString__mkstr("/"),
                        aphinString__mkstr(name.str),
                        aphinString__mkstr("/mod.json")
                        );
    if ( (module = fopen(path.str,"rw")) != NULL){
        path = aphinString__upDir(path);
        fclose(module);
        return path;
    }
    for(size_t i = 0; i < conf.pathsCount; ++i){
        path = aphinString__append(4,
                        aphinString__mkstr(conf.pathsToSpots[i].str),
                        aphinString__mkstr("/"),
                        aphinString__mkstr(name.str),
                        aphinString__mkstr("/mod.json"));
        if ( (module = fopen(path.str,"rw")) != NULL){
            path = aphinString__upDir(path);
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
    t.name              = aphinString__mkstr(jsonVal->u.object.values[0].value->u.string.ptr);
    t.version           = aphinString__mkstr(jsonVal->u.object.values[1].value->u.string.ptr);
    t.path              = aphinString__upDir(path);

    json_value* deps_d  = jsonVal->u.object.values[2].value->u.object.values[0].value;
    json_value* deps_s  = jsonVal->u.object.values[2].value->u.object.values[1].value;

    t.dep.dinamic_s     = deps_d->u.array.length;
    t.dep.static_s      = deps_s->u.array.length;
    t.dep.dynamic_d     = (str*)malloc(deps_d->u.array.length);
    t.dep.static_d      = (str*)malloc(deps_s->u.array.length);

    for(size_t i = 0; i < t.dep.static_s; ++i)
       t.dep.static_d[i] = aphinString__mkstr(deps_s->u.array.values[i]->u.string.ptr);
    for(size_t i = 0; i < t.dep.dinamic_s; ++i)
       t.dep.dynamic_d[i] = aphinString__mkstr(deps_d->u.array.values[i]->u.string.ptr);

    if ( t.dep.static_s != 0 )
    t.dep.static_d[0] = aphinString__mkstr(deps_s->u.array.values[0]->u.string.ptr);
    fclose(desk);
    free(buff);
    return t;
}
uint8_t cpmCompiller__buildApp(str* pathToMod,str* COMPILE_FLAGS){
    str linkString = cpmCompiller__build(pathToMod);
    if ( COMPILE_FLAGS != NULL )
        linkString = aphinString__append(3,
            linkString,
            aphinString__mkstr(" "),
            *COMPILE_FLAGS);
    system(linkString.str);
    return 1;
}
str cpmCompiller__build(str* pathToMod){

    str modPath;
    if ( pathToMod == NULL )    modPath = detectModulePath();   
    else                        modPath = *pathToMod;
    moduleInfo modInf   = parseJsonInfo(aphinString__append(3,
                modPath,
                aphinString__mkstr("/"),
                aphinString__mkstr("mod.json")));
    modInf.path         = modPath;
    str linkingStr      = aphinString__append(4,
                                    aphinString__mkstr("gcc -o "),
                                    aphinString__mkstr(modPath.str),
                                    aphinString__mkstr("/build/relise/"),
                                    aphinString__mkstr(modInf.name.str));
    str         includedFiles = aphinString__mkstr(" ");
    cpmCompiller__buildModule(modInf,&linkingStr,&includedFiles);
    return linkingStr;
}

uint8_t cpmCompiller__buildModule(moduleInfo mod, str* linkingStr,str* includedFiles){
    for(size_t i = 0; i < mod.dep.static_s; ++i){
        str depPath = getModuleFromSpot(
                aphinString__upDir(detectModulePath()),
                readConfig(),
                aphinString__mkstr(mod.dep.static_d[i].str));
        depPath = aphinString__append(2,
                depPath,
                aphinString__mkstr("/mod.json"));
        cpmCompiller__buildModule(parseJsonInfo(depPath),linkingStr,includedFiles);
    } 
    mod.path = getModuleFromSpot(
            aphinString__upDir(detectModulePath()),
            readConfig(),
            aphinString__mkstr(mod.name.str));
    str commandString = generateAssemblingStr(mod.path,linkingStr,includedFiles);
    system(commandString.str);
    aphinString__drop(commandString);
}

str generateAssemblingStr(str modPath,str* linker,str* includedFiles){
    size_t          toComp  = 0;
    str             commOut = aphinString__mkstr(" -o "),
                    commIn  = aphinString__mkstr(" ");
    str             srcDir  = aphinString__append(2,
                                aphinString__mkstr(modPath.str),
                                aphinString__mkstr("/src/")),
                    objDir  = aphinString__append(2,
                                aphinString__mkstr(modPath.str),
                                aphinString__mkstr("/build/obj/")); 
    DIR*            d       = opendir(srcDir.str);
    struct stat     sourceinfo,
                    objInfo;
    struct dirent*  entry; 
    str             sourcesPath,
                    objectsPath;
    while (( entry = readdir(d)) != NULL){
        if (!aphinString__cmpstr(aphinString__mkstr(entry->d_name),
                    aphinString__mkstr("."))) continue;
        if (!aphinString__cmpstr(aphinString__mkstr(entry->d_name),
                    aphinString__mkstr(".."))) continue;

       sourcesPath = aphinString__append(2,
               aphinString__mkstr(srcDir.str),
               aphinString__mkstr(entry->d_name));
       objectsPath = aphinString__append(2,
               aphinString__mkstr(objDir.str),
               aphinString__changeFormat(
                   aphinString__mkstr(entry->d_name),
                   aphinString__mkstr(".o")));
       stat(sourcesPath.str,&sourceinfo);
       int f = stat(objectsPath.str,&objInfo);
        if (!aphinString__substOf(aphinString__mkstr(entry->d_name),aphinString__mkstr(includedFiles->str)))
       *linker = aphinString__append(3,
               *linker,
               aphinString__mkstr(" "),
               aphinString__mkstr(objectsPath.str));
        *includedFiles = aphinString__append(3,
            *includedFiles,
            aphinString__mkstr(" "),
            aphinString__mkstr(sourcesPath.str));
       if ( f == -1 || objInfo.st_mtim.tv_sec < sourceinfo.st_mtim.tv_sec ){
           commIn   = aphinString__append(3,
                        commIn,
                        sourcesPath,
                        aphinString__mkstr(" "));
           commOut  = aphinString__append(3,
                        commOut,
                        objectsPath,
                        aphinString__mkstr(" "));
            ++toComp;
       } else {
           aphinString__drop(objectsPath);
           aphinString__drop(sourcesPath);
       }
    }
    closedir(d);
    return toComp > 0 ? aphinString__append(3,
            aphinString__mkstr("gcc -c"),
            commIn,
            commOut) : aphinString__mkstr(" ");
}

str generateSOStr(){

}


