/*********************************************************************
* Filename:   TEA.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

    void CRYPTO_TEA_EncryptBlock(uint8_t srcBytes[8], const uint8_t srcKey[16]);
    void CRYPTO_TEA_DecryptBlock(uint8_t srcBytes[8], const uint8_t srcKey[16]);

#if defined(__cplusplus)
}
#endif