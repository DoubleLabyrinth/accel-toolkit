#include "../rc4.h"

void accelc_RC4_process(uint8_t srcBytes[], size_t srcBytesLength,
                        RC4_STATE* srcState) {
    int i = 0, j = 0;
    for (size_t k = 0; k < srcBytesLength; ++k) {
        i = (i + 1) % 256;
        j = (j + srcState->SBox[i]) % 256;
        uint8_t temp = srcState->SBox[i];
        srcState->SBox[i] = srcState->SBox[j];
        srcState->SBox[j] = temp;
        srcBytes[k] ^= srcState->SBox[(srcState->SBox[i] + srcState->SBox[j]) % 256];
    }
}

int accelc_RC4_set_key(const uint8_t srcKey[], size_t srcKeyLength,
                       RC4_KEY* dstKey) {
    if (srcKeyLength == 0)
        return CRYPTO_RC4_INVALID_KEY_LENGTH;
    if (srcKeyLength > CRYPTO_RC4_MAX_KEY_LENGTH)
        return CRYPTO_RC4_KEY_TOO_LONG;

    for (int i = 0; i < 256; ++i)
        dstKey->SBox[i] = srcKey[i % srcKeyLength];
    return CRYPTO_RC4_SUCCESS;
}

void accelc_RC4_reset(RC4_STATE* srcState, const RC4_KEY* srcKey) {
    for (int i = 0; i < 256; ++i)
        srcState->SBox[i] = (uint8_t)i;
    for (int i = 0, j = 0; i < 256; i++) {
        j = (j + srcState->SBox[i] + srcKey->SBox[i]) % 256;
        uint8_t temp = srcState->SBox[i];
        srcState->SBox[i] = srcState->SBox[j];
        srcState->SBox[j] = temp;
    }
}