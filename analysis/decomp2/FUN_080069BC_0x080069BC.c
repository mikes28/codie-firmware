/* ==== FUN_080069BC @ 0x80069bc ==== */

int FUN_080069BC(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_080002AC(param_1,0);
  if (iVar1 != 0) {
    param_1 = param_1 + -0x80000000;
  }
  return param_1;
}


