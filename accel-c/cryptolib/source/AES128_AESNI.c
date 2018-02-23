/*********************************************************************
* Filename:   AES128_AESNI.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../AES128.h"
#include <intrin.h>

void CRYPTO_AES128_EncryptBlock_AESNI(uint8_t srcBytes[16], const uint32_t srcExpandedKey[44]) {
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
    buffer = _mm_aesenclast_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 10));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void CRYPTO_AES128_DecryptBlock_AESNI(uint8_t srcBytes[16], const uint32_t srcExpandedKey[44]) {
    __m128i buffer = _mm_lddqu_si128((__m128i*)(srcBytes));

    buffer = _mm_xor_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 10));
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

void CRYPTO_AES128_DecryptBlock_AESNI_Fast(uint8_t srcBytes[16], const uint32_t srcInvExpandedKey[44]) {
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
    buffer = _mm_aesdeclast_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 10));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void CRYPTO_AES128_KeyExpansion_AESNI(const uint8_t srcKey[16], uint32_t dstExpandedKey[44]) {
    __m128i buffer;
    __m128i assist_key;

    buffer = _mm_lddqu_si128((const __m128i*)(srcKey));
    _mm_storeu_si128((__m128i*)(dstExpandedKey), buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x01), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstExpandedKey) + 1, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x02), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstExpandedKey) + 2, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x04), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstExpandedKey) + 3, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x08), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstExpandedKey) + 4, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x10), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstExpandedKey) + 5, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x20), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstExpandedKey) + 6, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x40), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstExpandedKey) + 7, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x80), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstExpandedKey) + 8, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x1B), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstExpandedKey) + 9, buffer);

    assist_key = _mm_shuffle_epi32(_mm_aeskeygenassist_si128(buffer, 0x36), 0xFF);
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, _mm_slli_si128(buffer, 4));
    buffer = _mm_xor_si128(buffer, assist_key);
    _mm_storeu_si128((__m128i*)(dstExpandedKey) + 10, buffer);
}

void CRYPTO_AES128_GetInvExpandedKey_AESNI(const uint32_t srcExpandedKey[44], uint32_t dstInvExpandedKey[44]) {
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey), _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 10));
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
