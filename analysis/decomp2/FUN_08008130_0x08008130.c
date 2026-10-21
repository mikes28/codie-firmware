/* ==== FUN_08008130 @ 0x8008130 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008130(void)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint local_28;
  int local_24;
  uint local_20;
  int local_1c;
  
  FUN_0800A558(0,&local_20);
  if (((local_1c != 0) || (2000 < local_20)) && (_DAT_20000630 == 0)) {
    FUN_08002ac0(0x39,2,2);
    FUN_08002ac0(0x32,1);
    _DAT_20000630 = 1;
  }
  if (-1 < (int)(_DAT_2000062c << 0x19)) {
    disableIRQinterrupts();
    enableIRQinterrupts();
    if ((3 < _DAT_20000622) && (iVar1 = FUN_08002E78(&DAT_2000061c,&DAT_20000fa0,4), iVar1 == 4)) {
      if (DAT_20000fa1 < 0x81) {
        _DAT_2000062c = _DAT_2000062c | 0x40;
        FUN_0800A4DC(0x20000638);
      }
      else {
        _DAT_20000640 = _DAT_20000640 + 1;
      }
    }
    if (-1 < (int)(_DAT_2000062c << 0x19)) goto LAB_0800827c;
  }
  FUN_0800A500(0x20000638,&local_28);
  if ((local_24 == 0) && (local_28 < 0x3a99)) {
    iVar1 = FUN_08002A60(_DAT_20000fa0);
    if (iVar1 == 0) {
      iVar1 = FUN_080080AC(&DAT_20000fa0);
      if (iVar1 == 0) {
        _DAT_2000062c = _DAT_2000062c & 0xffffffbf;
        _DAT_20000640 = 0;
      }
      else {
        FUN_0800A4DC(0x20000638);
        _DAT_20000640 = 0;
      }
    }
    else {
      disableIRQinterrupts();
      enableIRQinterrupts();
      uVar3 = _DAT_20000fa0 >> 8 & 0xff;
      if (uVar3 <= _DAT_20000622) {
        uVar2 = FUN_08002E78(&DAT_2000061c);
        if (uVar2 == (_DAT_20000fa0 >> 8 & 0xff)) {
          (**(code **)(iVar1 + 8))((int)&local_28 - (uVar3 + 7 & 0xfffffff8));
          _DAT_20000640 = 0;
        }
        _DAT_2000062c = _DAT_2000062c & 0xffffffbf;
      }
    }
  }
  else {
    _DAT_2000062c = _DAT_2000062c & 0xffffffbf;
  }
LAB_0800827c:
  if ((int)(_DAT_2000062c << 0x1e) < 0) {
    FUN_0800A2D0(4,"BLE framing error",0);
    FUN_0800b548(0x40004800,2);
    _DAT_2000062c = _DAT_2000062c & 0xfffffffd;
  }
  if ((int)(_DAT_2000062c << 0x1b) < 0) {
    FUN_0800b548(0x40004800,0x10);
    _DAT_2000062c = _DAT_2000062c & 0xffffffef;
  }
  if ((int)(_DAT_2000062c << 0x1d) < 0) {
    FUN_0800A2D0(4,"BLE USART noise",0);
    FUN_0800b548(0x40004800,4);
    _DAT_2000062c = _DAT_2000062c & 0xfffffffb;
  }
  if ((int)(_DAT_2000062c << 0x1c) < 0) {
    FUN_0800A2D0(4,"BLE USART overrun",0);
    FUN_0800b548(0x40004800,8);
    _DAT_2000062c = _DAT_2000062c & 0xfffffff7;
  }
  if (5 < _DAT_20000640) {
    FUN_08002f38(&DAT_2000061c);
    _DAT_20000640 = 0;
  }
  if (((int)(_DAT_2000062c << 0x1a) < 0) && (_DAT_20000622 < _DAT_20000620)) {
    _DAT_2000062c = _DAT_2000062c & 0xffffffdf;
    FUN_0800B4EC(0x40004800,0x50105,1);
  }
  return;
}


