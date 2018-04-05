#pragma once
#include "def.h"

#if defined(__cplusplus)
extern "C" {
#endif

    void accelc_NTT(const coeff_t* __restrict src, size_t len,
                    coeff_t* __restrict dst,
                    coeff_t g, coeff_t P);

    void accelc_NTT_inv(const coeff_t* __restrict src, size_t len,
                        coeff_t* __restrict dst,
                        coeff_t g, coeff_t P);

    extern const uint64_t accelc_NTT_2px_prime_64[56];
    extern const uint64_t accelc_NTT_2px_primitive_root_64[56];
    extern const uint32_t accelc_NTT_2px_prime_32[28];
    extern const uint32_t accelc_NTT_2px_primitive_root_32[28];

    void accelc_FNTT_base2(const coeff_t* __restrict src, size_t len,
                           coeff_t* __restrict dst,
                           coeff_t g, coeff_t P);

#if defined(__cplusplus)
}
#endif