#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define SHA224_DIGEST_SIZE 28
#define SHA256_DIGEST_SIZE 32
#define SHA384_DIGEST_SIZE 48
#define SHA512_DIGEST_SIZE 64

    typedef struct _SHA224_DIGEST {
        union {
            uint8_t byte[SHA224_DIGEST_SIZE];
            uint32_t dword[SHA224_DIGEST_SIZE / sizeof(uint32_t)];
        };
    } SHA224_DIGEST;

    typedef struct _SHA224_BUFFER {
        union {
            uint8_t byte[32];
            uint32_t dword[32 / sizeof(uint32_t)];
        };
    } SHA224_BUFFER;

    typedef struct _SHA256_DIGEST {
        union {
            uint8_t byte[SHA256_DIGEST_SIZE];
            uint32_t dword[SHA256_DIGEST_SIZE / sizeof(uint32_t)];
        };
    } SHA256_DIGEST, SHA256_BUFFER;

    typedef struct _SHA384_BUFFER {
        union {
            uint8_t byte[64];
            uint32_t dword[64 / sizeof(uint32_t)];
            uint64_t qword[64 / sizeof(uint64_t)];
        };
    } SHA384_BUFFER;

    typedef struct _SHA384_DIGEST {
        union {
            uint8_t byte[SHA384_DIGEST_SIZE];
            uint32_t dword[SHA384_DIGEST_SIZE / sizeof(uint32_t)];
            uint64_t qword[SHA384_DIGEST_SIZE / sizeof(uint64_t)];
        };
    } SHA384_DIGEST;

    typedef struct _SHA512_DIGEST {
        union {
            uint8_t byte[SHA512_DIGEST_SIZE];
            uint32_t dword[SHA512_DIGEST_SIZE / sizeof(uint32_t)];
            uint64_t qword[SHA512_DIGEST_SIZE / sizeof(uint64_t)];
        };
    } SHA512_DIGEST, SHA512_BUFFER;

    //=========================
    //  SHA224
    //=========================

    void accelc_SHA224_init(SHA224_BUFFER* HashBuffer);

    void accelc_SHA224_update(const void* srcBytes, size_t srcBytesLength, SHA224_BUFFER* HashBuffer);

    void accelc_SHA224_final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength,
                             const SHA224_BUFFER* __restrict HashBuffer,
                             SHA224_DIGEST* __restrict Hash);

    void accelc_SHA224(const void* srcBytes, size_t srclen, SHA224_DIGEST* Hash);

    //=========================
    //  SHA256
    //=========================

    void accelc_SHA256_init(SHA256_BUFFER* HashBuffer);

    void accelc_SHA256_update(const void* srcBytes, size_t srcBytesLength,
                              SHA256_BUFFER* HashBuffer);

    void accelc_SHA256_final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength,
                             const SHA256_BUFFER* HashBuffer, SHA256_DIGEST* Hash);

    void accelc_SHA256(const void* srcBytes, size_t srclen, SHA256_DIGEST* Hash);

    //=========================
    //  SHA384
    //=========================

    void accelc_SHA384_init(SHA384_BUFFER* HashBuffer);

    void accelc_SHA384_update(const void* srcBytes, size_t srcBytesLength, SHA384_BUFFER* HashBuffer);

    void accelc_SHA384_final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength,
                             const SHA384_BUFFER* __restrict HashBuffer, SHA384_DIGEST* __restrict Hash);

    void accelc_SHA384(const void* srcBytes, size_t srclen, SHA384_DIGEST* Hash);

    //=========================
    //  SHA512
    //=========================

    void accelc_SHA512_init(SHA512_BUFFER* HashBuffer);

    void accelc_SHA512_update(const void* srcBytes, size_t srcBytesLength,
                              SHA512_BUFFER* HashBuffer);

    void accelc_SHA512_final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength,
                             const SHA512_BUFFER* HashBuffer, SHA512_DIGEST* Hash);

    void accelc_SHA512(const void* srcBytes, size_t srclen, SHA512_DIGEST* Hash);

#if defined(__cplusplus)
}
#endif
