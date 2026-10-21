/* ==== FUN_08000380 @ 0x8000380 ==== */

int FUN_08000380(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = FUN_080002E8(param_1,0x4f000000);
  if (iVar1 == 0) {
    iVar1 = FUN_08001208(param_1);
  }
  else {
    FUN_08000E5C(param_1,0x4f000000);
    iVar1 = FUN_08001208();
    iVar1 = iVar1 + -0x80000000;
  }
  return iVar1;
}


