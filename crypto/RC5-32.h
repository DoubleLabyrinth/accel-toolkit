/*********************************************************************
* Filename:   RC5-32.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#define CRYPTO_RC5_32_SUCCESS 0
#define CRYPTO_RC5_32_BUFFER_TOO_SHORT -1

#define CRYPTO_RC5_32_MIN_KEY_LENGTH 0u
#define CRYPTO_RC5_32_MAX_KEY_LENGTH 255u

#if defined(__cplusplus)
extern "C" {
#endif

    void CRYPTO_RC5_32_EncryptBlock(uint8_t srcBytes[8], const uint32_t srcExpandedKey[], uint8_t round_count);
    void CRYPTO_RC5_32_DecryptBlock(uint8_t srcBytes[8], const uint32_t srcExpandedKey[], uint8_t round_count);

    // srcKeyLength can be 0x00 to 0xFF.
    // round_count can be 0x00 to 0xFF.
    // The length of dstExpandedKey must be at least 2 * (round_count + 1).
    int CRYPTO_RC5_32_KeyExpansion(const uint8_t srcKey[],
                                   uint8_t srcKeyLength,
                                   uint8_t round_count,
                                   uint32_t dstExpandedKey[], size_t dstExpandedKeyBufLength);

#if defined(__cplusplus)
}
#endif
