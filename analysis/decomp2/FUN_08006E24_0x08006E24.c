/* ==== FUN_08006E24 @ 0x8006e24 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined1 FUN_08006E24(void)

{
  int iVar1;
  
  iVar1 = FUN_08007CE4();
  if (iVar1 == 0) {
    DAT_200004aa = 4;
  }
  else if (_DAT_200004a4 < 0x2081) {
    DAT_200004aa = 0x11;
  }
  else {
    DAT_200004aa = 0x12;
  }
  return DAT_200004aa;
}


