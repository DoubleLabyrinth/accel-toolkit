#pragma once
#include "def.h"
#include <intrin.h>
#include <memory.h>

#if defined(_M_X64)
#define _addcarry_uintr _addcarry_u64
#elif defined(_M_IX86)
#define _addcarry_uintr _addcarry_u32
#endif

#if defined(__cplusplus)
extern "C" {
#endif

    // Equivalent to "summand += addend + carry;"
    // ASSERT:
    // 1. summand_length >= addend_length
    // 2. addend_length > 0
    inline uint8_t MATH_AddAssign(uintr_t* __restrict summand, size_t summand_length,
                                  const uintr_t* __restrict addend, size_t addend_length,
                                  uint8_t carry) {
        for (size_t i = 0; i < addend_length; ++i)
            carry = _addcarry_uintr(carry, summand[i], addend[i], summand + i);
        for (size_t i = addend_length; carry != 0 && i < summand_length; ++i)
            carry = _addcarry_uintr(carry, summand[i], 0, summand + i);

        return carry;
    }

    // Equivalent to "summand += addend + carry;"
    // ASSERT:
    // 1. summand_length > 0
    inline uint8_t MATH_AddAssign_s(uintr_t* __restrict summand, size_t summand_length, 
                                    uintr_t addend, 
                                    uint8_t carry) {
        carry = _addcarry_uintr(carry, *summand, addend, summand);
        for (size_t i = 1; carry != 0 && i < summand_length; ++i)
            carry = _addcarry_uintr(carry, summand[i], 0, summand + i);
        return carry;
    }

    // Equivalent to "sum = summand + addend + carry;"
    // ASSERT: 
    // 1. summand_length >= addend_length
    // 2. addend_length > 0
    // 3. sum length >= summand_length
    inline uint8_t MATH_AddTo(const uintr_t* summand, size_t summand_length,
                              const uintr_t* addend, size_t addend_length,
                              uint8_t carry,
                              uintr_t* __restrict sum) {
        size_t i = 0;
        for (; i < addend_length; ++i)
            carry = _addcarry_uintr(carry, summand[i], addend[i], sum + i);
        for (; carry != 0 && i < summand_length; ++i)
            carry = _addcarry_uintr(carry, summand[i], 0, sum + i);
        memcpy(sum + i, summand + i, (summand_length - i) * sizeof(uintr_t));
        return carry;
    }

    // Equivalent to "sum = summand + addend + carry;"
    // ASSERT: 
    // 1. summand_length > 0
    // 3. sum length >= summand_length
    inline uint8_t MATH_AddTo_s(const uintr_t* summand, size_t summand_length,
                                uintr_t addend,
                                uint8_t carry,
                                uintr_t* __restrict sum) {

        carry = _addcarry_uintr(carry, *summand, addend, sum);

        size_t i = 1;
        for (; carry != 0 && i < summand_length; ++i)
            carry = _addcarry_uintr(carry, summand[i], 0, sum + i);

        memcpy(sum + i, summand + i, (summand_length - i) * sizeof(uintr_t));
        return carry;
    }

#if defined(__cplusplus)
}
#endif

#undef _addcarry_uintr