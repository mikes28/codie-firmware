/* ==== FUN_080071DC @ 0x80071dc ==== */

undefined8 FUN_080071DC(uint param_1,uint param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  uint uVar12;
  char cVar13;
  int iVar14;
  int iVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  uint uVar19;
  uint uVar20;
  uint uVar21;
  
  if (DAT_200004b1 == param_1) {
    cVar13 = '\0';
  }
  else if (DAT_200004b2 == param_1) {
    cVar13 = '\x01';
  }
  else if (DAT_200004b3 == param_1) {
    cVar13 = '\x02';
  }
  else {
    uVar5 = 0;
    cVar13 = '\x03';
    if (DAT_200004b4 != param_1) goto LAB_080073d4;
  }
  if (param_2 == 0) {
    iVar14 = (int)cVar13;
    uVar12 = (uint)*(ushort *)(iVar14 * 2 + 0x200004b6);
    uVar16 = (uint)*(ushort *)((iVar14 + 4) * 2 + 0x200004b6);
    uVar5 = uVar12 + uVar16;
    uVar17 = (uint)*(ushort *)((iVar14 + 8) * 2 + 0x200004b6);
    uVar1 = uVar5 + uVar17;
    uVar18 = (uint)*(ushort *)((iVar14 + 0xc) * 2 + 0x200004b6);
    uVar2 = uVar1 + uVar18;
    uVar19 = (uint)*(ushort *)((iVar14 + 0x10) * 2 + 0x200004b6);
    uVar3 = uVar2 + uVar19;
    uVar20 = (uint)*(ushort *)((iVar14 + 0x14) * 2 + 0x200004b6);
    uVar4 = uVar3 + uVar20;
    uVar21 = (uint)*(ushort *)((iVar14 + 0x18) * 2 + 0x200004b6);
    param_2 = (uint)CARRY4(uVar12,uVar16) + (uint)CARRY4(uVar5,uVar17) + (uint)CARRY4(uVar1,uVar18)
              + (uint)CARRY4(uVar2,uVar19) + (uint)CARRY4(uVar3,uVar20) + (uint)CARRY4(uVar4,uVar21)
    ;
    uVar5 = (uint)*(ushort *)((iVar14 + 0x1c) * 2 + 0x200004b6) + uVar4 + uVar21 >> 3 & 0xffff;
  }
  else {
    uVar5 = 0;
    if (param_2 == 1) {
      iVar14 = (int)cVar13;
      uVar5 = (uint)*(ushort *)(iVar14 * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar15 = uVar5 - param_3;
      }
      else {
        iVar15 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 4) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar6 = uVar5 - param_3;
      }
      else {
        iVar6 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 8) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar7 = uVar5 - param_3;
      }
      else {
        iVar7 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 0xc) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar8 = uVar5 - param_3;
      }
      else {
        iVar8 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 0x10) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar9 = uVar5 - param_3;
      }
      else {
        iVar9 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 0x14) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar10 = uVar5 - param_3;
      }
      else {
        iVar10 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 0x18) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar11 = uVar5 - param_3;
      }
      else {
        iVar11 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 0x1c) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar14 = uVar5 - param_3;
      }
      else {
        iVar14 = param_3 - uVar5;
      }
      param_2 = (uint)(iVar14 + iVar15 + iVar6 + iVar7 + iVar8 + iVar9 + iVar10 + iVar11) >> 3;
      uVar5 = param_2 & 0xffff;
    }
  }
LAB_080073d4:
  return CONCAT44(param_2,uVar5);
}


