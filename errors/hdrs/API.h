#include "deps.h"
#include "libs.h"

// MACRO to end the program work
// required stdlib.h
#define PANIC exit(0)
#define NO_ERR 0
#define MALLOC_FN malloc
#define FREE_FN free

#define Return(a) {err e = {0,\
MALLOC_FN(sizeof(a)),sizeof(a)};\
return e;}

#define Error(a) {err e = {1,\
a};\
return e;}

typedef struct err{
    const uint16_t errCode;
    union u {
        struct d{
            void* data;
            const size_t size;
        }d;
        void (*reaction)(struct err,void*);
    }u;
} err;

void upckErr(err e, void* data);




