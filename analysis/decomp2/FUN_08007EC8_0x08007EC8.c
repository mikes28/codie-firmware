/* ==== FUN_08007EC8 @ 0x8007ec8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08007EC8(void)

{
  undefined1 uVar1;
  int iVar2;
  
  iVar2 = FUN_0800B54C(0x40004800,0x50105);
  if (iVar2 == 1) {
    _DAT_2000062c = _DAT_4000481c & 0x1f | _DAT_2000062c & 0xffffffe0;
    if (((_DAT_4000481c & 0x1f) == 0) || ((int)(_DAT_4000481c << 0x1b) < 0)) {
      if (_DAT_20000622 < _DAT_20000620) {
        uVar1 = FUN_0800b4e4(0x40004800);
        disableIRQinterrupts();
        if (_DAT_20000622 < _DAT_20000620) {
          enableIRQinterrupts();
          *_DAT_20000628 = uVar1;
          _DAT_20000628 = _DAT_20000628 + 1;
          if (_DAT_20000628 == _DAT_2000061c + _DAT_20000620) {
            _DAT_20000628 = _DAT_2000061c;
          }
          disableIRQinterrupts();
          _DAT_20000622 = _DAT_20000622 + 1;
          enableIRQinterrupts();
        }
      }
      else {
        _DAT_2000062c = _DAT_2000062c | 0x20;
        FUN_0800B4EC(0x40004800,0x50105,0);
      }
    }
    else {
      FUN_0800B524(0x40004800,8,1);
    }
  }
  return;
}


