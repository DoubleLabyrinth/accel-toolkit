/*********************************************************************
* Filename:   div.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include "def.h"
#include <intrin.h>

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(_M_X64)

    #if defined(_MSC_VER)

    uint64_t MATH_Divmod_Intrinsic(uint64_t dividend_l, uint64_t dividend_h,
                                   uint64_t divisor,
                                   uint64_t* remainder);

    uint64_t MATH_Div_Intrinsic(uint64_t dividend_l, uint64_t dividend_h,
                                uint64_t divisor);

    uint64_t MATH_Mod_Intrinsic(uint64_t dividend_l, uint64_t dividend_h,
                                uint64_t divisor);

    #elif defined(__GNUC__)

    __attribute__((always_inline))
    uint64_t MATH_Divmod_Intrinsic(uint64_t dividend_l, uint64_t dividend_h,
                                   uint64_t divisor,
                                   uint64_t* remainder) {
        uint64_t quotient;
        __asm__(".intel_syntax;"
                "div rcx;"
                ".att_syntax;"
                : "=a"(quotient), "=d"(*remainder)
                : "a"(dividend_l), "d"(dividend_h), "c"(divisor));
        return quotient;
    }

    __attribute__((always_inline))
    uint64_t MATH_Div_Intrinsic(uint64_t dividend_l, uint64_t dividend_h,
                                uint64_t divisor) {
        uint64_t quotient;
        __asm__(".intel_syntax;"
                "div rcx;"
                ".att_syntax;"
                : "=a"(quotient)
                : "a"(dividend_l), "d"(dividend_h), "c"(divisor));
        return quotient;
    }

    __attribute__((always_inline))
    uint64_t MATH_Mod_Intrinsic(uint64_t dividend_l, uint64_t dividend_h,
                                uint64_t divisor) {
        uint64_t remainder;
        __asm__(".intel_syntax;"
                "div rcx;"
                ".att_syntax;"
                : "=d"(remainder)
                : "a"(dividend_l), "d"(dividend_h), "c"(divisor));
        return remainder;
    }

    #endif
    
#elif defined(_M_IX86)

    #if defined(_MSC_VER)

    __forceinline
    uint32_t __fastcall MATH_Divmod_Intrinsic(uint32_t dividend_l, uint32_t dividend_h,
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
    uint32_t __fastcall MATH_Div_Intrinsic(uint32_t dividend_l, uint32_t dividend_h,
                                           uint32_t divisor) {
        __asm {
            mov eax, dividend_l
            mov edx, dividend_h
            div divisor
        }
    }

    __forceinline
    uint32_t __fastcall MATH_Mod_Intrinsic(uint32_t dividend_l, uint32_t dividend_h,
                                           uint32_t divisor) {
        __asm {
            mov eax, dividend_l
            mov edx, dividend_h
            div divisor
            mov eax, edx
        }
    }

    #elif defined(__GNUC__)

    __attribute__((always_inline))
    uint32_t MATH_Divmod_Intrinsic(uint32_t dividend_l, uint32_t dividend_h,
                                   uint32_t divisor,
                                   uint32_t* remainder) {
        uint32_t quotient;
        __asm__(".intel_syntax;"
                "div ecx;"
                ".att_syntax;"
                : "=a"(quotient), "=d"(*remainder)
                : "a"(dividend_l), "d"(dividend_h), "c"(divisor));
        return quotient;
    }

    __attribute__((always_inline))
    uint32_t MATH_Div_Intrinsic(uint32_t dividend_l, uint32_t dividend_h,
                                uint32_t divisor) {
        uint32_t quotient;
        __asm__(".intel_syntax;"
                "div ecx;"
                ".att_syntax;"
                : "=a"(quotient)
                : "a"(dividend_l), "d"(dividend_h), "c"(divisor));
        return quotient;
    }

    __attribute__((always_inline))
    uint32_t MATH_Mod_Intrinsic(uint32_t dividend_l, uint32_t dividend_h,
                                uint32_t divisor) {
        uint32_t remainder;
        __asm__(".intel_syntax;"
                "div ecx;"
                ".att_syntax;"
                : "=d"(remainder)
                : "a"(dividend_l), "d"(dividend_h), "c"(divisor));
        return remainder;
    }

    #endif
    
#endif

    // Equivalent to "dividend /= divisor;"
    // ASSERT:
    // 1. dividend_length > 0
    inline uintr_t MATH_DivAssign(uintr_t dividend[], size_t dividend_length,
                                  uintr_t divisor) {
        uintr_t* cur = dividend + dividend_length - 1;
        uintr_t remainder = *cur % divisor;
        *cur = *cur / divisor;
        while (cur != dividend) {
            --cur;
            *cur = MATH_Divmod_Intrinsic(*cur, remainder, divisor, &remainder);
        }
        return remainder;
    }

    // quotient = dividend / divisor
    // The remainder stores in dividend buffer when function return
    // ASSERT:
    // 1. dividend_length > 0 and divisor_length > 0
    // 2. quotient_length >= dividend_length
    void MATH_Div(uintr_t* __restrict dividend, size_t dividend_length,
                  const uintr_t* __restrict divisor, size_t divisor_length,
                  uintr_t* __restrict quotient, size_t quotient_length);


#if defined(__cplusplus)
}
#endif