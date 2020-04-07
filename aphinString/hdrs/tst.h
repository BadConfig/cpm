#include "stdio.h"

#define assert(a) \
    if ( !a ){ \
        printf("failed"); \
        return 0;\
    }


