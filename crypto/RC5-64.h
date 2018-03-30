/*********************************************************************
* Filename:   RC5-64.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#define CRYPTO_RC5_64_SUCCESS 0
#define CRYPTO_RC5_64_BUFFER_TOO_SHORT -1

#define CRYPTO_RC5_64_MIN_KEY_LENGTH 0u
#define CRYPTO_RC5_64_MAX_KEY_LENGTH 255u

#if defined(__cplusplus)
extern "C" {
#endif

    void CRYPTO_RC5_64_EncryptBlock(uint8_t srcBytes[16], const uint64_t srcExpandedKey[], uint8_t round_count);
    void CRYPTO_RC5_64_DecryptBlock(uint8_t srcBytes[16], const uint64_t srcExpandedKey[], uint8_t round_count);

    // srcKeyLength can be 0x00 to 0xFF.
    // round_count can be 0x00 to 0xFF.
    // The length of dstExpandedKey must be at least 2 * (round_count + 1).
    int CRYPTO_RC5_64_KeyExpansion(const uint8_t srcKey[],
                                   uint8_t srcKeyLength,
                                   uint8_t round_count,
                                   uint64_t dstExpandedKey[], size_t dstExpandedKeyBufLength);

#if defined(__cplusplus)
}
#endif
