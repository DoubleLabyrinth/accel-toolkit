#include "../num_theory.h"

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
inline coeff_t _mulmod_coeff(coeff_t multiplier, coeff_t multiplicand,
                             coeff_t modulus) {
    coeff_t ret;
    __asm__(
#if defined(_M_X64) || defined(__x86_64__)
            "mulq %%rdx;"
            "cmpq %%rcx, %%rdx;"
            "jb fin%=;"
            "movq %%rax, %%rbx;"
            "movq %%rdx, %%rax;"
            "xorq %%rdx, %%rdx;"
            "divq %%rcx;"
            "movq %%rbx, %%rax;"
            "fin%=: divq %%rcx;"
#elif defined(_M_IX86) || defined(__i386__)
            "mull %%edx;"
            "cmpl %%ecx, %%edx;"
            "jb fin%=;"
            "movl %%eax, %%ebx;"
            "movl %%edx, %%eax;"
            "xorl %%edx, %%edx;"
            "divl %%ecx;"
            "movl %%ebx, %%eax;"
            "fin%=: divl %%ecx;"
#endif
            : "=d"(ret)
            : "a"(multiplier), "d"(multiplicand), "c"(modulus)
            : "bx");
    return ret;
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

size_t accelc_NTT(const coeff_t* __restrict src, size_t len,
                  coeff_t* __restrict dst,
                  coeff_t g, coeff_t P) {
    if ((P - 1) % len != 0)
        return 0;

    // dst[n] = sigma_k_(0)_to_(len - 1)_(src[k] * w ^ nk % P)
    // while w = g ^ ((P - 1) / len) % P
    coeff_t w = accelc_powmod(g, (P - 1) / len, P);

    for (size_t n = 0; n < len; n++) {
        coeff_t result = 0;
        for (size_t k = 0; k < len; k++) {
            coeff_t temp = _mulmod_coeff(src[k], accelc_powmod(w, n * k, P), P);
            if (_addcarry_coeff(0, result, temp, &result))
                result = _mod_asm(result, 1, P);
        }
        dst[n] = result;
    }

    return len;
}

size_t accelc_NTT_inverse(const coeff_t* __restrict src, size_t len,
                          coeff_t* __restrict dst,
                          coeff_t g, coeff_t P) {
    if ((P - 1) % len != 0)
        return 0;

    // dst[n] = (len ^ -1 % P) * sigma_k_(0)_to_(len - 1)_(src[k] * (w ^ -1 % P) ^ nk % P)
    // while w = g ^ ((P - 1) / len) % P
    coeff_t w = accelc_powmod(g, (P - 1) / len, P);
    w = accelc_reciprocal_Fermat(w, P);

    coeff_t len_reciprocal = accelc_reciprocal_Fermat(len, P);

    for (size_t n = 0; n < len; n++) {
        coeff_t result = 0;
        for (size_t k = 0; k < len; k++) {
            coeff_t temp = _mulmod_coeff(src[k], accelc_powmod(w, n * k, P), P);
            if (_addcarry_coeff(0, result, temp, &result))
                result = _mod_asm(result, 1, P);
        }
        dst[n] = _mulmod_coeff(len_reciprocal, result, P);
    }

    return len;
}