#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    inline size_t accelc_Base16_GetRequireLength(size_t srclen) {
        return 2 * srclen;
    }

    size_t accelc_Base16_Encode(const void* __restrict src, size_t len,
                                char* __restrict out_buf, size_t out_len);

    size_t accelc_Base16_Encode_ssse3(const void* __restrict src, size_t len,
                                      char* __restrict out_buf, size_t out_len);

    size_t accelc_Base16_Encode_avx2(const void* __restrict src, size_t len,
                                     char* __restrict out_buf, size_t out_len);

    size_t accelc_Base16_Decode(const char* __restrict src, size_t len,
                                void* __restrict out_buf, size_t out_len);

    size_t accelc_Base16_Decode_ssse3(const char* __restrict src, size_t len,
                                      void* __restrict out_buf, size_t out_len);

    size_t accelc_Base16_Decode_avx2(const char* __restrict src, size_t len,
                                     void* __restrict out_buf, size_t out_len);

    int accelc_Base16_Check(const char* src, size_t len);

    int accelc_Base16_Check_ssse3(const char* src, size_t len);

    int accelc_Base16_Check_avx2(const char* src, size_t len);

#ifdef __cplusplus
}
#endif