#pragma once
#include "def.h"

#if defined(__cplusplus)
extern "C" {
#endif

    // Equivalent to "multiplier *= multiplicand;"
    // ASSERT:
    // 1. multiplier_length > 0
    coeff_t math_uintx_mul_s(coeff_t* multiplier, size_t multiplier_length,
                             coeff_t multiplicand);

    // Equivalent to "product = multiplier * multiplicand;"
    // Just grade-school multiplication,  time complexity O(n ^ 2)
    // ASSERT:
    // 1. multiplier_length > 0
    // 2. multiplicand_length > 0
    // 3. product length = multiplier_length + multiplicand_length
    void math_uintx_mul_to(const coeff_t* multiplier, size_t multiplier_length,
                           const coeff_t* multiplicand, size_t multiplicand_length,
                           coeff_t* __restrict product);

    // Equivalent to "product = multiplier * multiplicand;"
    // ASSERT:
    // 1. multiplier_length > 0
    // 3. product length = multiplier_length + 1
    void math_uintx_mul_s_to(const coeff_t* multiplier, size_t multiplier_length,
                             coeff_t multiplicand,
                             coeff_t* __restrict product);

    // Equivalent to "product = multiplier * multiplicand;"
    // Karatsuba multiplication,  time complexity O(n ^ log_2(3)) ~= O(n ^ 1.585)
    // Parameter 'length' is the length of both multiplier and multiplicand
    // ASSERT:
    // 1. length is a power of 2.
    // 2. product length = 2 * length
    // 3. product must be cleared.
    void math_uintx_mul_to_Karatsuba(const coeff_t* multiplier,
                                     const coeff_t* multiplicand,
                                     size_t length,
                                     coeff_t* __restrict product);

#if defined(__cplusplus)
}
#endif
