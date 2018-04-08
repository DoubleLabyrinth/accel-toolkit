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

uint8_t accelc_uintx_add_s(coeff_t* summand, size_t summand_length,
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

uint8_t accelc_uintx_addto_s(const coeff_t* summand, size_t summand_length,
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