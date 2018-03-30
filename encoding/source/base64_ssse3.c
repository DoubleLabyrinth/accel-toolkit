#include "../base64.h"
#include <x86intrin.h>
#include <memory.h>

extern const char accelc_Base64Table[64];

size_t accelc_Base64Encode_ssse3(const void* __restrict src, size_t len,
                                 char* __restrict out_buf, size_t out_len) {
    if (len == 0)
        return (size_t) -1;

    size_t minimum_buf_len = 4 * (len / 3 + (len % 3 == 0 ? 0 : 1));
    if (out_len < minimum_buf_len)
        return minimum_buf_len;

    // process 12 bytes at a time
    // convert them to 16-bytes-long chars which just fit a xmm register
    const int32_t(* const src_blocks)[3] = src;
    __m128i* const dst_blocks = (__m128i*) out_buf;
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
        __m128i offset_for_AtoZ = _mm_shuffle_epi8(_mm_set1_epi8('A'),
                                                   _mm_cmpgt_epi8(fin, _mm_set1_epi8(25)));
        // 26 ~ 51
        __m128i offset_for_atoz = _mm_shuffle_epi8(_mm_set1_epi8('a' - 26),
                                                   _mm_or_si128(_mm_cmpgt_epi8(fin, _mm_set1_epi8(51)),
                                                                _mm_cmplt_epi8(fin, _mm_set1_epi8(26))
                                                   ));
        // 52 ~ 61
        __m128i offset_for_0to9 = _mm_shuffle_epi8(_mm_set1_epi8('0' - 52),
                                                   _mm_or_si128(_mm_cmpgt_epi8(fin, _mm_set1_epi8(61)),
                                                                _mm_cmplt_epi8(fin, _mm_set1_epi8(52))
                                                   ));
        // 62
        temp = _mm_cmpeq_epi8(fin, _mm_set1_epi8(62));
        __m128i offset_for_plus = _mm_shuffle_epi8(_mm_set1_epi8('+' - 62),
                                                   _mm_andnot_si128(temp, _mm_set1_epi8(-1)));
        // 63
        temp = _mm_cmpeq_epi8(fin, _mm_set1_epi8(63));
        __m128i offset_for_slash = _mm_shuffle_epi8(_mm_set1_epi8('/' - 63),
                                                    _mm_andnot_si128(temp, _mm_set1_epi8(-1)));

        fin = _mm_add_epi8(fin, offset_for_AtoZ);
        fin = _mm_add_epi8(fin, offset_for_atoz);
        fin = _mm_add_epi8(fin, offset_for_0to9);
        fin = _mm_add_epi8(fin, offset_for_plus);
        fin = _mm_add_epi8(fin, offset_for_slash);

        _mm_storeu_si128(dst_blocks + i, fin);
    }

    const uint8_t* src_left = (const uint8_t*)src_blocks[blocks_len];
    char* dst_left = (char*)(dst_blocks + blocks_len);
    size_t left_len = len % (3 * sizeof(int32_t));

    for (size_t i = 0; i < left_len / 3; ++i) {
        dst_left[0] = accelc_Base64Table[src_left[0] >> 2];
        dst_left[1] = accelc_Base64Table[(src_left[0] << 4 | src_left[1] >> 4) & 0x3F];
        dst_left[2] = accelc_Base64Table[(src_left[1] << 2 | src_left[2] >> 6) & 0x3F];
        dst_left[3] = accelc_Base64Table[src_left[2] & 0x3F];

        src_left += 3;
        dst_left += 4;
    }

    switch (left_len % 3) {
        case 1:
            dst_left[0] = accelc_Base64Table[src_left[0] >> 2];
            dst_left[1] = accelc_Base64Table[src_left[0] << 4 & 0x3F];
            dst_left[2] = '=';
            dst_left[3] = '=';
            break;
        case 2:
            dst_left[0] = accelc_Base64Table[src_left[0] >> 2];
            dst_left[1] = accelc_Base64Table[(src_left[0] << 4 | src_left[1] >> 4) & 0x3F];
            dst_left[2] = accelc_Base64Table[src_left[1] << 2 & 0x3F];
            dst_left[3] = '=';
            break;
        default:
            break;
    }

    memset(out_buf + minimum_buf_len, 0, out_len - minimum_buf_len);
    return out_len;
}

size_t accelc_Base64Decode_ssse3(const char* __restrict src, size_t len,
                                 void* __restrict out_buf, size_t out_len) {

}