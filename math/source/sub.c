/*********************************************************************
* Filename:   sub.c
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#include "../arithmetic.h"
#include <x86intrin.h>
#include <memory.h>

#if defined(_M_X64) || defined(__x86_64__)
#define _subborrow_coeff _subborrow_u64
#elif defined(_M_IX86) || defined(__i386__)
#define _subborrow_coeff _subborrow_u32
#endif

uint8_t accelc_uintx_sub(coeff_t* __restrict minuend, size_t minuend_length,
                         const coeff_t* __restrict subtrahend, size_t subtrahend_length,
                         uint8_t borrow) {

    for (size_t i = 0; i < subtrahend_length; ++i)
        borrow = _subborrow_coeff(borrow, minuend[i], subtrahend[i], minuend + i);
    for (size_t i = subtrahend_length; borrow != 0 && i < minuend_length; ++i)
        borrow = _subborrow_coeff(borrow, minuend[i], 0, minuend + i);

    return borrow;
}

uint8_t accelc_uintx_sub_s(coeff_t* __restrict minuend, size_t minuend_length,
                           coeff_t subtrahend,
                           uint8_t borrow) {
    borrow = _subborrow_coeff(borrow, *minuend, subtrahend, minuend);
    for (size_t i = 0; borrow != 0 && i < minuend_length; ++i)
        borrow = _subborrow_coeff(borrow, minuend[i], 0, minuend + i);

    return borrow;
}

uint8_t accelc_uintx_subto(const coeff_t* minuend, size_t minuend_length,
                           const coeff_t* subtrahend, size_t subtrahend_length,
                           uint8_t borrow,
                           coeff_t* __restrict diff) {
    size_t i = 0;
    for (; i < subtrahend_length; ++i)
        borrow = _subborrow_coeff(borrow, minuend[i], subtrahend[i], diff + i);
    for (; borrow != 0 && i < minuend_length; ++i)
        borrow = _subborrow_coeff(borrow, minuend[i], 0, diff + i);

    memcpy(diff + i, minuend + i, (minuend_length - i) * sizeof(coeff_t));

    return borrow;
}

uint8_t accelc_uintx_subto_s(const coeff_t* minuend, size_t minuend_length,
                             coeff_t subtrahend,
                             uint8_t borrow,
                             coeff_t* __restrict diff) {
    borrow = _subborrow_coeff(borrow, *minuend, subtrahend, diff);

    size_t i = 0;
    for (; borrow != 0 && i < minuend_length; ++i)
        borrow = _subborrow_coeff(borrow, minuend[i], 0, diff + i);

    memcpy(diff + i, minuend + i, (minuend_length - i) * sizeof(coeff_t));

    return borrow;
}
