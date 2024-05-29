/*
The observer program help you analyze and monitor packet traffic.

gcc -DVERBOSE ../src/core/HttpService.c ../src/config.c ../src/utils/string.c main.c -o main \
-I ../include/ \
-I ../thirdparty/tlse \
-L ../build/ -lEnet -lTlse

don't forget to copy the config file.
or just run make command.

TODO: switch to subserver (?)
*/

#include <ctype.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include <enet/enet.h>
#include "core/HttpService.h"
#include "config.h"
#include "utils/string.h"

#define MAX_PARTS 100
#define BONUS_LENGTH 255

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

user_t user;
ENetPeer *ENetServerPeer;
ENetPeer *ENetRelayPeer;
char isSendToServer;

char **split(char *str, char *delim)
{
    char** parts = malloc(MAX_PARTS * sizeof(char*));
    if (parts == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    int part_count = 0;
    char* token = strtok(str, delim);  // Get the first part

    while (token != NULL && part_count < MAX_PARTS) {
        char *item = malloc(strlen(token) + BONUS_LENGTH);
        if (item == NULL) {
            fprintf(stderr, "Memory allocation failed!\n");
            exit(1);
        }
        strcpy(item, token);
        parts[part_count] = item; // Duplicate the part to avoid overwriting
        part_count++;
        token = strtok(NULL, delim);   // Get the next part
    }

    parts[part_count] = NULL; // Terminate the array with a NULL pointer

    return parts;
}

int main(void)
{
    ENetAddress *ENetGrowtopiaAddress;
    ENetAddress *ENetProxyAddress;

    if (!InitConfig()) {
        printf("Failed to initialize config.\n");
        return 1;
    }

    puts("Starting web server...");
    pthread_t server;
    pthread_create(&server, NULL, HTTPSServer, NULL);
    
    memset(&user, 0, sizeof(user_t));
    user.wk = generateHex(16);
    user.rid = generateHex(16);
    user.deviceID = generateHex(16);
    user.mac = generateHex(0);

    ENetProxyAddress = malloc(sizeof(ENetAddress));

    ENetProxyAddress->host = 0;
    ENetProxyAddress->port = 17091;
    
    enet_initialize();

    ENetHost *enetServer = enet_host_create(ENetProxyAddress, 1024, 10, 0, 0);
    enetServer->checksum = enet_crc32;
    enetServer->usingNewPacketForServer = 0;
    enet_host_compress_with_range_coder(enetServer);

    ENetHost *enetRelay = enet_host_create(NULL, 1, 2, 0, 0);
    enetRelay->checksum = enet_crc32;
    enetRelay->usingNewPacket = 1;
    enet_host_compress_with_range_coder(enetRelay);

    unsigned char server_destroyed = 0;
    unsigned char relay_destroyed = 0;
    
    puts("starting event loop...");
    while (1) {
        ENetEvent enetServerEvent;
        ENetEvent enetRelayEvent;

        while (enet_host_service(enetServer, &enetServerEvent, 5) > 0) {
            ENetServerPeer = enetServerEvent.peer;
            switch (enetServerEvent.type) {
            case ENET_EVENT_TYPE_CONNECT:
                puts("[SERVER EVENT] connected to the server's peer");

                if (!ENetRelayPeer) {
                    struct HTTPInfo info;
                    info = HTTPSClient(config->serverDataIP);

                    char** arr = strsplit(info.buffer + (findstr(info.buffer, "server|") - 7), "\n", 0);

                    enet_address_set_host(ENetGrowtopiaAddress, arr[findarray(arr, "server|")] + 7);
                    ENetGrowtopiaAddress->port = atoi(arr[findarray(arr, "port|")] + 5);
                    ENetRelayPeer = enet_host_connect(enetRelay, ENetGrowtopiaAddress, 2, 0);
                    printf("[RELAY EVENT] connecting to %s:%s...\n", arr[findarray(arr, "server|")] + 7, arr[findarray(arr, "port|")] + 5);
                    if (user.isMetaMalloc) free(user.meta);
                    asprintf(&user.meta, "%s", arr[findarray(arr, "meta|")] + 5);
                    user.isMetaMalloc = 1;

                    free(arr);
                }
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                if (ENetRelayPeer) {
                    if (enetServerEvent.packet->data[0] == 2) {
                        char** loginInfo = split((char *)&enetServerEvent.packet->data[4], "\n");
                        sprintf(loginInfo[findarray(loginInfo, "meta|")], "meta|%s", user.meta);
                        char* updatedMeta = arrayjoin(loginInfo, "\n", 1);
                        enet_packet_resize(enetServerEvent.packet, strlen(updatedMeta) + 1);
                        strcpy((char *)&enetServerEvent.packet->data[4], updatedMeta);
                    }

                    puts("[SERVER EVENT] packet received from the server peer...");
                    puts("[SERVER EVENT] forward it to the relay peer...");

                    for (int i = 0; i < enetServerEvent.packet->dataLength; i++) {
                        char byte = enetServerEvent.packet->data[i];
                        if (isalnum(byte))
                            putchar(byte);
                        else
                            printf(" %02hhX ", byte);
                    }
                    puts("");
                    enet_peer_send(ENetRelayPeer, 0, enetServerEvent.packet);
                }
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                puts("[SERVER EVENT] the server's peer disconnected.");
                enet_host_flush(enetServer);
                enet_peer_disconnect_now(ENetServerPeer, 0);

                server_destroyed = 1;

                if (!relay_destroyed) {
                    enet_host_flush(enetRelay);
                    enet_peer_disconnect_now(ENetRelayPeer, 0);
                    relay_destroyed = 1;
                } else relay_destroyed = 0;
                break;
            default:
                break;
            }
        }

        while(enet_host_service(enetRelay, &enetRelayEvent, 5) > 0) {
            switch (enetRelayEvent.type) {
            case ENET_EVENT_TYPE_CONNECT:
                puts("[RELAY EVENT] the relay connected to the Growtopia server peer");
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                puts("[RELAY EVENT] packet received from the relay peer...");
                puts("[RELAY EVENT] forward it to the server peer...");
                for (int i = 0; i < enetRelayEvent.packet->dataLength; i++) {
                    char byte = enetRelayEvent.packet->data[i];
                    if (isalnum(byte))
                        putchar(byte);
                    else
                        printf(" %02hhX ", byte);
                }
                puts("");
                enet_peer_send(ENetServerPeer, 0, enetRelayEvent.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                puts("[RELAY EVENT] the relay peer disconnected.");
                enet_host_flush(enetRelay);
                enet_peer_disconnect_now(ENetRelayPeer, 0);
                ENetRelayPeer = NULL;

                relay_destroyed = 1;                

                if (!server_destroyed) {
                    enet_host_flush(enetServer);
                    enet_peer_disconnect_now(ENetServerPeer, 0);
                    server_destroyed = 1;
                } else server_destroyed = 0;
                break;
            default:
                break;
            }
        }
    }
    return 0;
}
