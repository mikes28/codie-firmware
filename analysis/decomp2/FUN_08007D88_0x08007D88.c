/* ==== FUN_08007D88 @ 0x8007d88 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08007D88(int param_1,byte *param_2,int param_3,byte *param_4)

{
  int iVar1;
  byte *pbVar2;
  
  _DAT_20000f88 = (param_1 + param_3) * 0x528;
  FUN_0800A4DC(0x20000f98);
  _DAT_20000f90 = 0;
  _DAT_20000f94 = 0;
  if ((param_1 != 0) && (pbVar2 = param_2 + (param_1 - 1U & 0xff) + 1, _DAT_20000f88 != 0)) {
LAB_08007dfe:
    do {
      iVar1 = FUN_0800b53c(0x40004800,0x80);
      if (iVar1 == 0) {
        FUN_0800A500(0x20000f98,&DAT_20000f90);
        if ((_DAT_20000f94 == 0) && (_DAT_20000f90 <= _DAT_20000f88)) goto LAB_08007dfe;
      }
      _DAT_40004828 = (ushort)*param_2;
      param_2 = param_2 + 1;
      FUN_0800A500(0x20000f98,&DAT_20000f90);
      if ((param_2 == pbVar2) || ((_DAT_20000f94 != 0 || (_DAT_20000f88 <= _DAT_20000f90)))) break;
    } while( true );
  }
  pbVar2 = param_4 + (param_3 - 1U & 0xffff) + 1;
  if (param_3 != 0) {
LAB_08007e6a:
    do {
      iVar1 = FUN_0800b53c(0x40004800,0x80);
      if ((iVar1 == 0) && (FUN_0800A500(0x20000f98,&DAT_20000f90), _DAT_20000f94 == 0)) {
        if (_DAT_20000f90 <= _DAT_20000f88) goto LAB_08007e6a;
      }
      _DAT_40004828 = (ushort)*param_4;
      param_4 = param_4 + 1;
      FUN_0800A500(0x20000f98,&DAT_20000f90);
    } while (param_4 != pbVar2);
  }
  if ((_DAT_20000f94 != 0) || (_DAT_20000f88 < _DAT_20000f90)) {
    (*_DAT_20000a10)("ERR:BLE_serout_TO\n");
  }
  return;
}


