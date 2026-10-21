/* ==== FUN_0800AFA0 @ 0x800afa0 ==== */

void FUN_0800AFA0(ushort *param_1,int param_2)

{
  if (param_2 == 0) {
    *param_1 = *param_1 & 0xfffe;
  }
  else {
    *param_1 = *param_1 | 1;
  }
  return;
}


