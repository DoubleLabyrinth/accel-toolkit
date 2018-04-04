#pragma once
#include <stdint.h>
#include <stddef.h>

#define CAST128_SUCCESS 0
#define CAST128_USERKEY_TOO_SHORT (-1)
#define CAST128_USERKEY_TOO_LONG (-2)

#define CAST128_MIN_USERKEY_LENGTH 5
#define CAST128_MAX_USERKEY_LENGTH 16

#if defined(__cplusplus)
extern "C" {
#endif

#define CAST128_BLOCK_SIZE 8

    typedef struct _CAST128_KEY {
        uint32_t Km[16];
        uint32_t Kr[16];
        uint8_t UserKeyLength;
    } CAST128_KEY;

    void accelc_CAST128_encrypt(uint8_t srcBytes[CAST128_BLOCK_SIZE], const CAST128_KEY* srcKey);

    void accelc_CAST128_decrypt(uint8_t srcBytes[CAST128_BLOCK_SIZE], const CAST128_KEY* srcKey);

    int accelc_CAST128_set_key(const uint8_t* srcUserKey, uint8_t srcUserKeyLength, CAST128_KEY* dstKey);

#if defined(__cplusplus)
}
#endif
