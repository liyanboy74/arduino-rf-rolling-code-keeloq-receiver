/*
  https://github.com/liyanboy74/KeeLoq
*/
#ifndef __keeloq_h__
#define __keeloq_h__

#include <stdint.h>

// Uncomment if compiler doesn't have 64 bit variable support!
#define USE_VIRTUAL_UINT64_VARIABLE


#ifdef USE_VIRTUAL_UINT64_VARIABLE
typedef struct{
    uint32_t H;
    uint32_t L;
}uint64_v;
#else
typedef uint64_t uint64_v ;
#endif

#define KEELOQ_NROUNDS  528
#define NLF_LOOKUP_CONSTANT 0x3a5c742e

#ifdef __cplusplus
extern "C" {
#endif

uint16_t nlf(uint16_t d);
void keeloq_encrypt(uint64_v *key, uint32_t *plaintext, uint32_t *ciphertext, uint16_t nrounds);
void keeloq_decrypt(uint64_v *key, uint32_t *plaintext, uint32_t *ciphertext, uint16_t nrounds);

#ifdef __cplusplus
}
#endif

#endif
