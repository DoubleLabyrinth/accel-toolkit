/*********************************************************************
* Filename:   Twofish192.cpp
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../Twofish192.h"
#include <intrin.h>

extern const uint8_t CRYPTO_Twofish_gf_0x14D_multi_table[256][256];
extern const uint8_t CRYPTO_Twofish_gf_0x169_multi_table[256][256];
extern const uint8_t CRYPTO_Twofish_MDS[4][4];
extern const uint8_t CRYPTO_Twofish_RS[4][8];
extern const uint8_t CRYPTO_Twofish_q0[256];
extern const uint8_t CRYPTO_Twofish_q1[256];

static uint32_t func_h_192(uint32_t X, const uint32_t L[3]) {
    ((uint8_t*)&X)[0] = CRYPTO_Twofish_q1[((uint8_t*)&X)[0]] ^ ((const uint8_t*)L)[8];
    ((uint8_t*)&X)[1] = CRYPTO_Twofish_q1[((uint8_t*)&X)[1]] ^ ((const uint8_t*)L)[9];
    ((uint8_t*)&X)[2] = CRYPTO_Twofish_q0[((uint8_t*)&X)[2]] ^ ((const uint8_t*)L)[10];
    ((uint8_t*)&X)[3] = CRYPTO_Twofish_q0[((uint8_t*)&X)[3]] ^ ((const uint8_t*)L)[11];

    ((uint8_t*)&X)[0] = CRYPTO_Twofish_q1[CRYPTO_Twofish_q0[CRYPTO_Twofish_q0[((uint8_t*)&X)[0]] ^ ((const uint8_t*)L)[4]] ^ ((const uint8_t*)L)[0]];
    ((uint8_t*)&X)[1] = CRYPTO_Twofish_q0[CRYPTO_Twofish_q0[CRYPTO_Twofish_q1[((uint8_t*)&X)[1]] ^ ((const uint8_t*)L)[5]] ^ ((const uint8_t*)L)[1]];
    ((uint8_t*)&X)[2] = CRYPTO_Twofish_q1[CRYPTO_Twofish_q1[CRYPTO_Twofish_q0[((uint8_t*)&X)[2]] ^ ((const uint8_t*)L)[6]] ^ ((const uint8_t*)L)[2]];
    ((uint8_t*)&X)[3] = CRYPTO_Twofish_q0[CRYPTO_Twofish_q1[CRYPTO_Twofish_q1[((uint8_t*)&X)[3]] ^ ((const uint8_t*)L)[7]] ^ ((const uint8_t*)L)[3]];

    uint8_t z[4] = { 0 };
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            z[i] ^= CRYPTO_Twofish_gf_0x169_multi_table[CRYPTO_Twofish_MDS[i][j]][((uint8_t*)&X)[j]];
        }
    }

    return *(uint32_t*)z;
}

void CRYPTO_Twofish192_EncryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[40], const uint32_t srcS[3]) {
    uint32_t* const srcBytes_dword_ptr = (uint32_t*)srcBytes;
    
    srcBytes_dword_ptr[0] ^= srcExpandedKey[0];
    srcBytes_dword_ptr[1] ^= srcExpandedKey[1];
    srcBytes_dword_ptr[2] ^= srcExpandedKey[2];
    srcBytes_dword_ptr[3] ^= srcExpandedKey[3];

    for (int Round = 0; Round < 15; ++Round) {
        uint32_t T0 = func_h_192(srcBytes_dword_ptr[0], srcS);
        uint32_t T1 = _rotl(srcBytes_dword_ptr[1], 8);
        T1 = func_h_192(T1, srcS);
        uint32_t F0 = T0 + T1 + srcExpandedKey[2 * Round + 8];
        uint32_t F1 = T0 + T1 * 2u + srcExpandedKey[2 * Round + 9];

        F0 ^= srcBytes_dword_ptr[2];
        F1 ^= _rotl(srcBytes_dword_ptr[3], 1);
        srcBytes_dword_ptr[2] = srcBytes_dword_ptr[0];
        srcBytes_dword_ptr[3] = srcBytes_dword_ptr[1];
        srcBytes_dword_ptr[0] = _rotr(F0, 1);
        srcBytes_dword_ptr[1] = F1;
    }

    uint32_t T0 = func_h_192(srcBytes_dword_ptr[0], srcS);
    uint32_t T1 = _rotl(srcBytes_dword_ptr[1], 8);
    T1 = func_h_192(T1, srcS);
    uint32_t F0 = T0 + T1 + srcExpandedKey[38];
    uint32_t F1 = T0 + T1 * 2u + srcExpandedKey[39];

    F0 ^= srcBytes_dword_ptr[2];
    F1 ^= _rotl(srcBytes_dword_ptr[3], 1);
    srcBytes_dword_ptr[2] = _rotr(F0, 1);
    srcBytes_dword_ptr[3] = F1;

    srcBytes_dword_ptr[0] ^= srcExpandedKey[4];
    srcBytes_dword_ptr[1] ^= srcExpandedKey[5];
    srcBytes_dword_ptr[2] ^= srcExpandedKey[6];
    srcBytes_dword_ptr[3] ^= srcExpandedKey[7];
}

void CRYPTO_Twofish192_DecryptBlock(uint8_t srcBytes[16], const uint32_t srcExpandedKey[40], const uint32_t srcS[3]) {
    uint32_t* const srcBytes_dword_ptr = (uint32_t*)srcBytes;
    
    srcBytes_dword_ptr[0] ^= srcExpandedKey[4];
    srcBytes_dword_ptr[1] ^= srcExpandedKey[5];
    srcBytes_dword_ptr[2] ^= srcExpandedKey[6];
    srcBytes_dword_ptr[3] ^= srcExpandedKey[7];

    uint32_t T0 = func_h_192(srcBytes_dword_ptr[0], srcS);
    uint32_t T1 = _rotl(srcBytes_dword_ptr[1], 8);
    T1 = func_h_192(T1, srcS);
    uint32_t F0 = T0 + T1 + srcExpandedKey[38];
    uint32_t F1 = T0 + T1 * 2u + srcExpandedKey[39];

    srcBytes_dword_ptr[2] = _rotl(srcBytes_dword_ptr[2], 1) ^ F0;
    F1 ^= srcBytes_dword_ptr[3];
    srcBytes_dword_ptr[3] = _rotr(F1, 1);

    for (int Round = 14; Round >= 0; --Round) {
        T0 = func_h_192(srcBytes_dword_ptr[2], srcS);
        T1 = func_h_192(_rotl(srcBytes_dword_ptr[3], 8), srcS);
        F0 = T0 + T1 + srcExpandedKey[2 * Round + 8];
        F1 = T0 + T1 * 2u + srcExpandedKey[2 * Round + 9];

        F0 ^= _rotl(srcBytes_dword_ptr[0], 1);
        F1 ^= srcBytes_dword_ptr[1];
        srcBytes_dword_ptr[0] = srcBytes_dword_ptr[2];
        srcBytes_dword_ptr[1] = srcBytes_dword_ptr[3];
        srcBytes_dword_ptr[3] = _rotr(F1, 1);
        srcBytes_dword_ptr[2] = F0;
    }

    srcBytes_dword_ptr[0] ^= srcExpandedKey[0];
    srcBytes_dword_ptr[1] ^= srcExpandedKey[1];
    srcBytes_dword_ptr[2] ^= srcExpandedKey[2];
    srcBytes_dword_ptr[3] ^= srcExpandedKey[3];
}

void CRYPTO_Twofish192_KeyExpansion(const uint8_t srcKey[24], uint32_t dstExpandedKey[40], uint32_t dstS[3]) {
    uint32_t M_e[3] = { ((const uint32_t*)srcKey)[0], ((const uint32_t*)srcKey)[2], ((const uint32_t*)srcKey)[4] };
    uint32_t M_o[3] = { ((const uint32_t*)srcKey)[1], ((const uint32_t*)srcKey)[3], ((const uint32_t*)srcKey)[5] };

    dstS[0] = 0;
    dstS[1] = 0;
    dstS[2] = 0;

    for (int i = 8; i < 12; ++i) {
        for (int j = 0; j < 8; ++j)
            ((uint8_t*)dstS)[i] ^= CRYPTO_Twofish_gf_0x14D_multi_table[CRYPTO_Twofish_RS[i - 8][j]][srcKey[j]];
    }
    for (int i = 4; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            ((uint8_t*)dstS)[i] ^= CRYPTO_Twofish_gf_0x14D_multi_table[CRYPTO_Twofish_RS[i - 4][j]][srcKey[8 + j]];
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 8; ++j)
            ((uint8_t*)dstS)[i] ^= CRYPTO_Twofish_gf_0x14D_multi_table[CRYPTO_Twofish_RS[i][j]][srcKey[16 + j]];
    }

    const uint32_t Rou = 0x01010101;
    for (uint32_t i = 0; i < 20u; ++i) {
        uint32_t A = func_h_192(2u * i * Rou, M_e);
        uint32_t B = func_h_192((2u * i + 1u) * Rou, M_o);
        B = _rotl(B, 8);
        dstExpandedKey[2u * i] = A + B;
        dstExpandedKey[2u * i + 1u] = _rotl(A + 2u * B, 9);
    }
}
