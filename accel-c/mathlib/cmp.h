/*********************************************************************
* Filename:   cmp.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include "def.h"
#include <intrin.h>

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
    inline int8_t MATH_UnsignedCompare(const uintptr_t src1[], size_t length1,
                                       const uintptr_t src2[], size_t length2) {

        const uintptr_t* cur_src1 = src1 + length1 - 1;
        const uintptr_t* cur_src2 = src2 + length2 - 1;
        const uintptr_t* const rlast_src1 = src1 - 1;

        while (*cur_src1 == 0 && cur_src1 != src1) {
            --length1;
            --cur_src1;
        }
        while (*cur_src2 == 0 && cur_src2 != src2) {
            --length2;
            --cur_src2;
        }

        if (length1 == length2) {
            while (*cur_src1 == *cur_src2 && cur_src1 != src1) {
                --cur_src1;
                --cur_src2;
            }

            if (*cur_src1 > *cur_src2)
                return 1;
            else if (*cur_src1 < *cur_src2)
                return -1;
            else
                return 0;
        } else {
            return length1 > length2 ? 1 : -1;
        }
    }

#if defined(__cplusplus)
}
#endif