#include "keeloq.h"

uint16_t nlf(uint16_t d)
{
    return (((uint32_t)(NLF_LOOKUP_CONSTANT) >> d) & 0x1);
}

void keeloq_encrypt(uint64_v *key, uint32_t *plaintext, uint32_t *ciphertext, uint16_t nrounds)
{
    uint16_t out, xor, nlf_input,i,k,ki;
    *ciphertext = *plaintext;
    for (i = 0; i < nrounds; i++)
    {
        nlf_input = (((*ciphertext >> 31) & 0x1) << 4) | (((*ciphertext >> 26) & 0x1) << 3) |
                    (((*ciphertext >> 20) & 0x1) << 2) | (((*ciphertext >> 9) & 0x1) << 1) | ((*ciphertext >> 1) & 0x1);

        out = nlf(nlf_input);

        ki=i % 64;
        #ifdef USE_VIRTUAL_UINT64_VARIABLE
        if(ki>31)k=(key->H>>(ki-32))&0x1;
        else k=(key->L>>ki)&0x1;
        #else
        k=((uint64_t)*key >> ki) & 0x1;
        #endif

        xor = k ^ ((*ciphertext >> 16) & 0x1) ^ (*ciphertext & 0x1) ^ out;
        *ciphertext = (*ciphertext >> 1) | ((uint32_t)(xor) << 31);
    }
}

void keeloq_decrypt(uint64_v *key, uint32_t *plaintext, uint32_t *ciphertext, uint16_t nrounds)
{
    uint16_t out, xor, nlf_input,i,k,ki;
	*plaintext = *ciphertext;
    for (i = 0; i < nrounds; i++)
    {
        nlf_input = (((*plaintext >> 30) & 0x1) << 4) | (((*plaintext >> 25) & 0x1) << 3) |
                    (((*plaintext >> 19) & 0x1) << 2) | (((*plaintext >> 8) & 0x1) << 1) | (*plaintext & 0x1);

        out = nlf(nlf_input);

        ki=(uint16_t)(15 - i) % 64;
        #ifdef USE_VIRTUAL_UINT64_VARIABLE
        if(ki>31)k=(key->H>>(ki-32))&0x1;
        else k=(key->L>>ki)&0x1;
        #else
        k=((*key >> ki) & 0x1);
        #endif

        xor = k ^ ((*plaintext >> 31) & 0x1) ^ ((*plaintext >> 15) & 0x1) ^ out;
        *plaintext = (*plaintext << 1) | xor;
    }
}
