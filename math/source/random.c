#include "../random.h"

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#endif

#if defined(_M_X64) || defined(__x86_64__)
#define _addcarry_coeff _addcarry_u64
#elif defined(_M_IX86) || defined(__i386__)
#define _addcarry_coeff _addcarry_u32
#endif

static __attribute__((always_inline))
inline coeff_t _mod_asm(coeff_t dividend_l, coeff_t dividend_h,
                        coeff_t divisor) {
    coeff_t remainder;
    __asm__(
#if defined(_M_X64) || defined(__x86_64__)
            "divq %3;"
#elif defined(_M_IX86) || defined(__i386__)
            "divl %3;"
#endif
            : "=d"(remainder)
            : "a"(dividend_l), "d"(dividend_h), "r"(divisor));
    return remainder;
}

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
            : "a"(multiplier), "r"(multiplicand));
    return product_l;
}

coeff_t accelc_LCG_next(LCG_PARAM* param) {
    coeff_t temp[2];
    temp[0] = _mul_coeff(param->A, param->Seed, temp + 1);
    if (_addcarry_coeff(_addcarry_coeff(0,
                                        temp[0], param->B,
                                        temp),
                        temp[1], 0,
                        temp + 1)) {
        temp[1] = _mod_asm(temp[1], 1, param->P);
    }

    if (temp[1] >= param->P)
        temp[1] %= param->P;
    param->Seed = _mod_asm(temp[0], temp[1], param->P);
    return param->Seed;
}