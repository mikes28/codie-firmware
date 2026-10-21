/* ==== FUN_08007734 @ 0x8007734 ==== */

void FUN_08007734(void)

{
  int iVar1;
  
  iVar1 = FUN_0800a8b8(0x20000);
  if (iVar1 == 1) {
    if (DAT_2000051a == '\a') {
      FUN_0800A87C(0x40020058,0);
      FUN_0800B3C8(0x40014000,0x100,0);
      FUN_0800b3c0(0x40014000,2);
      FUN_0800B38C(0x40014000,2,1);
    }
    FUN_0800a8c8(0x20000);
  }
  iVar1 = FUN_0800a8b8(0x40000);
  if (iVar1 == 1) {
    FUN_0800a8c8(0x40000);
  }
  return;
}


