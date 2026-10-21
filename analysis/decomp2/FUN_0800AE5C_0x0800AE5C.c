/* ==== FUN_0800AE5C @ 0x800ae5c ==== */

void FUN_0800AE5C(int param_1,ushort param_2,ushort param_3,short param_4)

{
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xfffe;
  *(ushort *)(param_1 + 0x18) = param_3 | *(ushort *)(param_1 + 0x18) & 0xff0c | param_4 << 4;
  *(ushort *)(param_1 + 0x20) = param_2 | *(ushort *)(param_1 + 0x20) & 0xfff5 | 1;
  return;
}


