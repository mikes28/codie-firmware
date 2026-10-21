/* ==== FUN_0800b3e0 @ 0x800b3e0 ==== */

void FUN_0800b3e0(int param_1,ushort param_2)

{
  *(ushort *)(param_1 + 8) = param_2 | *(ushort *)(param_1 + 8) & 0xff8f;
  return;
}


