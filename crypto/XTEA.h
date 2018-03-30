/*********************************************************************
* Filename:   XTEA.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

    void CRYPTO_XTEA_EncryptBlock(uint8_t srcBytes[8], const uint8_t srcKey[16], uint32_t round_count);
    void CRYPTO_XTEA_DecryptBlock(uint8_t srcBytes[8], const uint8_t srcKey[16], uint32_t round_count);

#if defined(__cplusplus)
}
#endif
