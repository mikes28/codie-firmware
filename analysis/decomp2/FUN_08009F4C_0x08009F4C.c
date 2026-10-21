/* ==== FUN_08009F4C @ 0x8009f4c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08009F4C(undefined4 *param_1)

{
  int iVar1;
  
  FUN_08003040();
  iVar1 = FUN_08009E30(0x8009cf9);
  if (iVar1 == 0) {
    _DAT_20000a0c = *param_1;
  }
  else {
    FUN_08006848(param_1,2);
  }
  return 0;
}


