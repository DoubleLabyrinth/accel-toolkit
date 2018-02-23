/*********************************************************************
* Filename:   MD4.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../MD4.h"
#include <intrin.h>

#define HASH_MD4_BLOCKSIZE 64

void HASH_MD4_Initialize(uint32_t HashBuffer[4]) {
    HashBuffer[0] = 0x67452301;
    HashBuffer[1] = 0xEFCDAB89;
    HashBuffer[2] = 0x98BADCFE;
    HashBuffer[3] = 0x10325476;
}

void HASH_MD4_MainCycle(const void* srcBytes, size_t srcBytesLength, uint32_t HashBuffer[4]) {
    uint32_t AA = 0, BB = 0, CC = 0, DD = 0;
    size_t RoundsOfMainCycle = srcBytesLength / 64;

    const uint32_t(*Ptr)[16] = (const uint32_t (*)[16])srcBytes;

    for (size_t i = 0; i < RoundsOfMainCycle; ++i) {
        AA = HashBuffer[0];
        BB = HashBuffer[1];
        CC = HashBuffer[2];
        DD = HashBuffer[3];

#define MD4_F(X, Y, Z) (((X) & (Y)) | (~(X) & (Z)))
#define MD4_G(X, Y, Z) (((X) & (Y)) | ((X) & (Z)) | ((Y) & (Z)))
#define MD4_H(X, Y, Z) ((X) ^ (Y) ^ (Z))

#define MD4_FF(A, B, C, D, K, s)        \
(A) += MD4_F((B), (C), (D)) + (K);      \
(A) = _rotl((A), (s));                  \


#define MD4_GG(A, B, C, D, K, s)                \
(A) += MD4_G((B), (C), (D)) + (K) + 0x5A827999; \
(A) = _rotl((A), (s));                          \


#define MD4_HH(A, B, C, D, K, s)                \
(A) += MD4_H((B), (C), (D)) + (K) + 0x6ED9EBA1; \
(A) = _rotl((A), (s));                          \


        MD4_FF(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][0], 3)  //MD4_FF is a MACRO, no need to add ';'
        MD4_FF(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][1], 7)
        MD4_FF(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][2], 11)
        MD4_FF(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][3], 19)
        MD4_FF(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][4], 3)
        MD4_FF(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][5], 7)
        MD4_FF(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][6], 11)
        MD4_FF(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][7], 19)
        MD4_FF(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][8], 3)
        MD4_FF(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][9], 7)
        MD4_FF(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][10], 11)
        MD4_FF(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][11], 19)
        MD4_FF(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][12], 3)
        MD4_FF(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][13], 7)
        MD4_FF(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][14], 11)
        MD4_FF(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][15], 19)

        MD4_GG(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][0], 3)
        MD4_GG(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][4], 5)
        MD4_GG(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][8], 9)
        MD4_GG(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][12], 13)
        MD4_GG(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][1], 3)
        MD4_GG(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][5], 5)
        MD4_GG(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][9], 9)
        MD4_GG(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][13], 13)
        MD4_GG(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][2], 3)
        MD4_GG(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][6], 5)
        MD4_GG(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][10], 9)
        MD4_GG(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][14], 13)
        MD4_GG(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][3], 3)
        MD4_GG(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][7], 5)
        MD4_GG(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][11], 9)
        MD4_GG(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][15], 13)

        MD4_HH(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][0], 3)
        MD4_HH(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][8], 9)
        MD4_HH(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][4], 11)
        MD4_HH(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][12], 15)
        MD4_HH(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][2], 3)
        MD4_HH(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][10], 9)
        MD4_HH(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][6], 11)
        MD4_HH(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][14], 15)
        MD4_HH(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][1], 3)
        MD4_HH(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][9], 9)
        MD4_HH(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][5], 11)
        MD4_HH(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][13], 15)
        MD4_HH(HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], Ptr[i][3], 3)
        MD4_HH(HashBuffer[3], HashBuffer[0], HashBuffer[1], HashBuffer[2], Ptr[i][11], 9)
        MD4_HH(HashBuffer[2], HashBuffer[3], HashBuffer[0], HashBuffer[1], Ptr[i][7], 11)
        MD4_HH(HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[0], Ptr[i][15], 15)
        
        HashBuffer[0] += AA;
        HashBuffer[1] += BB;
        HashBuffer[2] += CC;
        HashBuffer[3] += DD;
    }
}

void HASH_MD4_Final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength, uint32_t HashBuffer[4]) {
    if (LeftBytesLength >= HASH_MD4_BLOCKSIZE) {
        HASH_MD4_MainCycle(LeftBytes, LeftBytesLength, HashBuffer);
        LeftBytes = (const uint8_t*)LeftBytes + (LeftBytesLength / HASH_MD4_BLOCKSIZE) * HASH_MD4_BLOCKSIZE;
        LeftBytesLength %= HASH_MD4_BLOCKSIZE;
    }

    uint8_t Extra[128] = { 0 };
    for (size_t i = 0; i < LeftBytesLength; ++i)
        Extra[i] = ((const uint8_t*)LeftBytes)[i];

    Extra[LeftBytesLength] = 0x80;
    *(uint64_t*)(Extra + (LeftBytesLength >= 64 - 8 ? 128 - 8 : 64 - 8)) = TotalBytesLength * 8;

    HASH_MD4_MainCycle(Extra, LeftBytesLength >= 64 - 8 ? 128 : 64, HashBuffer);
}
