/* ==== FUN_08008700 @ 0x8008700 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008700(void)

{
  int iVar1;
  undefined4 local_20;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_19;
  
  if (-1 < (int)((uint)DAT_200006f0 << 0x1e)) {
    FUN_0800AD5C(0x20000,1);
    local_1c = 0;
    local_19 = 1;
    local_1b = 0;
    local_20 = 0x1000;
    FUN_0800A964(0x48000000,&local_20);
    FUN_0800AD7C(1,1);
    FUN_0800AE34(0,0xc);
    _DAT_20000fb8 = 0x1000;
    DAT_20000fbc = 0;
    DAT_20000fbd = 0x10;
    DAT_20000fbe = 1;
    FUN_0800A8D4();
    iVar1 = FUN_0800aabc(0x48000000,0x1000);
    _DAT_200006f4 = (uint)(iVar1 == 0);
    DAT_200006f0 = 2;
  }
  return;
}


