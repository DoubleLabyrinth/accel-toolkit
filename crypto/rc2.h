#pragma once
#include <stdint.h>
#include <stddef.h>

#define RC2_SUCCESS 0
#define RC2_INVALID_KEY_LENGTH (-1)
#define RC2_KEY_TOO_LONG (-2)
#define RC2_EFFECTIVE_KEY_TOO_LONG (-3)

#define RC2_MIN_KEY_LENGTH 1
#define RC2_MAX_KEY_LENGTH 128
#define RC2_BLOCK_SIZE 8

#if defined(__cplusplus)
extern "C" {
#endif

    typedef struct _RC2_KEY {
        uint16_t ExpandedKey[64];
    } RC2_KEY;

    void accelc_RC2_encrypt(uint8_t srcBytes[RC2_BLOCK_SIZE], const RC2_KEY* srcKey);
    void accelc_RC2_decrypt(uint8_t srcBytes[RC2_BLOCK_SIZE], const RC2_KEY* srcKey);

    int accelc_RC2_set_key(const uint8_t* srcKey,
                           uint8_t srcKeyLength,
                           uint16_t eftKey_BitLength,
                           RC2_KEY* dstKey);

#if defined(__cplusplus)
}
#endif
