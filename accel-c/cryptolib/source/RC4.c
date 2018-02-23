/*********************************************************************
* Filename:   RC4.cpp
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../RC4.h"

static inline int RC4_process(uint8_t srcBytes[],
                               size_t srcBytesLength,
                               const uint8_t srcKey[],
                               uint16_t srcKeyLength) {
    if (srcKeyLength == 0)
        return CRYPTO_RC4_INVALID_KEY_LENGTH;
    if (srcKeyLength > CRYPTO_RC4_MAX_KEY_LENGTH)
        return CRYPTO_RC4_KEY_TOO_LONG;

    uint8_t S[256] = { 0 };
    uint8_t S2[256] = { 0 };

    for (int i = 0; i < 256; ++i) {
        S[i] = (uint8_t)i;
        S2[i] = srcKey[i % srcKeyLength];
    }

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + S2[i]) % 256;
        uint8_t temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }

    int i = 0;
    j = 0;
    for (size_t k = 0; k < srcBytesLength; ++k) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        uint8_t temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        srcBytes[k] ^= S[(S[i] + S[j]) % 256];
    }

    return CRYPTO_RC4_SUCCESS;
}

int CRYPTO_RC4_Decrypt(uint8_t srcBytes[],
                       size_t srcBytesLength,
                       const uint8_t srcKey[],
                       uint16_t srcKeyLength) {
    return RC4_process(srcBytes, srcKeyLength, srcKey, srcKeyLength);
}

int CRYPTO_RC4_Encrypt(uint8_t srcBytes[],
                       size_t srcBytesLength,
                       const uint8_t srcKey[],
                       uint16_t srcKeyLength) {
    return RC4_process(srcBytes, srcKeyLength, srcKey, srcKeyLength);
}
