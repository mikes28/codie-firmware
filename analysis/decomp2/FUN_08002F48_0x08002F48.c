/* ==== FUN_08002F48 @ 0x8002f48 ==== */

void FUN_08002F48(int param_1)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  uint uVar7;
  undefined4 uVar8;
  char cVar9;
  
  iVar2 = FUN_080002AC(param_1,0);
  if (iVar2 == 0) {
    cVar9 = '\x01';
  }
  else {
    param_1 = param_1 + -0x80000000;
    cVar9 = -1;
  }
  iVar2 = FUN_080002E8(param_1,0x40c90fdb);
  if (iVar2 != 0) {
    FUN_08000818(param_1,0x40c90fdb);
    FUN_08001208();
    uVar3 = FUN_08001248();
    uVar3 = FUN_08000BFC(uVar3,0x40c90fdb);
    param_1 = FUN_08000E5C(param_1,uVar3);
  }
  iVar2 = FUN_080002D4(param_1,0x40490fdb);
  if (iVar2 != 0) {
    cVar9 = -cVar9;
    param_1 = FUN_08000E5C(param_1,0x40490fdb);
  }
  iVar2 = FUN_080002D4(param_1,0x3fc90fdb);
  if (iVar2 != 0) {
    param_1 = FUN_08000E5C(0x40490fdb,param_1);
  }
  uVar3 = FUN_08000BFC(param_1,0x41f80000);
  uVar3 = FUN_08000818(uVar3,0x3fc90fdb);
  bVar1 = FUN_08000380();
  uVar7 = (uint)bVar1;
  uVar8 = *(undefined4 *)(&DAT_0800c37c + uVar7 * 4);
  uVar4 = FUN_08001248((int)cVar9);
  uVar5 = FUN_08000E5C(*(undefined4 *)(&DAT_0800c37c + (uVar7 + 1) * 4),uVar8);
  uVar6 = FUN_08001248(uVar7);
  uVar3 = FUN_08000E5C(uVar3,uVar6);
  uVar3 = FUN_08000BFC(uVar5,uVar3);
  uVar3 = FUN_08000474(uVar8,uVar3);
  FUN_08000BFC(uVar4,uVar3);
  return;
}


