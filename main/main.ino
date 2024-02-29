#include <Arduino.h>
#include <string.h>
#include <stdio.h>

#include "radio.h"
#include "keeloq.h"
#include "hcs301.h"

#define LED_PIN LED_BUILTIN

char Buffer[64];
uint8_t i=0;

uint64_v key= {0x03C81E8F,0x2B6E076C};

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
    blink();
    
    hcs_fix=*(struct hcsFixed*)&radio.dataF;

    ciphertext=(uint32_t)radio.dataE;
    keeloq_decrypt(&key,&plaintext,&ciphertext,KEELOQ_NROUNDS);
    hcs_enc=*(struct hcsEncrypted*)&plaintext;

    sprintf(Buffer,"%03d: fix=%08lX : btn=%lX ser=%lX ",++i,radio.dataF,hcs_fix.btn,hcs_fix.ser);
    Serial.print(Buffer);
    sprintf(Buffer,", enc=[%08lX] --> dec=%08lX : ",radio.dataE,plaintext);
    Serial.print(Buffer);
    sprintf(Buffer,"btn=%lX ovr=%lX disc=%lX C=%lX \r\n",hcs_enc.btn,hcs_enc.ovr,hcs_enc.disc,hcs_enc.counter);
    Serial.print(Buffer);

    radio_rx_reset(&radio);
  }
}
