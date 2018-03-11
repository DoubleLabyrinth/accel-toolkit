#pragma once
#include <stdint.h>
#include <stddef.h>
#include <intrin.h>

namespace accelpp::memory {

#if defined(__AVX2__)       /* AVX2 enabled */

#define BinaryOperation_Start(op_for_u256, op_for_u128, op_for_basetype)                                \
    constexpr size_t u256_count = size / sizeof(__m256i);                                               \
    constexpr size_t u128_count = size % sizeof(__m256i) >= sizeof(__m128i) ? 1 : 0;                    \
    constexpr size_t u64_count = size % sizeof(__m128i) >= sizeof(uint64_t) ? 1 : 0;                    \
                                                                                                        \
    __m256i* const u256ptr_dst = reinterpret_cast<__m256i*>(dst);                                       \
    const __m256i* const u256ptr_src = reinterpret_cast<const __m256i*>(src);                           \
                                                                                                        \
    __m128i* const u128ptr_dst = reinterpret_cast<__m128i*>(u256ptr_dst + u256_count);                  \
    const __m128i* const u128ptr_src = reinterpret_cast<const __m128i*>(u256ptr_src + u256_count);      \
                                                                                                        \
    uint64_t* const u64ptr_dst = reinterpret_cast<uint64_t*>(u128ptr_dst + u128_count);                 \
    const uint64_t* const u64ptr_src = reinterpret_cast<const uint64_t*>(u128ptr_src + u128_count);     \
                                                                                                        \
    for (size_t i = 0; i < u256_count; ++i)                                                             \
        _mm256_storeu_si256(u256ptr_dst + i,                                                            \
                            op_for_u256(_mm256_lddqu_si256(u256ptr_dst + i),                            \
                                        _mm256_lddqu_si256(u256ptr_src + i))                            \
        );                                                                                              \
                                                                                                        \
    if (u128_count)                                                                                     \
        _mm_storeu_si128(u128ptr_dst,                                                                   \
                         op_for_u128(_mm_loadu_si128(u128ptr_dst),                                      \
                                     _mm_loadu_si128(u128ptr_src))                                      \
        );                                                                                              \
                                                                                                        \
    if (u64_count)                                                                                      \
        *u64ptr_dst op_for_basetype *u64ptr_src;

#elif defined(_M_X64) || _M_IX86_FP >= 2    /* SSE2 enabled */

#define BinaryOperation_Start(op_for_u256, op_for_u128, op_for_basetype)                                \
    constexpr size_t u128_count = size / sizeof(__m128i);                                               \
    constexpr size_t u64_count = size % sizeof(__m128i) >= sizeof(uint64_t) ? 1 : 0;                    \
                                                                                                        \
    __m128i* const u128ptr_dst = reinterpret_cast<__m128i*>(dst);                                       \
    const __m128i* const u128ptr_src = reinterpret_cast<const __m128i*>(src);                           \
                                                                                                        \
    uint64_t* const u64ptr_dst = reinterpret_cast<uint64_t*>(u128ptr_dst + u128_count);                 \
    const uint64_t* const u64ptr_src = reinterpret_cast<const uint64_t*>(u128ptr_src + u128_count);     \
                                                                                                        \
    for (size_t i = 0; i < u128_count; ++i)                                                             \
        _mm_storeu_si128(u128ptr_dst + i,                                                               \
                         op_for_u128(_mm_loadu_si128(u128ptr_dst + i),                                  \
                                     _mm_loadu_si128(u128ptr_src + i))                                  \
        );                                                                                              \
                                                                                                        \
    if (u64_count)                                                                                      \
        *u64ptr_dst op_for_basetype *u64ptr_src;

#else

#define BinaryOperation_Start(op_for_u256, op_for_u128, op_for_basetype)                                \
    constexpr size_t u64_count = size / sizeof(uint64_t);                                               \
                                                                                                        \
    uint64_t* const u64ptr_dst = reinterpret_cast<uint64_t*>(dst);                                      \
    const uint64_t* const u64ptr_src = reinterpret_cast<const uint64_t*>(src);                          \
                                                                                                        \
    for (size_t i = 0; i < u64_count; ++i)                                                              \
        u64ptr_dst[i] op_for_basetype u64ptr_src[i];

#endif




#define BinaryOperation_End(op_for_basetype)                                                            \
    constexpr size_t u32_count = size % sizeof(uint64_t) >= sizeof(uint32_t) ? 1 : 0;                   \
    constexpr size_t u16_count = size % sizeof(uint32_t) >= sizeof(uint16_t) ? 1 : 0;                   \
    constexpr size_t u8_count = size % sizeof(uint16_t) >= sizeof(uint8_t) ? 1 : 0;                     \
                                                                                                        \
    uint32_t* const u32ptr_dst = reinterpret_cast<uint32_t*>(u64ptr_dst + u64_count);                   \
    const uint32_t* const u32ptr_src = reinterpret_cast<const uint32_t*>(u64ptr_src + u64_count);       \
                                                                                                        \
    uint16_t* const u16ptr_dst = reinterpret_cast<uint16_t*>(u32ptr_dst + u32_count);                   \
    const uint16_t* const u16ptr_src = reinterpret_cast<const uint16_t*>(u32ptr_src + u32_count);       \
                                                                                                        \
    uint8_t* const u8ptr_dst = reinterpret_cast<uint8_t*>(u16ptr_dst + u16_count);                      \
    const uint8_t* const u8ptr_src = reinterpret_cast<const uint8_t*>(u16ptr_src + u16_count);          \
                                                                                                        \
    if (u32_count)                                                                                      \
        *u32ptr_dst op_for_basetype *u32ptr_src;                                                        \
                                                                                                        \
    if (u16_count)                                                                                      \
        *u16ptr_dst op_for_basetype *u16ptr_src;                                                        \
                                                                                                        \
    if (u8_count)                                                                                       \
        *u8ptr_dst op_for_basetype *u8ptr_src;


    template<size_t size>
    void Xor(void* dst, const void* src) {
        BinaryOperation_Start(_mm256_xor_si256, _mm_xor_si128, ^=)
        BinaryOperation_End(^=)
    }

    template<size_t size>
    void And(void* dst, const void* src) {
        BinaryOperation_Start(_mm256_and_si256, _mm_and_si128, &=)
        BinaryOperation_End(&=)
    }

    template<size_t size>
    void Or(void* dst, const void* src) {
        BinaryOperation_Start(_mm256_or_si256, _mm_or_si128, |=)
        BinaryOperation_End(|=)
    }

    template<size_t size>
    void Not(void* dst) {
#if defined(__AVX2__)       /* AVX2 enabled */
    constexpr size_t u256_count = size / sizeof(__m256i);
    constexpr size_t u128_count = size % sizeof(__m256i) >= sizeof(__m128i) ? 1 : 0;
    constexpr size_t u64_count = size % sizeof(__m128i) >= sizeof(uint64_t) ? 1 : 0;
    
    __m256i* const u256ptr_dst = reinterpret_cast<__m256i*>(dst);
    __m128i* const u128ptr_dst = reinterpret_cast<__m128i*>(u256ptr_dst + u256_count);
    uint64_t* const u64ptr_dst = reinterpret_cast<uint64_t*>(u128ptr_dst + u128_count);

    for (size_t i = 0; i < u256_count; ++i)
        _mm256_storeu_si256(u256ptr_dst + i,
                            _mm256_andnot_si256(_mm256_lddqu_si256(u256ptr_dst + i),
                                                _mm256_set1_epi8(-1))
        );

    if (u128_count)
        _mm_storeu_si128(u128ptr_dst,
                         _mm_andnot_si128(_mm_loadu_si128(u128ptr_dst),
                                          _mm_set1_epi8(-1))
        );

    if (u64_count)
        *u64ptr_dst = ~(*u64ptr_dst);

#elif defined(_M_X64) || _M_IX86_FP >= 2    /* SSE2 enabled */
    constexpr size_t u128_count = size / sizeof(__m128i);
    constexpr size_t u64_count = size % sizeof(__m128i) >= sizeof(uint64_t) ? 1 : 0;

    __m128i* const u128ptr_dst = reinterpret_cast<__m128i*>(dst);
    uint64_t* const u64ptr_dst = reinterpret_cast<uint64_t*>(u128ptr_dst + u128_count);

    for (size_t i = 0; i < u128_count; ++i)
        _mm_storeu_si128(u128ptr_dst + i,
                         _mm_andnot_si128(_mm_loadu_si128(u128ptr_dst + i),
                                          _mm_set1_epi8(-1))
        );

    if (u64_count)
        *u64ptr_dst = ~(*u64ptr_dst);
#else
    constexpr size_t u64_count = size / sizeof(uint64_t);

    uint64_t* const u64ptr_dst = reinterpret_cast<uint64_t*>(dst);

    for (size_t i = 0; i < u64_count; ++i)
        u64ptr_dst[i] = ~(u64ptr_src[i]);
#endif
    constexpr size_t u32_count = size % sizeof(uint64_t) >= sizeof(uint32_t) ? 1 : 0;
    constexpr size_t u16_count = size % sizeof(uint32_t) >= sizeof(uint16_t) ? 1 : 0;
    constexpr size_t u8_count = size % sizeof(uint16_t) >= sizeof(uint8_t) ? 1 : 0;

    uint32_t* const u32ptr_dst = reinterpret_cast<uint32_t*>(u64ptr_dst + u64_count);
    uint16_t* const u16ptr_dst = reinterpret_cast<uint16_t*>(u32ptr_dst + u32_count);
    uint8_t* const u8ptr_dst = reinterpret_cast<uint8_t*>(u16ptr_dst + u16_count);

    if (u32_count)
        *u32ptr_dst = ~(*u32ptr_dst);

    if (u16_count)
        *u16ptr_dst = ~(*u16ptr_dst);

    if (u8_count)
        *u8ptr_dst = ~(*u8ptr_dst);
    }

#undef BinaryOperation_Start
#undef BinaryOperation_End

}
