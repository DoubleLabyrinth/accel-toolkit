#include <tchar.h>
#include <stdio.h>
#include <windows.h>

#define DO_TEST(x)                                      \
    if (x()) {                                          \
        _tprintf(TEXT("%s passed.\n"), TEXT(#x));       \
    } else {                                            \
        _tprintf(TEXT("%s --> failed!\n"), TEXT(#x));   \
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
}

int _tmain(int argc, PTSTR argv[], PTSTR envp[]) {
    MESSAGE("-----accel::MathTest::UIntAddTestX-----");
    DO_TEST(accel::MathTest::UIntAddTest0);
    DO_TEST(accel::MathTest::UIntAddTest1);
    DO_TEST(accel::MathTest::UIntAddTest2);
    DO_TEST(accel::MathTest::UIntAddTest3);

    MESSAGE("");
    MESSAGE("-----accel::MathTest::UIntSubTestX-----");
    DO_TEST(accel::MathTest::UIntSubTest0);
    DO_TEST(accel::MathTest::UIntSubTest1);
    DO_TEST(accel::MathTest::UIntSubTest2);
    DO_TEST(accel::MathTest::UIntSubTest3);

    MESSAGE("");
    MESSAGE("-----accel::MathTest::UIntMulTestX-----");
    DO_TEST(accel::MathTest::UIntMulTest0);
    DO_TEST(accel::MathTest::UIntMulTest1);
    DO_TEST(accel::MathTest::UIntMulTest2); 
    DO_TEST(accel::MathTest::UIntMulTest3ST);
    DO_TEST([]() -> bool { return accel::MathTest::UIntMulTest3MT(2); });
    DO_TEST([]() -> bool { return accel::MathTest::UIntMulTest3MT(4); });
    DO_TEST([]() -> bool { return accel::MathTest::UIntMulTest3MT(8); });

    return 0;
}
