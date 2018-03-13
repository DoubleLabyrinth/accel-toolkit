#pragma once
#include "def.h"

#if defined(__cplusplus)
extern "C" {
#endif

    // return Base ^ Exponent % Modulus
    coeff_t math_uintx_powmod_s(coeff_t Base, coeff_t Exponent, coeff_t Modulus);

    // ASSERT:
    // 1. Base and Modulus have the same length, specified by Length
    // 2. Length must a power of 2
    // 3. ExponentLength must be at least 1 (and not need to be a power of 2)
    void math_uintx_powmod(coeff_t* __restrict Base, size_t Length,
                           const coeff_t* Exponent, size_t ExponentLength,
                           const coeff_t* Modulus);

#if defined(__cplusplus)
}
#endif