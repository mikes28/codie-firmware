/* ==== FUN_08009E0C @ 0x8009e0c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009E0C(void)

{
  if ((DAT_200009e0 & 6) == 2) {
    _DAT_48000028 = 0x80;
    DAT_200009e0 = DAT_200009e0 | 4;
  }
  return;
}


