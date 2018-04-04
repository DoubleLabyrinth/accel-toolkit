#include "../bitwise.h"

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#endif

#define BinaryOperation_Start(op_for_u256, op_for_u128, op_for_basetype)        \
    size_t u256_len = len / sizeof(__m256i);                                    \
    char u128_flag = len % sizeof(__m256i) >= sizeof(__m128i) ? 1 : 0;          \
    char u64_flag = len % sizeof(__m128i) >= sizeof(uint64_t) ? 1 : 0;          \
                                                                                \
    __m256i* dst_u256 = (__m256i*)(dst);                                        \
    const __m256i* src_u256 = (const __m256i*)(src);                            \
                                                                                \
    __m128i* dst_u128 = (__m128i*)(dst_u256 + u256_len);                        \
    const __m128i* src_u128 = (const __m128i*)(src_u256 + u256_len);            \
                                                                                \
    uint64_t* dst_u64 = (uint64_t*)(dst_u128 + u128_flag);                      \
    const uint64_t* src_u64 = (const uint64_t*)(src_u128 + u128_flag);          \
                                                                                \
    for (size_t i = 0; i < u256_len; ++i)                                       \
        _mm256_storeu_si256(dst_u256 + i,                                       \
                            op_for_u256(_mm256_lddqu_si256(dst_u256 + i),       \
                                        _mm256_lddqu_si256(src_u256 + i)));     \
    if (u128_flag)                                                              \
        _mm_storeu_si128(dst_u128,                                              \
                         op_for_u128(_mm_loadu_si128(dst_u128),                 \
                                     _mm_loadu_si128(src_u128)));               \
    if (u64_flag)                                                               \
        *dst_u64 op_for_basetype *src_u64;

#define BinaryOperation_End(op_for_basetype)                                \
    char u32_flag = len % sizeof(uint64_t) >= sizeof(uint32_t) ? 1 : 0;     \
    char u16_flag = len % sizeof(uint32_t) >= sizeof(uint16_t) ? 1 : 0;     \
    char u8_flag = len % sizeof(uint16_t) >= sizeof(uint8_t) ? 1 : 0;       \
                                                                            \
    uint32_t* dst_u32 = (uint32_t*)(dst_u64 + u64_flag);                    \
    const uint32_t* src_u32 = (const uint32_t*)(src_u64 + u64_flag);        \
                                                                            \
    uint16_t* dst_u16 = (uint16_t*)(dst_u32 + u32_flag);                    \
    const uint16_t* src_u16 = (const uint16_t*)(src_u32 + u32_flag);        \
                                                                            \
    uint8_t* dst_u8 = (uint8_t*)(dst_u16 + u16_flag);                       \
    const uint8_t* src_u8 = (const uint8_t*)(src_u16 + u16_flag);           \
                                                                            \
    if (u32_flag)                                                           \
        *dst_u32 op_for_basetype *src_u32;                                  \
                                                                            \
    if (u16_flag)                                                           \
        *dst_u16 op_for_basetype *src_u16;                                  \
                                                                            \
    if (u8_flag)                                                            \
        *dst_u8 op_for_basetype *src_u8;

void accelc_memory_xor_avx2(void* dst, const void* src, size_t len) {
    BinaryOperation_Start(_mm256_xor_si256, _mm_xor_si128, ^=)
    BinaryOperation_End(^=)
}

void accelc_memory_and_avx2(void* dst, const void* src, size_t len) {
    BinaryOperation_Start(_mm256_and_si256, _mm_and_si128, &=)
    BinaryOperation_End(&=)
}

void accelc_memory_or_avx2(void* dst, const void* src, size_t len) {
    BinaryOperation_Start(_mm256_or_si256, _mm_or_si128, |=)
    BinaryOperation_End(|=)
}

void accelc_memory_not_avx2(void* dst, size_t len) {
    size_t u256_len = len / sizeof(__m256i);
    char u128_flag = len % sizeof(__m256i) >= sizeof(__m128i) ? 1 : 0;
    char u64_flag = len % sizeof(__m128i) >= sizeof(uint64_t) ? 1 : 0;

    __m256i* dst_u256 = (__m256i*)(dst);
    __m128i* dst_u128 = (__m128i*)(dst_u256 + u256_len);
    uint64_t* dst_u64 = (uint64_t*)(dst_u128 + u128_flag);

    for (size_t i = 0; i < u256_len; ++i)
        _mm256_storeu_si256(dst_u256 + i,
                            _mm256_andnot_si256(_mm256_lddqu_si256(dst_u256 + i),
                                                _mm256_set1_epi8(-1)));
    if (u128_flag)
        _mm_storeu_si128(dst_u128,
                         _mm_andnot_si128(_mm_loadu_si128(dst_u128),
                                          _mm_set1_epi8(-1)));
    if (u64_flag)
        *dst_u64 = ~(*dst_u64);

    char u32_flag = len % sizeof(uint64_t) >= sizeof(uint32_t) ? 1 : 0;
    char u16_flag = len % sizeof(uint32_t) >= sizeof(uint16_t) ? 1 : 0;
    char u8_flag = len % sizeof(uint16_t) >= sizeof(uint8_t) ? 1 : 0;

    uint32_t* dst_u32 = (uint32_t*)(dst_u64 + u64_flag);
    uint16_t* dst_u16 = (uint16_t*)(dst_u32 + u32_flag);
    uint8_t* dst_u8 = (uint8_t*)(dst_u16 + u16_flag);

    if (u32_flag)
        *dst_u32 = ~(*dst_u32);

    if (u16_flag)
        *dst_u16 = ~(*dst_u16);

    if (u8_flag)
        *dst_u8 = ~(*dst_u8);
}
