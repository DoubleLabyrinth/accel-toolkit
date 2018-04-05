#include "../num_theory.h"
#include "../arithmetic.h"
#include <memory.h>

// Get the greatest common divisor of a and b
// The return value is the GCD
coeff_t accelc_GCD(coeff_t a, coeff_t b) {
    if (a == 0)
        return b;
    if (b == 0)
        return a;

    coeff_t rem;
    while ((rem = a % b) != 0) {
        a = b;
        b = rem;
    }
    return b;
}

// Get the Greatest Common Divisor of a and b
// If return value if 1, the GCD value is stored in b
// If return value if 0, the GCD value is stored in a
int accelc_uintx_GCD(coeff_t* __restrict a, size_t a_length,
                     coeff_t* __restrict b, size_t b_length) {
    //                       a                    b
    // Step_1:    _a -> |         |    _b -> |         |      _a %= b; if (_a == 0) goto Final else Swap(_a, _b) and Swap(_a_length, _b_length)
    // Step_2:    _b -> |         |    _a -> |         |      _a %= b; if (_a == 0) goto Final else Swap(_a, _b) and Swap(_a_length, _b_length)
    // Step_3:    _a -> |         |    _b -> |         |      _a %= b; if (_a == 0) goto Final else Swap(_a, _b) and Swap(_a_length, _b_length)
    //   .
    //   .
    //   .
    // Final:  if _a -> |    0    |    _b -> |   xxx   |      copy _b to a
    //       else _b -> |   xxx   |    _a -> |    0    |      just return

    coeff_t* _a = a;
    coeff_t* _b = b;

    size_t _a_length = a_length;
    size_t _b_length = b_length;

    while(1) {
        _a_length = accelc_uintx_mod(_a, _a_length, _b, _b_length);
        if (_a_length == 1 && _a[0] == 0) {
            if (_b != a)
                return 1;
            else
                return 0;
        }

        // Swap(_a, _b)
        coeff_t* temp = _a;
        _a = _b;
        _b = temp;

        // Swap(_a_length, _b_length)
        size_t temp = _a_length;
        _a_length = _b_length;
        _b_length = temp;
    }
}

