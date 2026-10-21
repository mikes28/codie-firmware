/* ==== FUN_0800a77c @ 0x800a77c ==== */

void FUN_0800a77c(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xfffffffd;
  *(uint *)(param_1 + 0xc) = param_2 | *(uint *)(param_1 + 0xc);
  return;
}


