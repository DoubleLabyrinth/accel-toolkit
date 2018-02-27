#pragma once
#include "cryptolib/RC2.h"

namespace accelpp::crypto::symmetric {

    template<size_t _KeyLength, size_t _eftKeyBitLength>
    class RC2 {
        static_assert(_KeyLength <= CRYPTO_RC2_MAX_KEY_LENGTH, "RC2: Key is too long.");
        static_assert(_KeyLength * 8 >= _eftKeyBitLength, "RC2: Illegal eftKeyBitLength.");
    private:
        uint16_t ExpandedKey[64];
    public:

        static constexpr size_t eftKeyBitLength = _eftKeyBitLength;
        static constexpr size_t KeyByteLength = _KeyLength;
        static constexpr size_t BlockSize = 8;

        void SetKey(const uint8_t (&srcKey)[_KeyLength]) {
            CRYPTO_RC2_KeyExpansion(srcKey, _KeyLength, _eftKeyBitLength, ExpandedKey);
        }

        void Encrypt(uint8_t (&srcBytes)[BlockSize]) const {
            CRYPTO_RC2_EncryptBlock(srcBytes, ExpandedKey);
        }

        void Decrypt(uint8_t (&srcBytes)[BlockSize]) const {
            CRYPTO_RC2_DecryptBlock(srcBytes, ExpandedKey);
        }

        RC2(const uint8_t (&srcKey)[_KeyLength]) {
            SetKey(srcKey);
        }

        ~RC2() {
            ClearKey();
        }

        void ClearKey() volatile {
            volatile uint32_t* p = reinterpret_cast<volatile uint32_t*>(ExpandedKey);
            for (int i = 0; i < sizeof(ExpandedKey) / sizeof(uint32_t); ++i)
                p[i] = 0;
        }

    };

}