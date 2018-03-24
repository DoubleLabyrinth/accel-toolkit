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

size_t accelcpp_Base64Encode(const void* __restrict src, size_t len,
                             char* __restrict out_buf, size_t out_len) {
    if (len == 0)
        return -1;

    size_t minium_buf_len = 4 * (len / 3 + (len % 3 == 0 ? 0 : 1));
    if (out_len < minium_buf_len)
        return minium_buf_len;

    const uint8_t (*const blocks)[3] = src;
    const size_t blocks_len = len / 3;

    for (size_t i = 0; i < blocks_len; ++i) {
        out_buf[4 * i] = accelcpp_Base64Table[blocks[i][0] >> 2];
        out_buf[4 * i + 1] = accelcpp_Base64Table[(blocks[i][0] << 4 | blocks[i][1] >> 4) & 0x3F];
        out_buf[4 * i + 2] = accelcpp_Base64Table[(blocks[i][1] << 2 | blocks[i][2] >> 6) & 0x3F];
        out_buf[4 * i + 3] = accelcpp_Base64Table[blocks[i][2] & 0x3F];
    }

    switch (len % 3) {
        case 1:
            out_buf[4 * blocks_len] = accelcpp_Base64Table[blocks[blocks_len][0] >> 2];
            out_buf[4 * blocks_len + 1] = accelcpp_Base64Table[(blocks[blocks_len][0] << 4) & 0x3F];
            out_buf[4 * blocks_len + 2] = '=';
            out_buf[4 * blocks_len + 3] = '=';
            break;
        case 2:
            out_buf[4 * blocks_len] = accelcpp_Base64Table[blocks[blocks_len][0] >> 2];
            out_buf[4 * blocks_len + 1] = accelcpp_Base64Table[(blocks[blocks_len][0] << 4 | blocks[blocks_len][1] >> 4) & 0x3F];
            out_buf[4 * blocks_len + 2] = accelcpp_Base64Table[blocks[blocks_len][1] << 2 & 0x3F];
            out_buf[4 * blocks_len + 3] = '=';
            break;
        default:
            break;
    }

    memset(out_buf + minium_buf_len, 0, out_len - minium_buf_len);
    return out_len;
}

size_t accelcpp_Base64Decode(const char* __restrict src, size_t len,
                             void* __restrict out_buf, size_t out_len) {

}