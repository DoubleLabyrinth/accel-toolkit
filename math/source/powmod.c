#include "../num_theory.h"
#include "../arithmetic.h"
#include <memory.h>
#include <alloca.h>

static __attribute__((always_inline))
inline uint8_t _bittest_coeff(const coeff_t* mask, coeff_t index) {
    uint8_t ret;
    __asm__("xorb %%al, %%al;"
#if defined(_M_X64) || defined(__x86_64__)
            "btq %2, %1;"
#elif defined(_M_IX86) || defined(__i386__)
            "btl %2, %1;"
#endif
            "adcb %%al, %%al;"
            : "=a"(ret)
            : "m"(*mask), "r"(index)
    );
    return ret;
}

static __attribute__((always_inline))
inline uint8_t _bittestandreset_coeff(coeff_t* mask, coeff_t index) {
    uint8_t ret;
    __asm__("xorb %%al, %%al;"
#if defined(_M_X64) || defined(__x86_64__)
            "btrq %2, %1;"
#elif defined(_M_IX86) || defined(__i386__)
            "btrl %2, %1;"
#endif
            "adcb %%al, %%al;"
            : "=a"(ret), "+m"(mask)
            : "r"(index)
    );
    return ret;
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

coeff_t accelc_powmod(coeff_t Base, coeff_t Exponent, coeff_t Modulus) {
    coeff_t ret = 1 % Modulus;
    coeff_t temp[2];

    Base %= Modulus;
    while (Exponent) {
        if (Exponent % 2 == 1) {
            temp[0] = _mul_coeff(ret, Base, temp + 1);
            ret = _mod_asm(temp[0], temp[1], Modulus);
        }
        Exponent /= 2;
        temp[0] = _mul_coeff(Base, Base, temp + 1);
        Base = _mod_asm(temp[0], temp[1], Modulus);
    }

    return ret;
}

coeff_t accelc_Reciprocal_Fermat(coeff_t a, coeff_t P) {
    return accelc_powmod(a, P - 2, P);
}

// ASSERT:
// 1. Base and Modulus have the same length, specified by Length
// 2. Length must a power of 2
// 3. ExponentLength must be at least 1 (and not need to be a power of 2)
void accelc_uintx_powmod(coeff_t* __restrict Base, size_t Length,
                         const coeff_t* Exponent, size_t ExponentLength,
                         const coeff_t* Modulus) {
    // ---alloca memory layout
    // |________|________|________|________|________|________|________|________|
    // |  ret   |        |  ret * Base % c |   Base |        | Base * Base % c |
    // |  ret   |        |   ret_second    |  _Base |        |   _Base_second  |
    coeff_t* ret = alloca(Length * 8 * sizeof(coeff_t));
    memset(ret, 0, Length * sizeof(coeff_t));
    *ret = 1;

    coeff_t* ret_second = ret + 2 * Length;

    coeff_t* _Base = ret + 4 * Length;
    memcpy(_Base, Base, Length * sizeof(coeff_t));

    coeff_t* _Base_second = ret + 6 * Length;

    const coeff_t* pExponent_highest = Exponent + ExponentLength - 1;
    while (*pExponent_highest == 0 && pExponent_highest != Exponent)
        --pExponent_highest;

    const coeff_t* cur = Exponent;
    while (cur != pExponent_highest) {
        for (coeff_t bp = 0; bp < sizeof(coeff_t) * 8; ++bp) {
            if (_bittest_coeff(cur, bp)) {
                accelc_uintx_multo_Karatsuba(ret, _Base, Length, ret_second);
                accelc_uintx_mod(ret_second, 2 * Length, Modulus, Length);

                coeff_t* temp = ret;
                ret = ret_second;
                ret_second = temp;
            }
            accelc_uintx_multo_Karatsuba(_Base, _Base, Length, _Base_second);
            accelc_uintx_mod(_Base_second, 2 * Length, Modulus, Length);

            coeff_t* temp = _Base;
            _Base = _Base_second;
            _Base_second = temp;
        }
        cur++;
    }

    coeff_t Exponent_highest = *pExponent_highest;
    for (coeff_t bp = 0; bp < sizeof(coeff_t) * 8 && Exponent_highest != 0; ++bp) {
        if (_bittestandreset_coeff(&Exponent_highest, bp)) {
            accelc_uintx_multo_Karatsuba(ret, _Base, Length, ret_second);
            accelc_uintx_mod(ret_second, 2 * Length, Modulus, Length);
            
            coeff_t* temp = ret;
            ret = ret_second;
            ret_second = temp;
        }
        accelc_uintx_multo_Karatsuba(_Base, _Base, Length, _Base_second);
        accelc_uintx_mod(_Base_second, 2 * Length, Modulus, Length);
        
        coeff_t* temp = _Base;
        _Base = _Base_second;
        _Base_second = temp;
    }
    memcpy(Base, ret, Length * sizeof(coeff_t));
}