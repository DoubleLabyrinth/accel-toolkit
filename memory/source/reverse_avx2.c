#include "../reverse.h"

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#define _byteswap_ushort __builtin_bswap16
#define _byteswap_ulong __builtin_bswap32
#define _byteswap_uint64 __builtin_bswap64
#endif

void accelc_memory_reverse_avx2(void* dst, size_t len) {
    if (len == 0 || len == 1) return;

    size_t u256_count = len / (2 * sizeof(__m256i));
    char u128_flag = len % (2 * sizeof(__m256i)) >= 2 * sizeof(__m128i) ? 1 : 0;
    char u64_flag = len % (2 * sizeof(__m128i)) >= 2 * sizeof(uint64_t) ? 1 : 0;

    __m256i* dst_u256 = dst;
    __m256i* dst_u256_r = (__m256i*)((uint8_t*)dst + len) - 1;

    __m128i* dst_u128 = (__m128i*)(dst_u256 + u256_count);
    __m128i* dst_u128_r = (__m128i*)(dst_u256_r + 1 - u256_count) - 1;

    uint64_t* dst_u64 = (uint64_t*)(dst_u128 + u128_flag);
    uint64_t* dst_u64_r = (uint64_t*)(dst_u128_r + 1 - u128_flag) - 1;

    for (size_t i = 0; i < u256_count; ++i) {
        __m256i temp1 = _mm256_lddqu_si256(dst_u256 + i);
        temp1 = _mm256_permute2x128_si256(temp1, temp1, 1);

        __m256i temp2 = _mm256_shuffle_epi8(_mm256_lddqu_si256(dst_u256_r - i),
                                            _mm256_set_epi8(0, 1, 2, 3, 4, 5, 6, 7,
                                                            8, 9, 10, 11, 12, 13, 14, 15,
                                                            0, 1, 2, 3, 4, 5, 6, 7,
                                                            8, 9, 10, 11, 12, 13, 14, 15));
        _mm256_storeu_si256(dst_u256 + i,
                            _mm256_permute2x128_si256(temp2, temp2, 1));

        _mm256_storeu_si256(dst_u256_r - i,
                            _mm256_shuffle_epi8(temp1,
                                                _mm256_set_epi8(0, 1, 2, 3, 4, 5, 6, 7,
                                                                8, 9, 10, 11, 12, 13, 14, 15,
                                                                0, 1, 2, 3, 4, 5, 6, 7,
                                                                8, 9, 10, 11, 12, 13, 14, 15)));
    }

    if (u128_flag) {
        __m128i temp = _mm_loadu_si128(dst_u128);
        _mm_storeu_si128(dst_u128,
                         _mm_shuffle_epi8(_mm_loadu_si128(dst_u128_r),
                                          _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7,
                                                       8, 9, 10, 11, 12, 13, 14, 15)));
        _mm_storeu_si128(dst_u128_r,
                         _mm_shuffle_epi8(temp,
                                          _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7,
                                                       8, 9, 10, 11, 12, 13, 14, 15)));
    }

    if (u64_flag) {
        uint64_t temp = *dst_u64;
        *dst_u64 = _byteswap_uint64(*dst_u64_r);
        *dst_u64_r = _byteswap_uint64(temp);
    }

    char u32_flag = len % (2 * sizeof(uint64_t)) >= 2 * sizeof(uint32_t) ? 1 : 0;
    char u16_flag = len % (2 * sizeof(uint32_t)) >= 2 * sizeof(uint16_t) ? 1 : 0;
    char u8_flag = len % (2 * sizeof(uint16_t)) >= 2 * sizeof(uint8_t) ? 1 : 0;

    uint32_t* dst_u32 = (uint32_t*)(dst_u64 + u64_flag);
    uint32_t* dst_u32_r = (uint32_t*)(dst_u64_r + 1 - u64_flag) - 1;

    uint16_t* dst_u16 = (uint16_t*)(dst_u32 + u32_flag);
    uint16_t* dst_u16_r = (uint16_t*)(dst_u32_r + 1 - u32_flag) - 1;

    uint8_t* dst_u8 = (uint8_t*)(dst_u16 + u16_flag);
    uint8_t* dst_u8_r = (uint8_t*)(dst_u16_r + 1 - u16_flag) - 1;

    if (u32_flag) {
        uint32_t temp = *dst_u32;
        *dst_u32 = _byteswap_ulong(*dst_u32_r);
        *dst_u32_r = _byteswap_ulong(temp);
    }

    if (u16_flag) {
        uint16_t temp = *dst_u16;
        *dst_u16 = _byteswap_ushort(*dst_u16_r);
        *dst_u16_r = _byteswap_ushort(temp);
    }

    if (u8_flag) {
        uint8_t temp = *dst_u8;
        *dst_u8 = *dst_u8_r;
        *dst_u8_r = temp;
    }
}