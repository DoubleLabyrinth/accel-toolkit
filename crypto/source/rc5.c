#include "../rc5.h"
#include <memory.h>

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#define _rotl16 _rotwl
#define _rotr16 _rotwr
#define _rotl64 __rolq
#define _rotr64 __rorq
#endif

const uint16_t accelc_RC5_P16 = 0xB7E1;
const uint16_t accelc_RC5_Q16 = 0x9E37;

void accelc_RC5_16_encrypt(uint8_t srcBytes[RC5_16_BLOCK_SIZE], const uint16_t ExpandedKey[], uint8_t round_count) {
    uint16_t* const A = (uint16_t*)srcBytes;
    uint16_t* const B = (uint16_t*)srcBytes + 1;

    *A += ExpandedKey[0];
    *B += ExpandedKey[1];
    for (int i = 1; i <= round_count; ++i) {
        *A = _rotl16(*A ^ *B, (uint8_t)(*B % 16)) + ExpandedKey[i * 2];
        *B = _rotl16(*B ^ *A, (uint8_t)(*A % 16)) + ExpandedKey[i * 2 + 1];
    }
}

void accelc_RC5_16_decrypt(uint8_t srcBytes[RC5_16_BLOCK_SIZE], const uint16_t ExpandedKey[], uint8_t round_count) {
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

int accelc_RC5_16_set_key(const uint8_t srcKey[],
                          uint8_t srcKeyLength,
                          uint8_t round_count,
                          uint16_t dstExpandedKey[], size_t dstExpandedKeyBufLength) {

    if (dstExpandedKeyBufLength < 2 * ((size_t)round_count + 1))
        return RC5_16_BUFFER_TOO_SHORT;

    uint16_t L[128] = { 0 };
    int c = (srcKeyLength + 1) / 2;
    if (c == 0) c = 1;

    memcpy(L, srcKey, srcKeyLength);

    int t = (round_count + 1) * 2;
    dstExpandedKey[0] = accelc_RC5_P16;
    for (int i = 1; i < t; ++i)
        dstExpandedKey[i] = dstExpandedKey[i - 1] + accelc_RC5_Q16;

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

    return RC5_16_SUCCESS;
}

const uint32_t accelc_RC5_P32 = 0xB7E15163;
const uint32_t accelc_RC5_Q32 = 0x9E3779B9;

void accelc_RC5_32_encrypt(uint8_t srcBytes[RC5_32_BLOCK_SIZE], const uint32_t srcExpandedKey[], uint8_t round_count) {
    uint32_t* const A = (uint32_t*)srcBytes;
    uint32_t* const B = (uint32_t*)srcBytes + 1;

    *A += srcExpandedKey[0];
    *B += srcExpandedKey[1];
    for (int i = 1; i <= round_count; ++i) {
        *A = _rotl(*A ^ *B, (int)(*B % 32)) + srcExpandedKey[i * 2];
        *B = _rotl(*B ^ *A, (int)(*A % 32)) + srcExpandedKey[i * 2 + 1];
    }
}

void accelc_RC5_32_decrypt(uint8_t srcBytes[RC5_32_BLOCK_SIZE], const uint32_t srcExpandedKey[], uint8_t round_count) {
    uint32_t* const A = (uint32_t*)srcBytes;
    uint32_t* const B = (uint32_t*)srcBytes + 1;

    for (int i = round_count; i > 0; --i) {
        *B = _rotr(*B - srcExpandedKey[i * 2 + 1], (int)(*A % 32)) ^ *A;
        *A = _rotr(*A - srcExpandedKey[i * 2], (int)(*B % 32)) ^ *B;
    }

    *B -= srcExpandedKey[1];
    *A -= srcExpandedKey[0];
}

int accelc_RC5_32_set_key(const uint8_t srcKey[],
                          uint8_t srcKeyLength,
                          uint8_t round_count,
                          uint32_t dstExpandedKey[], size_t dstExpandedKeyBufLength) {

    if (dstExpandedKeyBufLength < 2 * ((size_t)round_count + 1))
        return RC5_32_BUFFER_TOO_SHORT;

    uint32_t L[64] = { 0 };
    int c = (srcKeyLength + 3) / 4;
    if (c == 0) c = 1;

    memcpy(L, srcKey, srcKeyLength);

    int t = (round_count + 1) * 2;
    dstExpandedKey[0] = accelc_RC5_P32;
    for (int i = 1; i < t; ++i)
        dstExpandedKey[i] = dstExpandedKey[i - 1] + accelc_RC5_Q32;

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

    return RC5_32_SUCCESS;
}

const uint64_t accelc_RC5_P64 = 0xB7E151628AED2A6B;
const uint64_t accelc_RC5_Q64 = 0x9E3779B97F4A7C15;

void accelc_RC5_64_encrypt(uint8_t srcBytes[RC5_64_BLOCK_SIZE], const uint64_t ExpandedKey[], uint8_t round_count) {
    uint64_t* const A = (uint64_t*)srcBytes;
    uint64_t* const B = (uint64_t*)srcBytes + 1;

    *A += ExpandedKey[0];
    *B += ExpandedKey[1];
    for (int i = 1; i <= round_count; ++i) {
        *A = _rotl64(*A ^ *B, (int)(*B % 64)) + ExpandedKey[i * 2];
        *B = _rotl64(*B ^ *A, (int)(*A % 64)) + ExpandedKey[i * 2 + 1];
    }
}

void accelc_RC5_64_decrypt(uint8_t srcBytes[RC5_64_BLOCK_SIZE], const uint64_t ExpandedKey[], uint8_t round_count) {
    uint64_t* const A = (uint64_t*)srcBytes;
    uint64_t* const B = (uint64_t*)srcBytes + 1;

    for (int i = round_count; i > 0; --i) {
        *B = _rotr64(*B - ExpandedKey[i * 2 + 1], (int)(*A % 64)) ^ *A;
        *A = _rotr64(*A - ExpandedKey[i * 2], (int)(*B % 64)) ^ *B;
    }

    *B -= ExpandedKey[1];
    *A -= ExpandedKey[0];
}

int accelc_RC5_64_set_key(const uint8_t srcKey[],
                          uint8_t srcKeyLength,
                          uint8_t round_count,
                          uint64_t dstExpandedKey[], size_t dstExpandedKeyBufLength) {

    if (dstExpandedKeyBufLength < 2 * ((size_t)round_count + 1))
        return RC5_64_BUFFER_TOO_SHORT;

    uint64_t L[32] = { 0 };
    int c = (srcKeyLength + 7) / 8;
    if (c == 0) c = 1;

    memcpy(L, srcKey, srcKeyLength);

    int t = (round_count + 1) * 2;
    dstExpandedKey[0] = accelc_RC5_P64;
    for (int i = 1; i < t; ++i)
        dstExpandedKey[i] = dstExpandedKey[i - 1] + accelc_RC5_Q64;

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

    return RC5_64_SUCCESS;
}