#include <Arduino.h>

#include "radio.h"
#include "keeloq.h"

#define LED_PIN LED_BUILTIN

typedef struct{
  uint32_t counter:16;
  uint32_t disc:10;
  uint32_t ovr:2;
  uint32_t btn:4;
}encrypted_p_s;

typedef struct{
  uint32_t ser:28;
  uint32_t btn:4;
}fixed_p_s;


uint64_v key= {0x5cec6701,0xb79fd949};


void blink() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\r\nStarting..");
  pinMode(LED_PIN, OUTPUT);
  radio_init(&radio);
  Serial.println("Ready");
}

void loop() {
  if (radio_rx_data_is_ready(&radio)) {
    blink();
    Serial.print(radio.dataF);
    Serial.print(radio.dataE);
    Serial.println(radio.dataVR);

    // Decrypt ciphertext to plaintext
       // keeloq_decrypt(&key,&plaintext,&ciphertext,KEELOQ_NROUNDS);

    radio_rx_reset(&radio);
  }
}
