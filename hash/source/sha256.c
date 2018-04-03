#include "../sha2.h"

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#define _byteswap_ulong __bswapd
#define _byteswap_uint64 __bswapq
#endif

#define SHA256_BLOCKSIZE 64

const uint32_t accelc_SHA256_K[64] = {
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
    0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
    0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
    0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
    0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
    0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
    0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
};

void accelc_SHA256_init(SHA256_BUFFER* HashBuffer) {
    HashBuffer->dword[0] = 0x6A09E667;
    HashBuffer->dword[1] = 0xBB67AE85;
    HashBuffer->dword[2] = 0x3C6EF372;
    HashBuffer->dword[3] = 0xA54FF53A;
    HashBuffer->dword[4] = 0x510E527F;
    HashBuffer->dword[5] = 0x9B05688C;
    HashBuffer->dword[6] = 0x1F83D9AB;
    HashBuffer->dword[7] = 0x5BE0CD19;
}

void accelc_SHA256_update(const void* srcBytes, size_t srcBytesLength, SHA256_BUFFER* HashBuffer) {
    uint32_t Buffer[64] = { 0 };
    uint32_t a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;
    const uint32_t (*MessageBlock)[16] = srcBytes;

    size_t RoundsOfMainCycle = srcBytesLength / 64;
    for (size_t i = 0; i < RoundsOfMainCycle; ++i) {

        for (int j = 0; j < 16; ++j)
            Buffer[j] = _byteswap_ulong(MessageBlock[i][j]);

        for (int j = 16; j < 64; j++) {
            Buffer[j] = _rotr(Buffer[j - 2], 17) ^ _rotr(Buffer[j - 2], 19) ^ Buffer[j - 2] >> 10;
            Buffer[j] += Buffer[j - 7];
            Buffer[j] += _rotr(Buffer[j - 15], 7) ^ _rotr(Buffer[j - 15], 18) ^ Buffer[j - 15] >> 3;
            Buffer[j] += Buffer[j - 16];
        }

        a = HashBuffer->dword[0];
        b = HashBuffer->dword[1];
        c = HashBuffer->dword[2];
        d = HashBuffer->dword[3];
        e = HashBuffer->dword[4];
        f = HashBuffer->dword[5];
        g = HashBuffer->dword[6];
        h = HashBuffer->dword[7];

        for (int j = 0; j < 64; j++) {
            uint32_t T1 = h + (_rotr(e, 6) ^ _rotr(e, 11) ^ _rotr(e, 25)) + ((e & f) ^ (~e & g)) + accelc_SHA256_K[j] + Buffer[j];
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

        HashBuffer->dword[0] += a;
        HashBuffer->dword[1] += b;
        HashBuffer->dword[2] += c;
        HashBuffer->dword[3] += d;
        HashBuffer->dword[4] += e;
        HashBuffer->dword[5] += f;
        HashBuffer->dword[6] += g;
        HashBuffer->dword[7] += h;
    }
}

void accelc_SHA256_final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength,
                         const SHA256_BUFFER* HashBuffer, SHA256_DIGEST* Hash) {

    if (HashBuffer != Hash)
        *Hash = *HashBuffer;

    if (LeftBytesLength >= SHA256_BLOCKSIZE) {
        accelc_SHA256_update(LeftBytes, LeftBytesLength, Hash);
        LeftBytes = (const uint8_t*)LeftBytes + (LeftBytesLength / SHA256_BLOCKSIZE) * SHA256_BLOCKSIZE;
        LeftBytesLength %= SHA256_BLOCKSIZE;
    }

    uint8_t Extra[128] = { 0 };
    for (size_t i = 0; i < LeftBytesLength; ++i)
        Extra[i] = ((const uint8_t*)LeftBytes)[i];

    Extra[LeftBytesLength] = 0x80;
    *(uint64_t*)(Extra + (LeftBytesLength >= 64 - 8 ? 128 - 8 : 64 - 8)) = _byteswap_uint64(TotalBytesLength * 8);

    accelc_SHA256_update(Extra, LeftBytesLength >= 56 ? 128 : 64, Hash);

    for (int i = 0; i < 8; ++i)
        Hash->dword[i] = _byteswap_ulong(Hash->dword[i]);
}

void accelc_SHA256(const void* srcBytes, size_t srclen, SHA256_DIGEST* Hash) {
    accelc_SHA256_init(Hash);
    accelc_SHA256_update(srcBytes, srclen, Hash);
    accelc_SHA256_final((uint8_t*)srcBytes + (srclen / SHA256_BLOCKSIZE) * SHA256_BLOCKSIZE, srclen % SHA256_BLOCKSIZE, srclen, Hash, Hash);
}
