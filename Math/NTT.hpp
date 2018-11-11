#pragma once
#include <stddef.h>
#include <stdint.h>

namespace accel::Math {

#if defined(_M_IX86)

    void NumberTheoTrans(const uint32_t src[], uint32_t dst[], size_t len, 
                         uint32_t g, uint32_t P);

    bool FastNumberTheoTrans(const uint32_t src[], uint32_t dst[], size_t len,
                             uint32_t g, uint32_t P, 
                             size_t NumberOfThreads = 1);

    void INumberTheoTrans(const uint32_t src[], uint32_t dst[], size_t len, 
                          uint32_t g, uint32_t P);

    bool IFastNumberTheoTrans(const uint32_t src[], uint32_t dst[], size_t len,
                              uint32_t g, uint32_t P, 
                              size_t NumberOfThreads = 1);

#elif defined(_M_X64)

    void NumberTheoTrans(const uint64_t src[], uint64_t dst[], size_t len, 
                         uint64_t g, uint64_t P);

    bool FastNumberTheoTrans(const uint64_t src[], uint64_t dst[], size_t len,
                             uint64_t g, uint64_t P, 
                             size_t NumberOfThreads = 1);

    void INumberTheoTrans(const uint64_t src[], uint64_t dst[], size_t len, 
                          uint64_t g, uint64_t P);

    bool IFastNumberTheoTrans(const uint64_t src[], uint64_t dst[], size_t len,
                              uint64_t g, uint64_t P, 
                              size_t NumberOfThreads = 1);

#else
#error "NOT Implement"
#endif

}

