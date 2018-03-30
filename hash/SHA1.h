/*********************************************************************
* Filename:   SHA1.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif
    
    void HASH_SHA1_Initialize(uint32_t HashBuffer[5]);

    void HASH_SHA1_MainCycle(const void* srcBytes, size_t srcBytesLength, 
                             uint32_t HashBuffer[5]);

    void HASH_SHA1_Final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength, 
                         uint32_t HashBuffer[5]);

#if defined(__cplusplus)
}
#endif