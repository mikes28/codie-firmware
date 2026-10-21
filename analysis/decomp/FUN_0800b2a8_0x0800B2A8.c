/* ==== FUN_0800b2a8 @ 0x800b2a8 ==== */

void FUN_0800b2a8(int param_1,short param_2)

{
  *(ushort *)(param_1 + 0x1c) = *(ushort *)(param_1 + 0x1c) & 0xf3ff;
  *(ushort *)(param_1 + 0x1c) = param_2 << 8 | *(ushort *)(param_1 + 0x1c);
  return;
}


