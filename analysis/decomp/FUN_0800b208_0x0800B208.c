/* ==== FUN_0800b208 @ 0x800b208 ==== */

void FUN_0800b208(int param_1,short param_2)

{
  *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & 0xf7ff | param_2 << 8;
  return;
}


