#pragma once
#include "hashlib/SHA256.h"
#include <memory.h>

namespace accelpp::hash {

    class SHA256 {
    public:

        static constexpr size_t BlockSize = 64;
        static constexpr size_t DigestSize = 32;

        struct HashValue {
            union {
                uint8_t bytes[32];
                uint32_t dword[8];
            };

            bool operator==(const HashValue& other) {
                return memcmp(bytes, other.bytes, sizeof(bytes)) == 0;
            }

            bool operator!=(const HashValue& other) {
                return memcmp(bytes, other.bytes, sizeof(bytes)) != 0;
            }
        };

        SHA256() : LeftBytes({ }), TotalBytesLength(0) {
            HASH_SHA256_Initialize(HashBuffer.dword);
        }

        SHA256(const void* src, size_t src_length) : TotalBytesLength(src_length) {
            size_t BlockCount = src_length / BlockSize;
            LeftBytes.Length = static_cast<uint8_t>(src_length % BlockSize);

            for (uint8_t i = 0; i < LeftBytes.Length; ++i)
                LeftBytes.Bytes[i] = reinterpret_cast<const uint8_t(*)[BlockSize]>(src)[BlockCount][i];

            HASH_SHA256_Initialize(HashBuffer.dword);
            if (BlockCount)
                HASH_SHA256_MainCycle(reinterpret_cast<const uint8_t*>(src), src_length, HashBuffer.dword);
        }

        void Update(const void* src, size_t src_length) {
            TotalBytesLength += src_length;

            const uint8_t* start_ptr = reinterpret_cast<const uint8_t*>(src);
            while (LeftBytes.Length != BlockSize - 1 && src_length != 0) {
                LeftBytes.Bytes[LeftBytes.Length] = *start_ptr;
                ++LeftBytes.Length;
                ++start_ptr;
                --src_length;
            }

            if (src_length == 0)
                return;

            LeftBytes.Length = *start_ptr;
            ++start_ptr;
            --src_length;
            HASH_SHA256_MainCycle(reinterpret_cast<const uint8_t*>(&LeftBytes), sizeof(LeftBytes), HashBuffer.dword);
            memset(&LeftBytes, 0, sizeof(LeftBytes));


            size_t BlockCount = src_length / BlockSize;
            LeftBytes.Length = static_cast<uint8_t>(src_length % BlockSize);
            for (uint8_t i = 0; i < LeftBytes.Length; ++i)
                LeftBytes.Bytes[i] = reinterpret_cast<const uint8_t(*)[BlockSize]>(start_ptr)[BlockCount][i];

            if (BlockCount)
                HASH_SHA256_MainCycle(reinterpret_cast<const uint8_t*>(src), src_length, HashBuffer.dword);
        }

        HashValue Digest() {
            HashValue ret = HashBuffer;
            HASH_SHA256_Final(LeftBytes.Bytes, LeftBytes.Length, TotalBytesLength, ret.dword);
            return ret;
        }

        void Reset() {
            HASH_SHA256_Initialize(HashBuffer.dword);
            memset(&LeftBytes, 0, sizeof(LeftBytes));
            TotalBytesLength = 0;
        }

    private:
        HashValue HashBuffer;

        struct {
            uint8_t Bytes[63];
            uint8_t Length;
        } LeftBytes;

        uint64_t TotalBytesLength;
    };

}