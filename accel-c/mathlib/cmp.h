/*********************************************************************
* Filename:   cmp.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include "def.h"

#if defined(__cplusplus)
extern "C" {
#endif

    // Compare src1 and src2 which are both unsigned integers. 
    // Return Value:
    //       0 if src1 = src2
    //       1 if src1 > src2
    //      -1 if src1 < src2
    // ASSERT:
    // 1. length1 > 0 
    // 2. length2 > 0
    int8_t math_uintx_cmp(const coeff_t src1[], size_t length1,
                          const coeff_t src2[], size_t length2);

    // Check if src is zero. 
    // Return Value:
    //       0 if src is not zero
    //       1 if src is zero
    // ASSERT:
    // 1. length > 0 
    int8_t math_uintx_isZero(const coeff_t src[], size_t length);

    // Get the bit length of src. 
    // ASSERT:
    // 1. length > 0 
    size_t math_uintx_bit_length(const coeff_t src[], size_t length);

#if defined(__cplusplus)
}
#endif