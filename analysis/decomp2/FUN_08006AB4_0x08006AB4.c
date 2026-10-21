/* ==== FUN_08006AB4 @ 0x8006ab4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006AB4(undefined4 *param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  
  uVar6 = param_1[5];
  iVar1 = FUN_080002AC(uVar6,0);
  uVar2 = FUN_08000BFC(uVar6,param_1[3]);
  uVar6 = FUN_08000E5C(uVar6,param_2);
  uVar3 = FUN_080069BC();
  uVar7 = param_1[8];
  iVar4 = FUN_080002AC(uVar3,uVar7);
  if (iVar4 == 0) {
    uVar3 = FUN_08000BFC(uVar6,param_1[1]);
    uVar5 = FUN_08000BFC(uVar6,*param_1);
    uVar2 = FUN_08000474(uVar5,uVar2);
    uVar2 = FUN_08000474(uVar2,param_1[4]);
    uVar2 = FUN_08000474(uVar2,uVar3);
    uVar5 = FUN_08000E5C(uVar6,_DAT_20000f7c);
    uVar5 = FUN_08000BFC(uVar5,param_1[2]);
    uVar2 = FUN_08000474(uVar2,uVar5);
    iVar4 = FUN_080002D4(uVar7,0);
    if ((iVar4 == 0) || (iVar4 = FUN_080002A0(uVar6,0), iVar4 == 0)) goto LAB_08006b6c;
  }
  else {
    uVar3 = FUN_08000BFC(param_1[1],0);
    uVar6 = FUN_08000BFC(*param_1,0);
    uVar2 = FUN_08000474(uVar6,uVar2);
    uVar2 = FUN_08000474(uVar2,param_1[4]);
    uVar2 = FUN_08000474(uVar2,uVar3);
    uVar6 = FUN_08000E5C(0,_DAT_20000f7c);
    uVar6 = FUN_08000BFC(uVar6,param_1[2]);
    uVar2 = FUN_08000474(uVar2,uVar6);
    iVar4 = FUN_080002D4(uVar7,0);
    uVar6 = 0;
    if (iVar4 == 0) goto LAB_08006b6c;
    uVar6 = 0;
  }
  iVar4 = FUN_080002A0(param_1[5],0);
  if (iVar4 != 0) {
    param_1[4] = 0;
    uVar2 = 0;
    uVar3 = 0;
  }
LAB_08006b6c:
  uVar7 = param_1[7];
  iVar4 = FUN_080002D4(uVar2,uVar7);
  if ((iVar4 == 0) && (iVar4 = FUN_080002AC(uVar2,param_1[6]), iVar4 == 0)) {
    uVar3 = FUN_08000474(param_1[4],uVar3);
    param_1[4] = uVar3;
  }
  else {
    uVar2 = FUN_08000E5C(uVar2,uVar3);
  }
  uVar3 = FUN_08001248(-(uint)(iVar1 != 0) | 1);
  uVar3 = FUN_08000BFC(uVar3,uVar2);
  iVar1 = FUN_080002AC(uVar3,0);
  if ((iVar1 != 0) && (param_1[10] == 0)) {
    uVar2 = 0;
  }
  iVar1 = FUN_080002D4(uVar2,uVar7);
  if (iVar1 == 0) {
    uVar7 = uVar2;
  }
  uVar2 = param_1[6];
  iVar1 = FUN_080002AC(uVar7,uVar2);
  if (iVar1 == 0) {
    uVar2 = uVar7;
  }
  param_1[9] = uVar2;
  _DAT_20000f7c = uVar6;
  return uVar2;
}


