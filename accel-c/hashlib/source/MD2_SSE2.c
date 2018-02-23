/*********************************************************************
* Filename:   MD2_SSE2.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../MD2.h"
#include <intrin.h>

extern const uint8_t HASH_MD2_PI_SUBST[256];

void HASH_MD2_Initialize_SSE2(uint32_t HashBuffer[4]) {
    _mm_storeu_si128((__m128i*)HashBuffer, _mm_setzero_si128());
}

void HASH_MD2_MainCycle_SSE2(const void* srcBytes, size_t srcBytesLength, uint32_t HashBuffer[4]) {
    size_t RoundsOfMainCycle = srcBytesLength / 16;
    const __m128i* MessageBlock = (const __m128i*)srcBytes;
    __m128i Buffer[3];

    Buffer[0] = _mm_loadu_si128((__m128i*)HashBuffer);

    for (size_t i = 0; i < RoundsOfMainCycle; ++i) {
        Buffer[1] = _mm_loadu_si128(MessageBlock + i);
        Buffer[2] = _mm_xor_si128(Buffer[1], Buffer[0]);

        uint8_t t = 0;
        for (int j = 0; j < 18; ++j) {
            for (int k = 0; k < 16; ++k) {
                Buffer[0].m128i_u8[k] ^= HASH_MD2_PI_SUBST[t];
                t = Buffer[0].m128i_u8[k];
            }

            for (int k = 0; k < 16; ++k) {
                Buffer[1].m128i_u8[k] ^= HASH_MD2_PI_SUBST[t];
                t = Buffer[1].m128i_u8[k];
            }

            for (int k = 0; k < 16; ++k) {
                Buffer[2].m128i_u8[k] ^= HASH_MD2_PI_SUBST[t];
                t = Buffer[2].m128i_u8[k];
            }
            
            t += j;
        }
    }

    _mm_storeu_si128((__m128i*)HashBuffer, Buffer[0]);
}
