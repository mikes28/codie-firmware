/* ==== FUN_08009D30 @ 0x8009d30 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009D30(void)

{
  int iVar1;
  
  iVar1 = FUN_0800a948(0x20);
  if (iVar1 != 0) {
    iVar1 = FUN_0800aabc(0x48000400,0x20);
    if (iVar1 == 0) {
      if ((int)(_DAT_20000a00 << 0x1f) < 0) {
        if (_DAT_200009ec != 0 || _DAT_200009e8 != 0) {
          FUN_0800A4DC(&DAT_200009f0);
          _DAT_20000a00 = _DAT_20000a00 | 2;
        }
      }
    }
    else {
      FUN_0800A4DC(&DAT_200009e8);
    }
    FUN_0800a958(0x20);
  }
  return;
}


