/* ==== FUN_08005FBC @ 0x8005fbc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08005FBC(undefined4 param_1)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = (uint)_DAT_2000035e;
  if (uVar2 != 0) {
    if (-1 < (int)(uVar2 << 0x17)) {
      return false;
    }
    iVar1 = FUN_08005F94(uVar2);
    if (iVar1 == 0) {
      return false;
    }
  }
  _DAT_20000360 = FUN_08005F04(param_1);
  if (_DAT_20000360 != 0) {
    _DAT_20000358 = 0;
    _DAT_2000035e = (ushort)param_1;
    _DAT_2000035c = 0;
  }
  return _DAT_20000360 != 0;
}


