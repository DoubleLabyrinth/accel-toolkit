#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(_M_X64)
typedef uint64_t uintr_t;
#elif defined(_M_IX86)
typedef uint32_t uintr_t;
#endif