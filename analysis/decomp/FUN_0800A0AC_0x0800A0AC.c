/* ==== FUN_0800A0AC @ 0x800a0ac ==== */

undefined4
FUN_0800A0AC(undefined4 param_1,undefined1 *param_2,undefined4 *param_3,int *param_4,int *param_5)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined1 uVar4;
  
  *param_2 = 0;
  uVar1 = FUN_0800B764(param_1,&DAT_0800c6a4,param_3,param_4,param_4);
  iVar2 = FUN_0800b740(uVar1,&DAT_0800b82c);
  if (iVar2 == 0) {
    uVar4 = 3;
  }
  else {
    iVar2 = FUN_0800b740(uVar1,&DAT_0800b828);
    if (iVar2 == 0) {
      uVar4 = 2;
    }
    else {
      iVar2 = FUN_0800b740(uVar1,&DAT_0800b824);
      if (iVar2 != 0) {
        return 0;
      }
      uVar4 = 1;
    }
  }
  *param_2 = uVar4;
  uVar1 = FUN_0800B764(0,&DAT_0800c6a4);
  *param_3 = uVar1;
  iVar2 = FUN_0800B764(0,&DAT_0800c6a4);
  *param_4 = iVar2;
  if (param_3 == (undefined4 *)0x0) {
    uVar1 = 0;
  }
  else if (param_4 == (int *)0x0) {
    uVar1 = 0;
  }
  else {
    iVar3 = FUN_0800b754(iVar2);
    *param_5 = iVar2 + iVar3 + 1;
    uVar1 = 1;
  }
  return uVar1;
}


