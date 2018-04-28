#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define SHA1_DIGEST_SIZE 20

    typedef struct _SHA1_DIGEST {
        union {
            uint8_t byte[SHA1_DIGEST_SIZE];
            uint32_t dword[SHA1_DIGEST_SIZE / sizeof(uint32_t)];
        };
    } SHA1_DIGEST, SHA1_BUFFER;

    void accelc_SHA1_init(SHA1_BUFFER* HashBuffer);

    void accelc_SHA1_update(const void* srcBytes, size_t srcBytesLength, SHA1_BUFFER* HashBuffer);

    void accelc_SHA1_final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength,
                           const SHA1_BUFFER* HashBuffer, SHA1_DIGEST* Hash);

    void accelc_SHA1(const void* srcBytes, size_t srclen, SHA1_DIGEST* Hash);

#if defined(__cplusplus)
}
#endif