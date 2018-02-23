/*********************************************************************
* Filename:   AES256.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../AES256.h"
#include <intrin.h>

extern const uint8_t CRYPTO_AES_SBox[256];
extern const uint32_t CRYPTO_AES_Rcon[11];
extern const uint8_t CRYPTO_AES_Multiply0x02[256];
extern const uint8_t CRYPTO_AES_Multiply0x03[256];
extern const uint8_t CRYPTO_AES_InverseSBox[256];
extern const uint8_t CRYPTO_AES_Multiply0x09[256];
extern const uint8_t CRYPTO_AES_Multiply0x0B[256];
extern const uint8_t CRYPTO_AES_Multiply0x0D[256];
extern const uint8_t CRYPTO_AES_Multiply0x0E[256];

void CRYPTO_AES256_EncryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[60]) {
    ((uint64_t*)srcBytes)[0] ^= ((const uint64_t*)srcExpandedKey)[0];
    ((uint64_t*)srcBytes)[1] ^= ((const uint64_t*)srcExpandedKey)[1];

    uint8_t ShiftTemp = 0;

#define Swap(X, Y, Temp) Temp = X;\
    X = Y;\
    Y = Temp;

    for (int i = 1; i < 14; ++i) {
        for (int j = 0; j < 16; ++j)
            srcBytes[j] = CRYPTO_AES_SBox[srcBytes[j]];

        //Shift rows starts;
        //Shift the second row;
        Swap(srcBytes[1], srcBytes[5], ShiftTemp)  //Swap is a MACRO, no need to add ';'.
        Swap(srcBytes[5], srcBytes[9], ShiftTemp)
        Swap(srcBytes[9], srcBytes[13], ShiftTemp)
        //Shift the third row;
        Swap(srcBytes[2], srcBytes[10], ShiftTemp)
        Swap(srcBytes[6], srcBytes[14], ShiftTemp)
        //Shift the fourth row;
        Swap(srcBytes[3], srcBytes[15], ShiftTemp)
        Swap(srcBytes[15], srcBytes[11], ShiftTemp)
        Swap(srcBytes[11], srcBytes[7], ShiftTemp)
        //Shift rows ends;

        for (int j = 0; j < 16; j += 4) {
            uint8_t temp[4];
            *(uint32_t*)temp = ((uint32_t*)srcBytes)[j / 4];

            srcBytes[j] = (uint8_t)(CRYPTO_AES_Multiply0x02[temp[0]] ^ CRYPTO_AES_Multiply0x03[temp[1]] ^ temp[2] ^ temp[3]);
            srcBytes[j + 1] = (uint8_t)(temp[0] ^ CRYPTO_AES_Multiply0x02[temp[1]] ^ CRYPTO_AES_Multiply0x03[temp[2]] ^ temp[3]);
            srcBytes[j + 2] = (uint8_t)(temp[0] ^ temp[1] ^ CRYPTO_AES_Multiply0x02[temp[2]] ^ CRYPTO_AES_Multiply0x03[temp[3]]);
            srcBytes[j + 3] = (uint8_t)(CRYPTO_AES_Multiply0x03[temp[0]] ^ temp[1] ^ temp[2] ^ CRYPTO_AES_Multiply0x02[temp[3]]);
        }

        ((uint64_t*)srcBytes)[0] ^= ((const uint64_t*)srcExpandedKey)[i * 2];
        ((uint64_t*)srcBytes)[1] ^= ((const uint64_t*)srcExpandedKey)[i * 2 + 1];
    }

    for (int j = 0; j < 16; ++j)
        srcBytes[j] = CRYPTO_AES_SBox[srcBytes[j]];

    //Shift rows starts;
    //Shift the second row;
    Swap(srcBytes[1], srcBytes[5], ShiftTemp)  //Swap is a MACRO, no need to add ';'.
    Swap(srcBytes[5], srcBytes[9], ShiftTemp)
    Swap(srcBytes[9], srcBytes[13], ShiftTemp)
    //Shift the third row;
    Swap(srcBytes[2], srcBytes[10], ShiftTemp)
    Swap(srcBytes[6], srcBytes[14], ShiftTemp)
    //Shift the fourth row;
    Swap(srcBytes[3], srcBytes[15], ShiftTemp)
    Swap(srcBytes[15], srcBytes[11], ShiftTemp)
    Swap(srcBytes[11], srcBytes[7], ShiftTemp)
    //Shift rows ends;

    ((uint64_t*)srcBytes)[0] ^= ((const uint64_t*)srcExpandedKey)[28];
    ((uint64_t*)srcBytes)[1] ^= ((const uint64_t*)srcExpandedKey)[29];
}

void CRYPTO_AES256_DecryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[60]) {
    ((uint64_t*)srcBytes)[0] ^= ((const uint64_t*)srcExpandedKey)[28];
    ((uint64_t*)srcBytes)[1] ^= ((const uint64_t*)srcExpandedKey)[29];

    uint8_t ShiftTemp = 0;

    for (int i = 13; i > 0; --i) {
        //Inverse Shift rows starts;
        //Inverse shift the second row;
        Swap(srcBytes[13], srcBytes[9], ShiftTemp)
        Swap(srcBytes[9], srcBytes[5], ShiftTemp)
        Swap(srcBytes[5], srcBytes[1], ShiftTemp)
        //Inverse shift the third row;
        Swap(srcBytes[14], srcBytes[6], ShiftTemp)
        Swap(srcBytes[10], srcBytes[2], ShiftTemp)
        //Inverse shift the fourth row;
        Swap(srcBytes[3], srcBytes[7], ShiftTemp)
        Swap(srcBytes[7], srcBytes[11], ShiftTemp)
        Swap(srcBytes[11], srcBytes[15], ShiftTemp)

        for (int j = 0; j < 16; ++j)
            srcBytes[j] = CRYPTO_AES_InverseSBox[srcBytes[j]];

        ((uint64_t*)srcBytes)[0] ^= ((const uint64_t*)srcExpandedKey)[i * 2];
        ((uint64_t*)srcBytes)[1] ^= ((const uint64_t*)srcExpandedKey)[i * 2 + 1];

        for (int j = 0; j < 16; j += 4) {
            uint8_t temp[4];
            *(uint32_t*)temp = ((uint32_t*)srcBytes)[j / 4];

            srcBytes[j] = (uint8_t)(CRYPTO_AES_Multiply0x0E[temp[0]] ^ CRYPTO_AES_Multiply0x0B[temp[1]] ^ CRYPTO_AES_Multiply0x0D[temp[2]] ^ CRYPTO_AES_Multiply0x09[temp[3]]);
            srcBytes[j + 1] = (uint8_t)(CRYPTO_AES_Multiply0x09[temp[0]] ^ CRYPTO_AES_Multiply0x0E[temp[1]] ^ CRYPTO_AES_Multiply0x0B[temp[2]] ^ CRYPTO_AES_Multiply0x0D[temp[3]]);
            srcBytes[j + 2] = (uint8_t)(CRYPTO_AES_Multiply0x0D[temp[0]] ^ CRYPTO_AES_Multiply0x09[temp[1]] ^ CRYPTO_AES_Multiply0x0E[temp[2]] ^ CRYPTO_AES_Multiply0x0B[temp[3]]);
            srcBytes[j + 3] = (uint8_t)(CRYPTO_AES_Multiply0x0B[temp[0]] ^ CRYPTO_AES_Multiply0x0D[temp[1]] ^ CRYPTO_AES_Multiply0x09[temp[2]] ^ CRYPTO_AES_Multiply0x0E[temp[3]]);
        }
    }

    //Inverse Shift rows starts;
    //Inverse shift the second row;
    Swap(srcBytes[13], srcBytes[9], ShiftTemp)
    Swap(srcBytes[9], srcBytes[5], ShiftTemp)
    Swap(srcBytes[5], srcBytes[1], ShiftTemp)
    //Inverse shift the third row;
    Swap(srcBytes[14], srcBytes[6], ShiftTemp)
    Swap(srcBytes[10], srcBytes[2], ShiftTemp)
    //Inverse shift the fourth row;
    Swap(srcBytes[3], srcBytes[7], ShiftTemp)
    Swap(srcBytes[7], srcBytes[11], ShiftTemp)
    Swap(srcBytes[11], srcBytes[15], ShiftTemp)

    for (int j = 0; j < 16; ++j)
        srcBytes[j] = CRYPTO_AES_InverseSBox[srcBytes[j]];

    ((uint64_t*)srcBytes)[0] ^= ((const uint64_t*)srcExpandedKey)[0];
    ((uint64_t*)srcBytes)[1] ^= ((const uint64_t*)srcExpandedKey)[1];
}

void CRYPTO_AES256_KeyExpansion(const uint8_t srcKey[32], uint32_t dstExpandedKey[60]) {
    ((uint64_t*)dstExpandedKey)[0] = ((const uint64_t*)srcKey)[0];
    ((uint64_t*)dstExpandedKey)[1] = ((const uint64_t*)srcKey)[1];
    ((uint64_t*)dstExpandedKey)[2] = ((const uint64_t*)srcKey)[2];
    ((uint64_t*)dstExpandedKey)[3] = ((const uint64_t*)srcKey)[3];

    for (int i = 8; i < 60; ++i) {
        uint32_t temp = dstExpandedKey[i - 1];
        if (i % 8 == 0) {
            temp = _rotr(temp, 8);
            ((uint8_t*)&temp)[0] = CRYPTO_AES_SBox[((uint8_t*)&temp)[0]];
            ((uint8_t*)&temp)[1] = CRYPTO_AES_SBox[((uint8_t*)&temp)[1]];
            ((uint8_t*)&temp)[2] = CRYPTO_AES_SBox[((uint8_t*)&temp)[2]];
            ((uint8_t*)&temp)[3] = CRYPTO_AES_SBox[((uint8_t*)&temp)[3]];
            temp ^= CRYPTO_AES_Rcon[i / 8];
        }
        if (i % 8 == 4) {
            ((uint8_t*)&temp)[0] = CRYPTO_AES_SBox[((uint8_t*)&temp)[0]];
            ((uint8_t*)&temp)[1] = CRYPTO_AES_SBox[((uint8_t*)&temp)[1]];
            ((uint8_t*)&temp)[2] = CRYPTO_AES_SBox[((uint8_t*)&temp)[2]];
            ((uint8_t*)&temp)[3] = CRYPTO_AES_SBox[((uint8_t*)&temp)[3]];
        }
        dstExpandedKey[i] = dstExpandedKey[i - 8] ^ temp;
    }
}
