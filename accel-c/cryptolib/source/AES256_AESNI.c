/*********************************************************************
* Filename:   AES256_AESNI.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../AES256.h"
#include <intrin.h>

void CRYPTO_AES256_EncryptBlock_AESNI(uint8_t srcBytes[16], const uint32_t srcExpandedKey[60]) {
    __m128i buffer = _mm_lddqu_si128((__m128i*)srcBytes);

    buffer = _mm_xor_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey)));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 1));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 2));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 3));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 4));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 5));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 6));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 7));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 8));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 9));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 10));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 11));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 12));
    buffer = _mm_aesenc_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 13));
    buffer = _mm_aesenclast_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 14));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void CRYPTO_AES256_DecryptBlock_AESNI(uint8_t srcBytes[16], const uint32_t srcExpandedKey[60]) {
    __m128i buffer = _mm_lddqu_si128((__m128i*)(srcBytes));

    buffer = _mm_xor_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 14));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 13)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 12)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 11)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 10)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 9)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 8)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 7)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 6)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 5)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 4)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 3)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 2)));
    buffer = _mm_aesdec_si128(buffer, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 1)));
    buffer = _mm_aesdeclast_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey)));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void CRYPTO_AES256_DecryptBlock_AESNI_Fast(uint8_t srcBytes[16], const uint32_t srcInvExpandedKey[60]) {
    __m128i buffer = _mm_lddqu_si128((__m128i*)(srcBytes));

    buffer = _mm_xor_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey)));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 1));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 2));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 3));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 4));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 5));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 6));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 7));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 8));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 9));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 10));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 11));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 12));
    buffer = _mm_aesdec_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 13));
    buffer = _mm_aesdeclast_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 14));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void CRYPTO_AES256_KeyExpansion_AESNI(const uint8_t srcKey[32], uint32_t dstExpandedKey[60]) {
    __m128i buffer_l, buffer_h;
    __m128i assist_key;

    buffer_l = _mm_lddqu_si128((const __m128i*)srcKey);
    buffer_h = _mm_lddqu_si128((const __m128i*)srcKey + 1);
    _mm_storeu_si128((__m128i*)dstExpandedKey, buffer_l);
    _mm_storeu_si128((__m128i*)dstExpandedKey + 1, buffer_h);

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
    _mm_storeu_si128((__m128i*)dstExpandedKey + 2, buffer_l);
    _mm_storeu_si128((__m128i*)dstExpandedKey + 3, buffer_h);

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
    _mm_storeu_si128((__m128i*)dstExpandedKey + 4, buffer_l);
    _mm_storeu_si128((__m128i*)dstExpandedKey + 5, buffer_h);

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
    _mm_storeu_si128((__m128i*)dstExpandedKey + 6, buffer_l);
    _mm_storeu_si128((__m128i*)dstExpandedKey + 7, buffer_h);

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
    _mm_storeu_si128((__m128i*)dstExpandedKey + 8, buffer_l);
    _mm_storeu_si128((__m128i*)dstExpandedKey + 9, buffer_h);

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
    _mm_storeu_si128((__m128i*)dstExpandedKey + 10, buffer_l);
    _mm_storeu_si128((__m128i*)dstExpandedKey + 11, buffer_h);

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
    _mm_storeu_si128((__m128i*)dstExpandedKey + 12, buffer_l);
    _mm_storeu_si128((__m128i*)dstExpandedKey + 13, buffer_h);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer_h, 0x40), _MM_SHUFFLE(3, 3, 3, 3));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, _mm_slli_si128(buffer_l, 4));
    buffer_l = _mm_xor_si128(buffer_l, assist_key);
    _mm_storeu_si128((__m128i*)dstExpandedKey + 14, buffer_l);
}

void CRYPTO_AES256_GetInvExpandedKey_AESNI(const uint32_t srcExpandedKey[60], uint32_t dstInvExpandedKey[60]) {
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey), _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 14));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 1, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 13)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 1, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 12)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 1, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 11)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 1, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 10)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 1, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 9)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 2, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 8)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 3, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 7)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 4, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 6)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 5, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 5)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 6, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 4)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 7, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 3)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 8, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 2)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 9, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 1)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 10, _mm_lddqu_si128((const __m128i*)(srcExpandedKey)));
}
