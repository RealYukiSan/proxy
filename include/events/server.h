#ifndef SERVER_EVENT_H
#define SERVER_EVENT_H
#include <enet/enet.h>

#include <config.h>

void ConnectToServer(void);
void ClientRecivePacket(ENetEvent ev, ENetPeer* server, ENetPeer* relay);
void ServerDisconnect(void);

#endif // SERVER_EVENT_H
