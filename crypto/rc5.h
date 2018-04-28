#pragma once
#include <stdint.h>
#include <stddef.h>

#define RC5_16_SUCCESS 0
#define RC5_32_SUCCESS 0
#define RC5_64_SUCCESS 0

#define RC5_16_BUFFER_TOO_SHORT (-1)
#define RC5_32_BUFFER_TOO_SHORT (-1)
#define RC5_64_BUFFER_TOO_SHORT (-1)

#define RC5_16_MIN_KEY_LENGTH 0
#define RC5_32_MIN_KEY_LENGTH 0
#define RC5_64_MIN_KEY_LENGTH 0

#define RC5_16_MAX_KEY_LENGTH 255
#define RC5_32_MAX_KEY_LENGTH 255
#define RC5_64_MAX_KEY_LENGTH 255

#define RC5_16_BLOCK_SIZE 4
#define RC5_32_BLOCK_SIZE 8
#define RC5_64_BLOCK_SIZE 16

#if defined(__cplusplus)
extern "C" {
#endif

    void accelc_RC5_16_encrypt(uint8_t srcBytes[RC5_16_BLOCK_SIZE], const uint16_t ExpandedKey[], uint8_t round_count);
    void accelc_RC5_16_decrypt(uint8_t srcBytes[RC5_16_BLOCK_SIZE], const uint16_t ExpandedKey[], uint8_t round_count);

    // srcKeyLength can be 0x00 to 0xFF.
    // round_count can be 0x00 to 0xFF.
    // The length of dstExpandedKey must be at least 2 * (round_count + 1).
    int accelc_RC5_16_set_key(const uint8_t srcKey[],
                              uint8_t srcKeyLength,
                              uint8_t round_count,
                              uint16_t dstExpandedKey[], size_t dstExpandedKeyBufLength);




    void accelc_RC5_32_encrypt(uint8_t srcBytes[RC5_32_BLOCK_SIZE], const uint32_t srcExpandedKey[], uint8_t round_count);
    void accelc_RC5_32_decrypt(uint8_t srcBytes[RC5_32_BLOCK_SIZE], const uint32_t srcExpandedKey[], uint8_t round_count);

    // srcKeyLength can be 0x00 to 0xFF.
    // round_count can be 0x00 to 0xFF.
    // The length of dstExpandedKey must be at least 2 * (round_count + 1).
    int accelc_RC5_32_set_key(const uint8_t srcKey[],
                              uint8_t srcKeyLength,
                              uint8_t round_count,
                              uint32_t dstExpandedKey[], size_t dstExpandedKeyBufLength);




    void accelc_RC5_64_encrypt(uint8_t srcBytes[RC5_64_BLOCK_SIZE], const uint64_t srcExpandedKey[], uint8_t round_count);
    void accelc_RC5_64_decrypt(uint8_t srcBytes[RC5_64_BLOCK_SIZE], const uint64_t srcExpandedKey[], uint8_t round_count);

    // srcKeyLength can be 0x00 to 0xFF.
    // round_count can be 0x00 to 0xFF.
    // The length of dstExpandedKey must be at least 2 * (round_count + 1).
    int accelc_RC5_64_set_key(const uint8_t srcKey[],
                              uint8_t srcKeyLength,
                              uint8_t round_count,
                              uint64_t dstExpandedKey[], size_t dstExpandedKeyBufLength);

#if defined(__cplusplus)
}
#endif