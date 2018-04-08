#include "../ntt.h"
#include "../../def_asm.h"
#include "../../num_theory.h"
#include <memory.h>

static __attribute__((always_inline))
inline coeff_t _addmod_coeff(coeff_t a, coeff_t b, coeff_t P) {
    coeff_t ret;
    __asm__("add " __D__ ", " __A__ ";"
            "jnc _addmod_coeff_cmp%=;"
            "sub " __C__ ", " __A__ ";"
            "jmp _addmod_coeff_fin%=;"
            "_addmod_coeff_cmp%=: cmp " __C__ ", " __A__ ";"
            "jb _addmod_coeff_fin%=;"
            "sub " __C__ ", " __A__ ";"
            "_addmod_coeff_fin%=:"
            : "=a"(ret)
            : "a"(a), "d"(b), "c"(P));
    return ret;
}

static __attribute__((always_inline))
inline coeff_t _submod_coeff(coeff_t a, coeff_t b, coeff_t P) {
    coeff_t ret;
    __asm__("sub " __D__ ", " __A__ ";"
            "jnc _submod_coeff_fin%=;"
            "add " __C__ ", " __A__ ";"
            "_submod_coeff_fin%=:"
    : "=a"(ret)
    : "a"(a), "d"(b), "c"(P));
    return ret;
}

static int _FNTT_base2_impl(const coeff_t* __restrict src, size_t len,
                            coeff_t* __restrict dst,
                            coeff_t w, coeff_t P, size_t gap) {
    if (len == 2) {
        if (gap == 1) {
            dst[0] = _addmod_coeff(src[0], src[gap], P);
            dst[1] = _submod_coeff(src[0], src[gap], P);
        } else {
            dst[0] = _addmod_coeff(src[0], src[gap], P);
            dst[2] = _submod_coeff(src[0], src[gap], P);
        }
    } else {
        coeff_t* buffer = malloc(sizeof(coeff_t) * len);
        if (buffer == 0)
            return 0;

        coeff_t* dst_even = buffer;
        coeff_t* dst_odd = buffer + 1;

        coeff_t next_w = _mulmod_coeff(w, w, P);
        if (1 != _FNTT_base2_impl(src, len / 2,
                                  dst_even,
                                  next_w,
                                  P,
                                  gap * 2)) {
            free(buffer);
            return 0;
        }
        if (1 != _FNTT_base2_impl(src + gap, len / 2,
                                  dst_odd,
                                  next_w,
                                  P,
                                  gap * 2)) {
            free(buffer);
            return 0;
        }

        if (gap == 1) {
            for (size_t k = 0, pair = len / 2; k < pair; ++k) {
                coeff_t left_part = dst_even[2 * k];
                coeff_t right_part = _mulmod_coeff(accelc_powmod(w, k, P), dst_odd[2 * k], P);
                dst[k] = _addmod_coeff(left_part, right_part, P);
                dst[k + pair] = _submod_coeff(left_part, right_part, P);
            }
        } else {
            for (size_t k = 0, pair = len / 2; k < pair; ++k) {
                coeff_t left_part = dst_even[2 * k];
                coeff_t right_part = _mulmod_coeff(accelc_powmod(w, k, P), dst_odd[2 * k], P);
                dst[2 * k] = _addmod_coeff(left_part, right_part, P);
                dst[2 * (k + pair)] = _submod_coeff(left_part, right_part, P);
            }
        }

        free(buffer);
    }
    return 1;
}

static int _FNTT_base2_extend_impl(const coeff_t* __restrict src, size_t len,
                                  coeff_t* __restrict dst,
                                  coeff_t w, coeff_t P, size_t gap) {
    if (len == 2) {
        if (gap == 1) {
            coeff_t right_part = _mulmod_coeff(src[gap], w, P);
            dst[0] = _addmod_coeff(src[0], src[gap], P);
            dst[2] = _submod_coeff(src[0], src[gap], P);
            dst[1] = _addmod_coeff(src[0], right_part, P);
            dst[3] = _submod_coeff(src[0], right_part, P);
        } else {
            coeff_t right_part = _mulmod_coeff(src[gap], w, P);
            dst[0] = _addmod_coeff(src[0], src[gap], P);
            dst[4] = _submod_coeff(src[0], src[gap], P);
            dst[2] = _addmod_coeff(src[0], right_part, P);
            dst[6] = _submod_coeff(src[0], right_part, P);
        }
    } else {
        coeff_t* buffer = malloc(sizeof(coeff_t) * 2 * len);
        if (buffer == 0)
            return 0;

        coeff_t* dst_even = buffer;
        coeff_t* dst_odd = buffer + 1;

        coeff_t next_w = _mulmod_coeff(w, w, P);
        if (1 != _FNTT_base2_extend_impl(src, len / 2,
                                         dst_even,
                                         next_w,
                                         P,
                                         gap * 2)) {
            free(buffer);
            return 0;
        }
        if (1 != _FNTT_base2_extend_impl(src + gap, len / 2,
                                         dst_odd,
                                         next_w,
                                         P,
                                         gap * 2)) {
            free(buffer);
            return 0;
        }

        if (gap == 1) {
            for (size_t k = 0, pair = len; k < pair; ++k) {
                coeff_t left_part = dst_even[2 * k];
                coeff_t right_part = _mulmod_coeff(accelc_powmod(w, k, P), dst_odd[2 * k], P);
                dst[k] = _addmod_coeff(left_part, right_part, P);
                dst[k + pair] = _submod_coeff(left_part, right_part, P);
            }
        } else {
            for (size_t k = 0, pair = len; k < pair; ++k) {
                coeff_t left_part = dst_even[2 * k];
                coeff_t right_part = _mulmod_coeff(accelc_powmod(w, k, P), dst_odd[2 * k], P);
                dst[2 * k] = _addmod_coeff(left_part, right_part, P);
                dst[2 * (k + pair)] = _submod_coeff(left_part, right_part, P);
            }
        }

        free(buffer);
    }
    return 1;
}

int accelc_FNTT_base2(const coeff_t* __restrict src, size_t len,
                      coeff_t* __restrict dst,
                      coeff_t g, coeff_t P) {
    return _FNTT_base2_impl(src, len, dst, accelc_powmod(g, (P - 1) / len, P), P, 1);
}

int accelc_iFNTT_base2(const coeff_t* __restrict src, size_t len,
                       coeff_t* __restrict dst,
                       coeff_t g, coeff_t P) {
    g = accelc_Reciprocal_Fermat(g, P);
    if (1 != _FNTT_base2_impl(src, len, dst, accelc_powmod(g, (P - 1) / len, P), P, 1))
        return 0;

    coeff_t len_Reciprocal = accelc_Reciprocal_Fermat(len, P);
    for (size_t i = 0; i < len; ++i)
        dst[i] = _mulmod_coeff(len_Reciprocal, dst[i], P);
    return 1;
}

int accelc_FNTT_base2_extend(const coeff_t* __restrict src, size_t len,
                             coeff_t* __restrict dst,
                             coeff_t g, coeff_t P) {
    return _FNTT_base2_extend_impl(src, len, dst, accelc_powmod(g, (P - 1) / (2 * len), P), P, 1);
}
