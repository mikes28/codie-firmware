/* ==== FUN_0800A8D4 @ 0x800a8d4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800A8D4(uint *param_1)

{
  if (*(char *)((int)param_1 + 6) == '\0') {
    *(uint *)(&DAT_40010400 + (byte)param_1[1]) =
         *(uint *)(&DAT_40010400 + (byte)param_1[1]) & ~*param_1;
  }
  else {
    _DAT_40010400 = _DAT_40010400 & ~*param_1;
    _DAT_40010404 = _DAT_40010404 & ~*param_1;
    *(uint *)(&DAT_40010400 + (byte)param_1[1]) =
         *(uint *)(&DAT_40010400 + (byte)param_1[1]) | *param_1;
    _DAT_40010408 = _DAT_40010408 & ~*param_1;
    _DAT_4001040c = _DAT_4001040c & ~*param_1;
    if (*(byte *)((int)param_1 + 5) == 0x10) {
      _DAT_40010408 = _DAT_40010408 | *param_1;
      _DAT_4001040c = *param_1 | _DAT_4001040c;
    }
    else {
      *(uint *)(&DAT_40010400 + *(byte *)((int)param_1 + 5)) =
           *param_1 | *(uint *)(&DAT_40010400 + *(byte *)((int)param_1 + 5));
    }
  }
  return;
}


