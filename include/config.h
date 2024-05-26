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

extern config_t *config;
int InitConfig(void);
void LoadConfig(void); 
#endif // CONFIG_H
