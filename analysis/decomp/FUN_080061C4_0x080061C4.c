/* ==== FUN_080061C4 @ 0x80061c4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_080061C4(undefined4 *param_1,char *param_2)

{
  undefined1 local_14 [4];
  
  FUN_08003040();
  if (_DAT_200003ac != 0) {
    FUN_08006828(_DAT_200003ac,1);
    FUN_080060DC(0);
    _DAT_200003ac = 0;
  }
  local_14[2] = 0x3c;
  local_14._0_2_ = *(undefined2 *)(param_2 + 1);
  if (*param_2 == '\0') {
    FUN_08005FBC(0x107);
    FUN_08005EC4(0x107,0,local_14._0_3_);
    FUN_08005EC4(0x107,1,param_2[3]);
    FUN_08005EC4(0x107,2,param_2[4]);
  }
  else if (*param_2 == '\x01') {
    FUN_08005FBC(0x108);
    FUN_08005EC4(0x108,0,local_14._0_3_);
    FUN_08005EC4(0x108,1,param_2[3]);
    FUN_08005EC4(0x108,2,param_2[4]);
  }
  _DAT_200003ac = *param_1;
  *param_1 = 0;
  return 0;
}


