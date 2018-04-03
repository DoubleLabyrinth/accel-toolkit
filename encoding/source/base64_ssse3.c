#include "../base64.h"

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#endif

extern const char accelc_Base64_Table[64];
extern const uint8_t accelc_Base64_InverseTable[128];

size_t accelc_Base64_Encode_ssse3(const void* __restrict src, size_t len,
                                  char* __restrict out_buf, size_t out_len) {
    if (len == 0)
        return 0;

    size_t minimum_buf_len = 4 * (len / 3 + (len % 3 == 0 ? 0 : 1));
    if (out_len < minimum_buf_len)
        return minimum_buf_len;

    // process 12 bytes at a time
    // convert them to 16-bytes-long chars which just fit a xmm register
    const int32_t(* const src_blocks)[3] = src;
    __m128i* const dst_blocks = (__m128i*)out_buf;
    const size_t blocks_len = len / (3 * sizeof(int32_t));

    for (size_t i = 0; i < blocks_len; ++i) {
        __m128i temp = _mm_set_epi32(0, src_blocks[i][2], src_blocks[i][1], src_blocks[i][0]);

        temp = _mm_shuffle_epi8(temp, _mm_set_epi8(2 + 9, 0 + 9, 1 + 9, 2 + 9,
                                                   2 + 6, 0 + 6, 1 + 6, 2 + 6,
                                                   2 + 3, 0 + 3, 1 + 3, 2 + 3,
                                                   2, 0, 1, 2));

        __m128i fin = _mm_and_si128(temp, _mm_set1_epi32(0x3F000000));

        temp = _mm_srli_epi32(temp, 2);
        fin = _mm_xor_si128(fin, _mm_and_si128(temp, _mm_set1_epi32(0x003F0000)));

        temp = _mm_srli_epi32(temp, 2);
        fin = _mm_xor_si128(fin, _mm_and_si128(temp, _mm_set1_epi32(0x00003F00)));

        temp = _mm_srli_epi32(temp, 2);
        fin = _mm_xor_si128(fin, _mm_and_si128(temp, _mm_set1_epi32(0x0000003F)));

        fin = _mm_shuffle_epi8(fin, _mm_set_epi8(3 + 12, 0 + 12, 1 + 12, 2 + 12,
                                                 3 + 8, 0 + 8, 1 + 8, 2 + 8,
                                                 3 + 4, 0 + 4, 1 + 4, 2 + 4,
                                                 3, 0, 1, 2));

        // 0 ~ 25
        __m128i offset = _mm_shuffle_epi8(_mm_set1_epi8('A'),
                                          _mm_cmpgt_epi8(fin, _mm_set1_epi8(25)));

        // 26 ~ 51
        offset = _mm_xor_si128(offset,
                               _mm_shuffle_epi8(_mm_set1_epi8('a' - 26),
                                                _mm_or_si128(_mm_cmpgt_epi8(fin, _mm_set1_epi8(51)),
                                                             _mm_cmpgt_epi8(_mm_set1_epi8(26), fin))));
        // 52 ~ 61
        offset = _mm_xor_si128(offset,
                               _mm_shuffle_epi8(_mm_set1_epi8('0' - 52),
                                                _mm_or_si128(_mm_cmpgt_epi8(fin, _mm_set1_epi8(61)),
                                                             _mm_cmpgt_epi8(_mm_set1_epi8(52), fin))));
        // 62
        offset = _mm_xor_si128(offset,
                               _mm_shuffle_epi8(_mm_set1_epi8('+' - 62),
                                                _mm_andnot_si128(_mm_cmpeq_epi8(fin, _mm_set1_epi8(62)),
                                                                 _mm_set1_epi8(-1))));
        // 63
        offset = _mm_xor_si128(offset,
                               _mm_shuffle_epi8(_mm_set1_epi8('/' - 63),
                                                _mm_andnot_si128(_mm_cmpeq_epi8(fin, _mm_set1_epi8(63)),
                                                                 _mm_set1_epi8(-1))));

        fin = _mm_add_epi8(fin, offset);
        _mm_storeu_si128(dst_blocks + i, fin);
    }

    size_t left_len = len % (3 * sizeof(int32_t));
    const uint8_t* src_left = (const uint8_t*)(src_blocks + blocks_len);
    char* dst_left = (char*)(dst_blocks + blocks_len);

    for (; left_len >= 3; left_len -= 3, src_left += 3, dst_left += 4) {
        dst_left[0] = accelc_Base64_Table[src_left[0] >> 2];
        dst_left[1] = accelc_Base64_Table[(src_left[0] << 4 | src_left[1] >> 4) & 0x3F];
        dst_left[2] = accelc_Base64_Table[(src_left[1] << 2 | src_left[2] >> 6) & 0x3F];
        dst_left[3] = accelc_Base64_Table[src_left[2] & 0x3F];
    }

    switch (left_len) {
        case 1:
            dst_left[0] = accelc_Base64_Table[src_left[0] >> 2];
            dst_left[1] = accelc_Base64_Table[(src_left[0] << 4) & 0x3F];
            dst_left[2] = '=';
            dst_left[3] = '=';
            break;
        case 2:
            dst_left[0] = accelc_Base64_Table[src_left[0] >> 2];
            dst_left[1] = accelc_Base64_Table[(src_left[0] << 4 | src_left[1] >> 4) & 0x3F];
            dst_left[2] = accelc_Base64_Table[src_left[1] << 2 & 0x3F];
            dst_left[3] = '=';
            break;
        default:
            break;
    }

    return minimum_buf_len;
}

size_t accelc_Base64_Check_ssse3(const char* src, size_t len) {
    if (len == 0 || len % 4 != 0)
        return 0;

    const __m128i* src_blocks = (const __m128i*)src;
    size_t blocks_len = len / sizeof(__m128i);

    size_t i = 0;
    for (; i < blocks_len; ++i) {
        __m128i temp = _mm_lddqu_si128(src_blocks + i);

        __m128i mask = _mm_cmpeq_epi8(temp, _mm_set1_epi8('+'));
        mask = _mm_xor_si128(mask, _mm_cmpeq_epi8(temp, _mm_set1_epi8('/')));
        mask = _mm_xor_si128(mask, _mm_and_si128(_mm_cmpgt_epi8(temp, _mm_set1_epi8('A' - 1)),
                                                 _mm_cmpgt_epi8(_mm_set1_epi8('Z' + 1), temp)));
        mask = _mm_xor_si128(mask, _mm_and_si128(_mm_cmpgt_epi8(temp, _mm_set1_epi8('a' - 1)),
                                                 _mm_cmpgt_epi8(_mm_set1_epi8('z' + 1), temp)));
        mask = _mm_xor_si128(mask, _mm_and_si128(_mm_cmpgt_epi8(temp, _mm_set1_epi8('0' - 1)),
                                                 _mm_cmpgt_epi8(_mm_set1_epi8('9' + 1), temp)));
        if (_mm_movemask_epi8(mask) != 0xFFFF)
            break;
    }

    i *= sizeof(__m128i);

    size_t j = 0;
    for (; j + i < len; ++j) {
        if (src[i + j] >= 'A' && src[i + j] <= 'Z')
            continue;
        if (src[i + j] >= 'a' && src[i + j] <= 'z')
            continue;
        if (src[i + j] >= '0' && src[i + j] <= '9')
            continue;
        if (src[i + j] == '+' || src_blocks[i + j] == '/')
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

size_t accelc_Base64_Decode_ssse3(const char* __restrict src, size_t len,
                                  void* __restrict out_buf, size_t out_len) {
    size_t minimum_buf_len = accelc_Base64_Check_ssse3(src, len);
    if (minimum_buf_len == 0)
        return 0;

    const __m128i* src_blocks = (const __m128i*)src;
    size_t blocks_len = len / sizeof(__m128i);
    if (blocks_len != 0) blocks_len--;
    char (*dst_blocks)[12] = out_buf;

    for (size_t i = 0; i < blocks_len; ++i) {
        __m128i temp = _mm_lddqu_si128(src_blocks + i);

        {   // to raw data split
            __m128i cmp_result;
            __m128i offset;

            cmp_result = _mm_cmpeq_epi8(temp, _mm_set1_epi8('+'));
            offset = _mm_shuffle_epi8(_mm_set1_epi8('+' - 62),
                                      _mm_andnot_si128(cmp_result,
                                                       _mm_set1_epi8(-1)));
            cmp_result = _mm_cmpeq_epi8(temp, _mm_set1_epi8('/'));
            offset = _mm_xor_si128(offset,
                                   _mm_shuffle_epi8(_mm_set1_epi8('/' - 63),
                                                    _mm_andnot_si128(cmp_result,
                                                                     _mm_set1_epi8(-1))));

            cmp_result = _mm_or_si128(_mm_cmpgt_epi8(temp, _mm_set1_epi8('Z')),
                                      _mm_cmpgt_epi8(_mm_set1_epi8('A'), temp));
            offset = _mm_xor_si128(offset,
                                   _mm_shuffle_epi8(_mm_set1_epi8('A'), cmp_result));

            cmp_result = _mm_or_si128(_mm_cmpgt_epi8(temp, _mm_set1_epi8('z')),
                                      _mm_cmpgt_epi8(_mm_set1_epi8('a'), temp));
            offset = _mm_xor_si128(offset,
                                   _mm_shuffle_epi8(_mm_set1_epi8('a' - 26), cmp_result));

            cmp_result = _mm_or_si128(_mm_cmpgt_epi8(temp, _mm_set1_epi8('9')),
                                      _mm_cmpgt_epi8(_mm_set1_epi8('0'), temp));
            offset = _mm_xor_si128(offset,
                                   _mm_shuffle_epi8(_mm_set1_epi8('0' - 52), cmp_result));

            temp = _mm_sub_epi8(temp, offset);
        }

        temp = _mm_shuffle_epi8(temp, _mm_set_epi8(0 + 12, 1 + 12, 2 + 12, 3 + 12,
                                                   0 + 8, 1 + 8, 2 + 8, 3 + 8,
                                                   0 + 4, 1 + 4, 2 + 4, 3 + 4,
                                                   0, 1, 2, 3));
        temp = _mm_xor_si128(_mm_and_si128(temp,
                                           _mm_set1_epi32(0xFF00FF00)),
                             _mm_sll_epi16(_mm_and_si128(temp,
                                                         _mm_set1_epi32(0x00FF00FF)),
                                           2));

        __m128i fin0 = _mm_shuffle_epi8(_mm_srl_epi16(temp, 2), _mm_set_epi8(-1, -1, -1, -1,
                                                                             0 + 12, 1 + 12, -1,
                                                                             0 + 8, 1 + 8, -1,
                                                                             0 + 4, 1 + 4, -1,
                                                                             0, 1, -1));
        __m128i fin1 = _mm_shuffle_epi8(_mm_sll_epi16(temp, 2), _mm_set_epi8(-1, -1, -1, -1,
                                                                             -1, 2 + 12, 3 + 12,
                                                                             -1, 2 + 8, 3 + 8,
                                                                             -1, 2 + 4, 3 + 4,
                                                                             -1, 2, 3));
        _mm_maskmoveu_si128(_mm_xor_si128(fin0, fin1), _mm_set_epi32(0, -1, -1, -1), dst_blocks[i]);
    }

    const char (*src_left_blocks)[4] = (const char (*)[4])(src_blocks + blocks_len);
    uint8_t (*dst_left_blocks)[3] = (uint8_t (*)[3])(dst_blocks + blocks_len);
    size_t left_blocks_len = (len % sizeof(__m128i)) / 4;

    for (size_t i = 0; i < left_blocks_len; ++i){
        if (src_left_blocks[i][2] == '=') {
            dst_left_blocks[i][0] = accelc_Base64_InverseTable[src_left_blocks[i][0]] << 2 | accelc_Base64_InverseTable[src_left_blocks[i][1]] >> 4;
        } else if (src_left_blocks[i][3] == '=') {
            dst_left_blocks[i][0] = accelc_Base64_InverseTable[src_left_blocks[i][0]] << 2 | accelc_Base64_InverseTable[src_left_blocks[i][1]] >> 4;
            dst_left_blocks[i][1] = accelc_Base64_InverseTable[src_left_blocks[i][1]] << 4 | accelc_Base64_InverseTable[src_left_blocks[i][2]] >> 2;
        } else {
            dst_left_blocks[i][0] = accelc_Base64_InverseTable[src_left_blocks[i][0]] << 2 | accelc_Base64_InverseTable[src_left_blocks[i][1]] >> 4;
            dst_left_blocks[i][1] = accelc_Base64_InverseTable[src_left_blocks[i][1]] << 4 | accelc_Base64_InverseTable[src_left_blocks[i][2]] >> 2;
            dst_left_blocks[i][2] = accelc_Base64_InverseTable[src_left_blocks[i][2]] << 6 | accelc_Base64_InverseTable[src_left_blocks[i][3]];
        }
    }

    return minimum_buf_len;
}