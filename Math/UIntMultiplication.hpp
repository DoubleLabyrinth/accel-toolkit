#pragma once
#include <stddef.h>
#include <stdint.h>

namespace accel::Math {

#if defined(_M_IX86)

    uint32_t UIntMul(uint32_t a[], size_t a_len,
                    uint32_t b);

    bool UIntMul(const uint32_t a[], size_t a_len,
                 const uint32_t b[], size_t b_len,
                 uint32_t product[], size_t product_len);

    bool UIntMul(const uint32_t a[], size_t a_len,
                 uint32_t b,
                 uint32_t product[], size_t product_len);

    bool UIntMulKaratsuba(const uint32_t a[], const uint32_t b[], size_t len,
                          uint32_t product[], size_t product_len,
                          size_t NumberOfThreads = 1);

#elif defined(_M_X64)

    uint64_t UIntMul(uint64_t a[], size_t a_len,
                     uint64_t b);

    bool UIntMul(const uint64_t a[], size_t a_len,
                 const uint64_t b[], size_t b_len,
                 uint64_t product[], size_t product_len);

    bool UIntMul(const uint64_t a[], size_t a_len,
                 uint64_t b,
                 uint64_t product[], size_t product_len);

    bool UIntMulKaratsuba(const uint64_t a[], const uint64_t b[], size_t len,
                          uint64_t product[], size_t product_len,
                          size_t NumberOfThreads = 1);

#endif

}

