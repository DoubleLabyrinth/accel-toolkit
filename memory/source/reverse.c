#include "../reverse.h"

#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__)
#include <x86intrin.h>
#define _byteswap_ushort __builtin_bswap16
#define _byteswap_ulong __builtin_bswap32
#define _byteswap_uint64 __builtin_bswap64
#endif

void accelc_memory_reverse(void* dst, size_t len) {
    if (len == 0 || len == 1) return;

    size_t u64_count = len / (2 * sizeof(uint64_t));

    uint64_t* dst_u64 = dst;
    uint64_t* dst_u64_r = (uint64_t*)((uint8_t*)dst + len) - 1;

    for (size_t i = 0; i < u64_count; ++i) {
        uint64_t temp = dst_u64[i];
        dst_u64[i] = _byteswap_uint64(*(dst_u64_r - i));
        *(dst_u64_r - i) = _byteswap_uint64(temp);
    }

    char u32_flag = len % (2 * sizeof(uint64_t)) >= 2 * sizeof(uint32_t) ? 1 : 0;
    char u16_flag = len % (2 * sizeof(uint32_t)) >= 2 * sizeof(uint16_t) ? 1 : 0;
    char u8_flag = len % (2 * sizeof(uint16_t)) >= 2 * sizeof(uint8_t) ? 1 : 0;

    uint32_t* dst_u32 = (uint32_t*)(dst_u64 + u64_count);
    uint32_t* dst_u32_r = (uint32_t*)(dst_u64_r + 1 - u64_count) - 1;

    uint16_t* dst_u16 = (uint16_t*)(dst_u32 + u32_flag);
    uint16_t* dst_u16_r = (uint16_t*)(dst_u32_r + 1 - u32_flag) - 1;

    uint8_t* dst_u8 = (uint8_t*)(dst_u16 + u16_flag);
    uint8_t* dst_u8_r = (uint8_t*)(dst_u16_r + 1 - u16_flag) - 1;

    if (u32_flag) {
        uint32_t temp = *dst_u32;
        *dst_u32 = _byteswap_ulong(*dst_u32_r);
        *dst_u32_r = _byteswap_ulong(temp);
    }

    if (u16_flag) {
        uint16_t temp = *dst_u16;
        *dst_u16 = _byteswap_ushort(*dst_u16_r);
        *dst_u16_r = _byteswap_ushort(temp);
    }

    if (u8_flag) {
        uint8_t temp = *dst_u8;
        *dst_u8 = *dst_u8_r;
        *dst_u8_r = temp;
    }
}