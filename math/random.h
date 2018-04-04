#pragma once
#include "def.h"

#if defined(__cplusplus)
extern "C" {
#endif

    typedef struct _LCG_PARAM {
        coeff_t A;
        coeff_t Seed;
        coeff_t B;
        coeff_t P;
    } LCG_PARAM;

    coeff_t accelc_LCG_next(LCG_PARAM* param);

#if defined(__cplusplus)
}
#endif