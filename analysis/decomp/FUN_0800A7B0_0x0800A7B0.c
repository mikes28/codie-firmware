/* ==== FUN_0800A7B0 @ 0x800a7b0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800A7B0(uint *param_1)

{
  *param_1 = *param_1 & 0xfffe;
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  if (param_1 == (uint *)0x40020008) {
    _DAT_40020004 = _DAT_40020004 | 0xf;
  }
  else if (param_1 == (uint *)0x4002001c) {
    _DAT_40020004 = _DAT_40020004 | 0xf0;
  }
  else if (param_1 == (uint *)0x40020030) {
    _DAT_40020004 = _DAT_40020004 | 0xf00;
  }
  else if (param_1 == (uint *)0x40020044) {
    _DAT_40020004 = _DAT_40020004 | 0xf000;
  }
  else if (param_1 == (uint *)0x40020058) {
    _DAT_40020004 = _DAT_40020004 | 0xf0000;
  }
  return;
}


