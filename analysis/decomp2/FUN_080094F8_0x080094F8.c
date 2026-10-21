/* ==== FUN_080094F8 @ 0x80094f8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080094F8(void)

{
  undefined8 uVar1;
  byte bVar2;
  undefined2 uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  bool bVar13;
  uint local_28;
  int local_24;
  
  FUN_0800A558(0x20000fd0,&local_28);
  if ((local_24 == 0) && (uVar1 = CONCAT44(_DAT_200008ac,_DAT_200008a8), local_28 <= _DAT_200008c2))
  goto LAB_080095cc;
  uVar10 = (uint)DAT_200008a4;
  uVar3 = FUN_080071DC(2,1,_DAT_20000882);
  *(undefined2 *)(uVar10 * 2 + 0x20000884) = uVar3;
  bVar2 = DAT_200008a4;
  uVar10 = (uint)DAT_200008a4;
  uVar12 = (uint)DAT_200008c0;
  if (uVar12 == 0) {
    uVar7 = 0;
    uVar4 = 0;
  }
  else {
    uVar5 = uVar12 - 1 & 0xff;
    uVar7 = (uint)*(ushort *)(uVar10 * 2 + 0x20000884);
    uVar4 = 0;
    uVar9 = uVar5;
    if (uVar10 != 0) {
      uVar9 = uVar10 - 1 & 0xff;
    }
    uVar11 = 1;
    if (uVar12 != 1) {
      if ((uVar12 - 1 & 1) != 0) {
        uVar4 = (uint)*(ushort *)(uVar9 * 2 + 0x20000884);
        bVar13 = CARRY4(uVar7,uVar4);
        uVar7 = uVar7 + uVar4;
        uVar4 = (uint)bVar13;
        uVar11 = uVar5;
        if (uVar9 != 0) {
          uVar11 = uVar9 - 1 & 0xff;
        }
        uVar9 = uVar11;
        uVar11 = 2;
        if (uVar12 == 2) goto LAB_080095a0;
      }
      do {
        uVar8 = (uint)*(ushort *)(uVar9 * 2 + 0x20000884);
        bVar13 = CARRY4(uVar7,uVar8);
        uVar8 = uVar7 + uVar8;
        uVar6 = uVar5;
        if (uVar9 != 0) {
          uVar6 = uVar9 - 1 & 0xff;
        }
        uVar9 = (uint)*(ushort *)(uVar6 * 2 + 0x20000884);
        uVar7 = uVar8 + uVar9;
        uVar4 = uVar4 + bVar13 + (uint)CARRY4(uVar8,uVar9);
        uVar9 = uVar5;
        if (uVar6 != 0) {
          uVar9 = uVar6 - 1 & 0xff;
        }
        uVar11 = (uVar11 + 1 & 0xff) + 1 & 0xff;
      } while (uVar11 != uVar12);
    }
  }
LAB_080095a0:
  _DAT_200008a8 = FUN_080002fc(uVar7,uVar4,uVar12,0);
  if (uVar10 == uVar12) {
    DAT_200008a4 = 0;
  }
  else {
    DAT_200008a4 = bVar2 + 1;
  }
  FUN_0800A4DC(0x20000fd0);
  uVar1 = _DAT_200008a8;
LAB_080095cc:
  _DAT_200008ac = (undefined4)((ulonglong)uVar1 >> 0x20);
  _DAT_200008a8 = (undefined4)uVar1;
  return;
}


