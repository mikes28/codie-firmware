/* ==== FUN_0800B524 @ 0x800b524 ==== */

void FUN_0800B524(int param_1,ushort param_2,int param_3)

{
  if (param_3 == 0) {
    *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & ~param_2;
  }
  else {
    *(ushort *)(param_1 + 0x18) = param_2 | *(ushort *)(param_1 + 0x18);
  }
  return;
}


