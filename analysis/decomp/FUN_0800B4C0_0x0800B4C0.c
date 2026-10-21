/* ==== FUN_0800B4C0 @ 0x800b4c0 ==== */

void FUN_0800B4C0(uint *param_1,int param_2)

{
  if (param_2 == 0) {
    *param_1 = *param_1 & 0xfffffffe;
  }
  else {
    *param_1 = *param_1 | 1;
  }
  return;
}


