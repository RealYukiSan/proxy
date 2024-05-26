#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <config.h>
#include <utils/string.h>

void LoadConfig(config_t *config) {
    FILE *file = fopen("config.conf", "rb");
    size_t size;

    if (!file) {
        perror("Failed to open config file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = malloc(size + 1);
    fread(content, size, 1, file);
    fclose(file);
    
    char **script = strsplit(content, "\n", 0);

    int idx = 0;

    while (script[idx]) {
        if (script[idx][0] == '#') continue; // NOTE: Skip if is a comment
        
        char **var = strsplit(script[idx++], "=", 0);
        
        if (strcmp(var[0], "usingServerData") == 0) {
            if (var[1][0] == '1') 
                config->usingServerData = 1;
            else 
                config->usingServerData = 0;
        }
        if (strcmp(var[0], "serverDataIP") == 0)
            strcopy(&config->serverDataIP, var[1]);
        if (strcmp(var[0], "manualIP") == 0)
            strcopy(&config->manualIP, var[1]);
        if (strcmp(var[0], "manualPort") == 0)
            config->manualPort = atoi(var[1]);
        if (strcmp(var[0], "manualMeta") == 0)
            strcopy(&config->manualMeta, var[1]);
        if (strcmp(var[0], "usingNewPacket") == 0) {
            if (var[1][0] == '1') 
                config->usingNewPacket = 1;
            else
                config->usingNewPacket = 0;
        }
        if (strcmp(var[0], "httpsPort") == 0) 
            config->httpsPort = atoi(var[1]);
        if (strcmp(var[0], "skipGazette") == 0) {
            if (var[1][0] == '1') 
                config->skipGazette = 1;
            else
                config->skipGazette = 0;
        }
        if (strcmp(var[0], "isSpoofed") == 0) {
            if (var[1][0] == '1')
                config->isSpoofed = 1;
            else
                config->isSpoofed = 0;
        }

        free(var);
    }

    printf("[LOADED CONFIGURATION] Using Server Data\t: %s\n", config->usingServerData ? "Yes" : "No");
    if (config->usingServerData)
        printf("[LOADED CONFIGURATION] Growtopia Server IP\t: %s\n", config->serverDataIP);
    else {
        printf("[LOADED CONFIGURATION] Growtopia Custom Server IP\t: %s\n", config->manualIP);
        printf("[LOADED CONFIGURATION] Growtopia Custom Server PORT\t: %d\n", config->manualPort);
        printf("[LOADED CONFIGURATION] Growtopia Custom Server META\t: %s\n", config->manualMeta);
    }
    printf("[LOADED CONFIGURATION] Using New Packet\t\t: %s\n", config->usingNewPacket ? "Yes" : "No");
    printf("[LOADED CONFIGURATION] Server Local PORT\t: %d\n", config->httpsPort);
    printf("[LOADED CONFIGURATION] Skip Gazette\t\t: %s\n", config->skipGazette ? "Yes" : "No");
    printf("[LOADED CONFIGURATION] Is Spoofed\t\t: %s\n", config->isSpoofed ? "Yes" : "No");

    free(script);
    free(content);
}

config_t *InitConfig(void) {
    config_t *config = malloc(sizeof(config_t));
    if (!config) return NULL;
    LoadConfig(config);
    return config;
}

