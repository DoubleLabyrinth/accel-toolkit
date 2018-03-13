#pragma once
#include "def.h"

#if defined(__cplusplus)
extern "C" {
#endif

    // Equivalent to "minuend -= (subtrahend + borrow);"
    // ASSERT:
    // 1. minuend_length >= subtrahend_length
    // 2. subtrahend_length > 0
    uint8_t math_uintx_sub(coeff_t* __restrict minuend, size_t minuend_length,
                           const coeff_t* __restrict subtrahend, size_t subtrahend_length,
                           uint8_t borrow);

    // Equivalent to "minuend -= (subtrahend + borrow);"
    // ASSERT:
    // 1. minuend_length > 0
    uint8_t math_uintx_sub_s(coeff_t* __restrict minuend, size_t minuend_length,
                             coeff_t subtrahend,
                             uint8_t borrow);

    // Equivalent to "diff = minuend - (subtrahend + borrow);"
    // ASSERT:
    // 1. minuend_length >= subtrahend_length
    // 2. subtrahend_length > 0
    // 3. diff length >= minuend_length
    uint8_t math_uintx_sub_to(const coeff_t* minuend, size_t minuend_length,
                              const coeff_t* subtrahend, size_t subtrahend_length,
                              uint8_t borrow,
                              coeff_t* __restrict diff);

    // Equivalent to "diff = minuend - (subtrahend + borrow);"
    // ASSERT:
    // 1. minuend_length > 0
    // 3. diff length >= minuend_length
    uint8_t math_uintx_sub_s_to(const coeff_t* minuend, size_t minuend_length,
                                coeff_t subtrahend,
                                uint8_t borrow,
                                coeff_t* __restrict diff);

#if defined(__cplusplus)
}
#endif
