#include "../UIntAddition.hpp"
#include "../UIntSubtraction.hpp"
#include "../UIntMultiplication.hpp"
#include <intrin.h>
#include <memory.h>
#include <malloc.h>

namespace accel::Math {

#if defined(_M_IX86) || defined(_M_X64)

#if defined(_M_IX86)
#define _addcarry_uX _addcarry_u32
#define uintX_t uint32_t
#else
#define _addcarry_uX _addcarry_u64
#define uintX_t uint64_t
#endif

#define KARATSUBA_CUT_LENGTH 8

    // product = a * b
    //         = a1a0 * b1*b0
    //         = (a1 * b1) | (a1 * b0) + (a0 * b1) | a0 * b0
    //         = (a1 * b1) | (a1 + a0) * (b1 + b0) - (a1 * b1) - (a0 * b0) | a0 * b0
    void KaratsubaSTInternal(const uintX_t a[], const uintX_t b[],
                               size_t len,
                               uintX_t product[]) {

        if (len <= KARATSUBA_CUT_LENGTH) {
            UIntMul(a, len, b, len, product, 2 * len);
            return;
        }

        KaratsubaSTInternal(a,
                            b,
                            len / 2,
                            product);

        KaratsubaSTInternal(a + len / 2,
                            b + len / 2,
                            len / 2,
                            product + len);

        uintX_t* buffer = reinterpret_cast<uintX_t*>(malloc(len * 2 * sizeof(uintX_t)));

        uint8_t carry_a = UIntAdd(a, len / 2, 
                                  a + len / 2, len / 2, 
                                  buffer, len / 2, 
                                  0);
        uint8_t carry_b = UIntAdd(b, len / 2, 
                                  b + len / 2, len / 2, 
                                  buffer + len / 2, len / 2, 
                                  0);

        uint8_t carry = 0;
        KaratsubaSTInternal(buffer,
                            buffer + len / 2,
                            len / 2,
                            buffer + len);

        if (carry_a)
            carry += UIntAdd(buffer + len + len / 2, len / 2,
                             buffer + len / 2, len / 2,
                             0);

        if (carry_b)
            carry += UIntAdd(buffer + len + len / 2, len / 2,
                             buffer, len / 2,
                             0);

        if (carry_a && carry_b)
            carry++;

        carry -= UIntSub(buffer + len, len,
                         product, len,
                         0);

        carry -= UIntSub(buffer + len, len,
                         product + len, len,
                         0);

        UIntAdd(product + len / 2, len + len / 2,
                buffer + len, len,
                0);

        UIntAdd(product + len + len / 2, len / 2,
                0,
                carry);

        free(buffer);
    }

#endif

}

