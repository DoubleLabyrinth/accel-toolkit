#include "../aes.h"

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#endif

void accelc_AES128_encrypt_aesni(uint8_t srcBytes[AES_BLOCK_SIZE], const AES_KEY* srcKey) {
    __m128i buffer = _mm_loadu_si128((__m128i*)srcBytes);

    buffer = _mm_xor_si128(buffer, _mm_loadu_si128((const __m128i*)(srcKey)));
    buffer = _mm_aesenc_si128(buffer, _mm_loadu_si128((const __m128i*)(srcKey) + 1));
    buffer = _mm_aesenc_si128(buffer, _mm_loadu_si128((const __m128i*)(srcKey) + 2));
    buffer = _mm_aesenc_si128(buffer, _mm_loadu_si128((const __m128i*)(srcKey) + 3));
    buffer = _mm_aesenc_si128(buffer, _mm_loadu_si128((const __m128i*)(srcKey) + 4));
    buffer = _mm_aesenc_si128(buffer, _mm_loadu_si128((const __m128i*)(srcKey) + 5));
    buffer = _mm_aesenc_si128(buffer, _mm_loadu_si128((const __m128i*)(srcKey) + 6));
    buffer = _mm_aesenc_si128(buffer, _mm_loadu_si128((const __m128i*)(srcKey) + 7));
    buffer = _mm_aesenc_si128(buffer, _mm_loadu_si128((const __m128i*)(srcKey) + 8));
    buffer = _mm_aesenc_si128(buffer, _mm_loadu_si128((const __m128i*)(srcKey) + 9));
    buffer = _mm_aesenclast_si128(buffer, _mm_loadu_si128((const __m128i*)(srcKey) + 10));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void accelc_AES128_decrypt_aesni(uint8_t srcBytes[AES_BLOCK_SIZE], const AES_KEY* srcKey) {
    __m128i buffer = _mm_loadu_si128((__m128i*)(srcBytes));

    buffer = _mm_xor_si128(buffer, _mm_loadu_si128((const __m128i*)(srcKey) + 10));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 9)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 8)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 7)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 6)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 5)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 4)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 3)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 2)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 1)));
    buffer = _mm_aesdeclast_si128(buffer, _mm_loadu_si128((const __m128i*)(srcKey)));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void accelc_AES128_decrypt_aesni_fast(uint8_t srcBytes[AES_BLOCK_SIZE], const AES_KEY* srcInverseKey) {
    __m128i buffer = _mm_loadu_si128((__m128i*)(srcBytes));

    buffer = _mm_xor_si128(buffer, _mm_loadu_si128((const __m128i*)(srcInverseKey)));
    buffer = _mm_aesdec_si128(buffer, _mm_loadu_si128((const __m128i*)(srcInverseKey) + 1));
    buffer = _mm_aesdec_si128(buffer, _mm_loadu_si128((const __m128i*)(srcInverseKey) + 2));
    buffer = _mm_aesdec_si128(buffer, _mm_loadu_si128((const __m128i*)(srcInverseKey) + 3));
    buffer = _mm_aesdec_si128(buffer, _mm_loadu_si128((const __m128i*)(srcInverseKey) + 4));
    buffer = _mm_aesdec_si128(buffer, _mm_loadu_si128((const __m128i*)(srcInverseKey) + 5));
    buffer = _mm_aesdec_si128(buffer, _mm_loadu_si128((const __m128i*)(srcInverseKey) + 6));
    buffer = _mm_aesdec_si128(buffer, _mm_loadu_si128((const __m128i*)(srcInverseKey) + 7));
    buffer = _mm_aesdec_si128(buffer, _mm_loadu_si128((const __m128i*)(srcInverseKey) + 8));
    buffer = _mm_aesdec_si128(buffer, _mm_loadu_si128((const __m128i*)(srcInverseKey) + 9));
    buffer = _mm_aesdeclast_si128(buffer, _mm_loadu_si128((const __m128i*)(srcInverseKey) + 10));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void accelc_AES128_set_key_aesni(const uint8_t srcUserKey[AES128_USERKEY_LENGTH], AES_KEY* dstKey) {
    __m128i buffer;
    __m128i assist_key;

    buffer = _mm_loadu_si128((const __m128i*)(srcUserKey));
    _mm_storeu_si128((__m128i*)(dstKey), buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x01), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstKey) + 1, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x02), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstKey) + 2, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x04), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstKey) + 3, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x08), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstKey) + 4, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x10), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstKey) + 5, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x20), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstKey) + 6, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x40), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstKey) + 7, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x80), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstKey) + 8, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x1B), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstKey) + 9, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x36), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstKey) + 10, buffer);
}

void accelc_AES128_set_invkey_aesni(const AES_KEY* __restrict srcKey, AES_KEY* __restrict dstInverseKey) {
    _mm_storeu_si128((__m128i*)(dstInverseKey), _mm_loadu_si128((const __m128i*)(srcKey) + 10));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 1, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 9)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 2, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 8)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 3, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 7)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 4, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 6)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 5, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 5)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 6, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 4)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 7, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 3)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 8, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 2)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 9, _mm_aesimc_si128(_mm_loadu_si128((const __m128i*)(srcKey) + 1)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 10, _mm_loadu_si128((const __m128i*)(srcKey)));
}



void accelc_AES192_encrypt_aesni(uint8_t srcBytes[AES_BLOCK_SIZE], const AES_KEY* srcKey) {
    __m128i buffer = _mm_lddqu_si128((__m128i*)srcBytes);

    buffer = _mm_xor_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey)));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 1));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 2));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 3));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 4));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 5));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 6));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 7));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 8));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 9));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 10));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 11));
    buffer = _mm_aesenclast_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 12));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void accelc_AES192_decrypt_aesni(uint8_t srcBytes[AES_BLOCK_SIZE], const AES_KEY* srcKey) {
    __m128i buffer = _mm_lddqu_si128((__m128i*)(srcBytes));

    buffer = _mm_xor_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 12));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 11)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 10)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 9)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 8)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 7)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 6)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 5)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 4)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 3)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 2)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 1)));
    buffer = _mm_aesdeclast_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey)));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void accelc_AES192_decrypt_aesni_fast(uint8_t srcBytes[AES_BLOCK_SIZE], const AES_KEY* srcInverseKey) {
    __m128i buffer = _mm_lddqu_si128((__m128i*)(srcBytes));

    buffer = _mm_xor_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey)));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 1));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 2));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 3));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 4));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 5));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 6));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 7));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 8));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 9));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 10));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 11));
    buffer = _mm_aesdeclast_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 12));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void accelc_AES192_set_invkey_aesni(const AES_KEY* __restrict srcKey, AES_KEY* __restrict dstInverseKey) {
    _mm_storeu_si128((__m128i*)(dstInverseKey), _mm_lddqu_si128((const __m128i*)(srcKey) + 12));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 1, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 11)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 2, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 10)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 3, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 9)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 4, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 8)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 5, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 7)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 6, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 6)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 7, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 5)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 8, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 4)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 9, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 3)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 10, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 2)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 11, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 1)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 12, _mm_lddqu_si128((const __m128i*)(srcKey)));
}



void accelc_AES256_encrypt_aesni(uint8_t srcBytes[AES_BLOCK_SIZE], const AES_KEY* srcKey) {
    __m128i buffer = _mm_lddqu_si128((__m128i*)srcBytes);

    buffer = _mm_xor_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey)));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 1));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 2));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 3));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 4));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 5));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 6));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 7));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 8));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 9));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 10));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 11));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 12));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 13));
    buffer = _mm_aesenclast_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 14));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void accelc_AES256_decrypt_aesni(uint8_t srcBytes[AES_BLOCK_SIZE], const AES_KEY* srcKey) {
    __m128i buffer = _mm_lddqu_si128((__m128i*)(srcBytes));

    buffer = _mm_xor_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey) + 14));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 13)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 12)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 11)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 10)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 9)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 8)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 7)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 6)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 5)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 4)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 3)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 2)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 1)));
    buffer = _mm_aesdeclast_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcKey)));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void accelc_AES256_decrypt_aesni_fast(uint8_t srcBytes[AES_BLOCK_SIZE], const AES_KEY* srcInverseKey) {
    __m128i buffer = _mm_lddqu_si128((__m128i*)(srcBytes));

    buffer = _mm_xor_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey)));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 1));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 2));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 3));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 4));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 5));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 6));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 7));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 8));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 9));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 10));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 11));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 12));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 13));
    buffer = _mm_aesdeclast_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInverseKey) + 14));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void accelc_AES256_set_key_aesni(const uint8_t srcUserKey[AES256_USERKEY_LENGTH], AES_KEY* dstKey) {
    __m128i buffer_l, buffer_h;
    __m128i assist_key;

    buffer_l = _mm_lddqu_si128((const __m128i*)srcUserKey);
    buffer_h = _mm_lddqu_si128((const __m128i*)srcUserKey + 1);
    _mm_storeu_si128((__m128i*)dstKey, buffer_l);
    _mm_storeu_si128((__m128i*)dstKey + 1, buffer_h);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x01), _MM_SHUFFLE(3, 3, 3, 3));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, assist_key);
    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x01), _MM_SHUFFLE(2, 2, 2, 2));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, assist_key);
    _mm_storeu_si128((__m128i*)dstKey + 2, buffer_l);
    _mm_storeu_si128((__m128i*)dstKey + 3, buffer_h);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x02), _MM_SHUFFLE(3, 3, 3, 3));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, assist_key);
    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x02), _MM_SHUFFLE(2, 2, 2, 2));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, assist_key);
    _mm_storeu_si128((__m128i*)dstKey + 4, buffer_l);
    _mm_storeu_si128((__m128i*)dstKey + 5, buffer_h);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x04), _MM_SHUFFLE(3, 3, 3, 3));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, assist_key);
    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x04), _MM_SHUFFLE(2, 2, 2, 2));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, assist_key);
    _mm_storeu_si128((__m128i*)dstKey + 6, buffer_l);
    _mm_storeu_si128((__m128i*)dstKey + 7, buffer_h);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x08), _MM_SHUFFLE(3, 3, 3, 3));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, assist_key);
    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x08), _MM_SHUFFLE(2, 2, 2, 2));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, assist_key);
    _mm_storeu_si128((__m128i*)dstKey + 8, buffer_l);
    _mm_storeu_si128((__m128i*)dstKey + 9, buffer_h);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x10), _MM_SHUFFLE(3, 3, 3, 3));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, assist_key);
    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x10), _MM_SHUFFLE(2, 2, 2, 2));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, assist_key);
    _mm_storeu_si128((__m128i*)dstKey + 10, buffer_l);
    _mm_storeu_si128((__m128i*)dstKey + 11, buffer_h);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x20), _MM_SHUFFLE(3, 3, 3, 3));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, assist_key);
    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x20), _MM_SHUFFLE(2, 2, 2, 2));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, _mm_slli_si128(buffer_h, 4));
    buffer_h = _mm_xor_si128(buffer_h, assist_key);
    _mm_storeu_si128((__m128i*)dstKey + 12, buffer_l);
    _mm_storeu_si128((__m128i*)dstKey + 13, buffer_h);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x40), _MM_SHUFFLE(3, 3, 3, 3));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, assist_key);
    _mm_storeu_si128((__m128i*)dstKey + 14, buffer_l);
}

void accelc_AES256_set_invkey_aesni(const AES_KEY* __restrict srcKey, AES_KEY* __restrict dstInverseKey) {
    _mm_storeu_si128((__m128i*)(dstInverseKey), _mm_lddqu_si128((const __m128i*)(srcKey) + 14));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 1, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 13)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 1, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 12)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 1, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 11)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 1, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 10)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 1, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 9)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 2, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 8)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 3, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 7)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 4, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 6)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 5, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 5)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 6, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 4)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 7, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 3)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 8, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 2)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 9, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcKey) + 1)));
    _mm_storeu_si128((__m128i*)(dstInverseKey) + 10, _mm_lddqu_si128((const __m128i*)(srcKey)));
}
