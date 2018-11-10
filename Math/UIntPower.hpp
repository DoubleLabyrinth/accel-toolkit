#pragma once
#include <stddef.h>
#include <stdint.h>

namespace accel::Math {

#if defined(_M_IX86)

    uint32_t UIntPowerModule(uint32_t Base, uint32_t Exponent, uint32_t Modulus);

#elif defined(_M_X64)

    uint64_t UIntPowerModule(uint64_t Base, uint64_t Exponent, uint64_t Modulus);

#else
#error "NOT Implement"
#endif

}
