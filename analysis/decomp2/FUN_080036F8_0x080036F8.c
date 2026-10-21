/* ==== FUN_080036F8 @ 0x80036f8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_080036F8(undefined4 *param_1,ushort *param_2)

{
  FUN_08003040();
  FUN_0800328C((int)(char)param_2[1],1);
  FUN_0800328C((int)*(char *)((int)param_2 + 3),2);
  DAT_200002b4 = 1;
  _DAT_200002b0 = 1;
  if (10 < *param_2) {
    _DAT_200002b0 = *param_2 - 10;
  }
  _DAT_200002b2 = _DAT_200002b0;
  _DAT_200002d0 = FUN_08008E00(1);
  _DAT_200002d8 = FUN_08008E00(2);
  if (_DAT_20000350 != 0) {
    FUN_08006828(_DAT_20000350,1);
  }
  _DAT_20000350 = *param_1;
  *param_1 = 0;
  return 0;
}


