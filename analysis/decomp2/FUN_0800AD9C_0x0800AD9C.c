/* ==== FUN_0800AD9C @ 0x800ad9c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800AD9C(uint param_1,int param_2)

{
  if (param_2 == 0) {
    _DAT_4002101c = _DAT_4002101c & ~param_1;
  }
  else {
    _DAT_4002101c = param_1 | _DAT_4002101c;
  }
  return;
}


