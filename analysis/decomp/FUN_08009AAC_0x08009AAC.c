/* ==== FUN_08009AAC @ 0x8009aac ==== */

void FUN_08009AAC(void)

{
  if (-1 < (int)((uint)DAT_200008f8 << 0x1d)) {
    FUN_0800B4EC(0x40004c00,0x50105,1);
    FUN_0800B4C0(0x40004c00,1);
    DAT_200008f8 = DAT_200008f8 | 4;
  }
  return;
}


