#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define MD5_DIGEST_SIZE 16

    typedef struct _MD5_DIGEST {
        union {
            uint8_t byte[MD5_DIGEST_SIZE];
            uint32_t dword[MD5_DIGEST_SIZE / sizeof(uint32_t)];
        };
    } MD5_DIGEST, MD5_BUFFER;

    void accelc_MD5_init(MD5_BUFFER* HashBuffer);

    void accelc_MD5_update(const void* srcBytes, size_t srcBytesLength,
                           MD5_BUFFER* HashBuffer);

    void accelc_MD5_final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength,
                          const MD5_BUFFER* HashBuffer, MD5_DIGEST* Hash);

    void accelc_MD5(const void* srcBytes, size_t srclen, MD5_DIGEST* Hash);

#if defined(__cplusplus)
}
#endif
