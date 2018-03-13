/*********************************************************************
* Filename:   gcd.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include "def.h"

#if defined(__cplusplus)
extern "C" {
#endif

    // Get the greatest common divisor of a and b
    // The return value is the GCD
    coeff_t math_uintx_gcds(coeff_t a, coeff_t b);

    // Get the greatest common divisor of a and b
    // The GCD value stores in a when return
    // ASSERT:
    // 1. a_length >= b_length
    // 2. b_length > 0
    void math_uintx_gcd(coeff_t* __restrict a, size_t a_length,
                        coeff_t* __restrict b, size_t b_length);
#if defined(__cplusplus)
}
#endif