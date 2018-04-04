#include "../arithmetic.h"
#include <memory.h>

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#endif

#if defined(_M_X64) || defined(__x86_64__)
#define _addcarry_coeff _addcarry_u64
#define _subborrow_coeff _subborrow_u64
#elif defined(_M_IX86) || defined(__i386__)
#define _addcarry_coeff _addcarry_u32
#define _subborrow_coeff _subborrow_u32
#endif

static __attribute__((always_inline))
inline coeff_t _bsr_coeff(coeff_t mask) {
    coeff_t ret;
    __asm__(
#if defined(_M_X64) || defined(__x86_64__)
            "bsrq %1, %%rax;"
#elif defined(_M_IX86) || defined(__i386__)
            "bsrl %1, %%eax;"
#endif
            : "=a"(ret)
            : "r"(mask)
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
            : "a"(multiplier), "r"(multiplicand)
    );
    return product_l;
}

static __attribute__((always_inline))
inline coeff_t _divmod_asm(coeff_t dividend_l, coeff_t dividend_h,
                           coeff_t divisor,
                           coeff_t* remainder) {
    coeff_t quotient;
    __asm__(
#if defined(_M_X64) || defined(__x86_64__)
            "divq %4;"
#elif defined(_M_IX86) || defined(__i386__)
            "divl %4;"
#endif
            : "=a"(quotient), "=d"(*remainder)
            : "a"(dividend_l), "d"(dividend_h), "r"(divisor)
    );
    return quotient;
}

static __attribute__((always_inline))
inline coeff_t _div_asm(coeff_t dividend_l, coeff_t dividend_h,
                        coeff_t divisor) {
    coeff_t quotient;
    __asm__(
#if defined(_M_X64) || defined(__x86_64__)
            "divq %3;"
#elif defined(_M_IX86) || defined(__i386__)
            "divl %3;"
#endif
            : "=a"(quotient)
            : "a"(dividend_l), "d"(dividend_h), "r"(divisor)
            );
    return quotient;
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
            : "a"(dividend_l), "d"(dividend_h), "r"(divisor)
            );
    return remainder;
}

static __attribute__((always_inline))
inline coeff_t __shiftR_2coeff(coeff_t _Low, coeff_t _High, unsigned char _Shift) {
    coeff_t ret;
    __asm__(
#if defined(_M_X64) || defined(__x86_64__)
            "shrdq %%cl, %%rdx, %%rax;"
#elif defined(_M_IX86) || defined(__i386__)
            "shrdl %%cl, %%edx, %%eax;"
#endif
            : "=a"(ret)
            : "a"(_Low), "d"(_High), "c"(_Shift)
    );
    return ret;
}


// eft_length cannot be 0.
// diff1 is nonzero means dividend_length is (eft_length + 1) and divisor_length is eft_length,
// otherwise both dividend_length and divisor_length are eft_length.
static inline coeff_t _div_helper_(coeff_t* __restrict dividend,
                                   const coeff_t* __restrict divisor,
                                   size_t eft_length, int diff1) {
    if (eft_length == 1) {
        coeff_t quotient;
        if (diff1) {
            quotient = _divmod_asm(dividend[0], dividend[1],
                                   divisor[0],
                                   dividend);
            dividend[1] = 0;
        } else {
            quotient = *dividend / *divisor;
            *dividend = *dividend % *divisor;
        }
        return quotient;
    }

    uint8_t shift = (uint8_t)_bsr_coeff(divisor[eft_length - 1]);
    shift++;

    coeff_t quotient;
    if (shift == sizeof(coeff_t) * 8) {
        quotient = _div_asm(dividend[eft_length - 1],
                            diff1 ? dividend[eft_length] : 0,
                            divisor[eft_length - 1]);
    } else {
        quotient = _div_asm(
                __shiftR_2coeff(dividend[eft_length - 2], dividend[eft_length - 1], shift),
                diff1 ? __shiftR_2coeff(dividend[eft_length - 1], dividend[eft_length], shift) :
                dividend[eft_length - 1] >> shift,
                __shiftR_2coeff(divisor[eft_length - 2], divisor[eft_length - 1], shift)
        );
    }

    unsigned char borrow = 0;
    coeff_t product_carry = 0;
    coeff_t temp_product[2];
    for (size_t i = 0; i < eft_length; ++i) {
        temp_product[0] = _mul_coeff(divisor[i], quotient, temp_product + 1);
        product_carry = temp_product[1] + _addcarry_coeff(0, temp_product[0], product_carry, temp_product);
        borrow = _subborrow_coeff(borrow, dividend[i], temp_product[0], dividend + i);
    }

    if (diff1) {
        borrow = _subborrow_coeff(borrow, dividend[eft_length], product_carry, dividend + eft_length);
        product_carry = 0;
    }

    while (borrow != 0 || product_carry != 0) {
        quotient--;
        unsigned char carry = 0;
        for (size_t i = 0; i < eft_length; ++i)
            carry = _addcarry_coeff(carry, dividend[i], divisor[i], dividend + i);
        if (diff1)
            carry = _addcarry_coeff(carry, dividend[eft_length], 0, dividend + eft_length);
        borrow -= carry;
    }

    return quotient;
}

coeff_t accelc_uintx_div_s(coeff_t* dividend, size_t dividend_length,
                           coeff_t divisor) {
    coeff_t* cur = dividend + dividend_length - 1;
    coeff_t remainder = *cur % divisor;
    *cur = *cur / divisor;
    while (cur != dividend) {
        --cur;
        *cur = _divmod_asm(*cur, remainder, divisor, &remainder);
    }
    return remainder;
}

void accelc_uintx_divmod(coeff_t* __restrict dividend, size_t dividend_length,
                         const coeff_t* __restrict divisor, size_t divisor_length,
                         coeff_t* __restrict quotient, size_t quotient_length) {

    const coeff_t* cur = dividend + dividend_length - 1;
    while (*cur == 0 && cur != dividend) {
        --dividend_length;
        --cur;
    }

    cur = divisor + divisor_length - 1;
    while (*cur == 0 && cur != divisor) {
        --divisor_length;
        --cur;
    }

    if (dividend_length < divisor_length) {
        memset(quotient, 0, quotient_length * sizeof(coeff_t));
        return;
    } else {
        size_t quotient_expect_eft_length = dividend_length - divisor_length + 1;
        memset(quotient + quotient_expect_eft_length,
               0,
               (quotient_length - quotient_expect_eft_length) * sizeof(coeff_t));

        coeff_t* cur_dividend = dividend + (dividend_length - divisor_length);
        coeff_t* cur_quotient = quotient + (dividend_length - divisor_length);
        coeff_t remainder_highest = 0;
        do {
            *cur_quotient = _div_helper_(cur_dividend, divisor, divisor_length, remainder_highest != 0);
            --cur_quotient;
            --cur_dividend;
            remainder_highest = cur_dividend[divisor_length];
        } while (cur_quotient != quotient - 1);
    }
}

size_t accelc_uintx_mod(coeff_t* __restrict dividend, size_t dividend_length,
                        const coeff_t* __restrict divisor, size_t divisor_length) {

    const coeff_t* cur = dividend + dividend_length - 1;
    while (*cur == 0 && cur != dividend) {
        --dividend_length;
        --cur;
    }

    cur = divisor + divisor_length - 1;
    while (*cur == 0 && cur != divisor) {
        --divisor_length;
        --cur;
    }

    if (dividend_length < divisor_length) {
        return dividend_length;
    } else {
        size_t quotient_expect_eft_length = dividend_length - divisor_length + 1;
        coeff_t* cur_dividend = dividend + (dividend_length - divisor_length);
        coeff_t remainder_highest = 0;
        for (size_t i = 0; i < quotient_expect_eft_length; ++i) {
            _div_helper_(cur_dividend, divisor, divisor_length, remainder_highest != 0);
            --cur_dividend;
            remainder_highest = cur_dividend[divisor_length];
        }

        cur_dividend += divisor_length;
        while (remainder_highest == 0 && cur_dividend != dividend) {
            --cur_dividend;
            remainder_highest = *cur_dividend;
        }
        return (cur_dividend - dividend) + 1;
    }
}