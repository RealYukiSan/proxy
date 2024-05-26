#include <sha256.h>
#include <ctype.h>

#include <utils/string.h>
#include <utils/crypto.h>

char* sha256Gen(char* data) {
    char* result = malloc(65); // 32 + 1
    BYTE sha256res[32];
    SHA256_CTX ctx;
    sha256Init(&ctx);
    sha256UpdateLen(&ctx, data);
    sha256Final(&ctx, sha256res);

    for (int a = 0, b = 0; a < 32; a++) {
        sprintf(result + b, "%02x", sha256res[a]);
        b += 2;
    }
    result[64] = '\0';

    return result;
}

char* generateKlv(char* gameVersion, char* hash, char* rid, char* protocol, char isAndroid) {
    char* gameVersion_sha256 = sha256Gen(gameVersion);
    char* hash_sha256 = sha256Gen(hash);
    char* protocol_sha256 = sha256Gen(protocol);
    char* rid_sha256 = sha256Gen(rid);

    char* result = sha256Gen(CatchMessage("%s198c4213effdbeb93ca64ea73c1f505f%s82a2e2940dd1b100f0d41d23b0bb6e4d%sc64f7f09cdd0c682e730d2f936f36ac2%s27d8da6190880ce95591215f2c9976a6", gameVersion_sha256, hash_sha256, protocol_sha256, rid_sha256));

    free(gameVersion_sha256);
    free(hash_sha256);
    free(protocol_sha256);
    free(rid_sha256);

    return result;
}

int32_t protonHash(const char* data) {
    int hash = 0x55555555;
    while(*data) hash = (hash >> 27) + (hash << 5) + *data++;

    return hash;
}
