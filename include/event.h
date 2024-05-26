#ifndef EVENT_H
#define EVENT_H

typedef struct {
    char* meta;
    char isMetaMalloc;
    char* wk;
    char* rid;
    char* mac;
    char* gid;
    char* deviceID;
    char isLogin;
} user_t;

typedef struct {
    char OnConsoleMessage;
    char OnSendToServer;
    char OnSpawn;
    char OnTalkBubble;
    char OnDialogRequest;
} packet_t;

typedef struct {
    char *message;
} console_t;

typedef struct {
    char* serverAddress;
    int port;
    int token;
    int userID;
    char* UUIDToken;
    int unkInt;
    char* rawSplit;
} server_t;

typedef struct {
    int LocalNetid;
} spawn_t;

extern char isSendToServer;
extern user_t user;
extern packet_t OnPacket;
extern console_t OnConsoleMessage;
extern server_t OnSendToServer;
extern spawn_t OnSpawn;

#endif // EVENT_H
