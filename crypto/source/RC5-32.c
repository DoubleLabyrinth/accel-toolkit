/*********************************************************************
* Filename:   RC5-32.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../RC5-32.h"
#include <intrin.h>
#include <memory.h>

const uint32_t CRYPTO_RC5_32_P32 = 0xB7E15163;
const uint32_t CRYPTO_RC5_32_Q32 = 0x9E3779B9;

void CRYPTO_RC5_32_EncryptBlock(uint8_t srcBytes[8], const uint32_t srcExpandedKey[], uint8_t round_count) {
    uint32_t* const A = (uint32_t*)srcBytes;
    uint32_t* const B = (uint32_t*)srcBytes + 1;

    *A += srcExpandedKey[0];
    *B += srcExpandedKey[1];
    for (int i = 1; i <= round_count; ++i) {
        *A = _rotl(*A ^ *B, (int)(*B % 32)) + srcExpandedKey[i * 2];
        *B = _rotl(*B ^ *A, (int)(*A % 32)) + srcExpandedKey[i * 2 + 1];
    }
}

void CRYPTO_RC5_32_DecryptBlock(uint8_t srcBytes[8], const uint32_t srcExpandedKey[], uint8_t round_count) {
    uint32_t* const A = (uint32_t*)srcBytes;
    uint32_t* const B = (uint32_t*)srcBytes + 1;

    for (int i = round_count; i > 0; --i) {
        *B = _rotr(*B - srcExpandedKey[i * 2 + 1], (int)(*A % 32)) ^ *A;
        *A = _rotr(*A - srcExpandedKey[i * 2], (int)(*B % 32)) ^ *B;
    }

    *B -= srcExpandedKey[1];
    *A -= srcExpandedKey[0];
}

int CRYPTO_RC5_32_KeyExpansion(const uint8_t srcKey[],
                               uint8_t srcKeyLength,
                               uint8_t round_count,
                               uint32_t dstExpandedKey[], size_t dstExpandedKeyBufLength) {

    if (dstExpandedKeyBufLength < 2 * ((size_t)round_count + 1))
        return CRYPTO_RC5_32_BUFFER_TOO_SHORT;

    uint32_t L[64] = { 0 };
    int c = (srcKeyLength + 3) / 4;
    if (c == 0) c = 1;

    memcpy(L, srcKey, srcKeyLength);

    int t = (round_count + 1) * 2;
    dstExpandedKey[0] = CRYPTO_RC5_32_P32;
    for (int i = 1; i < t; ++i)
        dstExpandedKey[i] = dstExpandedKey[i - 1] + CRYPTO_RC5_32_Q32;

    int ii = 0, jj = 0;
    uint32_t A = 0, B = 0;
    for (int i = 0, fin = 3 * (t > c ? t : c); i < fin; ++i) {
        dstExpandedKey[ii] = _rotl(dstExpandedKey[ii] + A + B, 3);
        A = dstExpandedKey[ii];

        L[jj] = _rotl(L[jj] + A + B, (A + B) % 32);
        B = L[jj];

        ii = (ii + 1) % t;
        jj = (jj + 1) % c;
    }

    return CRYPTO_RC5_32_SUCCESS;
}
