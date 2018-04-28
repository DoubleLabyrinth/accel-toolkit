#include "../base64.h"

const char accelc_Base64_Table[64] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'
};

const uint8_t accelc_Base64_InverseTable[128] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3f,
        0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
        0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00
};

size_t accelc_Base64_Encode(const void* __restrict src, size_t len,
                            char* __restrict out_buf, size_t out_len) {
    if (len == 0)
        return 0;

    size_t minimum_buf_len = 4 * (len / 3 + (len % 3 == 0 ? 0 : 1));
    if (out_len < minimum_buf_len)
        return minimum_buf_len;

    const uint8_t (*src_blocks)[3] = src;
    char (*dst_blocks)[4] = (char (*)[4])out_buf;
    const size_t blocks_len = len / 3;

    for (size_t i = 0; i < blocks_len; ++i) {
        dst_blocks[i][0] = accelc_Base64_Table[src_blocks[i][0] >> 2];
        dst_blocks[i][1] = accelc_Base64_Table[(src_blocks[i][0] << 4 | src_blocks[i][1] >> 4) & 0x3F];
        dst_blocks[i][2] = accelc_Base64_Table[(src_blocks[i][1] << 2 | src_blocks[i][2] >> 6) & 0x3F];
        dst_blocks[i][3] = accelc_Base64_Table[src_blocks[i][2] & 0x3F];
    }

    switch (len % 3) {
        case 1:
            dst_blocks[blocks_len][0] = accelc_Base64_Table[src_blocks[blocks_len][0] >> 2];
            dst_blocks[blocks_len][1] = accelc_Base64_Table[(src_blocks[blocks_len][0] << 4) & 0x3F];
            dst_blocks[blocks_len][2] = '=';
            dst_blocks[blocks_len][3] = '=';
            break;
        case 2:
            dst_blocks[blocks_len][0] = accelc_Base64_Table[src_blocks[blocks_len][0] >> 2];
            dst_blocks[blocks_len][1] = accelc_Base64_Table[(src_blocks[blocks_len][0] << 4 | src_blocks[blocks_len][1] >> 4) & 0x3F];
            dst_blocks[blocks_len][2] = accelc_Base64_Table[src_blocks[blocks_len][1] << 2 & 0x3F];
            dst_blocks[blocks_len][3] = '=';
            break;
        default:
            break;
    }

    return minimum_buf_len;
}

size_t accelc_Base64_Check(const char* src, size_t len) {
    if (len % 4 != 0)
        return 0;

    size_t i = 0;
    for (; i < len; ++i) {
        if (src[i] >= 'A' && src[i] <= 'Z')
            continue;
        if (src[i] >= 'a' && src[i] <= 'z')
            continue;
        if (src[i] >= '0' && src[i] <= '9')
            continue;
        if (src[i] == '+' || src[i] == '/')
            continue;
        break;
    }

    size_t j = 0;
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

size_t accelc_Base64_Decode(const char* __restrict src, size_t len,
                            void* __restrict out_buf, size_t out_len) {
    size_t minimum_buf_len = accelc_Base64_Check(src, len);
    if (minimum_buf_len == 0)
        return 0;
    if (out_len < minimum_buf_len)
        return minimum_buf_len;

    const char (*src_blocks)[4] = (const char (*)[4])src;
    uint8_t (*dst_blocks)[3] = (uint8_t (*)[3])out_buf;
    size_t blocks_len = len / 4 - 1;

    for (size_t i = 0; i < blocks_len; ++i) {
        dst_blocks[i][0] = accelc_Base64_InverseTable[src_blocks[i][0]] << 2 | accelc_Base64_InverseTable[src_blocks[i][1]] >> 4;
        dst_blocks[i][1] = accelc_Base64_InverseTable[src_blocks[i][1]] << 4 | accelc_Base64_InverseTable[src_blocks[i][2]] >> 2;
        dst_blocks[i][2] = accelc_Base64_InverseTable[src_blocks[i][2]] << 6 | accelc_Base64_InverseTable[src_blocks[i][3]];
    }

    dst_blocks[blocks_len][0] = accelc_Base64_InverseTable[src_blocks[blocks_len][0]] << 2 | accelc_Base64_InverseTable[src_blocks[blocks_len][1]] >> 4;

    if (src_blocks[blocks_len][2] == '=') {
        return blocks_len * 3 + 1;
    } else if(src_blocks[blocks_len][3] == '=') {
        dst_blocks[blocks_len][1] = accelc_Base64_InverseTable[src_blocks[blocks_len][1]] << 4 | accelc_Base64_InverseTable[src_blocks[blocks_len][2]] >> 2;
        return blocks_len * 3 + 2;
    } else {
        dst_blocks[blocks_len][1] = accelc_Base64_InverseTable[src_blocks[blocks_len][1]] << 4 | accelc_Base64_InverseTable[src_blocks[blocks_len][2]] >> 2;
        dst_blocks[blocks_len][1] = accelc_Base64_InverseTable[src_blocks[blocks_len][2]] << 6 | accelc_Base64_InverseTable[src_blocks[blocks_len][3]];
        return blocks_len * 3 + 3;
    }
}
