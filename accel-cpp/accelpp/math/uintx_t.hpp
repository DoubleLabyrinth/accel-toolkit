#pragma once
#include "mathlib/add.h"
#include "mathlib/sub.h"
#include "mathlib/mul.h"
#include "mathlib/div.h"
#include "mathlib/cmp.h"
#include "../memory/bitwise.hpp"
#include <type_traits>
#include <memory.h>

namespace accelpp::math {

    template<size_t _bits>
    class uintx_t {
        static_assert(_bits > sizeof(uintmax_t) * 8 && (_bits & (_bits - 1)) == 0,
                      "[accelpp::math::uintx_t]: _bits must be larger than (sizeof(uintmax_t) * 8) and must be a power of 2.");
    public:
        static constexpr size_t length = _bits / (sizeof(coeff_t) * 8);
    private:

        //-------------------assign_by_type---------------------------

        template<size_t dst_length, typename _Type>
        struct assign_by_type {
            static void impl(coeff_t (&dst)[dst_length], 
                             typename std::enable_if<std::is_integral<_Type>::value, _Type>::type x) {
                if (sizeof(_Type) <= sizeof(coeff_t)) {
                    dst[0] = static_cast<coeff_t>(x);
                    memset(dst + 1,
                           x < static_cast<_Type>(0) ? -1 : 0,
                           sizeof(dst) - sizeof(coeff_t));
                } else {
                    *reinterpret_cast<_Type*>(dst) = x;
                    memset(reinterpret_cast<_Type*>(dst) + 1,
                           x < static_cast<_Type>(0) ? -1 : 0,
                           sizeof(dst) - sizeof(_Type));
                }
            }
        };

        template<size_t dst_length, size_t src_length>
        struct assign_by_type<dst_length, coeff_t (&)[src_length]> {
        private:
            template<bool truncate>
            static void _impl(coeff_t (&dst)[dst_length], const coeff_t (&src)[src_length]);

            template<>
            static void _impl<false>(coeff_t (&dst)[dst_length], const coeff_t (&src)[src_length]) {
                memcpy(dst, src, sizeof(src));
                memset(dst + src_length, 0, sizeof(dst) - sizeof(src));
            }

            template<>
            [[deprecated("Warning: Truncation detected while assigning. If this is an error, please add \"#pragma warning(default: 4996)\" to make it a warning.")]]
            static void _impl<true>(coeff_t (&dst)[dst_length], const coeff_t (&src)[src_length]) {
                memcpy(dst, src, sizeof(dst));
            }
        public:
            static void impl(coeff_t (&dst)[dst_length], const coeff_t (&src)[src_length]) {
                _impl<(dst_length < src_length)>(dst, src);
            }
        };

        //---------------------add_by_type-------------------------

        template<size_t summand_length, typename _Type>
        struct add_by_type {
            static void impl(coeff_t (&summand)[summand_length], 
                             typename std::enable_if<std::is_integral<_Type>::value, _Type>::type x) {
                coeff_t x_ext[sizeof(x) / sizeof(coeff_t) == 0 ? 1 : sizeof(x) / sizeof(coeff_t)] = { };

                if (x < static_cast<_Type>(0)) {
                    *reinterpret_cast<_Type*>(x_ext) = -x;
                    math_uintx_sub(summand, summand_length, x_ext, sizeof(x_ext) / sizeof(coeff_t), 0);
                } else {
                    *reinterpret_cast<_Type*>(x_ext) = x;
                    math_uintx_add(summand, summand_length, x_ext, sizeof(x_ext) / sizeof(coeff_t), 0);
                }
            }
        };

        template<size_t summand_length, size_t addend_length>
        struct add_by_type<summand_length, coeff_t (&)[addend_length]> {
        private:
            template<bool truncate>
            static void _impl(coeff_t (&summand)[summand_length], const coeff_t (&addend)[addend_length]);

            template<>
            static void _impl<false>(coeff_t (&summand)[summand_length], const coeff_t (&addend)[addend_length]) {
                math_uintx_add(summand, summand_length, addend, addend_length, 0);
            }

            template<>
            [[deprecated("Warning: Truncation detected while adding. If this is an error, please add \"#pragma warning(default: 4996)\" to make it a warning.")]]
            static void _impl<true>(coeff_t (&summand)[summand_length], const coeff_t (&addend)[addend_length]) {
                math_uintx_add(summand, summand_length, addend, summand_length, 0);
            }
        public:
            static void impl(coeff_t (&summand)[summand_length], const coeff_t (&addend)[addend_length]) {
                _impl<(summand_length < addend_length)>(summand, addend);
            }
        };

        //-------------------sub_by_type---------------------------

        template<size_t minuend_length, typename _Type>
        struct sub_by_type {
            static void impl(coeff_t (&minuend)[minuend_length], 
                             typename std::enable_if<std::is_integral<_Type>::value, _Type>::type x) {
                coeff_t x_ext[sizeof(x) / sizeof(coeff_t) == 0 ? 1 : sizeof(x) / sizeof(coeff_t)] = { };

                if (x < static_cast<_Type>(0)) {
                    *reinterpret_cast<_Type*>(x_ext) = -x;
                    math_uintx_add(minuend, minuend_length, x_ext, sizeof(x_ext) / sizeof(coeff_t), 0);
                } else {
                    *reinterpret_cast<_Type*>(x_ext) = x;
                    math_uintx_sub(minuend, minuend_length, x_ext, sizeof(x_ext) / sizeof(coeff_t), 0);
                }
            }
        };

        template<size_t minuend_length, size_t subtrahend_length>
        struct sub_by_type<minuend_length, coeff_t (&)[subtrahend_length]> {
        private:
            template<bool truncate>
            static void _impl(coeff_t (&minuend)[minuend_length], const coeff_t (&subtrahend)[subtrahend_length]);

            template<>
            static void _impl<false>(coeff_t (&minuend)[minuend_length], const coeff_t (&subtrahend)[subtrahend_length]) {
                math_uintx_sub(minuend, minuend_length, subtrahend, subtrahend_length, 0);
            }

            template<>
            [[deprecated("Warning: Truncation detected while subtracting. If this is an error, please add \"#pragma warning(default: 4996)\" to make it a warning.")]]
            static void _impl<true>(coeff_t (&minuend)[minuend_length], const coeff_t (&subtrahend)[subtrahend_length]) {
                math_uintx_sub(minuend, minuend_length, subtrahend, subtrahend_length, 0);
            }
        public:
            static void impl(coeff_t (&minuend)[minuend_length], const coeff_t (&subtrahend)[subtrahend_length]) {
                _impl<(minuend_length < subtrahend_length)>(minuend, subtrahend);
            }
        };


        //-------------------mul_by_type-----------------------------
        template<size_t multiplier_length, typename _Type>
        struct mul_by_type {
            static void impl(coeff_t (&multiplier)[multiplier_length], 
                             typename std::enable_if<std::is_integral<_Type>::value, _Type>::type x) {
                if (x < static_cast<_Type>(0)) {
                    coeff_t x_ext[multiplier_length];
                    coeff_t product[2 * multiplier_length] = { };
                    *reinterpret_cast<_Type*>(x_ext) = x;
                    memset(reinterpret_cast<_Type*>(x_ext) + 1, -1, sizeof(x_ext) - sizeof(_Type));
                    math_uintx_mul_to_Karatsuba(multiplier, x_ext, multiplier_length, product);
                    memcpy(multiplier, product, sizeof(multiplier));
                } else {
                    if (sizeof(_Type) <= sizeof(coeff_t)) {
                        math_uintx_muls(multiplier, multiplier_length, x);
                    } else {
                        coeff_t x_ext[sizeof(_Type) / sizeof(coeff_t)];
                        *reinterpret_cast<_Type*>(x_ext) = x;
                        coeff_t product[multiplier_length + sizeof(x_ext) / sizeof(coeff_t)];
                        math_uintx_mul_to(x_ext, sizeof(x_ext) / sizeof(coeff_t), multiplier, multiplier_length, product);
                        memcpy(multiplier, product, sizeof(multiplier));
                    }
                }
            }
        };

        template<size_t multiplier_length, size_t multiplicand_length>
        struct mul_by_type<multiplier_length, coeff_t (&)[multiplicand_length]> {
        private:

            template<bool truncate>
            static void _impl(coeff_t (&multiplier)[multiplier_length], const coeff_t (&multiplicand)[multiplicand_length]);

            template<>
            static void _impl<false>(coeff_t (&multiplier)[multiplier_length], const coeff_t (&multiplicand)[multiplicand_length]) {
                coeff_t product[2 * multiplier_length] = { };

                if (multiplier_length == multiplicand_length) {
                    math_uintx_mul_to_Karatsuba(multiplier, multiplicand, multiplier_length, product);
                } else {
                    coeff_t multiplicand_ext[multiplier_length];
                    memcpy(multiplicand_ext, multiplicand, sizeof(multiplicand));
                    memset(multiplicand_ext + multiplicand_length, 0, sizeof(multiplicand_ext) - sizeof(multiplicand));
                    math_uintx_mul_to_Karatsuba(multiplier, multiplicand_ext, multiplier_length, product);
                }

                memcpy(multiplier, product, sizeof(multiplier));
            }

            template<>
            [[deprecated("Warning: Truncation detected while multiplying. If this is an error, please add \"#pragma warning(default: 4996)\" to make it a warning.")]]
            static void _impl<true>(coeff_t (&multiplier)[multiplier_length], const coeff_t (&multiplicand)[multiplicand_length]) {
                coeff_t product[2 * multiplier_length] = { };
                math_uintx_mul_to_Karatsuba(multiplier, multiplicand, multiplier_length, product);
                memcpy(multiplier, product, sizeof(multiplier));
            }
        public:
            static void impl(coeff_t (&multiplier)[multiplier_length], const coeff_t (&multiplicand)[multiplicand_length]) {
                _impl<(multiplier_length < multiplicand_length)>(multiplier, multiplicand);
            }
        };

        //-------------------div_by_type-----------------------------
        template<size_t dividend_length, typename _Type>
        struct div_by_type {
            static void impl(coeff_t (&dividend)[dividend_length], 
                             typename std::enable_if<std::is_integral<_Type>::value, _Type>::type x) {
                if (x < static_cast<_Type>(0)) {
                    coeff_t x_ext[dividend_length];
                    *reinterpret_cast<_Type*>(x_ext) = x;
                    memset(reinterpret_cast<_Type*>(x_ext) + 1, -1, sizeof(x_ext) - sizeof(_Type));

                    coeff_t quotient[dividend_length];
                    math_uintx_divmod(dividend, dividend_length, x_ext, dividend_length, quotient, dividend_length);
                    memcpy(dividend, quotient, sizeof(dividend));
                } else {
                    if (sizeof(x) <= sizeof(coeff_t)) {
                        math_uintx_divs(dividend, dividend_length, static_cast<coeff_t>(x));
                    } else {
                        coeff_t x_ext[sizeof(_Type) / sizeof(coeff_t)];
                        *reinterpret_cast<_Type*>(x_ext) = x;

                        coeff_t quotient[dividend_length];
                        math_uintx_divmod(dividend, dividend_length, x_ext, sizeof(x_ext) / sizeof(coeff_t), quotient, dividend_length);
                        memcpy(dividend, quotient, sizeof(dividend));
                    }
                }
            }
        };

        template<size_t dividend_length, size_t divisor_length>
        struct div_by_type<dividend_length, coeff_t (&)[divisor_length]> {
        private:
            template<bool truncate>
            static void _impl(coeff_t (&dividend)[dividend_length], const coeff_t (&divisor)[divisor_length]);

            template<>
            static void _impl<false>(coeff_t (&dividend)[dividend_length], const coeff_t (&divisor)[divisor_length]) {
                coeff_t quotient[dividend_length];
                math_uintx_divmod(dividend, dividend_length, divisor, divisor_length, quotient, dividend_length);
                memcpy(dividend, quotient, sizeof(dividend));
            }

            template<>
            [[deprecated("Warning: Truncation detected while dividing. If this is an error, please add \"#pragma warning(default: 4996)\" to make it a warning.")]]
            static void _impl<true>(coeff_t (&dividend)[dividend_length], const coeff_t (&divisor)[divisor_length]) {
                coeff_t quotient[dividend_length];
                math_uintx_divmod(dividend, dividend_length, divisor, divisor_length, quotient, dividend_length);
                memcpy(dividend, quotient, sizeof(dividend));
            }
        public:
            static void impl(coeff_t (&dividend)[dividend_length], const coeff_t (&divisor)[divisor_length]) {
                _impl<(dividend_length < divisor_length)>(dividend, divisor);
            }
        };

        //-------------------mod_by_type-----------------------------
        template<size_t dividend_length, typename _Type>
        struct mod_by_type {
            static void impl(coeff_t (&dividend)[dividend_length], 
                             typename std::enable_if<std::is_integral<_Type>::value, _Type>::type x) {
                if (x < static_cast<_Type>(0)) {
                    coeff_t x_ext[dividend_length];
                    *reinterpret_cast<_Type*>(x_ext) = x;
                    memset(reinterpret_cast<_Type*>(x_ext) + 1, -1, sizeof(x_ext) - sizeof(_Type));

                    coeff_t quotient[dividend_length];
                    math_uintx_divmod(dividend, dividend_length, x_ext, dividend_length, quotient, dividend_length);
                } else {
                    if (sizeof(x) <= sizeof(coeff_t)) {
                        coeff_t remainder = math_uintx_divs(dividend, dividend_length, static_cast<coeff_t>(x));
                        memset(dividend, 0, sizeof(dividend));
                        dividend[0] = remainder;
                    } else {
                        coeff_t x_ext[sizeof(_Type) / sizeof(coeff_t)];
                        *reinterpret_cast<_Type*>(x_ext) = x;

                        coeff_t quotient[dividend_length];
                        math_uintx_divmod(dividend, dividend_length, x_ext, sizeof(x_ext) / sizeof(coeff_t), quotient, dividend_length);
                    }
                }
            }
        };

        template<size_t dividend_length, size_t divisor_length>
        struct mod_by_type<dividend_length, coeff_t (&)[divisor_length]> {
        private:
            template<bool truncate>
            static void _impl(coeff_t (&dividend)[dividend_length], const coeff_t (&divisor)[divisor_length]);

            template<>
            static void _impl<false>(coeff_t (&dividend)[dividend_length], const coeff_t (&divisor)[divisor_length]) {
                coeff_t quotient[dividend_length];
                math_uintx_divmod(dividend, dividend_length, divisor, divisor_length, quotient, dividend_length);
            }

            template<>
            [[deprecated("Warning: Truncation detected while modularizing. If this is an error, please add \"#pragma warning(default: 4996)\" to make it a warning.")]]
            static void _impl<true>(coeff_t (&dividend)[dividend_length], const coeff_t (&divisor)[divisor_length]) {
                coeff_t quotient[dividend_length];
                math_uintx_divmod(dividend, dividend_length, divisor, divisor_length, quotient, dividend_length);
            }
        public:
            static void impl(coeff_t (&dividend)[dividend_length], const coeff_t (&divisor)[divisor_length]) {
                _impl<(dividend_length < divisor_length)>(dividend, divisor);
            }
        };

        //-------------------cmp_by_type-----------------------------
        template<size_t dst_length, typename _Type>
        struct cmp_by_type {
            static bool impl(coeff_t (&dst)[dst_length], 
                             typename std::enable_if<std::is_integral<_Type>::value, _Type>::type x) {
                if (x < static_cast<_Type>(0)) {
                    coeff_t x_ext[dst_length];
                    memset(reinterpret_cast<_Type*>(x_ext) + 1, -1, sizeof(x_ext) - sizeof(_Type));
                    return math_uintx_cmp(dst, dst_length, x_ext, dst_length);
                } else {
                    coeff_t x_ext[sizeof(_Type) / sizeof(coeff_t) == 0 ? 1 : sizeof(_Type) / sizeof(coeff_t)] = { };
                    *reinterpret_cast<_Type*>(x_ext) = x;
                    return math_uintx_cmp(dst, dst_length, x_ext, sizeof(x) / sizeof(coeff_t));
                }
            }
        };

        //-----------------shift_right---------------------------------

        void shift_right(size_t shift) {
            if (shift == 0)
                return;

            if (shift >= _bits) {
                memset(value, 0, sizeof(value));
                return;
            }

            size_t shift_uintrs = shift / (sizeof(coeff_t) * 8);
            size_t shift_bits = shift % (sizeof(coeff_t) * 8);
            coeff_t* cur_ptr = value;

            for (size_t i = shift_uintrs; i < length - 1; ++i) {
                *cur_ptr = (value[i] >> shift_bits) | (value[i + 1] << (sizeof(coeff_t) * 8 - shift_bits));
                ++cur_ptr;
            }

            *cur_ptr = value[length - 1] >> shift_bits;
            ++cur_ptr;
            memset(cur_ptr, 0, ((value + length) - cur_ptr) * sizeof(coeff_t));
        }

        //-----------------shift_left---------------------------------

        void shift_left(size_t shift) {
            if (shift == 0)
                return;

            if (shift >= _bits) {
                memset(value, 0, sizeof(value));
                return;
            }

            size_t shift_uintrs = shift / (sizeof(coeff_t) * 8);
            size_t shift_bits = shift % (sizeof(coeff_t) * 8);
            coeff_t* cur_ptr = value + length;

            for (size_t i = length - shift_uintrs - 1; i > 0; --i) {
                --cur_ptr;
                *cur_ptr = (value[i] << shift_bits) | (value[i - 1] >> (sizeof(coeff_t) * 8 - shift_bits));
            }

            --cur_ptr;
            *cur_ptr = value[0] << shift_bits;
            memset(value, 0, (cur_ptr - value) * sizeof(coeff_t));
        }

    public:

        coeff_t value[length];

        // return true if truncated, else return false.
        bool assign_from_bytes(const void* bytes_ptr, size_t bytes_length) {
            if (sizeof(value) >= bytes_length) {
                memcpy(value, bytes_ptr, bytes_length);
                memset(reinterpret_cast<uint8_t*>(value) + bytes_length, 0, sizeof(value) - bytes_length);
                return false;
            } else {
                memcpy(value, bytes_ptr, sizeof(value));
                return true;
            }
        }

        //------------------------------
        //      constructor
        //------------------------------

        uintx_t() = default;
        uintx_t(const uintx_t<_bits>&) = default;
        uintx_t(uintx_t<_bits>&&) = default;

        template<typename _Type>
        uintx_t(_Type x) {
            assign_by_type<length, _Type>::impl(value, x);
        }

        template<size_t src_length>
        uintx_t(const coeff_t (&src)[src_length]) {
            assign_by_type<length, coeff_t (&)[src_length]>::impl(value, src);
        }

        uintx_t(const void* bytes_ptr, size_t bytes_length) {
            assign_from_bytes(bytes_ptr, bytes_length);
        }

        template<size_t others_bits>
        uintx_t(const uintx_t<others_bits>& other) {
            assign_by_type<length, coeff_t (&)[other.length]>::impl(value, other.value);
        }

        //------------------------------
        //      operator!=
        //------------------------------

        template<typename _Type>
        bool operator!=(_Type x) const {
            return cmp_by_type<length, _Type>(value, x) != 0;
        }

        template<size_t others_bits>
        bool operator!=(const uintx_t<others_bits>& other) const {
            return math_uintx_cmp(value, length, other.value, other.length) != 0;
        }

        //------------------------------
        //      operator==
        //------------------------------

        template<typename _Type>
        bool operator==(_Type x) const {
            return cmp_by_type<length, _Type>(value, x) == 0;
        }

        template<size_t others_bits>
        bool operator==(const uintx_t<others_bits>& other) const {
            return math_uintx_cmp(value, length, other.value, other.length) == 0;
        }

        //------------------------------
        //      operator<
        //------------------------------

        template<typename _Type>
        bool operator<(_Type x) const {
            return cmp_by_type<length, _Type>(value, x) == -1;
        }

        template<size_t others_bits>
        bool operator<(const uintx_t<others_bits>& other) const {
            return math_uintx_cmp(value, length, other.value, other.length) == -1;
        }

        //------------------------------
        //      operator>
        //------------------------------

        template<typename _Type>
        bool operator>(_Type x) const {
            return cmp_by_type<length, _Type>(value, x) == 1;
        }

        template<size_t others_bits>
        bool operator>(const uintx_t<others_bits>& other) const {
            return math_uintx_cmp(value, length, other.value, other.length) == 1;
        }

        //------------------------------
        //      operator<<
        //------------------------------

        uintx_t<_bits> operator<<(size_t shift) const {
            uintx_t<_bits> ret = *this;
            ret.shift_left(shift);
            return ret;
        }

        //------------------------------
        //      operator>>
        //------------------------------

        uintx_t<_bits> operator>>(size_t shift) const {
            uintx_t<_bits> ret = *this;
            ret.shift_right(shift);
            return ret;
        }

        //------------------------------
        //      operator=
        //------------------------------

        uintx_t<_bits>& operator=(const uintx_t<_bits>&) = default;
        uintx_t<_bits>& operator=(uintx_t<_bits>&&) = default;

        template<typename _Type>
        uintx_t<_bits>& operator=(_Type x) {
            assign_by_type<length, _Type>(value, x);
            return *this;
        }

        template<size_t others_bits>
        uintx_t<_bits>& operator=(const uintx_t<others_bits>& other) {
            if (&other != this)
                assign_by_type<length, coeff_t (&)[other.length]>::impl(value, other.value);
            return *this;
        }

        //------------------------------
        //      operator+=
        //------------------------------

        template<typename _Type>
        uintx_t<_bits>& operator+=(_Type x) {
            add_by_type<length, _Type>(value, x);
            return *this;
        }

        template<size_t others_bits>
        uintx_t<_bits>& operator+=(const uintx_t<others_bits>& other) {
            add_by_type<length, coeff_t (&)[other.length]>::impl(value, other.value);
            return *this;
        }

        //------------------------------
        //      operator-=
        //------------------------------

        template<typename _Type>
        uintx_t<_bits>& operator-=(_Type x) {
            sub_by_type<length, _Type>(value, x);
            return *this;
        }

        template<size_t others_bits>
        uintx_t<_bits>& operator-=(const uintx_t<others_bits>& other) {
            sub_by_type<length, coeff_t (&)[other.length]>::impl(value, other.value);
            return *this;
        }

        //------------------------------
        //      operator*=
        //------------------------------

        template<typename _Type>
        uintx_t<_bits>& operator*=(_Type x) {
            mul_by_type<length, _Type>(value, x);
            return *this;
        }

        template<size_t others_bits>
        uintx_t<_bits>& operator*=(const uintx_t<others_bits>& other) {
            mul_by_type<length, coeff_t (&)[other.length]>::impl(value, other.value);
            return *this;
        }

        //------------------------------
        //      operator/=
        //------------------------------

        template<typename _Type>
        uintx_t<_bits>& operator/=(_Type x) {
            div_by_type<length, _Type>(value, x);
            return *this;
        }

        template<size_t others_bits>
        uintx_t<_bits>& operator/=(const uintx_t<others_bits>& other) {
            if(&other != this)
                div_by_type<length, coeff_t (&)[other.length]>::impl(value, other.value);
            else {
                memset(value, 0, sizeof(value));
                value[0] = 1;
            }
            return *this;
        }

        //------------------------------
        //      operator%=
        //------------------------------

        template<typename _Type>
        uintx_t<_bits>& operator%=(_Type x) {
            mod_by_type<length, _Type>(value, x);
            return *this;
        }

        template<size_t others_bits>
        uintx_t<_bits>& operator%=(const uintx_t<others_bits>& other) {
            if (&other != this)
                mod_by_type<length, coeff_t (&)[other.length]>::impl(value, other.value);
            else
                memset(value, 0, sizeof(value));
            return *this;
        }

        //------------------------------
        //      operator<<=
        //------------------------------

        uintx_t<_bits>& operator<<=(size_t shift) {
            shift_left(shift);
            return *this;
        }

        //------------------------------
        //      operator>>=
        //------------------------------

        uintx_t<_bits>& operator>>=(size_t shift) {
            shift_right(shift);
            return *this;
        }

        //------------------------------
        //      operator&=
        //------------------------------

        uintx_t<_bits>& operator&=(const uintx_t<_bits>& other) {
            if (&other != this)
                accelpp::memory::And<sizeof(value)>(value, other.value);
            return *this;
        }

        //------------------------------
        //      operator|=
        //------------------------------

        uintx_t<_bits>& operator|=(const uintx_t<_bits>& other) {
            if (&other != this)
                accelpp::memory::Or<sizeof(value)>(value, other.value);
            return *this;
        }

        //------------------------------
        //      operator^=
        //------------------------------

        uintx_t<_bits>& operator^=(const uintx_t<_bits>& other) {
            if (&other != this)
                accelpp::memory::Xor<sizeof(value)>(value, other.value);
            else
                memset(value, 0, sizeof(value));
            return *this;
        }

        //------------------------------
        //      operator&
        //------------------------------

        uintx_t<_bits> operator&(const uintx_t<_bits>& other) const {
            uintx_t<_bits> ret = *this;
            ret &= other;
            return ret;
        }

        //------------------------------
        //      operator|
        //------------------------------

        uintx_t<_bits> operator|(const uintx_t<_bits>& other) const {
            uintx_t<_bits> ret = *this;
            ret |= other;
            return ret;
        }

        //------------------------------
        //      operator^
        //------------------------------

        uintx_t<_bits> operator^(const uintx_t<_bits>& other) const {
            uintx_t<_bits> ret = *this;
            ret ^= other;
            return ret;
        }

        //------------------------------
        //      operator~
        //------------------------------

        uintx_t<_bits> operator~() const {
            uintx_t<_bits> ret = *this;
            accelpp::memory::Not<sizeof(value)>(ret.value);
            return ret;
        }

        //------------------------------
        //      operator+
        //------------------------------

        template<typename _Type, size_t b_bits, typename = std::enable_if<std::is_integral<_Type>::value, _Type>::type>
        friend uintx_t<b_bits> operator+(_Type a, 
                                         const uintx_t<b_bits>& b);

        template<size_t a_bits, size_t b_bits>
        friend uintx_t<(a_bits > b_bits ? a_bits : b_bits)> 
            operator+(const uintx_t<a_bits>& a, const uintx_t<b_bits>& b);


        //------------------------------
        //      operator-
        //------------------------------

        template<typename _Type, size_t b_bits, typename = std::enable_if<std::is_integral<_Type>::value, _Type>::type>
        friend uintx_t<b_bits> operator-(_Type a, 
                                         const uintx_t<b_bits>& b);

        template<size_t a_bits, size_t b_bits>
        friend uintx_t<(a_bits > b_bits ? a_bits : b_bits)>
            operator-(const uintx_t<a_bits>& a, const uintx_t<b_bits>& b);

        //------------------------------
        //      operator*
        //------------------------------

        template<typename _Type, size_t b_bits, typename = std::enable_if<std::is_integral<_Type>::value, _Type>::type>
        friend uintx_t<b_bits> operator*(_Type a, 
                                         const uintx_t<b_bits>& b);

        template<size_t a_bits, size_t b_bits>
        friend uintx_t<(a_bits > b_bits ? a_bits : b_bits)>
            operator*(const uintx_t<a_bits>& a, const uintx_t<b_bits>& b);

        //------------------------------
        //      operator/
        //------------------------------

        template<typename _Type, size_t b_bits, typename = std::enable_if<std::is_integral<_Type>::value, _Type>::type>
        friend uintx_t<b_bits> operator/(_Type a,
                                         const uintx_t<b_bits>& b);

        template<size_t a_bits, size_t b_bits>
        friend uintx_t<(a_bits > b_bits ? a_bits : b_bits)>
            operator/(const uintx_t<b_bits>& a, const uintx_t<b_bits>& b);

        //------------------------------
        //      operator%
        //------------------------------

        template<typename _Type, size_t b_bits, typename = std::enable_if<std::is_integral<_Type>::value, _Type>::type>
        friend uintx_t<b_bits> operator%(_Type a, 
                                         const uintx_t<b_bits>& b);

        template<size_t a_bits, size_t b_bits>
        friend uintx_t<(a_bits > b_bits ? a_bits : b_bits)>
            operator%(const uintx_t<a_bits>& a, const uintx_t<b_bits>& b);

    };

    template<typename _Type, size_t b_bits, typename = std::enable_if<std::is_integral<_Type>::value, _Type>::type>
    uintx_t<b_bits> operator+(_Type a, 
                              const uintx_t<b_bits>& b) {
        uintx_t<b_bits> ret = b;
        ret.add_by_type<ret.length, _Type>(a);
        return ret;
    }

    template<size_t a_bits, size_t b_bits>
    uintx_t<(a_bits > b_bits ? a_bits : b_bits)>
        operator+(const uintx_t<a_bits>& a, const uintx_t<b_bits>& b) {
        uintx_t<(a_bits > b_bits ? a_bits : b_bits)> ret;

        if (a.length >= b.length) 
            math_uintx_add_to(a.value, a.length, b.value, b.length, 0, ret.value);
        else 
            math_uintx_add_to(b.value, b.length, a.value, a.length, 0, ret.value);

        return ret;
    }

    template<typename _Type, size_t b_bits, typename = std::enable_if<std::is_integral<_Type>::value, _Type>::type>
    uintx_t<b_bits> operator-(_Type a, 
                              const uintx_t<b_bits>& b) {
        uintx_t<b_bits> ret = a;
        ret -= b;
        return ret;
    }

    template<size_t a_bits, size_t b_bits>
    uintx_t<(a_bits > b_bits ? a_bits : b_bits)>
        operator-(const uintx_t<a_bits>& a, const uintx_t<b_bits>& b) {
        uintx_t<(a_bits > b_bits ? a_bits : b_bits)> ret;

        if (a.length >= b.length) {
            math_uintx_sub_to(a.value, a.length, b.value, b.length, 0, ret.value);
        } else {
            memcpy(ret.value, a.value, sizeof(a.value));
            memset(ret.value + a.length, 0, sizeof(ret.value) - sizeof(a.value));
            math_uintx_sub(ret.value, ret.length, b.value, b.length, 0);
        }

        return ret;
    }

    template<typename _Type, size_t b_bits, typename = std::enable_if<std::is_integral<_Type>::value, _Type>::type>
    uintx_t<b_bits> operator*(_Type a,
                              const uintx_t<b_bits>& b) {
        uintx_t<b_bits> ret = b;
        ret.mul_by_type<ret.length, _Type>(a);
        return ret;
    }

    template<size_t a_bits, size_t b_bits>
    uintx_t<(a_bits > b_bits ? a_bits : b_bits)>
        operator*(const uintx_t<a_bits>& a, const uintx_t<b_bits>& b) {
        if (a.length == b.length) {
            coeff_t product[2 * a.length];
            math_uintx_mul_to_Karatsuba(a.value, b.value, a.length, product);
            return *reinterpret_cast<uintx_t<a_bits>*>(product);
        } else {
            uintx_t<(a_bits > b_bits ? a_bits : b_bits)> ret = a_bits > b_bits ? b : a;
            ret *= a_bits > b_bits ? a : b;
            return ret;
        }
    }

    template<typename _Type, size_t b_bits, typename = std::enable_if<std::is_integral<_Type>::value, _Type>::type>
    uintx_t<b_bits> operator/(_Type a,
                              const uintx_t<b_bits>& b) {
        uintx_t<b_bits> ret = a;
        ret /= b;
        return ret;
    }

    template<size_t a_bits, size_t b_bits>
    uintx_t<(a_bits > b_bits ? a_bits : b_bits)>
        operator/(const uintx_t<a_bits>& a, const uintx_t<b_bits>& b) {
        uintx_t<(a_bits > b_bits ? a_bits : b_bits)> ret = a;
        ret /= b;
        return ret;
    }

    template<typename _Type, size_t b_bits, typename = std::enable_if<std::is_integral<_Type>::value, _Type>::type>
    uintx_t<b_bits> operator%(_Type a, 
                              const uintx_t<b_bits>& b) {
        uintx_t<b_bits> ret = a;
        ret %= b;
        return ret;
    }

    template<size_t a_bits, size_t b_bits>
    uintx_t<(a_bits > b_bits ? a_bits : b_bits)>
        operator%(const uintx_t<a_bits>& a, const uintx_t<b_bits>& b) {
        uintx_t<(a_bits > b_bits ? a_bits : b_bits)> ret = a;
        ret %= b;
        return ret;
    }

}
