/**
 * By Liyanboy74
 * at www.ioelectro.ir
 * https://github.com/liyanboy74/arduino-rf-rolling-code-keeloq-receiver
 */

struct hcsEncrypted {
  uint32_t counter : 16;
  uint32_t disc : 10;
  uint32_t ovr : 2;
  uint32_t btn : 4;
};

struct hcsFixed {
  uint32_t ser : 28;
  uint32_t btn : 4;
};
