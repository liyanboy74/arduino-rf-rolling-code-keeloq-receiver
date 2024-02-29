#ifndef __my_radio_h__
#define __my_radio_h__

#include <Arduino.h>
#include <stdint.h>

#define RADIO_SIGNAL_PIN 2  //Intrrupt 0

enum radio_s_e {
  RADIO_OFF=0,
  RADIO_LISTEN,
  RADIO_SYNC_OK,
  RADIO_DATA_OK
};

struct oneBit {
  uint16_t on;
  uint16_t off;
};

struct pulseFilter {
  uint16_t pulselength;
  uint16_t err;
};

struct radioRXC {
  uint8_t index;
  uint8_t sp;
  enum radio_s_e state;
  uint16_t pulselength;
  const struct pulseFilter *filter;
  uint32_t dataE, dataF;
  uint8_t dataVR;
};

#ifdef __cplusplus
extern "C" {
#endif

void radio_init(struct radioRXC *obj);
void radio_rx_reset(struct radioRXC *obj);
bool radio_rx_data_is_ready(struct radioRXC *obj);

uint32_t flip32(uint32_t Data);

void RADIO_SIGNAL_PIN_handler();

extern struct radioRXC radio;

#ifdef __cplusplus
}
#endif

#endif //__radio_h__
