#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define MD4_DIGEST_SIZE 16

    typedef struct _MD4_DIGEST {
        union {
            uint8_t byte[MD4_DIGEST_SIZE];
            uint32_t dword[MD4_DIGEST_SIZE / sizeof(uint32_t)];
        };
    } MD4_DIGEST, MD4_BUFFER;

    void accelc_MD4_init(MD4_BUFFER* HashBuffer);

    void accelc_MD4_update(const void* srcBytes, size_t srcBytesLength,
                           MD4_BUFFER* HashBuffer);

    void accelc_MD4_final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength,
                          const MD4_BUFFER* HashBuffer, MD4_DIGEST* Hash);

    void accelc_MD4(const void* srcBytes, size_t srclen, MD4_DIGEST* Hash);

#if defined(__cplusplus)
}
#endif