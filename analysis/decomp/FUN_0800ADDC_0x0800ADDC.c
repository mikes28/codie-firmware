/* ==== FUN_0800ADDC @ 0x800addc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

uint FUN_0800ADDC(uint param_1)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = param_1 >> 5;
  uVar1 = _DAT_40021000;
  if (((uVar2 != 0) && (uVar1 = _DAT_40021020, uVar2 != 1)) && (uVar1 = _DAT_40021034, uVar2 == 2))
  {
    uVar1 = _DAT_40021024;
  }
  return uVar1 >> (param_1 & 0x1f) & 1;
}


