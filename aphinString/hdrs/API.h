#pragma once
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

// different ways to make strings
str     s(char* lit);
str     a(size_t count, ...); // uses str typa as args
int8_t  cmp(str a, str b);
void    drop(str);
str     aphinString__upDir(str dir);
str     aphinString__cpyBytes(char* from, size_t size);
str     aphinString__changeFormat(str path,str fmt);
uint8_t aphinString__substOf(str substr,str string);

