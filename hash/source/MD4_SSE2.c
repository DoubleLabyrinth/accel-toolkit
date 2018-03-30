/*********************************************************************
* Filename:   MD4_SSE2.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../MD4.h"
#include <intrin.h>

#define HASH_MD4_BLOCKSIZE 64

void HASH_MD4_Initialize_SSE2(uint32_t HashBuffer[4]) {
    _mm_storeu_si128((__m128i*)HashBuffer, _mm_set_epi32(0x10325476, 0x98BADCFE, 0xEFCDAB89, 0x67452301));
}

