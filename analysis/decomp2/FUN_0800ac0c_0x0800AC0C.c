/* ==== FUN_0800ac0c @ 0x800ac0c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800ac0c(uint param_1)

{
  _DAT_40021004 = param_1 & 0xffff | _DAT_40021004 & 0xffffbfff;
  _DAT_40021030 = param_1 >> 0x10 | _DAT_40021030 & 0xfffffeff;
  return;
}


