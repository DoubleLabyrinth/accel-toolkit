/*********************************************************************
* Filename:   RC4.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stddef.h>
#include <stdint.h>

#define CRYPTO_RC4_SUCCESS 0
#define CRYPTO_RC4_INVALID_KEY_LENGTH -1
#define CRYPTO_RC4_KEY_TOO_LONG -2

#define CRYPTO_RC4_MIN_KEY_LENGTH 1u
#define CRYPTO_RC4_MAX_KEY_LENGTH 256u

#if defined(__cplusplus)
extern "C" {
#endif

    inline void CRYPTO_RC4_Process(uint8_t srcBytes[], size_t srcBytesLength,
                                   uint8_t srcS[256]) {
        int i = 0, j = 0;
        for (size_t k = 0; k < srcBytesLength; ++k) {
            i = (i + 1) % 256;
            j = (j + srcS[i]) % 256;
            uint8_t temp = srcS[i];
            srcS[i] = srcS[j];
            srcS[j] = temp;
            srcBytes[k] ^= srcS[(srcS[i] + srcS[j]) % 256];
        }
    }

    inline int CRYPTO_RC4_KeyExpansion(const uint8_t srcKey[], size_t srcKeyLength,
                                       uint8_t dstS2[256]) {
        if (srcKeyLength == 0)
            return CRYPTO_RC4_INVALID_KEY_LENGTH;
        if (srcKeyLength > CRYPTO_RC4_MAX_KEY_LENGTH)
            return CRYPTO_RC4_KEY_TOO_LONG;

        for (int i = 0; i < 256; ++i)
            dstS2[i] = srcKey[i % srcKeyLength];
        return CRYPTO_RC4_SUCCESS;
    }

    inline void CRYPTO_RC4_Reset(uint8_t srcS[256], const uint8_t srcS2[256]) {
        for (int i = 0; i < 256; ++i)
            srcS[i] = (uint8_t)i;
        for (int i = 0, j = 0; i < 256; i++) {
            j = (j + srcS[i] + srcS2[i]) % 256;
            uint8_t temp = srcS[i];
            srcS[i] = srcS[j];
            srcS[j] = temp;
        }
    }

#if defined(__cplusplus)
}
#endif
