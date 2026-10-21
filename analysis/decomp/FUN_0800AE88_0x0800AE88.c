/* ==== FUN_0800AE88 @ 0x800ae88 ==== */

void FUN_0800AE88(int param_1,short param_2,short param_3,short param_4)

{
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xffef;
  *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & 0xcff | param_3 << 8 | param_4 << 0xc;
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xff5f | 0x10 | param_2 << 4;
  return;
}


