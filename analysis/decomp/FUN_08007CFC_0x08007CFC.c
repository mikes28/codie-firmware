/* ==== FUN_08007CFC @ 0x8007cfc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08007CFC(void)

{
  int iVar1;
  undefined4 uVar2;
  
  _DAT_4800140c = _DAT_4800140c & 0xffffcfff | 0x1000;
  FUN_08002DD4(10);
  iVar1 = FUN_0800aabc(0x48001400,0x40);
  if (iVar1 == 0) {
    _DAT_4800140c = _DAT_4800140c & 0xffffcfff;
    uVar2 = 0;
  }
  else {
    _DAT_4800140c = _DAT_4800140c & 0xffffcfff | 0x2000;
    FUN_08002DD4(10);
    iVar1 = FUN_0800aabc(0x48001400,0x40);
    if (iVar1 == 0) {
      _DAT_4800140c = _DAT_4800140c & 0xffffcfff;
      uVar2 = 1;
    }
    else {
      _DAT_4800140c = _DAT_4800140c & 0xffffcfff;
      uVar2 = 2;
    }
  }
  return uVar2;
}


