/* ==== FUN_08008E9C @ 0x8008e9c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008E9C(void)

{
  int iVar1;
  
  iVar1 = FUN_0800B3A8(0x40014800,1);
  if (iVar1 != 0) {
    FUN_0800AFA0(0x40014800,0);
    FUN_0800A87C(0x40020008,0);
    FUN_0800B38C(0x40014800,1,0);
    _DAT_2000085c = 0;
    FUN_0800b3c0(0x40014800,1);
  }
  return;
}


