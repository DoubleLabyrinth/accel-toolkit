/*********************************************************************
* Filename:   Twofish192.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

    void CRYPTO_Twofish192_EncryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[40], const uint32_t srcS[3]);
    void CRYPTO_Twofish192_DecryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[40], const uint32_t srcS[3]);
    void CRYPTO_Twofish192_KeyExpansion(const uint8_t srcKey[24], uint32_t dstExpandedKey[40], uint32_t dstS[3]);

#if defined(__cplusplus)
}
#endif
