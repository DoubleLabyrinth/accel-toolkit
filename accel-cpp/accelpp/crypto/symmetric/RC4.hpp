#pragma once
#include "cryptolib/RC4.h"

namespace accelpp::crypto::symmetric {

    template<size_t _KeyLength>
    class RC4 {
        static_assert(_KeyLength <= CRYPTO_RC4_MAX_KEY_LENGTH, "RC4: Key is too long.");
    public:
        uint8_t S[256];
        uint8_t S2[256];
    public:

        static constexpr size_t KeyByteLength = _KeyLength;
        static constexpr size_t BlockSize = -1;

        void SetKey(const uint8_t (&srcKey)[_KeyLength]) {
            CRYPTO_RC4_KeyExpansion(srcKey, _KeyLength, S2);
        }

        void Reset() {
            CRYPTO_RC4_Reset(S, S2);
        }

        void Encrypt(uint8_t srcBytes[], size_t srcBytesLength) {
            CRYPTO_RC4_Process(srcBytes, srcBytesLength, S);
        }

        void Decrypt(uint8_t srcBytes[], size_t srcBytesLength) {
            CRYPTO_RC4_Process(srcBytes, srcBytesLength, S);
        }

        RC4(const uint8_t (&srcKey)[_KeyLength]) {
            SetKey(srcKey);
        }

        ~RC4() {
            ClearKey();
        }

        void ClearKey() volatile {
            volatile uint32_t* p =reinterpret_cast<volatile uint32_t*>(S);
            for (size_t i = 0; i < sizeof(S) / sizeof(uint32_t); ++i)
                p[i] = 0;
            p = reinterpret_cast<volatile uint32_t*>(S2);
            for (size_t i = 0; i < sizeof(S2) / sizeof(uint32_t); ++i)
                p[i] = 0;
        }

    };

}