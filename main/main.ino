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
uint8_t i = 0;

// KEY for keeloq algoritm
// must be same as transmiter key, 64bit LSB-first
uint8_t key[] = { 0x56, 0x4a, 0xbc, 0x07, 0x57, 0x1e, 0x62, 0x94 };
// uint8_t key[] = { 0xf8, 0x75, 0xdc, 0x74, 0xa6, 0xbf, 0x6e, 0x85 };

// some useful structher for bitfildes
struct hcsFixed hcs_fix;
struct hcsEncrypted hcs_enc;

// buffers for keeloq
uint32_t temp;

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
    // LED indikator
    blink();

    // the received data (fix,encripted,vr) stored in [radio.dataF,radio.dataE,radio.dataVR] and ready for reading by the software.
    // using hcs301.h we can format and read bit by bitfilds [hcs_fix,hcs_enc].
    memcpy(&hcs_fix, &radio.dataF, sizeof(radio.dataF));

    // Print data to serial com port
    sprintf(Buffer, "%03d: fix=%08lX : vr=%X btn=%lX ser=%lX ", ++i, radio.dataF, radio.dataVR, hcs_fix.btn, hcs_fix.ser);
    Serial.print(Buffer);

    // Check if all key pressed for SEED
    if (hcs_fix.btn == 0xf) {
      //Print SEED
      sprintf(Buffer, ", SEED=[%08lX] \r\n", radio.dataE);
      Serial.print(Buffer);

    } else {
      // copy encripted Data to keeloq buffer for decripting
      memcpy(&temp, &radio.dataE, sizeof(radio.dataE));

      // decrypting encripted data by keeloq algoritm and key.
      keeloq_decrypt(key, &temp, KEELOQ_NROUNDS);

      // copy decripted data from keeloq buffer
      // using hcs301.h we can format and read bit by bitfilds [hcs_fix,hcs_enc].
      memcpy(&hcs_enc, &temp, sizeof(temp));

      // Check disc
      if (hcs_enc.disc == (hcs_fix.ser & 0x3ff)) {
        // Everything is OK
        sprintf(Buffer, ", enc= [%08lX] --> dec=%08lX : ", radio.dataE, temp);
        Serial.print(Buffer);
        sprintf(Buffer, "btn=%lX ovr=%lX disc=%lX C=%lX \r\n", hcs_enc.btn, hcs_enc.ovr, hcs_enc.disc, hcs_enc.counter);
        Serial.print(Buffer);
      } else {
        Serial.print(", WRONG KEY!\r\n");
      }
    }

    // continue receiving radio signal
    radio_rx_reset(&radio);
  }
}
