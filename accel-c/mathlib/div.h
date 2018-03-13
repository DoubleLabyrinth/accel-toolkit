/*********************************************************************
* Filename:   div.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include "def.h"

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(_M_X64)

    uint64_t __fastcall math_uintx_divmod_asm(uint64_t dividend_l, uint64_t dividend_h,
                                              uint64_t divisor,
                                              uint64_t* remainder);

    uint64_t __fastcall math_uintx_div_asm(uint64_t dividend_l, uint64_t dividend_h,
                                           uint64_t divisor);

    uint64_t __fastcall math_uintx_mod_asm(uint64_t dividend_l, uint64_t dividend_h,
                                           uint64_t divisor);

    
#elif defined(_M_IX86)

    __forceinline
    uint32_t __fastcall math_uintx_divmod_asm(uint32_t dividend_l, uint32_t dividend_h,
                                              uint32_t divisor,
                                              uint32_t* remainder) {
        __asm {
            mov eax, dividend_l
            mov edx, dividend_h
            div divisor
            mov ecx, remainder
            mov dword ptr[ecx], edx
        }
    }

    __forceinline
    uint32_t __fastcall math_uintx_div_asm(uint32_t dividend_l, uint32_t dividend_h,
                                           uint32_t divisor) {
        __asm {
            mov eax, dividend_l
            mov edx, dividend_h
            div divisor
        }
    }

    __forceinline
    uint32_t __fastcall math_uintx_mod_asm(uint32_t dividend_l, uint32_t dividend_h,
                                           uint32_t divisor) {
        __asm {
            mov eax, dividend_l
            mov edx, dividend_h
            div divisor
            mov eax, edx
        }
    }
    
#endif

    // Equivalent to "dividend /= divisor;"
    // ASSERT:
    // 1. dividend_length > 0
    coeff_t math_uintx_div_s(coeff_t dividend[], size_t dividend_length,
                             coeff_t divisor);

    // quotient = dividend / divisor
    // The remainder stores in dividend buffer when function return
    // ASSERT:
    // 1. dividend_length > 0 and divisor_length > 0
    // 2. quotient_length >= dividend_length
    void math_uintx_divmod(coeff_t* __restrict dividend, size_t dividend_length,
                           const coeff_t* __restrict divisor, size_t divisor_length,
                           coeff_t* __restrict quotient, size_t quotient_length);

    // dividend %= divisor
    // The return value is the effective length of modulo.
    // ASSERT:
    // 1. dividend_length > 0 and divisor_length > 0
    // 2. quotient_length >= dividend_length
    size_t math_uintx_mod(coeff_t* __restrict dividend, size_t dividend_length,
                          const coeff_t* __restrict divisor, size_t divisor_length);

#if defined(__cplusplus)
}
#endif
