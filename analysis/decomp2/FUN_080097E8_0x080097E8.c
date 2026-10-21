/* ==== FUN_080097E8 @ 0x80097e8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080097E8(int param_1)

{
  int iVar1;
  
  if (param_1 != 0) {
    while (iVar1 = FUN_0800aabc(0x48000800,0x2000), iVar1 != 0) {
      FUN_0800ab10();
    }
  }
  _DAT_48000828 = 0x1000;
  do {
    FUN_0800ab10();
  } while( true );
}


