#pragma once
#include "def.h"

#if defined(__cplusplus)
extern "C" {
#endif

    // Get the greatest common divisor of a and b
    // The return value is the GCD
    coeff_t accelc_gcd(coeff_t a, coeff_t b);

    // Get the greatest common divisor of a and b
    // The GCD value stores in a when return
    // ASSERT:
    // 1. a_length >= b_length
    // 2. b_length > 0
    void accelc_uintx_gcd(coeff_t* __restrict a, size_t a_length,
                          coeff_t* __restrict b, size_t b_length);

    // return Base ^ Exponent % Modulus
    coeff_t accelc_powmod(coeff_t Base, coeff_t Exponent, coeff_t Modulus);
    coeff_t accelc_numtheory_reciprocal(coeff_t a, coeff_t P);

    // ASSERT:
    // 1. Base and Modulus have the same length, specified by Length
    // 2. Length must a power of 2
    // 3. ExponentLength must be at least 1 (and not need to be a power of 2)
    void accelc_uintx_powmod(coeff_t* __restrict Base, size_t Length,
                             const coeff_t* Exponent, size_t ExponentLength,
                             const coeff_t* Modulus);



    size_t accelc_NTT(const coeff_t* __restrict src, size_t len,
                      coeff_t* __restrict dst,
                      coeff_t g, coeff_t P);

    size_t accelc_NTT_inverse(const coeff_t* __restrict src, size_t len,
                              coeff_t* __restrict dst,
                              coeff_t g, coeff_t P);



#if defined(__cplusplus)
}
#endif