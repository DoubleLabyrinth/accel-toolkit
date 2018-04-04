#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

    void accelc_memory_reverse(void* dst, size_t len);
    void accelc_memory_reverse_ssse3(void* dst, size_t len);
    void accelc_memory_reverse_avx2(void* dst, size_t len);

#if defined(__cplusplus)
}
#endif