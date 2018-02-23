#pragma once
#include "hashlib/MD2.h"
#include <memory.h>

#pragma comment(lib, "hashlib.lib")

namespace accelpp::hash {

    class MD2 {
    public:
        
        static constexpr size_t BlockSize = 16;
        static constexpr size_t DigestSize = 16;

        struct HashValue {
            union {
                uint8_t bytes[16];
                uint32_t dword[4];
            };

            bool operator==(const HashValue& other) {
                return memcmp(bytes, other.bytes, sizeof(bytes)) == 0;
            }

            bool operator!=(const HashValue& other) {
                return memcmp(bytes, other.bytes, sizeof(bytes)) != 0;
            }
        };

        MD2(const void* src, size_t src_length) {
            HASH_MD2_Initialize(_HashValue.dword);
            HASH_MD2_MainCycle(src, src_length, _HashValue.dword);
            HASH_MD2_Final(src, src_length, _HashValue.dword);
        }

        // MD2 does not support Update.
//         void Update(const void* src, size_t src_length) {
//
//         }

        HashValue Digest() {
            return _HashValue;
        }

    private:
        HashValue _HashValue;

    };

}