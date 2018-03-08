#pragma once
#include "def.h"
#include <intrin.h>
#include <memory.h>

#if defined(_M_X64)
#define _subborrow_uintr _subborrow_u64
#elif defined(_M_IX86)
#define _subborrow_uintr _subborrow_u32
#endif

#if defined(__cplusplus)
extern "C" {
#endif

    // Equivalent to "minuend -= (subtrahend + borrow);"
    // ASSERT:
    // 1. minuend_length >= subtrahend_length
    // 2. subtrahend_length > 0
    inline uint8_t MATH_SubAssign(uintr_t* __restrict minuend, size_t minuend_length,
                                  const uintr_t* __restrict subtrahend, size_t subtrahend_length,
                                  uint8_t borrow) {

        for (size_t i = 0; i < subtrahend_length; ++i)
            borrow = _subborrow_uintr(borrow, minuend[i], subtrahend[i], minuend + i);
        for (size_t i = subtrahend_length; borrow != 0 && i < minuend_length; ++i)
            borrow = _subborrow_uintr(borrow, minuend[i], 0, minuend + i);

        return borrow;
    }

    // Equivalent to "minuend -= (subtrahend + borrow);"
    // ASSERT:
    // 1. minuend_length > 0
    inline uint8_t MATH_SubAssign_s(uintr_t* __restrict minuend, size_t minuend_length,
                                    uintr_t subtrahend,
                                    uint8_t borrow) {
        borrow = _subborrow_uintr(borrow, *minuend, subtrahend, minuend);
        for (size_t i = 0; borrow != 0 && i < minuend_length; ++i)
            borrow = _subborrow_uintr(borrow, minuend[i], 0, minuend + i);

        return borrow;
    }

    // Equivalent to "diff = minuend - (subtrahend + borrow);"
    // ASSERT:
    // 1. minuend_length >= subtrahend_length
    // 2. subtrahend_length > 0
    // 3. diff length >= minuend_length
    inline uint8_t MATH_SubTo(const uintr_t* minuend, size_t minuend_length,
                              const uintr_t* subtrahend, size_t subtrahend_length,
                              uint8_t borrow,
                              uintr_t* __restrict diff) {
        size_t i = 0;
        for (; i < subtrahend_length; ++i)
            borrow = _subborrow_uintr(borrow, minuend[i], subtrahend[i], diff + i);
        for (; borrow != 0 && i < minuend_length; ++i)
            borrow = _subborrow_uintr(borrow, minuend[i], 0, diff + i);
        memcpy(diff + i, minuend + i, (minuend_length - i) * sizeof(uintr_t));
        return borrow;
    }

    // Equivalent to "diff = minuend - (subtrahend + borrow);"
    // ASSERT:
    // 1. minuend_length > 0
    // 3. diff length >= minuend_length
    inline uint8_t MATH_SubTo_s(const uintr_t* minuend, size_t minuend_length,
                                uintr_t subtrahend,
                                uint8_t borrow,
                                uintr_t* __restrict diff) {
        borrow = _subborrow_uintr(borrow, *minuend, subtrahend, diff);

        size_t i = 0;
        for (; borrow != 0 && i < minuend_length; ++i)
            borrow = _subborrow_uintr(borrow, minuend[i], 0, diff + i);

        memcpy(diff + i, minuend + i, (minuend_length - i) * sizeof(uintr_t));
        return borrow;
    }

#if defined(__cplusplus)
}
#endif

#undef _subborrow_uintr