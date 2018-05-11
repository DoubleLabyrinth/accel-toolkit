#include "../ripemd.h"

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#endif

#define RIPEMD160_BLOCKSIZE 64

void accelc_RIPEMD160_init(RIPEMD160_BUFFER* HashBuffer) {
    HashBuffer->dword[0] = 0x67452301;
    HashBuffer->dword[1] = 0xefcdab89;
    HashBuffer->dword[2] = 0x98badcfe;
    HashBuffer->dword[3] = 0x10325476;
    HashBuffer->dword[4] = 0xc3d2e1f0;
}

static inline uint32_t F(uint32_t x, uint32_t y, uint32_t z) {
    return x ^ y ^ z;
}

static inline uint32_t G(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) | (~x & z);
}

static inline uint32_t H(uint32_t x, uint32_t y, uint32_t z) {
    return (x | ~y) ^ z;
}

static inline uint32_t I(uint32_t x, uint32_t y, uint32_t z) {
    return (x & z) | (y & ~z);
}

static inline uint32_t J(uint32_t x, uint32_t y, uint32_t z) {
    return x ^ (y | ~z);
}

#define OperateX(Func, T, A, B, C, D, E, X, K, s)                   \
    (T) = _rotl((A) + Func((B), (C), (D)) + (X) + (K), (s)) + (E);  \
    (A) = (E);                                                      \
    (E) = (D);                                                      \
    (D) = _rotl((C), 10);                                           \
    (C) = (B);                                                      \
    (B) = (T);                                                      \


void accelc_RIPEMD160_update(const void* srcBytes, size_t srcBytesLength,
                             RIPEMD160_BUFFER* HashBuffer) {
    uint32_t A, B, C, D, E;
    uint32_t AA, BB, CC, DD, EE;
    uint32_t T;
    size_t RoundsOfMainCycle = srcBytesLength / RIPEMD160_BLOCKSIZE;
    
    const uint32_t (*Ptr)[16] = srcBytes;
    
    for (size_t i = 0; i < RoundsOfMainCycle; ++i) {
        A = AA = HashBuffer->dword[0];
        B = BB = HashBuffer->dword[1];
        C = CC = HashBuffer->dword[2];
        D = DD = HashBuffer->dword[3];
        E = EE = HashBuffer->dword[4];
        
        OperateX(F, T, A, B, C, D, E, Ptr[i][0], 0, 11)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][5], 0x50A28BE6, 8)
        OperateX(F, T, A, B, C, D, E, Ptr[i][1], 0, 14)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][14], 0x50A28BE6, 9)
        OperateX(F, T, A, B, C, D, E, Ptr[i][2], 0, 15)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][7], 0x50A28BE6, 9)
        OperateX(F, T, A, B, C, D, E, Ptr[i][3], 0, 12)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][0], 0x50A28BE6, 11)
        OperateX(F, T, A, B, C, D, E, Ptr[i][4], 0, 5)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][9], 0x50A28BE6, 13)
        OperateX(F, T, A, B, C, D, E, Ptr[i][5], 0, 8)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][2], 0x50A28BE6, 15)
        OperateX(F, T, A, B, C, D, E, Ptr[i][6], 0, 7)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][11], 0x50A28BE6, 15)
        OperateX(F, T, A, B, C, D, E, Ptr[i][7], 0, 9)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][4], 0x50A28BE6, 5)
        OperateX(F, T, A, B, C, D, E, Ptr[i][8], 0, 11)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][13], 0x50A28BE6, 7)
        OperateX(F, T, A, B, C, D, E, Ptr[i][9], 0, 13)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][6], 0x50A28BE6, 7)
        OperateX(F, T, A, B, C, D, E, Ptr[i][10], 0, 14)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][15], 0x50A28BE6, 8)
        OperateX(F, T, A, B, C, D, E, Ptr[i][11], 0, 15)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][8], 0x50A28BE6, 11)
        OperateX(F, T, A, B, C, D, E, Ptr[i][12], 0, 6)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][1], 0x50A28BE6, 14)
        OperateX(F, T, A, B, C, D, E, Ptr[i][13], 0, 7)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][10], 0x50A28BE6, 14)
        OperateX(F, T, A, B, C, D, E, Ptr[i][14], 0, 9)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][3], 0x50A28BE6, 12)
        OperateX(F, T, A, B, C, D, E, Ptr[i][15], 0, 8)
        OperateX(J, T, AA, BB, CC, DD, EE, Ptr[i][12], 0x50A28BE6, 6)
        
        OperateX(G, T, A, B, C, D, E, Ptr[i][7], 0x5A827999, 7)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][6], 0x5C4DD124, 9)
        OperateX(G, T, A, B, C, D, E, Ptr[i][4], 0x5A827999, 6)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][11], 0x5C4DD124, 13)
        OperateX(G, T, A, B, C, D, E, Ptr[i][13], 0x5A827999, 8)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][3], 0x5C4DD124, 15)
        OperateX(G, T, A, B, C, D, E, Ptr[i][1], 0x5A827999, 13)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][7], 0x5C4DD124, 7)
        OperateX(G, T, A, B, C, D, E, Ptr[i][10], 0x5A827999, 11)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][0], 0x5C4DD124, 12)
        OperateX(G, T, A, B, C, D, E, Ptr[i][6], 0x5A827999, 9)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][13], 0x5C4DD124, 8)
        OperateX(G, T, A, B, C, D, E, Ptr[i][15], 0x5A827999, 7)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][5], 0x5C4DD124, 9)
        OperateX(G, T, A, B, C, D, E, Ptr[i][3], 0x5A827999, 15)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][10], 0x5C4DD124, 11)
        OperateX(G, T, A, B, C, D, E, Ptr[i][12], 0x5A827999, 7)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][14], 0x5C4DD124, 7)
        OperateX(G, T, A, B, C, D, E, Ptr[i][0], 0x5A827999, 12)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][15], 0x5C4DD124, 7)
        OperateX(G, T, A, B, C, D, E, Ptr[i][9], 0x5A827999, 15)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][8], 0x5C4DD124, 12)
        OperateX(G, T, A, B, C, D, E, Ptr[i][5], 0x5A827999, 9)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][12], 0x5C4DD124, 7)
        OperateX(G, T, A, B, C, D, E, Ptr[i][2], 0x5A827999, 11)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][4], 0x5C4DD124, 6)
        OperateX(G, T, A, B, C, D, E, Ptr[i][14], 0x5A827999, 7)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][9], 0x5C4DD124, 15)
        OperateX(G, T, A, B, C, D, E, Ptr[i][11], 0x5A827999, 13)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][1], 0x5C4DD124, 13)
        OperateX(G, T, A, B, C, D, E, Ptr[i][8], 0x5A827999, 12)
        OperateX(I, T, AA, BB, CC, DD, EE, Ptr[i][2], 0x5C4DD124, 11)
        
        OperateX(H, T, A, B, C, D, E, Ptr[i][3], 0x6ED9EBA1, 11)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][15], 0x6D703EF3, 9)
        OperateX(H, T, A, B, C, D, E, Ptr[i][10], 0x6ED9EBA1, 13)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][5], 0x6D703EF3, 7)
        OperateX(H, T, A, B, C, D, E, Ptr[i][14], 0x6ED9EBA1, 6)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][1], 0x6D703EF3, 15)
        OperateX(H, T, A, B, C, D, E, Ptr[i][4], 0x6ED9EBA1, 7)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][3], 0x6D703EF3, 11)
        OperateX(H, T, A, B, C, D, E, Ptr[i][9], 0x6ED9EBA1, 14)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][7], 0x6D703EF3, 8)
        OperateX(H, T, A, B, C, D, E, Ptr[i][15], 0x6ED9EBA1, 9)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][14], 0x6D703EF3, 6)
        OperateX(H, T, A, B, C, D, E, Ptr[i][8], 0x6ED9EBA1, 13)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][6], 0x6D703EF3, 6)
        OperateX(H, T, A, B, C, D, E, Ptr[i][1], 0x6ED9EBA1, 15)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][9], 0x6D703EF3, 14)
        OperateX(H, T, A, B, C, D, E, Ptr[i][2], 0x6ED9EBA1, 14)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][11], 0x6D703EF3, 12)
        OperateX(H, T, A, B, C, D, E, Ptr[i][7], 0x6ED9EBA1, 8)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][8], 0x6D703EF3, 13)
        OperateX(H, T, A, B, C, D, E, Ptr[i][0], 0x6ED9EBA1, 13)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][12], 0x6D703EF3, 5)
        OperateX(H, T, A, B, C, D, E, Ptr[i][6], 0x6ED9EBA1, 6)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][2], 0x6D703EF3, 14)
        OperateX(H, T, A, B, C, D, E, Ptr[i][13], 0x6ED9EBA1, 5)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][10], 0x6D703EF3, 13)
        OperateX(H, T, A, B, C, D, E, Ptr[i][11], 0x6ED9EBA1, 12)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][0], 0x6D703EF3, 13)
        OperateX(H, T, A, B, C, D, E, Ptr[i][5], 0x6ED9EBA1, 7)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][4], 0x6D703EF3, 7)
        OperateX(H, T, A, B, C, D, E, Ptr[i][12], 0x6ED9EBA1, 5)
        OperateX(H, T, AA, BB, CC, DD, EE, Ptr[i][13], 0x6D703EF3, 5)
        
        OperateX(I, T, A, B, C, D, E, Ptr[i][1], 0x8F1BBCDC, 11)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][8], 0x7A6D76E9, 15)
        OperateX(I, T, A, B, C, D, E, Ptr[i][9], 0x8F1BBCDC, 12)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][6], 0x7A6D76E9, 5)
        OperateX(I, T, A, B, C, D, E, Ptr[i][11], 0x8F1BBCDC, 14)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][4], 0x7A6D76E9, 8)
        OperateX(I, T, A, B, C, D, E, Ptr[i][10], 0x8F1BBCDC, 15)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][1], 0x7A6D76E9, 11)
        OperateX(I, T, A, B, C, D, E, Ptr[i][0], 0x8F1BBCDC, 14)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][3], 0x7A6D76E9, 14)
        OperateX(I, T, A, B, C, D, E, Ptr[i][8], 0x8F1BBCDC, 15)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][11], 0x7A6D76E9, 14)
        OperateX(I, T, A, B, C, D, E, Ptr[i][12], 0x8F1BBCDC, 9)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][15], 0x7A6D76E9, 6)
        OperateX(I, T, A, B, C, D, E, Ptr[i][4], 0x8F1BBCDC, 8)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][0], 0x7A6D76E9, 14)
        OperateX(I, T, A, B, C, D, E, Ptr[i][13], 0x8F1BBCDC, 9)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][5], 0x7A6D76E9, 6)
        OperateX(I, T, A, B, C, D, E, Ptr[i][3], 0x8F1BBCDC, 14)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][12], 0x7A6D76E9, 9)
        OperateX(I, T, A, B, C, D, E, Ptr[i][7], 0x8F1BBCDC, 5)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][2], 0x7A6D76E9, 12)
        OperateX(I, T, A, B, C, D, E, Ptr[i][15], 0x8F1BBCDC, 6)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][13], 0x7A6D76E9, 9)
        OperateX(I, T, A, B, C, D, E, Ptr[i][14], 0x8F1BBCDC, 8)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][9], 0x7A6D76E9, 12)
        OperateX(I, T, A, B, C, D, E, Ptr[i][5], 0x8F1BBCDC, 6)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][7], 0x7A6D76E9, 5)
        OperateX(I, T, A, B, C, D, E, Ptr[i][6], 0x8F1BBCDC, 5)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][10], 0x7A6D76E9, 15)
        OperateX(I, T, A, B, C, D, E, Ptr[i][2], 0x8F1BBCDC, 12)
        OperateX(G, T, AA, BB, CC, DD, EE, Ptr[i][14], 0x7A6D76E9, 8)
        
        OperateX(J, T, A, B, C, D, E, Ptr[i][4], 0xA953FD4E, 9)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][12], 0, 8)
        OperateX(J, T, A, B, C, D, E, Ptr[i][0], 0xA953FD4E, 15)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][15], 0, 5)
        OperateX(J, T, A, B, C, D, E, Ptr[i][5], 0xA953FD4E, 5)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][10], 0, 12)
        OperateX(J, T, A, B, C, D, E, Ptr[i][9], 0xA953FD4E, 11)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][4], 0, 9)
        OperateX(J, T, A, B, C, D, E, Ptr[i][7], 0xA953FD4E, 6)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][1], 0, 12)
        OperateX(J, T, A, B, C, D, E, Ptr[i][12], 0xA953FD4E, 8)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][5], 0, 5)
        OperateX(J, T, A, B, C, D, E, Ptr[i][2], 0xA953FD4E, 13)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][8], 0, 14)
        OperateX(J, T, A, B, C, D, E, Ptr[i][10], 0xA953FD4E, 12)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][7], 0, 6)
        OperateX(J, T, A, B, C, D, E, Ptr[i][14], 0xA953FD4E, 5)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][6], 0, 8)
        OperateX(J, T, A, B, C, D, E, Ptr[i][1], 0xA953FD4E, 12)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][2], 0, 13)
        OperateX(J, T, A, B, C, D, E, Ptr[i][3], 0xA953FD4E, 13)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][13], 0, 6)
        OperateX(J, T, A, B, C, D, E, Ptr[i][8], 0xA953FD4E, 14)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][14], 0, 5)
        OperateX(J, T, A, B, C, D, E, Ptr[i][11], 0xA953FD4E, 11)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][0], 0, 15)
        OperateX(J, T, A, B, C, D, E, Ptr[i][6], 0xA953FD4E, 8)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][3], 0, 13)
        OperateX(J, T, A, B, C, D, E, Ptr[i][15], 0xA953FD4E, 5)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][9], 0, 11)
        OperateX(J, T, A, B, C, D, E, Ptr[i][13], 0xA953FD4E, 6)
        OperateX(F, T, AA, BB, CC, DD, EE, Ptr[i][11], 0, 11)
        
        T = HashBuffer->dword[1] + C + DD;
        HashBuffer->dword[1] = HashBuffer->dword[2] + D + EE;
        HashBuffer->dword[2] = HashBuffer->dword[3] + E + AA;
        HashBuffer->dword[3] = HashBuffer->dword[4] + A + BB;
        HashBuffer->dword[4] = HashBuffer->dword[0] + B + CC;
        HashBuffer->dword[0] = T;
    }
}

void accelc_RIPEMD160_final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength,
                            const RIPEMD160_BUFFER* HashBuffer, RIPEMD160_DIGEST* Hash) {
    if (HashBuffer != Hash)
        *Hash = *HashBuffer;
    
    if (LeftBytesLength >= RIPEMD160_BLOCKSIZE) {
        accelc_RIPEMD160_update(LeftBytes, LeftBytesLength, Hash);
        LeftBytes = (const uint8_t*)LeftBytes + (LeftBytesLength / RIPEMD160_BLOCKSIZE) * RIPEMD160_BLOCKSIZE;
        LeftBytesLength %= RIPEMD160_BLOCKSIZE;
    }
    
    uint8_t Extra[128] = { 0 };
    for (size_t i = 0; i < LeftBytesLength; ++i)
        Extra[i] = ((const uint8_t*)LeftBytes)[i];
    
    Extra[LeftBytesLength] = 0x80;
    *(uint64_t*)(Extra + (LeftBytesLength >= 64 - 8 ? 128 - 8 : 64 - 8)) = TotalBytesLength * 8;
    
    accelc_RIPEMD160_update(Extra, LeftBytesLength >= 64 - 8 ? 128 : 64, Hash);
}

void accelc_RIPEMD160(const void* srcBytes, size_t srclen, RIPEMD160_DIGEST* Hash) {
    accelc_RIPEMD160_init(Hash);
    accelc_RIPEMD160_update(srcBytes, srclen, Hash);
    accelc_RIPEMD160_final((uint8_t*)srcBytes + (srclen / RIPEMD160_BLOCKSIZE) * RIPEMD160_BLOCKSIZE, srclen % RIPEMD160_BLOCKSIZE, srclen,
                           Hash, Hash);
}
