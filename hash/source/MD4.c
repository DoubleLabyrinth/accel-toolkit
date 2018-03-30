#include "../MD4.h"
#include <x86intrin.h>
#include <memory.h>

#define MD4_BLOCKSIZE 64

void accelc_MD4_init(MD4_BUFFER* HashBuffer) {
    HashBuffer->dword[0] = 0x67452301;
    HashBuffer->dword[1] = 0xEFCDAB89;
    HashBuffer->dword[2] = 0x98BADCFE;
    HashBuffer->dword[3] = 0x10325476;
}

void accelc_MD4_update(const void* srcBytes, size_t srcBytesLength, MD4_BUFFER* HashBuffer) {
    uint32_t AA = 0, BB = 0, CC = 0, DD = 0;
    size_t RoundsOfMainCycle = srcBytesLength / MD4_BLOCKSIZE;

    const uint32_t(*Ptr)[16] = (const uint32_t (*)[16])srcBytes;

    for (size_t i = 0; i < RoundsOfMainCycle; ++i) {
        AA = HashBuffer->dword[0];
        BB = HashBuffer->dword[1];
        CC = HashBuffer->dword[2];
        DD = HashBuffer->dword[3];

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


        MD4_FF(HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], Ptr[i][0], 3)  //MD4_FF is a MACRO, no need to add ';'
        MD4_FF(HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], Ptr[i][1], 7)
        MD4_FF(HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], Ptr[i][2], 11)
        MD4_FF(HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], Ptr[i][3], 19)
        MD4_FF(HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], Ptr[i][4], 3)
        MD4_FF(HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], Ptr[i][5], 7)
        MD4_FF(HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], Ptr[i][6], 11)
        MD4_FF(HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], Ptr[i][7], 19)
        MD4_FF(HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], Ptr[i][8], 3)
        MD4_FF(HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], Ptr[i][9], 7)
        MD4_FF(HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], Ptr[i][10], 11)
        MD4_FF(HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], Ptr[i][11], 19)
        MD4_FF(HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], Ptr[i][12], 3)
        MD4_FF(HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], Ptr[i][13], 7)
        MD4_FF(HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], Ptr[i][14], 11)
        MD4_FF(HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], Ptr[i][15], 19)

        MD4_GG(HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], Ptr[i][0], 3)
        MD4_GG(HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], Ptr[i][4], 5)
        MD4_GG(HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], Ptr[i][8], 9)
        MD4_GG(HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], Ptr[i][12], 13)
        MD4_GG(HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], Ptr[i][1], 3)
        MD4_GG(HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], Ptr[i][5], 5)
        MD4_GG(HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], Ptr[i][9], 9)
        MD4_GG(HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], Ptr[i][13], 13)
        MD4_GG(HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], Ptr[i][2], 3)
        MD4_GG(HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], Ptr[i][6], 5)
        MD4_GG(HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], Ptr[i][10], 9)
        MD4_GG(HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], Ptr[i][14], 13)
        MD4_GG(HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], Ptr[i][3], 3)
        MD4_GG(HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], Ptr[i][7], 5)
        MD4_GG(HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], Ptr[i][11], 9)
        MD4_GG(HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], Ptr[i][15], 13)

        MD4_HH(HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], Ptr[i][0], 3)
        MD4_HH(HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], Ptr[i][8], 9)
        MD4_HH(HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], Ptr[i][4], 11)
        MD4_HH(HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], Ptr[i][12], 15)
        MD4_HH(HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], Ptr[i][2], 3)
        MD4_HH(HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], Ptr[i][10], 9)
        MD4_HH(HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], Ptr[i][6], 11)
        MD4_HH(HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], Ptr[i][14], 15)
        MD4_HH(HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], Ptr[i][1], 3)
        MD4_HH(HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], Ptr[i][9], 9)
        MD4_HH(HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], Ptr[i][5], 11)
        MD4_HH(HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], Ptr[i][13], 15)
        MD4_HH(HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], Ptr[i][3], 3)
        MD4_HH(HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], HashBuffer->dword[2], Ptr[i][11], 9)
        MD4_HH(HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], HashBuffer->dword[1], Ptr[i][7], 11)
        MD4_HH(HashBuffer->dword[1], HashBuffer->dword[2], HashBuffer->dword[3], HashBuffer->dword[0], Ptr[i][15], 15)
        
        HashBuffer->dword[0] += AA;
        HashBuffer->dword[1] += BB;
        HashBuffer->dword[2] += CC;
        HashBuffer->dword[3] += DD;
    }
}

void accelc_MD4_final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength,
                      const MD4_BUFFER* HashBuffer, MD4_DIGEST* Hash) {
    memcpy(Hash, HashBuffer, sizeof(MD4_BUFFER));
    if (LeftBytesLength >= MD4_BLOCKSIZE) {
        accelc_MD4_update(LeftBytes, LeftBytesLength, Hash);
        LeftBytes = (const uint8_t*)LeftBytes + (LeftBytesLength / MD4_BLOCKSIZE) * MD4_BLOCKSIZE;
        LeftBytesLength %= MD4_BLOCKSIZE;
    }

    uint8_t Extra[128] = { 0 };
    for (size_t i = 0; i < LeftBytesLength; ++i)
        Extra[i] = ((const uint8_t*)LeftBytes)[i];

    Extra[LeftBytesLength] = 0x80;
    *(uint64_t*)(Extra + (LeftBytesLength >= 64 - 8 ? 128 - 8 : 64 - 8)) = TotalBytesLength * 8;

    accelc_MD4_update(Extra, LeftBytesLength >= 64 - 8 ? 128 : 64, Hash);
}

void accelc_MD4(const void* srcBytes, size_t srclen, MD4_DIGEST* Hash) {
    MD4_BUFFER hash_buf;
    accelc_MD4_init(&hash_buf);
    accelc_MD4_update(srcBytes, srclen, &hash_buf);
    accelc_MD4_final((uint8_t*)srcBytes + (srclen / MD4_BLOCKSIZE) * MD4_BLOCKSIZE, srclen % MD4_BLOCKSIZE, srclen, &hash_buf, Hash);
}