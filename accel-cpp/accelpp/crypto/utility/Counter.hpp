#pragma once
#include "../def.hpp"
#include "../../memory/reverse.hpp"

namespace accelpp::crypto::utility {

    template<size_t _size, bool _little_endian, size_t _prefix_size, size_t _postfix_size>
    class Counter {
        static_assert(_size > _prefix_size + _postfix_size, "[accelpp::crypto::utility::Counter]: The sum of _prefix_size and _postfix_size is too large.");
    private:

        template<size_t __prefix_size, size_t __postfix_size>
        union Descriptor {
            struct {
                uint8_t prefix[__prefix_size];
                uint8_t var[_size - __prefix_size - __postfix_size];
                uint8_t postfix[__postfix_size];
            };

            Block<_size> all;

            void SetPrefix(const uint8_t (&src)[__prefix_size]) {
                for (size_t i = 0; i < __prefix_size; ++i)
                    prefix[i] = src[i];
            }

            void SetPostfix(const uint8_t (&src)[__postfix_size]) {
                for (size_t i = 0; i < __postfix_size; ++i)
                    postfix[i] = src[i];
            }

            void SetVar(const uint8_t (&src)[_size - __prefix_size - __postfix_size]) {
                for (size_t i = 0; i < (_size - __prefix_size - __postfix_size); ++i)
                    var[i] = src[i];
            }
        };

        template<size_t __prefix_size>
        union Descriptor<__prefix_size, 0> {
            struct {
                uint8_t prefix[__prefix_size];
                uint8_t var[_size - __prefix_size];
            };

            Block<_size> all;

            void SetPrefix(const uint8_t (&src)[__prefix_size]) {
                for (size_t i = 0; i < __prefix_size; ++i)
                    prefix[i] = src[i];
            }

            void SetVar(const uint8_t (&src)[_size - __prefix_size]) {
                for (size_t i = 0; i < (_size - __prefix_size); ++i)
                    var[i] = src[i];
            }
        };

        template<size_t __postfix_size>
        union Descriptor {
            struct {
                uint8_t var[_size - __postfix_size];
                uint8_t postfix[__postfix_size];
            };

            Block<_size> all;

            void SetPostfix(const uint8_t(&src)[__postfix_size]) {
                for (size_t i = 0; i < __postfix_size; ++i)
                    postfix[i] = src[i];
            }

            void SetVar(const uint8_t(&src)[_size - __postfix_size]) {
                for (size_t i = 0; i < (_size - __postfix_size); ++i)
                    var[i] = src[i];
            }
        };

    public:

        static constexpr size_t CounterSize = _size;
        static constexpr size_t PrefixSize = _prefix_size;
        static constexpr size_t VariableSize = _size - (_prefix_size + _postfix_size);
        static constexpr size_t PostfixSize = _postfix_size;

        Descriptor<PrefixSize, PostfixSize> desc;

        Counter() : desc{ } { }

        Counter(const uint64_t init_value) : desc{ } {
            for (size_t i = 0; i < VariableSize; ++i)
                desc.var[i] = reinterpret_cast<const uint8_t*>(&init_value)[i];
        }

        Counter(const uint8_t (&init_value)[VariableSize]) : desc{ } {
            for (size_t i = 0; i < VariableSize; ++i)
                desc.var[i] = init_value[i];
        }

        void Update() {
            uint8_t carry = _addcarry_u8(1, desc.var[0], 0, desc.var);
            for (size_t i = 1; carry != 0 && i < VariableSize; ++i)
                carry = _addcarry_u8(carry, desc.var[i], 0, desc.var + i);
        }

        Block<CounterSize> GetVector() const {
            if (_little_endian) {
                return desc.all;
            } else {
                Descriptor<PrefixSize, PostfixSize> ret = desc;
                accelpp::memory::Reverse<VariableSize>(ret.var);
                return ret.all;
            }
        }
    };

    // you can add your custom 

}