#pragma once
#include "def.hpp"

namespace accelpp::crypto {

    enum class CipherMode {
        VectorBased, 
        ECB, 
        CBC,
        PCBC,
        CFB,
        OFB,
        CTR
    };

    template<typename BlockCipherType, CipherMode Mode, typename... other>
    class SymmetricCipher;

    template<typename BlockCipherType>
    class SymmetricCipher<BlockCipherType, CipherMode::VectorBased> : public BlockCipherType {
    private:
        accelpp::crypto::Block<BlockCipherType::BlockSize> InitialVector;
    public:

        accelpp::crypto::Block<BlockCipherType::BlockSize> CurrentVector;

        SymmetricCipher(const uint8_t (&srcKey)[BlockCipherType::KeyByteLength]) : 
            BlockCipherType(srcKey), 
            InitialVector{ },
            CurrentVector{ } { }

        SymmetricCipher(const uint8_t(&srcIV)[BlockCipherType::BlockSize],
                        const uint8_t(&srcKey)[BlockCipherType::KeyByteLength]) : 
            BlockCipherType(srcKey),
            InitialVector(srcIV),
            CurrentVector(srcIV) { }

        void SetIV(const uint8_t(&srcIV)[BlockCipherType::BlockSize]) {
            InitialVector = srcIV;
            CurrentVector = srcIV;
        }

        const accelpp::crypto::Block<BlockCipherType::BlockSize>& GetIV() const {
            return InitialVector;
        }

        void ResetCV() {
            CurrentVector = InitialVector;
        }

        void ClearVector() volatile {
            InitialVector.Clear();
            CurrentVector.Clear();
        }

    };

    template<typename BlockCipherType>
    class SymmetricCipher<BlockCipherType, CipherMode::ECB> : public BlockCipherType {
    public:

        SymmetricCipher(const uint8_t (&srcKey)[BlockCipherType::KeyByteLength]) : 
            BlockCipherType(srcKey) { }

        void Encrypt(accelpp::crypto::Block<BlockCipherType::BlockSize> srcBlocks[], size_t srcBlocksCount) const {
            for (size_t i = 0; i < srcBlocksCount; ++i)
                BlockCipherType::Encrypt(srcBlocks[i]);
        }

        void Decrypt(accelpp::crypto::Block<BlockCipherType::BlockSize> srcBlocks[], size_t srcBlocksCount) const {
            for (size_t i = 0; i < srcBlocksCount; ++i)
                BlockCipherType::Decrypt(srcBlocks[i]);
        }

    };

    template<typename BlockCipherType>
    class SymmetricCipher<BlockCipherType, CipherMode::CBC> : 
        public SymmetricCipher<BlockCipherType, CipherMode::VectorBased> {
    public:

        SymmetricCipher(const uint8_t (&srcKey)[BlockCipherType::KeyByteLength]) :
            SymmetricCipher<BlockCipherType, CipherMode::VectorBased>(srcKey) { }

        SymmetricCipher(const uint8_t (&srcIV)[BlockCipherType::BlockSize],
                        const uint8_t (&srcKey)[BlockCipherType::KeyByteLength]) :
            SymmetricCipher<BlockCipherType, CipherMode::VectorBased>(srcIV, srcKey) { }

        void Encrypt(accelpp::crypto::Block<BlockCipherType::BlockSize> srcBlocks[], size_t srcBlocksCount) {
            for (size_t i = 0; i < srcBlocksCount; ++i) {
                srcBlocks[i] ^= CurrentVector;
                BlockCipherType::Encrypt(srcBlocks[i]);
                CurrentVector = srcBlocks[i];
            }
        }

        void Decrypt(accelpp::crypto::Block<BlockCipherType::BlockSize> srcBlocks[], size_t srcBlocksCount) {
            for (size_t i = 0; i < srcBlocksCount; ++i) {
                accelpp::crypto::Block<BlockCipherType::BlockSize> NextVector = srcBlocks[i];
                BlockCipherType::Decrypt(srcBlocks[i]);
                srcBlocks[i] ^= CurrentVector;
                CurrentVector = NextVector;
            }
        }

    };

    template<typename BlockCipherType>
    class SymmetricCipher<BlockCipherType, CipherMode::PCBC> :
        public SymmetricCipher<BlockCipherType, CipherMode::VectorBased> {
    public:

        SymmetricCipher(const uint8_t (&srcKey)[BlockCipherType::KeyByteLength]) :
            SymmetricCipher<BlockCipherType, CipherMode::VectorBased>(srcKey) { }

        SymmetricCipher(const uint8_t (&srcIV)[BlockCipherType::BlockSize],
                        const uint8_t (&srcKey)[BlockCipherType::KeyByteLength]) :
            SymmetricCipher<BlockCipherType, CipherMode::VectorBased>(srcIV, srcKey) { }

        void Encrypt(accelpp::crypto::Block<BlockCipherType::BlockSize> srcBlocks[], size_t srcBlocksCount) {
            for (size_t i = 0; i < srcBlocksCount; ++i) {
                accelpp::crypto::Block<BlockCipherType::BlockSize> temp = srcBlocks[i];
                srcBlocks[i] ^= CurrentVector;
                BlockCipherType::Encrypt(srcBlocks[i]);
                temp ^= srcBlocks[i];
                CurrentVector = temp;
            }
        }

        void Decrypt(accelpp::crypto::Block<BlockCipherType::BlockSize> srcBlocks[], size_t srcBlocksCount) {
            for (size_t i = 0; i < srcBlocksCount; ++i) {
                accelpp::crypto::Block<BlockCipherType::BlockSize> temp = srcBlocks[i];
                BlockCipherType::Decrypt(srcBlocks[i]);
                srcBlocks[i] ^= CurrentVector;
                temp ^= srcBlocks[i];
                CurrentVector = temp;
            }
        }

    };

    template<typename BlockCipherType>
    class SymmetricCipher<BlockCipherType, CipherMode::CFB> :
        public SymmetricCipher<BlockCipherType, CipherMode::VectorBased> {
    public:

        SymmetricCipher(const uint8_t (&srcKey)[BlockCipherType::KeyByteLength]) :
            SymmetricCipher<BlockCipherType, CipherMode::VectorBased>(srcKey) { }

        SymmetricCipher(const uint8_t (&srcIV)[BlockCipherType::BlockSize],
                        const uint8_t (&srcKey)[BlockCipherType::KeyByteLength]) :
            SymmetricCipher<BlockCipherType, CipherMode::VectorBased>(srcIV, srcKey) { }

        void Encrypt(accelpp::crypto::Block<BlockCipherType::BlockSize> srcBlocks[], size_t srcBlocksCount) {
            for (size_t i = 0; i < srcBlocksCount; ++i) {
                BlockCipherType::Encrypt(CurrentVector);
                srcBlocks[i] ^= CurrentVector;
                CurrentVector = srcBlocks[i];
            }
        }

        template<>
        void Decrypt(accelpp::crypto::Block<BlockCipherType::BlockSize> srcBlocks[], size_t srcBlocksCount) {
            for (size_t i = 0; i < srcBlocksCount; ++i) {
                BlockCipherType::Encrypt(CurrentVector);
                accelpp::crypto::Block<BlockCipherType::BlockSize> NextVector = srcBlocks[i];
                srcBlocks[i] ^= CurrentVector;
                CurrentVector = NextVector;
            }
        }

    };

    template<typename BlockCipherType>
    class SymmetricCipher<BlockCipherType, CipherMode::OFB> :
        public SymmetricCipher<BlockCipherType, CipherMode::VectorBased> {
    public:

        SymmetricCipher(const uint8_t (&srcKey)[BlockCipherType::KeyByteLength]) :
            SymmetricCipher<BlockCipherType, CipherMode::VectorBased>(srcKey) { }

        SymmetricCipher(const uint8_t (&srcIV)[BlockCipherType::BlockSize],
                        const uint8_t (&srcKey)[BlockCipherType::KeyByteLength]) :
            SymmetricCipher<BlockCipherType, CipherMode::VectorBased>(srcIV, srcKey) { }

        void Encrypt(accelpp::crypto::Block<BlockCipherType::BlockSize> srcBlocks[], size_t srcBlocksCount) {
            for (size_t i = 0; i < srcBlocksCount; ++i) {
                BlockCipherType::Encrypt(CurrentVector);
                srcBlocks[i] ^= CurrentVector;
            }
        }

        void Decrypt(accelpp::crypto::Block<BlockCipherType::BlockSize> srcBlocks[], size_t srcBlocksCount) {
            for (size_t i = 0; i < srcBlocksCount; ++i) {
                BlockCipherType::Encrypt(CurrentVector)
                srcBlocks[i] ^= CurrentVector;
            }
        }

    };

    template<typename BlockCipherType, typename CounterType>
    class SymmetricCipher<BlockCipherType, CipherMode::CTR, CounterType> : public BlockCipherType {
        static_assert(CounterType::CounterSize == BlockCipherType::BlockSize, "[accelpp::crypto::SymmetricCipher]: CounterSize mismatches");
    public:

        CounterType Counter;

        SymmetricCipher(const uint8_t (&srcKey)[BlockCipherType::KeyByteLength]) : 
            BlockCipherType(srcKey) { }

        void Encrypt(accelpp::crypto::Block<BlockCipherType::BlockSize> srcBlocks[], size_t srcBlocksCount) {
            for (size_t i = 0; i < srcBlocksCount; ++i) {
                accelpp::crypto::Block<BlockCipherType::BlockSize> CurrentVector = Counter.GetVector();
                BlockCipherType::Encrypt(CurrentVector);
                srcBlocks[i] ^= CurrentVector;
                Counter.Update();
            }
        }

        void Decrypt(accelpp::crypto::Block<BlockCipherType::BlockSize> srcBlocks[], size_t srcBlocksCount) {
            for (size_t i = 0; i < srcBlocksCount; ++i) {
                accelpp::crypto::Block<BlockCipherType::BlockSize> CurrentVector = Counter.GetVector();
                BlockCipherType::Encrypt(CurrentVector);
                srcBlocks[i] ^= CurrentVector;
                Counter.Update();
            }
        }
    };
}