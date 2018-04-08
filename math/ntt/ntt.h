#pragma once
#include "../def.h"

#if defined(__cplusplus)
extern "C" {
#endif

    void accelc_NTT(const coeff_t* __restrict src, size_t len,
                    coeff_t* __restrict dst,
                    coeff_t g, coeff_t P);

    void accelc_iNTT(const coeff_t* __restrict src, size_t len,
                     coeff_t* __restrict dst,
                     coeff_t g, coeff_t P);

    int accelc_FNTT_base2(const coeff_t* __restrict src, size_t len,
                          coeff_t* __restrict dst,
                          coeff_t g, coeff_t P);

    int accelc_iFNTT_base2(const coeff_t* __restrict src, size_t len,
                           coeff_t* __restrict dst,
                           coeff_t g, coeff_t P);

    int accelc_FNTT_base2_extend(const coeff_t* __restrict src, size_t len,
                                 coeff_t* __restrict dst,
                                 coeff_t g, coeff_t P);

#if defined(__cplusplus)
}
#endif