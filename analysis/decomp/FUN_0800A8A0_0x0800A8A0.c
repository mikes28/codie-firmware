/* ==== FUN_0800A8A0 @ 0x800a8a0 ==== */

void FUN_0800A8A0(uint *param_1,uint param_2,int param_3)

{
  if (param_3 == 0) {
    *param_1 = *param_1 & ~param_2;
  }
  else {
    *param_1 = param_2 | *param_1;
  }
  return;
}


