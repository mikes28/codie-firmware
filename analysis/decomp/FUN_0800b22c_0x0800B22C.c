/* ==== FUN_0800b22c @ 0x800b22c ==== */

void FUN_0800b22c(int param_1,short param_2)

{
  *(ushort *)(param_1 + 0x1c) = *(ushort *)(param_1 + 0x1c) & 0xf7ff | param_2 << 8;
  return;
}


