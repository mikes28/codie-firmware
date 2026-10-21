/* ==== FUN_08005EC4 @ 0x8005ec4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08005EC4(uint param_1,int param_2,undefined4 param_3)

{
  uint uVar1;
  
  uVar1 = (uint)_DAT_2000035e;
  if (uVar1 == param_1) {
    *(undefined4 *)(&DAT_2000035c + (param_2 + 2) * 4) = param_3;
  }
  return uVar1 == param_1;
}


