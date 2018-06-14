#include "../arithmetic.h"
#include "../def_asm.h"
#include <memory.h>

uint8_t accelc_uintx_add(coeff_t* __restrict summand, size_t summand_length,
                         const coeff_t* __restrict addend, size_t addend_length,
                         uint8_t carry) {
    for (size_t i = 0; i < addend_length; ++i)
        carry = _addcarry_coeff(carry, summand[i], addend[i], summand + i);
    for (size_t i = addend_length; carry != 0 && i < summand_length; ++i)
        carry = _addcarry_coeff(carry, summand[i], 0, summand + i);

    return carry;
}

uint8_t accelc_uintx_fastadd(coeff_t* summand, size_t summand_length,
                             coeff_t addend,
                             uint8_t carry) {
    carry = _addcarry_coeff(carry, *summand, addend, summand);
    for (size_t i = 1; carry != 0 && i < summand_length; ++i)
        carry = _addcarry_coeff(carry, summand[i], 0, summand + i);
    return carry;
}

uint8_t accelc_uintx_addto(const coeff_t* summand, size_t summand_length,
                           const coeff_t* addend, size_t addend_length,
                           uint8_t carry,
                           coeff_t* __restrict sum) {
    size_t i = 0;
    for (; i < addend_length; ++i)
        carry = _addcarry_coeff(carry, summand[i], addend[i], sum + i);
    for (; carry != 0 && i < summand_length; ++i)
        carry = _addcarry_coeff(carry, summand[i], 0, sum + i);

    memcpy(sum + i, summand + i, (summand_length - i) * sizeof(coeff_t));

    return carry;
}

uint8_t accelc_uintx_fastaddto(const coeff_t* summand, size_t summand_length,
                               coeff_t addend,
                               uint8_t carry,
                               coeff_t* __restrict sum) {

    carry = _addcarry_coeff(carry, *summand, addend, sum);

    size_t i = 1;
    for (; carry != 0 && i < summand_length; ++i)
        carry = _addcarry_coeff(carry, summand[i], 0, sum + i);

    memcpy(sum + i, summand + i, (summand_length - i) * sizeof(coeff_t));

    return carry;
}

uint8_t accelc_uintx_sub(coeff_t* __restrict minuend, size_t minuend_length,
                         const coeff_t* __restrict subtrahend, size_t subtrahend_length,
                         uint8_t borrow) {

    for (size_t i = 0; i < subtrahend_length; ++i)
        borrow = _subborrow_coeff(borrow, minuend[i], subtrahend[i], minuend + i);
    for (size_t i = subtrahend_length; borrow != 0 && i < minuend_length; ++i)
        borrow = _subborrow_coeff(borrow, minuend[i], 0, minuend + i);

    return borrow;
}

uint8_t accelc_uintx_fastsub(coeff_t* __restrict minuend, size_t minuend_length,
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

uint8_t accelc_uintx_fastsubto(const coeff_t* minuend, size_t minuend_length,
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