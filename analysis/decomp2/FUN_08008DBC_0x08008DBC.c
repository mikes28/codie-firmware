/* ==== FUN_08008DBC @ 0x8008dbc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008DBC(void)

{
  undefined4 in_r3;
  
  if ((DAT_20000704 & 6) == 2) {
    _DAT_48000018 = 0x8000;
    FUN_0800AFA0(0x40012c00,1,0x48000000,0x90,in_r3);
    FUN_0800AFA0(0x40000000,1);
    DAT_20000704 = DAT_20000704 | 4;
  }
  return;
}


