#include "keeloq.h"

uint8_t nlf(uint8_t d) {
  return (((uint32_t)(NLF_LOOKUP_CONSTANT) >> d) & 0x1);
}

void keeloq_encrypt(uint8_t *key, uint32_t *data, const uint16_t nrounds) {
  uint32_t x;
  uint16_t loop;
  uint8_t o, nlf_input, k, ki;

  for (loop = 0; loop < nrounds; loop++) {
    nlf_input = (((*data >> 31) & 0x1) << 4) | (((*data >> 26) & 0x1) << 3) | (((*data >> 20) & 0x1) << 2) | (((*data >> 9) & 0x1) << 1) | ((*data >> 1) & 0x1);

    o = nlf(nlf_input);
    ki = loop % 64;
    k = key[ki / 8] >> (ki % 8);
    x = k ^ (*data >> 16) ^ *data ^ o;
    *data = (*data >> 1) | (x << 31);
  }
}

void keeloq_decrypt(uint8_t *key, uint32_t *data, const uint16_t nrounds) {
  uint32_t x;
  uint16_t loop;
  uint8_t o, nlf_input, k, ki;

  for (loop = 0; loop < nrounds; loop++) {
    nlf_input = (((*data >> 30) & 0x1) << 4) | (((*data >> 25) & 0x1) << 3) | (((*data >> 19) & 0x1) << 2) | (((*data >> 8) & 0x1) << 1) | (*data & 0x1);

    o = nlf(nlf_input);
    ki = (uint16_t)(15 - loop) % 64;
    k = key[ki / 8] >> (ki % 8);
    x = k ^ (*data >> 31) ^ (*data >> 15) ^ o;
    *data = (*data << 1) | x & 1;
  }
}
