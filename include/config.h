#ifndef CONFIG_H
#define CONFIG_H
typedef struct {
    char usingServerData;
    char* serverDataIP;
    char* manualIP;
    char* manualMeta;
    int manualPort;
    char usingNewPacket;
    short httpsPort;
    char skipGazette;
    char isSpoofed;
} config_t;

config_t *InitConfig(void);
void LoadConfig(config_t *config); 
#endif // CONFIG_H
