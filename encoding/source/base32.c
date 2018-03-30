#include "../base32.h"

const char accelc_Base32Table[32] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', '2', '3', '4', '5', '6', '7',
};

size_t accelc_Base32_Encode(const void* __restrict src, size_t len,
                            char* __restrict out_buf, size_t out_len) {
    if (len == 0)
        return (size_t) -1;

    size_t minimum_buf_len = 8 * (len / 5 + (len % 5 == 0 ? 0 : 1));
    if (out_len < minimum_buf_len)
        return minimum_buf_len;

    const uint8_t (* const blocks)[5] = src;
    const size_t blocks_len = len / 5;

    for (size_t i = 0; i < blocks_len; ++i) {
        out_buf[8 * i] = accelc_Base32Table[blocks[i][0] >> 3];
        out_buf[8 * i + 1] = accelc_Base32Table[(blocks[i][0] << 2 | blocks[i][1] >> 6) & 0x1F];
        out_buf[8 * i + 2] = accelc_Base32Table[(blocks[i][1] >> 1) & 0x1F];
        out_buf[8 * i + 3] = accelc_Base32Table[(blocks[i][1] << 4 | blocks[i][2] >> 4) & 0x1F];
        out_buf[8 * i + 4] = accelc_Base32Table[(blocks[i][2] << 1 | blocks[i][3] >> 7) & 0x1F];
        out_buf[8 * i + 5] = accelc_Base32Table[(blocks[i][3] >> 2) & 0x1F];
        out_buf[8 * i + 6] = accelc_Base32Table[(blocks[i][3] << 3 | blocks[i][4] >> 5) & 0x1F];
        out_buf[8 * i + 7] = accelc_Base32Table[blocks[i][4] & 0x1F];
    }

    switch (len % 5) {
        case 1:
            out_buf[8 * blocks_len] = accelc_Base32Table[blocks[blocks_len][0] >> 3];
            out_buf[8 * blocks_len + 1] = accelc_Base32Table[(blocks[blocks_len][0] << 2) & 0x3F];
            out_buf[8 * blocks_len + 2] = '=';
            out_buf[8 * blocks_len + 3] = '=';
            out_buf[8 * blocks_len + 4] = '=';
            out_buf[8 * blocks_len + 5] = '=';
            out_buf[8 * blocks_len + 6] = '=';
            out_buf[8 * blocks_len + 7] = '=';
            break;
        case 2:
            out_buf[8 * blocks_len] = accelc_Base32Table[blocks[blocks_len][0] >> 3];
            out_buf[8 * blocks_len + 1] = accelc_Base32Table[(blocks[blocks_len][0] << 2 | blocks[blocks_len][1] >> 6) & 0x1F];
            out_buf[8 * blocks_len + 2] = accelc_Base32Table[(blocks[blocks_len][1] >> 1) & 0x1F];
            out_buf[8 * blocks_len + 3] = accelc_Base32Table[(blocks[blocks_len][1] << 4) & 0x1F];
            out_buf[8 * blocks_len + 4] = '=';
            out_buf[8 * blocks_len + 5] = '=';
            out_buf[8 * blocks_len + 6] = '=';
            out_buf[8 * blocks_len + 7] = '=';
            break;
        case 3:
            out_buf[8 * blocks_len] = accelc_Base32Table[blocks[blocks_len][0] >> 3];
            out_buf[8 * blocks_len + 1] = accelc_Base32Table[(blocks[blocks_len][0] << 2 | blocks[blocks_len][1] >> 6) & 0x1F];
            out_buf[8 * blocks_len + 2] = accelc_Base32Table[(blocks[blocks_len][1] >> 1) & 0x1F];
            out_buf[8 * blocks_len + 3] = accelc_Base32Table[(blocks[blocks_len][1] << 4 | blocks[blocks_len][2] >> 4) & 0x1F];
            out_buf[8 * blocks_len + 4] = accelc_Base32Table[(blocks[blocks_len][2] << 1) & 0x1F];
            out_buf[8 * blocks_len + 5] = '=';
            out_buf[8 * blocks_len + 6] = '=';
            out_buf[8 * blocks_len + 7] = '=';
            break;
        case 4:
            out_buf[8 * blocks_len] = accelc_Base32Table[blocks[blocks_len][0] >> 3];
            out_buf[8 * blocks_len + 1] = accelc_Base32Table[(blocks[blocks_len][0] << 2 | blocks[blocks_len][1] >> 6) & 0x1F];
            out_buf[8 * blocks_len + 2] = accelc_Base32Table[(blocks[blocks_len][1] >> 1) & 0x1F];
            out_buf[8 * blocks_len + 3] = accelc_Base32Table[(blocks[blocks_len][1] << 4 | blocks[blocks_len][2] >> 4) & 0x1F];
            out_buf[8 * blocks_len + 4] = accelc_Base32Table[(blocks[blocks_len][2] << 1 | blocks[blocks_len][3] >> 7) & 0x1F];
            out_buf[8 * blocks_len + 5] = accelc_Base32Table[(blocks[blocks_len][3] >> 2) & 0x1F];
            out_buf[8 * blocks_len + 6] = accelc_Base32Table[(blocks[blocks_len][3] << 3) & 0x1F];
            out_buf[8 * blocks_len + 7] = '=';
            break;
        default:
            break;
    }

    return minimum_buf_len;
}