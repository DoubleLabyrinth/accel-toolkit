#pragma once
#include "hashlib/SHA384.h"
#include <memory.h>

namespace accelpp::hash {

    class SHA384 {
    public:

        static constexpr size_t BlockSize = 128;
        static constexpr size_t DigestSize = 48;

        struct HashValue {
            union {
                uint8_t bytes[48];
                uint32_t dword[12];
                uint64_t qword[6];
            };

            bool operator==(const HashValue& other) {
                return memcmp(bytes, other.bytes, sizeof(bytes)) == 0;
            }

            bool operator!=(const HashValue& other) {
                return memcmp(bytes, other.bytes, sizeof(bytes)) != 0;
            }
        };

        SHA384() : LeftBytes({ }), TotalBytesLength(0) {
            HASH_SHA384_Initialize(HashBuffer);
        }

        SHA384(const void* src, size_t src_length) : TotalBytesLength(src_length) {
            size_t BlockCount = src_length / BlockSize;
            LeftBytes.Length = static_cast<uint8_t>(src_length % BlockSize);

            for (uint8_t i = 0; i < LeftBytes.Length; ++i)
                LeftBytes.Bytes[i] = reinterpret_cast<const uint8_t(*)[BlockSize]>(src)[BlockCount][i];

            HASH_SHA384_Initialize(HashBuffer);
            if (BlockCount)
                HASH_SHA384_MainCycle(reinterpret_cast<const uint8_t*>(src), src_length, HashBuffer);
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
            HASH_SHA384_MainCycle(reinterpret_cast<const uint8_t*>(&LeftBytes), sizeof(LeftBytes), HashBuffer);
            memset(&LeftBytes, 0, sizeof(LeftBytes));


            size_t BlockCount = src_length / BlockSize;
            LeftBytes.Length = static_cast<uint8_t>(src_length % BlockSize);
            for (uint8_t i = 0; i < LeftBytes.Length; ++i)
                LeftBytes.Bytes[i] = reinterpret_cast<const uint8_t(*)[BlockSize]>(start_ptr)[BlockCount][i];

            if (BlockCount)
                HASH_SHA384_MainCycle(reinterpret_cast<const uint8_t*>(src), src_length, HashBuffer);
        }

        HashValue Digest() {
            uint64_t HashResult[8];
            memcpy(HashResult, HashBuffer, sizeof(HashBuffer));

            HASH_SHA384_Final(LeftBytes.Bytes, LeftBytes.Length, TotalBytesLength, HashResult);

            HashValue ret;
            memcpy(ret.bytes, HashResult, sizeof(ret.bytes));
            return ret;
        }

        void Reset() {
            HASH_SHA384_Initialize(HashBuffer);
            memset(&LeftBytes, 0, sizeof(LeftBytes));
            TotalBytesLength = 0;
        }

    private:
        uint64_t HashBuffer[8];

        struct {
            uint8_t Bytes[127];
            uint8_t Length;
        } LeftBytes;

        uint64_t TotalBytesLength;
    };

}