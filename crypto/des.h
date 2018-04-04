#pragma once
#include <stdint.h>
#include <stddef.h>

#define DES_SUCCESS 0
#define DES_INVALID_KEY (-1)

#if defined(__cplusplus)
extern "C" {
#endif

#define DES_USERKEY_LENGTH 8
#define DES_BLOCK_SIZE 8

    typedef struct _DES_KEY {
        uint8_t ExpandedKey[16][6];
    } DES_KEY;

    void accelc_DES_encrypt(uint8_t srcBytes[DES_BLOCK_SIZE], const DES_KEY* srcKey);
    void accelc_DES_decrypt(uint8_t srcBytes[DES_BLOCK_SIZE], const DES_KEY* srcKey);

    void accelc_3DES_encrypt(uint8_t srcBytes[DES_BLOCK_SIZE],
                             DES_KEY* srcKey1,
                             DES_KEY* srcKey2,
                             DES_KEY* srcKey3);

    void accelc_3DES_decrypt(uint8_t srcBytes[DES_BLOCK_SIZE],
                             const DES_KEY* srcKey1,
                             const DES_KEY* srcKey2,
                             const DES_KEY* srcKey3);

    //
    // Failed if function return DES_INVALID_KEY.
    // If and only if the number of bit '1' contained in each uint8_t of array srcKey is not odd, the function will return fail.
    // 
    int accelc_DES_set_key(const uint8_t srcUserKey[DES_USERKEY_LENGTH], DES_KEY* dstKey);

#if defined(__cplusplus)
}
#endif
