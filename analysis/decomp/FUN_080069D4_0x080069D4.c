/* ==== FUN_080069D4 @ 0x80069d4 ==== */

void FUN_080069D4(char *param_1,uint param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  char cVar6;
  undefined8 uVar7;
  
  if (*param_1 == '-') {
    cVar6 = -1;
  }
  else {
    cVar6 = '\x01';
  }
  uVar5 = (uint)(*param_1 == '-');
  uVar4 = (uint)(byte)param_1[uVar5];
  if (uVar4 == 0) {
    iVar1 = 0;
    iVar3 = 0;
  }
  else {
    iVar1 = 0;
    iVar3 = 0;
    do {
      if ((param_2 < 0xb) || (uVar4 < 0x3a)) {
        uVar7 = FUN_0800033c(param_2,0,iVar1,iVar3);
        uVar2 = (uint)uVar7 + uVar4;
        iVar1 = uVar2 - 0x30;
        iVar3 = (int)((ulonglong)uVar7 >> 0x20) + (uint)CARRY4((uint)uVar7,uVar4) + -1 +
                (uint)(0x2f < uVar2);
      }
      else {
        uVar7 = FUN_0800033c(param_2,0,iVar1,iVar3);
        uVar2 = uVar4 + (uint)uVar7;
        iVar1 = uVar2 - 0x57;
        iVar3 = (int)((ulonglong)uVar7 >> 0x20) + (uint)CARRY4(uVar4,(uint)uVar7) + -1 +
                (uint)(0x56 < uVar2);
      }
      uVar5 = uVar5 + 1 & 0xff;
      uVar4 = (uint)(byte)param_1[uVar5];
    } while (uVar4 != 0);
  }
  FUN_0800033c((int)cVar6,(int)cVar6 >> 0x1f,iVar1,iVar3);
  return;
}


