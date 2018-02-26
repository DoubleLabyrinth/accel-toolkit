#include "accelpp/hash/SHA224.hpp"

#define ConstAsciiString(s) s, sizeof(s) - 1

namespace accel::test {

	bool SHA224_CorrectnessTest() {

        // Generated by Python 3.6.3
        // SHA224("") = d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f
        // SHA224("a") = abd37534c7d9a2efb9465de931cd7055ffdb8879563ae98078d6d6d5
        // SHA224("abc") = 23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7
        // SHA224("message digest") = 2cb21c83ae2f004de7e81c3c7019cbcb65b71ab656b22d6d0c39b8eb
        // SHA224("abcdefghijklmnopqrstuvwxyz") = 45a5f72c39c5cff2522eb3429799e49e5f44b356ef926bcf390dccc2
        // SHA224("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") = bff72b4fcb7d75e5632900ac5f90d219e05e97a7bde72e740db393d9
        // SHA224("12345678901234567890123456789012345678901234567890123456789012345678901234567890") = b50aecbe4e9bb0b57bc5f3ae760a8e01db24f203fb3cdcd13148046e

        accelpp::hash::SHA224::HashValue sha224_correct_value[7] = {
            { 0xd1, 0x4a, 0x02, 0x8c, 0x2a, 0x3a, 0x2b, 0xc9, 0x47, 0x61, 0x02, 0xbb, 0x28, 0x82, 0x34, 0xc4, 0x15, 0xa2, 0xb0, 0x1f, 0x82, 0x8e, 0xa6, 0x2a, 0xc5, 0xb3, 0xe4, 0x2f },
            { 0xab, 0xd3, 0x75, 0x34, 0xc7, 0xd9, 0xa2, 0xef, 0xb9, 0x46, 0x5d, 0xe9, 0x31, 0xcd, 0x70, 0x55, 0xff, 0xdb, 0x88, 0x79, 0x56, 0x3a, 0xe9, 0x80, 0x78, 0xd6, 0xd6, 0xd5 },
            { 0x23, 0x09, 0x7d, 0x22, 0x34, 0x05, 0xd8, 0x22, 0x86, 0x42, 0xa4, 0x77, 0xbd, 0xa2, 0x55, 0xb3, 0x2a, 0xad, 0xbc, 0xe4, 0xbd, 0xa0, 0xb3, 0xf7, 0xe3, 0x6c, 0x9d, 0xa7 },
            { 0x2c, 0xb2, 0x1c, 0x83, 0xae, 0x2f, 0x00, 0x4d, 0xe7, 0xe8, 0x1c, 0x3c, 0x70, 0x19, 0xcb, 0xcb, 0x65, 0xb7, 0x1a, 0xb6, 0x56, 0xb2, 0x2d, 0x6d, 0x0c, 0x39, 0xb8, 0xeb },
            { 0x45, 0xa5, 0xf7, 0x2c, 0x39, 0xc5, 0xcf, 0xf2, 0x52, 0x2e, 0xb3, 0x42, 0x97, 0x99, 0xe4, 0x9e, 0x5f, 0x44, 0xb3, 0x56, 0xef, 0x92, 0x6b, 0xcf, 0x39, 0x0d, 0xcc, 0xc2 },
            { 0xbf, 0xf7, 0x2b, 0x4f, 0xcb, 0x7d, 0x75, 0xe5, 0x63, 0x29, 0x00, 0xac, 0x5f, 0x90, 0xd2, 0x19, 0xe0, 0x5e, 0x97, 0xa7, 0xbd, 0xe7, 0x2e, 0x74, 0x0d, 0xb3, 0x93, 0xd9 },
            { 0xb5, 0x0a, 0xec, 0xbe, 0x4e, 0x9b, 0xb0, 0xb5, 0x7b, 0xc5, 0xf3, 0xae, 0x76, 0x0a, 0x8e, 0x01, 0xdb, 0x24, 0xf2, 0x03, 0xfb, 0x3c, 0xdc, 0xd1, 0x31, 0x48, 0x04, 0x6e }
        };

        accelpp::hash::SHA224 sha224;
        if (sha224.Digest() != sha224_correct_value[0])
            return false;

        sha224.Update("a", 1);
        if (sha224.Digest() != sha224_correct_value[1])
            return false;

        sha224.Update("bc", 2);
        if (sha224.Digest() != sha224_correct_value[2])
            return false;

        sha224.Reset();
        sha224.Update(ConstAsciiString("message digest"));
        if (sha224.Digest() != sha224_correct_value[3])
            return false;

        sha224.Reset();
        sha224.Update(ConstAsciiString("abcdefghijklmnopqrstuvwxyz"));
        if (sha224.Digest() != sha224_correct_value[4])
            return false;

        sha224.Reset();
        sha224.Update(ConstAsciiString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"));
        if (sha224.Digest() != sha224_correct_value[5])
            return false;

        sha224.Reset();
        sha224.Update(ConstAsciiString("12345678901234567890123456789012345678901234567890123456789012345678901234567890"));
        if (sha224.Digest() != sha224_correct_value[6])
            return false;

        const char CustomString[] = "The quick brown fox jumps over the lazy dog";
        accelpp::hash::SHA224::HashValue sha224_CustomString = { 0x73, 0x0e, 0x10, 0x9b, 0xd7, 0xa8, 0xa3, 0x2b, 0x1c, 0xb9, 0xd9, 0xa0, 0x9a, 0xa2, 0x32, 0x5d,
                                                               0x24, 0x30, 0x58, 0x7d, 0xdb, 0xc0, 0xc3, 0x8b, 0xad, 0x91, 0x15, 0x25 };
        return accelpp::hash::SHA224(ConstAsciiString(CustomString)).Digest() == sha224_CustomString;
	}

}

