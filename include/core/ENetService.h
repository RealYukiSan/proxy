#ifndef ENETSERVICE_H
#define ENETSERVICE_H

#include <enet/enet.h>

extern ENetAddress *ENetGrowtopiaAddress;
extern ENetAddress *ENetProxyAddress;
extern ENetHost *ENetServer;
extern ENetHost *ENetRelay;
extern ENetPeer *ENetServerPeer;
extern ENetPeer *ENetRelayPeer;

int ENetInit(void);
ENetHost *CreateENetServer(void);
ENetHost *CreateENetClient(void);
void ENetDestroy(void);

#endif // ENETSERVICE_H
