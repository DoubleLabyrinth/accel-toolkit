#include "../twofish.h"

#ifdef _MSC_VER
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#endif

extern const uint8_t accelc_Twofish_GF0x14D_Mul_Table[256][256];
extern const uint8_t accelc_Twofish_GF0x169_Mul_Table[256][256];
extern const uint8_t accelc_Twofish_MDS[4][4];
extern const uint8_t accelc_Twofish_RS[4][8];
extern const uint8_t accelc_Twofish_q0[256];
extern const uint8_t accelc_Twofish_q1[256];

static uint32_t func_h_128(uint32_t X, const uint32_t L[2]) {
    ((uint8_t*)&X)[0] = accelc_Twofish_q1[accelc_Twofish_q0[accelc_Twofish_q0[((uint8_t*)&X)[0]] ^ ((const uint8_t*)L)[4]] ^ ((const uint8_t*)L)[0]];
    ((uint8_t*)&X)[1] = accelc_Twofish_q0[accelc_Twofish_q0[accelc_Twofish_q1[((uint8_t*)&X)[1]] ^ ((const uint8_t*)L)[5]] ^ ((const uint8_t*)L)[1]];
    ((uint8_t*)&X)[2] = accelc_Twofish_q1[accelc_Twofish_q1[accelc_Twofish_q0[((uint8_t*)&X)[2]] ^ ((const uint8_t*)L)[6]] ^ ((const uint8_t*)L)[2]];
    ((uint8_t*)&X)[3] = accelc_Twofish_q0[accelc_Twofish_q1[accelc_Twofish_q1[((uint8_t*)&X)[3]] ^ ((const uint8_t*)L)[7]] ^ ((const uint8_t*)L)[3]];

    uint8_t z[4] = { 0 };
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            z[i] ^= accelc_Twofish_GF0x169_Mul_Table[accelc_Twofish_MDS[i][j]][((uint8_t*)&X)[j]];
        }
    }

    return *(uint32_t*)z;
}

void accelc_Twofish128_encrypt(uint8_t srcBytes[TWOFISH_BLOCK_SIZE], const TWOFISH_KEY* srcKey) {
    uint32_t* const srcBytes_dword_ptr = (uint32_t*)srcBytes;

    srcBytes_dword_ptr[0] ^= srcKey->ExpandedKey[0];
    srcBytes_dword_ptr[1] ^= srcKey->ExpandedKey[1];
    srcBytes_dword_ptr[2] ^= srcKey->ExpandedKey[2];
    srcBytes_dword_ptr[3] ^= srcKey->ExpandedKey[3];

    for (int Round = 0; Round < 15; ++Round) {
        uint32_t T0 = func_h_128(srcBytes_dword_ptr[0], srcKey->SBox);
        uint32_t T1 = _rotl(srcBytes_dword_ptr[1], 8);
        T1 = func_h_128(T1, srcKey->SBox);
        uint32_t F0 = T0 + T1 + srcKey->ExpandedKey[2 * Round + 8];
        uint32_t F1 = T0 + T1 * 2u + srcKey->ExpandedKey[2 * Round + 9];

        F0 ^= srcBytes_dword_ptr[2];
        F1 ^= _rotl(srcBytes_dword_ptr[3], 1);
        srcBytes_dword_ptr[2] = srcBytes_dword_ptr[0];
        srcBytes_dword_ptr[3] = srcBytes_dword_ptr[1];
        srcBytes_dword_ptr[0] = _rotr(F0, 1);
        srcBytes_dword_ptr[1] = F1;
    }

    uint32_t T0 = func_h_128(srcBytes_dword_ptr[0], srcKey->SBox);
    uint32_t T1 = _rotl(srcBytes_dword_ptr[1], 8);
    T1 = func_h_128(T1, srcKey->SBox);
    uint32_t F0 = T0 + T1 + srcKey->ExpandedKey[38];
    uint32_t F1 = T0 + T1 * 2u + srcKey->ExpandedKey[39];

    F0 ^= srcBytes_dword_ptr[2];
    F1 ^= _rotl(srcBytes_dword_ptr[3], 1);
    srcBytes_dword_ptr[2] = _rotr(F0, 1);
    srcBytes_dword_ptr[3] = F1;

    srcBytes_dword_ptr[0] ^= srcKey->ExpandedKey[4];
    srcBytes_dword_ptr[1] ^= srcKey->ExpandedKey[5];
    srcBytes_dword_ptr[2] ^= srcKey->ExpandedKey[6];
    srcBytes_dword_ptr[3] ^= srcKey->ExpandedKey[7];
}

void accelc_Twofish128_decrypt(uint8_t srcBytes[TWOFISH_BLOCK_SIZE], const TWOFISH_KEY* srcKey) {
    uint32_t* const srcBytes_dword_ptr = (uint32_t*)srcBytes;

    srcBytes_dword_ptr[0] ^= srcKey->ExpandedKey[4];
    srcBytes_dword_ptr[1] ^= srcKey->ExpandedKey[5];
    srcBytes_dword_ptr[2] ^= srcKey->ExpandedKey[6];
    srcBytes_dword_ptr[3] ^= srcKey->ExpandedKey[7];

    uint32_t T0 = func_h_128(srcBytes_dword_ptr[0], srcKey->SBox);
    uint32_t T1 = _rotl(srcBytes_dword_ptr[1], 8);
    T1 = func_h_128(T1, srcKey->SBox);
    uint32_t F0 = T0 + T1 + srcKey->ExpandedKey[38];
    uint32_t F1 = T0 + T1 * 2u + srcKey->ExpandedKey[39];

    srcBytes_dword_ptr[2] = _rotl(srcBytes_dword_ptr[2], 1) ^ F0;
    F1 ^= srcBytes_dword_ptr[3];
    srcBytes_dword_ptr[3] = _rotr(F1, 1);

    for (int Round = 14; Round >= 0; --Round) {
        T0 = func_h_128(srcBytes_dword_ptr[2], srcKey->SBox);
        T1 = func_h_128(_rotl(srcBytes_dword_ptr[3], 8), srcKey->SBox);
        F0 = T0 + T1 + srcKey->ExpandedKey[2 * Round + 8];
        F1 = T0 + T1 * 2u + srcKey->ExpandedKey[2 * Round + 9];

        F0 ^= _rotl(srcBytes_dword_ptr[0], 1);
        F1 ^= srcBytes_dword_ptr[1];
        srcBytes_dword_ptr[0] = srcBytes_dword_ptr[2];
        srcBytes_dword_ptr[1] = srcBytes_dword_ptr[3];
        srcBytes_dword_ptr[3] = _rotr(F1, 1);
        srcBytes_dword_ptr[2] = F0;
    }

    srcBytes_dword_ptr[0] ^= srcKey->ExpandedKey[0];
    srcBytes_dword_ptr[1] ^= srcKey->ExpandedKey[1];
    srcBytes_dword_ptr[2] ^= srcKey->ExpandedKey[2];
    srcBytes_dword_ptr[3] ^= srcKey->ExpandedKey[3];
}

void accelc_Twofish128_set_key(const uint8_t srcKey[TWOFISH128_USERKEY_SIZE], TWOFISH_KEY* dstKey) {
    uint32_t M_e[2] = { ((const uint32_t*)srcKey)[0], ((const uint32_t*)srcKey)[2] };
    uint32_t M_o[2] = { ((const uint32_t*)srcKey)[1], ((const uint32_t*)srcKey)[3] };

    dstKey->SBox[0] = 0;
    dstKey->SBox[1] = 0;
    dstKey->SBox[2] = 0;
    dstKey->SBox[3] = 0;

    for (int i = 4; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            ((uint8_t*)dstKey->SBox)[i] ^= accelc_Twofish_GF0x14D_Mul_Table[accelc_Twofish_RS[i - 4][j]][srcKey[j]];
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j)
            ((uint8_t*)dstKey->SBox)[i] ^= accelc_Twofish_GF0x14D_Mul_Table[accelc_Twofish_RS[i][j]][srcKey[8 + j]];
    }

    const uint32_t Rou = 0x01010101;
    for (uint32_t i = 0; i < 20u; ++i) {
        uint32_t A = func_h_128(2u * i * Rou, M_e);
        uint32_t B = func_h_128((2u * i + 1u) * Rou, M_o);
        B = _rotl(B, 8);
        dstKey->ExpandedKey[2u * i] = A + B;
        dstKey->ExpandedKey[2u * i + 1u] = _rotl(A + 2u * B, 9);
    }
}

static uint32_t func_h_192(uint32_t X, const uint32_t L[3]) {
    ((uint8_t*)&X)[0] = accelc_Twofish_q1[((uint8_t*)&X)[0]] ^ ((const uint8_t*)L)[8];
    ((uint8_t*)&X)[1] = accelc_Twofish_q1[((uint8_t*)&X)[1]] ^ ((const uint8_t*)L)[9];
    ((uint8_t*)&X)[2] = accelc_Twofish_q0[((uint8_t*)&X)[2]] ^ ((const uint8_t*)L)[10];
    ((uint8_t*)&X)[3] = accelc_Twofish_q0[((uint8_t*)&X)[3]] ^ ((const uint8_t*)L)[11];

    ((uint8_t*)&X)[0] = accelc_Twofish_q1[accelc_Twofish_q0[accelc_Twofish_q0[((uint8_t*)&X)[0]] ^ ((const uint8_t*)L)[4]] ^ ((const uint8_t*)L)[0]];
    ((uint8_t*)&X)[1] = accelc_Twofish_q0[accelc_Twofish_q0[accelc_Twofish_q1[((uint8_t*)&X)[1]] ^ ((const uint8_t*)L)[5]] ^ ((const uint8_t*)L)[1]];
    ((uint8_t*)&X)[2] = accelc_Twofish_q1[accelc_Twofish_q1[accelc_Twofish_q0[((uint8_t*)&X)[2]] ^ ((const uint8_t*)L)[6]] ^ ((const uint8_t*)L)[2]];
    ((uint8_t*)&X)[3] = accelc_Twofish_q0[accelc_Twofish_q1[accelc_Twofish_q1[((uint8_t*)&X)[3]] ^ ((const uint8_t*)L)[7]] ^ ((const uint8_t*)L)[3]];

    uint8_t z[4] = { 0 };
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            z[i] ^= accelc_Twofish_GF0x169_Mul_Table[accelc_Twofish_MDS[i][j]][((uint8_t*)&X)[j]];
        }
    }

    return *(uint32_t*)z;
}

void accelc_Twofish192_encrypt(uint8_t srcBytes[TWOFISH_BLOCK_SIZE], const TWOFISH_KEY* srcKey) {
    uint32_t* const srcBytes_dword_ptr = (uint32_t*)srcBytes;

    srcBytes_dword_ptr[0] ^= srcKey->ExpandedKey[0];
    srcBytes_dword_ptr[1] ^= srcKey->ExpandedKey[1];
    srcBytes_dword_ptr[2] ^= srcKey->ExpandedKey[2];
    srcBytes_dword_ptr[3] ^= srcKey->ExpandedKey[3];

    for (int Round = 0; Round < 15; ++Round) {
        uint32_t T0 = func_h_192(srcBytes_dword_ptr[0], srcKey->SBox);
        uint32_t T1 = _rotl(srcBytes_dword_ptr[1], 8);
        T1 = func_h_192(T1, srcKey->SBox);
        uint32_t F0 = T0 + T1 + srcKey->ExpandedKey[2 * Round + 8];
        uint32_t F1 = T0 + T1 * 2u + srcKey->ExpandedKey[2 * Round + 9];

        F0 ^= srcBytes_dword_ptr[2];
        F1 ^= _rotl(srcBytes_dword_ptr[3], 1);
        srcBytes_dword_ptr[2] = srcBytes_dword_ptr[0];
        srcBytes_dword_ptr[3] = srcBytes_dword_ptr[1];
        srcBytes_dword_ptr[0] = _rotr(F0, 1);
        srcBytes_dword_ptr[1] = F1;
    }

    uint32_t T0 = func_h_192(srcBytes_dword_ptr[0], srcKey->SBox);
    uint32_t T1 = _rotl(srcBytes_dword_ptr[1], 8);
    T1 = func_h_192(T1, srcKey->SBox);
    uint32_t F0 = T0 + T1 + srcKey->ExpandedKey[38];
    uint32_t F1 = T0 + T1 * 2u + srcKey->ExpandedKey[39];

    F0 ^= srcBytes_dword_ptr[2];
    F1 ^= _rotl(srcBytes_dword_ptr[3], 1);
    srcBytes_dword_ptr[2] = _rotr(F0, 1);
    srcBytes_dword_ptr[3] = F1;

    srcBytes_dword_ptr[0] ^= srcKey->ExpandedKey[4];
    srcBytes_dword_ptr[1] ^= srcKey->ExpandedKey[5];
    srcBytes_dword_ptr[2] ^= srcKey->ExpandedKey[6];
    srcBytes_dword_ptr[3] ^= srcKey->ExpandedKey[7];
}

void accelc_Twofish192_decrypt(uint8_t srcBytes[TWOFISH_BLOCK_SIZE], const TWOFISH_KEY* srcKey) {
    uint32_t* const srcBytes_dword_ptr = (uint32_t*)srcBytes;

    srcBytes_dword_ptr[0] ^= srcKey->ExpandedKey[4];
    srcBytes_dword_ptr[1] ^= srcKey->ExpandedKey[5];
    srcBytes_dword_ptr[2] ^= srcKey->ExpandedKey[6];
    srcBytes_dword_ptr[3] ^= srcKey->ExpandedKey[7];

    uint32_t T0 = func_h_192(srcBytes_dword_ptr[0], srcKey->SBox);
    uint32_t T1 = _rotl(srcBytes_dword_ptr[1], 8);
    T1 = func_h_192(T1, srcKey->SBox);
    uint32_t F0 = T0 + T1 + srcKey->ExpandedKey[38];
    uint32_t F1 = T0 + T1 * 2u + srcKey->ExpandedKey[39];

    srcBytes_dword_ptr[2] = _rotl(srcBytes_dword_ptr[2], 1) ^ F0;
    F1 ^= srcBytes_dword_ptr[3];
    srcBytes_dword_ptr[3] = _rotr(F1, 1);

    for (int Round = 14; Round >= 0; --Round) {
        T0 = func_h_192(srcBytes_dword_ptr[2], srcKey->SBox);
        T1 = func_h_192(_rotl(srcBytes_dword_ptr[3], 8), srcKey->SBox);
        F0 = T0 + T1 + srcKey->ExpandedKey[2 * Round + 8];
        F1 = T0 + T1 * 2u + srcKey->ExpandedKey[2 * Round + 9];

        F0 ^= _rotl(srcBytes_dword_ptr[0], 1);
        F1 ^= srcBytes_dword_ptr[1];
        srcBytes_dword_ptr[0] = srcBytes_dword_ptr[2];
        srcBytes_dword_ptr[1] = srcBytes_dword_ptr[3];
        srcBytes_dword_ptr[3] = _rotr(F1, 1);
        srcBytes_dword_ptr[2] = F0;
    }

    srcBytes_dword_ptr[0] ^= srcKey->ExpandedKey[0];
    srcBytes_dword_ptr[1] ^= srcKey->ExpandedKey[1];
    srcBytes_dword_ptr[2] ^= srcKey->ExpandedKey[2];
    srcBytes_dword_ptr[3] ^= srcKey->ExpandedKey[3];
}

void accelc_Twofish192_set_key(const uint8_t srcKey[TWOFISH192_USERKEY_SIZE], TWOFISH_KEY* dstKey) {
    uint32_t M_e[3] = { ((const uint32_t*)srcKey)[0], ((const uint32_t*)srcKey)[2], ((const uint32_t*)srcKey)[4] };
    uint32_t M_o[3] = { ((const uint32_t*)srcKey)[1], ((const uint32_t*)srcKey)[3], ((const uint32_t*)srcKey)[5] };

    dstKey->SBox[0] = 0;
    dstKey->SBox[1] = 0;
    dstKey->SBox[2] = 0;
    dstKey->SBox[3] = 0;

    for (int i = 8; i < 12; ++i) {
        for (int j = 0; j < 8; ++j)
            ((uint8_t*)dstKey->SBox)[i] ^= accelc_Twofish_GF0x14D_Mul_Table[accelc_Twofish_RS[i - 8][j]][srcKey[j]];
    }
    for (int i = 4; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            ((uint8_t*)dstKey->SBox)[i] ^= accelc_Twofish_GF0x14D_Mul_Table[accelc_Twofish_RS[i - 4][j]][srcKey[8 + j]];
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j)
            ((uint8_t*)dstKey->SBox)[i] ^= accelc_Twofish_GF0x14D_Mul_Table[accelc_Twofish_RS[i][j]][srcKey[16 + j]];
    }

    const uint32_t Rou = 0x01010101;
    for (uint32_t i = 0; i < 20u; ++i) {
        uint32_t A = func_h_192(2u * i * Rou, M_e);
        uint32_t B = func_h_192((2u * i + 1u) * Rou, M_o);
        B = _rotl(B, 8);
        dstKey->ExpandedKey[2u * i] = A + B;
        dstKey->ExpandedKey[2u * i + 1u] = _rotl(A + 2u * B, 9);
    }
}

static uint32_t func_h_256(uint32_t X, const uint32_t L[4]) {
    ((uint8_t*)&X)[0] = accelc_Twofish_q1[((uint8_t*)&X)[0]] ^ ((const uint8_t*)L)[12];
    ((uint8_t*)&X)[1] = accelc_Twofish_q0[((uint8_t*)&X)[1]] ^ ((const uint8_t*)L)[13];
    ((uint8_t*)&X)[2] = accelc_Twofish_q0[((uint8_t*)&X)[2]] ^ ((const uint8_t*)L)[14];
    ((uint8_t*)&X)[3] = accelc_Twofish_q1[((uint8_t*)&X)[3]] ^ ((const uint8_t*)L)[15];

    ((uint8_t*)&X)[0] = accelc_Twofish_q1[((uint8_t*)&X)[0]] ^ ((const uint8_t*)L)[8];
    ((uint8_t*)&X)[1] = accelc_Twofish_q1[((uint8_t*)&X)[1]] ^ ((const uint8_t*)L)[9];
    ((uint8_t*)&X)[2] = accelc_Twofish_q0[((uint8_t*)&X)[2]] ^ ((const uint8_t*)L)[10];
    ((uint8_t*)&X)[3] = accelc_Twofish_q0[((uint8_t*)&X)[3]] ^ ((const uint8_t*)L)[11];

    ((uint8_t*)&X)[0] = accelc_Twofish_q1[accelc_Twofish_q0[accelc_Twofish_q0[((uint8_t*)&X)[0]] ^ ((const uint8_t*)L)[4]] ^ ((const uint8_t*)L)[0]];
    ((uint8_t*)&X)[1] = accelc_Twofish_q0[accelc_Twofish_q0[accelc_Twofish_q1[((uint8_t*)&X)[1]] ^ ((const uint8_t*)L)[5]] ^ ((const uint8_t*)L)[1]];
    ((uint8_t*)&X)[2] = accelc_Twofish_q1[accelc_Twofish_q1[accelc_Twofish_q0[((uint8_t*)&X)[2]] ^ ((const uint8_t*)L)[6]] ^ ((const uint8_t*)L)[2]];
    ((uint8_t*)&X)[3] = accelc_Twofish_q0[accelc_Twofish_q1[accelc_Twofish_q1[((uint8_t*)&X)[3]] ^ ((const uint8_t*)L)[7]] ^ ((const uint8_t*)L)[3]];

    uint8_t z[4] = { 0 };
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            z[i] ^= accelc_Twofish_GF0x169_Mul_Table[accelc_Twofish_MDS[i][j]][((uint8_t*)&X)[j]];
        }
    }

    return *(uint32_t*)z;
}

void accelc_Twofish256_encrypt(uint8_t srcBytes[TWOFISH_BLOCK_SIZE], const TWOFISH_KEY* srcKey) {
    uint32_t* const srcBytes_dword_ptr = (uint32_t*)srcBytes;

    srcBytes_dword_ptr[0] ^= srcKey->ExpandedKey[0];
    srcBytes_dword_ptr[1] ^= srcKey->ExpandedKey[1];
    srcBytes_dword_ptr[2] ^= srcKey->ExpandedKey[2];
    srcBytes_dword_ptr[3] ^= srcKey->ExpandedKey[3];

    for (int Round = 0; Round < 15; ++Round) {
        uint32_t T0 = func_h_256(srcBytes_dword_ptr[0], srcKey->SBox);
        uint32_t T1 = _rotl(srcBytes_dword_ptr[1], 8);
        T1 = func_h_256(T1, srcKey->SBox);
        uint32_t F0 = T0 + T1 + srcKey->ExpandedKey[2 * Round + 8];
        uint32_t F1 = T0 + T1 * 2u + srcKey->ExpandedKey[2 * Round + 9];

        F0 ^= srcBytes_dword_ptr[2];
        F1 ^= _rotl(srcBytes_dword_ptr[3], 1);
        srcBytes_dword_ptr[2] = srcBytes_dword_ptr[0];
        srcBytes_dword_ptr[3] = srcBytes_dword_ptr[1];
        srcBytes_dword_ptr[0] = _rotr(F0, 1);
        srcBytes_dword_ptr[1] = F1;
    }

    uint32_t T0 = func_h_256(srcBytes_dword_ptr[0], srcKey->SBox);
    uint32_t T1 = _rotl(srcBytes_dword_ptr[1], 8);
    T1 = func_h_256(T1, srcKey->SBox);
    uint32_t F0 = T0 + T1 + srcKey->ExpandedKey[38];
    uint32_t F1 = T0 + T1 * 2u + srcKey->ExpandedKey[39];

    F0 ^= srcBytes_dword_ptr[2];
    F1 ^= _rotl(srcBytes_dword_ptr[3], 1);
    srcBytes_dword_ptr[2] = _rotr(F0, 1);
    srcBytes_dword_ptr[3] = F1;

    srcBytes_dword_ptr[0] ^= srcKey->ExpandedKey[4];
    srcBytes_dword_ptr[1] ^= srcKey->ExpandedKey[5];
    srcBytes_dword_ptr[2] ^= srcKey->ExpandedKey[6];
    srcBytes_dword_ptr[3] ^= srcKey->ExpandedKey[7];
}

void accelc_Twofish256_decrypt(uint8_t srcBytes[TWOFISH_BLOCK_SIZE], const TWOFISH_KEY* srcKey) {
    uint32_t* const srcBytes_dword_ptr = (uint32_t*)srcBytes;

    srcBytes_dword_ptr[0] ^= srcKey->ExpandedKey[4];
    srcBytes_dword_ptr[1] ^= srcKey->ExpandedKey[5];
    srcBytes_dword_ptr[2] ^= srcKey->ExpandedKey[6];
    srcBytes_dword_ptr[3] ^= srcKey->ExpandedKey[7];

    uint32_t T0 = func_h_256(srcBytes_dword_ptr[0], srcKey->SBox);
    uint32_t T1 = _rotl(srcBytes_dword_ptr[1], 8);
    T1 = func_h_256(T1, srcKey->SBox);
    uint32_t F0 = T0 + T1 + srcKey->ExpandedKey[38];
    uint32_t F1 = T0 + T1 * 2u + srcKey->ExpandedKey[39];

    srcBytes_dword_ptr[2] = _rotl(srcBytes_dword_ptr[2], 1) ^ F0;
    F1 ^= srcBytes_dword_ptr[3];
    srcBytes_dword_ptr[3] = _rotr(F1, 1);

    for (int Round = 14; Round >= 0; --Round) {
        T0 = func_h_256(srcBytes_dword_ptr[2], srcKey->SBox);
        T1 = func_h_256(_rotl(srcBytes_dword_ptr[3], 8), srcKey->SBox);
        F0 = T0 + T1 + srcKey->ExpandedKey[2 * Round + 8];
        F1 = T0 + T1 * 2u + srcKey->ExpandedKey[2 * Round + 9];


        F0 ^= _rotl(srcBytes_dword_ptr[0], 1);
        F1 ^= srcBytes_dword_ptr[1];
        srcBytes_dword_ptr[0] = srcBytes_dword_ptr[2];
        srcBytes_dword_ptr[1] = srcBytes_dword_ptr[3];
        srcBytes_dword_ptr[3] = _rotr(F1, 1);
        srcBytes_dword_ptr[2] = F0;
    }

    srcBytes_dword_ptr[0] ^= srcKey->ExpandedKey[0];
    srcBytes_dword_ptr[1] ^= srcKey->ExpandedKey[1];
    srcBytes_dword_ptr[2] ^= srcKey->ExpandedKey[2];
    srcBytes_dword_ptr[3] ^= srcKey->ExpandedKey[3];
}

void accelc_Twofish256_set_key(const uint8_t srcKey[TWOFISH256_USERKEY_SIZE], TWOFISH_KEY* dstKey) {
    uint32_t M_e[4] = { ((const uint32_t*)srcKey)[0],
                        ((const uint32_t*)srcKey)[2],
                        ((const uint32_t*)srcKey)[4],
                        ((const uint32_t*)srcKey)[6] };

    uint32_t M_o[4] = { ((const uint32_t*)srcKey)[1],
                        ((const uint32_t*)srcKey)[3],
                        ((const uint32_t*)srcKey)[5],
                        ((const uint32_t*)srcKey)[7] };

    dstKey->SBox[0] = 0;
    dstKey->SBox[1] = 0;
    dstKey->SBox[2] = 0;
    dstKey->SBox[3] = 0;

    for (int i = 12; i < 16; ++i) {
        for (int j = 0; j < 8; ++j)
            ((uint8_t*)dstKey->SBox)[i] ^= accelc_Twofish_GF0x14D_Mul_Table[accelc_Twofish_RS[i - 12][j]][srcKey[j]];
    }
    for (int i = 8; i < 12; ++i) {
        for (int j = 0; j < 8; ++j)
            ((uint8_t*)dstKey->SBox)[i] ^= accelc_Twofish_GF0x14D_Mul_Table[accelc_Twofish_RS[i - 8][j]][srcKey[8 + j]];
    }
    for (int i = 4; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            ((uint8_t*)dstKey->SBox)[i] ^= accelc_Twofish_GF0x14D_Mul_Table[accelc_Twofish_RS[i - 4][j]][srcKey[16 + j]];
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j)
            ((uint8_t*)dstKey->SBox)[i] ^= accelc_Twofish_GF0x14D_Mul_Table[accelc_Twofish_RS[i][j]][srcKey[24 + j]];
    }

    const uint32_t Rou = 0x01010101;
    for (uint32_t i = 0; i < 20u; ++i) {
        uint32_t A = func_h_256(2u * i * Rou, M_e);
        uint32_t B = func_h_256((2u * i + 1u) * Rou, M_o);
        B = _rotl(B, 8);
        dstKey->ExpandedKey[2u * i] = A + B;
        dstKey->ExpandedKey[2u * i + 1u] = _rotl(A + 2u * B, 9);
    }
}