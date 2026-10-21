/* ==== FUN_08009268 @ 0x8009268 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08009268(void)

{
  undefined4 in_r3;
  bool bVar1;
  
  bVar1 = _DAT_2000085c == 0;
  if (bVar1) {
    FUN_0800b3c0(0x40014800,2,&DAT_20000710,0xa6,in_r3);
    FUN_0800b3c0(0x40014800,1);
    FUN_0800a89c(0x40020008,0x13e);
    FUN_0800b1ec(0x40014800,0);
    FUN_0800A87C(0x40020008,1);
    _DAT_40014824 = 0;
    FUN_0800AFA0(0x40014800,1);
    FUN_0800b3a4(0x40014800,1);
    _DAT_2000085c = 1;
  }
  return bVar1;
}


