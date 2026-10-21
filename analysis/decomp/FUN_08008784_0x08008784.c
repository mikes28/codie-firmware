/* ==== FUN_08008784 @ 0x8008784 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008784(void)

{
  int iVar1;
  undefined4 in_r3;
  
  if (_DAT_200006f4 != _DAT_20000fc0) {
    iVar1 = FUN_0800aabc(0x48000000,0x1000,0x80,0x90,in_r3);
    _DAT_200006f4 = (uint)(iVar1 == 0);
    if (_DAT_200006f4 != _DAT_20000fc0) {
      if (_DAT_200006f4 == 0) {
        (*_DAT_200006f8)(1);
      }
      else {
        (*_DAT_200006f8)(2);
      }
      _DAT_20000fc0 = _DAT_200006f4;
    }
  }
  return;
}


