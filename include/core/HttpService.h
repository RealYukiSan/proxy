#ifndef HTTPSERVER_H
#define HTTPSERVER_H 
#include <config.h>

struct HTTPInfo {
    unsigned char* buffer;
    int bufferLen;
};

struct HTTPInfo HTTPSClient(const char* website);
void* HTTPSServer(void *config);

#endif // HTTPSERVER_H
