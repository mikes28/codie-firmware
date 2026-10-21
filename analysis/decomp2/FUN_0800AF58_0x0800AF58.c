/* ==== FUN_0800AF58 @ 0x800af58 ==== */

void FUN_0800AF58(ushort *param_1,int param_2)

{
  if (param_2 == 0) {
    *param_1 = *param_1 & 0xfffb;
  }
  else {
    *param_1 = *param_1 | 4;
  }
  return;
}


