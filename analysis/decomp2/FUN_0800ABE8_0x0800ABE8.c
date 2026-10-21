/* ==== FUN_0800ABE8 @ 0x800abe8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800ABE8(int param_1)

{
  if (param_1 == 0) {
    _DAT_40021024 = _DAT_40021024 & 0xfffffffe;
  }
  else {
    _DAT_40021024 = _DAT_40021024 | 1;
  }
  return;
}


