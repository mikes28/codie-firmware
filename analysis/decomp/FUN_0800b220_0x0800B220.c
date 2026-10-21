/* ==== FUN_0800b220 @ 0x800b220 ==== */

void FUN_0800b220(int param_1,ushort param_2)

{
  *(ushort *)(param_1 + 0x1c) = param_2 | *(ushort *)(param_1 + 0x1c) & 0xfff7;
  return;
}


