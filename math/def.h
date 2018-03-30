#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(_M_X64) || defined(__x86_64__)
typedef uint64_t coeff_t;
#elif defined(_M_IX86) || defined(__i386__)
typedef uint32_t coeff_t;
#endif