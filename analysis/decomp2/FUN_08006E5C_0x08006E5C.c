/* ==== FUN_08006E5C @ 0x8006e5c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006E5C(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_08007980();
  if ((iVar1 == 0) && (_DAT_200004ac == 0)) {
    FUN_08006CFC();
    DAT_200004ab = 1;
    _DAT_200004ac = 0x8006d35;
    uVar2 = 1;
  }
  else {
    DAT_200004a1 = 2;
    uVar2 = 0;
  }
  return uVar2;
}


