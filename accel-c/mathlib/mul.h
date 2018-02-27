#pragma once
#include "def.h"
#include <intrin.h>
#include <memory.h>

#if defined(_M_X64)
#define _addcarry_uintr _addcarry_u64
#define _mulx_uintr _mulx_u64
#elif defined(_M_IX86)
#define _addcarry_uintr _addcarry_u32
#define _mulx_uintr _mulx_u32
#endif

#if defined(__cplusplus)
extern "C" {
#endif

    // Equivalent to "multiplier *= multiplicand;"
    // ASSERT:
    // 1. multiplier_length > 0
    inline uintr_t MATH_MulAssign(uintr_t* multiplier, size_t multiplier_length,
                                  uintr_t multiplicand) {
        uintr_t temp[2];
        uintr_t carry = 0;
        for (size_t i = 0; i < multiplier_length; ++i) {
            temp[0] = _mulx_uintr(multiplier[i], multiplicand, temp + 1);
            carry = temp[1] + _addcarry_uintr(0, temp[0], carry, multiplier + i);
        }

        return carry;
    }

    // Equivalent to "product = multiplier * multiplicand;"
    // Just grade-school multiplication,  time complexity O(n ^ 2)
    // ASSERT:
    // 1. multiplier_length > 0
    // 2. multiplicand_length > 0
    // 3. product length = multiplier_length + multiplicand_length
    inline void MATH_Mul(const uintr_t* __restrict multiplier, size_t multiplier_length,
                         const uintr_t* __restrict multiplicand, size_t multiplicand_length,
                         uintr_t* __restrict product) {
        memset(product, 0, (multiplier_length + multiplicand_length) * sizeof(uintr_t));

        uintr_t temp[2];
        for (size_t i = 0; i < multiplier_length; ++i) {
            for (size_t j = 0; j < multiplicand_length; ++j) {
                temp[0] = _mulx_uintr(multiplicand[j], multiplier[i], temp + 1);
                if (_addcarry_uintr(_addcarry_uintr(0, product[i + j], temp[0], product + i + j),
                                    product[i + j + 1],
                                    temp[1],
                                    product + i + j + 1)) {
                    uintr_t* block = product + i + j + 2;
                    while (_addcarry_uintr(1, *block, 0, block))
                        ++block;
                }
            }
        }
    }

    // Equivalent to "product = multiplier * multiplicand;"
    // Karatsuba multiplication,  time complexity O(n ^ log_2(3)) ~= O(n ^ 1.585)
    // Parameter 'length' is the length of both multiplier and multiplicand
    // ASSERT:
    // 1. length is a power of 2.
    // 2. product length = 2 * length
    // 3. product must be cleared.
    void MATH_Mul_Karatsuba(const uintr_t* multiplier,
                            const uintr_t* multiplicand,
                            size_t length,
                            uintr_t* __restrict product);

#if defined(__cplusplus)
}
#endif

#undef _addcarry_uintr
#undef _mulx_uintr