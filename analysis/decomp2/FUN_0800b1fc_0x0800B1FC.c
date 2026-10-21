/* ==== FUN_0800b1fc @ 0x800b1fc ==== */

void FUN_0800b1fc(int param_1,ushort param_2)

{
  *(ushort *)(param_1 + 0x18) = param_2 | *(ushort *)(param_1 + 0x18) & 0xfff7;
  return;
}


