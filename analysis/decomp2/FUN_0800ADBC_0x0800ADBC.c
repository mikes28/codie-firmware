/* ==== FUN_0800ADBC @ 0x800adbc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800ADBC(uint param_1,int param_2)

{
  if (param_2 == 0) {
    _DAT_4002100c = _DAT_4002100c & ~param_1;
  }
  else {
    _DAT_4002100c = param_1 | _DAT_4002100c;
  }
  return;
}


