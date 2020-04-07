#include "stdio.h"
#define assert(a) if (a) { \
    printf("error in assert: ##a##\n"); \
    return 1;               \
}                           \
else {return 0;}


#define tst(a) \
    if (a == 1) { \
        printf("error in test func: ##a##\n");\
    } else \
    printf("test a clear\n");


int test__fs__mkstr_1();

int tst__fs__append_1();
