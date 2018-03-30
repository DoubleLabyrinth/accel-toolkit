/*********************************************************************
* Filename:   RC2.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#define CRYPTO_RC2_SUCCESS 0
#define CRYPTO_RC2_INVALID_KEY_LENGTH -1
#define CRYPTO_RC2_KEY_TOO_LONG -2
#define CRYPTO_RC2_EFFECTIVE_KEY_TOO_LONG -3

#define CRYPTO_RC2_MIN_KEY_LENGTH 1u
#define CRYPTO_RC2_MAX_KEY_LENGTH 128u

#if defined(__cplusplus)
extern "C" {
#endif

    void CRYPTO_RC2_EncryptBlock(uint8_t srcBytes[8], const uint16_t srcExpandedKey[64]);
    void CRYPTO_RC2_DecryptBlock(uint8_t srcBytes[8], const uint16_t srcExpandedKey[64]);

    int CRYPTO_RC2_KeyExpansion(const uint8_t srcKey[],
                                uint8_t srcKeyLength,
                                uint16_t eftKey_BitLength,
                                uint16_t dstExpandedKey[64]);

#if defined(__cplusplus)
}
#endif
