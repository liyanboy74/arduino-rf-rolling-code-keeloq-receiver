#include <Arduino.h>
#include <string.h>
#include <stdio.h>

#include "radio.h"
#include "keeloq.h"
#include "hcs301.h"

#define LED_PIN LED_BUILTIN

char Buffer[64];

uint64_v key= {0x5cec6701,0xb79fd949};

struct hcsFixed hcs_fix;
struct hcsEncrypted hcs_enc;

uint32_t ciphertext,plaintext;

void blink() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  radio_init(&radio);
  Serial.println("Ready");
}

void loop() {
  if (radio_rx_data_is_ready(&radio)) {

    
    hcs_fix=*(struct hcsFixed*)&radio.dataF;

    ciphertext=radio.dataE;
    keeloq_decrypt(&key,&plaintext,&ciphertext,KEELOQ_NROUNDS);
    hcs_enc=*(struct hcsEncrypted*)&plaintext;

    sprintf(Buffer, "%x \r\n",hcs_fix.ser);
    
    Serial.print(Buffer);

    blink();
    radio_rx_reset(&radio);
  }
}
