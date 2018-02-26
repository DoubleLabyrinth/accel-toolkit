#include "accelpp/hash/MD5.hpp"

#define ConstAsciiString(s) s, sizeof(s) - 1

namespace accel::test {

	bool MD5_CorrectnessTest() {

        //f from RFC 1321
        // MD5("") = d41d8cd98f00b204e9800998ecf8427e
        // MD5("a") = 0cc175b9c0f1b6a831c399e269772661
        // MD5("abc") = 900150983cd24fb0d6963f7d28e17f72
        // MD5("message digest") = f96b697d7cb7938d525a2f31aaf161d0
        // MD5("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
        // MD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") = d174ab98d277d9f5a5611c2c9f419d9f
        // MD5("12345678901234567890123456789012345678901234567890123456789012345678901234567890") = 57edf4a22be3c955ac49da2e2107b67a

        accelpp::hash::MD5::HashValue md5_correct_value[7] = {
            { 0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04, 0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e },
            { 0x0c, 0xc1, 0x75, 0xb9, 0xc0, 0xf1, 0xb6, 0xa8, 0x31, 0xc3, 0x99, 0xe2, 0x69, 0x77, 0x26, 0x61 },
            { 0x90, 0x01, 0x50, 0x98, 0x3c, 0xd2, 0x4f, 0xb0, 0xd6, 0x96, 0x3f, 0x7d, 0x28, 0xe1, 0x7f, 0x72 },
            { 0xf9, 0x6b, 0x69, 0x7d, 0x7c, 0xb7, 0x93, 0x8d, 0x52, 0x5a, 0x2f, 0x31, 0xaa, 0xf1, 0x61, 0xd0 },
            { 0xc3, 0xfc, 0xd3, 0xd7, 0x61, 0x92, 0xe4, 0x00, 0x7d, 0xfb, 0x49, 0x6c, 0xca, 0x67, 0xe1, 0x3b },
            { 0xd1, 0x74, 0xab, 0x98, 0xd2, 0x77, 0xd9, 0xf5, 0xa5, 0x61, 0x1c, 0x2c, 0x9f, 0x41, 0x9d, 0x9f },
            { 0x57, 0xed, 0xf4, 0xa2, 0x2b, 0xe3, 0xc9, 0x55, 0xac, 0x49, 0xda, 0x2e, 0x21, 0x07, 0xb6, 0x7a }
        };

        accelpp::hash::MD5 md5;
        if (md5.Digest() != md5_correct_value[0])
            return false;

        md5.Update("a", 1);
        if (md5.Digest() != md5_correct_value[1])
            return false;

        md5.Update("bc", 2);
        if (md5.Digest() != md5_correct_value[2])
            return false;

        md5.Reset();
        md5.Update("message digest", 14);
        if (md5.Digest() != md5_correct_value[3])
            return false;

        md5.Reset();
        md5.Update(ConstAsciiString("abcdefghijklmnopqrstuvwxyz"));
        if (md5.Digest() != md5_correct_value[4])
            return false;

        md5.Reset();
        md5.Update(ConstAsciiString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"));
        if (md5.Digest() != md5_correct_value[5])
            return false;

        md5.Reset();
        md5.Update(ConstAsciiString("12345678901234567890123456789012345678901234567890123456789012345678901234567890"));
        if (md5.Digest() != md5_correct_value[6])
            return false;

        const char CustomString[] = "The quick brown fox jumps over the lazy dog";
        accelpp::hash::MD5::HashValue md5_CustomString = { 0x9e, 0x10, 0x7d, 0x9d, 0x37, 0x2b, 0xb6, 0x82, 0x6b, 0xd8, 0x1d, 0x35, 0x42, 0xa4, 0x19, 0xd6 };
        return accelpp::hash::MD5(ConstAsciiString(CustomString)).Digest() == md5_CustomString;
	}

}