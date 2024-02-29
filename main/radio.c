#include "radio.h"

struct radioRXC radio;
struct pulseFilter f1 = { .pulselength = 400, .err = 100 };

void radio_init(struct radioRXC *obj) {

  pinMode(RADIO_SIGNAL_PIN, INPUT_PULLUP);
  attachInterrupt(0, RADIO_SIGNAL_PIN_handler, CHANGE);

  obj->state = RADIO_LISTEN;
  obj->filter = &f1;
}

bool is_squad_pulse(struct radioRXC *obj, struct oneBit *d) {
  if (abs((int)d->on - (int)d->off) < obj->filter->err) return true;
  return false;
}

bool radio_is_valid_pulse(struct radioRXC *obj, struct oneBit *d) {
  uint16_t m = min(d->on, d->off);
  if (m < ((obj->filter->pulselength) - (obj->filter->err))) return false;
  if (m > ((obj->filter->pulselength) + (obj->filter->err))) return false;
  return true;
}

void radio_check_synce_pulse(struct radioRXC *obj, struct oneBit *d) {
  if (is_squad_pulse(obj, d)) obj->sp++;
  else if (d->off > (d->on * 8) && d->off < (d->on * 12) && obj->sp > 10) {
    obj->index = 0;
    obj->pulselength = 0;
    obj->dataE = 0;
    obj->dataF = 0;
    obj->dataVR = 0;
    obj->state = RADIO_SYNC_OK;
    obj->sp = 0;
  } else {
    obj->sp = 0;
    return;
  }
}

void radio_rx_get_bit(struct radioRXC *obj, uint8_t bit_value) {
  if (obj->index < 66) {
    if (obj->index < 32) {
      obj->dataE = obj->dataE << 1;
      if (bit_value) obj->dataE = obj->dataE | 1;
    } else if (obj->index < 64) {
      obj->dataF = obj->dataF << 1;
      if (bit_value) obj->dataF = obj->dataF | 1;
    } else if (obj->index < 66) {
      obj->dataVR = obj->dataVR << 1;
      if (bit_value) obj->dataVR = obj->dataVR | 1;
    }
    obj->index++;
    if (obj->index == 66) {
      obj->state = RADIO_DATA_OK;
    }
  }
}

void radio_rx_get_err(struct radioRXC *obj) {
  obj->state = RADIO_LISTEN;
  obj->sp = 0;
}

void radio_ckeck_bit_pulse(struct radioRXC *obj, struct oneBit *d) {
  if (d->on < d->off) {
    if ((d->on * 3) > d->off) {
      radio_rx_get_bit(obj, 1);
      if (!obj->pulselength) obj->pulselength = d->on;
    } else radio_rx_get_err(obj);
  } else {
    if ((d->off * 3) > d->on) radio_rx_get_bit(obj, 0);
    else radio_rx_get_err(obj);
  }
}

void radio_check_sig_pulse(struct radioRXC *obj, struct oneBit *d) {
  if (radio_is_valid_pulse(obj, d)) {
    switch (obj->state) {
      case RADIO_OFF: break;
      case RADIO_LISTEN: radio_check_synce_pulse(obj, d); break;
      case RADIO_SYNC_OK: radio_ckeck_bit_pulse(obj, d); break;
      case RADIO_DATA_OK: break;
      default: break;
    }
  }
}

void radio_ckeck_end_bit_pulse(struct radioRXC *obj, struct oneBit *d) {
  if (obj->index == 65) {
    if (d->on < (int)((obj->pulselength * (float)1.5))) radio_rx_get_bit(obj, 1);
    else radio_rx_get_bit(obj, 0);
  }
}

void radio_rx_reset(struct radioRXC *obj) {
  obj->state = RADIO_LISTEN;
}

void radio_rx_stop(struct radioRXC *obj) {
  obj->state = RADIO_OFF;
}

bool radio_rx_data_is_ready(struct radioRXC *obj) {
  if (obj->state == RADIO_DATA_OK) return true;
  else return false;
}

void RADIO_SIGNAL_PIN_handler() {
  static uint32_t tempt = 0, oldt = 0, dift = 0;
  static struct oneBit tempb;

  tempt = micros();
  dift = tempt - oldt;

  // Remove fast noise
  if (dift > 100) {
    if (digitalRead(RADIO_SIGNAL_PIN)) {
      tempb.off = dift;
      radio_check_sig_pulse(&radio, &tempb);
    } else {
      tempb.on = dift;
      radio_ckeck_end_bit_pulse(&radio, &tempb);
    }
  }

  oldt = tempt;
}
