/*********************************************************************
* Filename:   div.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../div.h"
#include "../cmp.h"
#include <memory.h>

#if defined(_M_X64)
#define _BitScanReverse_uintr _BitScanReverse64
#define _addcarry_uintr _addcarry_u64
#define _subborrow_uintr _subborrow_u64
#define _mulx_uintr _mulx_u64
#elif defined(_M_IX86)
#define _BitScanReverse_uintr _BitScanReverse
#define _addcarry_uintr _addcarry_u32
#define _subborrow_uintr _subborrow_u32
#define _mulx_uintr _mulx_u32
#endif

static inline
uintr_t __shiftright_uintr(uintr_t _Low, uintr_t _High, unsigned char _Shift) {
    return (_Low >> _Shift) | (_High << (sizeof(uintr_t) * 8 - _Shift));
}

// eft_length cannot be 0.
// diff1 is nonzero means dividend_length is (eft_length + 1) and divisor_length is eft_length,
// otherwise both dividend_length and divisor_length are eft_length.
static inline uintr_t _div_helper_(uintr_t* __restrict dividend,
                                   const uintr_t* __restrict divisor,
                                   size_t eft_length, int diff1) {
    if (eft_length == 1) {
        if (diff1) {
            uintr_t quotient = MATH_Divmod_Intrinsic(dividend[0], dividend[1],
                                                     divisor[0],
                                                     dividend);
            dividend[1] = 0;
            return quotient;
        } else {
            *dividend = *dividend % *divisor;
            return *dividend / *divisor;
        }
    }

    unsigned long shift;
    _BitScanReverse_uintr(&shift, divisor[eft_length - 1]);
    shift++;

    uintr_t quotient;
    if (shift == sizeof(uintr_t) * 8) {
        quotient = MATH_Div_Intrinsic(dividend[eft_length - 1],
                                      diff1 ? dividend[eft_length] : 0,
                                      divisor[eft_length - 1]);
    } else {
        quotient = MATH_Div_Intrinsic(
            __shiftright_uintr(dividend[eft_length - 2], dividend[eft_length - 1], shift),
            diff1 ? __shiftright_uintr(dividend[eft_length - 1], dividend[eft_length], shift) : dividend[eft_length - 1] >> shift,
            __shiftright_uintr(divisor[eft_length - 2], divisor[eft_length - 1], shift)
        );
    }

    unsigned char borrow = 0;
    uintr_t product_carry = 0;
    uintr_t temp_product[2];
    for (size_t i = 0; i < eft_length; ++i) {
        temp_product[0] = _mulx_uintr(divisor[i], quotient, temp_product + 1);
        product_carry = temp_product[1] + _addcarry_uintr(0, temp_product[0], product_carry, temp_product);
        borrow = _subborrow_uintr(borrow, dividend[i], temp_product[0], dividend + i);
    }

    if (diff1) {
        borrow = _subborrow_uintr(borrow, dividend[eft_length], product_carry, dividend + eft_length);
        product_carry = 0;
    }

    while (borrow != 0 || product_carry != 0) {
        quotient--;
        unsigned char carry = 0;
        for (size_t i = 0; i < eft_length; ++i)
            carry = _addcarry_uintr(carry, dividend[i], divisor[i], dividend + i);
        if (diff1)
            carry = _addcarry_uintr(carry, dividend[eft_length], 0, dividend + eft_length);
        borrow -= carry;
    }

    return quotient;
}

void MATH_Div(uintr_t* __restrict dividend, size_t dividend_length,
              const uintr_t* __restrict divisor, size_t divisor_length,
              uintr_t* __restrict quotient, size_t quotient_length) {

    const uintr_t* cur = dividend + dividend_length - 1;
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
        memset(quotient, 0, quotient_length * sizeof(uintr_t));
        return;
    } else {
        size_t quotient_expect_eft_length = dividend_length - divisor_length + 1;
        memset(quotient + quotient_expect_eft_length,
               0,
               (quotient_length - quotient_expect_eft_length) * sizeof(uintr_t));

        uintr_t* cur_dividend = dividend + (dividend_length - divisor_length);
        uintr_t* cur_quotient = quotient + (dividend_length - divisor_length);
        uintr_t remainder_highest = 0;
        do {
            *cur_quotient = _div_helper_(cur_dividend, divisor, divisor_length, remainder_highest != 0);
            --cur_quotient;
            --cur_dividend;
            remainder_highest = cur_dividend[divisor_length];
        } while (cur_quotient != quotient - 1);
    }
}

void MATH_Mod(uintr_t* __restrict dividend, size_t dividend_length,
              const uintr_t* __restrict divisor, size_t divisor_length) {

    const uintr_t* cur = dividend + dividend_length - 1;
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
        return;
    } else {
        size_t quotient_expect_eft_length = dividend_length - divisor_length + 1;
        uintr_t* cur_dividend = dividend + (dividend_length - divisor_length);
        uintr_t remainder_highest = 0;
        for (size_t i = 0; i < quotient_expect_eft_length; ++i) {
            _div_helper_(cur_dividend, divisor, divisor_length, remainder_highest != 0);
            --cur_dividend;
            remainder_highest = cur_dividend[divisor_length];
        }
    }
}