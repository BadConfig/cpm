#include "deps.h"

typedef struct moduleInfo{
    str name;
    str version;
    struct deps{
        str* dynamic_d;
        size_t dinamic_s;
        str* static_d;
        size_t static_s;
    } dep;
    str path;
} moduleInfo;

uint8_t     cpmCompiller__build();
uint8_t     cpmCompiller__buildModule(moduleInfo mod);
// creates executable test file using test folder
uint8_t     cpmCompiller__buildTests(str modName);
// creates an app if main is found in your module
uint8_t     cpmCompiller__buildApp(str modName);

