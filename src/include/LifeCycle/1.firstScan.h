#ifndef _FIRST_SCAN_H_
#define _FIRST_SCAN_H_

#include "global.h"
#include <regex>
#include "symbol.h"
extern SymbolTable S;

FILE *changeTabToSpace();
FILE *recordFunctionAndCompositeName();
void removeTempFile();
static string getFileNameAll(string str);

#endif
