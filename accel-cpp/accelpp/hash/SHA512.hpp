#pragma once
#include "hashlib/SHA512.h"
#include <memory.h>

#pragma comment(lib, "accel-c.lib")

namespace accelpp::hash {

    class SHA512 {
    public:

        static constexpr size_t BlockSize = 128;
        static constexpr size_t DigestSize = 64;

        struct HashValue {
            union {
                uint8_t bytes[64];
                uint32_t dword[16];
                uint64_t qword[8];
            };

            bool operator==(const HashValue& other) {
                return memcmp(bytes, other.bytes, sizeof(bytes)) == 0;
            }

            bool operator!=(const HashValue& other) {
                return memcmp(bytes, other.bytes, sizeof(bytes)) != 0;
            }
        };

        SHA512() : LeftBytes({ }), TotalBytesLength(0) {
            HASH_SHA512_Initialize(HashBuffer.qword);
        }

        SHA512(const void* src, size_t src_length) : TotalBytesLength(src_length) {
            size_t BlockCount = src_length / BlockSize;
            LeftBytes.Length = static_cast<uint8_t>(src_length % BlockSize);

            for (uint8_t i = 0; i < LeftBytes.Length; ++i)
                LeftBytes.Bytes[i] = reinterpret_cast<const uint8_t(*)[BlockSize]>(src)[BlockCount][i];

            HASH_SHA512_Initialize(HashBuffer.qword);
            if (BlockCount)
                HASH_SHA512_MainCycle(reinterpret_cast<const uint8_t*>(src), src_length, HashBuffer.qword);
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
            HASH_SHA512_MainCycle(reinterpret_cast<const uint8_t*>(&LeftBytes), sizeof(LeftBytes), HashBuffer.qword);
            memset(&LeftBytes, 0, sizeof(LeftBytes));


            size_t BlockCount = src_length / BlockSize;
            LeftBytes.Length = static_cast<uint8_t>(src_length % BlockSize);
            for (uint8_t i = 0; i < LeftBytes.Length; ++i)
                LeftBytes.Bytes[i] = reinterpret_cast<const uint8_t(*)[BlockSize]>(start_ptr)[BlockCount][i];

            if (BlockCount)
                HASH_SHA512_MainCycle(reinterpret_cast<const uint8_t*>(src), src_length, HashBuffer.qword);
        }

        HashValue Digest() {
            HashValue ret = HashBuffer;
            HASH_SHA512_Final(LeftBytes.Bytes, LeftBytes.Length, TotalBytesLength, ret.qword);
            return ret;
        }

        void Reset() {
            HASH_SHA512_Initialize(HashBuffer.qword);
            memset(&LeftBytes, 0, sizeof(LeftBytes));
            TotalBytesLength = 0;
        }

    private:
        HashValue HashBuffer;

        struct {
            uint8_t Bytes[127];
            uint8_t Length;
        } LeftBytes;

        uint64_t TotalBytesLength;
    };

}