#ifndef RELAY_EVENT_H
#define RELAY_EVENT_H
#include <enet.h>
#include <event.h>

void ConnectToRelay(void);
void RelayRecivePacket(ENetEvent ev, ENetPeer *server, ENetPeer *relay, config_t *config);
void RelayClosed(ENetPeer *server);

#endif // RELAY_EVENT_H
