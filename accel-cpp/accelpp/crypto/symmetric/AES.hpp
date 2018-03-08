#pragma once
#include "cryptolib/AES128.h"
#include "cryptolib/AES192.h"
#include "cryptolib/AES256.h"
#include "../utility.hpp"

namespace accelpp::crypto::symmetric {

    template<size_t _KeyBits, bool aesni_enable>
    class AES;

    template<size_t _KeyBits>
    class AES<_KeyBits, false> {
        static_assert(_KeyBits == 128 || _KeyBits == 192 || _KeyBits == 256, "AES: Not supported.");
    private:
        uint32_t ExpandedKey[28 + _KeyBits / 8];
    public:

        static constexpr size_t KeyByteLength = _KeyBits / 8;
        static constexpr size_t BlockSize = 16;

        void SetKey(const uint8_t (&srcKey)[KeyByteLength]) {
            switch (_KeyBits) {
                case 128:
                    CRYPTO_AES128_KeyExpansion(srcKey, ExpandedKey);
                    break;
                case 192:
                    CRYPTO_AES192_KeyExpansion(srcKey, ExpandedKey);
                    break;
                case 256:
                    CRYPTO_AES256_KeyExpansion(srcKey, ExpandedKey);
                    break;
                default:
                    break;  // nothing
            }
        }

        void Encrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            switch (_KeyBits) {
                case 128:
                    CRYPTO_AES128_EncryptBlock(srcBytes.value, ExpandedKey);
                    break;
                case 192:
                    CRYPTO_AES192_EncryptBlock(srcBytes.value, ExpandedKey);
                    break;
                case 256:
                    CRYPTO_AES256_EncryptBlock(srcBytes.value, ExpandedKey);
                    break;
                default:
                    break;  // nothing
            }
        }

        void Decrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            switch (_KeyBits) {
                case 128:
                    CRYPTO_AES128_DecryptBlock(srcBytes.value, ExpandedKey);
                    break;
                case 192:
                    CRYPTO_AES192_DecryptBlock(srcBytes.value, ExpandedKey);
                    break;
                case 256:
                    CRYPTO_AES256_DecryptBlock(srcBytes.value, ExpandedKey);
                    break;
                default:
                    break;  // nothing
            }
        }

        AES(const uint8_t (&srcKey)[KeyByteLength]) {
            SetKey(srcKey);
        }

        ~AES() {
            ClearKey();
        }

        void ClearKey() volatile {
            volatile uint32_t* p = ExpandedKey;
            for (int i = 0; i < 28 + _KeyBits / 8; ++i)
                p[i] = 0;
        }
    };

    template<size_t _KeyBits>
    class AES<_KeyBits, true> {
        static_assert(_KeyBits == 128 || _KeyBits == 192 || _KeyBits == 256, "AES: Not supported.");
    private:
        uint32_t ExpandedKey[28 + _KeyBits / 8];
        uint32_t InverseExpandedKey[28 + _KeyBits / 8];
    public:

        static constexpr size_t KeyByteLength = _KeyBits / 8;
        static constexpr size_t BlockSize = 16;

        void SetKey(const uint8_t (&srcKey)[KeyByteLength]) {
            switch (_KeyBits) {
                case 128:
                    CRYPTO_AES128_KeyExpansion_AESNI(srcKey, ExpandedKey);
                    CRYPTO_AES128_GetInvExpandedKey_AESNI(ExpandedKey, InverseExpandedKey);
                    break;
                case 192:
                    CRYPTO_AES192_KeyExpansion_AESNI_AVX2(srcKey, ExpandedKey);
                    CRYPTO_AES192_GetInvExpandedKey_AESNI(ExpandedKey, InverseExpandedKey);
                    break;
                case 256:
                    CRYPTO_AES256_KeyExpansion_AESNI(srcKey, ExpandedKey);
                    CRYPTO_AES256_GetInvExpandedKey_AESNI(ExpandedKey, InverseExpandedKey);
                    break;
                default:
                    break;  // nothing
            }
        }

        void Encrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            switch (_KeyBits) {
                case 128:
                    CRYPTO_AES128_EncryptBlock_AESNI(srcBytes.value, ExpandedKey);
                    break;
                case 192:
                    CRYPTO_AES192_EncryptBlock_AESNI(srcBytes.value, ExpandedKey);
                    break;
                case 256:
                    CRYPTO_AES256_EncryptBlock_AESNI(srcBytes.value, ExpandedKey);
                    break;
                default:
                    break;  // nothing
            }
        }

        void Decrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            switch (_KeyBits) {
                case 128:
                    CRYPTO_AES128_DecryptBlock_AESNI_Fast(srcBytes.value, InverseExpandedKey);
                    break;
                case 192:
                    CRYPTO_AES192_DecryptBlock_AESNI_Fast(srcBytes.value, InverseExpandedKey);
                    break;
                case 256:
                    CRYPTO_AES256_DecryptBlock_AESNI_Fast(srcBytes.value, InverseExpandedKey);
                    break;
                default:
                    break;  // nothing
            }
        }

        AES(const uint8_t (&srcKey)[KeyByteLength]) {
            SetKey(srcKey);
        }

        ~AES() {
            ClearKey();
        }

        void ClearKey() volatile {
            volatile uint32_t* p = ExpandedKey;
            for (int i = 0; i < 28 + _KeyBits / 8; ++i)
                p[i] = 0;
            p = InverseExpandedKey;
            for (int i = 0; i < 28 + _KeyBits / 8; ++i)
                p[i] = 0;
        }
    };

    typedef AES<128, false> ASE128;
    typedef AES<192, false> ASE192;
    typedef AES<256, false> ASE256;

    typedef AES<128, true> AES128_Fast;
    typedef AES<192, true> ASE192_Fast;
    typedef AES<256, true> ASE256_Fast;

}
