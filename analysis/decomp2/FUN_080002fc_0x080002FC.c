/* ==== FUN_080002fc @ 0x80002fc ==== */

int FUN_080002fc(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  
  if ((param_4 == 0) && (param_3 == 0)) {
    if (param_2 != 0 || param_1 != 0) {
      param_1 = -1;
    }
    return param_1;
  }
  iVar1 = FUN_0800043C(param_1);
  return iVar1;
}


