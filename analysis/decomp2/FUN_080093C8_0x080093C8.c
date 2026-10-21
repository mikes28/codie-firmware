/* ==== FUN_080093C8 @ 0x80093c8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080093C8(void)

{
  undefined4 local_18;
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  undefined1 local_11;
  
  if (-1 < (int)((uint)DAT_20000870 << 0x1e)) {
    FUN_0800AD5C(0x20000,1);
    local_14 = 1;
    local_12 = 0;
    local_11 = 0;
    local_13 = 0;
    local_18 = 0x10;
    FUN_0800A964(0x48000000,&local_18);
    _DAT_48000028 = 0x10;
    DAT_20000870 = 2;
  }
  return;
}


