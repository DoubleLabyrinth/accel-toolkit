#include "../powmod.h"
#include "../mul.h"
#include "../div.h"

#if defined(_M_X64)
#define _bittest_uintr _bittest64
#define _bittestandreset_uintr _bittestandreset64
#elif defined(_M_IX86)
#define _bittest_uintr _bittest
#define _bittestandreset_uintr _bittestandreset
#else
#error "No matched architecture found."
#endif

// ASSERT:
// 1. Base and Modulus have the same length, specified by Length
// 2. Length must a power of 2
// 3. ExponentLength must be at least 1 (and not need to be a power of 2)
void MATH_PowMod(uintr_t* __restrict Base, size_t Length, 
                 const uintr_t* Exponent, size_t ExponentLength, 
                 const uintr_t* Modulus) {
    // ---alloca memory layout
    // |________|________|________|________|________|________|________|________|
    // |  ret   |        |  ret * Base % c |   Base |        | Base * Base % c |
    // |  ret   |        |   ret_second    |  _Base |        |   _Base_second  |
    uintr_t* ret = alloca(Length * 8 * sizeof(uintr_t));
    memset(ret, 0, Length * sizeof(uintr_t));
    *ret = 1;

    uintr_t* ret_second = ret + 2 * Length;
    memset(ret_second, 0, Length * 2 * sizeof(uintr_t));

    uintr_t* _Base = ret + 4 * Length;
    uintr_t* _Base_second = ret + 6 * Length;
    memcpy(_Base, Base, Length * sizeof(uintr_t));
    memset(_Base_second, 0, Length * 2 * sizeof(uintr_t));

    const uintr_t* pExponent_highest = Exponent + ExponentLength - 1;
    while (*pExponent_highest == 0 && pExponent_highest != Exponent)
        --pExponent_highest;
    
    const uintr_t* cur = Exponent;
    while (cur != pExponent_highest) {
#if defined(_M_X64)
        for (int64_t bp = 0; bp < sizeof(int64_t) * 8; ++bp) {
#elif defined(_M_IX86)
        for (long bp = 0; bp < sizeof(long) * 8; ++bp) {
#endif
            if (_bittest_uintr(cur, bp)) {
                MATH_Mul_Karatsuba(ret, _Base, Length, ret_second);
                MATH_Mod(ret_second, 2 * Length, Modulus, Length);
                memset(ret, 0, Length * 2 * sizeof(uintr_t));
                uintr_t* temp = ret;
                ret = ret_second;
                ret_second = temp;
            }
            MATH_Mul_Karatsuba(_Base, _Base, Length, _Base_second);
            MATH_Mod(_Base_second, 2 * Length, Modulus, Length);
            memset(_Base, 0, Length * 2 * sizeof(uintr_t));
            uintr_t* temp = _Base;
            _Base = _Base_second;
            _Base_second = temp;
        }
        cur++;
    }

    uintr_t Exponent_highest = *pExponent_highest;

#if defined(_M_X64)
    for (int64_t bp = 0; bp < sizeof(int64_t) * 8 && Exponent_highest != 0; ++bp) {
#elif defined(_M_IX86)
    for (long bp = 0; bp < sizeof(long) * 8 && Exponent_highest != 0; ++bp) {
#endif
        if (_bittestandreset_uintr(&Exponent_highest, bp)) {
            MATH_Mul_Karatsuba(ret, _Base, Length, ret_second);
            MATH_Mod(ret_second, 2 * Length, Modulus, Length);
            memset(ret, 0, Length * 2 * sizeof(uintr_t));
            uintr_t* temp = ret;
            ret = ret_second;
            ret_second = temp;
        }
        MATH_Mul_Karatsuba(_Base, _Base, Length, _Base_second);
        MATH_Mod(_Base_second, 2 * Length, Modulus, Length);
        memset(_Base, 0, Length * 2 * sizeof(uintr_t));
        uintr_t* temp = _Base;
        _Base = _Base_second;
        _Base_second = temp;
    }
    memcpy(Base, ret, Length * sizeof(uintr_t));
}