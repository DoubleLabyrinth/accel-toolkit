/*********************************************************************
* Filename:   AES128.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

    void CRYPTO_AES128_EncryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[44]);
    void CRYPTO_AES128_DecryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[44]);
    void CRYPTO_AES128_KeyExpansion(const uint8_t srcKey[16], uint32_t dstExpandedKey[44]);

    void CRYPTO_AES128_EncryptBlock_AESNI(uint8_t srcBytes[16], const uint32_t srcExpandedKey[44]);
    void CRYPTO_AES128_DecryptBlock_AESNI(uint8_t srcBytes[16], const uint32_t srcExpandedKey[44]);
    void CRYPTO_AES128_DecryptBlock_AESNI_Fast(uint8_t srcBytes[16], const uint32_t srcInvExpandedKey[44]);

    void CRYPTO_AES128_KeyExpansion_AESNI(const uint8_t srcKey[16], uint32_t dstExpandedKey[44]);
    void CRYPTO_AES128_GetInvExpandedKey_AESNI(const uint32_t srcExpandedKey[44], uint32_t dstInvExpandedKey[44]);

#if defined(__cplusplus)
}
#endif
