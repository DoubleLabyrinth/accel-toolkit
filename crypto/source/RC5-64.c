/*********************************************************************
* Filename:   RC5-64.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../RC5-64.h"
#include <intrin.h>
#include <memory.h>

const uint64_t P64 = 0xB7E151628AED2A6B;
const uint64_t Q64 = 0x9E3779B97F4A7C15;

void CRYPTO_RC5_64_EncryptBlock(uint8_t srcBytes[16], const uint64_t ExpandedKey[], uint8_t round_count) {
    uint64_t* const A = (uint64_t*)srcBytes;
    uint64_t* const B = (uint64_t*)srcBytes + 1;

    *A += ExpandedKey[0];
    *B += ExpandedKey[1];
    for (int i = 1; i <= round_count; ++i) {
        *A = _rotl64(*A ^ *B, (int)(*B % 64)) + ExpandedKey[i * 2];
        *B = _rotl64(*B ^ *A, (int)(*A % 64)) + ExpandedKey[i * 2 + 1];
    }
}

void CRYPTO_RC5_64_DecryptBlock(uint8_t srcBytes[16], const uint64_t ExpandedKey[], uint8_t round_count) {
    uint64_t* const A = (uint64_t*)srcBytes;
    uint64_t* const B = (uint64_t*)srcBytes + 1;

    for (int i = round_count; i > 0; --i) {
        *B = _rotr64(*B - ExpandedKey[i * 2 + 1], (int)(*A % 64)) ^ *A;
        *A = _rotr64(*A - ExpandedKey[i * 2], (int)(*B % 64)) ^ *B;
    }

    *B -= ExpandedKey[1];
    *A -= ExpandedKey[0];
}

int CRYPTO_RC5_64_KeyExpansion(const uint8_t srcKey[],
                               uint8_t srcKeyLength,
                               uint8_t round_count,
                               uint64_t dstExpandedKey[], size_t dstExpandedKeyBufLength) {

    if (dstExpandedKeyBufLength < 2 * ((size_t)round_count + 1))
        return CRYPTO_RC5_64_BUFFER_TOO_SHORT;

    uint64_t L[32] = { 0 };
    int c = (srcKeyLength + 7) / 8;
    if (c == 0) c = 1;

    memcpy(L, srcKey, srcKeyLength);

    int t = (round_count + 1) * 2;
    dstExpandedKey[0] = P64;
    for (int i = 1; i < t; ++i)
        dstExpandedKey[i] = dstExpandedKey[i - 1] + Q64;

    int ii = 0, jj = 0;
    uint64_t A = 0, B = 0;
    for (int i = 0, fin = 3 * (t > c ? t : c); i < fin; ++i) {
        dstExpandedKey[ii] = _rotl64(dstExpandedKey[ii] + A + B, 3);
        A = dstExpandedKey[ii];

        L[jj] = _rotl64(L[jj] + A + B, (int)((A + B) % 64));
        B = L[jj];

        ii = (ii + 1) % t;
        jj = (jj + 1) % c;
    }
    
    return CRYPTO_RC5_64_SUCCESS;
}
