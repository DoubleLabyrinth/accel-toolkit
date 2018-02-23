/*********************************************************************
* Filename:   BLowfish.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#define CRYPTO_BLOWFISH_SUCCESS 0
#define CRYPTO_BLOWFISH_INVALID_KEY_LENGTH -1
#define CRYPTO_BLOWFISH_KEY_TOO_LONG -2

#define CRYPTO_BLOWFISH_LITTLE_ENDIAN 0
#define CRYPTO_BLOWFISH_BIG_ENDIAN 1

#define CRYPTO_BLOWFISH_MIN_KEY_LENGTH 1u
#define CRYPTO_BLOWFISH_MAX_KEY_LENGTH 56u

#if defined(__cplusplus)
extern "C" {
#endif

    void CRYPTO_Blowfish_EncryptBlock(uint8_t srcBytes[8],
                                      const uint32_t srcSubKeys[18],
                                      const uint32_t srcSBox[4][256],
                                      int Endian);

    void CRYPTO_Blowfish_DecryptBlock(uint8_t srcBytes[8],
                                      const uint32_t srcSubKeys[18],
                                      const uint32_t srcSBox[4][256],
                                      int Endian);

    int CRYPTO_Blowfish_KeyExpansion(uint8_t srcKey[],
                                     uint8_t srcKeyLength,
                                     uint32_t dstSubKeys[18],
                                     uint32_t dstSBox[4][256]);

#if defined(__cplusplus)
}
#endif
