#pragma once
#include "def.h"

#if defined(__cplusplus)
extern "C" {
#endif

    // Equivalent to "summand += addend + carry;"
    // ASSERT:
    // 1. summand_length >= addend_length
    // 2. addend_length > 0
    uint8_t math_uintx_add(coeff_t* __restrict summand, size_t summand_length,
                           const coeff_t* __restrict addend, size_t addend_length,
                           uint8_t carry);

    // Equivalent to "summand += addend + carry;"
    // ASSERT:
    // 1. summand_length > 0
    uint8_t math_uintx_adds(coeff_t* summand, size_t summand_length,
                            coeff_t addend,
                            uint8_t carry);

    // Equivalent to "sum = summand + addend + carry;"
    // ASSERT: 
    // 1. summand_length >= addend_length
    // 2. addend_length > 0
    // 3. sum length >= summand_length
    uint8_t math_uintx_add_to(const coeff_t* summand, size_t summand_length,
                              const coeff_t* addend, size_t addend_length,
                              uint8_t carry,
                              coeff_t* __restrict sum);

    // Equivalent to "sum = summand + addend + carry;"
    // ASSERT: 
    // 1. summand_length > 0
    // 3. sum length >= summand_length
    uint8_t math_uintx_adds_to(const coeff_t* summand, size_t summand_length,
                               coeff_t addend,
                               uint8_t carry,
                               coeff_t* __restrict sum);

#if defined(__cplusplus)
}
#endif
