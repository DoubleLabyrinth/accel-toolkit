#include "../UIntAddition.hpp"
#include <intrin.h>
#include <utility>

namespace accel::Math {

#if defined(_M_IX86) || defined(_M_X64)

#if defined(_M_IX86)
#define _addcarry_uX _addcarry_u32
#define uintX_t uint32_t
#else
#define _addcarry_uX _addcarry_u64
#define uintX_t uint64_t
#endif

    uint8_t UIntAdd(uintX_t a[], size_t a_len,
                    const uintX_t b[], size_t b_len,
                    uint8_t carry) {
        // if a_len < b_len, a may overflow
        // we do not handle this situation
        if (a_len < b_len)
            return static_cast<uint8_t>(-1);

        size_t i = 0;

        for (; i < b_len; ++i) {
            carry = _addcarry_uX(carry, a[i], b[i], a + i);
        }

        for (; carry != 0 && i < a_len; ++i) {
            carry = _addcarry_uX(carry, a[i], 0, a + i);
        }

        return carry;
    }

    uint8_t UIntAdd(uintX_t a[], size_t a_len,
                    uintX_t b,
                    uint8_t carry) {
        if (a_len < 1)
            return carry;

        carry = _addcarry_uX(carry, a[0], b, a);
        for (size_t i = 1; carry != 0 && i < a_len; ++i) {
            carry = _addcarry_uX(carry, a[i], 0, a + i);
        }

        return carry;
    }

    uint8_t UIntAdd(const uintX_t a[], size_t a_len,
                    const uintX_t b[], size_t b_len,
                    uintX_t sum[], size_t sum_len,
                    uint8_t carry) {
        if (a_len < b_len) {
            std::swap(a, b);
            std::swap(a_len, b_len);
        }

        // sum_len must be greater than or equal to max(a_len, b_len)
        if (sum_len < a_len) 
            return static_cast<uint8_t>(-1);

        size_t i = 0;

        for (; i < b_len; ++i) {
            carry = _addcarry_uX(carry, a[i], b[i], sum + i);
        }

        for (; carry != 0 && i < a_len; ++i) {
            carry = _addcarry_uX(carry, a[i], 0, sum + i);
        }

        memcpy(sum + i, a + i, (a_len - i) * sizeof(uintX_t));
        memset(sum + a_len, 0, (sum_len - a_len) * sizeof(uintX_t));
        return carry;
    }

    uint8_t UIntAdd(const uintX_t a[], size_t a_len,
                    uintX_t b,
                    uintX_t sum[], size_t sum_len,
                    uint8_t carry) {
        if (a_len < 1)
            return carry;

        if (sum_len < a_len)
            return static_cast<uint8_t>(-1);

        size_t i = 1;

        carry = _addcarry_uX(carry, a[0], b, sum);
        for (; carry != 0 && i < a_len; ++i) {
            carry = _addcarry_uX(carry, a[i], 0, sum + i);
        }

        memcpy(sum + i, a + i, (a_len - i) * sizeof(uintX_t));
        memset(sum + a_len, 0, (sum_len - a_len) * sizeof(uintX_t));
        return carry;
    }

#endif

}

