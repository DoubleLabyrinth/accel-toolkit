#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define AES_BLOCK_SIZE 16
#define AES128_USERKEY_LENGTH 16
#define AES192_USERKEY_LENGTH 24
#define AES256_USERKEY_LENGTH 32

    typedef struct _AES128_KEY {
        union {
            uint8_t byte[176];
            uint16_t word[88];
            uint32_t dword[44];
            uint64_t qword[22];
        };

    } AES128_KEY;

    typedef struct _AES192_KEY {
        union {
            uint8_t byte[208];
            uint16_t word[104];
            uint32_t dword[52];
            uint64_t qword[26];
        };

    } AES192_KEY;

    typedef struct _AES256_KEY {
        union {
            uint8_t byte[240];
            uint16_t word[120];
            uint32_t dword[60];
            uint64_t qword[30];
        };

    } AES256_KEY;

    //
    //  encrypt
    //
    void accelc_AES128_encrypt(uint8_t srcBytes[AES_BLOCK_SIZE], const AES128_KEY* srcKey);
    void accelc_AES192_encrypt(uint8_t srcBytes[AES_BLOCK_SIZE], const AES192_KEY* srcKey);
    void accelc_AES256_encrypt(uint8_t srcBytes[AES_BLOCK_SIZE], const AES256_KEY* srcKey);

    //
    //  dncrypt
    //
    void accelc_AES128_decrypt(uint8_t srcBytes[AES_BLOCK_SIZE], const AES128_KEY* srcKey);
    void accelc_AES192_decrypt(uint8_t srcBytes[AES_BLOCK_SIZE], const AES192_KEY* srcKey);
    void accelc_AES256_decrypt(uint8_t srcBytes[AES_BLOCK_SIZE], const AES256_KEY* srcKey);

    //
    //  set_key
    //
    void accelc_AES128_set_key(const uint8_t srcUserKey[AES128_USERKEY_LENGTH], AES128_KEY* dstKey);
    void accelc_AES192_set_key(const uint8_t srcUserKey[AES192_USERKEY_LENGTH], AES192_KEY* dstKey);
    void accelc_AES256_set_key(const uint8_t srcUserKey[AES256_USERKEY_LENGTH], AES256_KEY* dstKey);

    //
    //  encrypt_aesni
    //
    void accelc_AES128_encrypt_aesni(uint8_t srcBytes[AES_BLOCK_SIZE], const AES128_KEY* srcKey);
    void accelc_AES192_encrypt_aesni(uint8_t srcBytes[AES_BLOCK_SIZE], const AES192_KEY* srcKey);
    void accelc_AES256_encrypt_aesni(uint8_t srcBytes[AES_BLOCK_SIZE], const AES256_KEY* srcKey);

    //
    //  decrypt_aesni
    //
    void accelc_AES128_decrypt_aesni(uint8_t srcBytes[AES_BLOCK_SIZE], const AES128_KEY* srcKey);
    void accelc_AES192_decrypt_aesni(uint8_t srcBytes[AES_BLOCK_SIZE], const AES192_KEY* srcKey);
    void accelc_AES256_decrypt_aesni(uint8_t srcBytes[AES_BLOCK_SIZE], const AES256_KEY* srcKey);

    //
    //  decrypt_aesni_fast
    //
    void accelc_AES128_decrypt_aesni_fast(uint8_t srcBytes[AES_BLOCK_SIZE], const AES128_KEY* srcInverseKey);
    void accelc_AES192_decrypt_aesni_fast(uint8_t srcBytes[AES_BLOCK_SIZE], const AES192_KEY* srcInverseKey);
    void accelc_AES256_decrypt_aesni_fast(uint8_t srcBytes[AES_BLOCK_SIZE], const AES256_KEY* srcInverseKey);

    //
    //  set_key_aesni
    //
    void accelc_AES128_set_key_aesni(const uint8_t srcUserKey[AES128_USERKEY_LENGTH], AES128_KEY* dstKey);
    void accelc_AES192_set_key_aesni(const uint8_t srcUserKey[AES128_USERKEY_LENGTH], AES192_KEY* dstKey);
    void accelc_AES256_set_key_aesni(const uint8_t srcUserKey[AES256_USERKEY_LENGTH], AES256_KEY* dstKey);

    //
    //  set_invkey_aesni
    //
    void accelc_AES128_set_invkey_aesni(const AES128_KEY* __restrict srcKey, AES128_KEY* __restrict dstInverseKey);
    void accelc_AES192_set_invkey_aesni(const AES192_KEY* __restrict srcKey, AES192_KEY* __restrict dstInverseKey);
    void accelc_AES256_set_invkey_aesni(const AES256_KEY* __restrict srcKey, AES256_KEY* __restrict dstInverseKey);

#if defined(__cplusplus)
}
#endif

