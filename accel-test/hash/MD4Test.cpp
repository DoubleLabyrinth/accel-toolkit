#include "accelpp/hash/MD4.hpp"

#define ConstAsciiString(s) s, sizeof(s) - 1

namespace accel::test {

	bool MD4_CorrectnessTest() {

        // from RFC 1320
        // MD4("") = 31d6cfe0d16ae931b73c59d7e0c089c0
        // MD4("a") = bde52cb31de33e46245e05fbdbd6fb24
        // MD4("abc") = a448017aaf21d8525fc10ae87aa6729d
        // MD4("message digest") = d9130a8164549fe818874806e1c7014b
        // MD4("abcdefghijklmnopqrstuvwxyz") = d79e1c308aa5bbcdeea8ed63df412da9
        // MD4("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") = 043f8582f241db351ce627e153e7f0e4
        // MD4("12345678901234567890123456789012345678901234567890123456789012345678901234567890") = e33b4ddc9c38f2199c3e7b164fcc0536

        accelpp::hash::MD4::HashValue md4_correct_value[7] = {
            { 0x31, 0xd6, 0xcf, 0xe0, 0xd1, 0x6a, 0xe9, 0x31, 0xb7, 0x3c, 0x59, 0xd7, 0xe0, 0xc0, 0x89, 0xc0 },
            { 0xbd, 0xe5, 0x2c, 0xb3, 0x1d, 0xe3, 0x3e, 0x46, 0x24, 0x5e, 0x05, 0xfb, 0xdb, 0xd6, 0xfb, 0x24 },
            { 0xa4, 0x48, 0x01, 0x7a, 0xaf, 0x21, 0xd8, 0x52, 0x5f, 0xc1, 0x0a, 0xe8, 0x7a, 0xa6, 0x72, 0x9d },
            { 0xd9, 0x13, 0x0a, 0x81, 0x64, 0x54, 0x9f, 0xe8, 0x18, 0x87, 0x48, 0x06, 0xe1, 0xc7, 0x01, 0x4b },
            { 0xd7, 0x9e, 0x1c, 0x30, 0x8a, 0xa5, 0xbb, 0xcd, 0xee, 0xa8, 0xed, 0x63, 0xdf, 0x41, 0x2d, 0xa9 },
            { 0x04, 0x3f, 0x85, 0x82, 0xf2, 0x41, 0xdb, 0x35, 0x1c, 0xe6, 0x27, 0xe1, 0x53, 0xe7, 0xf0, 0xe4 },
            { 0xe3, 0x3b, 0x4d, 0xdc, 0x9c, 0x38, 0xf2, 0x19, 0x9c, 0x3e, 0x7b, 0x16, 0x4f, 0xcc, 0x05, 0x36 }
        };

        accelpp::hash::MD4 md4;
        if (md4.Digest() != md4_correct_value[0])
            return false;

        md4.Update("a", 1);
        if (md4.Digest() != md4_correct_value[1])
            return false;

        md4.Update("bc", 2);
        if (md4.Digest() != md4_correct_value[2])
            return false;

        md4.Reset();
        md4.Update(ConstAsciiString("message digest"));
        if (md4.Digest() != md4_correct_value[3])
            return false;

        md4.Reset();
        md4.Update(ConstAsciiString("abcdefghijklmnopqrstuvwxyz"));
        if (md4.Digest() != md4_correct_value[4])
            return false;

        md4.Reset();
        md4.Update(ConstAsciiString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"));
        if (md4.Digest() != md4_correct_value[5])
            return false;

        md4.Reset();
        md4.Update(ConstAsciiString("12345678901234567890123456789012345678901234567890123456789012345678901234567890"));
        if (md4.Digest() != md4_correct_value[6])
            return false;

        const char CustomString[] = "The quick brown fox jumps over the lazy dog";
        accelpp::hash::MD4::HashValue md4_CustomString = { 0x1b, 0xee, 0x69, 0xa4, 0x6b, 0xa8, 0x11, 0x18, 0x5c, 0x19, 0x47, 0x62, 0xab, 0xae, 0xae, 0x90 };
        return accelpp::hash::MD4(ConstAsciiString(CustomString)).Digest() == md4_CustomString;
	}

}