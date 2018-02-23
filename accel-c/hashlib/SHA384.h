/*********************************************************************
* Filename:   SHA384.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

    void HASH_SHA384_Initialize(uint64_t HashBuffer[8]);

    void HASH_SHA384_MainCycle(const void* srcBytes, size_t srcBytesLength,
                               uint64_t HashBuffer[8]);

    void HASH_SHA384_Final(const void* LeftBytes, size_t LeftBytesLength, uint64_t TotalBytesLength,
                           uint64_t HashBuffer[8]);

#if defined(__cplusplus)
}
#endif
