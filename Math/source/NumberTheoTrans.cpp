#include "../NTT.hpp"
#include "../UIntPower.hpp"
#include "AsmHelper.h"
#include <intrin.h>

namespace accel::Math {

#if defined(_M_IX86) || defined(_M_X64)

#if defined(_M_IX86)
#define _addcarry_uX _addcarry_u32
#define _umulmod(a, b, m) _umulmod64(a, b, m)
#define _umod(low, high, divisor) _umod64(low, high, divisor)
#define uintX_t uint32_t
#else
#define _addcarry_uX _addcarry_u64
#define _umulmod(a, b, m) _umulmod128(a, b, m)
#define _umod(low, high, divisor) _umod128(low, high, divisor)
#define uintX_t uint64_t
#endif

    // dst[n] = sigma_k_(0)_to_(len - 1)_(src[k] * w ^ nk % P)
    // while w = g ^ ((P - 1) / len) % P
    void NumberTheoTrans(const uintX_t src[], uintX_t dst[], size_t len, uintX_t g, uintX_t P) {
        uintX_t w = UIntPowerModule(g, (P - 1) / len, P);

        for (size_t n = 0; n < len; ++n) {
            uintX_t sum_l = 0;
            uintX_t sum_h = 0;
            for (size_t k = 0; k < len; ++k) {
                sum_h += _addcarry_uX(0,
                                      sum_l,
                                      _umulmod(src[k], UIntPowerModule(UIntPowerModule(w, n, P), k, P), P),
                                      &sum_l);
                if (sum_h >= P) {
                    sum_h = 0;
                    sum_l %= P;
                }
            }
            dst[n] = _umod(sum_l, sum_h, P);
        }
    }

    // dst[n] = (len ^ -1 % P) * sigma_k_(0)_to_(len - 1)_(src[k] * (w ^ -1 % P) ^ nk % P)
    // while w = g ^ ((P - 1) / len) % P
    void INumberTheoTrans(const uintX_t src[], uintX_t dst[], size_t len, uintX_t g, uintX_t P) {
        uintX_t w;
        uintX_t len_reciprocal;
        // g = g ^ -1 % P
        // w = g ^ ((P - 1) / len) % P
        // len_reciprocal = len ^ -1 % P
        g = UIntPowerModule(g, P - 2, P);
        w = UIntPowerModule(g, (P - 1) / len, P);
        len_reciprocal = UIntPowerModule(len, P - 2, P);

        for (size_t n = 0; n < len; ++n) {
            uintX_t sum_l = 0;
            uintX_t sum_h = 0;
            for (size_t k = 0; k < len; ++k) {
                sum_h += _addcarry_uX(0,
                                      sum_l,
                                      _umulmod(src[k], UIntPowerModule(UIntPowerModule(w, n, P), k, P), P),
                                      &sum_l);

                if (sum_h >= P) {
                    sum_h = 0;
                    sum_l %= P;
                }
            }

            if (sum_h) 
                sum_l = _umod(sum_l, sum_h, P);

            dst[n] = _umulmod(len_reciprocal, sum_l, P);
        }
    }

#endif

}
