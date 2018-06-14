#pragma once
#include "def.h"

#if defined(__cplusplus)
extern "C" {
#endif

    // Equivalent to "summand += addend + carry;"
    // Return carry flag
    // ASSERT:
    // 1. summand_length >= addend_length
    // 2. addend_length > 0
    uint8_t accelc_uintx_add(coeff_t* __restrict summand, size_t summand_length,
                             const coeff_t* __restrict addend, size_t addend_length,
                             uint8_t carry);

    // Equivalent to "summand += addend + carry;"
    // Return carry flag
    // ASSERT:
    // 1. summand_length > 0
    uint8_t accelc_uintx_fastadd(coeff_t* summand, size_t summand_length,
                                 coeff_t addend,
                                 uint8_t carry);

    // Equivalent to "sum = summand + addend + carry;"
    // Return carry flag
    // ASSERT:
    // 1. summand_length >= addend_length
    // 2. addend_length > 0
    // 3. sum length >= summand_length
    uint8_t accelc_uintx_addto(const coeff_t* summand, size_t summand_length,
                               const coeff_t* addend, size_t addend_length,
                               uint8_t carry,
                               coeff_t* __restrict sum);

    // Equivalent to "sum = summand + addend + carry;"
    // Return carry flag
    // ASSERT:
    // 1. summand_length > 0
    // 3. sum length >= summand_length
    uint8_t accelc_uintx_fastaddto(const coeff_t* summand, size_t summand_length,
                                   coeff_t addend,
                                   uint8_t carry,
                                   coeff_t* __restrict sum);

    // Equivalent to "minuend -= (subtrahend + borrow);"
    // Return carry(borrow) flag
    // ASSERT:
    // 1. minuend_length >= subtrahend_length
    // 2. subtrahend_length > 0
    uint8_t accelc_uintx_sub(coeff_t* __restrict minuend, size_t minuend_length,
                             const coeff_t* __restrict subtrahend, size_t subtrahend_length,
                             uint8_t borrow);

    // Equivalent to "minuend -= (subtrahend + borrow);"
    // Return carry(borrow) flag
    // ASSERT:
    // 1. minuend_length > 0
    uint8_t accelc_uintx_fastsub(coeff_t* __restrict minuend, size_t minuend_length,
                                 coeff_t subtrahend,
                                 uint8_t borrow);

    // Equivalent to "diff = minuend - (subtrahend + borrow);"
    // Return carry(borrow) flag
    // ASSERT:
    // 1. minuend_length >= subtrahend_length
    // 2. subtrahend_length > 0
    // 3. diff length >= minuend_length
    uint8_t accelc_uintx_subto(const coeff_t* minuend, size_t minuend_length,
                               const coeff_t* subtrahend, size_t subtrahend_length,
                               uint8_t borrow,
                               coeff_t* __restrict diff);

    // Equivalent to "diff = minuend - (subtrahend + borrow);"
    // Return carry(borrow) flag
    // ASSERT:
    // 1. minuend_length > 0
    // 3. diff length >= minuend_length
    uint8_t accelc_uintx_fastsubto(const coeff_t* minuend, size_t minuend_length,
                                   coeff_t subtrahend,
                                   uint8_t borrow,
                                   coeff_t* __restrict diff);

    // Equivalent to "multiplier *= multiplicand;"
    // Return carry coeff_t
    // ASSERT:
    // 1. multiplier_length > 0
    coeff_t accelc_uintx_fastmul(coeff_t* multiplier, size_t multiplier_length,
                                 coeff_t multiplicand);

    // Equivalent to "product = multiplier * multiplicand;"
    // Just grade-school multiplication,  time complexity O(n ^ 2)
    // ASSERT:
    // 1. multiplier_length > 0
    // 2. multiplicand_length > 0
    // 3. product length = multiplier_length + multiplicand_length
    void accelc_uintx_multo(const coeff_t* multiplier, size_t multiplier_length,
                            const coeff_t* multiplicand, size_t multiplicand_length,
                            coeff_t* __restrict product);

    // Equivalent to "product = multiplier * multiplicand;"
    // ASSERT:
    // 1. multiplier_length > 0
    // 3. product buffer length >= multiplier_length
    coeff_t accelc_uintx_fastmulto(const coeff_t* multiplier, size_t multiplier_length,
                                   coeff_t multiplicand,
                                   coeff_t* __restrict product);

    // Equivalent to "product = multiplier * multiplicand;"
    // Karatsuba multiplication,  time complexity O(n ^ log_2(3)) ~= O(n ^ 1.585)
    // Parameter 'length' is the length of both multiplier and multiplicand
    // ASSERT:
    // 1. length is a power of 2.
    // 2. product length = 2 * length
    // 3. product must be cleared.
    void accelc_uintx_multo_Karatsuba(const coeff_t* multiplier,
                                      const coeff_t* multiplicand,
                                      size_t length,
                                      coeff_t* __restrict product);

    int accelc_uintx_multo_FNTT(const coeff_t* multiplier, const coeff_t* multiplicand,
                                size_t length,
                                coeff_t* __restrict product);

    // Equivalent to "dividend /= divisor;"
    // ASSERT:
    // 1. dividend_length > 0
    coeff_t accelc_uintx_div_s(coeff_t* dividend, size_t dividend_length,
                               coeff_t divisor);

    // quotient = dividend / divisor
    // The remainder stores in dividend buffer when function return
    // ASSERT:
    // 1. dividend_length > 0 and divisor_length > 0
    // 2. quotient_length >= dividend_length
    void accelc_uintx_divmod(coeff_t* __restrict dividend, size_t dividend_length,
                             const coeff_t* __restrict divisor, size_t divisor_length,
                             coeff_t* __restrict quotient, size_t quotient_length);

    // dividend %= divisor
    // The return value is the effective length of modulo.
    // ASSERT:
    // 1. dividend_length > 0 and divisor_length > 0
    // 2. quotient_length >= dividend_length
    size_t accelc_uintx_mod(coeff_t* __restrict dividend, size_t dividend_length,
                            const coeff_t* __restrict divisor, size_t divisor_length);

#if defined(__cplusplus)
}
#endif
