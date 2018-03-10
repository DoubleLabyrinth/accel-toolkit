#pragma once
#include "cryptolib/DES.h"
#include "../def.hpp"

namespace accelpp::crypto::symmetric {

    class DES {
    private:
        uint8_t ExpandedKey[16][6];
    public:

        static constexpr size_t KeyByteLength = 8;
        static constexpr size_t BlockSize = 8;

        struct Exception {
            class InvalidKey { };
        };

        void SetKey(const uint8_t (&srcKey)[KeyByteLength]) throw(Exception::InvalidKey) {
            int status = CRYPTO_DES_KeyExpansion(srcKey, ExpandedKey);
            if (status == CRYPTO_DES_INVALID_KEY)
                throw Exception::InvalidKey();
        }

        void Encrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            CRYPTO_DES_EncryptBlock(srcBytes.value, ExpandedKey);
        }

        void Decrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            CRYPTO_DES_DecryptBlock(srcBytes.value, ExpandedKey);
        }

        DES(const uint8_t (&srcKey)[KeyByteLength]) throw(Exception::InvalidKey) {
            SetKey(srcKey);
        }

        ~DES() {
            ClearKey();
        }

        void ClearKey() volatile {
            volatile uint32_t* p = reinterpret_cast<volatile uint32_t*>(ExpandedKey);
            for (int i = 0; i < sizeof(ExpandedKey) / sizeof(uint32_t); ++i)
                p[i] = 0;
        }
    };

}