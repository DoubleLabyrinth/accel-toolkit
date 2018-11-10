#include "../UIntPower.hpp"
#include "AsmHelper.h"
#include <intrin.h>

namespace accel::Math {

#if defined(_M_IX86) || defined(_M_X64)

#if defined(_M_IX86)
#define _umultiply(a, b, c) *reinterpret_cast<uint64_t*>(c) = __emulu(a, b)
#define _umod(low, high, divisor) _umod64(low, high, divisor)
#define uintX_t uint32_t
#else
#define _umultiply(a, b, c) c[0] = _umul128(a, b, c + 1)
#define _umod(low, high, divisor) _umod128(low, high, divisor)
#define uintX_t uint64_t
#endif

    uintX_t UIntPowerModule(uintX_t Base, uintX_t Exponent, uintX_t Modulus) {
        uintX_t ret = 1 % Modulus;
        uintX_t temp[2];
        Base %= Modulus;
        while (Exponent) {
            if (Exponent % 2 == 1) {
                _umultiply(ret, Base, temp);
                ret = _umod(temp[0], temp[1], Modulus);
            }
            Exponent /= 2;
            _umultiply(Base, Base, temp);
            Base = _umod(temp[0], temp[1], Modulus);
        }
        return ret;
    }


#endif

}
