/* ==== FUN_08008910 @ 0x8008910 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008910(void)

{
  if ((DAT_200006fc & 6) == 2) {
    _DAT_48000818 = 4;
    FUN_0800B244(0x40000400,0,1);
    FUN_0800B244(0x40000400,4,1);
    FUN_0800B244(0x40000400,8,1);
    FUN_0800B244(0x40000400,0xc,1);
    FUN_0800b3a4(0x40000400,1);
    FUN_0800AFA0(0x40000400,1);
    FUN_0800AFBC(0x40000400,1);
    DAT_200006fc = DAT_200006fc | 4;
  }
  return;
}


