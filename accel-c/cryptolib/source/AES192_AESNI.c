/*********************************************************************
* Filename:   AES192_AESNI.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../AES192.h"
#include <intrin.h>

void CRYPTO_AES192_EncryptBlock_AESNI(uint8_t srcBytes[16], const uint32_t srcExpandedKey[52]) {
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
    buffer = _mm_aesenclast_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 12));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void CRYPTO_AES192_DecryptBlock_AESNI(uint8_t srcBytes[16], const uint32_t srcExpandedKey[52]) {
    __m128i buffer = _mm_lddqu_si128((__m128i*)(srcBytes));

    buffer = _mm_xor_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 12));
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

void CRYPTO_AES192_DecryptBlock_AESNI_Fast(uint8_t srcBytes[16], const uint32_t srcInvExpandedKey[52]) {
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
    buffer = _mm_aesdeclast_si128(buffer, _mm_lddqu_si128((const __m128i*)(srcInvExpandedKey) + 12));

    _mm_storeu_si128((__m128i*)(srcBytes), buffer);
}

void CRYPTO_AES192_GetInvExpandedKey_AESNI(const uint32_t srcExpandedKey[52], uint32_t dstInvExpandedKey[52]) {
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey), _mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 12));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 1, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 11)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 2, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 10)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 3, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 9)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 4, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 8)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 5, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 7)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 6, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 6)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 7, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 5)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 8, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 4)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 9, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 3)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 10, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 2)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 11, _mm_aesimc_si128(_mm_lddqu_si128((const __m128i*)(srcExpandedKey) + 1)));
    _mm_storeu_si128((__m128i*)(dstInvExpandedKey) + 12, _mm_lddqu_si128((const __m128i*)(srcExpandedKey)));
}