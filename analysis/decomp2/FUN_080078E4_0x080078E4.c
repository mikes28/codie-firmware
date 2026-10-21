/* ==== FUN_080078E4 @ 0x80078e4 ==== */

bool FUN_080078E4(undefined4 param_1,undefined4 param_2,undefined1 param_3)

{
  bool bVar1;
  
  bVar1 = DAT_2000051a == '\0';
  if (bVar1) {
    FUN_08007830(param_1,0x20000530,param_2);
    DAT_20000588 = (undefined1)param_2;
    DAT_20000589 = param_3;
    FUN_080077A0(0);
  }
  return bVar1;
}


