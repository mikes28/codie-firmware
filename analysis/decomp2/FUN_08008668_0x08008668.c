/* ==== FUN_08008668 @ 0x8008668 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4
FUN_08008668(undefined4 *param_1,undefined1 *param_2,undefined4 param_3,undefined4 param_4)

{
  FUN_08003040();
  if (_DAT_200006e8 != 0) {
    FUN_08008538(1);
    FUN_08006828(_DAT_200006e8,1);
    _DAT_200006e8 = 0;
  }
  FUN_080084C0(*(undefined2 *)(param_2 + 1),*(undefined2 *)(param_2 + 3),*param_2,0,param_4);
  _DAT_200006e8 = *param_1;
  *param_1 = 0;
  return 0;
}


