/* ==== FUN_0800AD7C @ 0x800ad7c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800AD7C(uint param_1,int param_2)

{
  if (param_2 == 0) {
    _DAT_40021018 = _DAT_40021018 & ~param_1;
  }
  else {
    _DAT_40021018 = param_1 | _DAT_40021018;
  }
  return;
}


