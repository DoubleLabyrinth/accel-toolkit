/*********************************************************************
* Filename:   mul_Karatsuba.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../mul.h"

#if defined(_M_X64)
#define _addcarry_uintr _addcarry_u64
#define _mulx_uintr _mulx_u64
#elif defined(_M_IX86)
#define _addcarry_uintr _addcarry_u32
#define _mulx_uintr _mulx_u32
#endif

// Equivalent to "product = multiplier * multiplicand;"
// Karatsuba multiplication,  time complexity O(n ^ log_2(3)) ~= O(n ^ 1.585)
// Parameter 'length' is the length of both multiplier and multiplicand
// ASSERT:
// 1. length is a power of 2.
// 2. product length = 2 * length
void MATH_Mul_Karatsuba(const uintr_t* __restrict multiplier,
                        const uintr_t* __restrict multiplicand,
                        size_t length,
                        uintr_t* __restrict product) {
    if (length <= 8) {
        uintr_t temp[2];
        for (size_t i = 0; i < length; ++i) {
            for (size_t j = 0; j < length; ++j) {
                temp[0] = _mulx_uintr(multiplier[j], multiplicand[i], temp + 1);
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
        return;
    }

    MATH_Mul_Karatsuba(multiplier,
                       multiplicand,
                       length / 2,
                       product);
    MATH_Mul_Karatsuba(multiplier + length / 2,
                       multiplicand + length / 2,
                       length / 2,
                       product + length);

    uintr_t* buffer = alloca(length * 2);
    memset(buffer, 0, length * 2);

    uint8_t carry_a = 0;
    const uintr_t* addend_ptr = multiplier + length / 2;
    uintr_t* sum_ptr = buffer;
    for (size_t i = 0; i < length / 2; ++i)
        carry_a = _addcarry_uintr(carry_a, multiplier[i], addend_ptr[i], sum_ptr + i);

    uint8_t carry_b = 0;
    addend_ptr = multiplicand + length / 2;
    sum_ptr = buffer + length / 2;
    for (size_t i = 0; i < length / 2; ++i)
        carry_b = _addcarry_uintr(carry_b, multiplicand[i], addend_ptr[i], sum_ptr + i);

    uint8_t carry = 0;
    MATH_Mul_Karatsuba(buffer,
                       buffer + length / 2,
                       length / 2,
                       buffer + length);

    if (carry_a)
        carry += MATH_AddCarry(buffer + length + length / 2, length / 2,
                               buffer + length / 2, length / 2,
                               0);

    if (carry_b)
        carry += MATH_AddCarry(buffer + length + length / 2, length / 2,
                               buffer, length / 2,
                               0);

    if (carry_a && carry_b)
        carry++;

    carry -= MATH_SubBorrow(buffer + length, length,
                            product, length,
                            0);
    carry -= MATH_SubBorrow(buffer + length, length,
                            product + length, length,
                            0);
    MATH_AddCarry(product + length / 2, length + length / 2,
                  buffer + length, length,
                  0);


    {
        uintr_t* p = product + length + length / 2;
        carry = _addcarry_uintr(0, *p, carry, p);
        while (carry) {
            ++p;
            carry = _addcarry_uintr(carry, *p, 0, p);
        }
    }
}
