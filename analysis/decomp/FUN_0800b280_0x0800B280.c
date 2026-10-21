/* ==== FUN_0800b280 @ 0x800b280 ==== */

void FUN_0800b280(int param_1,short param_2)

{
  *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & 0xf3ff;
  *(ushort *)(param_1 + 0x18) = param_2 << 8 | *(ushort *)(param_1 + 0x18);
  return;
}


