/*********************************************************************
* Filename:   RC4.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stddef.h>
#include <stdint.h>

#define CRYPTO_RC4_SUCCESS 0
#define CRYPTO_RC4_INVALID_KEY_LENGTH -1
#define CRYPTO_RC4_KEY_TOO_LONG -2

#define CRYPTO_RC4_MIN_KEY_LENGTH 1u
#define CRYPTO_RC4_MAX_KEY_LENGTH 256u

#if defined(__cplusplus)
extern "C" {
#endif

    int CRYPTO_RC4_Encrypt(uint8_t srcBytes[],
                           size_t srcBytesLength,
                           const uint8_t srcKey[],
                           uint16_t srcKeyLength);

    int CRYPTO_RC4_Decrypt(uint8_t srcBytes[],
                           size_t srcBytesLength,
                           const uint8_t srcKey[],
                           uint16_t srcKeyLength);

#if defined(__cplusplus)
}
#endif
