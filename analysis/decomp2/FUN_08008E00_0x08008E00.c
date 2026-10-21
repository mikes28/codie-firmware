/* ==== FUN_08008E00 @ 0x8008e00 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

uint FUN_08008E00(int param_1)

{
  uint uVar1;
  
  if (param_1 == 1) {
    uVar1 = (_DAT_40000024 & 0xffff) + _DAT_20000708 * 0x10000;
  }
  else if (param_1 == 2) {
    uVar1 = (_DAT_40012c24 & 0xffff) + _DAT_2000070c * 0x10000;
  }
  else {
    uVar1 = 0;
    if (param_1 == 0x20) {
      uVar1 = (_DAT_40000024 & 0xffff) + _DAT_20000708 * 0x10000 + _DAT_2000070c * 0x10000 +
              (_DAT_40012c24 & 0xffff) >> 1;
    }
  }
  return uVar1;
}


