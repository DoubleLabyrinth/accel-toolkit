#include "../ntt.h"
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

const uint64_t accelc_NTT_2px_prime_64[56] = {
        0x0000000000000000, 0x7fffffffffffffe7, 0x7ffffffffffffefd, 0x7ffffffffffffe89,
        0x7ffffffffffffd21, 0x7ffffffffffffd21, 0x7ffffffffffff8c1, 0x7ffffffffffff081,
        0x7fffffffffffef01, 0x7fffffffffffbc01, 0x7fffffffffffbc01, 0x7ffffffffffe7801,
        0x7ffffffffffbc001, 0x7ffffffffffbc001, 0x7ffffffffffbc001, 0x7fffffffffef0001,
        0x7fffffffffef0001, 0x7fffffffff620001, 0x7ffffffffef00001, 0x7ffffffffef00001,
        0x7ffffffffef00001, 0x7ffffffff9e00001, 0x7ffffffff9000001, 0x7ffffffff9000001,
        0x7ffffffff9000001, 0x7fffffffd2000001, 0x7fffffffc8000001, 0x7fffffffc8000001,
        0x7fffffff60000001, 0x7fffffff60000001, 0x7ffffff900000001, 0x7ffffff900000001,
        0x7ffffff900000001, 0x7fffff9200000001, 0x7fffff5000000001, 0x7fffff5000000001,
        0x7fffff5000000001, 0x7ffffe0000000001, 0x7ffffe0000000001, 0x7ffffe0000000001,
        0x7ffffe0000000001, 0x7ffffe0000000001, 0x7fffe40000000001, 0x7fffe00000000001,
        0x7fffe00000000001, 0x7fffe00000000001, 0x7fe7c00000000001, 0x7fe1000000000001,
        0x7fe1000000000001, 0x7fa8000000000001, 0x7fa8000000000001, 0x7fa8000000000001,
        0x7e40000000000001, 0x7e40000000000001, 0x7e40000000000001, 0x6280000000000001
};

const uint64_t accelc_NTT_2px_primitive_root_64[56] = {
        0x0000000000000000, 0x0000000000000003, 0x0000000000000002, 0x0000000000000003,
        0x0000000000000005, 0x0000000000000005, 0x000000000000001f, 0x000000000000000e,
        0x000000000000000a, 0x0000000000000007, 0x0000000000000007, 0x0000000000000005,
        0x000000000000000d, 0x000000000000000d, 0x000000000000000d, 0x0000000000000005,
        0x0000000000000005, 0x000000000000000b, 0x000000000000000a, 0x000000000000000a,
        0x000000000000000a, 0x000000000000000b, 0x0000000000000003, 0x0000000000000003,
        0x0000000000000003, 0x0000000000000003, 0x000000000000000a, 0x000000000000000a,
        0x0000000000000003, 0x0000000000000003, 0x0000000000000003, 0x0000000000000003,
        0x0000000000000003, 0x000000000000000d, 0x0000000000000005, 0x0000000000000005,
        0x0000000000000005, 0x0000000000000007, 0x0000000000000007, 0x0000000000000007,
        0x0000000000000007, 0x0000000000000007, 0x0000000000000003, 0x0000000000000005,
        0x0000000000000005, 0x0000000000000005, 0x0000000000000003, 0x0000000000000003,
        0x0000000000000003, 0x0000000000000003, 0x0000000000000003, 0x0000000000000003,
        0x0000000000000006, 0x0000000000000006, 0x0000000000000006, 0x0000000000000003
};

const uint32_t accelc_NTT_2px_prime_32[28] = {
        0x00000000, 0x7fffffff, 0x7fffffed, 0x7fffff69,
        0x7fffff61, 0x7fffff61, 0x7ffffe01, 0x7ffffe01,
        0x7ffffe01, 0x7ffffe01, 0x7fffd801, 0x7fffd801,
        0x7ffe9001, 0x7ffe6001, 0x7ffe0001, 0x7ffe0001,
        0x7ffe0001, 0x7ffe0001, 0x7ff80001, 0x7ff80001,
        0x7f000001, 0x7f000001, 0x7f000001, 0x7f000001,
        0x7f000001, 0x7e000001, 0x78000001, 0x78000001
};

const uint32_t accelc_NTT_2px_primitive_root_32[28] = {
        0x00000000, 0x00000007, 0x00000002, 0x00000005,
        0x00000003, 0x00000003, 0x0000000a, 0x0000000a,
        0x0000000a, 0x0000000a, 0x00000003, 0x00000003,
        0x0000000b, 0x00000005, 0x00000005, 0x00000005,
        0x00000005, 0x00000005, 0x00000013, 0x00000013,
        0x00000003, 0x00000003, 0x00000003, 0x00000003,
        0x00000003, 0x00000005, 0x0000001f, 0x0000001f
};

void accelc_NTT(const coeff_t* __restrict src, size_t len,
                coeff_t* __restrict dst,
                coeff_t g, coeff_t P) {
    // dst[n] = sigma_k_(0)_to_(len - 1)_(src[k] * w ^ nk % P)
    // while w = g ^ ((P - 1) / len) % P
    coeff_t w = accelc_powmod(g, (P - 1) / len, P);
    for (size_t n = 0; n < len; n++) {
        coeff_t sum = 0;
        for (size_t k = 0; k < len; k++) {
            coeff_t temp = _mulmod_coeff(src[k], accelc_powmod(w, n * k, P), P);
            uint8_t carry = _addcarry_coeff(0, sum, temp, &sum);
            sum = _mod_asm(sum, carry, P);
        }
        dst[n] = sum;
    }
}

void accelc_iNTT(const coeff_t* __restrict src, size_t len,
                 coeff_t* __restrict dst,
                 coeff_t g, coeff_t P) {
    // dst[n] = (len ^ -1 % P) * sigma_k_(0)_to_(len - 1)_(src[k] * (w ^ -1 % P) ^ nk % P)
    // while w = g ^ ((P - 1) / len) % P
    coeff_t w = accelc_powmod(g, (P - 1) / len, P);
    w = accelc_Reciprocal_Fermat(w, P); // w = w ^ -1 % P

    coeff_t len_reciprocal = accelc_Reciprocal_Fermat(len, P);

    for (size_t n = 0; n < len; n++) {
        coeff_t result = 0;
        for (size_t k = 0; k < len; k++) {
            coeff_t temp = _mulmod_coeff(src[k], accelc_powmod(w, n * k, P), P);
            if (_addcarry_coeff(0, result, temp, &result))
                result = _mod_asm(result, 1, P);
        }
        dst[n] = _mulmod_coeff(len_reciprocal, result, P);
    }
}

static void accelc_FNTT_base2_impl(const coeff_t* __restrict src, size_t len,
                                   coeff_t* __restrict dst,
                                   coeff_t g, coeff_t P, size_t gap) {
    if (len == 2) {
        coeff_t temp[2];
        uint8_t carry;

        carry = _addcarry_coeff(0, src[0], src[gap], temp);
        temp[0] = _mod_asm(temp[0], carry, P);
        carry = _addcarry_coeff(0, src[0], P - src[gap], temp + 1);
        temp[1] = _mod_asm(temp[1], carry, P);

        dst[0] = temp[0];
        dst[gap] = temp[1];
    } else {
        accelc_FNTT_base2_impl(src, len / 2, dst, g, P, gap * 2);
        accelc_FNTT_base2_impl(src + gap, len / 2, dst + gap, g, P, gap * 2);

        coeff_t* dst_even = dst;
        coeff_t* dst_odd = dst + gap;
        coeff_t w = accelc_powmod(g, (P - 1) / len, P);
        for (size_t i = 0, pair = len / 2; i < pair; ++i) {
            coeff_t wk = accelc_powmod(w, i, P);
            wk = _mulmod_coeff(wk, dst_odd[i * 2 * gap], P);

            coeff_t temp[2];
            uint8_t carry;

            carry = _addcarry_coeff(0, dst_even[i * 2 * gap], wk, temp);
            temp[0] = _mod_asm(temp[0], carry, P);
            carry = _addcarry_coeff(0, dst_even[i * 2 * gap], P - wk, temp + 1);
            temp[1] = _mod_asm(temp[1], carry, P);

            dst_even[i * 2 * gap] = temp[0];
            dst_odd[i * 2 * gap] = temp[1];
        }
    }
}

static void accelc_iFNTT_base2_impl(const coeff_t* __restrict src, size_t len,
                                    coeff_t* __restrict dst,
                                    coeff_t g, coeff_t P, size_t gap) {
    if (len == 2) {
        coeff_t temp[2];
        uint8_t carry;

        carry = _addcarry_coeff(0, src[0], src[gap], temp);
        temp[0] = _mod_asm(temp[0], carry, P);
        carry = _addcarry_coeff(0, src[0], P - src[gap], temp + 1);
        temp[1] = _mod_asm(temp[1], carry, P);

        dst[0] = temp[0];
        dst[gap] = temp[1];
    } else {
        accelc_FNTT_base2_impl(src, len / 2, dst, g, P, gap * 2);
        accelc_FNTT_base2_impl(src + gap, len / 2, dst + gap, g, P, gap * 2);

        coeff_t* dst_even = dst;
        coeff_t* dst_odd = dst + gap;
        coeff_t w = accelc_powmod(g, (P - 1) / len, P);
        for (size_t i = 0, pair = len / 2; i < pair; ++i) {
            coeff_t wk = accelc_powmod(w, i, P);
            wk = _mulmod_coeff(wk, dst_odd[i * 2 * gap], P);

            coeff_t temp[2];
            uint8_t carry;

            carry = _addcarry_coeff(0, dst_even[i * 2 * gap], wk, temp);
            temp[0] = _mod_asm(temp[0], carry, P);
            carry = _addcarry_coeff(0, dst_even[i * 2 * gap], P - wk, temp + 1);
            temp[1] = _mod_asm(temp[1], carry, P);

            dst_even[i * 2 * gap] = temp[0];
            dst_odd[i * 2 * gap] = temp[1];
        }
    }
}

void accelc_FNTT_base2(const coeff_t* __restrict src, size_t len,
                       coeff_t* __restrict dst,
                       coeff_t g, coeff_t P) {
    accelc_FNTT_base2_impl(src, len, dst, g, P, 1);
}

void accelc_iFNTT_base2(const coeff_t* __restrict src, size_t len,
                        coeff_t* __restrict dst,
                        coeff_t g, coeff_t P) {
    g = accelc_Reciprocal_Fermat(g, P);
    accelc_FNTT_base2_impl(src, len, dst, g, P, 1);

    coeff_t len_Reciprocal = accelc_Reciprocal_Fermat(len, P);
    for (size_t i = 0; i < len; ++i)
        dst[i] = _mulmod_coeff(len_Reciprocal, dst[i], P);
}