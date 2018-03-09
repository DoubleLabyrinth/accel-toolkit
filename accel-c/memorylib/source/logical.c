#include "../memory.h"

void MEMORY_XOR(void* dst, const void *src, size_t size) {
#if defined(__AVX512F__)
#error "Todo feature"
#elif defined(__AVX2__)  // if AVX2 enabled
    const size_t round = size / (256 / 8);
    const size_t left = size % (256 / 8);
    __m256i* p_m256i_dst = (__m256i*)dst;
    const __m256i* p_m256i_src = (const __m256i*)src;

    for (size_t i = 0; i < round; ++i)
        _mm256_storeu_si256(p_m256i_dst + i,
                            _mm256_xor_si256(_mm256_lddqu_si256(p_m256i_dst + i),
                                             _mm256_lddqu_si256(p_m256i_src + i))
        );

    for (size_t i = 0; i < left; ++i)
        p_m256i_dst[round].m256i_u8[i] ^= p_m256i_src[round].m256i_u8[i];
#elif defined(_M_X64) || _M_IX86_FP >= 2  // if SSE2 enabled
    const size_t round = size / (128 / 8);
    const size_t left = size % (128 / 8);
    __m128i* p_m128i_dst = (__m128i*)dst;
    const __m128i* p_m128i_src = (const __m128i*)src;

    for (size_t i = 0; i < round; ++i)
        _mm_storeu_si128(p_m128i_dst + i,
                         _mm_xor_si128(_mm_loadu_si128(p_m128i_dst + i),
                                       _mm_loadu_si128(p_m128i_src + i))
        );

    for (size_t i = 0; i < left; ++i)
        p_m128i_dst[round].m128i_u8[i] ^= p_m128i_src[round].m128i_u8[i];
#else
    const size_t round = size / (64 / 8);
    const size_t left = size % (64 / 8);
    uint64_t* p_u64_dst = (uint64_t*)dst;
    const uint64_t* p_u64_src = (const uint64_t*)src;

    for (size_t i = 0; i < round; ++i)
        p_u64_dst[i] ^= p_u64_src[i];

    for (size_t i = 0; i < left; ++i)
        ((uint8_t*)(p_u64_dst + round))[i] ^= ((uint8_t*)(p_u64_src + round))[i];
#endif
}

void MEMORY_AND(void* dst, const void *src, size_t size) {
#if defined(__AVX512F__)
#error "Todo feature"
#elif defined(__AVX2__)  // if AVX2 enabled
    const size_t round = size / (256 / 8);
    const size_t left = size % (256 / 8);
    __m256i* p_m256i_dst = (__m256i*)dst;
    const __m256i* p_m256i_src = (const __m256i*)src;

    for (size_t i = 0; i < round; ++i)
        _mm256_storeu_si256(p_m256i_dst + i,
                            _mm256_and_si256(_mm256_lddqu_si256(p_m256i_dst + i),
                                             _mm256_lddqu_si256(p_m256i_src + i))
        );

    for (size_t i = 0; i < left; ++i)
        p_m256i_dst[round].m256i_u8[i] &= p_m256i_src[round].m256i_u8[i];
#elif defined(_M_X64) || _M_IX86_FP >= 2  // if SSE2 enabled
    const size_t round = size / (128 / 8);
    const size_t left = size % (128 / 8);
    __m128i* p_m128i_dst = (__m128i*)dst;
    const __m128i* p_m128i_src = (const __m128i*)src;

    for (size_t i = 0; i < round; ++i)
        _mm_storeu_si128(p_m128i_dst + i,
                         _mm_and_si128(_mm_loadu_si128(p_m128i_dst + i),
                                       _mm_loadu_si128(p_m128i_src + i))
        );

    for (size_t i = 0; i < left; ++i)
        p_m128i_dst[round].m128i_u8[i] &= p_m128i_src[round].m128i_u8[i];
#else
    const size_t round = size / (64 / 8);
    const size_t left = size % (64 / 8);
    uint64_t* p_u64_dst = (uint64_t*)dst;
    const uint64_t* p_u64_src = (const uint64_t*)src;

    for (size_t i = 0; i < round; ++i)
        p_u64_dst[i] &= p_u64_src[i];

    for (size_t i = 0; i < left; ++i)
        ((uint8_t*)(p_u64_dst + round))[i] &= ((uint8_t*)(p_u64_src + round))[i];
#endif
}

void MEMORY_OR(void* dst, const void *src, size_t size) {
#if defined(__AVX512F__)
#error "Todo feature"
#elif defined(__AVX2__)  // if AVX2 enabled
    const size_t round = size / (256 / 8);
    const size_t left = size % (256 / 8);
    __m256i* p_m256i_dst = (__m256i*)dst;
    const __m256i* p_m256i_src = (const __m256i*)src;

    for (size_t i = 0; i < round; ++i)
        _mm256_storeu_si256(p_m256i_dst + i,
                            _mm256_or_si256(_mm256_lddqu_si256(p_m256i_dst + i),
                                            _mm256_lddqu_si256(p_m256i_src + i))
        );

    for (size_t i = 0; i < left; ++i)
        p_m256i_dst[round].m256i_u8[i] |= p_m256i_src[round].m256i_u8[i];
#elif defined(_M_X64) || _M_IX86_FP >= 2  // if SSE2 enabled
    const size_t round = size / (128 / 8);
    const size_t left = size % (128 / 8);
    __m128i* p_m128i_dst = (__m128i*)dst;
    const __m128i* p_m128i_src = (const __m128i*)src;

    for (size_t i = 0; i < round; ++i)
        _mm_storeu_si128(p_m128i_dst + i,
                         _mm_or_si128(_mm_loadu_si128(p_m128i_dst + i),
                                      _mm_loadu_si128(p_m128i_src + i))
        );

    for (size_t i = 0; i < left; ++i)
        p_m128i_dst[round].m128i_u8[i] |= p_m128i_src[round].m128i_u8[i];
#else
    const size_t round = size / (64 / 8);
    const size_t left = size % (64 / 8);
    uint64_t* p_u64_dst = (uint64_t*)dst;
    const uint64_t* p_u64_src = (const uint64_t*)src;

    for (size_t i = 0; i < round; ++i)
        p_u64_dst[i] |= p_u64_src[i];

    for (size_t i = 0; i < left; ++i)
        ((uint8_t*)(p_u64_dst + round))[i] |= ((uint8_t*)(p_u64_src + round))[i];
#endif
}

void MEMORY_NOT(void* dst, size_t size) {
#if defined(__AVX512F__)
#error "Todo feature"
#elif defined(__AVX2__)  // if AVX2 enabled
    const size_t round = size / (256 / 8);
    const size_t left = size % (256 / 8);
    __m256i* p_m256i_dst = (__m256i*)dst;

    for (size_t i = 0; i < round; ++i)
        _mm256_storeu_si256(p_m256i_dst + i,
                            _mm256_andnot_si256(_mm256_lddqu_si256(p_m256i_dst + i),
                                                _mm256_setzero_si256())
        );

    for (size_t i = 0; i < left; ++i)
        p_m256i_dst[round].m256i_u8[i] = ~p_m256i_dst[round].m256i_u8[i];
#elif defined(_M_X64) || _M_IX86_FP >= 2  // if SSE2 enabled
    const size_t round = size / (128 / 8);
    const size_t left = size % (128 / 8);
    __m128i* p_m128i_dst = (__m128i*)dst;

    for (size_t i = 0; i < round; ++i)
        _mm_storeu_si128(p_m128i_dst + i,
                         _mm_andnot_si128(_mm_loadu_si128(p_m128i_dst + i),
                                          _mm_setzero_si128())
        );

    for (size_t i = 0; i < left; ++i)
        p_m128i_dst[round].m128i_u8[i] = ~p_m128i_dst[round].m128i_u8[i];
#else
    const size_t round = size / (64 / 8);
    const size_t left = size % (64 / 8);
    uint64_t* p_u64_dst = (uint64_t*)dst;

    for (size_t i = 0; i < round; ++i)
        p_u64_dst[i] = ~p_u64_dst[i];

    for (size_t i = 0; i < left; ++i)
        ((uint8_t*)(p_u64_dst + round))[i] = ~((uint8_t*)(p_u64_dst + round))[i];
#endif
}
