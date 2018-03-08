#pragma once
#include "cryptolib/Blowfish.h"
#include "../utility.hpp"

namespace accelpp::crypto::symmetric {

    template<size_t _KeyLength, bool _UseBigEndian>
    class Blowfish {
        static_assert(_KeyLength <= CRYPTO_BLOWFISH_MAX_KEY_LENGTH, "Blowfish: Key is too long.");
    private:
        uint32_t SubKey[18];
        uint32_t SBox[4][256];
    public:

        static constexpr size_t KeyByteLength = _KeyLength;
        static constexpr size_t BlockSize = 8;

        void SetKey(const uint8_t (&srcKey)[_KeyLength], bool bigEndian = false) {
            CRYPTO_Blowfish_KeyExpansion(srcKey, _KeyLength, SubKey, SBox);
        }

        void Encrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            CRYPTO_Blowfish_EncryptBlock(srcBytes.value, SubKey, SBox, 
                                         _UseBigEndian ? CRYPTO_BLOWFISH_BIG_ENDIAN : CRYPTO_BLOWFISH_LITTLE_ENDIAN);
        }

        void Decrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            CRYPTO_Blowfish_DecryptBlock(srcBytes.value, SubKey, SBox,
                                         _UseBigEndian ? CRYPTO_BLOWFISH_BIG_ENDIAN : CRYPTO_BLOWFISH_LITTLE_ENDIAN);
        }

        Blowfish(const uint8_t (&srcKey)[_KeyLength], bool bigEndian = false) {
            SetKey(srcKey);
        }

        ~Blowfish() {
            ClearKey();
        }

        void ClearKey() volatile {
            volatile uint32_t* p = SubKey;
            for (int i = 0; i < sizeof(SubKey) / sizeof(uint32_t); ++i)
                p[i] = 0;
            p = reinterpret_cast<volatile uint32_t*>(SBox);
            for (int i = 0; i < sizeof(SBox) / sizeof(uint32_t); ++i)
                p[i] = 0;
        }
    };

}