#include "../base32.h"

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#endif

extern const uint8_t accelc_Base32_InverseTable[96];

size_t accelc_Base32_Encode_avx2(const void* __restrict src, size_t len,
                                 char* __restrict out_buf, size_t out_len) {

    if (len == 0)
        return 0;

    size_t minimum_buf_len = 8 * (len / 5 + (len % 5 == 0 ? 0 : 1));
    if (out_len < minimum_buf_len)
        return (size_t)-1;

    // process 20 bytes at a time
    // convert them to 32-bytes-long chars which just fit a ymm register
    const int16_t (* const src_blocks)[10] = src;
    __m256i* const dst_blocks = (__m256i*)out_buf;
    const size_t blocks_len = len / (10 * sizeof(int16_t));

    for (size_t i = 0; i < blocks_len; ++i) {
        __m256i temp = _mm256_set_epi16(0, 0, 0,
                                        src_blocks[i][9],
                                        src_blocks[i][8],
                                        src_blocks[i][7],
                                        src_blocks[i][6],
                                        src_blocks[i][5],
                                        0, 0, 0,
                                        src_blocks[i][4],
                                        src_blocks[i][3],
                                        src_blocks[i][2],
                                        src_blocks[i][1],
                                        src_blocks[i][0]);
        temp = _mm256_shuffle_epi8(temp, _mm256_set_epi8(4 + 5, 0 + 5, 1 + 5, 1 + 5, 2 + 5, 3 + 5, 3 + 5, 4 + 5,
                                                         4    , 0    , 1    , 1    , 2    , 3    , 3    , 4    ,
                                                         4 + 5, 0 + 5, 1 + 5, 1 + 5, 2 + 5, 3 + 5, 3 + 5, 4 + 5,
                                                         4    , 0    , 1    , 1    , 2    , 3    , 3    , 4    ));
        __m256i fin = _mm256_and_si256(temp, _mm256_set1_epi64x(0x1F00000000000000));

        temp = _mm256_srli_epi64(temp, 1);
        fin = _mm256_xor_si256(fin, _mm256_and_si256(temp, _mm256_set1_epi64x(0x0000001F00000000)));
        temp = _mm256_srli_epi64(temp, 1);
        fin = _mm256_xor_si256(fin, _mm256_and_si256(temp, _mm256_set1_epi64x(0x0000000000001F00)));
        temp = _mm256_srli_epi64(temp, 1);
        fin = _mm256_xor_si256(fin, _mm256_and_si256(temp, _mm256_set1_epi64x(0x001F000000000000)));
        temp = _mm256_srli_epi64(temp, 1);
        fin = _mm256_xor_si256(fin, _mm256_and_si256(temp, _mm256_set1_epi64x(0x000000001F000000)));
        temp = _mm256_srli_epi64(temp, 1);
        fin = _mm256_xor_si256(fin, _mm256_and_si256(temp, _mm256_set1_epi64x(0x000000000000001F)));
        temp = _mm256_srli_epi64(temp, 1);
        fin = _mm256_xor_si256(fin, _mm256_and_si256(temp, _mm256_set1_epi64x(0x00001F0000000000)));
        temp = _mm256_srli_epi64(temp, 1);
        fin = _mm256_xor_si256(fin, _mm256_and_si256(temp, _mm256_set1_epi64x(0x00000000001F0000)));

        fin = _mm256_shuffle_epi8(fin, _mm256_set_epi8(7 + 8, 0 + 8, 1 + 8, 2 + 8, 3 + 8, 4 + 8, 5 + 8, 6 + 8,
                                                       7    , 0    , 1    , 2    , 3    , 4    , 5    , 6    ,
                                                       7 + 8, 0 + 8, 1 + 8, 2 + 8, 3 + 8, 4 + 8, 5 + 8, 6 + 8,
                                                       7    , 0    , 1    , 2    , 3    , 4    , 5    , 6    ));

        __m256i offset = _mm256_blendv_epi8(_mm256_set1_epi8('A'), _mm256_set1_epi8('2' - 26),
                                            _mm256_cmpgt_epi8(fin, _mm256_set1_epi8(25)));

        fin = _mm256_add_epi8(fin, offset);
        _mm256_storeu_si256(dst_blocks + i, fin);
    }

    accelc_Base32_Encode(src_blocks[blocks_len],
                         len % (10 * sizeof(int16_t)),
                         (char*)(dst_blocks + blocks_len),
                         out_len - blocks_len * sizeof(__m256i));
    return minimum_buf_len;
}

size_t accelc_Base32_Check_avx2(const char* src, size_t len) {
    if (len == 0 || len % 8 != 0)
        return 0;

    const __m256i* src_blocks = (const __m256i*)src;
    size_t blocks_len = len / sizeof(__m256i);
    if (blocks_len != 0) blocks_len--;

    size_t i = 0;
    for (; i < blocks_len; ++i) {
        __m256i temp = _mm256_lddqu_si256(src_blocks + i);

        __m256i mask = _mm256_and_si256(_mm256_cmpgt_epi8(_mm256_set1_epi8('Z' + 1), temp),
                                        _mm256_cmpgt_epi8(temp, _mm256_set1_epi8('A' - 1)));
        mask = _mm256_xor_si256(mask, _mm256_and_si256(_mm256_cmpgt_epi8(_mm256_set1_epi8('7' + 1), temp),
                                                       _mm256_cmpgt_epi8(temp, _mm256_set1_epi8('2' - 1))));
        if (_mm256_movemask_epi8(mask) != 0xFFFFFFFF)
            return 0;
    }

    i *= sizeof(__m256i);

    size_t j = 0;
    for (; j + i < len; ++j) {
        if (src[i + j] >= 'A' && src[i + j] <= 'Z')
            continue;
        if (src[i + j] >= '2' && src[i + j] <= '7')
            continue;
        break;
    }

    i += j;
    j = 0;
    for (; j + i < len; ++j)
        if (src[i + j] != '=')
            return 0;

    switch (j) {
        case 0:
            return (i / 4) * 3;
        case 1:
            return (i / 4) * 3 + 2;
        case 2:
            return (i / 4) * 3 + 1;
        default:
            return 0;
    }
}

size_t accelc_Base32_Decode_avx2(const char* __restrict src, size_t len,
                                 void* __restrict out_buf, size_t out_len) {
    size_t minimum_buf_len = accelc_Base32_Check_avx2(src, len);
    if (minimum_buf_len == 0)
        return 0;

    if (out_len < minimum_buf_len)
        return minimum_buf_len;

    const __m256i* src_blocks = (const __m256i*)src;
    size_t blocks_len = len / sizeof(__m256i);
    if (blocks_len != 0) blocks_len--;
    char (*dst_blocks)[20] = out_buf;

    for (size_t i = 0; i < blocks_len; ++i) {
        __m256i temp = _mm256_lddqu_si256(src_blocks + i);

        {
            __m256i offset;
            offset = _mm256_blendv_epi8(_mm256_set1_epi8('2' - 26), _mm256_set1_epi8('A'),
                                        _mm256_cmpgt_epi8(temp, _mm256_set1_epi8('A' - 1)));
            temp = _mm256_sub_epi8(temp, offset);
        }

        temp = _mm256_xor_si256(_mm256_and_si256(temp, _mm256_set1_epi16(0x00FF)),
                                _mm256_and_si256(_mm256_slli_epi16(temp, 3), _mm256_set1_epi16(0xFF00)));
        temp = _mm256_shuffle_epi8(temp, _mm256_set_epi8(6 + 8, 7 + 8, 4 + 8, 5 + 8, 2 + 8, 3 + 8, 0 + 8, 1 + 8,
                                                         6, 7, 4, 5, 2, 3, 0, 1,
                                                         6 + 8, 7 + 8, 4 + 8, 5 + 8, 2 + 8, 3 + 8, 0 + 8, 1 + 8,
                                                         6, 7, 4, 5, 2, 3, 0, 1));

        __m256i fin = _mm256_and_si256(_mm256_slli_epi16(temp, 3), _mm256_set1_epi64x(0x000000000000FFFF));
        fin = _mm256_xor_si256(fin, _mm256_and_si256(_mm256_slli_epi16(temp, 1), _mm256_set1_epi64x(0x00000000FFFF0000)));
        fin = _mm256_xor_si256(fin, _mm256_and_si256(_mm256_slli_epi16(temp, 1), _mm256_set1_epi64x(0x0000FFFF00000000)));
        fin = _mm256_xor_si256(fin, _mm256_and_si256(_mm256_slli_epi16(temp, 3), _mm256_set1_epi64x(0xFFFF000000000000)));

        temp = _mm256_shuffle_epi8(temp, _mm256_set_epi8(6 + 8, 7 + 8, 4 + 8, 5 + 8, 2 + 8, 3 + 8, 0 + 8, 1 + 8,
                                                         6, 7, 4, 5, 2, 3, 0, 1,
                                                         6 + 8, 7 + 8, 4 + 8, 5 + 8, 2 + 8, 3 + 8, 0 + 8, 1 + 8,
                                                         6, 7, 4, 5, 2, 3, 0, 1));
        fin = _mm256_and_si256(temp, _mm256_set1_epi64x(0x000000000000FFFF));
        fin = _mm256_xor_si256(fin, _mm256_srli_epi64(_mm256_and_si256(temp, _mm256_set1_epi64x(0x00000000FFFF0000)), 8));
        fin = _mm256_xor_si256(fin, _mm256_srli_epi64(_mm256_and_si256(temp, _mm256_set1_epi64x(0x0000FFFF00000000)), 16));
        fin = _mm256_xor_si256(fin, _mm256_srli_epi64(_mm256_and_si256(temp, _mm256_set1_epi64x(0xFFFF000000000000)), 24));

        fin = _mm256_shuffle_epi8(fin, _mm256_set_epi8(0, 0, 0, 0, 0, 0, 12, 11, 10, 9, 8, 4, 3, 2 , 1, 0,
                                                       0, 0, 0, 0, 0, 0, 12, 11, 10, 9, 8, 4, 3, 2 , 1, 0));

        _mm_maskmoveu_si128(_mm256_extracti128_si256(fin, 0),
                            _mm_set_epi8(0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1),
                            dst_blocks[i]);
        _mm_maskmoveu_si128(_mm256_extracti128_si256(fin, 1),
                            _mm_set_epi8(0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1),
                            dst_blocks[i] + 10);
    }

    const char (*src_left_blocks)[8] = (const char (*)[8])(src_blocks + blocks_len);
    uint8_t (*dst_left_blocks)[5] = (uint8_t (*)[5])(dst_blocks + blocks_len);
    size_t left_blocks_len = (len - blocks_len * sizeof(__m256i)) / 8;

    for (size_t i = 0; i < left_blocks_len; ++i) {
        int padding;
        if (src_left_blocks[i][2] == '=')
            padding = 6;
        else if (src_left_blocks[i][4] == '=')
            padding = 4;
        else if (src_left_blocks[i][5] == '=')
            padding = 3;
        else if (src_left_blocks[i][7] == '=')
            padding = 1;
        else
            padding = 0;

        switch (padding) {
            case 0:
                dst_left_blocks[blocks_len][4] = accelc_Base32_InverseTable[src_left_blocks[i][6]] << 5 ^
                                                 accelc_Base32_InverseTable[src_left_blocks[i][7]];
            case 1:
                dst_left_blocks[blocks_len][3] = accelc_Base32_InverseTable[src_left_blocks[i][4]] << 7 ^
                                                 accelc_Base32_InverseTable[src_left_blocks[i][5]] << 2 ^
                                                 accelc_Base32_InverseTable[src_left_blocks[i][6]] >> 3;
            case 3:
                dst_left_blocks[blocks_len][2] = accelc_Base32_InverseTable[src_left_blocks[i][3]] << 4 ^
                                                 accelc_Base32_InverseTable[src_left_blocks[i][4]] >> 1;
            case 4:
                dst_left_blocks[blocks_len][1] = accelc_Base32_InverseTable[src_left_blocks[i][1]] << 6 ^
                                                 accelc_Base32_InverseTable[src_left_blocks[i][2]] << 1 ^
                                                 accelc_Base32_InverseTable[src_left_blocks[i][3]] >> 4;
            case 6:
                dst_left_blocks[blocks_len][0] = accelc_Base32_InverseTable[src_left_blocks[i][0]] << 3 ^
                                                 accelc_Base32_InverseTable[src_left_blocks[i][1]] >> 2;
            default:
                break;
        }
    }

    return minimum_buf_len;
}