/* ==== FUN_08006294 @ 0x8006294 ==== */

void FUN_08006294(int param_1)

{
  int iVar1;
  
  if (param_1 == 2) {
    DAT_20000410 = 2;
    iVar1 = FUN_08005FBC(4);
    if (iVar1 == 0) {
      FUN_08005F94(4);
    }
    FUN_08005EC4(4,0,2);
  }
  else {
    FUN_080060DC(4);
  }
  return;
}


