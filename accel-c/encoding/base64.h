#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    // Return -1 if len == 0
    // If return value > out_len, out_buf is too short. And return value is the minium length required.
    // If return value == out_len, convertion has been done successfully
    size_t accelcpp_Base64Encode(const void* __restrict src, size_t len,
                                 char* __restrict out_buf, size_t out_len);

    size_t accelcpp_Base64Encode_ssse3(const void* __restrict src, size_t len, 
                                       char* __restrict out_buf, size_t out_len);

    size_t accelcpp_Base64Encode_avx2(const void* __restrict src, size_t len,
                                      char* __restrict out_buf, size_t out_len);
    
#ifdef __cplusplus
}
#endif