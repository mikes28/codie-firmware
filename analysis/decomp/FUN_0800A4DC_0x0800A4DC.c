/* ==== FUN_0800A4DC @ 0x800a4dc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800A4DC(int *param_1)

{
  uint uVar1;
  bool bVar2;
  
  uVar1 = _DAT_20001018 >> 0x10;
  bVar2 = CARRY4(_DAT_40001024,_DAT_20001018 * 0x10000);
  *param_1 = _DAT_40001024 + _DAT_20001018 * 0x10000;
  param_1[1] = uVar1 + bVar2;
  return;
}


