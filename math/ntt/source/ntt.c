#include "../ntt.h"
#include "../../def_asm.h"
#include "../../num_theory.h"

void accelc_NTT(const coeff_t* __restrict src, size_t len,
                coeff_t* __restrict dst,
                coeff_t g, coeff_t P) {
    // dst[n] = sigma_k_(0)_to_(len - 1)_(src[k] * w ^ nk % P)
    // while w = g ^ ((P - 1) / len) % P
    coeff_t w = accelc_powmod(g, (P - 1) / len, P);
    for (size_t n = 0; n < len; n++) {
        coeff_t sum = 0;
        for (size_t k = 0; k < len; k++) {
            coeff_t temp = _mulmod_coeff(src[k], accelc_powmod(w, n * k, P), P);
            if (_addcarry_coeff(0, sum, temp, &sum))
                sum = _mod_only_coeff(sum, 1, P);
        }
        dst[n] = sum % P;
    }
}

void accelc_iNTT(const coeff_t* __restrict src, size_t len,
                 coeff_t* __restrict dst,
                 coeff_t g, coeff_t P) {
    // dst[n] = (len ^ -1 % P) * sigma_k_(0)_to_(len - 1)_(src[k] * (w ^ -1 % P) ^ nk % P)
    // while w = g ^ ((P - 1) / len) % P
    g = accelc_Reciprocal_Fermat(g, P);
    coeff_t w = accelc_powmod(g, (P - 1) / len, P);
    //w = accelc_Reciprocal_Fermat(w, P); // w = w ^ -1 % P

    coeff_t len_reciprocal = accelc_Reciprocal_Fermat(len, P);

    for (size_t n = 0; n < len; n++) {
        coeff_t sum = 0;
        for (size_t k = 0; k < len; k++) {
            coeff_t temp = _mulmod_coeff(src[k], accelc_powmod(w, n * k, P), P);
            if (_addcarry_coeff(0, sum, temp, &sum))
                sum = _mod_only_coeff(sum, 1, P);
        }
        dst[n] = _mulmod_coeff(len_reciprocal, sum, P);
    }
}
