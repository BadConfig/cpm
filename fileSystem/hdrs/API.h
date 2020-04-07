#pragma once
#include "deps.h"
#include "libs.h"
/* global functions:
 * this module presents tools
 * to work with creating etc
 * dirs files
 */
err         pwd();
err         makeDir(str path);
err         readFile(str path);
err         writeFile(str path, str data);

// error handler
void pwdError(err e,void* d);
void openFileError(err e,void* d);
void noCommandInterpreter(err e, void* d);