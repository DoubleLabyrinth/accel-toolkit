#pragma once
#include "cryptolib/XTEA.h"

namespace accelpp::crypto::symmetric {

    template<uint32_t _Round>
    class XTEA {
    private:
        alignas(uint32_t) uint8_t Key[16];
    public:

        static constexpr size_t KeyByteLength = sizeof(Key);
        static constexpr size_t BlockSize = 8;
        static constexpr size_t RoundCount = _Round;

        void SetKey(const uint8_t (&srcKey)[16]) {
            reinterpret_cast<uint64_t*>(Key)[0] = reinterpret_cast<const uint64_t*>(srcKey)[0];
            reinterpret_cast<uint64_t*>(Key)[1] = reinterpret_cast<const uint64_t*>(srcKey)[1];
        }

        void Encrypt(uint8_t (&srcBytes)[BlockSize]) const {
            CRYPTO_XTEA_EncryptBlock(srcBytes, Key, _Round);
        }

        void Decrypt(uint8_t (&srcBytes)[BlockSize]) const {
            CRYPTO_XTEA_DecryptBlock(srcBytes, Key, _Round);
        }

        XTEA(const uint8_t (&srcKey)[16]) {
            SetKey(srcKey);
        }

        ~XTEA() {
            ClearKey();
        }

        void ClearKey() volatile {
            reinterpret_cast<volatile uint64_t*>(Key)[0] = 0;
            reinterpret_cast<volatile uint64_t*>(Key)[1] = 0;
        }
    };

}
