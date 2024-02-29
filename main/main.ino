/**
 *  Arduino RF rolling code keeloq receiver 
 *  By: Liyanboy74
 *
 *  https://github.com/liyanboy74/arduino-rf-rolling-code-keeloq-receiver
 *
 *  Programmer:
 *  Software: https://github.com/ioelectro/hcs-programmer-soft
 *  Hardwer: https://github.com/ioelectro/arduino-hcs-programmer
 *  Doc: https://ioelectro.ir/blog/hcs-programmer/
 *
 *  Keeloq Encoder/Decoder:
 *  Algoritm: https://github.com/liyanboy74/KeeLoq
 *  Doc: https://ioelectro.ir/blog/keeloq/ 
 *
 *  just Connect RF 433 or 315 Mhz resiver to pin 2 (INT0) arduino UNO v3.
 *  for correct encoding the encrypted part of received packet we must know the key of keeloq algorithm.
 */

#include <Arduino.h>
#include <string.h>
#include <stdio.h>

#include "radio.h"
#include "keeloq.h"
#include "hcs301.h"

#define LED_PIN LED_BUILTIN

// some buffer for string formating in sprintf
char Buffer[64];
uint8_t i=0;

// key for keeloq algoritm
// must be same as transmiter key
uint64_v key= {0x03C81E8F,0x2B6E076C};

// some useful structher for bitfildes
struct hcsFixed hcs_fix;
struct hcsEncrypted hcs_enc;

// buffers for keeloq
uint32_t ciphertext,plaintext;

// jast toggel led
void blink() {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

// Setup - run once!
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  radio_init(&radio);
  Serial.println("Ready");
}

// main app
void loop() {
  // check if we have new received packet
  if (radio_rx_data_is_ready(&radio)) {

    // led indikator
    blink();

    // the received data (fix,encripted,vr) stored in [radio.dataF,radio.dataE,radio.dataVR] and ready for reading by the software.
    // using hcs301.h we can format and read bit by bit filds [hcs_fix,hcs_enc].
    hcs_fix=*(struct hcsFixed*)&radio.dataF;

    // decrypting encripted data by keeloq algoritm and key. 
    ciphertext=(uint32_t)radio.dataE;
    keeloq_decrypt(&key,&plaintext,&ciphertext,KEELOQ_NROUNDS);
    hcs_enc=*(struct hcsEncrypted*)&plaintext;

    // Print data to serial com port
    sprintf(Buffer,"%03d: fix=%08lX : vr=%X btn=%lX ser=%lX ",++i,radio.dataF,radio.dataVR,hcs_fix.btn,hcs_fix.ser);
    Serial.print(Buffer);
    sprintf(Buffer,", enc=[%08lX] --> dec=%08lX : ",radio.dataE,plaintext);
    Serial.print(Buffer);
    sprintf(Buffer,"btn=%lX ovr=%lX disc=%lX C=%lX \r\n",hcs_enc.btn,hcs_enc.ovr,hcs_enc.disc,hcs_enc.counter);
    Serial.print(Buffer);

    // continue receiving radio signal
    radio_rx_reset(&radio);
  }
}
