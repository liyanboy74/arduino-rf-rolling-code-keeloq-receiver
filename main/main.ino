#include <Arduino.h>

#define RADIO_SIGNAL_PIN 2  //Intrrupt 0
#define LED_PIN LED_BUILTIN

enum radio_s_e {
  RADIO_OFF,
  RADIO_LISTEN,
  RADIO_SYNC_OK,
  RADIO_DATA_OK
};

struct oneBit {
  uint16_t on;
  uint16_t off;
};

struct radioRXC {
  radio_s_e state;
  uint8_t sp;
  uint8_t index;
  uint16_t pulselength;
  uint32_t dataE, dataF;
  uint8_t dataVR;
};

struct radioRXC radio;

uint32_t tempt = 0, oldt = 0, dift = 0;
volatile oneBit tempb;

void radio_init(void) {

  pinMode(RADIO_SIGNAL_PIN, INPUT_PULLUP);
  attachInterrupt(0, RADIO_SIGNAL_PIN_handler, CHANGE);

  radio.state = RADIO_LISTEN;
  radio.sp = 0;
}

void radio_check_synce_pulse(void) {
  if (abs((int)tempb.on - (int)tempb.off) < 100)  // is squad pulse ?
    radio.sp++;
  else if (tempb.off > (tempb.on * 8) && tempb.off < (tempb.on * 12) && radio.sp > 10) {
    radio.index = 0;
    radio.pulselength = 0;
    radio.dataE = 0;
    radio.dataF = 0;
    radio.dataVR = 0;
    //radio.state = RADIO_SYNC_OK;
    radio.sp = 0;

    //Debug
    blink();
  } else
    radio.sp = 0;
}

void radio_rx_get_bit(uint8_t bit_value) {
  if (radio.index < 66) {
    if (radio.index < 32) {
      radio.dataE = radio.dataE << 1;
      if (bit_value) radio.dataE = radio.dataE | 1;
    } else if (radio.index < 64) {
      radio.dataF = radio.dataF << 1;
      if (bit_value) radio.dataF = radio.dataF | 1;
    } else if (radio.index < 66) {
      radio.dataVR = radio.dataVR << 1;
      if (bit_value) radio.dataVR = radio.dataVR | 1;
    }
    radio.index++;
    if (radio.index == 66) {
      radio.state = RADIO_DATA_OK;
    }
  }
}

void radio_rx_get_err(void) {
  radio.state = RADIO_LISTEN;
  radio.sp = 0;
}

void radio_ckeck_bit_pulse(void) {
  if (tempb.on < tempb.off) {
    if ((tempb.on * 3) > tempb.off) {
      radio_rx_get_bit(1);
      if (!radio.pulselength) radio.pulselength = tempb.on;
    } else radio_rx_get_err();
  } else {
    if ((tempb.off * 3) > tempb.on) radio_rx_get_bit(0);
    else radio_rx_get_err();
  }
}

void radio_check_sig_pulse(void) {
  uint16_t m = min(tempb.on, tempb.off);
  if (m > 300 && m < 500) {
    switch (radio.state) {
      case RADIO_OFF: break;
      case RADIO_LISTEN: radio_check_synce_pulse(); break;
      case RADIO_SYNC_OK: radio_ckeck_bit_pulse(); break;
      case RADIO_DATA_OK: break;
      default: break;
    }
  }
}

void radio_rx_reset(void) {
  radio.state = RADIO_LISTEN;
}

void radio_rx_stop(void) {
  radio.state = RADIO_OFF;
}

bool radio_rx_data_is_ready(void) {
  if (radio.state == RADIO_DATA_OK) return true;
  else return false;
}

void RADIO_SIGNAL_PIN_handler() {


  tempt = micros();
  dift = tempt - oldt;
  if (digitalRead(RADIO_SIGNAL_PIN)) {
    tempb.off = dift;
    radio_check_sig_pulse();
  } else {
    tempb.on = dift;
  }
  oldt = tempt;
}

void blink() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\r\nStarting..");
  pinMode(LED_PIN, OUTPUT);
  radio_init();
  Serial.println("Ready");
}

void loop() {
  if (radio_rx_data_is_ready()) {
    blink();
    radio_rx_reset();
  }
}
