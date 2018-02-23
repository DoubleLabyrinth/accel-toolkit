/*********************************************************************
* Filename:   AES192_AESNI_AVX2.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../AES192.h"
#include <intrin.h>

void CRYPTO_AES192_KeyExpansion_AESNI_AVX2(const uint8_t srcKey[24], uint32_t dstExpandedKey[52]) {
    const __m256i load_store_mask = _mm256_set_epi64x(0, 0x8000000000000000, 0x8000000000000000, 0x8000000000000000);
    __m256i buffer;
    __m256i assist_key;
    
    buffer = _mm256_maskload_epi64((const __int64*)srcKey, load_store_mask);
    _mm256_maskstore_epi64((__int64*)dstExpandedKey, buffer, load_store_mask);

    assist_key = _mm256_broadcastsi128_si256(_mm_shuffle_epi32(_mm_aeskeygenassist_si128(_mm256_extracti128_si256(buffer, 1), 0x01), _MM_SHUFFLE(1, 1, 1, 1)));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(buffer, buffer, 0x08), _mm256_set_epi32(7, 7, 7, 7, 0, 0, 0, 0)));
    buffer = _mm256_xor_si256(buffer, assist_key);
    _mm256_maskstore_epi64((__int64*)(dstExpandedKey + 6), buffer, load_store_mask);

    assist_key = _mm256_broadcastsi128_si256(_mm_shuffle_epi32(_mm_aeskeygenassist_si128(_mm256_extracti128_si256(buffer, 1), 0x02), _MM_SHUFFLE(1, 1, 1, 1)));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(buffer, buffer, 0x08), _mm256_set_epi32(7, 7, 7, 7, 0, 0, 0, 0)));
    buffer = _mm256_xor_si256(buffer, assist_key);
    _mm256_maskstore_epi64((__int64*)(dstExpandedKey + 12), buffer, load_store_mask);

    assist_key = _mm256_broadcastsi128_si256(_mm_shuffle_epi32(_mm_aeskeygenassist_si128(_mm256_extracti128_si256(buffer, 1), 0x04), _MM_SHUFFLE(1, 1, 1, 1)));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(buffer, buffer, 0x08), _mm256_set_epi32(7, 7, 7, 7, 0, 0, 0, 0)));
    buffer = _mm256_xor_si256(buffer, assist_key);
    _mm256_maskstore_epi64((__int64*)(dstExpandedKey + 18), buffer, load_store_mask);

    assist_key = _mm256_broadcastsi128_si256(_mm_shuffle_epi32(_mm_aeskeygenassist_si128(_mm256_extracti128_si256(buffer, 1), 0x08), _MM_SHUFFLE(1, 1, 1, 1)));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(buffer, buffer, 0x08), _mm256_set_epi32(7, 7, 7, 7, 0, 0, 0, 0)));
    buffer = _mm256_xor_si256(buffer, assist_key);
    _mm256_maskstore_epi64((__int64*)(dstExpandedKey + 24), buffer, load_store_mask);

    assist_key = _mm256_broadcastsi128_si256(_mm_shuffle_epi32(_mm_aeskeygenassist_si128(_mm256_extracti128_si256(buffer, 1), 0x10), _MM_SHUFFLE(1, 1, 1, 1)));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(buffer, buffer, 0x08), _mm256_set_epi32(7, 7, 7, 7, 0, 0, 0, 0)));
    buffer = _mm256_xor_si256(buffer, assist_key);
    _mm256_maskstore_epi64((__int64*)(dstExpandedKey + 30), buffer, load_store_mask);

    assist_key = _mm256_broadcastsi128_si256(_mm_shuffle_epi32(_mm_aeskeygenassist_si128(_mm256_extracti128_si256(buffer, 1), 0x20), _MM_SHUFFLE(1, 1, 1, 1)));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(buffer, buffer, 0x08), _mm256_set_epi32(7, 7, 7, 7, 0, 0, 0, 0)));
    buffer = _mm256_xor_si256(buffer, assist_key);
    _mm256_maskstore_epi64((__int64*)(dstExpandedKey + 36), buffer, load_store_mask);

    assist_key = _mm256_broadcastsi128_si256(_mm_shuffle_epi32(_mm_aeskeygenassist_si128(_mm256_extracti128_si256(buffer, 1), 0x40), _MM_SHUFFLE(1, 1, 1, 1)));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_permutevar8x32_epi32(_mm256_permute2x128_si256(buffer, buffer, 0x08), _mm256_set_epi32(7, 7, 7, 7, 0, 0, 0, 0)));
    buffer = _mm256_xor_si256(buffer, assist_key);
    _mm256_maskstore_epi64((__int64*)(dstExpandedKey + 42), buffer, load_store_mask);

    assist_key = _mm256_broadcastsi128_si256(_mm_shuffle_epi32(_mm_aeskeygenassist_si128(_mm256_extracti128_si256(buffer, 1), 0x80), _MM_SHUFFLE(1, 1, 1, 1)));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, _mm256_slli_si256(buffer, 4));
    buffer = _mm256_xor_si256(buffer, assist_key);
    _mm256_maskstore_epi64((__int64*)(dstExpandedKey + 48), buffer, _mm256_set_epi64x(0, 0, 0x8000000000000000, 0x8000000000000000));
}
