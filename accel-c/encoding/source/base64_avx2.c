#include "base64.h"
#include <intrin.h>
#include <memory.h>

static const char accelcpp_Base64Table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

size_t accelcpp_Base64Encode_avx2(const void* __restrict src, size_t len,
                                  char* __restrict out_buf, size_t out_len) {
    if (len == 0)
        return -1;

    size_t minium_buf_len = 4 * (len / 3 + (len % 3 == 0 ? 0 : 1));
    if (out_len < minium_buf_len)
        return minium_buf_len;

    // process 24 bytes at a time
    // convert them to 32-bytes-long chars which just fit a ymm register
    const int32_t (*const src_blocks)[6] = src;
    __m256i* const dst_blocks = (__m256i*)out_buf;
    const size_t blocks_len = len / (6 * sizeof(int32_t));

    for (size_t i = 0; i < blocks_len; ++i) {
        __m256i temp = _mm256_set_epi32(0, src_blocks[i][5], src_blocks[i][4], src_blocks[i][3],
                                        0, src_blocks[i][2], src_blocks[i][1], src_blocks[i][0]);

        temp = _mm256_shuffle_epi8(temp, _mm256_set_epi8(2 + 9, 0 + 9, 1 + 9, 2 + 9,
                                                         2 + 6, 0 + 6, 1 + 6, 2 + 6,
                                                         2 + 3, 0 + 3, 1 + 3, 2 + 3,
                                                         2, 0, 1, 2, 
                                                         2 + 9, 0 + 9, 1 + 9, 2 + 9,
                                                         2 + 6, 0 + 6, 1 + 6, 2 + 6,
                                                         2 + 3, 0 + 3, 1 + 3, 2 + 3,
                                                         2, 0, 1, 2));
        __m256i fin = _mm256_and_si256(temp, _mm256_set1_epi32(0x3F000000));

        temp = _mm256_srli_epi32(temp, 2);
        fin = _mm256_xor_si256(fin, _mm256_and_si256(temp, _mm256_set1_epi32(0x003F0000)));

        temp = _mm256_srli_epi32(temp, 2);
        fin = _mm256_xor_si256(fin, _mm256_and_si256(temp, _mm256_set1_epi32(0x00003F00)));

        temp = _mm256_srli_epi32(temp, 2);
        fin = _mm256_xor_si256(fin, _mm256_and_si256(temp, _mm256_set1_epi32(0x0000003F)));

        fin = _mm256_shuffle_epi8(fin, _mm256_set_epi8(3 + 12, 0 + 12, 1 + 12, 2 + 12,
                                                       3 + 8, 0 + 8, 1 + 8, 2 + 8,
                                                       3 + 4, 0 + 4, 1 + 4, 2 + 4,
                                                       3, 0, 1, 2,
                                                       3 + 12, 0 + 12, 1 + 12, 2 + 12,
                                                       3 + 8, 0 + 8, 1 + 8, 2 + 8,
                                                       3 + 4, 0 + 4, 1 + 4, 2 + 4,
                                                       3, 0, 1, 2));

        // 0 ~ 25
        __m256i offset = _mm256_blendv_epi8(_mm256_set1_epi8('A'), _mm256_setzero_si256(),
                                            _mm256_cmpgt_epi8(fin, _mm256_set1_epi8(25))    // if fin_x > 25, offset is set to zero and waiting to be decided.
        );

        // 26 ~ 51
        offset = _mm256_blendv_epi8(_mm256_set1_epi8('a' - 26), offset,
                                    _mm256_or_si256(_mm256_cmpgt_epi8(fin, _mm256_set1_epi8(51)),
                                                    _mm256_cmpgt_epi8(_mm256_set1_epi8(26), fin))
        );

        // 52 ~ 61
        offset = _mm256_blendv_epi8(_mm256_set1_epi8('0' - 52), offset,
                                    _mm256_or_si256(_mm256_cmpgt_epi8(fin, _mm256_set1_epi8(61)),
                                                    _mm256_cmpgt_epi8(_mm256_set1_epi8(52), fin))
        );

        // 62
        offset = _mm256_blendv_epi8(offset, _mm256_set1_epi8('+' - 62),
                                    _mm256_cmpeq_epi8(fin, _mm256_set1_epi8(62))
        );

        // 63
        offset = _mm256_blendv_epi8(offset, _mm256_set1_epi8('/' - 63),
                                    _mm256_cmpeq_epi8(fin, _mm256_set1_epi8(63))
        );

        fin = _mm256_add_epi8(fin, offset);
        _mm256_storeu_si256(dst_blocks + i, fin);
    }

    const uint8_t* src_left = (const uint8_t*)src_blocks[blocks_len];
    char* dst_left = (char*)(dst_blocks + blocks_len);
    size_t left_len = len % (6 * sizeof(int32_t));

    for (size_t i = 0; i < left_len / 3; ++i) {
        dst_left[0] = accelcpp_Base64Table[src_left[0] >> 2];
        dst_left[1] = accelcpp_Base64Table[(src_left[0] << 4 | src_left[1] >> 4) & 0x3F];
        dst_left[2] = accelcpp_Base64Table[(src_left[1] << 2 | src_left[2] >> 6) & 0x3F];
        dst_left[3] = accelcpp_Base64Table[src_left[2] & 0x3F];

        src_left += 3;
        dst_left += 4;
    }

    switch (left_len % 3) {
        case 1:
            dst_left[0] = accelcpp_Base64Table[src_left[0] >> 2];
            dst_left[1] = accelcpp_Base64Table[src_left[0] << 4 & 0x3F];
            dst_left[2] = '=';
            dst_left[3] = '=';
            break;
        case 2:
            dst_left[0] = accelcpp_Base64Table[src_left[0] >> 2];
            dst_left[1] = accelcpp_Base64Table[(src_left[0] << 4 | src_left[1] >> 4) & 0x3F];
            dst_left[2] = accelcpp_Base64Table[src_left[1] << 2 & 0x3F];
            dst_left[3] = '=';
            break;
        default:
            break;
    }

    memset(out_buf + minium_buf_len, 0, out_len - minium_buf_len);
    return out_len;
}
