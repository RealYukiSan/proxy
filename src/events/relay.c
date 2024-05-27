#include <stdio.h>
#include <string.h>
#include <enet/enet.h>

#include <config.h>
#include <event.h>
#include <events/relay.h>
#include <packet/packet.h>
#include <utils/string.h>

void ConnectToRelay(void) {
    printf("[RELAY EVENT] Proxy connected into Server\n");
}

void RelayRecivePacket(ENetEvent ev, ENetPeer *server, ENetPeer *relay) {
    switch(GetMessageTypeFromPacket(ev.packet)) {
        case 1: {
            printf("[RELAY EVENT] Server just send Hello Packet\n");
            printf("[RELAY EVENT] Sending login info\n");
            SendPacketPacket(ev.packet, server);
            break;
        }
        case 2: {
            char* packetText = GetTextPointerFromPacket(ev.packet);
            printf("[RELAY EVENT] Packet 2: received packet text: %s\n", packetText);
            enet_peerSend(ev.packet, server);
            break;
        }
        case 3: {
            char* packetText = GetTextPointerFromPacket(ev.packet);
            printf("[RELAY EVENT] Packet 3: received packet text: %s\n", packetText);
            enet_peerSend(ev.packet, server);
            break;
        }
        case 4: {
            printf("[RELAY EVENT] Packet 4: Received packet tank: %d\n", ev.packet->data[4]);
            switch(ev.packet->data[4]) {
                case 1: {
                    unsigned char* packetTank = GetExtendedDataPointerFromTankPacket(ev.packet->data + 4);
                    unsigned char count = packetTank++[0];

                    for (unsigned char a = 0; a < count; a++) {
                        unsigned char index = packetTank++[0];
                        unsigned char type = packetTank++[0];

                        switch(type) {
                            case 1: {
                                float value;
                                memcpy(&value, packetTank, 4); packetTank += 4;
                                printf("[RELAY EVENT] TankUpdatePacket Variable: float found at %d: %f\n", index, value);
                                break;
                            }
                            case 2: {
                                int strLen;
                                memcpy(&strLen, packetTank, 4); packetTank += 4;

                                char* value = malloc(strLen + 1);
                                memcpy(value, packetTank, strLen); packetTank += strLen;
                                value[strLen] = '\0';

                                switch(index) {
                                    case 0: {
                                        if (strcmp(value, "OnSendToServer") == 0) {
                                            memset(&OnSendToServer, 0, sizeof(OnSendToServer));
                                            OnPacket.OnSendToServer = 1;
                                        }
                                        else if (strcmp(value, "OnConsoleMessage") == 0) {
                                            memset(&OnConsoleMessage, 0, sizeof(OnConsoleMessage));
                                            OnPacket.OnConsoleMessage = 1;
                                        }
                                        else if (strcmp(value, "OnSpawn") == 0) {
                                            OnPacket.OnSpawn = 1;
                                        }
                                        else if (strcmp(value, "OnTalkBubble") == 0) {
                                            OnPacket.OnTalkBubble = 1;
                                        }
                                        else if (strcmp(value, "OnDialogRequest") == 0) {
                                            OnPacket.OnDialogRequest = 1;
                                        }
                                        break;
                                    }
                                    case 1: {
                                        if (OnPacket.OnSpawn) {
                                            char** toSplit = strsplit(value, "\n", 0);
                                            if (strcmp(toSplit[11], "type|local") == 0) {
                                                char** netid = strsplit(toSplit[findarray(toSplit, "netID|")], "|", 0);
                                                OnSpawn.LocalNetid = atoi(netid[1]);
                                                free(netid);
                                            }
                                            free(toSplit);
                                            OnPacket.OnSpawn = 0;
                                        }
                                        else if (OnPacket.OnConsoleMessage) {
                                            OnPacket.OnConsoleMessage = 0;
                                        }
                                        else if (OnPacket.OnDialogRequest) {
                                            OnPacket.OnDialogRequest = 0;
                                        }
                                        break;
                                    }
                                    case 2: {
                                        if (OnPacket.OnTalkBubble)
                                            OnPacket.OnTalkBubble = 0;
                                        break;
                                    }
                                    case 4: {
                                        if (OnPacket.OnSendToServer) {
                                            asprintf(&OnSendToServer.rawSplit, "%s", value);
                                            char** toSplit = strsplit(value, "|", 0);
                                            asprintf(&OnSendToServer.serverAddress, "%s", toSplit[0]);
                                            asprintf(&OnSendToServer.UUIDToken, "%s", toSplit[2]);
                                            free(toSplit);
                                        }
                                        break;
                                    }
                                }

                                printf("[RELAY EVENT] TankUpdatePacket Variable: string found at %d: %s\n", index, value);
                                free(value);
                                break;
                            }
                            case 3: {
                                float value1;
                                float value2;

                                memcpy(&value1, packetTank, 4); packetTank += 4;
                                memcpy(&value2, packetTank, 4); packetTank += 4;

                                printf("[RELAY EVENT] TankUpdatePacket Variable: vector found at %d: %f, %f\n", index, value1, value2);
                                break;
                            }
                            case 5: {
                                int value;
                                memcpy(&value, packetTank, 4); packetTank += 4;

                                printf("[RELAY EVENT] TankUpdatePacket Variable: integer X found at %d: %d\n", index, value);
                                break;
                            }
                            case 9: {
                                int value;
                                memcpy(&value, packetTank, 4); packetTank += 4;

                                switch(index) {
                                    case 1: {
                                        if (OnPacket.OnSendToServer) OnSendToServer.port = value;
                                        break;
                                    }
                                    case 2: {
                                        if (OnPacket.OnSendToServer) OnSendToServer.token = value;
                                        break;
                                    }
                                    case 3: {
                                        if (OnPacket.OnSendToServer) OnSendToServer.userID = value;
                                        break;
                                    }
                                    case 5: {
                                        if (OnPacket.OnSendToServer) OnSendToServer.unkInt = value;
                                        break;
                                    }
                                }

                                printf("[RELAY EVENT] TankUpdatePacket Variable: integer found at %d: %d\n", index, value);
                                break;
                            }
                            default: {
                                printf("[RELAY EVENT] TankUpdatePacket Variable: unknown variable type: %d\n", type);
                                break;
                            }
                        }
                    }
                    if (OnPacket.OnSendToServer) {
                        char** splitString = strsplit(OnSendToServer.rawSplit, "|", 0);
                        splitString[0] = "127.0.0.1";
                        enet_peerSend(onPacketCreate(0, 0, "sdddsd", "OnSendToServer", 17091, OnSendToServer.token, OnSendToServer.userID, arrayjoin(splitString, "|", 0), OnSendToServer.unkInt), server);
                        free(splitString);
                        free(OnSendToServer.rawSplit);
                        OnPacket.OnSendToServer = 0;
                        isSendToServer = 1;
                    }
                    else if (config->skipGazette == 2) {
                        // leave it empty if you want skip the gazette
                        // if you want add your own gazette, you can do

                        /*enet_peerSend(onPacketCreate(0, 0, "ss",
                        "OnDialogRequest",
                        "Your dialog here"), clientPeer);*/

                        config->skipGazette--;
                    } else
                        enet_peerSend(ev.packet, server);
                    break;
                }
                default: {
                    printf("[RELAY EVENT] Unknown stuff: %d\n", GetMessageTypeFromPacket(ev.packet));
                    enet_peerSend(ev.packet, server);
                    break;
                }
            }
            break;
        }
        default: {
            printf("[RELAY EVENT] Unknown message type: %d\n", GetMessageTypeFromPacket(ev.packet));
            enet_peerSend(ev.packet, server);
            break;
        }
    }
}

void RelayClosed(void) {
    printf("[RELAY EVENT] Proxy just disconnected from Server\n");
}
