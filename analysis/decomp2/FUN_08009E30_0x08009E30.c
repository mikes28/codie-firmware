/* ==== FUN_08009E30 @ 0x8009e30 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08009E30(undefined4 param_1)

{
  undefined4 uVar1;
  
  if ((int)(_DAT_20000a00 << 0x1f) < 0) {
    DAT_20000a08 = 2;
    uVar1 = 2;
  }
  else {
    if (_DAT_20000a0c != 0) {
      FUN_08006848(&DAT_20000a0c,2);
    }
    _DAT_48000018 = 0x80;
    FUN_08002DD4(8);
    _DAT_48000028 = 0x80;
    _DAT_20000a00 = _DAT_20000a00 | 1;
    FUN_0800A4DC(0x200009f8);
    _DAT_200009e8 = 0;
    _DAT_200009ec = 0;
    _DAT_200009f0 = 0;
    _DAT_200009f4 = 0;
    uVar1 = 0;
    _DAT_20000a04 = param_1;
  }
  return uVar1;
}


