/* ==== FUN_08000270 @ 0x8000270 ==== */

ulonglong FUN_08000270(int param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  
  if (param_2 == 0) {
    uVar1 = 0;
    if (param_1 != 0) {
      if (param_1 < 0) {
        uVar1 = 0x80000000;
      }
      else {
        uVar1 = 0x7fffffff;
      }
    }
    return (ulonglong)uVar1;
  }
  iVar2 = FUN_080001c8();
  return CONCAT44(param_1 - iVar2 * param_2,iVar2);
}


