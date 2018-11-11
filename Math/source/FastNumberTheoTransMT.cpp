#include "../NTT.hpp"
#include "../UIntPower.hpp"
#include "AsmHelper.h"
#include <malloc.h>
#include <windows.h>

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
                               uintX_t w, uintX_t P, size_t gap);

    struct FASTNTT_MT_B2_ARGS {
        const uintX_t* src; 
        uintX_t* dst;
        size_t len;
        uintX_t w; 
        uintX_t P;
        size_t gap;
        union {
            size_t NumberOfThreads;
            bool success;
        };
    };

    static DWORD WINAPI FastNttMT_b2_Internal(PVOID pParameter) {
        FASTNTT_MT_B2_ARGS* pArg = reinterpret_cast<FASTNTT_MT_B2_ARGS*>(pParameter);

        if (pArg->len == 2) {
            if (pArg->gap == 1) {
                pArg->dst[0] = _uaddmod(pArg->src[0], pArg->src[pArg->gap], pArg->P);
                pArg->dst[1] = _usubmod(pArg->src[0], pArg->src[pArg->gap], pArg->P);
            } else {
                pArg->dst[0] = _uaddmod(pArg->src[0], pArg->src[pArg->gap], pArg->P);
                pArg->dst[2] = _usubmod(pArg->src[0], pArg->src[pArg->gap], pArg->P);
            }
        } else {
            uintX_t* buffer = reinterpret_cast<uintX_t*>(malloc(pArg->len * sizeof(uintX_t)));
            if (buffer == nullptr) {
                pArg->success = false;
                return 0;
            }

            uintX_t* dst_even = buffer;
            uintX_t* dst_odd = buffer + 1;

            uintX_t next_w = _umulmod(pArg->w, pArg->w, pArg->P);
            if (pArg->NumberOfThreads >= 2) {
                HANDLE hThreads[2];
                FASTNTT_MT_B2_ARGS Args[2];

                Args[0].src = pArg->src;
                Args[0].dst = dst_even;
                Args[0].len = pArg->len / 2;
                Args[0].w = next_w;
                Args[0].P = pArg->P;
                Args[0].gap = pArg->gap * 2;
                Args[0].NumberOfThreads = pArg->NumberOfThreads / 2;

                Args[1].src = pArg->src + pArg->gap;
                Args[1].dst = dst_odd;
                Args[1].len = pArg->len / 2;
                Args[1].w = next_w;
                Args[1].P = pArg->P;
                Args[1].gap = pArg->gap * 2;
                Args[1].NumberOfThreads = pArg->NumberOfThreads / 2;

                hThreads[0] = CreateThread(NULL, 0, FastNttMT_b2_Internal, Args + 0, NULL, NULL);
                hThreads[1] = CreateThread(NULL, 0, FastNttMT_b2_Internal, Args + 1, NULL, NULL);
                WaitForMultipleObjects(_countof(hThreads), hThreads, TRUE, INFINITE);

                if (!Args[0].success || !Args[1].success) {
                    free(buffer);
                    pArg->success = false;
                    return 0;
                }
            } else {
                if (!FastNttST_b2_Internal(pArg->src, dst_even, pArg->len / 2,
                                           next_w,
                                           pArg->P,
                                           pArg->gap * 2)) {
                    free(buffer);
                    pArg->success = false;
                    return 0;
                }
                if (!FastNttST_b2_Internal(pArg->src + pArg->gap, dst_odd, pArg->len / 2,
                                           next_w,
                                           pArg->P,
                                           pArg->gap * 2)) {
                    free(buffer);
                    pArg->success = false;
                    return 0;
                }
            }

            if (pArg->gap == 1) {
                for (size_t k = 0, pair_offset = pArg->len / 2; k < pair_offset; ++k) {
                    uintX_t left_part = dst_even[2 * k];
                    uintX_t right_part = _umulmod(UIntPowerModule(pArg->w, k, pArg->P), dst_odd[2 * k], pArg->P);
                    pArg->dst[k] = _uaddmod(left_part, right_part, pArg->P);
                    pArg->dst[k + pair_offset] = _usubmod(left_part, right_part, pArg->P);
                }
            } else {
                for (size_t k = 0, pair_offset = pArg->len / 2; k < pair_offset; ++k) {
                    uintX_t left_part = dst_even[2 * k];
                    uintX_t right_part = _umulmod(UIntPowerModule(pArg->w, k, pArg->P), dst_odd[2 * k], pArg->P);
                    pArg->dst[2 * k] = _uaddmod(left_part, right_part, pArg->P);
                    pArg->dst[2 * (k + pair_offset)] = _usubmod(left_part, right_part, pArg->P);
                }
            }

            free(buffer);
        }
        pArg->success = true;
        return 0;
    }

    bool FastNumberTheoTrans(const uintX_t src[], uintX_t dst[], size_t len,
                             uintX_t g, uintX_t P, 
                             size_t NumberOfThreads) {
        // if len is not a power of 2
        // refuse to transform
        if (len == 0 || (len & (len - 1)))
            return false;

        // if NumberOfThreads is not a power of 2
        // refuse to transform
        if (NumberOfThreads == 0 || (NumberOfThreads & (NumberOfThreads - 1)))
            return false;

        if (len == 1) {
            dst[0] = src[0];
            return true;
        }

        if (NumberOfThreads == 1) {
            return FastNttST_b2_Internal(src, dst, len,
                                         UIntPowerModule(g, (P - 1) / len, P), P,
                                         1);
        } else {
            FASTNTT_MT_B2_ARGS Arg;
            Arg.src = src;
            Arg.dst = dst;
            Arg.len = len;
            Arg.w = UIntPowerModule(g, (P - 1) / len, P);
            Arg.P = P;
            Arg.gap = 1;
            Arg.NumberOfThreads = NumberOfThreads;
            FastNttMT_b2_Internal(&Arg);
            return Arg.success;
        }
    }

    bool IFastNumberTheoTrans(const uintX_t src[], uintX_t dst[], size_t len,
                              uintX_t g, uintX_t P,
                              size_t NumberOfThreads) {
        // if len is not a power of 2
        // refuse to transform
        if (len == 0 || (len & (len - 1)))
            return false;

        // if NumberOfThreads is not a power of 2
        // refuse to transform
        if (NumberOfThreads == 0 || (NumberOfThreads & (NumberOfThreads - 1)))
            return false;

        if (len == 1) {
            dst[0] = src[0];
            return true;
        }

        if (len == 1) {
            dst[0] = src[0];
            return true;
        }
        
        // g = g ^ -1 % P
        g = UIntPowerModule(g, P - 2, P);
        if (NumberOfThreads == 1) {
            if (!FastNttST_b2_Internal(src, dst, len,
                                       UIntPowerModule(g, (P - 1) / len, P), P,
                                       1)) 
                return false;
        } else {
            FASTNTT_MT_B2_ARGS Arg;
            Arg.src = src;
            Arg.dst = dst;
            Arg.len = len;
            Arg.w = UIntPowerModule(g, (P - 1) / len, P);
            Arg.P = P;
            Arg.gap = 1;
            Arg.NumberOfThreads = NumberOfThreads;
            FastNttMT_b2_Internal(&Arg);
            
            if (!Arg.success)
                return false;
        }

        uintX_t len_Reciprocal = UIntPowerModule(len, P - 2, P);
        for (size_t i = 0; i < len; ++i)
            dst[i] = _umulmod(len_Reciprocal, dst[i], P);
        return true;
    }

}
