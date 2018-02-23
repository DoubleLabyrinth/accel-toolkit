/*********************************************************************
* Filename:   AES256.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

    void CRYPTO_AES256_EncryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[60]);
    void CRYPTO_AES256_DecryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[60]);
    void CRYPTO_AES256_KeyExpansion(const uint8_t srcKey[32], uint32_t dstExpandedKey[60]);

    void CRYPTO_AES256_EncryptBlock_AESNI(uint8_t srcBytes[16], const uint32_t srcExpandedKey[60]);
    void CRYPTO_AES256_DecryptBlock_AESNI(uint8_t srcBytes[16], const uint32_t srcExpandedKey[60]);
    void CRYPTO_AES256_DecryptBlock_AESNI_Fast(uint8_t srcBytes[16], const uint32_t srcInvExpandedKey[60]);
    void CRYPTO_AES256_KeyExpansion_AESNI(const uint8_t srcKey[32], uint32_t dstExpandedKey[60]);
    void CRYPTO_AES256_GetInvExpandedKey_AESNI(const uint32_t srcExpandedKey[60], uint32_t dstInvExpandedKey[60]);

#if defined(__cplusplus)
}
#endif
