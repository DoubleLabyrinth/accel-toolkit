#include "../base32.h"

const char accelc_Base32_Table[32] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', '2', '3', '4', '5', '6', '7',
};

const uint8_t accelc_Base32_InverseTable[96] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 26, 27, 28, 29, 30, 31, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0
};

size_t accelc_Base32_Encode(const void* __restrict src, size_t len,
                            char* __restrict out_buf, size_t out_len) {
    if (len == 0)
        return (size_t) -1;

    size_t minimum_buf_len = 8 * (len / 5 + (len % 5 == 0 ? 0 : 1));
    if (out_len < minimum_buf_len)
        return minimum_buf_len;

    const uint8_t (* const src_blocks)[5] = src;
    char (*dst_blocks)[8] = (char (*)[8])out_buf;
    const size_t blocks_len = len / 5;

    for (size_t i = 0; i < blocks_len; ++i) {
        dst_blocks[i][0] = accelc_Base32_Table[src_blocks[i][0] >> 3];
        dst_blocks[i][1] = accelc_Base32_Table[(src_blocks[i][0] << 2 | src_blocks[i][1] >> 6) & 0x1F];
        dst_blocks[i][2] = accelc_Base32_Table[(src_blocks[i][1] >> 1) & 0x1F];
        dst_blocks[i][3] = accelc_Base32_Table[(src_blocks[i][1] << 4 | src_blocks[i][2] >> 4) & 0x1F];
        dst_blocks[i][4] = accelc_Base32_Table[(src_blocks[i][2] << 1 | src_blocks[i][3] >> 7) & 0x1F];
        dst_blocks[i][5] = accelc_Base32_Table[(src_blocks[i][3] >> 2) & 0x1F];
        dst_blocks[i][6] = accelc_Base32_Table[(src_blocks[i][3] << 3 | src_blocks[i][4] >> 5) & 0x1F];
        dst_blocks[i][7] = accelc_Base32_Table[src_blocks[i][4] & 0x1F];
    }

    switch (len % 5) {
        case 1:
            dst_blocks[blocks_len][0] = accelc_Base32_Table[src_blocks[blocks_len][0] >> 3];
            dst_blocks[blocks_len][1] = accelc_Base32_Table[(src_blocks[blocks_len][0] << 2) & 0x3F];
            dst_blocks[blocks_len][2] = '=';
            dst_blocks[blocks_len][3] = '=';
            dst_blocks[blocks_len][4] = '=';
            dst_blocks[blocks_len][5] = '=';
            dst_blocks[blocks_len][6] = '=';
            dst_blocks[blocks_len][7] = '=';
            break;
        case 2:
            dst_blocks[blocks_len][0] = accelc_Base32_Table[src_blocks[blocks_len][0] >> 3];
            dst_blocks[blocks_len][1] = accelc_Base32_Table[(src_blocks[blocks_len][0] << 2 | src_blocks[blocks_len][1] >> 6) & 0x1F];
            dst_blocks[blocks_len][2] = accelc_Base32_Table[(src_blocks[blocks_len][1] >> 1) & 0x1F];
            dst_blocks[blocks_len][3] = accelc_Base32_Table[(src_blocks[blocks_len][1] << 4) & 0x1F];
            dst_blocks[blocks_len][4] = '=';
            dst_blocks[blocks_len][5] = '=';
            dst_blocks[blocks_len][6] = '=';
            dst_blocks[blocks_len][7] = '=';
            break;
        case 3:
            dst_blocks[blocks_len][0] = accelc_Base32_Table[src_blocks[blocks_len][0] >> 3];
            dst_blocks[blocks_len][1] = accelc_Base32_Table[(src_blocks[blocks_len][0] << 2 | src_blocks[blocks_len][1] >> 6) & 0x1F];
            dst_blocks[blocks_len][2] = accelc_Base32_Table[(src_blocks[blocks_len][1] >> 1) & 0x1F];
            dst_blocks[blocks_len][3] = accelc_Base32_Table[(src_blocks[blocks_len][1] << 4 | src_blocks[blocks_len][2] >> 4) & 0x1F];
            dst_blocks[blocks_len][4] = accelc_Base32_Table[(src_blocks[blocks_len][2] << 1) & 0x1F];
            dst_blocks[blocks_len][5] = '=';
            dst_blocks[blocks_len][6] = '=';
            dst_blocks[blocks_len][7] = '=';
            break;
        case 4:
            dst_blocks[blocks_len][0] = accelc_Base32_Table[src_blocks[blocks_len][0] >> 3];
            dst_blocks[blocks_len][1] = accelc_Base32_Table[(src_blocks[blocks_len][0] << 2 | src_blocks[blocks_len][1] >> 6) & 0x1F];
            dst_blocks[blocks_len][2] = accelc_Base32_Table[(src_blocks[blocks_len][1] >> 1) & 0x1F];
            dst_blocks[blocks_len][3] = accelc_Base32_Table[(src_blocks[blocks_len][1] << 4 | src_blocks[blocks_len][2] >> 4) & 0x1F];
            dst_blocks[blocks_len][4] = accelc_Base32_Table[(src_blocks[blocks_len][2] << 1 | src_blocks[blocks_len][3] >> 7) & 0x1F];
            dst_blocks[blocks_len][5] = accelc_Base32_Table[(src_blocks[blocks_len][3] >> 2) & 0x1F];
            dst_blocks[blocks_len][6] = accelc_Base32_Table[(src_blocks[blocks_len][3] << 3) & 0x1F];
            dst_blocks[blocks_len][7] = '=';
            break;
        default:
            break;
    }

    return minimum_buf_len;
}

size_t accelc_Base32_Check(const char* src, size_t len) {
    if (len == 0 || len % 8 != 0)
        return 0;

    size_t i = 0;
    for (; i < len; ++i) {
        if (src[i] <= 'Z' && src[i] >= 'A')
            continue;
        if (src[i] <= '7' && src[i] >= '2')
            continue;
        break;
    }

    size_t j = 0;
    for (; j + i < len; ++j)
        if (src[i + j] != '=')
            return 0;

    switch (j) {
        case 0:
            return (len / 8) * 5;
        case 1:
            return (len / 8) * 5 + 4;
        case 3:
            return (len / 8) * 5 + 3;
        case 4:
            return (len / 8) * 5 + 2;
        case 6:
            return (len / 8) * 5 + 1;
        default:
            return 0;
    }
}

size_t accelc_Base32_Decode(const char* __restrict src, size_t len,
                            void* __restrict out_buf, size_t out_len) {
    size_t minimum_buf_len = accelc_Base32_Check(src, len);
    if (minimum_buf_len == 0)
        return 0;

    if (out_len < minimum_buf_len)
        return minimum_buf_len;

    const char (*src_blocks)[8] = (const char (*)[8])src;
    uint8_t (*dst_block)[5] = out_buf;
    size_t blocks_len = len / 8 - 1;
    for (size_t i = 0; i < blocks_len; ++i) {

        dst_block[i][0] = accelc_Base32_InverseTable[src_blocks[i][0]] << 3 ^
                          accelc_Base32_InverseTable[src_blocks[i][1]] >> 2;

        dst_block[i][1] = accelc_Base32_InverseTable[src_blocks[i][1]] << 6 ^
                          accelc_Base32_InverseTable[src_blocks[i][2]] << 1 ^
                          accelc_Base32_InverseTable[src_blocks[i][3]] >> 4;

        dst_block[i][2] = accelc_Base32_InverseTable[src_blocks[i][3]] << 4 ^
                          accelc_Base32_InverseTable[src_blocks[i][4]] >> 1;

        dst_block[i][3] = accelc_Base32_InverseTable[src_blocks[i][4]] << 7 ^
                          accelc_Base32_InverseTable[src_blocks[i][5]] << 2 ^
                          accelc_Base32_InverseTable[src_blocks[i][6]] >> 3;

        dst_block[i][4] = accelc_Base32_InverseTable[src_blocks[i][6]] << 5 ^
                          accelc_Base32_InverseTable[src_blocks[i][7]];
    }

    int padding = 0;
    for (int i = 0; i < 8; ++i)
        if (src_blocks[blocks_len][i] == '=') padding++;
    switch (padding) {
        case 0:
            dst_block[blocks_len][4] = accelc_Base32_InverseTable[src_blocks[blocks_len][6]] << 5 ^
                                       accelc_Base32_InverseTable[src_blocks[blocks_len][7]];
        case 1:
            dst_block[blocks_len][3] = accelc_Base32_InverseTable[src_blocks[blocks_len][4]] << 7 ^
                                       accelc_Base32_InverseTable[src_blocks[blocks_len][5]] << 2 ^
                                       accelc_Base32_InverseTable[src_blocks[blocks_len][6]] >> 3;
        case 3:
            dst_block[blocks_len][2] = accelc_Base32_InverseTable[src_blocks[blocks_len][3]] << 4 ^
                                       accelc_Base32_InverseTable[src_blocks[blocks_len][4]] >> 1;
        case 4:
            dst_block[blocks_len][1] = accelc_Base32_InverseTable[src_blocks[blocks_len][1]] << 6 ^
                                       accelc_Base32_InverseTable[src_blocks[blocks_len][2]] << 1 ^
                                       accelc_Base32_InverseTable[src_blocks[blocks_len][3]] >> 4;
        case 6:
            dst_block[blocks_len][0] = accelc_Base32_InverseTable[src_blocks[blocks_len][0]] << 3 ^
                                       accelc_Base32_InverseTable[src_blocks[blocks_len][1]] >> 2;
        default:
            break;
    }

    return minimum_buf_len;
}