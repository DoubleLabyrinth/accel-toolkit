#pragma once
#include "def.h"

#if defined(__cplusplus)
extern "C" {
#endif

    // Get the greatest common divisor of a and b
    // The return value is the GCD of a and b
    coeff_t accelc_GCD(coeff_t a, coeff_t b);

    // Get the Greatest Common Divisor of a and b
    // If return value if 1, the GCD value is stored in b and a must be 0
    // If return value if 0, the GCD value is stored in a and b must be 0
    int accelc_uintx_GCD(coeff_t* __restrict a, size_t a_length,
                         coeff_t* __restrict b, size_t b_length);

    // return Base ^ Exponent % Modulus
    coeff_t accelc_powmod(coeff_t Base, coeff_t Exponent, coeff_t Modulus);

    // ASSERT:
    // 1. Base and Modulus have the same length, specified by Length
    // 2. Length must a power of 2
    // 3. ExponentLength must be at least 1 (and not need to be a power of 2)
    void accelc_uintx_powmod(coeff_t* __restrict Base, size_t Length,
                             const coeff_t* Exponent, size_t ExponentLength,
                             const coeff_t* Modulus);

    coeff_t accelc_Reciprocal_Fermat(coeff_t a, coeff_t P);

    int accelc_MillerRabinTest(coeff_t n, size_t count);

    int accelc_uintx_MillerRabinTest(const coeff_t* src, size_t len, size_t count);





#if defined(__cplusplus)
}
#endif