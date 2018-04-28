#include "../tea.h"

static const uint32_t accelc_TEA_delta = 0x9E3779B9;

void accelc_TEA_encrypt(uint8_t srcBytes[TEA_BLOCK_SIZE], const TEA_KEY* srcKey) {
    uint32_t* const v = (uint32_t*)srcBytes;
    uint32_t sum = 0;

    for (int i = 0; i < 32; ++i) {
        sum += accelc_TEA_delta;
        v[0] += ((v[1] << 4) + srcKey->blocks_u32[0]) ^ (v[1] + sum) ^ ((v[1] >> 5) + srcKey->blocks_u32[1]);
        v[1] += ((v[0] << 4) + srcKey->blocks_u32[2]) ^ (v[0] + sum) ^ ((v[0] >> 5) + srcKey->blocks_u32[3]);
    }
}

void accelc_TEA_decrypt(uint8_t srcBytes[TEA_BLOCK_SIZE], const TEA_KEY* srcKey) {
    uint32_t* const v = (uint32_t*)srcBytes;
    uint32_t sum = 0xC6EF3720;  // 0xC6EF3720 == accelc_TEA_delta << 5

    for (int i = 0; i < 32; ++i) {
        v[1] -= ((v[0] << 4) + srcKey->blocks_u32[2]) ^ (v[0] + sum) ^ ((v[0] >> 5) + srcKey->blocks_u32[3]);
        v[0] -= ((v[1] << 4) + srcKey->blocks_u32[0]) ^ (v[1] + sum) ^ ((v[1] >> 5) + srcKey->blocks_u32[1]);
        sum -= accelc_TEA_delta;
    }
}

void accelc_XTEA_encrypt(uint8_t srcBytes[XTEA_BLOCK_SIZE], const XTEA_KEY* srcKey, uint32_t round_count) {
    uint32_t* const v = (uint32_t*)srcBytes;

    uint32_t sum = 0;
    for (uint32_t i = 0; i < round_count; i++) {
        v[0] += (((v[1] << 4) ^ (v[1] >> 5)) + v[1]) ^ (sum + srcKey->blocks_u32[sum & 3]);
        sum += accelc_TEA_delta;
        v[1] += (((v[0] << 4) ^ (v[0] >> 5)) + v[0]) ^ (sum + srcKey->blocks_u32[(sum >> 11) & 3]);
    }
}

void accelc_XTEA_decrypt(uint8_t srcBytes[XTEA_BLOCK_SIZE], const XTEA_KEY* srcKey, uint32_t round_count) {
    uint32_t* const v = (uint32_t*)srcBytes;

    uint32_t sum = accelc_TEA_delta * round_count;
    for (uint32_t i = 0; i < round_count; i++) {
        v[1] -= (((v[0] << 4) ^ (v[0] >> 5)) + v[0]) ^ (sum + srcKey->blocks_u32[(sum >> 11) & 3]);
        sum -= accelc_TEA_delta;
        v[0] -= (((v[1] << 4) ^ (v[1] >> 5)) + v[1]) ^ (sum + srcKey->blocks_u32[sum & 3]);
    }
}