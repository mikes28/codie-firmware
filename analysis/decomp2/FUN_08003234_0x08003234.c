/* ==== FUN_08003234 @ 0x8003234 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08003234(void)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  uVar1 = _DAT_200002d0;
  uVar2 = _DAT_200002d8;
  if ((DAT_20000354 & 6) == 2) {
    FUN_08008DBC();
    FUN_08008910();
    _DAT_200002d0 = FUN_08008E00(1);
    uVar2 = FUN_08008E00(2);
    DAT_20000354 = DAT_20000354 | 4;
    uVar1 = _DAT_200002d0;
  }
  _DAT_200002dc = (undefined4)((ulonglong)uVar2 >> 0x20);
  _DAT_200002d8 = (undefined4)uVar2;
  _DAT_200002d4 = (undefined4)((ulonglong)uVar1 >> 0x20);
  _DAT_200002d0 = (undefined4)uVar1;
  return;
}


