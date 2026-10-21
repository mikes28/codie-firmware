/* ==== FUN_08009968 @ 0x8009968 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009968(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  int iVar2;
  
  iVar2 = FUN_0800B54C(0x40004c00,0x50105,param_3,param_4,param_4);
  if (iVar2 == 1) {
    _DAT_200009d4 = _DAT_40004c1c & 0x1f | _DAT_200009d4 & 0xffffffe0;
    cVar1 = FUN_0800b4e4();
    disableIRQinterrupts();
    if (_DAT_200009ca < _DAT_200009c8) {
      enableIRQinterrupts();
      *_DAT_200009d0 = cVar1;
      _DAT_200009d0 = _DAT_200009d0 + 1;
      if (_DAT_200009d0 == _DAT_200009c4 + _DAT_200009c8) {
        _DAT_200009d0 = _DAT_200009c4;
      }
      disableIRQinterrupts();
      _DAT_200009ca = _DAT_200009ca + 1;
      enableIRQinterrupts();
    }
    else {
      (*_DAT_20000a10)("BUFOVF!!!!!\n");
    }
    if (cVar1 == '\n') {
      DAT_200009d8 = DAT_200009d8 + '\x01';
    }
  }
  return;
}


