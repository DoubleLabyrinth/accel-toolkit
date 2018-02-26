#include "accelpp/hash/MD2.hpp"

namespace accel::test {

	bool MD2_CorrectnessTest() {

        // from RFC 1319
        // MD2 test suite:
        // MD2("") = 8350e5a3e24c153df2275c9f80692773
        // MD2("a") = 32ec01ec4a6dac72c0ab96fb34c0b5d1
        // MD2("abc") = da853b0d3f88d99b30283a69e6ded6bb
        // MD2("message digest") = ab4f496bfb2a530b219ff33031fe06b0
        // MD2("abcdefghijklmnopqrstuvwxyz") = 4e8ddff3650292ab5a4108c3aa47940b
        // MD2("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") = da33def2a42df13975352846c30338cd
        // MD2("12345678901234567890123456789012345678901234567890123456789012345678901234567890") = d5976f79d83d3a0dc9806c3c66f3efd8

        accelpp::hash::MD2::HashValue md2_correct_value[7] = {
            { 0x83, 0x50, 0xe5, 0xa3, 0xe2, 0x4c, 0x15, 0x3d, 0xf2, 0x27, 0x5c, 0x9f, 0x80, 0x69, 0x27, 0x73 },
            { 0x32, 0xec, 0x01, 0xec, 0x4a, 0x6d, 0xac, 0x72, 0xc0, 0xab, 0x96, 0xfb, 0x34, 0xc0, 0xb5, 0xd1 },
            { 0xda, 0x85, 0x3b, 0x0d, 0x3f, 0x88, 0xd9, 0x9b, 0x30, 0x28, 0x3a, 0x69, 0xe6, 0xde, 0xd6, 0xbb },
            { 0xab, 0x4f, 0x49, 0x6b, 0xfb, 0x2a, 0x53, 0x0b, 0x21, 0x9f, 0xf3, 0x30, 0x31, 0xfe, 0x06, 0xb0 },
            { 0x4e, 0x8d, 0xdf, 0xf3, 0x65, 0x02, 0x92, 0xab, 0x5a, 0x41, 0x08, 0xc3, 0xaa, 0x47, 0x94, 0x0b },
            { 0xda, 0x33, 0xde, 0xf2, 0xa4, 0x2d, 0xf1, 0x39, 0x75, 0x35, 0x28, 0x46, 0xc3, 0x03, 0x38, 0xcd },
            { 0xd5, 0x97, 0x6f, 0x79, 0xd8, 0x3d, 0x3a, 0x0d, 0xc9, 0x80, 0x6c, 0x3c, 0x66, 0xf3, 0xef, 0xd8 }
        };

        if (accelpp::hash::MD2("", 0).Digest() != md2_correct_value[0])
            return false;
        
        if (accelpp::hash::MD2("a", 1).Digest() != md2_correct_value[1])
            return false;

        if (accelpp::hash::MD2("abc", 3).Digest() != md2_correct_value[2])
            return false;

        if (accelpp::hash::MD2("message digest", 14).Digest() != md2_correct_value[3])
            return false;

        if (accelpp::hash::MD2("abcdefghijklmnopqrstuvwxyz", 26).Digest() != md2_correct_value[4])
            return false;

        if (accelpp::hash::MD2("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", 62).Digest() != md2_correct_value[5])
            return false;

        if (accelpp::hash::MD2("12345678901234567890123456789012345678901234567890123456789012345678901234567890", 80).Digest() != md2_correct_value[6])
            return false;

		const char CustomString[] = "The quick brown fox jumps over the lazy dog";
        accelpp::hash::MD2::HashValue md2_CustomString = { 0x03, 0xd8, 0x5a, 0x0d, 0x62, 0x9d, 0x2c, 0x44, 0x2e, 0x98, 0x75, 0x25, 0x31, 0x9f, 0xc4, 0x71 };
        return accelpp::hash::MD2(CustomString, sizeof(CustomString) - 1).Digest() == md2_CustomString;
	}

}