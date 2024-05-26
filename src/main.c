#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <enet.h>
#include <time.h>

#include <config.h>
#include <event.h>
#include <utils/string.h>
#include <core/ENetService.h>
#include <core/HttpService.h>

#include <events/relay.h>
#include <events/server.h>

int run = 1;

void signal_recive(int status) {
    run = 0;
}

int main(void) {
    pthread_t server;
    config_t *config = InitConfig();
    
    if (!config) {
        printf("Failed to initialize config.\n");
        return 1;
    }

    srand(time(NULL));

    memset(&user, 0, sizeof(user_t));
    user.wk = generateHex(16);
    user.rid = generateHex(16);
    user.deviceID = generateHex(16);
    user.mac = generateHex(0);

    memset(&OnPacket, 0, sizeof(OnPacket));

    ENetInit();

    ENetHost *enetServer = CreateENetServer();
    ENetHost *enetRelay = CreateENetClient(config);
    
    pthread_create(&server, NULL, HTTPSServer, config);
    signal(SIGINT, signal_recive);

    while (run) {
        ENetEvent enetServerEvent;
        ENetEvent enetRelayEvent;

        while (enet_host_service(enetServer, &enetServerEvent, 5) > 0) {
            ENetServerPeer = enetServerEvent.peer;
            switch (enetServerEvent.type) {
            case ENET_EVENT_TYPE_CONNECT:
                ConnectToServer(config);
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                ClientRecivePacket(enetServerEvent, ENetServerPeer, ENetRelayPeer, config);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                ServerDisconnect(ENetRelayPeer);
                break;
            }
        }

        while(enet_host_service(enetRelay, &enetRelayEvent, 5) > 0) {
            switch (enetRelayEvent.type) {
            case ENET_EVENT_TYPE_CONNECT:
                ConnectToRelay();
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                RelayRecivePacket(enetRelayEvent, ENetServerPeer, ENetRelayPeer, config);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                RelayClosed(ENetServerPeer);
                break;
            }
        }
    } 
    
    ENetDestroy();
    free(config);
    
    printf("\n");
    printf("[PROXY MESSAGE] Proxy exited.\n");
}
