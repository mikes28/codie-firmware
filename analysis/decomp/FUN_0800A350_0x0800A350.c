/* ==== FUN_0800A350 @ 0x800a350 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800A350(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined1 local_19;
  
  if (_DAT_20001014 == 0) {
    return 0;
  }
  local_20 = 0;
  local_24 = 0;
  local_28 = 0;
  iVar1 = FUN_0800A0AC(param_1,&local_19,&local_20,&local_24,&local_28);
  uVar2 = local_20;
  iVar3 = _DAT_20001014;
  if (iVar1 == 0) {
    FUN_0800A2D0(4,"Invalid command!",0);
    return 0;
  }
  puVar5 = (undefined4 *)(_DAT_20001010 + (_DAT_20001014 + 0x1fffffff) * 8);
  uVar4 = _DAT_20001014 - 1;
  iVar1 = FUN_0800b740(local_20,*puVar5);
  puVar7 = puVar5;
  if (iVar1 == 0) {
LAB_0800a3c4:
    uVar2 = (*(code *)puVar7[1])(local_19,local_24,local_28);
  }
  else {
    puVar6 = puVar5 + -2;
    if (uVar4 != 0) {
      if ((uVar4 & 1) != 0) {
        uVar4 = iVar3 - 2;
        iVar3 = FUN_0800b740(uVar2,*puVar6);
        puVar7 = puVar6;
        if (iVar3 == 0) goto LAB_0800a3c4;
        puVar6 = puVar5 + -4;
        if (uVar4 == 0) goto LAB_0800a3e8;
      }
      do {
        iVar3 = FUN_0800b740(uVar2,*puVar6);
        puVar7 = puVar6;
        if (iVar3 == 0) goto LAB_0800a3c4;
        uVar4 = uVar4 - 2;
        iVar3 = FUN_0800b740(uVar2,puVar6[-2]);
        puVar7 = puVar6 + -2;
        if (iVar3 == 0) goto LAB_0800a3c4;
        puVar6 = puVar6 + -4;
      } while (uVar4 != 0);
    }
LAB_0800a3e8:
    FUN_0800A2D0(4,"No interface:",uVar2);
    uVar2 = 0;
  }
  return uVar2;
}


