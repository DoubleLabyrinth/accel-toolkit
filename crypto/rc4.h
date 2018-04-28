#pragma once
#include <stddef.h>
#include <stdint.h>

#define CRYPTO_RC4_SUCCESS 0
#define CRYPTO_RC4_INVALID_KEY_LENGTH (-1)
#define CRYPTO_RC4_KEY_TOO_LONG (-2)

#define CRYPTO_RC4_MIN_KEY_LENGTH 1u
#define CRYPTO_RC4_MAX_KEY_LENGTH 256u

#if defined(__cplusplus)
extern "C" {
#endif

    typedef struct _RC4_KEY {
        uint8_t SBox[256];
    } RC4_KEY, RC4_STATE;

    void accelc_RC4_process(uint8_t srcBytes[], size_t srcBytesLength,
                            RC4_STATE* srcState);

    int accelc_RC4_set_key(const uint8_t srcKey[], size_t srcKeyLength,
                           RC4_KEY* dstKey);

    void accelc_RC4_reset(RC4_STATE* srcState, const RC4_KEY* srcKey);

#define accelc_RC4_encrypt accelc_RC4_process
#define accelc_RC4_decrypt accelc_RC4_process

#if defined(__cplusplus)
}
#endif
