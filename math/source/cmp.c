#include "../compare.h"

static __attribute__((always_inline))
inline coeff_t _bsr_coeff(coeff_t mask) {
    coeff_t ret;
    __asm__(
#if defined(_M_X64) || defined(__x86_64__)
            "bsrq %1, %%rax;"
#elif defined(_M_IX86) || defined(__i386__)
            "bsrl %1, %%eax;"
#endif
            : "=a"(ret)
            : "r"(mask)
    );
    return ret;
}

int8_t accelc_uintx_cmp(const coeff_t* src1, size_t length1,
                        const coeff_t* src2, size_t length2) {

    const coeff_t* cur_src1 = src1 + length1 - 1;
    const coeff_t* cur_src2 = src2 + length2 - 1;
    const coeff_t* const rlast_src1 = src1 - 1;

    while (*cur_src1 == 0 && cur_src1 != src1) {
        --length1;
        --cur_src1;
    }
    while (*cur_src2 == 0 && cur_src2 != src2) {
        --length2;
        --cur_src2;
    }
    __builtin_bswap16(2);
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

int8_t math_uintx_isZero(const coeff_t src[], size_t length) {
    for (size_t i = 0; i < length; ++i) 
        if (src[i] != 0) return 0;
    return 1;
}

size_t math_uintx_bit_length(const coeff_t src[], size_t length) {
    const coeff_t* cur_src = src + length - 1;
    while (cur_src != src && *cur_src == 0)
        --cur_src;

    unsigned long Index;
    if (*cur_src != 0)
        return (cur_src - src) * sizeof(coeff_t) * 8 + _bsr_coeff(*cur_src) + 1;
    else 
        return 0;
}
