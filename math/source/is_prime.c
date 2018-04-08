#include "../num_theory.h"
#include "../def_asm.h"

#include "../random.h"

int accelc_MillerRabinTest(coeff_t n, size_t count) {
    if (n % 2 == 0 || n == 1)
        return 0;

    int s = _bsf_coeff(n - 1);
    coeff_t d = n >> s;    // n - 1 = 2 ^ s * d

    LCG_PARAM lcg_param = { 13, 0, 11, n };
    lcg_param.Seed = __rdtsc() % n;
    while (count-- > 0) {
        coeff_t a = accelc_LCG_next(&lcg_param);
        coeff_t ret = accelc_powmod(a, d, n);
        if (ret == 1 || ret == n - 1)
            continue;

        int go_next = 0;
        for (unsigned long i = 1; i < s; i++) {
            ret = accelc_powmod(ret, 2, n);
            if (ret == 1)
                return 0;
            if (ret == n - 1) {
                go_next = 1;
                break;
            }
        }
        if (go_next)
            continue;

        return 0;
    }
    return 1;
}