#include "deps.h"
#include "libs.h"
/* global functions:
 * this module presents tools
 * to work with creating etc
 * dirs files
 */
str         pwd();
void        makeDir(str path);
str         readFile(FILE* desk, int64_t offset, int64_t bytesToRead);
void        writeFile(uint32_t desk, str data);
