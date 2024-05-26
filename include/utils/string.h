#ifndef UTIL_STRING_H
#define UTIL_STRING_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** strsplit(const char* s, const char* delim, size_t* nb);
int strcopy(char** c, char* n);
int findstr(char* str, char* toFind);
int findarray(char** array, char* val);
char* generateHex(int len);
char* arrayjoin(char** array, char* joinVal, char autoRemove);
char* CatchMessage(const char *message, ...);

#endif // UTIL_STRING_H
