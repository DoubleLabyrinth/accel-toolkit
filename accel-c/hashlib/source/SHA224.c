/*********************************************************************
* Filename:   SHA224.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../SHA224.h"
#include <intrin.h>

#define HASH_SHA224_BLOCKSIZE 64

const uint32_t HASH_SHA224_K[64] = {
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
};

void HASH_SHA224_Initialize(uint32_t HashBuffer[8]) {
    HashBuffer[0] = 0xC1059ED8;
    HashBuffer[1] = 0x367CD507;
    HashBuffer[2] = 0x3070DD17;
    HashBuffer[3] = 0xF70E5939;
    HashBuffer[4] = 0xFFC00B31;
    HashBuffer[5] = 0x68581511;
    HashBuffer[6] = 0x64F98FA7;
    HashBuffer[7] = 0xBEFA4FA4;
}

void HASH_SHA224_MainCycle(const void* srcBytes, size_t srcBytesLength, uint32_t HashBuffer[8]) {
    uint32_t Buffer[64] = { 0 };
    uint32_t a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;
    const uint32_t (*MessageBlock)[16] = srcBytes;

    size_t RoundsOfMainCycle = srcBytesLength / 64;

    for (size_t i = 0; i < RoundsOfMainCycle; ++i) {
        
        for (int j = 0; j < 16; ++j)
            Buffer[j] = _byteswap_ulong(MessageBlock[i][j]);

        for (int j = 16; j < 64; ++j) {
            Buffer[j] = _rotr(Buffer[j - 2], 17) ^ _rotr(Buffer[j - 2], 19) ^ Buffer[j - 2] >> 10;
            Buffer[j] += Buffer[j - 7];
            Buffer[j] += _rotr(Buffer[j - 15], 7) ^ _rotr(Buffer[j - 15], 18) ^ Buffer[j - 15] >> 3;
            Buffer[j] += Buffer[j - 16];
        }
        a = HashBuffer[0];
        b = HashBuffer[1];
        c = HashBuffer[2];
        d = HashBuffer[3];
        e = HashBuffer[4];
        f = HashBuffer[5];
        g = HashBuffer[6];
        h = HashBuffer[7];

        for (int j = 0; j < 64; ++j) {
            uint32_t T1 = h + (_rotr(e, 6) ^ _rotr(e, 11) ^ _rotr(e, 25)) + ((e & f) ^ (~e & g)) + HASH_SHA224_K[j] + Buffer[j];
            uint32_t T2 = (_rotr(a, 2) ^ _rotr(a, 13) ^ _rotr(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        HashBuffer[0] += a;
        HashBuffer[1] += b;
        HashBuffer[2] += c;
        HashBuffer[3] += d;
        HashBuffer[4] += e;
        HashBuffer[5] += f;
        HashBuffer[6] += g;
        HashBuffer[7] += h;
    }
}

void HASH_SHA224_Final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength, uint32_t HashBuffer[8]) {
    if (LeftBytesLength >= HASH_SHA224_BLOCKSIZE) {
        HASH_SHA224_MainCycle(LeftBytes, LeftBytesLength, HashBuffer);
        LeftBytes = (const uint8_t*)LeftBytes + (LeftBytesLength / HASH_SHA224_BLOCKSIZE) * HASH_SHA224_BLOCKSIZE;
        LeftBytesLength %= HASH_SHA224_BLOCKSIZE;
    }

    uint8_t Extra[128] = { 0 };
    for (size_t i = 0; i < LeftBytesLength; ++i)
        Extra[i] = ((const uint8_t*)LeftBytes)[i];

    Extra[LeftBytesLength] = 0x80;
    *(uint64_t*)(Extra + (LeftBytesLength >= 64 - 8 ? 128 - 8 : 64 - 8)) = _byteswap_uint64(TotalBytesLength * 8);

    HASH_SHA224_MainCycle(Extra, LeftBytesLength >= 64 - 8 ? 128 : 64, HashBuffer);

    for (int i = 0; i < 8; ++i)
        HashBuffer[i] = _byteswap_ulong(HashBuffer[i]);
}
