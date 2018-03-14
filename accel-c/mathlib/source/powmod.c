#include "../powmod.h"
#include "../mul.h"
#include "../div.h"
#include <memory.h>
#include <malloc.h>

#if defined(_M_X64)
#define _bittest_coeff _bittest64
#define _bittestandreset_coeff _bittestandreset64
#define _mulx_coeff _mulx_u64
#elif defined(_M_IX86)
#define _bittest_coeff _bittest
#define _bittestandreset_coeff _bittestandreset
#define _mulx_coeff _mulx_u32
#else
#error "No matched architecture found."
#endif

coeff_t math_uintx_powmod_s(coeff_t Base, coeff_t Exponent, coeff_t Modulus) {
    coeff_t ret = 1 % Modulus;
    coeff_t temp[2];

    Base %= Modulus;
    while (Exponent) {
        if (Exponent % 2 == 1) {
            temp[0] = _mulx_coeff(ret, Base, temp + 1);
            ret = math_uintx_mod_asm(temp[0], temp[1], Modulus);
        }
        Exponent /= 2;
        temp[0] = _mulx_coeff(Base, Base, temp + 1);
        Base = math_uintx_mod_asm(temp[0], temp[1], Modulus);
    }

    return ret;
}

// ASSERT:
// 1. Base and Modulus have the same length, specified by Length
// 2. Length must a power of 2
// 3. ExponentLength must be at least 1 (and not need to be a power of 2)
void math_uintx_powmod(coeff_t* __restrict Base, size_t Length,
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
#if defined(_M_X64)
        for (int64_t bp = 0; bp < sizeof(int64_t) * 8; ++bp) {
#elif defined(_M_IX86)
        for (long bp = 0; bp < sizeof(long) * 8; ++bp) {
#endif
            if (_bittest_coeff(cur, bp)) {
                math_uintx_mul_to_Karatsuba(ret, _Base, Length, ret_second);
                math_uintx_mod(ret_second, 2 * Length, Modulus, Length);

                coeff_t* temp = ret;
                ret = ret_second;
                ret_second = temp;
            }
            math_uintx_mul_to_Karatsuba(_Base, _Base, Length, _Base_second);
            math_uintx_mod(_Base_second, 2 * Length, Modulus, Length);

            coeff_t* temp = _Base;
            _Base = _Base_second;
            _Base_second = temp;
        }
        cur++;
    }

    coeff_t Exponent_highest = *pExponent_highest;

#if defined(_M_X64)
    for (int64_t bp = 0; bp < sizeof(int64_t) * 8 && Exponent_highest != 0; ++bp) {
#elif defined(_M_IX86)
    for (long bp = 0; bp < sizeof(long) * 8 && Exponent_highest != 0; ++bp) {
#endif
        if (_bittestandreset_coeff(&Exponent_highest, bp)) {
            math_uintx_mul_to_Karatsuba(ret, _Base, Length, ret_second);
            math_uintx_mod(ret_second, 2 * Length, Modulus, Length);
            
            coeff_t* temp = ret;
            ret = ret_second;
            ret_second = temp;
        }
        math_uintx_mul_to_Karatsuba(_Base, _Base, Length, _Base_second);
        math_uintx_mod(_Base_second, 2 * Length, Modulus, Length);
        
        coeff_t* temp = _Base;
        _Base = _Base_second;
        _Base_second = temp;
    }
    memcpy(Base, ret, Length * sizeof(coeff_t));
}