#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define TEA_BLOCK_SIZE 8
#define XTEA_BLOCK_SIZE 8

    typedef struct _TEA_KEY {
        union {
            uint8_t bytes[16];
            uint32_t blocks_u32[4];
        };
    } TEA_KEY, XTEA_KEY;

    //
    //  encrypt
    //
    void accelc_TEA_encrypt(uint8_t srcBytes[TEA_BLOCK_SIZE], const TEA_KEY* srcKey);
    void accelc_XTEA_encrypt(uint8_t srcBytes[XTEA_BLOCK_SIZE], const XTEA_KEY* srcKey, uint32_t round_count);

    //
    //  decrypt
    //
    void accelc_TEA_decrypt(uint8_t srcBytes[TEA_BLOCK_SIZE], const TEA_KEY* srcKey);
    void accelc_XTEA_decrypt(uint8_t srcBytes[XTEA_BLOCK_SIZE], const XTEA_KEY* srcKey, uint32_t round_count);

#if defined(__cplusplus)
}
#endif