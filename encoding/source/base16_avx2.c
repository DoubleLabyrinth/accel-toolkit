#include "../base16.h"
#include <x86intrin.h>

extern const char accelc_Base16Table[16];

size_t accelc_Base16_Encode_avx2(const void* __restrict src, size_t len,
                                 char* __restrict out_buf, size_t out_len) {
    if (len == 0)
        return 0;

    size_t minimum_buf_len = 2 * len;
    if (out_len < minimum_buf_len)
        return (size_t)-1;

    const int64_t (* const src_blocks)[2] = src;
    __m256i* const dst_blocks = (__m256i*)out_buf;
    size_t blocks_len = len / (sizeof(int64_t) * 2);

    for (size_t i = 0; i < blocks_len; ++i) {
        __m256i temp = _mm256_set_epi64x(src_blocks[i][1], src_blocks[i][1] >> 4,
                                         src_blocks[i][0], src_blocks[i][0] >> 4);
        temp = _mm256_and_si256(temp, _mm256_set1_epi8(0x0F));
        temp = _mm256_shuffle_epi8(temp, _mm256_set_epi8(15, 7, 14, 6, 13, 5, 12, 4, 11, 3, 10, 2, 9, 1, 8, 0,
                                                         15, 7, 14, 6, 13, 5, 12, 4, 11, 3, 10, 2, 9, 1, 8, 0));
        temp = _mm256_add_epi8(temp, _mm256_shuffle_epi8(_mm256_set1_epi8(7),
                                                         _mm256_cmpgt_epi8(_mm256_set1_epi8(10), temp)));
        temp = _mm256_add_epi8(temp, _mm256_set1_epi8('0'));
        _mm256_storeu_si256(dst_blocks + i, temp);
    }

    char* const dst_left = (char*)out_buf + blocks_len * sizeof(__m256i);
    const uint8_t* const src_left = (const uint8_t*)src + blocks_len * (sizeof(int64_t) * 2);
    size_t left_len = len % (sizeof(int64_t) * 2);

    for(size_t i = 0; i < left_len; ++i) {
        dst_left[2 * i] = accelc_Base16Table[src_left[i] >> 4];
        dst_left[2 * i + 1] = accelc_Base16Table[src_left[i] & 0x0F];
    }

    return minimum_buf_len;
}

int accelc_Base16_Check_avx2(const char* src, size_t len) {
    if (len % 2 != 0)
        return 0;

    const __m256i* blocks = (const __m256i*)src;
    size_t blocks_len = len / sizeof(__m256i);

    for (size_t i = 0; i < blocks_len; ++i) {
        __m256i temp = _mm256_lddqu_si256(blocks + i);

        __m256i probe = _mm256_or_si256(_mm256_cmpgt_epi8(temp, _mm256_set1_epi8('F')),
                                        _mm256_cmpgt_epi8(_mm256_set1_epi8('0'), temp));
        if (_mm256_testz_si256(probe, probe) == 0)
            return 0;

        probe = _mm256_and_si256(_mm256_cmpgt_epi8(temp, _mm256_set1_epi8('9')),
                                 _mm256_cmpgt_epi8(_mm256_set1_epi8('A'), temp));
        if (_mm256_testz_si256(probe, probe) == 0)
            return 0;
    }

    if (len % sizeof(__m256i))
        return accelc_Base16_Check(src + blocks_len * sizeof(__m256i), len % sizeof(__m256i));
    else
        return 1;
}

size_t accelc_Base16_Decode_avx2(const char* __restrict src, size_t len,
                                 void* __restrict out_buf, size_t out_len) {
    if(len == 0 || len % 2 != 0)
        return 0;

    size_t minimum_buf_len = len / 2;
    if (out_len < minimum_buf_len)
        return (size_t)-1;

    const __m256i* const src_blocks = (const __m256i*)src;
    __m128i* const dst_blocks = (__m128i*)out_buf;
    size_t blocks_len = len / sizeof(__m256i);

    for(size_t i = 0; i < blocks_len; ++i) {
        __m256i temp = _mm256_lddqu_si256(src_blocks + i);
        temp = _mm256_sub_epi8(temp, _mm256_set1_epi8('0'));
        temp = _mm256_sub_epi8(temp, _mm256_shuffle_epi8(_mm256_set1_epi8(7),
                                                         _mm256_cmpgt_epi8(_mm256_set1_epi8(10),
                                                                           temp)));
        temp = _mm256_shuffle_epi8(temp, _mm256_set_epi8(15, 13, 11, 9, 7, 5, 3, 1, 14, 12, 10, 8, 6, 4, 2, 0,
                                                         15, 13, 11, 9, 7, 5, 3, 1, 14, 12, 10, 8, 6, 4, 2, 0));
        temp = _mm256_permute4x64_epi64(temp, _MM_SHUFFLE(3, 1, 2, 0));
        temp = _mm256_sllv_epi64(temp, _mm256_set_epi64x(0, 0, 4, 4));
        temp = _mm256_xor_si256(temp,
                                _mm256_permute4x64_epi64(temp,
                                                         _MM_SHUFFLE(3, 2, 3, 2)));
        _mm_storeu_si128(dst_blocks + i, _mm256_castsi256_si128(temp));
    }

    size_t left_len = len % sizeof(__m256i);
    for (size_t i = 0; i < left_len; i += 2) {
        uint8_t temp_h = (uint8_t)(src[blocks_len * sizeof(__m256i) + i] - '0');
        uint8_t temp_l = (uint8_t)(src[blocks_len * sizeof(__m256i) + i + 1] - '0');

        if (temp_h >= 10) temp_h -= 7;
        if (temp_l >= 10) temp_l -= 7;

        ((uint8_t*)out_buf)[blocks_len * sizeof(__m128i) + i / 2] = (temp_h << 4) ^ (temp_l);
    }

    return minimum_buf_len;
}
