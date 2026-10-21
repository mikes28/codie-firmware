/* ==== FUN_080030D0 @ 0x80030d0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080030D0(void)

{
  int iVar1;
  undefined4 uVar2;
  
  FUN_0800A558(0x200002a8,&DAT_20000f38);
  if ((_DAT_20000f3c != 0) || (500 < _DAT_20000f38)) {
    FUN_0800705c();
    iVar1 = FUN_08006E24();
    if (iVar1 << 0x1b < 0) {
      iVar1 = FUN_08006124();
      if (iVar1 != 0x103) {
        FUN_08005FBC(0x103);
      }
      iVar1 = FUN_08006124();
      if (iVar1 == 0x103) {
        iVar1 = FUN_0800705c();
        uVar2 = FUN_080001c4(*(short *)(iVar1 + 4) * 100,0x76c);
        FUN_08005EC4(0x103,1,uVar2);
        uVar2 = FUN_080030AC();
        FUN_08005EC4(0x103,0,uVar2);
      }
    }
    else {
      iVar1 = FUN_08006124();
      if (iVar1 == 0x103) {
        FUN_080060DC(0x103);
      }
    }
    FUN_08006E5C();
    FUN_0800A4DC(0x200002a8);
  }
  FUN_08006F0C();
  return;
}


