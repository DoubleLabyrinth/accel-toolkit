/*********************************************************************
* Filename:   SHA1.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../SHA1.h"
#include <intrin.h>

#define HASH_SHA1_BLOCKSIZE 64

void HASH_SHA1_Initialize(uint32_t HashBuffer[5]) {
    HashBuffer[0] = 0x67452301;
    HashBuffer[1] = 0xEFCDAB89;
    HashBuffer[2] = 0x98BADCFE;
    HashBuffer[3] = 0x10325476;
    HashBuffer[4] = 0xC3D2E1F0;
}

void HASH_SHA1_MainCycle(const void *srcBytes, size_t srcBytesLength, uint32_t HashBuffer[5]) {
    uint32_t Buffer[80] = { 0 };
    uint32_t a, b, c, d, e;
    const uint32_t (*MessageBlock)[16] = srcBytes;

    size_t RoundsOfMainCycle = srcBytesLength / 64;
    for (size_t i = 0; i < RoundsOfMainCycle; ++i) {

        for (int j = 0; j < 16; ++j)
            Buffer[j] = _byteswap_ulong(MessageBlock[i][j]);

        for (int j = 16; j < 80; ++j) {
            uint32_t temp = Buffer[j - 3] ^ Buffer[j - 8] ^ Buffer[j - 14] ^ Buffer[j - 16];
            Buffer[j] = _rotl(temp, 1);
        }
        a = HashBuffer[0];
        b = HashBuffer[1];
        c = HashBuffer[2];
        d = HashBuffer[3];
        e = HashBuffer[4];

        for (int j = 0; j < 20; ++j) {
            uint32_t T = _rotl(a, 5);
            T += ((b & c) ^ (~b & d)) + e + 0x5A827999 + Buffer[j];
            e = d;
            d = c;
            c = _rotl(b, 30);
            b = a;
            a = T;
        }
        for (int j = 20; j < 40; ++j) {
            uint32_t T = _rotl(a, 5);
            T += (b ^ c ^ d) + e + 0x6ED9EBA1 + Buffer[j];
            e = d;
            d = c;
            c = _rotl(b, 30);
            b = a;
            a = T;
        }
        for (int j = 40; j < 60; ++j) {
            uint32_t T = _rotl(a, 5);
            T += ((b & c) ^ (b & d) ^ (c & d)) + e + 0x8F1BBCDC + Buffer[j];
            e = d;
            d = c;
            c = _rotl(b, 30);
            b = a;
            a = T;
        }
        for (int j = 60; j < 80; ++j) {
            uint32_t T = _rotl(a, 5);
            T += (b ^ c ^ d) + e + 0xCA62C1D6 + Buffer[j];
            e = d;
            d = c;
            c = _rotl(b, 30);
            b = a;
            a = T;
        }
        HashBuffer[0] += a;
        HashBuffer[1] += b;
        HashBuffer[2] += c;
        HashBuffer[3] += d;
        HashBuffer[4] += e;
    }
}

void HASH_SHA1_Final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength, uint32_t HashBuffer[5]) {
    if (LeftBytesLength >= HASH_SHA1_BLOCKSIZE) {
        HASH_SHA1_MainCycle(LeftBytes, LeftBytesLength, HashBuffer);
        LeftBytes = (const uint8_t*)LeftBytes + (LeftBytesLength / HASH_SHA1_BLOCKSIZE) * HASH_SHA1_BLOCKSIZE;
        LeftBytesLength %= HASH_SHA1_BLOCKSIZE;
    }

    uint8_t Extra[128] = { 0 };
    for (size_t i = 0; i < LeftBytesLength; ++i)
        Extra[i] = ((const uint8_t*)LeftBytes)[i];

    Extra[LeftBytesLength] = 0x80;
    *(uint64_t*)(Extra + (LeftBytesLength >= 64 - 8 ? 128 - 8 : 64 - 8)) = _byteswap_uint64(TotalBytesLength * 8);

    HASH_SHA1_MainCycle(Extra, LeftBytesLength >= 56 ? 128 : 64, HashBuffer);

    HashBuffer[0] = _byteswap_ulong(HashBuffer[0]);
    HashBuffer[1] = _byteswap_ulong(HashBuffer[1]);
    HashBuffer[2] = _byteswap_ulong(HashBuffer[2]);
    HashBuffer[3] = _byteswap_ulong(HashBuffer[3]);
    HashBuffer[4] = _byteswap_ulong(HashBuffer[4]);
}

