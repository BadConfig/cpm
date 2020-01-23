#include "deps.h"
/* global functions:
 * this module presents tools
 * to work with creating etc
 * dirs files
 */
str         fileSystem__pwd();
int         fileSystem__mkfile(str path);
void        fileSystem__mkdir(str path);
str         fileSystem__rdfile(FILE* desk, int64_t offset, int64_t bytesToRead);
void        fileSystem__wrfile(uint32_t desk, str data);
FILE*       fileSystem__open(str path);
uint32_t    fileSystem__openInDir(str dir, str file);
void        fileSystem__close(FILE* desk);
