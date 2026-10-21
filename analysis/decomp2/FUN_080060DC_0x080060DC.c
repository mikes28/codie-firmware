/* ==== FUN_080060DC @ 0x80060dc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080060DC(uint param_1)

{
  if ((_DAT_2000035e == param_1) || (param_1 == 0)) {
    _DAT_20000360 = 0;
    _DAT_2000035e = 0;
  }
  if (DAT_2000037e != 0) {
    DAT_2000037e = DAT_2000037e - 1;
    FUN_08005FBC(*(undefined2 *)(&DAT_20000358 + (DAT_2000037e + 0x10) * 2));
  }
  return;
}


