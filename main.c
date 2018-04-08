#include "math/arithmetic.h"
#include "math/def_asm.h"
#include <memory.h>
#include <stdio.h>

void accelc_TEST_NTT();
void accelc_TEST_MULTIPLY();

int main(int argc, char* argv[], char* envp[]) {
    accelc_TEST_NTT();
    printf("======accelc_TEST_NTT done======\n");
    accelc_TEST_MULTIPLY();
    return 0;
}