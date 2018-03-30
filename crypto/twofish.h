#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define TWOFISH_BLOCK_SIZE 16
#define TWOFISH128_USERKEY_SIZE 16
#define TWOFISH192_USERKEY_SIZE 24
#define TWOFISH256_USERKEY_SIZE 32

    typedef struct _TWOFISH_KEY {
        uint32_t ExpandedKey[40];
        uint32_t SBox[4];
    } TWOFISH_KEY;

    void accelc_Twofish128_encrypt(uint8_t srcBytes[TWOFISH_BLOCK_SIZE], const TWOFISH_KEY* srcKey);
    void accelc_Twofish128_decrypt(uint8_t srcBytes[TWOFISH_BLOCK_SIZE], const TWOFISH_KEY* srcKey);
    void accelc_Twofish128_set_key(const uint8_t srcKey[TWOFISH128_USERKEY_SIZE], TWOFISH_KEY* dstKey);

    void accelc_Twofish192_encrypt(uint8_t srcBytes[TWOFISH_BLOCK_SIZE], const TWOFISH_KEY* srcKey);
    void accelc_Twofish192_decrypt(uint8_t srcBytes[TWOFISH_BLOCK_SIZE], const TWOFISH_KEY* srcKey);
    void accelc_Twofish192_set_key(const uint8_t srcKey[TWOFISH192_USERKEY_SIZE], TWOFISH_KEY* dstKey);

    void accelc_Twofish256_encrypt(uint8_t srcBytes[TWOFISH_BLOCK_SIZE], const TWOFISH_KEY* srcKey);
    void accelc_Twofish256_decrypt(uint8_t srcBytes[TWOFISH_BLOCK_SIZE], const TWOFISH_KEY* srcKey);
    void accelc_Twofish256_set_key(const uint8_t srcKey[TWOFISH256_USERKEY_SIZE], TWOFISH_KEY* dstKey);

#if defined(__cplusplus)
}
#endif
