/*********************************************************************
* Filename:   Twofish256.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

    void CRYPTO_Twofish256_EncryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[40], const uint32_t srcS[4]);
    void CRYPTO_Twofish256_DecryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[40], const uint32_t srcS[4]);
    void CRYPTO_Twofish256_KeyExpansion(const uint8_t srcKey[32], uint32_t dstExpandedKey[40], uint32_t dstS[4]);

#if defined(__cplusplus)
}
#endif
