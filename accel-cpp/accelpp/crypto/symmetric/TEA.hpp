#pragma once
#include "cryptolib/TEA.h"
#include "../def.hpp"

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

        void Encrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            CRYPTO_TEA_EncryptBlock(srcBytes.value, Key);
        }

        void Decrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            CRYPTO_TEA_DecryptBlock(srcBytes.value, Key);
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