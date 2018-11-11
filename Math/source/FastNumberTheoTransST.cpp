#include "../NTT.hpp"
#include "../UIntPower.hpp"
#include "AsmHelper.h"
#include <malloc.h>

namespace accel::Math {

#if defined(_M_IX86)
#define uintX_t uint32_t
#define _uaddmod _uaddmod64
#define _usubmod _usubmod64
#define _umulmod _umulmod64
#elif defined(_M_X64)
#define uintX_t uint64_t
#define _uaddmod _uaddmod128
#define _usubmod _usubmod128
#define _umulmod _umulmod128
#endif

    bool FastNttST_b2_Internal(const uintX_t src[], uintX_t dst[], size_t len,
                               uintX_t w, uintX_t P, size_t gap) {
        if (len == 2) {
            if (gap == 1) {
                dst[0] = _uaddmod(src[0], src[gap], P);
                dst[1] = _usubmod(src[0], src[gap], P);
            } else {
                dst[0] = _uaddmod(src[0], src[gap], P);
                dst[2] = _usubmod(src[0], src[gap], P);
            }
        } else {
            uintX_t* buffer = reinterpret_cast<uintX_t*>(malloc(len * sizeof(uintX_t)));
            if (buffer == nullptr)
                return false;

            uintX_t* dst_even = buffer;
            uintX_t* dst_odd = buffer + 1;

            uintX_t next_w = _umulmod(w, w, P);
            if (!FastNttST_b2_Internal(src, dst_even, len / 2,
                                       next_w,
                                       P,
                                       gap * 2)) {
                free(buffer);
                return false;
            }
            if (!FastNttST_b2_Internal(src + gap, dst_odd, len / 2,
                                       next_w,
                                       P,
                                       gap * 2)) {
                free(buffer);
                return false;
            }

            if (gap == 1) {
                for (size_t k = 0, pair = len / 2; k < pair; ++k) {
                    uintX_t left_part = dst_even[2 * k];
                    uintX_t right_part = _umulmod(UIntPowerModule(w, k, P), dst_odd[2 * k], P);
                    dst[k] = _uaddmod(left_part, right_part, P);
                    dst[k + pair] = _usubmod(left_part, right_part, P);
                }
            } else {
                for (size_t k = 0, pair = len / 2; k < pair; ++k) {
                    uintX_t left_part = dst_even[2 * k];
                    uintX_t right_part = _umulmod(UIntPowerModule(w, k, P), dst_odd[2 * k], P);
                    dst[2 * k] = _uaddmod(left_part, right_part, P);
                    dst[2 * (k + pair)] = _usubmod(left_part, right_part, P);
                }
            }

            free(buffer);
        }
        return true;
    }

    bool FastNttXST_b2_Internal(const uintX_t src[], uintX_t dst[], size_t len,
                                uintX_t w, uintX_t P, size_t gap) {
        if (len == 2) {
            if (gap == 1) {
                uintX_t right_part = _umulmod(src[gap], w, P);
                dst[0] = _uaddmod(src[0], src[gap], P);
                dst[2] = _usubmod(src[0], src[gap], P);
                dst[1] = _uaddmod(src[0], right_part, P);
                dst[3] = _usubmod(src[0], right_part, P);
            } else {
                uintX_t right_part = _umulmod(src[gap], w, P);
                dst[0] = _uaddmod(src[0], src[gap], P);
                dst[4] = _usubmod(src[0], src[gap], P);
                dst[2] = _uaddmod(src[0], right_part, P);
                dst[6] = _usubmod(src[0], right_part, P);
            }
        } else {
            uintX_t* buffer = reinterpret_cast<uintX_t*>(malloc(sizeof(uintX_t) * 2 * len));
            if (buffer == nullptr)
                return false;

            uintX_t* dst_even = buffer;
            uintX_t* dst_odd = buffer + 1;

            uintX_t next_w = _umulmod(w, w, P);
            if (!FastNttXST_b2_Internal(src, dst_even, len / 2,
                                        next_w,
                                        P,
                                        gap * 2)) {
                free(buffer);
                return false;
            }
            if (!FastNttXST_b2_Internal(src + gap, dst_odd, len / 2,
                                        next_w,
                                        P,
                                        gap * 2)) {
                free(buffer);
                return false;
            }

            if (gap == 1) {
                for (size_t k = 0, pair = len; k < pair; ++k) {
                    uintX_t left_part = dst_even[2 * k];
                    uintX_t right_part = _umulmod(UIntPowerModule(w, k, P), dst_odd[2 * k], P);
                    dst[k] = _uaddmod(left_part, right_part, P);
                    dst[k + pair] = _usubmod(left_part, right_part, P);
                }
            } else {
                for (size_t k = 0, pair = len; k < pair; ++k) {
                    uintX_t left_part = dst_even[2 * k];
                    uintX_t right_part = _umulmod(UIntPowerModule(w, k, P), dst_odd[2 * k], P);
                    dst[2 * k] = _uaddmod(left_part, right_part, P);
                    dst[2 * (k + pair)] = _usubmod(left_part, right_part, P);
                }
            }

            free(buffer);
        }
        return true;
    }

}

