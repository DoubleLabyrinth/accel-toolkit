cmake_minimum_required(VERSION 3.9)
project(accel_toolkit C)

set(CMAKE_C_STANDARD 11)

set_source_files_properties(
        memory/source/bitwise_sse2.c
        PROPERTIES
        COMPILE_FLAGS "${COMPILE_FLAGS} -msse2"
)

set_source_files_properties(
        encoding/source/base16_ssse3.c
        encoding/source/base32_ssse3.c
        encoding/source/base64_ssse3.c
        memory/source/reverse_ssse3.c
        PROPERTIES
        COMPILE_FLAGS "${COMPILE_FLAGS} -mssse3"
)

set_source_files_properties(
        encoding/source/base16_avx2.c
        encoding/source/base32_avx2.c
        encoding/source/base64_avx2.c
        memory/source/bitwise_avx2.c
        memory/source/reverse_avx2.c
        PROPERTIES
        COMPILE_FLAGS "${COMPILE_FLAGS} -mavx2"
)

set_source_files_properties(
        crypto/source/aes_aesni.c
        PROPERTIES
        COMPILE_FLAGS "${COMPILE_FLAGS} -maes"
)

set(accel_toolkit_src

        encoding/base64.h
        encoding/source/base64.c
        encoding/source/base64_avx2.c
        encoding/source/base64_ssse3.c

        encoding/base32.h
        encoding/source/base32.c
        encoding/source/base32_ssse3.c
        encoding/source/base32_avx2.c

        encoding/base16.h
        encoding/source/base16.c
        encoding/source/base16_ssse3.c
        encoding/source/base16_avx2.c

        crypto/aes.h
        crypto/blowfish.h
        crypto/cast.h
        crypto/des.h
        crypto/rc2.h
        crypto/rc4.h
        crypto/rc5.h
        crypto/tea.h
        crypto/twofish.h
        crypto/source/aes_constant.c
        crypto/source/aes.c
        crypto/source/aes_aesni.c
        crypto/source/blowfish.c
        crypto/source/cast128.c
        crypto/source/des.c
        crypto/source/rc2.c
        crypto/source/rc4.c
        crypto/source/rc5.c
        crypto/source/tea.c
        crypto/source/twofish.c
        crypto/source/twofish_constant.c

        hash/md2.h
        hash/md4.h
        hash/md5.h
        hash/sha1.h
        hash/sha2.h
        hash/source/md2.c
        hash/source/md4.c
        hash/source/md5.c
        hash/source/sha1.c
        hash/source/sha224.c
        hash/source/sha256.c
        hash/source/sha384.c
        hash/source/sha512.c

        math/def.h
        math/def_asm.h
        math/arithmetic.h
        math/compare.h
        math/num_theory.h
        math/ntt/ntt.h
        math/random.h
        math/source/add.c
        math/source/sub.c
        math/source/mul.c
        math/source/mul_Karatsuba.c
        math/source/mul_fntt.c
        math/source/div.c
        math/source/cmp.c
        math/source/gcd.c
        math/source/powmod.c
        math/ntt/source/ntt.c
        math/ntt/source/fntt.c
        math/source/is_prime.c
        math/source/random.c

        memory/bitwise.h
        memory/reverse.h
        memory/source/bitwise.c
        memory/source/bitwise_sse2.c
        memory/source/bitwise_avx2.c
        memory/source/reverse.c
        memory/source/reverse_ssse3.c
        memory/source/reverse_avx2.c

        test/source/math/test_ntt.c
        test/source/math/test_arithmetic.c
)

add_library(accel_toolkit
        SHARED
        ${accel_toolkit_src}
)

add_executable(accel_test ${accel_toolkit_src} main.c)

