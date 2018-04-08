#include "../random.h"
#include "../def_asm.h"

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#endif

coeff_t accelc_LCG_next(LCG_PARAM* param) {
    coeff_t temp[2];
    temp[0] = _mul_coeff(param->A, param->Seed, temp + 1);
    if (_addcarry_coeff(_addcarry_coeff(0,
                                        temp[0], param->B,
                                        temp),
                        temp[1], 0,
                        temp + 1)) {
        temp[1] = _mod_only_coeff(temp[1], 1, param->P);
    }

    if (temp[1] >= param->P)
        temp[1] %= param->P;
    param->Seed = _mod_only_coeff(temp[0], temp[1], param->P);
    return param->Seed;
}