#pragma once
#include <stdint.h>
#include <stddef.h>
#include <intrin.h>

namespace accelpp::memory {

    template<size_t size>
    void Reverse(void* src) {
        if (size == 0 || size == 1) return;

#if defined(__AVX2__)   // AVX2 enabled
        constexpr size_t u256_count = size / (2 * sizeof(__m256i));
        constexpr size_t u128_count = size % (2 * sizeof(__m256i)) >= 2 * sizeof(__m128i) ? 1 : 0;
        constexpr size_t u64_count = size % (2 * sizeof(__m128i)) >= 2 * sizeof(uint64_t) ? 1 : 0;


        __m256i* const u256ptr_src = reinterpret_cast<__m256i*>(src);
        __m256i* const u256ptr_src_r = reinterpret_cast<__m256i*>(reinterpret_cast<uint8_t*>(src) + size) - 1;

        __m128i* const u128ptr_src = reinterpret_cast<__m128i*>(u256ptr_src + u256_count);
        __m128i* const u128ptr_src_r = reinterpret_cast<__m128i*>(u256ptr_src_r + 1 - u256_count) - 1;

        uint64_t* const u64ptr_src = reinterpret_cast<uint64_t*>(u128ptr_src + u128_count);
        uint64_t* const u64ptr_src_r = reinterpret_cast<uint64_t*>(u128ptr_src_r + 1 - u128_count) - 1;

        for (size_t i = 0; i < u256_count; ++i) {
            __m256i temp1 = _mm256_lddqu_si256(u256ptr_src + i);
            temp1 = _mm256_permute2x128_si256(temp1, temp1, 1);

            __m256i temp2 = _mm256_shuffle_epi8(_mm256_lddqu_si256(u256ptr_src_r - i),
                                                _mm256_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                                                                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
            );
            _mm256_storeu_si256(u256ptr_src + i,
                                _mm256_permute2x128_si256(temp2, temp2, 1));

            _mm256_storeu_si256(u256ptr_src_r - i,
                                _mm256_shuffle_epi8(temp1,
                                                    _mm256_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                                                                    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
                                )
            );
        }

        if (u128_count) {
            __m128i temp = _mm_loadu_si128(u128ptr_src);
            _mm_storeu_si128(u128ptr_src,
                             _mm_shuffle_epi8(_mm_loadu_si128(u128ptr_src_r),
                                              _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
                             )
            );
            _mm_storeu_si128(u128ptr_src_r,
                             _mm_shuffle_epi8(temp,
                                              _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
                             )
            );
        }

        if (u64_count) {
            uint64_t temp = *u64ptr_src;
            *u64ptr_src = _byteswap_uint64(*u64ptr_src_r);
            *u64ptr_src_r = _byteswap_uint64(temp);
        }
#elif defined(_M_X64) || _M_IX86_FP >= 2    // SSE2 enabled
        constexpr size_t u128_count = size / (2 * sizeof(__m128i));
        constexpr size_t u64_count = size % (2 * sizeof(__m128i)) >= 2 * sizeof(uint64_t) ? 1 : 0;

        __m128i* const u128ptr_src = reinterpret_cast<__m128i*>(src);
        __m128i* const u128ptr_src_r = reinterpret_cast<__m128i*>(reinterpret_cast<uint8_t*>(src) + size) - 1;

        uint64_t* const u64ptr_src = reinterpret_cast<uint64_t*>(u128ptr_src + u128_count);
        uint64_t* const u64ptr_src_r = reinterpret_cast<uint64_t*>(u128ptr_src_r + 1 - u128_count) - 1;

        for (size_t i = 0; i < u128_count; ++i) {
            __m128i temp = _mm_loadu_si128(u128ptr_src + i);
            _mm_storeu_si128(u128ptr_src + i,
                             _mm_shuffle_epi8(_mm_lddqu_si128(u128ptr_src_r - i),
                                              _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
                             )
            );
            _mm_storeu_si128(u128ptr_src_r - i,
                             _mm_shuffle_epi8(temp,
                                              _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)
                             )
            );
        }

        if (u64_count) {
            uint64_t temp = *u64ptr_src;
            *u64ptr_src = _byteswap_uint64(*u64ptr_src_r);
            *u64ptr_src_r = _byteswap_uint64(temp);
        }
#else
        constexpr size_t u64_count = size / (2 * sizeof(uint64_t));

        uint64_t* const u64ptr_src = reinterpret_cast<uint64_t*>(src);
        uint64_t* const u64ptr_src_r = reinterpret_cast<uint64_t*>(reinterpret_cast<uint8_t*>(src) + size) - 1;

        for (size_t i = 0; i < u64_count; ++i) {
            uint64_t temp = u64ptr_src[i];
            u64ptr_src[i] = _byteswap_uint64(*(u64ptr_src_r - i));
            *(u64ptr_src_r - i) = _byteswap_uint64(temp);
        }
#endif
        constexpr size_t u32_count = size % (2 * sizeof(uint64_t)) >= 2 * sizeof(uint32_t) ? 1 : 0;
        constexpr size_t u16_count = size % (2 * sizeof(uint32_t)) >= 2 * sizeof(uint16_t) ? 1 : 0;
        constexpr size_t u8_count = size % (2 * sizeof(uint16_t)) >= 2 * sizeof(uint8_t) ? 1 : 0;

        uint32_t* const u32ptr_src = reinterpret_cast<uint32_t*>(u64ptr_src + u64_count);
        uint32_t* const u32ptr_src_r = reinterpret_cast<uint32_t*>(u64ptr_src_r + 1 - u64_count) - 1;

        uint16_t* const u16ptr_src = reinterpret_cast<uint16_t*>(u32ptr_src + u32_count);
        uint16_t* const u16ptr_src_r = reinterpret_cast<uint16_t*>(u32ptr_src_r + 1 - u32_count) - 1;

        uint8_t* const u8ptr_src = reinterpret_cast<uint8_t*>(u16ptr_src + u16_count);
        uint8_t* const u8ptr_src_r = reinterpret_cast<uint8_t*>(u16ptr_src_r + 1 - u16_count) - 1;

        if (u32_count) {
            uint32_t temp = *u32ptr_src;
            *u32ptr_src = _byteswap_ulong(*u32ptr_src_r);
            *u32ptr_src_r = _byteswap_ulong(temp);
        }

        if (u16_count) {
            uint16_t temp = *u16ptr_src;
            *u16ptr_src = _byteswap_ushort(*u16ptr_src_r);
            *u16ptr_src_r = _byteswap_ushort(temp);
        }

        if (u8_count) {
            uint8_t temp = *u8ptr_src;
            *u8ptr_src = *u8ptr_src_r;
            *u8ptr_src_r = temp;
        }
    }

}