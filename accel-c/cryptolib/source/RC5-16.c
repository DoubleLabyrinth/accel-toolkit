/*********************************************************************
* Filename:   RC5-16.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../RC5-16.h"
#include <intrin.h>
#include <memory.h>

const uint16_t CRYPTO_RC5_16_P16 = 0xB7E1;
const uint16_t CRYPTO_RC5_16_Q16 = 0x9E37;

void CRYPTO_RC5_16_EncryptBlock(uint8_t srcBytes[4], const uint16_t ExpandedKey[], uint8_t round_count) {
    uint16_t* const A = (uint16_t*)srcBytes;
    uint16_t* const B = (uint16_t*)srcBytes + 1;

    *A += ExpandedKey[0];
    *B += ExpandedKey[1];
    for (int i = 1; i <= round_count; ++i) {
        *A = _rotl16(*A ^ *B, (uint8_t)(*B % 16)) + ExpandedKey[i * 2];
        *B = _rotl16(*B ^ *A, (uint8_t)(*A % 16)) + ExpandedKey[i * 2 + 1];
    }
}

void CRYPTO_RC5_16_DecryptBlock(uint8_t srcBytes[4], const uint16_t ExpandedKey[], uint8_t round_count) {
    uint16_t* const A = (uint16_t*)srcBytes;
    uint16_t* const B = (uint16_t*)srcBytes + 1;

    for (int i = round_count; i > 0; --i) {
        *B -= ExpandedKey[i * 2 + 1];
        *B = _rotr16(*B, (uint8_t)(*A % 16)) ^ *A;

        *A -= ExpandedKey[i * 2];
        *A = _rotr16(*A, (uint8_t)(*B % 16)) ^ *B;
    }

    *B -= ExpandedKey[1];
    *A -= ExpandedKey[0];
}

int CRYPTO_RC5_16_KeyExpansion(const uint8_t srcKey[],
                               uint8_t srcKeyLength,
                               uint8_t round_count,
                               uint16_t dstExpandedKey[], size_t dstExpandedKeyBufLength) {

    if (dstExpandedKeyBufLength < 2 * ((size_t)round_count + 1))
        return CRYPTO_RC5_16_BUFFER_TOO_SHORT;

    uint16_t L[128] = { 0 };
    int c = (srcKeyLength + 1) / 2;
    if (c == 0) c = 1;

    memcpy(L, srcKey, srcKeyLength);

    int t = (round_count + 1) * 2;
    dstExpandedKey[0] = CRYPTO_RC5_16_P16;
    for (int i = 1; i < t; ++i)
        dstExpandedKey[i] = dstExpandedKey[i - 1] + CRYPTO_RC5_16_Q16;

    int ii = 0, jj = 0;
    uint16_t A = 0, B = 0;
    for (int i = 0, fin = 3 * (t > c ? t : c); i < fin; ++i) {
        dstExpandedKey[ii] = _rotl16(dstExpandedKey[ii] + A + B, 3);
        A = dstExpandedKey[ii];

        L[jj] = _rotl16(L[jj] + A + B, (A + B) % 16);
        B = L[jj];

        ii = (ii + 1) % t;
        jj = (jj + 1) % c;
    }

    return CRYPTO_RC5_16_SUCCESS;
}
