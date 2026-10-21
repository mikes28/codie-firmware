/* ==== FUN_08006158 @ 0x8006158 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006158(undefined4 param_1,undefined3 *param_2)

{
  undefined3 uVar1;
  int iVar2;
  
  FUN_08003040();
  if (_DAT_200003ac != 0) {
    FUN_08006828(_DAT_200003ac,1);
    iVar2 = FUN_08006124();
    if (iVar2 != 0x106) {
      FUN_080060DC(0);
    }
  }
  uVar1 = *param_2;
  iVar2 = FUN_08006124();
  if (iVar2 != 0x106) {
    FUN_08005FBC(0x106);
  }
  FUN_08005EC4(0x106,0,uVar1);
  FUN_08006130();
  _DAT_200003ac = 0;
  return 0;
}


