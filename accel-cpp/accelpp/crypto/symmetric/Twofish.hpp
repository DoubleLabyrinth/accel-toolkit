#pragma once
#include "cryptolib/Twofish128.h"
#include "cryptolib/Twofish192.h"
#include "cryptolib/Twofish256.h"
#include "../def.hpp"

namespace accelpp::crypto::symmetric {

    template<size_t _KeyBits>
    class Twofish {
        static_assert(_KeyBits == 128 || _KeyBits == 192 || _KeyBits == 256, "Twofish: Not supported.");
    private:
        uint32_t ExpandedKey[40];
        uint32_t S[_KeyBits / 64];
    public:

        static constexpr size_t KeyByteLength = _KeyBits / 8;
        static constexpr size_t BlockSize = 16;

        void SetKey(const uint8_t (&srcKey)[KeyByteLength]) {
            switch (_KeyBits) {
                case 128:
                    CRYPTO_Twofish128_KeyExpansion(srcKey, ExpandedKey, S);
                    break;
                case 192:
                    CRYPTO_Twofish192_KeyExpansion(srcKey, ExpandedKey, S);
                    break;
                case 256:
                    CRYPTO_Twofish256_KeyExpansion(srcKey, ExpandedKey, S);
                    break;
                default:
                    break;  // nothing
            }
        }

        void Encrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            switch (_KeyBits) {
                case 128:
                    CRYPTO_Twofish128_EncryptBlock(srcBytes.value, ExpandedKey, S);
                    break;
                case 192:
                    CRYPTO_Twofish192_EncryptBlock(srcBytes.value, ExpandedKey, S);
                    break;
                case 256:
                    CRYPTO_Twofish256_EncryptBlock(srcBytes.value, ExpandedKey, S);
                    break;
                default:
                    break;  // nothing
            }
        }

        void Decrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            switch (_KeyBits) {
                case 128:
                    CRYPTO_Twofish128_DecryptBlock(srcBytes.value, ExpandedKey, S);
                    break;
                case 192:
                    CRYPTO_Twofish192_DecryptBlock(srcBytes.value, ExpandedKey, S);
                    break;
                case 256:
                    CRYPTO_Twofish256_DecryptBlock(srcBytes.value, ExpandedKey, S);
                    break;
                default:
                    break;  // nothing
            }
        }

        Twofish(const uint8_t (&srcKey)[KeyByteLength]) {
            SetKey(srcKey);
        }

        ~Twofish() {
            ClearKey();
        }

        void ClearKey() volatile {
            volatile uint32_t* p = ExpandedKey;
            for (int i = 0; i < sizeof(ExpandedKey) / sizeof(uint32_t); ++i)
                p[i] = 0;
            p = S;
            for (int i = 0; i < sizeof(S) / sizeof(uint32_t); ++i)
                p[i] = 0;
        }
    };

}