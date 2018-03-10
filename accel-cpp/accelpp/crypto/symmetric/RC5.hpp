#pragma once
#include "cryptolib/RC5-16.h"
#include "cryptolib/RC5-32.h"
#include "cryptolib/RC5-64.h"
#include "../def.hpp"

namespace accelpp::crypto::symmetric {

    // _WordBitLength can be 16, 32 and 64.
    // _KeyLength can vary from 1 to 255
    // _Round can vary from 0 to 255
    template<size_t _WordBitLength, size_t _KeyLength, size_t _Round>
    class RC5 {
        static_assert(_WordBitLength == 16 || _WordBitLength == 32 || _WordBitLength == 64, "RC5: Not-supported w.");
        static_assert(_KeyLength <= CRYPTO_RC5_16_MAX_KEY_LENGTH, "RC5: Key is too long.");
        static_assert(_Round <= 0xFF, "RC5: Not-supported r.");
    private:
        template<size_t w>
        struct uint2w_t;

        template<>
        struct uint2w_t<16> {
            typedef uint16_t Type;
        };

        template<>
        struct uint2w_t<32> {
            typedef uint32_t Type;
        };

        template<>
        struct uint2w_t<64> {
            typedef uint64_t Type;
        };

        uint2w_t<_WordBitLength>::Type ExpandedKey[2 * (_Round + 1)];

    public:

        static constexpr size_t KeyByteLength = _KeyLength;
        static constexpr size_t BlockSize = (2 * _WordBitLength) / 8;
        static constexpr RoundCount = _Round;

        void SetKey(const uint8_t (&srcKey)[_KeyLength]) {
            switch (_WordBitLength) {
                case 16:
                    CRYPTO_RC5_16_KeyExpansion(srcKey, _KeyLength, _Round, ExpandedKey, sizeof(ExpandedKey));
                    break;
                case 32:
                    CRYPTO_RC5_32_KeyExpansion(srcKey, _KeyLength, _Round, ExpandedKey, sizeof(ExpandedKey));
                    break;
                case 64:
                    CRYPTO_RC5_64_KeyExpansion(srcKey, _KeyLength, _Round, ExpandedKey, sizeof(ExpandedKey));
                    break;
                default:
                    break;  // nothing
            }
        }

        void Encrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            switch (_WordBitLength) {
                case 16:
                    CRYPTO_RC5_16_EncryptBlock(srcBytes.value, ExpandedKey, _Round);
                    break;
                case 32:
                    CRYPTO_RC5_32_EncryptBlock(srcBytes.value, ExpandedKey, _Round);
                    break;
                case 64:
                    CRYPTO_RC5_64_EncryptBlock(srcBytes.value, ExpandedKey, _Round);
                    break;
                default:
                    break;  // nothing
            }
        }

        void Decrypt(accelpp::crypto::Block<BlockSize>& srcBytes) const {
            switch (_WordBitLength) {
                case 16:
                    CRYPTO_RC5_16_DecryptBlock(srcBytes.value, ExpandedKey, _Round);
                    break;
                case 32:
                    CRYPTO_RC5_32_DecryptBlock(srcBytes.value, ExpandedKey, _Round);
                    break;
                case 64:
                    CRYPTO_RC5_64_DecryptBlock(srcBytes.value, ExpandedKey, _Round);
                    break;
                default:
                    break;  // nothing
            }
        }

        RC5(const uint8_t (&srcKey)[_KeyLength]) {
            SetKey(srcKey);
        }

        ~RC5() {
            ClearKey();
        }

        void ClearKey() volatile {
            volatile uint2w_t<_WordBitLength>::Type* p = reinterpret_cast<volatile uint2w_t<_WordBitLength>::Type*>(ExpandedKey);
            for (int i = 0; i < 2 * (_Round + 1); ++i)
                p[i] = 0;
        }
    };

}