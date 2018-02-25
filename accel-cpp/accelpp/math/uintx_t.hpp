#pragma once
#include "mathlib/add.h"
#include "mathlib/sub.h"
#include "mathlib/mul.h"
#include "mathlib/div.h"
#include "mathlib/cmp.h"
#include <memory.h>

#if defined(_M_X64)
#define _addcarry_uintr _addcarry_u64
#define _subborrow_uintr _subborrow_u64
#define _mulx_uintr _mulx_u64
#elif defined(_M_IX86)
#define _addcarry_uintr _addcarry_u32
#define _subborrow_uintr _subborrow_u32
#define _mulx_uintr _mulx_u32
#endif

namespace accelpp::math {

    template<size_t bits>
    class uintx_t {

        static_assert(bits > sizeof(uintmax_t) * 8 && (bits & (bits - 1)) == 0, "bits must be larger than (sizeof(uintmax_t) * 8) and must be a power of 2.");

    private:

        //---------------------assign_from_type-------------------------
        template<typename _Type>
        void assign_from_type(_Type x) {
            static_assert(std::is_integral<_Type>::value, "Integral required.");
            if (sizeof(x) <= sizeof(uintr_t)) {
                value[0] = static_cast<uintr_t>(x);
                memset(value + 1,
                       x < static_cast<_Type>(0) ? -1 : 0,
                       sizeof(value) - sizeof(uintr_t));
            } else {
                *reinterpret_cast<_Type*>(value) = x;
                memset(reinterpret_cast<_Type*>(value) + 1,
                       x < static_cast<_Type>(0) ? -1 : 0,
                       sizeof(value) - sizeof(uintr_t));
            }
        }

        //-------------------assign_from_uintrs---------------------------
        template<bool truncate, size_t dst_length, size_t src_length>
        struct assign_from_uintrs {
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]);
        };

        template<size_t dst_length, size_t src_length>
        struct assign_from_uintrs<false, dst_length, src_length> {
            static_assert(dst_length >= src_length, "src buffer is too long.");
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]) {
                memcpy(dst, src, sizeof(src));
                memset(dst + src_length, 0, sizeof(dst) - sizeof(src));
            }
        };

        template<size_t dst_length, size_t src_length>
        struct assign_from_uintrs<true, dst_length, src_length> {
            static_assert(dst_length < src_length, "src buffer is too short.");
            [[deprecated("Warning: Truncation detected while assigning. If this is an error, please add \"#pragma warning(default: 4996)\" to make it a warning.")]]
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]) {
                memcpy(dst, src, sizeof(dst));
            }
        };


        //-------------------add_from_type-----------------------------
        template<typename _Type>
        void add_from_type(_Type x) {
            static_assert(std::is_integral<_Type>::value, "Integer required.");

            uintr_t x_ext[sizeof(x) / sizeof(uintr_t) == 0 ? 1 : sizeof(x) / sizeof(uintr_t)] = { };
            *reinterpret_cast<_Type*>(x_ext) = x < static_cast<_Type>(0) ? -x : x;
            if (x < static_cast<_Type>(0)) {
                MATH_SubBorrow(value, length, x_ext, sizeof(x_ext) / sizeof(uintr_t), 0);
            } else {
                MATH_AddCarry(value, length, x_ext, sizeof(x_ext) / sizeof(uintr_t), 0);
            }
        }

        //-------------------add_from_uintrs---------------------------
        template<bool truncate, size_t dst_length, size_t src_length>
        struct add_from_uintrs {
            static void impl(const uintr_t (&src)[src_length]);
        };

        template<size_t dst_length, size_t src_length>
        struct add_from_uintrs<false, dst_length, src_length> {
            static_assert(dst_length >= src_length, "src buffer is too long.");
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]) {
                MATH_AddCarry(dst, dst_length, src, src_length, 0);
            }
        };

        template<size_t dst_length, size_t src_length>
        struct add_from_uintrs<true, dst_length, src_length> {
            static_assert(dst_length < src_length, "src buffer is too short.");
            [[deprecated("Warning: Truncation detected while adding. If this is an error, please add \"#pragma warning(default: 4996)\" to make it a warning.")]]
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]) {
                MATH_AddCarry(dst, dst_length, src, dst_length, 0);
            }
        };





        //-------------------sub_from_type---------------------------
        template<typename _Type>
        void sub_from_type(_Type x) {
            static_assert(std::is_integral<_Type>::value, "Integer required.");

            uintr_t x_ext[sizeof(x) / sizeof(uintr_t) == 0 ? 1 : sizeof(x) / sizeof(uintr_t)] = { };
            *reinterpret_cast<_Type*>(x_ext) = x < static_cast<_Type>(0) ? -x : x;
            if (x < static_cast<_Type>(0)) {
                MATH_AddCarry(value, length, x_ext, sizeof(x_ext) / sizeof(uintr_t), 0);
            } else {
                MATH_SubBorrow(value, length, x_ext, sizeof(x_ext) / sizeof(uintr_t), 0);
            }
        }

        //-------------------sub_from_uintrs---------------------------
        template<bool truncate, size_t dst_length, size_t src_length>
        struct sub_from_uintrs {
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]);
        };

        template<size_t dst_length, size_t src_length>
        struct sub_from_uintrs<false, dst_length, src_length> {
            static_assert(dst_length >= src_length, "src buffer is too long.");
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]) {
                MATH_SubBorrow(dst, dst_length, src, src_length, 0);
            }
        };

        template<size_t dst_length, size_t src_length>
        struct sub_from_uintrs<true, dst_length, src_length> {
            static_assert(dst_length < src_length, "src buffer is too short.");
            [[deprecated("Warning: Truncation detected while subtracting. If this is an error, please add \"#pragma warning(default: 4996)\" to make it a warning.")]]
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]) {
                MATH_SubBorrow(dst, dst_length, src, dst_length, 0);
            }
        };





        //-------------------mul_from_type-----------------------------
        template<typename _Type>
        void mul_from_type(_Type x) {
            static_assert(std::is_integral<_Type>::value, "Integer required.");

            if (x < static_cast<_Type>(0)) {
                uintr_t x_ext[length];
                uintr_t product[2 * length] = { };
                *reinterpret_cast<_Type*>(x_ext) = x;
                memset(reinterpret_cast<_Type*>(x_ext) + 1, -1, sizeof(x_ext) - sizeof(_Type));
                MATH_Mul_Karatsuba(value, x_ext, length, product);
                memcpy(value, product, sizeof(value));
            } else {
                if (sizeof(x) <= sizeof(uintr_t)) {
                    MATH_MulAssign(value, length, x);
                } else {
                    uintr_t x_ext[sizeof(x) / sizeof(uintr_t)] = { };
                    *reinterpret_cast<_Type*>(x_ext) = x;
                    uintr_t product[length + sizeof(x_ext) / sizeof(uintr_t)];
                    MATH_Mul(x_ext, sizeof(x_ext) / sizeof(uintr_t), value, length, product);
                    memcpy(value, product, sizeof(value));
                }
            }
        }

        //-------------------mul_from_uintrs---------------------------

        template<bool truncate, size_t dst_length, size_t src_length>
        struct mul_from_uintrs {
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]);
        };

        template<size_t dst_length, size_t src_length>
        struct mul_from_uintrs<false, dst_length, src_length> {
            static_assert(dst_length >= src_length, "src buffer is too long.");
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]) {
                uintr_t product[2 * dst_length] = { };

                if (src_length == dst_length) {
                    MATH_Mul_Karatsuba(dst, src, dst_length, product);
                } else {
                    uintr_t src_ext[dst_length];
                    memcpy(src_ext, src, sizeof(src));
                    memset(src_ext + src_length, 0, sizeof(src_ext) - sizeof(src));
                    MATH_Mul_Karatsuba(dst, src_ext, dst_length, product);
                }

                memcpy(dst, product, sizeof(dst));
            }
        };

        template<size_t dst_length, size_t src_length>
        struct mul_from_uintrs<true, dst_length, src_length> {
            static_assert(dst_length < src_length, "src buffer is too short.");
            [[deprecated("Warning: Truncation detected while multiplying. If this is an error, please add \"#pragma warning(default: 4996)\" to make it a warning.")]]
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]) {
                uintr_t product[2 * dst_length] = { };
                MATH_Mul_Karatsuba(dst, src, dst_length, product);
                memcpy(dst, product, sizeof(dst));
            }
        };




        //-------------------div_from_type-----------------------------
        template<typename _Type>
        void div_from_type(_Type x) {
            static_assert(std::is_integral<_Type>::value, "Integer required.");

            if (x < static_cast<_Type>(0)) {
                uintr_t x_ext[length];
                *reinterpret_cast<_Type*>(x_ext) = x;
                memset(reinterpret_cast<_Type*>(x_ext) + 1, -1, sizeof(x_ext) - sizeof(_Type));
                
                uintr_t quotient[length];
                MATH_Div(value, length, x_ext, length, quotient, length);
                memcpy(value, quotient, sizeof(value));
            } else {
                if (sizeof(x) <= sizeof(uintr_t)) {
                    MATH_DivAssign(value, length, static_cast<uintr_t>(x));
                } else {
                    uintr_t x_ext[sizeof(x) / sizeof(uintr_t)];
                    *reinterpret_cast<_Type*>(x_ext) = x;
                    
                    uintr_t quotient[length];
                    MATH_Div(value, length, x_ext, sizeof(x_ext) / sizeof(uintr_t), quotient, length);
                    memcpy(value, quotient, sizeof(value));
                }
            }
        }

        //------------------div_from_uintrs----------------------------
        template<bool truncate, size_t dst_length, size_t src_length>
        struct div_from_uintrs {
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]);
        };

        template<size_t dst_length, size_t src_length>
        struct div_from_uintrs<false, dst_length, src_length> {
            static_assert(dst_length >= src_length, "src buffer is too long.");
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]) {
                uintr_t quotient[dst_length];
                MATH_Div(dst, dst_length, src, src_length, quotient, dst_length);
                memcpy(dst, quotient, sizeof(dst));
            }
        };

        template<size_t dst_length, size_t src_length>
        struct div_from_uintrs<true, dst_length, src_length> {
            static_assert(dst_length < src_length, "src buffer is too short.");
            [[deprecated("Warning: Truncation detected while dividing. If this is an error, please add \"#pragma warning(default: 4996)\" to make it a warning.")]]
            static void impl(uintr_t (&dst)[dst_length], const uintr_t(&src)[src_length]) {
                uintr_t quotient[dst_length];
                MATH_Div(dst, dst_length, src, dst_length, quotient, dst_length);
                memcpy(dst, quotient, sizeof(dst));
            }
        };




        //-------------------mod_from_type-----------------------------
        template<typename _Type>
        void mod_from_type(_Type x) {
            static_assert(std::is_integral<_Type>::value, "Integer required.");

            if (x < static_cast<_Type>(0)) {
                uintr_t x_ext[length];
                *reinterpret_cast<_Type*>(x_ext) = x;
                memset(reinterpret_cast<_Type*>(x_ext) + 1, -1, sizeof(x_ext) - sizeof(_Type));

                uintr_t quotient[length];
                MATH_Div(value, length, x_ext, length, quotient, length);
            } else {
                if (sizeof(x) <= sizeof(uintr_t)) {
                    uintr_t remainder = MATH_DivAssign(value, length, static_cast<uintr_t>(x));
                    memset(value, 0, sizeof(value));
                    value[0] = remainder;
                } else {
                    uintr_t x_ext[sizeof(x) / sizeof(uintr_t)];
                    *reinterpret_cast<_Type*>(x_ext) = x;

                    uintr_t quotient[length];
                    MATH_Div(value, length, x_ext, sizeof(x_ext) / sizeof(uintr_t), quotient, length);
                }
            }
        }

        //-------------------mod_from_uintrs-----------------------------
        template<bool truncate, size_t dst_length, size_t src_length>
        struct mod_from_uintrs {
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]);
        };

        template<size_t dst_length, size_t src_length>
        struct mod_from_uintrs<false, dst_length, src_length> {
            static_assert(dst_length >= src_length, "src buffer is too long.");
            static void impl(uintr_t (&dst)[dst_length], const uintr_t (&src)[src_length]) {
                uintr_t quotient[dst_length];
                MATH_Div(dst, dst_length, src, src_length, quotient, dst_length);
            }
        };

        template<size_t dst_length, size_t src_length>
        struct mod_from_uintrs<true, dst_length, src_length> {
            static_assert(dst_length < src_length, "src buffer is too short.");
            [[deprecated("Warning: Truncation detected while modulo. If this is an error, please add \"#pragma warning(default: 4996)\" to make it a warning.")]]
            static void impl(uintr_t (&dst)[dst_length], const uintr_t(&src)[src_length]) {
                uintr_t quotient[dst_length];
                MATH_Div(dst, dst_length, src, dst_length, quotient, dst_length);
            }
        };



        //-------------------cmp_from_type-----------------------------
        template<typename _Type>
        bool cmp_from_type(_Type x) {
            static_assert(std::is_integral<_Type>::value, "Integer required.");

            if (x < static_cast<_Type>(0)) {
                uintr_t x_ext[length];
                memset(reinterpret_cast<_Type*>(x_ext) + 1, -1, sizeof(x_ext) - sizeof(_Type));
                return MATH_Cmpare(value, length, x_ext, length);
            } else {
                uintr_t x_ext[sizeof(x) / sizeof(uintr_t)];
                *reinterpret_cast<_Type*>(x_ext) = x;
                return MATH_Cmpare(value, length, x_ext, sizeof(x) / sizeof(uintr_t));
            }
        }

        //-----------------shift_right---------------------------------
        void shift_right(size_t shift) {
            if (shift == 0)
                return;

            if (shift >= bits) {
                memset(value, 0, sizeof(value));
                return;
            }

            size_t shift_uintrs = shift / (sizeof(uintr_t) * 8);
            size_t shift_bits = shift % (sizeof(uintr_t) * 8);
            uintr_t* cur_ptr = value;

            for (size_t i = shift_uintrs; i < length - 1; ++i) {
                *cur_ptr = (value[i] >> shift_bits) | (value[i + 1] << (sizeof(uintr_t) * 8 - shift_bits));
                ++cur_ptr;
            }

            *cur_ptr = value[length - 1] >> shift_bits;
            ++cur_ptr;
            memset(cur_ptr, 0, ((value + length) - cur_ptr) * sizeof(uintr_t));
        }

        //-----------------shift_left---------------------------------
        void shift_left(size_t shift) {
            if (shift == 0)
                return;

            if (shift >= bits) {
                memset(value, 0, sizeof(value));
                return;
            }

            size_t shift_uintrs = shift / (sizeof(uintr_t) * 8);
            size_t shift_bits = shift % (sizeof(uintr_t) * 8);
            uintr_t* cur_ptr = value + length;

            for (size_t i = length - shift_uintrs - 1; i > 0; --i) {
                --cur_ptr;
                *cur_ptr = (value[i] << shift_bits) | (value[i - 1] >> (sizeof(uintr_t) * 8 - shift_bits));
            }

            --cur_ptr;
            *cur_ptr = value[0] << shift_bits;
            memset(value, 0, (cur_ptr - value) * sizeof(uintr_t));
        }

    public:
        static constexpr size_t length = bits / (sizeof(uintr_t) * 8);

        uintr_t value[length];

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

        uintx_t() { }

        template<typename _Type>
        uintx_t(_Type x) {
            assign_from_type<_Type>(x);
        }

        template<size_t src_length>
        uintx_t(const uintr_t (&src)[src_length]) {
            assign_from_uintrs<(src_length > length), length, src_length>::impl(value, src);
        }

        uintx_t(const void* bytes_ptr, size_t bytes_length) {
            assign_from_bytes(bytes_ptr, bytes_length);
        }

        template<size_t others_bits>
        uintx_t(const uintx_t<others_bits>& other) {
            assign_from_uintrs<(others_bits > bits), length, other.length>::impl(value, other.value);
        }

        //------------------------------
        //      operator!=
        //------------------------------

        template<typename _Type>
        bool operator!=(_Type x) const {
            return cmp_from_type<_Type>(x) != 0;
        }

        template<size_t others_bits>
        bool operator!=(const uintx_t<others_bits>& other) const {
            return MATH_Cmpare(value, length, other.value, other.length) != 0;
        }

        //------------------------------
        //      operator==
        //------------------------------

        template<typename _Type>
        bool operator==(_Type x) const {
            return cmp_from_type<_Type>(x) == 0;
        }

        template<size_t others_bits>
        bool operator==(const uintx_t<others_bits>& other) const {
            return MATH_Cmpare(value, length, other.value, other.length) == 0;
        }

        //------------------------------
        //      operator<
        //------------------------------

        template<typename _Type>
        bool operator<(_Type x) const {
            return cmp_from_type<_Type>(x) == -1;
        }

        template<size_t others_bits>
        bool operator<(const uintx_t<others_bits>& other) const {
            return MATH_Cmpare(value, length, other.value, other.length) == -1;
        }

        //------------------------------
        //      operator>
        //------------------------------

        template<typename _Type>
        bool operator>(_Type x) const {
            return cmp_from_type<_Type>(x) == 1;
        }

        template<size_t others_bits>
        bool operator>(const uintx_t<others_bits>& other) const {
            return MATH_Cmpare(value, length, other.value, other.length) == 1;
        }

        //------------------------------
        //      operator<<
        //------------------------------

        uintx_t<bits> operator<<(size_t shift) const {
            uintx_t<bits> ret = *this;
            ret.shift_left(shift);
            return ret;
        }

        //------------------------------
        //      operator>>
        //------------------------------

        uintx_t<bits> operator>>(size_t shift) const {
            uintx_t<bits> ret = *this;
            ret.shift_right(shift);
            return ret;
        }

        //------------------------------
        //      operator=
        //------------------------------

        template<typename _Type>
        uintx_t<bits>& operator=(_Type x) {
            assign_from_type<_Type>(x);
            return *this;
        }

        template<size_t others_bits>
        uintx_t<bits>& operator=(const uintx_t<others_bits>& other) {
            if (&other != this)
                assign_from_uintrs<(others_bits > bits), length, other.length>::impl(value, other.value);
            return *this;
        }

        //------------------------------
        //      operator+=
        //------------------------------

        template<typename _Type>
        uintx_t<bits>& operator+=(_Type x) {
            add_from_type<_Type>(x);
            return *this;
        }

        template<size_t others_bits>
        uintx_t<bits>& operator+=(const uintx_t<others_bits>& other) {
            add_from_uintrs<(others_bits > bits), length, other.length>::impl(value, other.value);
            return *this;
        }

        //------------------------------
        //      operator-=
        //------------------------------

        template<typename _Type>
        uintx_t<bits>& operator-=(_Type x) {
            sub_from_type<_Type>(x);
            return *this;
        }

        template<size_t others_bits>
        uintx_t<bits>& operator-=(const uintx_t<others_bits>& other) {
            sub_from_uintrs<(others_bits > bits), length, other.length>::impl(value, other.value);
            return *this;
        }

        //------------------------------
        //      operator*=
        //------------------------------

        template<typename _Type>
        uintx_t<bits>& operator*=(_Type x) {
            mul_from_type(x);
            return *this;
        }

        template<size_t others_bits>
        uintx_t<bits>& operator*=(const uintx_t<others_bits>& other) {
            mul_from_uintrs<(others_bits > bits), length, other.length>::impl(value, other.value);
            return *this;
        }

        //------------------------------
        //      operator/=
        //------------------------------

        template<typename _Type>
        uintx_t<bits>& operator/=(_Type x) {
            div_from_type(x);
            return *this;
        }

        template<size_t others_bits>
        uintx_t<bits>& operator/=(const uintx_t<others_bits>& other) {
            div_from_uintrs<(others_bits > bits), length, other.length>::impl(value, other.value);
            return *this;
        }

        //------------------------------
        //      operator%=
        //------------------------------

        template<typename _Type>
        uintx_t<bits>& operator%=(_Type x) {
            mod_from_type(x);
            return *this;
        }

        template<size_t others_bits>
        uintx_t<bits>& operator%=(const uintx_t<others_bits>& other) {
            mod_from_uintrs<(others_bits > bits), length, other.length>::impl(value, other.value);
            return *this;
        }

        //------------------------------
        //      operator<<=
        //------------------------------

        uintx_t<bits>& operator<<=(size_t shift) {
            shift_left(shift);
            return *this;
        }

        //------------------------------
        //      operator>>=
        //------------------------------

        uintx_t<bits>& operator>>=(size_t shift) {
            shift_right(shift);
            return *this;
        }

        //------------------------------
        //      operator+
        //------------------------------

        template<typename _Type>
        friend uintx_t<bits> operator+(_Type a, const uintx_t<bits>& b);

        template<typename _Type>
        friend uintx_t<bits> operator+(const uintx_t<bits>& a, _Type b);

        template<size_t others_bits>
        friend uintx_t<(bits > others_bits ? bits : others_bits)> 
            operator+(const uintx_t<bits>& a, const uintx_t<others_bits>& b);


        //------------------------------
        //      operator-
        //------------------------------

        template<typename _Type>
        friend uintx_t<bits> operator-(_Type a, const uintx_t<bits>& b);

        template<typename _Type>
        friend uintx_t<bits> operator-(const uintx_t<bits>& a, _Type b);

        template<size_t others_bits>
        friend uintx_t<(bits > others_bits ? bits : others_bits)>
            operator-(const uintx_t<bits>& a, const uintx_t<others_bits>& b);

        //------------------------------
        //      operator*
        //------------------------------

        template<typename _Type>
        friend uintx_t<bits> operator*(_Type a, const uintx_t<bits>& b);

        template<typename _Type>
        friend uintx_t<bits> operator*(const uintx_t<bits>& a, _Type b);

        template<size_t others_bits>
        friend uintx_t<(bits > others_bits ? bits : others_bits)>
            operator*(const uintx_t<bits>& a, const uintx_t<others_bits>& b);

        //------------------------------
        //      operator/
        //------------------------------

        template<typename _Type>
        friend uintx_t<bits> operator/(_Type a, const uintx_t<bits>& b);

        template<typename _Type>
        friend uintx_t<bits> operator/(const uintx_t<bits>& a, _Type b);

        template<size_t others_bits>
        friend uintx_t<(bits > others_bits ? bits : others_bits)>
            operator/(const uintx_t<bits>& a, const uintx_t<others_bits>& b);

        //------------------------------
        //      operator%
        //------------------------------

        template<typename _Type>
        friend uintx_t<bits> operator%(_Type a, const uintx_t<bits>& b);

        template<typename _Type>
        friend uintx_t<bits> operator%(const uintx_t<bits>& a, _Type b);

        template<size_t others_bits>
        friend uintx_t<(bits > others_bits ? bits : others_bits)>
            operator%(const uintx_t<bits>& a, const uintx_t<others_bits>& b);

    };





    template<size_t bits, typename _Type>
    uintx_t<bits> operator+(_Type a, const uintx_t<bits>& b) {
        uintx_t<bits> ret = b;
        ret.add_from_type(a);
        return ret;
    }

    template<size_t bits, typename _Type>
    uintx_t<bits> operator+(const uintx_t<bits>& a, _Type b) {
        uintx_t<bits> ret = a;
        ret.add_from_type(b);
        return ret;
    }

    template<size_t bits, size_t others_bits>
    uintx_t<(bits > others_bits ? bits : others_bits)> 
        operator+(const uintx_t<bits>& a, const uintx_t<others_bits>& b) {
        
        uintx_t<(bits > others_bits ? bits : others_bits)> ret;
        unsigned char carry = 0;
        if (a.length > b.length) {
            for (size_t i = 0; i < b.length; ++i)
                carry = _addcarry_uintr(carry, a.value[i], b.value[i], ret.value + i);
            memcpy(ret.value + b.length, a.value + b.length, sizeof(a.value) - sizeof(b.value));
            for (size_t i = b.length; carry != 0 && i < a.length; ++i)
                carry = _addcarry_uintr(carry, ret.value[i], 0, ret.value + i);
        } else {
            for (size_t i = 0; i < a.length; ++i)
                carry = _addcarry_uintr(carry, a.value[i], b.value[i], ret.value + i);
            memcpy(ret.value + a.length, b.value + a.length, sizeof(b.value) - sizeof(a.value));
            for (size_t i = a.length; carry != 0 && i < b.length; ++i)
                carry = _addcarry_uintr(carry, ret.value[i], 0, ret.value + i);
        }

        return ret;
    }




    template<size_t bits, typename _Type>
    uintx_t<bits> operator-(_Type a, const uintx_t<bits>& b) {
        uintx_t<bits> ret = a;
        ret::sub_from_uintrs<false, ret.length, b.length>::impl(ret.value, b.value);
        return ret;
    }

    template<size_t bits, typename _Type>
    uintx_t<bits> operator-(const uintx_t<bits>& a, _Type b) {
        uintx_t<bits> ret = a;
        ret.sub_from_type(b);
        return ret;
    }

    template<size_t bits, size_t others_bits>
    uintx_t<(bits > others_bits ? bits : others_bits)>
        operator-(const uintx_t<bits>& a, const uintx_t<others_bits>& b) {

        uintx_t<(bits > others_bits ? bits : others_bits)> ret;
        unsigned char borrow = 0;

        if (a.length > b.length) {
            for (size_t i = 0; i < b.length; ++i)
                borrow = _subborrow_uintr(borrow, a.value[i], b.value[i], ret.value + i);
            memcpy(ret.value + b.length, a.value + b.length, sizeof(a.value) - sizeof(b.value));
            for (size_t i = b.length; borrow != 0 && i < ret.length; ++i)
                borrow = _subborrow_uintr(borrow, ret.value[i], 0, ret.value + i);
        } else {
            size_t i = 0;
            for (; i < a.length; ++i)
                borrow = _subborrow_uintr(borrow, a.value[i], b.value[i], ret.value + i);
            for (; borrow != 0 && i < ret.length; ++i)
                borrow = _subborrow_uintr(borrow, 0, b.value[i], ret.value + i);
            memset(ret.value + i, 0, sizeof(ret.value) - i * sizeof(uintr_t));
        }

        return ret;
    }




    template<size_t bits, typename _Type>
    uintx_t<bits> operator*(_Type a, const uintx_t<bits>& b) {
        uintx_t<bits> ret = b;
        ret.mul_from_type(a);
        return ret;
    }

    template<size_t bits, typename _Type>
    uintx_t<bits> operator*(const uintx_t<bits>& a, _Type b) {
        uintx_t<bits> ret = a;
        ret.mul_from_type(b);
        return ret;
    }

    template<size_t bits, size_t others_bits>
    uintx_t<(bits > others_bits ? bits : others_bits)>
        operator*(const uintx_t<bits>& a, const uintx_t<others_bits>& b) {

        if (a.length == b.length) {
            uintr_t product[2 * a.length];
            uintrs_mul_uintrs_Karatsuba(a, b, product);
            return *reinterpret_cast<uintx_t<bits>*>(product);
        } else if(a.length < b.length) {
            uintx_t<others_bits> ret = a;
            ret::mul_from_uintrs<false, ret.length, b.length>::impl(ret.value, b.value);
            return ret;
        } else {
            uintx_t<bits> ret = b;
            ret::mul_from_uintrs<false, ret.length, a.length>::impl(ret.value, a.value);
            return ret;
        }
    }




    template<size_t bits, typename _Type>
    uintx_t<bits> operator/(_Type a, const uintx_t<bits>& b) {
        uintx_t<bits> ret = a;
        ret::div_from_uintrs<false, b.length>(b.value);
        return ret;
    }

    template<size_t bits, typename _Type>
    uintx_t<bits> operator/(const uintx_t<bits>& a, _Type b) {
        uintx_t<bits> ret = a;
        ret.div_from_type(b);
        return ret;
    }

    template<size_t bits, size_t others_bits>
    uintx_t<(bits > others_bits ? bits : others_bits)>
        operator/(const uintx_t<bits>& a, const uintx_t<others_bits>& b) {

        uintx_t<(bits > others_bits ? bits : others_bits)> ret = a;
        ret::div_from_uintrs<false, ret.length, b.length>::impl(ret.value, b.value);
        return ret;
    }




    template<size_t bits, typename _Type>
    uintx_t<bits> operator%(_Type a, const uintx_t<bits>& b) {
        uintx_t<bits> ret = a;
        ret::mod_from_uintrs<false, b.length>(b.value);
        return ret;
    }

    template<size_t bits, typename _Type>
    uintx_t<bits> operator%(const uintx_t<bits>& a, _Type b) {
        uintx_t<bits> ret = a;
        ret.mod_from_type(b);
        return ret;
    }

    template<size_t bits, size_t others_bits>
    uintx_t<(bits > others_bits ? bits : others_bits)>
        operator%(const uintx_t<bits>& a, const uintx_t<others_bits>& b) {

        uintx_t<(bits > others_bits ? bits : others_bits)> ret = a;
        ret::mod_from_uintrs<false, ret.length, b.length>::impl(ret.value, b.value);
        return ret;
    }

}

#undef _addcarry_uintr
#undef _subborrow_uintr
#undef _mulx_uintr