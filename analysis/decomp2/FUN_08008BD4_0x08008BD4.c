/* ==== FUN_08008BD4 @ 0x8008bd4 ==== */

void FUN_08008BD4(byte param_1,int param_2)

{
  if (100 < param_1) {
    param_1 = 100;
  }
  if (param_2 << 0x1f < 0) {
    DAT_200006ff = param_1;
  }
  if (param_2 << 0x1e < 0) {
    DAT_20000700 = param_1;
  }
  return;
}


