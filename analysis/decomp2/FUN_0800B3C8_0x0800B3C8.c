/* ==== FUN_0800B3C8 @ 0x800b3c8 ==== */

void FUN_0800B3C8(int param_1,ushort param_2,int param_3)

{
  if (param_3 == 0) {
    *(ushort *)(param_1 + 0xc) = *(ushort *)(param_1 + 0xc) & ~param_2;
  }
  else {
    *(ushort *)(param_1 + 0xc) = param_2 | *(ushort *)(param_1 + 0xc);
  }
  return;
}


