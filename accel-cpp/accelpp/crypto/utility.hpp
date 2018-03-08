#pragma once
#include <stdint.h>
#include <stddef.h>
#include <intrin.h>

namespace accelpp::crypto {

    template<size_t _size>
    struct Block {
        static_assert(_size == 1 || _size == 2 || _size == 4 || _size == 8 || _size == 16 || _size == 32, "accelpp::crypto::utility::Block: Not supported.");

        uint8_t value[_size];

        Block<_size>& operator^=(const Block<_size>& other) {
            switch (_size) {
                case 1:
                    value[0] ^= other.value[0];
                    break;
                case 2:
                    *reinterpret_cast<uint16_t*>(value) ^= *reinterpret_cast<uint16_t*>(other.value);
                    break;
                case 4:
                    *reinterpret_cast<uint32_t*>(value) ^= *reinterpret_cast<uint32_t*>(other.value);
                    break;
                case 8:
                    *reinterpret_cast<uint64_t*>(value) ^= *reinterpret_cast<uint64_t*>(other.value);
                    break;
                case 16:
#if defined(_M_X64) || _M_IX86_FP >= 2
                    _mm_storeu_si128(reinterpret_cast<__m128i*>(value),
                                     _mm_xor_si128(_mm_loadu_si128(reinterpret_cast<__m128i*>(value)),
                                                   _mm_loadu_si128(reinterpret_cast<__m128i*>(other.value))
                                     )
                    );
#else
                    reinterpret_cast<uint64_t*>(value)[0] ^= reinterpret_cast<uint64_t*>(other.value)[0];
                    reinterpret_cast<uint64_t*>(value)[1] ^= reinterpret_cast<uint64_t*>(other.value)[1];
#endif
                    break;
                case 32:
#if defined(__AVX2__)
                    _mm256_storeu_si256(reinterpret_cast<__m256i*>(value),
                                     _mm256_xor_si256(_mm256_lddqu_si256(reinterpret_cast<__m256i*>(value)),
                                                      _mm256_lddqu_si256(reinterpret_cast<__m256i*>(other.value))
                                     )
                    );
#else
                    reinterpret_cast<uint64_t*>(value)[0] ^= reinterpret_cast<uint64_t*>(other.value)[0];
                    reinterpret_cast<uint64_t*>(value)[1] ^= reinterpret_cast<uint64_t*>(other.value)[1];
                    reinterpret_cast<uint64_t*>(value)[2] ^= reinterpret_cast<uint64_t*>(other.value)[2];
                    reinterpret_cast<uint64_t*>(value)[3] ^= reinterpret_cast<uint64_t*>(other.value)[3];
#endif
                default:
                    break;  // nothing
            }
            return *this;
        }
    };

}