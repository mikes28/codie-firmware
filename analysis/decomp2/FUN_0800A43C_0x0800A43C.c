/* ==== FUN_0800A43C @ 0x800a43c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800A43C(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  iVar1 = FUN_0800B3A8(&DAT_40001000,1,param_3,param_4,param_4);
  if (iVar1 == 1) {
    _DAT_20001018 = _DAT_20001018 + 1;
    FUN_0800b3c0(&DAT_40001000,1);
  }
  return;
}


