#ifndef HTTPSERVER_H
#define HTTPSERVER_H 

struct HTTPInfo {
    unsigned char* buffer;
    int bufferLen;
};

struct HTTPInfo HTTPSClient(const char* website);
void* HTTPSServer(void *unused);

#endif // HTTPSERVER_H
