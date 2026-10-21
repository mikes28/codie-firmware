/* ==== FUN_080086C4 @ 0x80086c4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080086C4(void)

{
  int iVar1;
  
  iVar1 = FUN_0800a948(0x1000);
  if (iVar1 != 0) {
    iVar1 = FUN_0800aabc(0x48000000,0x1000);
    _DAT_200006f4 = (uint)(iVar1 == 0);
    FUN_0800a958(0x1000);
  }
  return;
}


