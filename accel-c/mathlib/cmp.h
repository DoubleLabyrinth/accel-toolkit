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
    int8_t math_uintx_cmp(const uintptr_t src1[], size_t length1,
                          const uintptr_t src2[], size_t length2);

#if defined(__cplusplus)
}
#endif