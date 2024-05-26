#include <stdio.h>
#include <string.h>
#include <enet/enet.h>

#include <config.h>
#include <event.h>
#include <core/HttpService.h>
#include <core/ENetService.h>
#include <utils/string.h>
#include <utils/crypto.h>
#include <packet/packet.h>

void ConnectToServer(void) {
    if (isSendToServer) {
        printf("[PROXY EVENT] Client connected into proxy\n");
        printf("[PROXY EVENT] Connecting to subserver...\n");
        enet_host_destroy(ENetRelay);

        ENetRelay = enet_host_create(NULL, 1, 2, 0, 0);
        ENetRelay->checksum = enet_crc32;
        ENetRelay->usingNewPacket = config->usingNewPacket;
        enet_host_compress_with_range_coder(ENetRelay);
        enet_address_set_host(ENetGrowtopiaAddress, OnSendToServer.serverAddress);
        ENetGrowtopiaAddress->port = OnSendToServer.port;
        ENetRelayPeer = enet_host_connect(ENetRelay, ENetGrowtopiaAddress, 2, 0);

        free(OnSendToServer.serverAddress);
        free(OnSendToServer.UUIDToken);

        isSendToServer = 0;
    } else {
        printf("[PROXY EVENT] Client connected into proxy\n");
        printf("[PROXY EVENT] Connecting to Growtopia Server...\n");

        if (config->usingServerData) {
            struct HTTPInfo info;
            info = HTTPSClient(config->serverDataIP);

            char** arr = strsplit(info.buffer + (findstr(info.buffer, "server|") - 7), "\n", 0);

            enet_address_set_host(ENetGrowtopiaAddress, arr[findarray(arr, "server|")] + 7);
            ENetGrowtopiaAddress->port = atoi(arr[findarray(arr, "port|")] + 5);
            ENetRelayPeer = enet_host_connect(ENetRelay, ENetGrowtopiaAddress, 2, 0);
            if (user.isMetaMalloc) free(user.meta);
            asprintf(&user.meta, "%s", arr[findarray(arr, "meta|")] + 5);
            user.isMetaMalloc = 1;

            free(arr);
        }
        else {
            enet_address_set_host(ENetProxyAddress, config->manualIP);
            ENetProxyAddress->port = config->manualPort;
            ENetRelayPeer = enet_host_connect(ENetRelay, ENetProxyAddress, 2, 0);
        }

    }
}

void ClientRecivePacket(ENetEvent ev, ENetPeer* server, ENetPeer* relay, config_t *config) {
    switch(GetMessageTypeFromPacket(ev.packet)) {
        case 2: {
            char* packetText = GetTextPointerFromPacket(ev.packet);

            if (!user.isLogin) {
                char** loginInfo = strsplit(packetText, "\n", 0);
                if (config->usingServerData)
                    loginInfo[findarray(loginInfo, "meta|")] = CatchMessage("meta|%s", user.meta);
                else 
                    loginInfo[findarray(loginInfo, "meta|")] = CatchMessage("meta|%s", config->manualMeta);

                if (config->isSpoofed) {
                    char* klvGen;

                    loginInfo[findarray(loginInfo, "wk|")] = CatchMessage("wk|%s", user.wk);
                    loginInfo[findarray(loginInfo, "rid|")] = CatchMessage("rid|%s", user.rid);
                    loginInfo[findarray(loginInfo, "mac|")] = CatchMessage("mac|%s", user.mac);
                    loginInfo[findarray(loginInfo, "hash|")] = CatchMessage("hash|%d", protonHash(CatchMessage("%sRT", user.mac)));
                    loginInfo[findarray(loginInfo, "hash2|")] = CatchMessage("hash2|%d", protonHash(CatchMessage("%sRT", user.deviceID)));

                    if (findarray(loginInfo, "gid|") == -1) 
                        klvGen = generateKlv(loginInfo[findarray(loginInfo, "game_version|")] + 13, loginInfo[findarray(loginInfo, "hash|")] + 5, user.rid, loginInfo[findarray(loginInfo, "protocol|")] + 9, 0);
                    else 
                        klvGen = generateKlv(loginInfo[findarray(loginInfo, "game_version|")] + 13, loginInfo[findarray(loginInfo, "hash|")] + 5, user.rid, loginInfo[findarray(loginInfo, "protocol|")] + 9, 1);

                    loginInfo[findarray(loginInfo, "klv|")] = CatchMessage("klv|%s", klvGen);

                    free(klvGen);
                }

                char* resultSpoofed = arrayjoin(loginInfo, "\n", 1);
                printf("[PROXY EVENT] Spoofed Login info: %s\n", resultSpoofed);
                sendPacket(2, resultSpoofed, relay);

                free(resultSpoofed);
                user.isLogin = 1;

                break;
            }

            printf("[PROXY EVENT] Packet 2: received packet text: %s\n", packetText);

            enet_peerSend(ev.packet, relay);
            break;
        }
        case 3: {
            char* packetText = GetTextPointerFromPacket(ev.packet);
            printf("[PROXY EVENT] Packet 3: received packet text: %s\n", packetText);
            if (strcmp(packetText, "action|quit") == 0) {
                running = 0;
                restart = 1;
            }
            
            enet_peerSend(ev.packet, relay);
            break;
        }
        case 4: {
            switch(ev.packet->data[4]) {
                case 26: {
                    enet_peerSend(ev.packet, relay);
                    enet_peer_disconnect_now(server, 0);
                    enet_peer_disconnect_now(relay, 0);
                    break;
                }
                default: {
                    printf("[PROXY EVENT] TankUpdatePacket: Unknown packet tank type: %d\n", ev.packet->data[4]);
                    enet_peerSend(ev.packet, relay);
                    break;
                }
            }
            break;
        }
        default: {
            printf("[PROXY EVENT] Unknown message type: %d\n", GetMessageTypeFromPacket(ev.packet));
            enet_peerSend(ev.packet, relay);
            break;
        }
    }
}

void ServerDisconnect(void) {
    printf("[PROXY EVENT] Client just disconnected from Proxy\n");
    running = 0;
    restart = 1;
}
