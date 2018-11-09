#include "../UIntSubtraction.hpp"
#include <intrin.h>
#include <utility>

namespace accel::Math {

#if defined(_M_IX86) || defined(_M_X64)

#if defined(_M_IX86)
#define _subborrow_uX _subborrow_u32
#define uintX_t uint32_t
#else
#define _subborrow_uX _subborrow_u64
#define uintX_t uint64_t
#endif

    uint8_t UIntSub(uintX_t a[], size_t a_len,
                    const uintX_t b[], size_t b_len,
                    uint8_t borrow) {
        // if a_len < b_len, a may overflow
        // we do not handle this situation
        if (a_len < b_len)
            return static_cast<uint8_t>(-1);

        size_t i = 0;

        for (; i < b_len; ++i) {
            borrow = _subborrow_uX(borrow, a[i], b[i], a + i);
        }

        for (; borrow != 0 && i < a_len; ++i) {
            borrow = _subborrow_uX(borrow, a[i], 0, a + i);
        }

        return borrow;
    }

    uint8_t UIntSub(uintX_t a[], size_t a_len,
                    uintX_t b,
                    uint8_t borrow) {
        if (a_len < 1)
            return borrow;

        borrow = _subborrow_uX(borrow, a[0], b, a);
        for (size_t i = 1; borrow != 0 && i < a_len; ++i) {
            borrow = _subborrow_uX(borrow, a[i], 0, a + i);
        }

        return borrow;
    }

    uint8_t UIntSub(const uintX_t a[], size_t a_len,
                    const uintX_t b[], size_t b_len,
                    uintX_t diff[], size_t diff_len,
                    uint8_t borrow) {
        if (a_len < b_len) {
            std::swap(a, b);
            std::swap(a_len, b_len);
        }

        // diff_len must be greater than or equal to max(a_len, b_len)
        if (diff_len < a_len)
            return static_cast<uint8_t>(-1);

        size_t i = 0;

        for (; i < b_len; ++i) {
            borrow = _subborrow_uX(borrow, a[i], b[i], diff + i);
        }

        for (; borrow != 0 && i < a_len; ++i) {
            borrow = _subborrow_uX(borrow, a[i], 0, diff + i);
        }

        memcpy(diff + i, a + i, (a_len - i) * sizeof(uintX_t));
        memset(diff + a_len, 0, (diff_len - a_len) * sizeof(uintX_t));
        return borrow;
    }

    uint8_t UIntSub(const uintX_t a[], size_t a_len,
                    uintX_t b,
                    uintX_t diff[], size_t diff_len,
                    uint8_t borrow) {
        if (a_len < 1)
            return borrow;

        if (diff_len < a_len)
            return static_cast<uint8_t>(-1);

        size_t i = 1;

        borrow = _subborrow_uX(borrow, a[0], b, diff);
        for (; borrow != 0 && i < a_len; ++i) {
            borrow = _subborrow_uX(borrow, a[i], 0, diff + i);
        }

        memcpy(diff + i, a + i, (a_len - i) * sizeof(uintX_t));
        memset(diff + a_len, 0, (diff_len - a_len) * sizeof(uintX_t));
        return borrow;
    }

#endif

}
