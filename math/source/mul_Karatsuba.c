#include "../arithmetic.h"
#include "../def_asm.h"
#include <memory.h>

#define KARATSUBA_MUL_CUT_LENGTH 8

// Equivalent to "product = multiplier * multiplicand;"
// Karatsuba multiplication,  time complexity O(n ^ log_2(3)) ~= O(n ^ 1.585)
// Parameter 'length' is the length of both multiplier and multiplicand
// ASSERT:
// 1. length is a power of 2.
// 2. product length = 2 * length
void accelc_uintx_multo_Karatsuba(const coeff_t* multiplier,
                                  const coeff_t* multiplicand,
                                  size_t length,
                                  coeff_t* __restrict product) {
    if (length <= KARATSUBA_MUL_CUT_LENGTH) {
        if (length == KARATSUBA_MUL_CUT_LENGTH)
            memset(product, 0, 2 * KARATSUBA_MUL_CUT_LENGTH * sizeof(coeff_t));

        coeff_t temp[2];
        for (size_t i = 0; i < length; ++i) {
            for (size_t j = 0; j < length; ++j) {
                temp[0] = _mul_coeff(multiplier[j], multiplicand[i], temp + 1);
                if (_addcarry_coeff(_addcarry_coeff(0, product[i + j], temp[0], product + i + j),
                                    product[i + j + 1],
                                    temp[1],
                                    product + i + j + 1)) {
                    coeff_t* block = product + i + j + 2;
                    while (_addcarry_coeff(1, *block, 0, block))
                        ++block;
                }
            }
        }
        return;
    }

    accelc_uintx_multo_Karatsuba(multiplier,
                                 multiplicand,
                                 length / 2,
                                 product);
    accelc_uintx_multo_Karatsuba(multiplier + length / 2,
                                 multiplicand + length / 2,
                                 length / 2,
                                 product + length);

    coeff_t* buffer = malloc(length * 2 * sizeof(coeff_t));
    memset(buffer, 0, length * 2 * sizeof(coeff_t));

    uint8_t carry_a = 0;
    const coeff_t* addend_ptr = multiplier + length / 2;
    coeff_t* sum_ptr = buffer;
    for (size_t i = 0; i < length / 2; ++i)
        carry_a = _addcarry_coeff(carry_a, multiplier[i], addend_ptr[i], sum_ptr + i);

    uint8_t carry_b = 0;
    addend_ptr = multiplicand + length / 2;
    sum_ptr = buffer + length / 2;
    for (size_t i = 0; i < length / 2; ++i)
        carry_b = _addcarry_coeff(carry_b, multiplicand[i], addend_ptr[i], sum_ptr + i);

    uint8_t carry = 0;
    accelc_uintx_multo_Karatsuba(buffer,
                                 buffer + length / 2,
                                 length / 2,
                                 buffer + length);

    if (carry_a)
        carry += accelc_uintx_add(buffer + length + length / 2, length / 2,
                                  buffer + length / 2, length / 2,
                                  0);

    if (carry_b)
        carry += accelc_uintx_add(buffer + length + length / 2, length / 2,
                                  buffer, length / 2,
                                  0);

    if (carry_a && carry_b)
        carry++;

    carry -= accelc_uintx_sub(buffer + length, length,
                              product, length,
                              0);
    carry -= accelc_uintx_sub(buffer + length, length,
                              product + length, length,
                              0);
    accelc_uintx_add(product + length / 2, length + length / 2,
                     buffer + length, length,
                     0);

    accelc_uintx_add_s(product + length + length / 2, length / 2,
                       0,
                       carry);
    free(buffer);
}
