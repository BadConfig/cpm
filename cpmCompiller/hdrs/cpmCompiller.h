#include "API.h"


typedef struct cpmConfig{
    str     name;
    str     email; 
    size_t  pathsCount;
    str*    pathsToSpots;  
} cpmConfig;

cpmConfig   readConfig();
moduleInfo  parseJsonInfo(str path); 
str         getModuleFromSpot (str currentSpot, cpmConfig conf, str name);
str         generateAssemblingStr(str modPath);
