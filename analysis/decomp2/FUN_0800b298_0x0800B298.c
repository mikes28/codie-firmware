/* ==== FUN_0800b298 @ 0x800b298 ==== */

void FUN_0800b298(int param_1,ushort param_2)

{
  *(ushort *)(param_1 + 0x1c) = *(ushort *)(param_1 + 0x1c) & 0xfff3;
  *(ushort *)(param_1 + 0x1c) = param_2 | *(ushort *)(param_1 + 0x1c);
  return;
}


