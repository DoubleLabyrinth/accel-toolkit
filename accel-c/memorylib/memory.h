#pragma once
#include <stdint.h>
#include <stddef.h>
#include <intrin.h>

#if defined(__cplusplus)
extern "C" {
#endif

    void MEMORY_XOR(void* dst, const void *src, size_t size);
    void MEMORY_AND(void* dst, const void *src, size_t size);
    void MEMORY_OR(void* dst, const void *src, size_t size);
    void MEMORY_NOT(void* dst, size_t size);

#if defined(__cplusplus)
}
#endif
