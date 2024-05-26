#ifndef SERVER_EVENT_H
#define SERVER_EVENT_H
#include <enet/enet.h>

#include <config.h>

void ConnectToServer(config_t *config);
void ClientRecivePacket(ENetEvent ev, ENetPeer* server, ENetPeer* relay, config_t *config);
void ServerDisconnect(ENetPeer *relay);

#endif // SERVER_EVENT_H
