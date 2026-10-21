/* ==== FUN_0800AF74 @ 0x800af74 ==== */

void FUN_0800AF74(ushort *param_1,int param_2)

{
  if (param_2 == 0) {
    *param_1 = *param_1 & 0xff7f;
  }
  else {
    *param_1 = *param_1 | 0x80;
  }
  return;
}


