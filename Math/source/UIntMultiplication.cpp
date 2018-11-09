#include "../UIntMultiplication.hpp"
#include <intrin.h>
#include <utility>

namespace accel::Math {

#if defined(_M_IX86) || defined(_M_X64)

#if defined(_M_IX86)
#define _umultiply(a, b, c) *reinterpret_cast<uint64_t*>(c) = __emulu(a, b)
#define _addcarry_uX _addcarry_u32
#define uintX_t uint32_t
#else
#define _umultiply(a, b, c) c[0] = _umul128(a, b, c + 1)
#define _addcarry_uX _addcarry_u64
#define uintX_t uint64_t
#endif

    uintX_t UIntMul(uintX_t a[], size_t a_len,
                    uintX_t b) {
        uintX_t temp[2];
        uintX_t carry = 0;
        for (size_t i = 0; i < a_len; ++i) {
            _umultiply(a[i], b, temp);
            carry = temp[1] + _addcarry_uX(0, temp[0], carry, a + i);
        }
        
        return carry;
    }

    bool UIntMul(const uintX_t a[], size_t a_len,
                 const uintX_t b[], size_t b_len,
                 uintX_t product[], size_t product_len) {

        if (a_len + b_len > product_len)
            return false;

        memset(product, 0, product_len * sizeof(uintX_t));

        uintX_t temp[2];
        for (size_t i = 0; i < a_len; ++i) {
            for (size_t j = 0; j < b_len; ++j) {
                _umultiply(b[j], a[i], temp);
                if (_addcarry_uX(_addcarry_uX(0, product[i + j], temp[0], product + i + j),
                                 product[i + j + 1],
                                 temp[1],
                                 product + i + j + 1)) {
                    uintX_t* block = product + i + j + 2;
                    while (_addcarry_uX(1, *block, 0, block))
                        ++block;
                }
            }
        }

        return true;
    }

    bool UIntMul(const uintX_t a[], size_t a_len,
                 uintX_t b,
                 uintX_t product[], size_t product_len) {
        if (product_len < a_len + 1)
            return false;

        uintX_t temp[2];
        uintX_t carry = 0;

        for (size_t i = 0; i < a_len; ++i) {
            _umultiply(a[i], b, temp);
            carry = temp[1] + _addcarry_uX(0, temp[0], carry, product + i);
        }

        product[a_len] = carry;
        
        return true;
    }

#endif

}
