#ifndef CRYPTO_UTIL_H
#define CRYPTO_UTIL_H
#include <ctype.h> 

char* sha256Gen(char* data);
char* generateKlv(char* gameVersion, char* hash, char* rid, char* protocol, char isAndroid);
int32_t protonHash(const char* data);

#endif // CRYPTO_UTIL_H
