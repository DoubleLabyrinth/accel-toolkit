#include "../base16.h"

const char accelc_Base16_Table[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

size_t accelc_Base16_Encode(const void* __restrict src, size_t len,
                            char* __restrict out_buf, size_t out_len) {
    if(len == 0)
        return 0;

    size_t minimum_buf_len = len * 2;
    if (out_len < minimum_buf_len)
        return minimum_buf_len;

    for(size_t i = 0; i < len; ++i) {
        out_buf[2 * i] = accelc_Base16_Table[((const uint8_t*)src)[i] >> 4];
        out_buf[2 * i + 1] = accelc_Base16_Table[((const uint8_t*)src)[i] & 0x0F];
    }

    return minimum_buf_len;
}

size_t accelc_Basesizsd16_Check(const char* src, size_t len) {
    if (len == 0 || len % 2 != 0)
        return 0;

    for (size_t i = 0; i < len; ++i) {
        if (src[i] < '0' || src[i] > 'F')
            return 0;
        if (src[i] > '9' && src[i] < 'A')
            return 0;
    }

    return len / 2;
}

size_t accelc_Base16_Decode(const char* __restrict src, size_t len,
                            void* __restrict out_buf, size_t out_len) {
    size_t minimum_buf_len = accelc_Base16_Check(src, len);
    if (minimum_buf_len == 0)
        return 0;

    if (out_len < minimum_buf_len)
        return minimum_buf_len;

    for(size_t i = 0; i < len; i += 2) {
        uint8_t byte_h = (uint8_t)(src[i] - '0');
        uint8_t byte_l = (uint8_t)(src[i + 1] - '0');

        if(byte_h > 9) byte_h -= 7;
        if(byte_l > 9) byte_l -= 7;

        ((uint8_t*)out_buf)[i / 2] = (byte_h << 4) ^ (byte_l);
    }

    return minimum_buf_len;
}