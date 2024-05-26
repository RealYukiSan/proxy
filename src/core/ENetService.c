#include <core/ENetService.h>

ENetAddress *ENetGrowtopiaAddress;
ENetAddress *ENetProxyAddress;
ENetHost *ENetServer;
ENetHost *ENetRelay;
ENetPeer *ENetServerPeer;
ENetPeer *ENetRelayPeer;

int ENetInit(void) {
    enet_initialize();
    
    ENetProxyAddress = malloc(sizeof(ENetAddress));
    if (!ENetProxyAddress) {
        return 0;
    }

    ENetProxyAddress->host = 0;
    ENetProxyAddress->port = 17091;
    
    ENetGrowtopiaAddress = malloc(sizeof(ENetAddress));
    if (!ENetGrowtopiaAddress) {
        return 0;
    }

    return 1;
}

ENetHost *CreateENetServer(void) {
    ENetServer = enet_host_create(ENetProxyAddress, 1024, 10, 0, 0);
    ENetServer->checksum = enet_crc32;
    enet_host_compress_with_range_coder(ENetServer);
    return ENetServer;
}

ENetHost *CreateENetClient(config_t *config) {
    ENetRelay = enet_host_create(NULL, 1, 2, 0, 0);
    ENetRelay->checksum = enet_crc32;
    ENetRelay->usingNewPacket = config->usingNewPacket;
    enet_host_compress_with_range_coder(ENetRelay);
    return ENetRelay;
}


void ENetDestroy(void) {
    free(ENetGrowtopiaAddress);
    free(ENetProxyAddress);
    if (ENetServerPeer) enet_peer_disconnect_now(ENetServerPeer, 0);
    if (ENetRelayPeer) enet_peer_disconnect_now(ENetRelayPeer, 0);
    enet_host_destroy(ENetRelay);
    enet_host_destroy(ENetServer);
    enet_deinitialize();
}
