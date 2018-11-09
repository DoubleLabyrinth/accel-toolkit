#pragma once
#include <stddef.h>
#include <stdint.h>

namespace accel::Math {


#if defined(_M_IX86)

    uint8_t UIntSub(uint32_t a[], size_t a_len,
                    const uint32_t b[], size_t b_len,
                    uint8_t borrow);

    uint8_t UIntSub(uint32_t a[], size_t a_len,
                    uint32_t b,
                    uint8_t borrow);

    uint8_t UIntSub(const uint32_t a[], size_t a_len,
                    const uint32_t b[], size_t b_len,
                    uint32_t diff[], size_t diff_len,
                    uint8_t borrow);

    uint8_t UIntSub(const uint32_t a[], size_t a_len,
                    uint32_t b,
                    uint32_t diff[], size_t diff_len,
                    uint8_t borrow);

#elif defined(_M_X64)

    uint8_t UIntSub(uint64_t a[], size_t a_len,
                    const uint64_t b[], size_t b_len,
                    uint8_t borrow);

    uint8_t UIntSub(uint64_t a[], size_t a_len,
                    uint64_t b,
                    uint8_t borrow);

    uint8_t UIntSub(const uint64_t a[], size_t a_len,
                    const uint64_t b[], size_t b_len,
                    uint64_t diff[], size_t diff_len,
                    uint8_t borrow);

    uint8_t UIntSub(const uint64_t a[], size_t a_len,
                    uint64_t b,
                    uint64_t diff[], size_t diff_len,
                    uint8_t borrow);

#else
#error "NOT Implement"
#endif

}

