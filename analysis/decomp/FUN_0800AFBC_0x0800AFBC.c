/* ==== FUN_0800AFBC @ 0x800afbc ==== */

void FUN_0800AFBC(int param_1,int param_2)

{
  if (param_2 == 0) {
    *(ushort *)(param_1 + 0x44) = *(ushort *)(param_1 + 0x44) & 0x7fff;
  }
  else {
    *(ushort *)(param_1 + 0x44) = *(ushort *)(param_1 + 0x44) | 0x8000;
  }
  return;
}


