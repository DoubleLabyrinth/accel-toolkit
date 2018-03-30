/*********************************************************************
* Filename:   CAST-128.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#define CRYPTO_CAST128_SUCCESS 0
#define CRYPTO_CAST128_KEY_TOO_SHORT -1
#define CRYPTO_CAST128_KEY_TOO_LONG -2

#define CRYPTO_CAST128_MIN_KEY_LENGTH 5u
#define CRYPTO_CAST128_MAX_KEY_LENGTH 16u

#if defined(__cplusplus)
extern "C" {
#endif

    void CRYPTO_CAST128_EncryptBlock(uint8_t srcBytes[8],
                                     const uint32_t srcKm[16],
                                     const uint32_t srcKr[16],
                                     uint8_t srcKeyLength);

    void CRYPTO_CAST128_DecryptBlock(uint8_t srcBytes[8],
                                     const uint32_t srcKm[16],
                                     const uint32_t srcKr[16],
                                     uint8_t srcKeyLength);

    int CRYPTO_CAST128_KeyExpansion(const uint8_t srcKey[],
                                    uint8_t srcKeyLength,
                                    uint32_t dstKm[16],
                                    uint32_t dstKr[16]);

#if defined(__cplusplus)
}
#endif
