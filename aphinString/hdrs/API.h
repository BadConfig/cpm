#pragma once
#ifndef aphinString
#define aphinString
#include "deps.h"

typedef struct str{
    size_t size;// string length
    char*  str; // string pointer in heap
} str;

/* global functions:
 * this module presents tools
 * to work with creating etc
 * dirs files
 */

str     aphinString__mkstr(char* lit);
str     aphinString__append(size_t count, ...);
int8_t  aphinString__cmpstr(str a, str b);
void    aphinString__drop(str);
str     aphinString__upDir(str dir);
str     aphinString__cpyBytes(char* from, size_t size);
str     aphinString__changeFormat(str path,str fmt);
uint8_t aphinString__substOf(str substr,str string);
#endif
