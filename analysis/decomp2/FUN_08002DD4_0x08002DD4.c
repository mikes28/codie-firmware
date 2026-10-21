/* ==== FUN_08002DD4 @ 0x8002dd4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08002DD4(int param_1)

{
  uint uVar1;
  
  uVar1 = FUN_08000128(_DAT_20000000,1000000);
  _DAT_e000e018 = (uVar1 & 0xffffff) - 1;
  _DAT_e000e014 = _DAT_e000e018;
  if (param_1 == 0) {
    _DAT_e000e010 = 4;
    return;
  }
  do {
  } while( true );
}


