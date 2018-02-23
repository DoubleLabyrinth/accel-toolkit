/*********************************************************************
* Filename:   TEA.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../TEA.h"

const uint32_t CRYPTO_TEA_Delta = 0x9E3779B9;

void CRYPTO_TEA_EncryptBlock(uint8_t srcBytes[8], const uint8_t srcKey[16]) {
    const uint32_t* const k = (const uint32_t*)srcKey;
    uint32_t* const v = (uint32_t*)srcBytes;
    uint32_t sum = 0;

    for (int i = 0; i < 32; ++i) {
        sum += CRYPTO_TEA_Delta;
        v[0] += ((v[1] << 4) + k[0]) ^ (v[1] + sum) ^ ((v[1] >> 5) + k[1]);
        v[1] += ((v[0] << 4) + k[2]) ^ (v[0] + sum) ^ ((v[0] >> 5) + k[3]);
    }
}

void CRYPTO_TEA_DecryptBlock(uint8_t srcBytes[8], const uint8_t srcKey[16]) {
    const uint32_t* const k = (const uint32_t*)srcKey;
    uint32_t* const v = (uint32_t*)srcBytes;
    uint32_t sum = 0xC6EF3720;  // 0xC6EF3720 == CRYPTO_TEA_Delta << 5

    for (int i = 0; i < 32; ++i) {
        v[1] -= ((v[0] << 4) + k[2]) ^ (v[0] + sum) ^ ((v[0] >> 5) + k[3]);
        v[0] -= ((v[1] << 4) + k[0]) ^ (v[1] + sum) ^ ((v[1] >> 5) + k[1]);
        sum -= CRYPTO_TEA_Delta;
    }
}
