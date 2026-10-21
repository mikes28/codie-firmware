/* ==== FUN_0800AE14 @ 0x800ae14 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800AE14(uint param_1,int param_2)

{
  if (param_2 == 0) {
    _DAT_40010000 = _DAT_40010000 & ~param_1;
  }
  else {
    _DAT_40010000 = param_1 | _DAT_40010000;
  }
  return;
}


