#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

    void accelc_memory_xor(void* dst, const void* src, size_t len);
    void accelc_memory_and(void* dst, const void* src, size_t len);
    void accelc_memory_or(void* dst, const void* src, size_t len);
    void accelc_memory_not(void* dst, size_t len);

    void accelc_memory_xor_sse2(void* dst, const void* src, size_t len);
    void accelc_memory_and_sse2(void* dst, const void* src, size_t len);
    void accelc_memory_or_sse2(void* dst, const void* src, size_t len);
    void accelc_memory_not_sse2(void* dst, size_t len);

    void accelc_memory_xor_avx2(void* dst, const void* src, size_t len);
    void accelc_memory_and_avx2(void* dst, const void* src, size_t len);
    void accelc_memory_or_avx2(void* dst, const void* src, size_t len);
    void accelc_memory_not_avx2(void* dst, size_t len);

#if defined(__cplusplus)
}
#endif