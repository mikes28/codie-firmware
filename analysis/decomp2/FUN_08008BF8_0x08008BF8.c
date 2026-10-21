/* ==== FUN_08008BF8 @ 0x8008bf8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008BF8(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  iVar1 = FUN_0800B3A8(0x40012c00,1,param_3,param_4,param_4);
  if (iVar1 != 0) {
    _DAT_2000070c = _DAT_2000070c + 1;
    FUN_0800b3c0(0x40012c00,1);
  }
  return;
}


