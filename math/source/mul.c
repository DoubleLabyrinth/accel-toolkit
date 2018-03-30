#include "../arithmetic.h"
#include <x86intrin.h>
#include <memory.h>

#if defined(_M_X64) || defined(__x86_64__)
#define _addcarry_coeff _addcarry_u64
#elif defined(_M_IX86) || defined(__i386__)
#define _addcarry_coeff _addcarry_u32
#endif

static __attribute__((always_inline))
inline coeff_t _mul_coeff(coeff_t multiplier, coeff_t multiplicand,
                          coeff_t* product_h) {
    coeff_t product_l;
    __asm__(
#if defined(_M_X64) || defined(__x86_64__)
            "mulq %3;"
#elif defined(_M_IX86) || defined(__i386__)
            "mull %3;"
#endif
            : "=a"(product_l), "=d"(*product_h)
            : "a"(multiplier), "r"(multiplicand)
            );
    return product_l;
}

coeff_t accelc_uintx_mul_s(coeff_t* multiplier, size_t multiplier_length,
                           coeff_t multiplicand) {
    coeff_t temp[2];
    coeff_t carry = 0;
    for (size_t i = 0; i < multiplier_length; ++i) {
        temp[0] = _mul_coeff(multiplier[i], multiplicand, temp + 1);
        carry = temp[1] + _addcarry_coeff(0, temp[0], carry, multiplier + i);
    }

    return carry;
}

void accelc_uintx_multo(const coeff_t* multiplier, size_t multiplier_length,
                        const coeff_t* multiplicand, size_t multiplicand_length,
                        coeff_t* __restrict product) {
    memset(product, 0, (multiplier_length + multiplicand_length) * sizeof(coeff_t));

    coeff_t temp[2];
    for (size_t i = 0; i < multiplier_length; ++i) {
        for (size_t j = 0; j < multiplicand_length; ++j) {
            temp[0] = _mul_coeff(multiplicand[j], multiplier[i], temp + 1);
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

void accelc_uintx_multo_s(const coeff_t* multiplier, size_t multiplier_length,
                          coeff_t multiplicand,
                          coeff_t* __restrict product) {
    coeff_t temp[2];
    coeff_t carry = 0;
    for (size_t i = 0; i < multiplier_length; ++i) {
        temp[0] = _mul_coeff(multiplier[i], multiplicand, temp + 1);
        carry = temp[1] + _addcarry_coeff(0, temp[0], carry, product + i);
    }

    product[multiplier_length] = carry;
}
