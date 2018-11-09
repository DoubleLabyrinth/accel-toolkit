#include "../UIntAddition.hpp"
#include "../UIntSubtraction.hpp"
#include "../UIntMultiplication.hpp"
#include <intrin.h>
#include <malloc.h>
#include <windows.h>

namespace accel::Math {

#if defined(_M_IX86) || defined(_M_X64)

#if defined(_M_IX86)
#define _addcarry_uX _addcarry_u32
#define uintX_t uint32_t
#else
#define _addcarry_uX _addcarry_u64
#define uintX_t uint64_t
#endif

#define KARATSUBA_CUT_LENGTH 8

    extern
    void KaratsubaSTInternal(const uintX_t a[], const uintX_t b[],
                               size_t len,
                               uintX_t product[]);

    struct KaratsubaArgs {
        const uintX_t* a;
        const uintX_t* b;
        size_t len;
        uintX_t* product;
        size_t NumberOfThreads;
    };

    struct UIntAddArgs {
        const uintX_t* a;
        size_t a_len;
        const uintX_t* b;
        size_t b_len;
        uintX_t* sum;
        size_t sum_len;
        uint8_t* p_carry;
    };

    static DWORD WINAPI KaratsubaUIntAddWrapper(PVOID pUIntAddArgs) {
        UIntAddArgs* pArgs = reinterpret_cast<UIntAddArgs*>(pUIntAddArgs);
        *pArgs->p_carry = UIntAdd(pArgs->a, pArgs->a_len,
                                  pArgs->b, pArgs->b_len,
                                  pArgs->sum, pArgs->sum_len,
                                  0);
        return 0;
    }

    DWORD WINAPI KaratsubaMTInternal(PVOID pKaratsubaArgs) {
        KaratsubaArgs* pArgs = reinterpret_cast<KaratsubaArgs*>(pKaratsubaArgs);

        if (pArgs->len <= KARATSUBA_CUT_LENGTH) {
            UIntMul(pArgs->a, pArgs->len,
                    pArgs->b, pArgs->len,
                    pArgs->product, 2 * pArgs->len);
            return 0;
        }

        if (pArgs->NumberOfThreads >= 2) {
            HANDLE Threads[2];
            KaratsubaArgs Args[2];

            Args[0].a = pArgs->a;
            Args[0].b = pArgs->b;
            Args[0].len = pArgs->len / 2;
            Args[0].product = pArgs->product;
            Args[0].NumberOfThreads = pArgs->NumberOfThreads / 2;

            Args[1].a = pArgs->a + pArgs->len / 2;
            Args[1].b = pArgs->b + pArgs->len / 2;
            Args[1].len = pArgs->len / 2;
            Args[1].product = pArgs->product + pArgs->len;
            Args[1].NumberOfThreads = pArgs->NumberOfThreads / 2;

            Threads[0] = CreateThread(NULL, 0, KaratsubaMTInternal, Args + 0, 0, NULL);
            Threads[1] = CreateThread(NULL, 0, KaratsubaMTInternal, Args + 1, 0, NULL);
            WaitForMultipleObjects(2, Threads, TRUE, INFINITE);
        } else {
            KaratsubaSTInternal(pArgs->a,
                                pArgs->b,
                                pArgs->len / 2,
                                pArgs->product);
            KaratsubaSTInternal(pArgs->a + pArgs->len / 2,
                                pArgs->b + pArgs->len / 2,
                                pArgs->len / 2,
                                pArgs->product + pArgs->len);
        }

        uintX_t* buffer = reinterpret_cast<uintX_t*>(malloc(pArgs->len * 2 * sizeof(uintX_t)));

        uint8_t carry_a, carry_b;
        if (pArgs->NumberOfThreads >= 2) {
            HANDLE Threads[2];
            UIntAddArgs Args[2];

            Args[0].a = pArgs->a;
            Args[0].a_len = pArgs->len / 2;
            Args[0].b = pArgs->a + pArgs->len / 2;
            Args[0].b_len = pArgs->len / 2;
            Args[0].sum = buffer;
            Args[0].sum_len = pArgs->len / 2;
            Args[0].p_carry = &carry_a;

            Args[1].a = pArgs->b;
            Args[1].a_len = pArgs->len / 2;
            Args[1].b = pArgs->b + pArgs->len / 2;
            Args[1].b_len = pArgs->len / 2;
            Args[1].sum = buffer + pArgs->len / 2;
            Args[1].sum_len = pArgs->len / 2;
            Args[1].p_carry = &carry_b;

            Threads[0] = CreateThread(NULL, 0, KaratsubaUIntAddWrapper, Args + 0, 0, NULL);
            Threads[1] = CreateThread(NULL, 0, KaratsubaUIntAddWrapper, Args + 1, 0, NULL);
            WaitForMultipleObjects(2, Threads, TRUE, INFINITE);
        } else {
            carry_a = UIntAdd(pArgs->a, pArgs->len / 2,
                              pArgs->a + pArgs->len / 2, pArgs->len / 2,
                              buffer, pArgs->len / 2,
                              0);
            carry_b = UIntAdd(pArgs->b, pArgs->len / 2,
                              pArgs->b + pArgs->len / 2, pArgs->len / 2,
                              buffer + pArgs->len / 2, pArgs->len / 2,
                              0);
        }

        uint8_t carry = 0;
        KaratsubaSTInternal(buffer,
                            buffer + pArgs->len / 2,
                            pArgs->len / 2,
                            buffer + pArgs->len);

        if (carry_a)
            carry += UIntAdd(buffer + pArgs->len + pArgs->len / 2, pArgs->len / 2,
                             buffer + pArgs->len / 2, pArgs->len / 2,
                             0);

        if (carry_b)
            carry += UIntAdd(buffer + pArgs->len + pArgs->len / 2, pArgs->len / 2,
                             buffer, pArgs->len / 2,
                             0);

        if (carry_a && carry_b)
            carry++;

        carry -= UIntSub(buffer + pArgs->len, pArgs->len,
                         pArgs->product, pArgs->len,
                         0);

        carry -= UIntSub(buffer + pArgs->len, pArgs->len,
                         pArgs->product + pArgs->len, pArgs->len,
                         0);

        UIntAdd(pArgs->product + pArgs->len / 2, pArgs->len + pArgs->len / 2,
                buffer + pArgs->len, pArgs->len,
                0);

        UIntAdd(pArgs->product + pArgs->len + pArgs->len / 2, pArgs->len / 2,
                0,
                carry);

        free(buffer);
        return 0;
    }

    // Equivalent to "product = a * b;"
    // Karatsuba multiplication,  time complexity O(n ^ log_2(3)) ~= O(n ^ 1.585)
    // Parameter 'length' is the length of both a and b
    // ASSERT:
    // 1. len is a power of 2
    // 2. product_len >= 2 * len
    // 3. NumberOfThreads is a power of 2 and cannot be 0
    bool UIntMulKaratsuba(const uintX_t a[], const uintX_t b[], size_t len,
                          uintX_t product[], size_t product_len, size_t NumberOfThreads) {
        // if len is not a power of 2, just return false
        if (len & (len - 1))
            return false;

        // if product buffer is too small, just return false
        if (product_len < 2 * len)
            return false;

        // NumberOfThreads cannot be 0
        if (NumberOfThreads == 0) 
            return false;

        // NumberOfThreads must be a power of 2
        if (NumberOfThreads & (NumberOfThreads - 1))
            return false;

        if (NumberOfThreads == 1) {
            KaratsubaSTInternal(a, b, len, product);
        } else {
            KaratsubaArgs Arg;
            Arg.a = a;
            Arg.b = b;
            Arg.len = len;
            Arg.product = product;
            Arg.NumberOfThreads = NumberOfThreads;
            KaratsubaMTInternal(&Arg);
        }

        if (product_len > len * 2)
            memset(product + 2 * len, 0, (product_len - 2 * len) * sizeof(uintX_t));

        return true;
    }

#endif

}
