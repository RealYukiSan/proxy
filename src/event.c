#include <event.h>

int running = 1;
int restart = 0;
char isSendToServer;
user_t user;
packet_t OnPacket;
console_t OnConsoleMessage;
server_t OnSendToServer;
spawn_t OnSpawn;


