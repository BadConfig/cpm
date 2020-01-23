#include <uv.h>
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>

#include "./hdrs/API.h"
int main(){
    fs__append(2,fs__mkstr("a"),fs__mkstr("b"));
    return 0;
}
