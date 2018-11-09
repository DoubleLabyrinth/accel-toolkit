#pragma once
#include <stddef.h>
#include <stdint.h>

namespace accel::Math {


#if defined(_M_IX86)

    uint8_t UIntAdd(uint32_t a[], size_t a_len,
                    const uint32_t b[], size_t b_len,
                    uint8_t carry);

    uint8_t UIntAdd(uint32_t a[], size_t a_len,
                    uint32_t b,
                    uint8_t carry);

    uint8_t UIntAdd(const uint32_t a[], size_t a_len,
                    const uint32_t b[], size_t b_len,
                    uint32_t sum[], size_t sum_len,
                    uint8_t carry);

    uint8_t UIntAdd(const uint32_t a[], size_t a_len,
                    uint32_t b,
                    uint32_t sum[], size_t sum_len,
                    uint8_t carry);

#elif defined(_M_X64)

    uint8_t UIntAdd(uint64_t a[], size_t a_len,
                    const uint64_t b[], size_t b_len,
                    uint8_t carry);

    uint8_t UIntAdd(uint64_t a[], size_t a_len,
                    uint64_t b,
                    uint8_t carry);

    uint8_t UIntAdd(const uint64_t a[], size_t a_len,
                    const uint64_t b[], size_t b_len,
                    uint64_t sum[], size_t sum_len,
                    uint8_t carry);

    uint8_t UIntAdd(const uint64_t a[], size_t a_len,
                    uint64_t b,
                    uint64_t sum[], size_t sum_len,
                    uint8_t carry);

#else
#error "NOT Implement"
#endif

}

