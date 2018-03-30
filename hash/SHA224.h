/*********************************************************************
* Filename:   SHA224.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif
    
    void HASH_SHA224_Initialize(uint32_t HashBuffer[8]);

    void HASH_SHA224_MainCycle(const void* srcBytes, size_t srcBytesLength, 
                               uint32_t HashBuffer[8]);

    void HASH_SHA224_Final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength, 
                           uint32_t HashBuffer[8]);

#if defined(__cplusplus)
}
#endif
