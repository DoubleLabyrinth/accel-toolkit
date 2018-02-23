/*********************************************************************
* Filename:   DES.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../DES.h"
#include <intrin.h>

const uint8_t CRYPTO_DES_S1[4][16] = {
    { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 },
    { 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8 },
    { 4 ,1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0 },
    { 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 }
};  //checked

const uint8_t CRYPTO_DES_S2[4][16] = {
    { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10 },
    { 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5 },
    { 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15 },
    { 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 }
};  //checked

const uint8_t CRYPTO_DES_S3[4][16] = {
    { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8 },
    { 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1 },
    { 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7 },
    { 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 }
};  //checked

const uint8_t CRYPTO_DES_S4[4][16] = {
    { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15 },
    { 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9 },
    { 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4 },
    { 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 }
};  //checked

const uint8_t CRYPTO_DES_S5[4][16] = {
    { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9 },
    { 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6 },
    { 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14 },
    { 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 }
};  //checked

const uint8_t CRYPTO_DES_S6[4][16] = {
    { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11 },
    { 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8 },
    { 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6 },
    { 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 }
};  //checked

const uint8_t CRYPTO_DES_S7[4][16] = {
    { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1 },
    { 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6 },
    { 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2 },
    { 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 }
};  //checked

const uint8_t CRYPTO_DES_S8[4][16] = {
    { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7 },
    { 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2 },
    { 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8 },
    { 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 }
};  //checked

void CRYPTO_DES_EncryptBlock(uint8_t srcBytes[8], const uint8_t srcExpandedKey[16][6]) {
    uint8_t Buffer[8];

    //Start initial permutation
    Buffer[0] = (
        (srcBytes[7] & 0x40) << 1 |
        (srcBytes[6] & 0x40) |
        (srcBytes[5] & 0x40) >> 1 |
        (srcBytes[4] & 0x40) >> 2 |
        (srcBytes[3] & 0x40) >> 3 |
        (srcBytes[2] & 0x40) >> 4 |
        (srcBytes[1] & 0x40) >> 5 |
        (srcBytes[0] & 0x40) >> 6
        );

    Buffer[1] = (
        (srcBytes[7] & 0x10) << 3 |
        (srcBytes[6] & 0x10) << 2 |
        (srcBytes[5] & 0x10) << 1 |
        (srcBytes[4] & 0x10) |
        (srcBytes[3] & 0x10) >> 1 |
        (srcBytes[2] & 0x10) >> 2 |
        (srcBytes[1] & 0x10) >> 3 |
        (srcBytes[0] & 0x10) >> 4
        );

    Buffer[2] = (
        (srcBytes[7] & 0x04) << 5 |
        (srcBytes[6] & 0x04) << 4 |
        (srcBytes[5] & 0x04) << 3 |
        (srcBytes[4] & 0x04) << 2 |
        (srcBytes[3] & 0x04) << 1 |
        (srcBytes[2] & 0x04) |
        (srcBytes[1] & 0x04) >> 1 |
        (srcBytes[0] & 0x04) >> 2
        );

    Buffer[3] = (
        (srcBytes[7] & 0x01) << 7 |
        (srcBytes[6] & 0x01) << 6 |
        (srcBytes[5] & 0x01) << 5 |
        (srcBytes[4] & 0x01) << 4 |
        (srcBytes[3] & 0x01) << 3 |
        (srcBytes[2] & 0x01) << 2 |
        (srcBytes[1] & 0x01) << 1 |
        (srcBytes[0] & 0x01)
        );

    Buffer[4] = (
        (srcBytes[7] & 0x80) |
        (srcBytes[6] & 0x80) >> 1 |
        (srcBytes[5] & 0x80) >> 2 |
        (srcBytes[4] & 0x80) >> 3 |
        (srcBytes[3] & 0x80) >> 4 |
        (srcBytes[2] & 0x80) >> 5 |
        (srcBytes[1] & 0x80) >> 6 |
        (srcBytes[0] & 0x80) >> 7
        );

    Buffer[5] = (
        (srcBytes[7] & 0x20) << 2 |
        (srcBytes[6] & 0x20) << 1 |
        (srcBytes[5] & 0x20) |
        (srcBytes[4] & 0x20) >> 1 |
        (srcBytes[3] & 0x20) >> 2 |
        (srcBytes[2] & 0x20) >> 3 |
        (srcBytes[1] & 0x20) >> 4 |
        (srcBytes[0] & 0x20) >> 5
        );

    Buffer[6] = (
        (srcBytes[7] & 0x08) << 4 |
        (srcBytes[6] & 0x08) << 3 |
        (srcBytes[5] & 0x08) << 2 |
        (srcBytes[4] & 0x08) << 1 |
        (srcBytes[3] & 0x08) |
        (srcBytes[2] & 0x08) >> 1 |
        (srcBytes[1] & 0x08) >> 2 |
        (srcBytes[0] & 0x08) >> 3
        );

    Buffer[7] = (
        (srcBytes[7] & 0x02) << 6 |
        (srcBytes[6] & 0x02) << 5 |
        (srcBytes[5] & 0x02) << 4 |
        (srcBytes[4] & 0x02) << 3 |
        (srcBytes[3] & 0x02) << 2 |
        (srcBytes[2] & 0x02) << 1 |
        (srcBytes[1] & 0x02) |
        (srcBytes[0] & 0x02) >> 1
        );

    //Start enciphering
    uint8_t* const LeftPart = Buffer;
    uint8_t* const RightPart = Buffer + 4;
    for (int i = 0; i < 16; ++i) {
        uint32_t temp = *(uint32_t*)RightPart;
        //Cipher function f
        //E BIT-SELECTION and xor by K
        uint8_t tmp[6];

        tmp[0] = (
            (
            (RightPart[3] << 7) |
                (RightPart[0] & 0xF8) >> 1 |
                (RightPart[0] & 0x18) >> 3
                ) ^ srcExpandedKey[i][0]
            );

        tmp[1] = (
            (
            (RightPart[0] << 5) |
                (RightPart[1] & 0x80) >> 3 |
                (RightPart[0] & 0x01) << 3 |
                (RightPart[1] >> 5)
                ) ^ srcExpandedKey[i][1]
            );

        tmp[2] = (
            (
            (RightPart[1] & 0x18) << 3 |
                (RightPart[1] & 0x1F) << 1 |
                (RightPart[2] >> 7)
                ) ^ srcExpandedKey[i][2]
            );

        tmp[3] = (
            (
            (RightPart[1] << 7) |
                (RightPart[2] & 0xF8) >> 1 |
                (RightPart[2] & 0x18) >> 3
                ) ^ srcExpandedKey[i][3]
            );

        tmp[4] = (
            (
            (RightPart[2] << 5) |
                (RightPart[3] & 0x80) >> 3 |
                (RightPart[2] & 0x01) << 3 |
                (RightPart[3] >> 5)
                ) ^ srcExpandedKey[i][4]
            );

        tmp[5] = (
            (
            (RightPart[3] & 0x18) << 3 |
                (RightPart[3] & 0x1F) << 1 |
                (RightPart[0] >> 7)
                ) ^ srcExpandedKey[i][5]
            );

        //S transformation
        RightPart[0] = (
            CRYPTO_DES_S1[(tmp[0] & 0x80) >> 6 | (tmp[0] & 0x04) >> 2][(tmp[0] & 0x78) >> 3] << 4 |
            CRYPTO_DES_S2[(tmp[0] & 0x02) | (tmp[1] & 0x10) >> 4][(tmp[0] & 0x01) << 3 | tmp[1] >> 5]
            );

        RightPart[1] = (
            CRYPTO_DES_S3[(tmp[1] & 0x08) >> 2 | (tmp[2] & 0x40) >> 6][(tmp[1] & 0x07) << 1 | tmp[2] >> 7] << 4 |
            CRYPTO_DES_S4[(tmp[2] & 0x20) >> 4 | (tmp[2] & 0x01)][(tmp[2] & 0x1E) >> 1]
            );

        RightPart[2] = (
            CRYPTO_DES_S5[(tmp[3] & 0x80) >> 6 | (tmp[3] & 0x04) >> 2][(tmp[3] & 0x78) >> 3] << 4 |
            CRYPTO_DES_S6[(tmp[3] & 0x02) | (tmp[4] & 0x10) >> 4][(tmp[3] & 0x01) << 3 | tmp[4] >> 5]
            );

        RightPart[3] = (
            CRYPTO_DES_S7[(tmp[4] & 0x08) >> 2 | (tmp[5] & 0x40) >> 6][(tmp[4] & 0x07) << 1 | tmp[5] >> 7] << 4 |
            CRYPTO_DES_S8[(tmp[5] & 0x20) >> 4 | (tmp[5] & 0x01)][(tmp[5] & 0x1E) >> 1]
            );

        //P transformation and xor by LeftPart
        tmp[0] = (
            (
            (RightPart[1] << 7) |
                (RightPart[0] & 0x02) << 5 |
                (RightPart[2] & 0x10) << 1 |
                (RightPart[2] & 0x08) << 1 |
                (RightPart[3] & 0x08) |
                (RightPart[1] & 0x10) >> 2 |
                (RightPart[3] & 0x10) >> 3 |
                (RightPart[2] >> 7)
                ) ^ LeftPart[0]
            );

        tmp[1] = (
            (
            (RightPart[0] & 0x80) |
                (RightPart[1] & 0x02) << 5 |
                (RightPart[2] & 0x02) << 4 |
                (RightPart[3] & 0x40) >> 2 |
                (RightPart[0] & 0x08) |
                (RightPart[2] & 0x40) >> 4 |
                (RightPart[3] & 0x02) |
                (RightPart[1] & 0x40) >> 6
                ) ^ LeftPart[1]
            );

        tmp[2] = (
            (
            (RightPart[0] & 0x40) << 1 |
                (RightPart[0] & 0x01) << 6 |
                (RightPart[2] & 0x01) << 5 |
                (RightPart[1] & 0x04) << 2 |
                (RightPart[3] & 0x01) << 3 |
                (RightPart[3] & 0x20) >> 3 |
                (RightPart[0] & 0x20) >> 4 |
                (RightPart[1] >> 7)
                ) ^ LeftPart[2]
            );

        tmp[3] = (
            (
            (RightPart[2] & 0x20) << 2 |
                (RightPart[1] & 0x08) << 3 |
                (RightPart[3] & 0x04) << 3 |
                (RightPart[0] & 0x04) << 2 |
                (RightPart[2] & 0x04) << 1 |
                (RightPart[1] & 0x20) >> 3 |
                (RightPart[0] & 0x10) >> 3 |
                (RightPart[3] >> 7)
                ) ^ LeftPart[3]
            );

        *(uint32_t*)RightPart = *(uint32_t*)tmp;
        *(uint32_t*)LeftPart = temp;
    }

    //Start inverse initial permutation
    srcBytes[0] = (
        (LeftPart[0] << 7) |
        (RightPart[0] & 0x01) << 6 |
        (LeftPart[1] & 0x01) << 5 |
        (RightPart[1] & 0x01) << 4 |
        (LeftPart[2] & 0x01) << 3 |
        (RightPart[2] & 0x01) << 2 |
        (LeftPart[3] & 0x01) << 1 |
        (RightPart[3] & 0x01)
        );

    srcBytes[1] = (
        (LeftPart[0] & 0x02) << 6 |
        (RightPart[0] & 0x02) << 5 |
        (LeftPart[1] & 0x02) << 4 |
        (RightPart[1] & 0x02) << 3 |
        (LeftPart[2] & 0x02) << 2 |
        (RightPart[2] & 0x02) << 1 |
        (LeftPart[3] & 0x02) |
        (RightPart[3] & 0x02) >> 1
        );

    srcBytes[2] = (
        (LeftPart[0] & 0x04) << 5 |
        (RightPart[0] & 0x04) << 4 |
        (LeftPart[1] & 0x04) << 3 |
        (RightPart[1] & 0x04) << 2 |
        (LeftPart[2] & 0x04) << 1 |
        (RightPart[2] & 0x04) |
        (LeftPart[3] & 0x04) >> 1 |
        (RightPart[3] & 0x04) >> 2
        );

    srcBytes[3] = (
        (LeftPart[0] & 0x08) << 4 |
        (RightPart[0] & 0x08) << 3 |
        (LeftPart[1] & 0x08) << 2 |
        (RightPart[1] & 0x08) << 1 |
        (LeftPart[2] & 0x08) |
        (RightPart[2] & 0x08) >> 1 |
        (LeftPart[3] & 0x08) >> 2 |
        (RightPart[3] & 0x08) >> 3
        );

    srcBytes[4] = (
        (LeftPart[0] & 0x10) << 3 |
        (RightPart[0] & 0x10) << 2 |
        (LeftPart[1] & 0x10) << 1 |
        (RightPart[1] & 0x10) |
        (LeftPart[2] & 0x10) >> 1 |
        (RightPart[2] & 0x10) >> 2 |
        (LeftPart[3] & 0x10) >> 3 |
        (RightPart[3] & 0x10) >> 4
        );

    srcBytes[5] = (
        (LeftPart[0] & 0x20) << 2 |
        (RightPart[0] & 0x20) << 1 |
        (LeftPart[1] & 0x20) |
        (RightPart[1] & 0x20) >> 1 |
        (LeftPart[2] & 0x20) >> 2 |
        (RightPart[2] & 0x20) >> 3 |
        (LeftPart[3] & 0x20) >> 4 |
        (RightPart[3] & 0x20) >> 5
        );

    srcBytes[6] = (
        (LeftPart[0] & 0x40) << 1 |
        (RightPart[0] & 0x40) |
        (LeftPart[1] & 0x40) >> 1 |
        (RightPart[1] & 0x40) >> 2 |
        (LeftPart[2] & 0x40) >> 3 |
        (RightPart[2] & 0x40) >> 4 |
        (LeftPart[3] & 0x40) >> 5 |
        (RightPart[3] & 0x40) >> 6
        );

    srcBytes[7] = (
        (LeftPart[0] & 0x80) |
        (RightPart[0] & 0x80) >> 1 |
        (LeftPart[1] & 0x80) >> 2 |
        (RightPart[1] & 0x80) >> 3 |
        (LeftPart[2] & 0x80) >> 4 |
        (RightPart[2] & 0x80) >> 5 |
        (LeftPart[3] & 0x80) >> 6 |
        (RightPart[3] & 0x80) >> 7
        );

    //*reinterpret_cast<uint64_t*>(Buffer) = 0;
}

void CRYPTO_DES_DecryptBlock(uint8_t srcBytes[8], const uint8_t srcExpandedKey[16][6]) {
    uint8_t Buffer[8];

    //Start initial permutation
    Buffer[0] = (
        (srcBytes[7] & 0x40) << 1 |
        (srcBytes[6] & 0x40) |
        (srcBytes[5] & 0x40) >> 1 |
        (srcBytes[4] & 0x40) >> 2 |
        (srcBytes[3] & 0x40) >> 3 |
        (srcBytes[2] & 0x40) >> 4 |
        (srcBytes[1] & 0x40) >> 5 |
        (srcBytes[0] & 0x40) >> 6
        );

    Buffer[1] = (
        (srcBytes[7] & 0x10) << 3 |
        (srcBytes[6] & 0x10) << 2 |
        (srcBytes[5] & 0x10) << 1 |
        (srcBytes[4] & 0x10) |
        (srcBytes[3] & 0x10) >> 1 |
        (srcBytes[2] & 0x10) >> 2 |
        (srcBytes[1] & 0x10) >> 3 |
        (srcBytes[0] & 0x10) >> 4
        );

    Buffer[2] = (
        (srcBytes[7] & 0x04) << 5 |
        (srcBytes[6] & 0x04) << 4 |
        (srcBytes[5] & 0x04) << 3 |
        (srcBytes[4] & 0x04) << 2 |
        (srcBytes[3] & 0x04) << 1 |
        (srcBytes[2] & 0x04) |
        (srcBytes[1] & 0x04) >> 1 |
        (srcBytes[0] & 0x04) >> 2
        );

    Buffer[3] = (
        (srcBytes[7] & 0x01) << 7 |
        (srcBytes[6] & 0x01) << 6 |
        (srcBytes[5] & 0x01) << 5 |
        (srcBytes[4] & 0x01) << 4 |
        (srcBytes[3] & 0x01) << 3 |
        (srcBytes[2] & 0x01) << 2 |
        (srcBytes[1] & 0x01) << 1 |
        (srcBytes[0] & 0x01)
        );

    Buffer[4] = (
        (srcBytes[7] & 0x80) |
        (srcBytes[6] & 0x80) >> 1 |
        (srcBytes[5] & 0x80) >> 2 |
        (srcBytes[4] & 0x80) >> 3 |
        (srcBytes[3] & 0x80) >> 4 |
        (srcBytes[2] & 0x80) >> 5 |
        (srcBytes[1] & 0x80) >> 6 |
        (srcBytes[0] & 0x80) >> 7
        );

    Buffer[5] = (
        (srcBytes[7] & 0x20) << 2 |
        (srcBytes[6] & 0x20) << 1 |
        (srcBytes[5] & 0x20) |
        (srcBytes[4] & 0x20) >> 1 |
        (srcBytes[3] & 0x20) >> 2 |
        (srcBytes[2] & 0x20) >> 3 |
        (srcBytes[1] & 0x20) >> 4 |
        (srcBytes[0] & 0x20) >> 5
        );

    Buffer[6] = (
        (srcBytes[7] & 0x08) << 4 |
        (srcBytes[6] & 0x08) << 3 |
        (srcBytes[5] & 0x08) << 2 |
        (srcBytes[4] & 0x08) << 1 |
        (srcBytes[3] & 0x08) |
        (srcBytes[2] & 0x08) >> 1 |
        (srcBytes[1] & 0x08) >> 2 |
        (srcBytes[0] & 0x08) >> 3
        );

    Buffer[7] = (
        (srcBytes[7] & 0x02) << 6 |
        (srcBytes[6] & 0x02) << 5 |
        (srcBytes[5] & 0x02) << 4 |
        (srcBytes[4] & 0x02) << 3 |
        (srcBytes[3] & 0x02) << 2 |
        (srcBytes[2] & 0x02) << 1 |
        (srcBytes[1] & 0x02) |
        (srcBytes[0] & 0x02) >> 1
        );

    //Start deciphering
    uint8_t* const LeftPart = Buffer + 4;
    uint8_t* const RightPart = Buffer;
    for (int i = 15; i >= 0; --i) {
        uint32_t temp = *(uint32_t*)LeftPart;
        //Cipher function f
        //E BIT-SELECTION and xor by ExpandedKey
        uint8_t tmp[6];
        tmp[0] = (
            (
            (LeftPart[3] << 7) |
                (LeftPart[0] & 0xF8) >> 1 |
                (LeftPart[0] & 0x18) >> 3
                ) ^ srcExpandedKey[i][0]
            );

        tmp[1] = (
            (
            (LeftPart[0] << 5) |
                (LeftPart[1] & 0x80) >> 3 |
                (LeftPart[0] & 0x01) << 3 |
                (LeftPart[1] >> 5)
                ) ^ srcExpandedKey[i][1]
            );

        tmp[2] = (
            (
            (LeftPart[1] & 0x18) << 3 |
                (LeftPart[1] & 0x1F) << 1 |
                (LeftPart[2] >> 7)
                ) ^ srcExpandedKey[i][2]
            );

        tmp[3] = (
            (
            (LeftPart[1] << 7) |
                (LeftPart[2] & 0xF8) >> 1 |
                (LeftPart[2] & 0x18) >> 3
                ) ^ srcExpandedKey[i][3]
            );

        tmp[4] = (
            (
            (LeftPart[2] << 5) |
                (LeftPart[3] & 0x80) >> 3 |
                (LeftPart[2] & 0x01) << 3 |
                (LeftPart[3] >> 5)
                ) ^ srcExpandedKey[i][4]
            );

        tmp[5] = (
            (
            (LeftPart[3] & 0x18) << 3 |
                (LeftPart[3] & 0x1F) << 1 |
                (LeftPart[0] >> 7)
                ) ^ srcExpandedKey[i][5]
            );

        //S transformation
        LeftPart[0] = (
            CRYPTO_DES_S1[(tmp[0] & 0x80) >> 6 | (tmp[0] & 0x04) >> 2][(tmp[0] & 0x78) >> 3] << 4 |
            CRYPTO_DES_S2[(tmp[0] & 0x02) | (tmp[1] & 0x10) >> 4][(tmp[0] & 0x01) << 3 | tmp[1] >> 5]
            );

        LeftPart[1] = (
            CRYPTO_DES_S3[(tmp[1] & 0x08) >> 2 | (tmp[2] & 0x40) >> 6][(tmp[1] & 0x07) << 1 | tmp[2] >> 7] << 4 |
            CRYPTO_DES_S4[(tmp[2] & 0x20) >> 4 | (tmp[2] & 0x01)][(tmp[2] & 0x1E) >> 1]
            );

        LeftPart[2] = (
            CRYPTO_DES_S5[(tmp[3] & 0x80) >> 6 | (tmp[3] & 0x04) >> 2][(tmp[3] & 0x78) >> 3] << 4 |
            CRYPTO_DES_S6[(tmp[3] & 0x02) | (tmp[4] & 0x10) >> 4][(tmp[3] & 0x01) << 3 | tmp[4] >> 5]
            );

        LeftPart[3] = (
            CRYPTO_DES_S7[(tmp[4] & 0x08) >> 2 | (tmp[5] & 0x40) >> 6][(tmp[4] & 0x07) << 1 | tmp[5] >> 7] << 4 |
            CRYPTO_DES_S8[(tmp[5] & 0x20) >> 4 | (tmp[5] & 0x01)][(tmp[5] & 0x1E) >> 1]
            );

        //P transformation and xor by LeftPart
        tmp[0] = (
            (
            (LeftPart[1] & 0x01) << 7 |
                (LeftPart[0] & 0x02) << 5 |
                (LeftPart[2] & 0x10) << 1 |
                (LeftPart[2] & 0x08) << 1 |
                (LeftPart[3] & 0x08) |
                (LeftPart[1] & 0x10) >> 2 |
                (LeftPart[3] & 0x10) >> 3 |
                (LeftPart[2] >> 7)
                ) ^ RightPart[0]
            );

        tmp[1] = (
            (
            (LeftPart[0] & 0x80) |
                (LeftPart[1] & 0x02) << 5 |
                (LeftPart[2] & 0x02) << 4 |
                (LeftPart[3] & 0x40) >> 2 |
                (LeftPart[0] & 0x08) |
                (LeftPart[2] & 0x40) >> 4 |
                (LeftPart[3] & 0x02) |
                (LeftPart[1] & 0x40) >> 6
                ) ^ RightPart[1]
            );

        tmp[2] = (
            (
            (LeftPart[0] & 0x40) << 1 |
                (LeftPart[0] & 0x01) << 6 |
                (LeftPart[2] & 0x01) << 5 |
                (LeftPart[1] & 0x04) << 2 |
                (LeftPart[3] & 0x01) << 3 |
                (LeftPart[3] & 0x20) >> 3 |
                (LeftPart[0] & 0x20) >> 4 |
                (LeftPart[1] >> 7)
                ) ^ RightPart[2]
            );

        tmp[3] = (
            (
            (LeftPart[2] & 0x20) << 2 |
                (LeftPart[1] & 0x08) << 3 |
                (LeftPart[3] & 0x04) << 3 |
                (LeftPart[0] & 0x04) << 2 |
                (LeftPart[2] & 0x04) << 1 |
                (LeftPart[1] & 0x20) >> 3 |
                (LeftPart[0] & 0x10) >> 3 |
                (LeftPart[3] >> 7)
                ) ^ RightPart[3]
            );

        *(uint32_t*)LeftPart = *(uint32_t*)tmp;
        *(uint32_t*)RightPart = temp;
    }

    //Start inverse initial permutation
    srcBytes[0] = (
        (RightPart[0] << 7) |
        (LeftPart[0] & 0x01) << 6 |
        (RightPart[1] & 0x01) << 5 |
        (LeftPart[1] & 0x01) << 4 |
        (RightPart[2] & 0x01) << 3 |
        (LeftPart[2] & 0x01) << 2 |
        (RightPart[3] & 0x01) << 1 |
        (LeftPart[3] & 0x01)
        );

    srcBytes[1] = (
        (RightPart[0] & 0x02) << 6 |
        (LeftPart[0] & 0x02) << 5 |
        (RightPart[1] & 0x02) << 4 |
        (LeftPart[1] & 0x02) << 3 |
        (RightPart[2] & 0x02) << 2 |
        (LeftPart[2] & 0x02) << 1 |
        (RightPart[3] & 0x02) |
        (LeftPart[3] & 0x02) >> 1
        );

    srcBytes[2] = (
        (RightPart[0] & 0x04) << 5 |
        (LeftPart[0] & 0x04) << 4 |
        (RightPart[1] & 0x04) << 3 |
        (LeftPart[1] & 0x04) << 2 |
        (RightPart[2] & 0x04) << 1 |
        (LeftPart[2] & 0x04) |
        (RightPart[3] & 0x04) >> 1 |
        (LeftPart[3] & 0x04) >> 2
        );

    srcBytes[3] = (
        (RightPart[0] & 0x08) << 4 |
        (LeftPart[0] & 0x08) << 3 |
        (RightPart[1] & 0x08) << 2 |
        (LeftPart[1] & 0x08) << 1 |
        (RightPart[2] & 0x08) |
        (LeftPart[2] & 0x08) >> 1 |
        (RightPart[3] & 0x08) >> 2 |
        (LeftPart[3] & 0x08) >> 3
        );

    srcBytes[4] = (
        (RightPart[0] & 0x10) << 3 |
        (LeftPart[0] & 0x10) << 2 |
        (RightPart[1] & 0x10) << 1 |
        (LeftPart[1] & 0x10) |
        (RightPart[2] & 0x10) >> 1 |
        (LeftPart[2] & 0x10) >> 2 |
        (RightPart[3] & 0x10) >> 3 |
        (LeftPart[3] & 0x10) >> 4
        );

    srcBytes[5] = (
        (RightPart[0] & 0x20) << 2 |
        (LeftPart[0] & 0x20) << 1 |
        (RightPart[1] & 0x20) |
        (LeftPart[1] & 0x20) >> 1 |
        (RightPart[2] & 0x20) >> 2 |
        (LeftPart[2] & 0x20) >> 3 |
        (RightPart[3] & 0x20) >> 4 |
        (LeftPart[3] & 0x20) >> 5
        );

    srcBytes[6] = (
        (RightPart[0] & 0x40) << 1 |
        (LeftPart[0] & 0x40) |
        (RightPart[1] & 0x40) >> 1 |
        (LeftPart[1] & 0x40) >> 2 |
        (RightPart[2] & 0x40) >> 3 |
        (LeftPart[2] & 0x40) >> 4 |
        (RightPart[3] & 0x40) >> 5 |
        (LeftPart[3] & 0x40) >> 6
        );

    srcBytes[7] = (
        (RightPart[0] & 0x80) |
        (LeftPart[0] & 0x80) >> 1 |
        (RightPart[1] & 0x80) >> 2 |
        (LeftPart[1] & 0x80) >> 3 |
        (RightPart[2] & 0x80) >> 4 |
        (LeftPart[2] & 0x80) >> 5 |
        (RightPart[3] & 0x80) >> 6 |
        (LeftPart[3] & 0x80) >> 7
        );
}

void CRYPTO_3DES_DecryptBlock(uint8_t srcBytes[8],
                              const uint8_t srcExpandedKey1[16][6],
                              const uint8_t srcExpandedKey2[16][6],
                              const uint8_t srcExpandedKey3[16][6]) {
    CRYPTO_DES_DecryptBlock(srcBytes, srcExpandedKey3);
    CRYPTO_DES_EncryptBlock(srcBytes, srcExpandedKey2);
    CRYPTO_DES_DecryptBlock(srcBytes, srcExpandedKey1);
}

void CRYPTO_3DES_EncryptBlock(uint8_t srcBytes[8],
                              const uint8_t srcExpandedKey1[16][6],
                              const uint8_t srcExpandedKey2[16][6],
                              const uint8_t srcExpandedKey3[16][6]) {
    CRYPTO_DES_EncryptBlock(srcBytes, srcExpandedKey1);
    CRYPTO_DES_DecryptBlock(srcBytes, srcExpandedKey2);
    CRYPTO_DES_EncryptBlock(srcBytes, srcExpandedKey3);
}

int CRYPTO_DES_KeyExpansion(const uint8_t srcKey[8], uint8_t dstExpandedKey[16][6]) {
    //check if cipher key is legal
    for (int i = 0; i < 8; ++i) {
        uint8_t temp = srcKey[i];
        uint8_t count = 0;

        while (temp) {
            temp &= temp - 1;
            ++count;
        }

        if (count % 2 == 0)
            return CRYPTO_DES_INVALID_KEY;
    }

    //Permuted choice 1
    uint8_t C_Part[4];
    uint8_t D_Part[4];
    C_Part[0] = (
        (srcKey[7] & 0x80) |
        (srcKey[6] & 0x80) >> 1 |
        (srcKey[5] & 0x80) >> 2 |
        (srcKey[4] & 0x80) >> 3 |
        (srcKey[3] & 0x80) >> 4 |
        (srcKey[2] & 0x80) >> 5 |
        (srcKey[1] & 0x80) >> 6 |
        (srcKey[0] & 0x80) >> 7
        );

    C_Part[1] = (
        (srcKey[7] & 0x40) << 1 |
        (srcKey[6] & 0x40) |
        (srcKey[5] & 0x40) >> 1 |
        (srcKey[4] & 0x40) >> 2 |
        (srcKey[3] & 0x40) >> 3 |
        (srcKey[2] & 0x40) >> 4 |
        (srcKey[1] & 0x40) >> 5 |
        (srcKey[0] & 0x40) >> 6
        );

    C_Part[2] = (
        (srcKey[7] & 0x20) << 2 |
        (srcKey[6] & 0x20) << 1 |
        (srcKey[5] & 0x20) |
        (srcKey[4] & 0x20) >> 1 |
        (srcKey[3] & 0x20) >> 2 |
        (srcKey[2] & 0x20) >> 3 |
        (srcKey[1] & 0x20) >> 4 |
        (srcKey[0] & 0x20) >> 5
        );

    C_Part[3] = (
        (srcKey[7] & 0x10) << 3 |
        (srcKey[6] & 0x10) << 2 |
        (srcKey[5] & 0x10) << 1 |
        (srcKey[4] & 0x10)
        );

    D_Part[0] = (
        (srcKey[7] & 0x02) << 6 |
        (srcKey[6] & 0x02) << 5 |
        (srcKey[5] & 0x02) << 4 |
        (srcKey[4] & 0x02) << 3 |
        (srcKey[3] & 0x02) << 2 |
        (srcKey[2] & 0x02) << 1 |
        (srcKey[1] & 0x02) |
        (srcKey[0] & 0x02) >> 1
        );

    D_Part[1] = (
        (srcKey[7] & 0x04) << 5 |
        (srcKey[6] & 0x04) << 4 |
        (srcKey[5] & 0x04) << 3 |
        (srcKey[4] & 0x04) << 2 |
        (srcKey[3] & 0x04) << 1 |
        (srcKey[2] & 0x04) |
        (srcKey[1] & 0x04) >> 1 |
        (srcKey[0] & 0x04) >> 2
        );

    D_Part[2] = (
        (srcKey[7] & 0x08) << 4 |
        (srcKey[6] & 0x08) << 3 |
        (srcKey[5] & 0x08) << 2 |
        (srcKey[4] & 0x08) << 1 |
        (srcKey[3] & 0x08) |
        (srcKey[2] & 0x08) >> 1 |
        (srcKey[1] & 0x08) >> 2 |
        (srcKey[0] & 0x08) >> 3
        );

    D_Part[3] = (
        (srcKey[3] & 0x10) << 3 |
        (srcKey[2] & 0x10) << 2 |
        (srcKey[1] & 0x10) << 1 |
        (srcKey[0] & 0x10)
        );

    //Generate K
#define ROL_28Bits(x, s) x[3] |= ((x[0] >> (8 - s)) << (4 - s)); \
    x[0] = (x[0] << s | x[1] >> (8 - s)); \
    x[1] = (x[1] << s | x[2] >> (8 - s)); \
    x[2] = (x[2] << s | x[3] >> (8 - s)); \
    x[3] = (x[3] << s);

    const uint8_t NumberOfROL[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
    for (int i = 0; i < 16; ++i) {
        ROL_28Bits(C_Part, NumberOfROL[i])
        ROL_28Bits(D_Part, NumberOfROL[i])
        //Permuted choice 2
        dstExpandedKey[i][0] = (
        (C_Part[1] & 0x04) << 5 |
            (C_Part[2] & 0x80) >> 1 |
            (C_Part[1] & 0x20) |
            (C_Part[2] & 0x01) << 4 |
            (C_Part[0] & 0x80) >> 4 |
            (C_Part[0] & 0x08) >> 1 |
            (C_Part[0] & 0x20) >> 4 |
            (C_Part[3] & 0x10) >> 4
            );

        dstExpandedKey[i][1] = (
            (C_Part[1] & 0x02) << 6 |
            (C_Part[0] & 0x04) << 4 |
            (C_Part[2] & 0x08) << 2 |
            (C_Part[1] & 0x40) >> 2 |
            (C_Part[2] & 0x02) << 2 |
            (C_Part[2] & 0x20) >> 3 |
            (C_Part[1] & 0x10) >> 3 |
            (C_Part[0] & 0x10) >> 4
            );

        dstExpandedKey[i][2] = (
            (C_Part[3] & 0x40) << 1 |
            (C_Part[0] & 0x01) << 6 |
            (C_Part[1] & 0x01) << 5 |
            (C_Part[0] & 0x02) << 3 |
            (C_Part[3] & 0x20) >> 2 |
            (C_Part[2] & 0x10) >> 2 |
            (C_Part[1] & 0x08) >> 2 |
            (C_Part[0] & 0x40) >> 6
            );

        dstExpandedKey[i][3] = (
            (D_Part[1] & 0x08) << 4 |
            (D_Part[2] & 0x01) << 6 |
            (D_Part[0] & 0x20) |
            (D_Part[1] & 0x80) >> 3 |
            (D_Part[2] & 0x20) >> 2 |
            (D_Part[3] & 0x20) >> 3 |
            (D_Part[0] & 0x40) >> 5 |
            (D_Part[1] & 0x10) >> 4
            );

        dstExpandedKey[i][4] = (
            (D_Part[2] & 0x02) << 6 |
            (D_Part[2] & 0x80) >> 1 |
            (D_Part[0] & 0x08) << 2 |
            (D_Part[2] & 0x10) |
            (D_Part[1] & 0x01) << 3 |
            (D_Part[2] & 0x08) >> 1 |
            (D_Part[1] & 0x20) >> 4 |
            (D_Part[3] & 0x10) >> 4
            );

        dstExpandedKey[i][5] = (
            (D_Part[0] & 0x04) << 5 |
            (D_Part[3] & 0x80) >> 1 |
            (D_Part[2] & 0x40) >> 1 |
            (D_Part[1] & 0x04) << 2 |
            (D_Part[2] & 0x04) << 1 |
            (D_Part[0] & 0x01) << 2 |
            (D_Part[0] & 0x80) >> 6 |
            (D_Part[0] & 0x10) >> 4
            );
    }

    return CRYPTO_DES_SUCCESS;
}
