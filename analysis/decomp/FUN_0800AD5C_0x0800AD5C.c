/* ==== FUN_0800AD5C @ 0x800ad5c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800AD5C(uint param_1,int param_2)

{
  if (param_2 == 0) {
    _DAT_40021014 = _DAT_40021014 & ~param_1;
  }
  else {
    _DAT_40021014 = param_1 | _DAT_40021014;
  }
  return;
}


