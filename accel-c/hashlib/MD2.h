/*********************************************************************
* Filename:   MD2.h
* Author:     Aiyu Xiao (xiao_ai_yu@live.cn)
*********************************************************************/
#pragma once
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

    void HASH_MD2_Initialize(uint32_t HashBuffer[4]);

    void HASH_MD2_MainCycle(const void* srcBytes, size_t srcBytesLength,
                            uint32_t HashBuffer[4]);

    void HASH_MD2_Final(const void* srcBytes, size_t srcBytesLength,
                        uint32_t HashBuffer[4]);

    //-----------------
    //  SSE2 Enable
    //-----------------

    void HASH_MD2_Initialize_SSE2(uint32_t HashBuffer[4]);

    void HASH_MD2_MainCycle_SSE2(const void* srcBytes, size_t srcBytesLength,
                                 uint32_t HashBuffer[4]);

#if defined(__cplusplus)
}
#endif