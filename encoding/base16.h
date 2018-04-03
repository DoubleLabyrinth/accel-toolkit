#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    // Return 0 if len == 0
    // If return value > out_len, function fails because out_buf is too short,
    //     and return value is the minimum length of out_buf required.
    // If return value <= out_len, conversion has been done successfully,
    //     and return value is the count, in bytes, written to out_buf.
    size_t accelc_Base16_Encode(const void* __restrict src, size_t len,
                                char* __restrict out_buf, size_t out_len);

    size_t accelc_Base16_Encode_ssse3(const void* __restrict src, size_t len,
                                      char* __restrict out_buf, size_t out_len);

    size_t accelc_Base16_Encode_avx2(const void* __restrict src, size_t len,
                                     char* __restrict out_buf, size_t out_len);



    // Return 0 if src is not a valid Base16-encoded string or len is 0.
    // If return value > out_len, function fails because out_buf is too short,
    //     and return value is the minimum length of out_buf required.
    // If return value <= out_len, conversion has been done successfully,
    //     and return value is the count, in bytes, written to out_buf.
    size_t accelc_Base16_Decode(const char* __restrict src, size_t len,
                                void* __restrict out_buf, size_t out_len);

    size_t accelc_Base16_Decode_ssse3(const char* __restrict src, size_t len,
                                      void* __restrict out_buf, size_t out_len);

    size_t accelc_Base16_Decode_avx2(const char* __restrict src, size_t len,
                                     void* __restrict out_buf, size_t out_len);



    // Return 0 if src is not a valid Base16-encoded string or len is 0.
    // Otherwise return value is length of decoded data.
    size_t accelc_Base16_Check(const char* src, size_t len);

    size_t accelc_Base16_Check_ssse3(const char* src, size_t len);

    size_t accelc_Base16_Check_avx2(const char* src, size_t len);

#ifdef __cplusplus
}
#endif