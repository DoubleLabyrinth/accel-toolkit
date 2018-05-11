#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif
    
#define RIPEME160_DIGEST_SIZE 20
    
    typedef struct _MD4_DIGEST {
        union {
            uint8_t byte[RIPEME160_DIGEST_SIZE];
            uint32_t dword[RIPEME160_DIGEST_SIZE / sizeof(uint32_t)];
        };
    } RIPEMD160_DIGEST, RIPEMD160_BUFFER;
    
    void accelc_RIPEMD160_init(RIPEMD160_BUFFER* HashBuffer);
    
    void accelc_RIPEMD160_update(const void* srcBytes, size_t srcBytesLength,
                                 RIPEMD160_BUFFER* HashBuffer);
    
    void accelc_RIPEMD160_final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength,
                                const RIPEMD160_BUFFER* HashBuffer, RIPEMD160_DIGEST* Hash);
    
    void accelc_RIPEMD160(const void* srcBytes, size_t srclen, RIPEMD160_DIGEST* Hash);
    
#if defined(__cplusplus)
}
#endif
