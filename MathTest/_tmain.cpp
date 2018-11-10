#include <tchar.h>
#include <stdio.h>
#include <windows.h>

#define DO_TEST(x)                                      \
    if ((x)()) {                                        \
        _tprintf(TEXT("%-64s passed.\n"), TEXT(#x));    \
    } else {                                            \
        _tprintf(TEXT("%-64s failed!\n"), TEXT(#x));    \
    }

#define MESSAGE(x) _putts(TEXT(x))

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

    bool _umulmodTest();
    bool _udivTest();
    bool _udivmodTest();
    bool _umodTest();

    bool UIntPowerModuleTest0();

    bool NumberTheoTransTest0();

    bool INumberTheoTransTest0();
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
    DO_TEST(accel::MathTest::_umulmodTest);
    DO_TEST(accel::MathTest::_udivTest);
    DO_TEST(accel::MathTest::_udivmodTest);
    DO_TEST(accel::MathTest::_umodTest);

    MESSAGE("");
    DO_TEST(accel::MathTest::UIntPowerModuleTest0);

    MESSAGE("");
    DO_TEST(accel::MathTest::NumberTheoTransTest0);

    MESSAGE("");
    DO_TEST(accel::MathTest::INumberTheoTransTest0);
    return 0;
}
