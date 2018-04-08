#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(_M_X64) || defined(__x86_64__)      // for 64 bit
    typedef uint64_t coeff_t;
#elif defined(_M_IX86) || defined(__i386__)     // for 32 bit
    typedef uint32_t coeff_t;
#endif