#ifndef ENETSERVICE_H
#define ENETSERVICE_H

#include <config.h>
#include <enet.h>

extern ENetAddress *ENetGrowtopiaAddress;
extern ENetAddress *ENetProxyAddress;
extern ENetHost *ENetServer;
extern ENetHost *ENetRelay;
extern ENetPeer *ENetServerPeer;
extern ENetPeer *ENetRelayPeer;

int ENetInit(void);
ENetHost *CreateENetServer(void);
ENetHost *CreateENetClient(config_t *config);
void ENetDestroy(void);

#endif // ENETSERVICE_H
