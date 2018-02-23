/*********************************************************************
* Filename:   XTEA.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../XTEA.h"

const uint32_t CRYPTO_XTEA_Delta = 0x9E3779B9;

void CRYPTO_XTEA_EncryptBlock(uint8_t srcBytes[8], const uint8_t srcKey[16], uint32_t round_count) {
    uint32_t* const v = (uint32_t*)srcBytes;
    const uint32_t* const k = (const uint32_t*)srcKey;

    uint32_t sum = 0;
    for (uint32_t i = 0; i < round_count; i++) {
        v[0] += (((v[1] << 4) ^ (v[1] >> 5)) + v[1]) ^ (sum + k[sum & 3]);
        sum += CRYPTO_XTEA_Delta;
        v[1] += (((v[0] << 4) ^ (v[0] >> 5)) + v[0]) ^ (sum + k[(sum >> 11) & 3]);
    }
}

void CRYPTO_XTEA_DecryptBlock(uint8_t srcBytes[8], const uint8_t srcKey[16], uint32_t round_count) {
    uint32_t* const v = (uint32_t*)srcBytes;
    const uint32_t* const k = (const uint32_t*)srcKey;

    uint32_t sum = CRYPTO_XTEA_Delta * round_count;
    for (uint32_t i = 0; i < round_count; i++) {
        v[1] -= (((v[0] << 4) ^ (v[0] >> 5)) + v[0]) ^ (sum + k[(sum >> 11) & 3]);
        sum -= CRYPTO_XTEA_Delta;
        v[0] -= (((v[1] << 4) ^ (v[1] >> 5)) + v[1]) ^ (sum + k[sum & 3]);
    }
}
