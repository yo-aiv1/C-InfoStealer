#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <../include/openssl/evp.h>

#include "../include/decryptor.h"


unsigned char *DataDecryptor(unsigned char *value, int size, unsigned char *key) {
    unsigned char *InitialVector = malloc((IV_END_INDEX - IV_START_INDEX) * sizeof(unsigned char));
    if (InitialVector == NULL) {
        return NULL;
    }
    memcpy(InitialVector, value + IV_START_INDEX, IV_END_INDEX - IV_START_INDEX);

    int EncryptedLen = size - IV_END_INDEX - 16;
    const unsigned char *SubString = value + IV_END_INDEX;

    unsigned char *EnecryptedValue = malloc(EncryptedLen);
    if (EnecryptedValue == NULL) {
        free(InitialVector);
        return NULL;
    }
    memcpy(EnecryptedValue, SubString, EncryptedLen);

    EVP_CIPHER_CTX *ctx;
    int len, FinalLen;

    ctx = EVP_CIPHER_CTX_new();

    EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, InitialVector);

    unsigned char *DecryptedValue = malloc(EncryptedLen + EVP_MAX_BLOCK_LENGTH);
    if (DecryptedValue == NULL) {
        EVP_CIPHER_CTX_free(ctx);
        return NULL;
    }

    EVP_DecryptUpdate(ctx, DecryptedValue, &len, EnecryptedValue, EncryptedLen);
    EVP_DecryptFinal_ex(ctx, DecryptedValue + len, &FinalLen);

    DecryptedValue[len + FinalLen] = '\0';

    EVP_CIPHER_CTX_free(ctx);
    free(InitialVector);
    free(EnecryptedValue);

    return DecryptedValue;
}
