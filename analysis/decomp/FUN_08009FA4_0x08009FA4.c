/* ==== FUN_08009FA4 @ 0x8009fa4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009FA4(void)

{
  int iVar1;
  undefined4 local_18;
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  undefined1 local_11;
  
  if ((_DAT_200025a8 == -0x281a0858) && (_DAT_200017a8 == 0x19c3fe40)) {
    _DAT_200017a8 = 0;
    _DAT_200025a8 = 0;
    FUN_0800ab88();
    _DAT_e000e010 = 0;
    _DAT_e000e014 = 0;
    _DAT_e000e018 = 0;
    FUN_0800AD5C(0x80000,1);
    local_14 = 1;
    local_12 = 0;
    local_11 = 0;
    local_13 = 0;
    local_18 = 0x5000;
    FUN_0800A964(0x48000800,&local_18);
    _DAT_48000818 = 0x4000;
    FUN_08009F8C();
  }
  FUN_0800A640();
  FUN_0800A464();
  FUN_0800A4C0();
  FUN_08006338();
  FUN_0800a150(0x20000a44,0x10);
  FUN_0800A1C8(0x20000a14,0);
  do {
    iVar1 = FUN_08009B14(&DAT_20000fe8,0x28);
    if (iVar1 != 0) {
      if (DAT_20000fe8 == '\0') {
        FUN_0800A2D0(4,"possible cmdBuf overflow",0);
      }
      FUN_0800A350(&DAT_20000fe8);
    }
    FUN_080063EC();
    FUN_0800ab10();
  } while( true );
}


