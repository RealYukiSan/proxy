#ifndef RELAY_EVENT_H
#define RELAY_EVENT_H
#include <enet/enet.h>

void ConnectToRelay(void);
void RelayRecivePacket(ENetEvent ev, ENetPeer *server, ENetPeer *relay);
void RelayClosed(void);

#endif // RELAY_EVENT_H
