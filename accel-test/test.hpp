#pragma once
#include <tchar.h>
#include <windows.h>

namespace accel::test {

    bool MD2_CorrectnessTest();
    bool MD4_CorrectnessTest();
    bool MD5_CorrectnessTest();
    bool SHA1_CorrectnessTest();
    bool SHA224_CorrectnessTest();
    bool SHA256_CorrectnessTest();
    bool SHA384_CorrectnessTest();
    bool SHA512_CorrectnessTest();

    inline void RunAllTest() {
        if (accel::test::MD2_CorrectnessTest())
            _tprintf_s(TEXT("MD2 passed.\n"));
        else
            _tprintf_s(TEXT("MD2 failed.\n"));

        if (accel::test::MD4_CorrectnessTest())
            _tprintf_s(TEXT("MD4 passed.\n"));
        else
            _tprintf_s(TEXT("MD4 failed.\n"));

        if (accel::test::MD5_CorrectnessTest())
            _tprintf_s(TEXT("MD5 passed.\n"));
        else
            _tprintf_s(TEXT("MD5 failed.\n"));

        if (accel::test::SHA1_CorrectnessTest())
            _tprintf_s(TEXT("SHA1 passed.\n"));
        else
            _tprintf_s(TEXT("SHA1 failed.\n"));

        if (accel::test::SHA224_CorrectnessTest())
            _tprintf_s(TEXT("SHA224 passed.\n"));
        else
            _tprintf_s(TEXT("SHA224 failed.\n"));

        if (accel::test::SHA256_CorrectnessTest())
            _tprintf_s(TEXT("SHA256 passed.\n"));
        else
            _tprintf_s(TEXT("SHA256 failed.\n"));

        if (accel::test::SHA384_CorrectnessTest())
            _tprintf_s(TEXT("SHA384 passed.\n"));
        else
            _tprintf_s(TEXT("SHA384 failed.\n"));

        if (accel::test::SHA512_CorrectnessTest())
            _tprintf_s(TEXT("SHA512 passed.\n"));
        else
            _tprintf_s(TEXT("SHA512 failed.\n"));
    }

}