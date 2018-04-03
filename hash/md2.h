#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define MD2_DIGEST_SIZE 16

    typedef struct _MD2_DIGEST {
        union {
            uint8_t byte[MD2_DIGEST_SIZE];
            uint32_t dword[MD2_DIGEST_SIZE / sizeof(uint32_t)];
        };
    } MD2_DIGEST;

    void accelc_MD2(const void* srcBytes, size_t srclen, MD2_DIGEST* Hash);

#if defined(__cplusplus)
}
#endif