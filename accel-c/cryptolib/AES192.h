/*********************************************************************
* Filename:   AES192.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

    void CRYPTO_AES192_EncryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[52]);
    void CRYPTO_AES192_DecryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[52]);
    void CRYPTO_AES192_KeyExpansion(const uint8_t srcKey[24], uint32_t dstExpandedKey[52]);

    void CRYPTO_AES192_EncryptBlock_AESNI(uint8_t srcBytes[16], const uint32_t srcExpandedKey[52]);
    void CRYPTO_AES192_DecryptBlock_AESNI(uint8_t srcBytes[16], const uint32_t srcExpandedKey[52]);
    void CRYPTO_AES192_DecryptBlock_AESNI_Fast(uint8_t srcBytes[16], const uint32_t srcInvExpandedKey[52]);

    void CRYPTO_AES192_KeyExpansion_AESNI_AVX2(const uint8_t srcKey[24], uint32_t dstExpandedKey[52]);
    void CRYPTO_AES192_GetInvExpandedKey_AESNI(const uint32_t srcExpandedKey[52], uint32_t dstInvExpandedKey[52]);
#if defined(__cplusplus)
}
#endif