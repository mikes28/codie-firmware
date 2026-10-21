/* ==== FUN_0800b270 @ 0x800b270 ==== */

void FUN_0800b270(int param_1,ushort param_2)

{
  *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & 0xfff3;
  *(ushort *)(param_1 + 0x18) = param_2 | *(ushort *)(param_1 + 0x18);
  return;
}


