#pragma once
#include "cryptolib/CAST-128.h"
#include "../def.hpp"

namespace accelpp::crypto::symmetric {

    template<uint8_t _KeyLength>
    class CAST128 {
        static_assert(CRYPTO_CAST128_MIN_KEY_LENGTH <= _KeyLength && _KeyLength <= CRYPTO_CAST128_MAX_KEY_LENGTH, 
                      "CAST128: Key is too short or too long.");
    private:
        uint32_t Km[16];
        uint32_t Kr[16];
    public:

        static constexpr size_t KeyByteLength = _KeyLength;
        static constexpr size_t BlockSize = 8;

        void SetKey(const uint8_t (&srcKey)[_KeyLength]) {
            CRYPTO_CAST128_KeyExpansion(srcKey, _KeyLength, Km, Kr);
        }

        void Encrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            CRYPTO_CAST128_EncryptBlock(srcBytes.value, Km, Kr, _KeyLength);
        }

        void Decrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            CRYPTO_CAST128_DecryptBlock(srcBytes.value, Km, Kr, _KeyLength);
        }

        CAST128(const uint8_t (&srcKey)[_KeyLength]) {
            SetKey(srcKey);
        }

        ~CAST128() {
            ClearKey();
        }

        void ClearKey() volatile {
            volatile uint32_t* p = Km;
            for (int i = 0; i < sizeof(Km) / sizeof(uint32_t); ++i)
                p[i] = 0;
            p = Kr;
            for (int i = 0; i < sizeof(Kr) / sizeof(uint32_t); ++i)
                p[i] = 0;
        }
    };

}