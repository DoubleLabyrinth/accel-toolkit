#include "def.h"

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#endif

#if defined(_M_X64) || defined(__x86_64__)      // for 64 bit

    #define _addcarry_coeff _addcarry_u64
    #define _subborrow_coeff _subborrow_u64
    #define _bsf_coeff __bsfq
    #define _bsr_coeff __bsrq

    #define __A__ "%%rax"
    #define __B__ "%%rbx"
    #define __C__ "%%rcx"
    #define __D__ "%%rdx"
    #define __SI__ "%%rsi"
    #define __DI__ "%%rdi"

    #define __MOV__ "movq"

#elif defined(_M_IX86) || defined(__i386__)     // for 32 bit

    #define _addcarry_coeff _addcarry_u32
    #define _subborrow_coeff _subborrow_u32
    #define _bsf_coeff __bsfd
    #define _bsr_coeff __bsrd

    #define __A__ "%%eax"
    #define __B__ "%%ebx"
    #define __C__ "%%ecx"
    #define __D__ "%%edx"
    #define __SI__ "%%esi"
    #define __DI__ "%%edi"

    #define __MOV__ "movd"

#endif


#if defined(__GNUC__)   // some inline assembly func

static __attribute__((always_inline))
inline coeff_t _mul_coeff(coeff_t multiplier, coeff_t multiplicand,
                          coeff_t* product_h) {
    coeff_t product_l;
    __asm__("mul %3;"
            : "=a"(product_l), "=d"(*product_h)
            : "a"(multiplier), "r"(multiplicand));
    return product_l;
}

static __attribute__((always_inline))
inline coeff_t _mulmod_coeff(coeff_t multiplier, coeff_t multiplicand,
                             coeff_t modulus) {
    coeff_t ret;
    __asm__("mul " __D__ ";"
            "cmp " __C__ ", " __D__ ";"
            "jb fin0%=;"
            "mov " __A__ ", " __B__ ";"
            "mov " __D__ ", " __A__ ";"
            "xor " __D__ ", " __D__ ";"
            "div " __C__ ";"
            "mov " __B__ ", " __A__ ";"
            "fin0%=: div " __C__ ";"
            : "=d"(ret)
            : "a"(multiplier), "d"(multiplicand), "c"(modulus)
            : "bx");
    return ret;
}






// Return the quotient of (dividend / divisor)
// When (dividend_h >= divisor), a division overflow would be triggered.
// Please ensure that (dividend_h < divisor)
static __attribute__((always_inline))
inline coeff_t _div_coeff(coeff_t dividend_l, coeff_t dividend_h,
                          coeff_t divisor,
                          coeff_t* remainder) {
    coeff_t quotient;
    __asm__("div %4;"
            : "=a"(quotient), "=d"(*remainder)
            : "a"(dividend_l), "d"(dividend_h), "r"(divisor));
    return quotient;
}

// Return the quotient
// This function won't trigger division overflow, so it's safe to use
// But it may be slower than _div_only_coeff
static __attribute__((always_inline))
inline coeff_t _div_safe_coeff(coeff_t dividend_l, coeff_t dividend_h,
                               coeff_t divisor,
                               coeff_t* __restrict quotient_l, coeff_t* __restrict quotient_h) {
    coeff_t remainder;
    __asm__("cmp " __C__ ", " __D__ ";"
            "jb fin1%=;"
            "mov " __A__ ", " __B__ ";"
            "mov " __D__ ", " __A__ ";"
            "xor " __D__ ", " __D__ ";"
            "div " __C__ ";"
            "mov " __A__ ", (" __SI__ ");"
            "mov " __B__ ", (" __A__ ");"
            "fin1%=: div " __C__ ";"
            "mov " __A__ ", (" __DI__ ");"
            : "=d"(remainder)
            : "a"(dividend_l), "d"(dividend_h), "c"(divisor), "D"(quotient_l), "S"(quotient_h));
    return remainder;
}




// Return the quotient of (dividend / divisor)
// When (dividend_h >= divisor), a division overflow would be triggered.
// Please ensure that (dividend_h < divisor)
static __attribute__((always_inline))
inline coeff_t _div_only_coeff(coeff_t dividend_l, coeff_t dividend_h,
                               coeff_t divisor) {
    coeff_t quotient;
    __asm__("div %3;"
            : "=a"(quotient)
            : "a"(dividend_l), "d"(dividend_h), "r"(divisor));
    return quotient;
}

// This function won't trigger division overflow, so it's safe to use
// But it may be slower than _div_only_coeff
static __attribute__((always_inline))
inline void _div_only_safe_coeff(coeff_t dividend_l, coeff_t dividend_h,
                                 coeff_t divisor,
                                 coeff_t* __restrict quotient_l, coeff_t* __restrict quotient_h) {
    __asm__("cmp " __C__ ", " __D__ ";"
            "jb fin2%=;"
            "mov " __A__ ", " __B__ ";"
            "mov " __D__ ", " __A__ ";"
            "xor " __D__ ", " __D__ ";"
            "div " __C__ ";"
            "mov " __A__ ", (" __SI__ ");"
            "mov " __B__ ", (" __A__ ");"
            "fin2%=: div " __C__ ";"
            "mov " __A__ ", (" __DI__ ");"
            :
            : "a"(dividend_l), "d"(dividend_h), "c"(divisor), "D"(quotient_l), "S"(quotient_h));
}






// Return the remainder of (dividend / divisor)
// When (dividend_h >= divisor), a division overflow would be triggered.
// Please ensure that (dividend_h < divisor)
static __attribute__((always_inline))
inline coeff_t _mod_only_coeff(coeff_t dividend_l, coeff_t dividend_h,
                               coeff_t divisor) {
    coeff_t remainder;
    __asm__("div %3;"
            : "=d"(remainder)
            : "a"(dividend_l), "d"(dividend_h), "r"(divisor));
    return remainder;
}

// Return the remainder of (dividend / divisor)
// This function won't trigger division overflow, so it's safe to use
// But it may be slower than _mod_only_coeff
static __attribute__((always_inline))
inline coeff_t _mod_only_safe_coeff(coeff_t dividend_l, coeff_t dividend_h,
                                    coeff_t divisor) {
    coeff_t remainder;
    __asm__("cmp " __C__ ", " __D__ ";"
            "jb fin3%=;"
            "mov " __A__ ", " __B__ ";"
            "mov " __D__ ", " __A__ ";"
            "xor " __D__ ", " __D__ ";"
            "div " __C__ ";"
            "mov " __B__ ", (" __A__ ");"
            "fin3%=: div " __C__ ";"
    : "=d"(remainder)
    : "a"(dividend_l), "d"(dividend_h), "c"(divisor));
    return remainder;
}

#elif defined(_MSC_VER)     // x64 do not support inline assembly,
                            // so only declaration

coeff_t _mul_coeff(coeff_t multiplier, coeff_t multiplicand, coeff_t* product_h);
coeff_t _mulmod_coeff(coeff_t multiplier, coeff_t multiplicand, coeff_t modulus);



coeff_t _div_coeff(coeff_t dividend_l, coeff_t dividend_h,
                   coeff_t divisor,
                   coeff_t* remainder);
coeff_t _div_safe_coeff(coeff_t dividend_l, coeff_t dividend_h,
                        coeff_t divisor,
                        coeff_t* __restrict quotient_l, coeff_t* __restrict quotient_h);



coeff_t _div_only_coeff(coeff_t dividend_l, coeff_t dividend_h,
                        coeff_t divisor);
void _div_only_safe_coeff(coeff_t dividend_l, coeff_t dividend_h,
                          coeff_t divisor,
                          coeff_t* __restrict quotient_l, coeff_t* __restrict quotient_h);



coeff_t _mod_only_coeff(coeff_t dividend_l, coeff_t dividend_h,
                        coeff_t divisor);
coeff_t _mod_only_safe_coeff(coeff_t dividend_l, coeff_t dividend_h,
                             coeff_t divisor);

#endif