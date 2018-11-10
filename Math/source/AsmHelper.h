#pragma once
#include <stdint.h>

extern "C" {

#if defined(_M_IX86)

    static __forceinline
    uint32_t __fastcall _umulmod64(uint32_t a, uint32_t b, uint32_t m) {
        __asm {
            mov ecx, a
            mov eax, b
            mul ecx
            mov ecx, m
            cmp edx, ecx
            jb _umulmod64_label0
            push ebx
            mov ebx, eax
            mov eax, edx
            xor edx, edx
            div ecx
            mov eax, ebx
            pop ebx
        _umulmod64_label0:
            div ecx
            mov eax, edx
        }
    }

    static __forceinline
    uint32_t __fastcall _udiv64(uint32_t dividend_l, uint32_t dividend_h,
                                uint32_t divisor) {
        __asm {
            mov eax, dividend_l
            mov edx, dividend_h
            mov ecx, divisor
            div ecx
        }
    }

    static __forceinline
    uint32_t __fastcall _udivmod64(uint32_t dividend_l, uint32_t dividend_h,
                                   uint32_t divisor,
                                   uint32_t* remainder) {
        __asm {
            mov eax, dividend_l
            mov edx, dividend_h
            mov ecx, divisor
            div ecx
            mov ecx, remainder
            mov dword ptr[ecx], edx
        }
    }

    static __forceinline
    uint32_t __fastcall _umod64(uint32_t dividend_l, uint32_t dividend_h,
                                uint32_t divisor) {
        __asm {
            mov eax, dividend_l
            mov edx, dividend_h
            mov ecx, divisor
            div ecx
            mov eax, edx
        }
    }

#elif defined(_M_X64)

    uint64_t __fastcall _umulmod128(uint64_t a, uint64_t b, uint64_t m);

    uint64_t __fastcall _udiv128(uint64_t dividend_l, uint64_t dividend_h,
                                 uint64_t divisor);

    uint64_t __fastcall _udivmod128(uint64_t dividend_l, uint64_t dividend_h,
                                    uint64_t divisor,
                                    uint64_t* remainder);

    uint64_t __fastcall _umod128(uint64_t dividend_l, uint64_t dividend_h,
                                 uint64_t divisor);

#else
#error "NOT Implement"
#endif

}

