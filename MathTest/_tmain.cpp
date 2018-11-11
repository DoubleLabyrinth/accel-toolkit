#include <tchar.h>
#include <stdio.h>
#include <windows.h>

#define MESSAGE(x) _putts(TEXT(x))

#define TIMING_ENABLE 0

#if TIMING_ENABLE == 0
#define DO_TEST(x)                                          \
    if ((x)()) {                                            \
        _tprintf(TEXT("%-72s passed.\n"), TEXT(#x));        \
    } else {                                                \
        _tprintf(TEXT("%-72s failed!\n"), TEXT(#x));        \
    }
#else
#define DO_TEST(x)                                                      \
    {                                                                   \
        bool pass;                                                      \
        LARGE_INTEGER t1, t2;                                           \
        QueryPerformanceCounter(&t1);                                   \
        pass = (x)();                                                   \
        QueryPerformanceCounter(&t2);                                   \
        if (pass) {                                                     \
            _tprintf(TEXT("%-72s passed. PerformanceCounter = %llu\n"), \
                     TEXT(#x), t2.QuadPart - t1.QuadPart);              \
        } else {                                                        \
            _tprintf(TEXT("%-72s failed! PerformanceCounter = %llu\n"), \
                     TEXT(#x), t2.QuadPart - t1.QuadPart);              \
        }                                                               \
    }
#endif

namespace accel::MathTest {
    bool UIntAddTest0();
    bool UIntAddTest1();
    bool UIntAddTest2();
    bool UIntAddTest3();

    bool UIntSubTest0();
    bool UIntSubTest1();
    bool UIntSubTest2();
    bool UIntSubTest3();

    bool UIntMulTest0();
    bool UIntMulTest1();
    bool UIntMulTest2();
    bool UIntMulTest3ST();
    bool UIntMulTest3MT(size_t Threads);

    bool _uaddmodTest();
    bool _usubmodTest();
    bool _umulmodTest();
    bool _udivTest();
    bool _udivmodTest();
    bool _umodTest();

    bool UIntPowerModuleTest0();

    bool NumberTheoTransTest0();
    bool FastNumberTheoTransTest0ST();
    bool FastNumberTheoTransTest0MT(size_t Threads);

    bool INumberTheoTransTest0();
    bool IFastNumberTheoTransTest0ST();
    bool IFastNumberTheoTransTest0MT(size_t Threads);
}

int _tmain(int argc, PTSTR argv[], PTSTR envp[]) {
    DO_TEST(accel::MathTest::UIntAddTest0);
    DO_TEST(accel::MathTest::UIntAddTest1);
    DO_TEST(accel::MathTest::UIntAddTest2);
    DO_TEST(accel::MathTest::UIntAddTest3);

    MESSAGE("");
    DO_TEST(accel::MathTest::UIntSubTest0);
    DO_TEST(accel::MathTest::UIntSubTest1);
    DO_TEST(accel::MathTest::UIntSubTest2);
    DO_TEST(accel::MathTest::UIntSubTest3);

    MESSAGE("");
    DO_TEST(accel::MathTest::UIntMulTest0);
    DO_TEST(accel::MathTest::UIntMulTest1);
    DO_TEST(accel::MathTest::UIntMulTest2); 
    DO_TEST(accel::MathTest::UIntMulTest3ST);
    DO_TEST([]() -> bool { return accel::MathTest::UIntMulTest3MT(2); });
    DO_TEST([]() -> bool { return accel::MathTest::UIntMulTest3MT(4); });
    DO_TEST([]() -> bool { return accel::MathTest::UIntMulTest3MT(8); });

    MESSAGE("");
    DO_TEST(accel::MathTest::_uaddmodTest);
    DO_TEST(accel::MathTest::_usubmodTest);
    DO_TEST(accel::MathTest::_umulmodTest);
    DO_TEST(accel::MathTest::_udivTest);
    DO_TEST(accel::MathTest::_udivmodTest);
    DO_TEST(accel::MathTest::_umodTest);

    MESSAGE("");
    DO_TEST(accel::MathTest::UIntPowerModuleTest0);

    MESSAGE("");
    DO_TEST(accel::MathTest::NumberTheoTransTest0);
    DO_TEST(accel::MathTest::FastNumberTheoTransTest0ST);
    DO_TEST([]() -> bool { return accel::MathTest::FastNumberTheoTransTest0MT(2); });
    DO_TEST([]() -> bool { return accel::MathTest::FastNumberTheoTransTest0MT(4); });
    DO_TEST([]() -> bool { return accel::MathTest::FastNumberTheoTransTest0MT(8); });

    MESSAGE("");
    DO_TEST(accel::MathTest::INumberTheoTransTest0);
    DO_TEST(accel::MathTest::IFastNumberTheoTransTest0ST);
    DO_TEST([]() -> bool { return accel::MathTest::IFastNumberTheoTransTest0MT(2); });
    DO_TEST([]() -> bool { return accel::MathTest::IFastNumberTheoTransTest0MT(4); });
    DO_TEST([]() -> bool { return accel::MathTest::IFastNumberTheoTransTest0MT(8); });
    return 0;
}
