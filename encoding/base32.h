#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    // Return -1 if len == 0
    // If return value > out_len, out_buf is too short. And return value is the minimum length required.
    // If return value == out_len, conversion has been done successfully
    size_t accelc_Base32_Encode(const void* __restrict src, size_t len,
                                char* __restrict out_buf, size_t out_len);

    size_t accelc_Base32_Encode_ssse3(const void* __restrict src, size_t len,
                                     char* __restrict out_buf, size_t out_len);

    size_t accelc_Base32_Encode_avx2(const void* __restrict src, size_t len,
                                    char* __restrict out_buf, size_t out_len);

    size_t accelc_Base32_Decode(const char* __restrict src, size_t len,
                               void* __restrict out_buf, size_t out_len);

    size_t accelc_Base32_Decode_ssse3(const char* __restrict src, size_t len,
                                     void* __restrict out_buf, size_t out_len);

    size_t accelc_Base32_Decode_avx2(const char* __restrict src, size_t len,
                                    void* __restrict out_buf, size_t out_len);

#ifdef __cplusplus
}
#endif
