/*********************************************************************
* Filename:   MD5.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../MD5.h"
#include <intrin.h>

#define HASH_MD5_BLOCKSIZE 64

void HASH_MD5_Initialize(uint32_t HashBuffer[4]) {
    HashBuffer[0] = 0x67452301;
    HashBuffer[1] = 0xEFCDAB89;
    HashBuffer[2] = 0x98BADCFE;
    HashBuffer[3] = 0x10325476;
}

void HASH_MD5_MainCycle(const void* srcBytes, size_t srcBytesLength, uint32_t HashBuffer[4]) {
    uint32_t AA = 0, BB = 0, CC = 0, DD = 0;
    size_t RoundsOfMainCycle = srcBytesLength / 64;
    const uint32_t(*Ptr)[16] = srcBytes;

    for (size_t i = 0; i < RoundsOfMainCycle; ++i) {
        AA = HashBuffer[0];
        BB = HashBuffer[1];
        CC = HashBuffer[2];
        DD = HashBuffer[3];

#define MD5_F(X, Y, Z) ((X & Y) | (~X & Z))
#define MD5_G(X, Y, Z) ((X & Z) | (Y & ~Z))
#define MD5_H(X, Y, Z) (X ^ Y ^ Z)
#define MD5_I(X, Y, Z) (Y ^ (X | ~Z))

#define MD5_FF(A, B, C, D, K, s, T)		\
    A += MD5_F(B, C, D) + K + T;		\
    A = _rotl(A, s);					\
    A += B;

#define MD5_GG(A, B, C, D, K, s, T)		\
    A += MD5_G(B, C, D) + K + T;		\
    A = _rotl(A, s);					\
    A += B;

#define MD5_HH(A, B, C, D, K, s, T)		\
    A += MD5_H(B, C, D) + K + T;		\
    A = _rotl(A, s);					\
    A += B;

#define MD5_II(A, B, C, D, K, s, T)		\
    A += MD5_I(B, C, D) + K + T;		\
    A = _rotl(A, s);					\
    A += B;

        MD5_FF(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][0], 7, 0xD76AA478)  //MD5_FF is a MACRO, no need to add ';'
        MD5_FF(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][1], 12, 0xE8C7B756)
        MD5_FF(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][2], 17, 0x242070DB)
        MD5_FF(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][3], 22, 0xC1BDCEEE)
        MD5_FF(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][4], 7, 0xF57C0FAF)
        MD5_FF(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][5], 12, 0x4787C62A)
        MD5_FF(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][6], 17, 0xA8304613)
        MD5_FF(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][7], 22, 0xFD469501)
        MD5_FF(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][8], 7, 0x698098D8)
        MD5_FF(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][9], 12, 0x8B44F7AF)
        MD5_FF(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][10], 17, 0xFFFF5BB1)
        MD5_FF(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][11], 22, 0x895CD7BE)
        MD5_FF(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][12], 7, 0x6B901122)
        MD5_FF(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][13], 12, 0xFD987193)
        MD5_FF(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][14], 17, 0xA679438E)
        MD5_FF(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][15], 22, 0x49B40821)

        MD5_GG(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][1], 5, 0xF61E2562)
        MD5_GG(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][6], 9, 0xC040B340)
        MD5_GG(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][11], 14, 0x265E5A51)
        MD5_GG(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][0], 20, 0xE9B6C7AA)
        MD5_GG(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][5], 5, 0xD62F105D)
        MD5_GG(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][10], 9, 0x02441453)
        MD5_GG(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][15], 14, 0xD8A1E681)
        MD5_GG(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][4], 20, 0xE7D3FBC8)
        MD5_GG(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][9], 5, 0x21E1CDE6)
        MD5_GG(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][14], 9, 0xC33707D6)
        MD5_GG(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][3], 14, 0xF4D50D87)
        MD5_GG(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][8], 20, 0x455A14ED)
        MD5_GG(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][13], 5, 0xA9E3E905)
        MD5_GG(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][2], 9, 0xFCEFA3F8)
        MD5_GG(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][7], 14, 0x676F02D9)
        MD5_GG(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][12], 20, 0x8D2A4C8A)

        MD5_HH(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][5], 4, 0xFFFA3942)
        MD5_HH(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][8], 11, 0x8771F681)
        MD5_HH(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][11], 16, 0x6D9D6122)
        MD5_HH(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][14], 23, 0xFDE5380C)
        MD5_HH(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][1], 4, 0xA4BEEA44)
        MD5_HH(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][4], 11, 0x4BDECFA9)
        MD5_HH(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][7], 16, 0xF6BB4B60)
        MD5_HH(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][10], 23, 0xBEBFBC70)
        MD5_HH(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][13], 4, 0x289B7EC6)
        MD5_HH(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][0], 11, 0xEAA127FA)
        MD5_HH(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][3], 16, 0xD4EF3085)
        MD5_HH(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][6], 23, 0x04881D05)
        MD5_HH(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][9], 4, 0xD9D4D039)
        MD5_HH(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][12], 11, 0xE6DB99E5)
        MD5_HH(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][15], 16, 0x1FA27CF8)
        MD5_HH(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][2], 23, 0xC4AC5665)

        MD5_II(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][0], 6, 0xF4292244)
        MD5_II(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][7], 10, 0x432AFF97)
        MD5_II(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][14], 15, 0xAB9423A7)
        MD5_II(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][5], 21, 0xFC93A039)
        MD5_II(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][12], 6, 0x655B59C3)
        MD5_II(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][3], 10, 0x8F0CCC92)
        MD5_II(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][10], 15, 0xFFEFF47D)
        MD5_II(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][1], 21, 0x85845DD1)
        MD5_II(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][8], 6, 0x6FA87E4F)
        MD5_II(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][15], 10, 0xFE2CE6E0)
        MD5_II(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][6], 15, 0xA3014314)
        MD5_II(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][13], 21, 0x4E0811A1)
        MD5_II(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][4], 6, 0xF7537E82)
        MD5_II(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][11], 10, 0xBD3AF235)
        MD5_II(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][2], 15, 0x2AD7D2BB)
        MD5_II(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][9], 21, 0xEB86D391)

#undef MD5_F
#undef MD5_G
#undef MD5_H
#undef MD5_I
#undef MD5_FF
#undef MD5_GG
#undef MD5_HH
#undef MD5_II

        HashBuffer[0] += AA;
        HashBuffer[1] += BB;
        HashBuffer[2] += CC;
        HashBuffer[3] += DD;
    }
}

void HASH_MD5_Final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength, uint32_t HashBuffer[4]) {
    if (LeftBytesLength >= HASH_MD5_BLOCKSIZE) {
        HASH_MD5_MainCycle(LeftBytes, LeftBytesLength, HashBuffer);
        LeftBytes = (const uint8_t*)LeftBytes + (LeftBytesLength / HASH_MD5_BLOCKSIZE) * HASH_MD5_BLOCKSIZE;
        LeftBytesLength %= HASH_MD5_BLOCKSIZE;
    }

    uint8_t Extra[128] = { 0 };
    for (size_t i = 0; i < LeftBytesLength; ++i)
        Extra[i] = ((const uint8_t*)LeftBytes)[i];

    Extra[LeftBytesLength] = 0x80;
    *(uint64_t*)(Extra + (LeftBytesLength >= 64 - 8 ? 128 - 8 : 64 - 8)) = TotalBytesLength * 8;

    HASH_MD5_MainCycle(Extra, LeftBytesLength >= 64 - 8 ? 128 : 64, HashBuffer);
}


