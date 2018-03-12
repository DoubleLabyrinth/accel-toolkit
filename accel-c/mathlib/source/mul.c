/*********************************************************************
* Filename:   mul.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../mul.h"
#include <intrin.h>
#include <memory.h>

#if defined(_M_X64)
#define _addcarry_coeff _addcarry_u64
#define _mulx_coeff _mulx_u64
#elif defined(_M_IX86)
#define _addcarry_coeff _addcarry_u32
#define _mulx_coeff _mulx_u32
#endif

coeff_t math_uintx_muls(coeff_t* multiplier, size_t multiplier_length,
                        coeff_t multiplicand) {
    coeff_t temp[2];
    coeff_t carry = 0;
    for (size_t i = 0; i < multiplier_length; ++i) {
        temp[0] = _mulx_coeff(multiplier[i], multiplicand, temp + 1);
        carry = temp[1] + _addcarry_coeff(0, temp[0], carry, multiplier + i);
    }

    return carry;
}

void math_uintx_mul_to(const coeff_t* multiplier, size_t multiplier_length,
                       const coeff_t* multiplicand, size_t multiplicand_length,
                       coeff_t* __restrict product) {
    memset(product, 0, (multiplier_length + multiplicand_length) * sizeof(coeff_t));

    coeff_t temp[2];
    for (size_t i = 0; i < multiplier_length; ++i) {
        for (size_t j = 0; j < multiplicand_length; ++j) {
            temp[0] = _mulx_coeff(multiplicand[j], multiplier[i], temp + 1);
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
}

void math_uintx_muls_to(const coeff_t* multiplier, size_t multiplier_length,
                        coeff_t multiplicand,
                        coeff_t* __restrict product) {
    coeff_t temp[2];
    coeff_t carry = 0;
    for (size_t i = 0; i < multiplier_length; ++i) {
        temp[0] = _mulx_coeff(multiplier[i], multiplicand, temp + 1);
        carry = temp[1] + _addcarry_coeff(0, temp[0], carry, product + i);
    }

    product[multiplier_length] = carry;
}
