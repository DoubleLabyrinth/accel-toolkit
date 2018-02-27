#pragma once
#include "cryptolib/TEA.h"

namespace accelpp::crypto::symmetric {

    class TEA {
    private:
        alignas(uint32_t) uint8_t Key[16];
    public:

        static constexpr size_t KeyByteLength = sizeof(Key);
        static constexpr size_t BlockSize = 8;

        void SetKey(const uint8_t (&srcKey)[16]) {
            reinterpret_cast<uint64_t*>(Key)[0] = reinterpret_cast<const uint64_t*>(srcKey)[0];
            reinterpret_cast<uint64_t*>(Key)[1] = reinterpret_cast<const uint64_t*>(srcKey)[1];
        }

        void Encrypt(uint8_t (&srcBytes)[BlockSize]) const {
            CRYPTO_TEA_EncryptBlock(srcBytes, Key);
        }

        void Decrypt(uint8_t (&srcBytes)[BlockSize]) const {
            CRYPTO_TEA_DecryptBlock(srcBytes, Key);
        }

        TEA(const uint8_t(&srcKey)[16]) {
            SetKey(srcKey);
        }

        ~TEA() {
            ClearKey();
        }

        void ClearKey() volatile {
            reinterpret_cast<volatile uint64_t*>(Key)[0] = 0;
            reinterpret_cast<volatile uint64_t*>(Key)[1] = 0;
        }
    };

}