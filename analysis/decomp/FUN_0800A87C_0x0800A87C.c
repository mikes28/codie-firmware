/* ==== FUN_0800A87C @ 0x800a87c ==== */

void FUN_0800A87C(uint *param_1,int param_2)

{
  if (param_2 == 0) {
    *param_1 = *param_1 & 0xfffe;
  }
  else {
    *param_1 = *param_1 | 1;
  }
  return;
}


