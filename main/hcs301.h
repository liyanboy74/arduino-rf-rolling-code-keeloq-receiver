
struct hcsEncrypted{
  uint32_t btn:4;
  uint32_t ovr:2;
  uint32_t disc:10;
  uint32_t counter:16;
};

struct hcsFixed{
  uint32_t btn:4;
  uint32_t ser:28;
};

