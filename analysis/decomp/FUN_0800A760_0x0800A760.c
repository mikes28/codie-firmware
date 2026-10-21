/* ==== FUN_0800A760 @ 0x800a760 ==== */

void FUN_0800A760(int param_1,int param_2)

{
  if (param_2 == 0) {
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xfffffffe;
  }
  else {
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) | 1;
  }
  return;
}


