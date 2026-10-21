/* ==== FUN_0800B38C @ 0x800b38c ==== */

void FUN_0800B38C(int param_1,ushort param_2,int param_3)

{
  if (param_3 == 0) {
    *(ushort *)(param_1 + 0xc) = *(ushort *)(param_1 + 0xc) & ~param_2;
  }
  else {
    *(ushort *)(param_1 + 0xc) = param_2 | *(ushort *)(param_1 + 0xc);
  }
  return;
}


