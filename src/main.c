#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <enet/enet.h>
#include <time.h>

#include <config.h>
#include <event.h>
#include <utils/string.h>
#include <core/ENetService.h>
#include <core/HttpService.h>

#include <events/relay.h>
#include <events/server.h>

static int initialized = 0;

void signal_recive(int status) {
    (void)status;
    running = 0;
}

int main(void) {
    if (!initialized) {
        if (!InitConfig()) {
            printf("Failed to initialize config.\n");
            return 1;
        }
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
    ENetHost *enetRelay = CreateENetClient();
    
    if (!initialized) { 
        pthread_t server;
        pthread_create(&server, NULL, HTTPSServer, NULL);
        initialized = 1;
    }

    signal(SIGINT, signal_recive);

    while (running) {
        ENetEvent enetServerEvent;
        ENetEvent enetRelayEvent;

        while (enet_host_service(enetServer, &enetServerEvent, 5) > 0) {
            ENetServerPeer = enetServerEvent.peer;
            switch (enetServerEvent.type) {
            case ENET_EVENT_TYPE_CONNECT:
                ConnectToServer();
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                ClientRecivePacket(enetServerEvent, ENetServerPeer, ENetRelayPeer);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                ServerDisconnect();
                break;
            default:
                break;
            }
        }

        while(enet_host_service(enetRelay, &enetRelayEvent, 5) > 0) {
            switch (enetRelayEvent.type) {
            case ENET_EVENT_TYPE_CONNECT:
                ConnectToRelay();
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                RelayRecivePacket(enetRelayEvent, ENetServerPeer, ENetRelayPeer);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                RelayClosed();
                break;
            default:
                break;
            }
        }
    } 
    
    ENetDestroy();

    if (user.isMetaMalloc) {
        free(user.meta);
        user.isMetaMalloc = 0;
    }

    free(user.wk);
    free(user.rid);
    free(user.deviceID);
    free(user.mac);

    if (restart) {
        printf("[PROXY MESSAGE] Got Restart Task, Restarting...\n");
        user.isLogin = 0;
        running = 1;
        restart = 0;
        return main();
    }

    if (config) 
        free(config);


    printf("\n");
    printf("[PROXY MESSAGE] Proxy exited.\n");
}
