#pragma once
#include "deps.h"
#include "libs.h"

typedef struct str{
    size_t size;// string length
    char*  str; // string pointer in heap
} str;

/* global functions:
 * this module presents tools
 * to work with creating etc
 * dirs files
 */

// creates strings from char* buffer till '\0'
str     s(char* lit);
// same, from n characters ! works bit faster
str     strn(char* lit, size_t n);
// input: str typed strings, free them, returns their concatination
str     strAdd(size_t count, ...); 
// returns the diffenence between first not same symbols
int8_t  strCmp(str a, str b);
// free the string
void    strDrop(str);
// path up
str     strUpDir(str dir);
// changes file extensision
str     strChExt(str path,str ext);
// look for substing returns the addr of first match
char*   strSubstr(str substr,str string);
// creates path from strings
str     strPath(uint32_t count, ...);
