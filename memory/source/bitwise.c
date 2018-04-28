#include "../bitwise.h"

#define BinaryOperation_Start(op_for_basetype)      \
    size_t u64_len = len / sizeof(uint64_t);        \
    const uint64_t* src_u64 = src;                  \
    uint64_t* dst_u64 = dst;                        \
                                                    \
    for (size_t i = 0; i < u64_len; ++i)            \
        dst_u64[i] op_for_basetype src_u64[i];

#define BinaryOperation_End(op_for_basetype)                                \
    char u32_flag = len % sizeof(uint64_t) >= sizeof(uint32_t) ? 1 : 0;     \
    char u16_flag = len % sizeof(uint32_t) >= sizeof(uint16_t) ? 1 : 0;     \
    char u8_flag = len % sizeof(uint16_t) >= sizeof(uint8_t) ? 1 : 0;       \
                                                                            \
    uint32_t* dst_u32 = (uint32_t*)(dst_u64 + u64_len);                     \
    const uint32_t* src_u32 = (const uint32_t*)(src_u64 + u64_len);         \
                                                                            \
    uint16_t* dst_u16 = (uint16_t*)(dst_u32 + u32_flag);                    \
    const uint16_t* src_u16 = (const uint16_t*)(src_u32 + u32_flag);        \
                                                                            \
    uint8_t* dst_u8 = (uint8_t*)(dst_u16 + u16_flag);                       \
    const uint8_t* src_u8 = (const uint8_t*)(src_u16 + u16_flag);           \
                                                                            \
    if (u32_flag)                                                           \
        *dst_u32 op_for_basetype *src_u32;                                  \
                                                                            \
    if (u16_flag)                                                           \
        *dst_u16 op_for_basetype *src_u16;                                  \
                                                                            \
    if (u8_flag)                                                            \
        *dst_u8 op_for_basetype *src_u8;

void accelc_memory_xor(void* dst, const void* src, size_t len) {
    BinaryOperation_Start(^=)
    BinaryOperation_End(^=)
}

void accelc_memory_and(void* dst, const void* src, size_t len) {
    BinaryOperation_Start(&=)
    BinaryOperation_End(&=)
}

void accelc_memory_or(void* dst, const void* src, size_t len) {
    BinaryOperation_Start(|=)
    BinaryOperation_End(|=)
}

void accelc_memory_not(void* dst, size_t len) {
    size_t u64_len = len / sizeof(uint64_t);
    uint64_t* dst_u64 = dst;

    for (size_t i = 0; i < u64_len; ++i)
        dst_u64[i] = ~(dst_u64[i]);

    char u32_flag = len % sizeof(uint64_t) >= sizeof(uint32_t) ? 1 : 0;
    char u16_flag = len % sizeof(uint32_t) >= sizeof(uint16_t) ? 1 : 0;
    char u8_flag = len % sizeof(uint16_t) >= sizeof(uint8_t) ? 1 : 0;

    uint32_t* const dst_u32 = (uint32_t*)(dst_u64 + u64_len);
    uint16_t* const dst_u16 = (uint16_t*)(dst_u32 + u32_flag);
    uint8_t* const dst_u8 = (uint8_t*)(dst_u16 + u16_flag);

    if (u32_flag)
        *dst_u32 = ~(*dst_u32);

    if (u16_flag)
        *dst_u16 = ~(*dst_u16);

    if (u8_flag)
        *dst_u8 = ~(*dst_u8);
}
