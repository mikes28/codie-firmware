/* ==== FUN_080001b0 @ 0x80001b0 ==== */

ulonglong FUN_080001b0(int param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  
  if (param_2 == 0) {
    uVar1 = 0;
    if (param_1 != 0) {
      uVar1 = 0xffffffff;
    }
    return (ulonglong)uVar1;
  }
  iVar2 = FUN_0800012c();
  return CONCAT44(param_1 - iVar2 * param_2,iVar2);
}


