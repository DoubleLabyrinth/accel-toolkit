/*********************************************************************
* Filename:   DES.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#define CRYPTO_DES_SUCCESS 0
#define CRYPTO_DES_INVALID_KEY -1

#if defined(__cplusplus)
extern "C" {
#endif

    void CRYPTO_DES_EncryptBlock(uint8_t srcBytes[8], const uint8_t srcExpandedKey[16][6]);
    void CRYPTO_DES_DecryptBlock(uint8_t srcBytes[8], const uint8_t srcExpandedKey[16][6]);

    void CRYPTO_3DES_EncryptBlock(uint8_t srcBytes[8],
                                  const uint8_t srcExpandedKey1[16][6],
                                  const uint8_t srcExpandedKey2[16][6],
                                  const uint8_t srcExpandedKey3[16][6]);

    void CRYPTO_3DES_DecryptBlock(uint8_t srcBytes[8],
                                  const uint8_t srcExpandedKey1[16][6],
                                  const uint8_t srcExpandedKey2[16][6],
                                  const uint8_t srcExpandedKey3[16][6]);

    //
    // Failed if function return CRYPTO_DES_INVALID_KEY.
    // If and only if the number of bit '1' contained in each uint8_t of array srcKey is not odd, the function will return fail.
    // 
    int CRYPTO_DES_KeyExpansion(const uint8_t srcKey[8], uint8_t dstExpandedKey[16][6]);

#if defined(__cplusplus)
}
#endif
