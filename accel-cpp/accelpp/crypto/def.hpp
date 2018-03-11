#pragma once
#include <stdint.h>
#include <stddef.h>
#include <intrin.h>
#include "../memory/logical.hpp"

namespace accelpp::crypto {

    // available size is 4, 8, 16 and 32
    template<size_t _size>
    struct Block {
        static_assert(_size == 4 || _size == 8 || _size == 16 || _size == 32, "accelpp::crypto::Block: Not supported.");

        static constexpr size_t BlockSize = _size;

        uint8_t value[BlockSize];

        Block() = default;
        Block(const Block<_size>&) = default;
        Block(Block<_size>&&) = default;

        Block(const uint8_t (&srcBytes)[BlockSize]) {
            for (size_t i = 0; i < BlockSize; ++i)
                value[i] = srcBytes[i];
        }

        Block<_size>& operator=(const Block<_size>&) = default;
        Block<_size>& operator=(Block<_size>&&) = default;

        Block<_size>& operator=(const uint8_t (&srcBytes)[BlockSize]) {
            for (size_t i = 0; i < BlockSize; ++i)
                value[i] = srcBytes[i];
            return *this;
        }

        Block<_size>& operator^=(const Block<_size>& other) {
            accelpp::memory::Xor<BlockSize>(value, other.value);
            return *this;
        }

        Block<_size> operator^(const Block<_size>& other) const {
            Block<_size> ret = *this;
            accelpp::memory::Xor<BlockSize>(ret.value, other.value);
            return ret;
        }

        void Clear() volatile {
            for (size_t i = 0; i < BlockSize; ++i)
                value[i] = 0;
        }
    };

}
