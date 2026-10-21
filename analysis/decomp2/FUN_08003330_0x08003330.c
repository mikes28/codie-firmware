/* ==== FUN_08003330 @ 0x8003330 ==== */

// WARNING: Removing unreachable block (ram,0x080033ee)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08003330(void)

{
  undefined8 uVar1;
  undefined8 uVar2;
  char cVar3;
  short sVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  uint uVar9;
  uint uVar10;
  int iVar11;
  longlong lVar12;
  uint local_28;
  int local_24;
  
  uVar1 = _DAT_20000f50;
  uVar2 = _DAT_20000f68;
  if ((int)((uint)DAT_20000354 << 0x1d) < 0) {
    FUN_0800A558(0x20000f60,&DAT_20000f40);
    if ((((_DAT_20000f44 != 0) || (DAT_200002cc < _DAT_20000f40)) &&
        ((int)((uint)DAT_200002b4 << 0x1d) < 0)) && (DAT_200002bc != '\0')) {
      uVar5 = FUN_08009444();
      uVar6 = FUN_08001248((int)DAT_200002bc);
      uVar8 = _DAT_200002c0;
      uVar5 = FUN_08001248(uVar5);
      uVar7 = FUN_08000BFC(uVar8,_DAT_200002c4);
      uVar7 = FUN_08000BFC(uVar7,uVar5);
      _DAT_200002f4 = FUN_08000474(uVar6,uVar7);
      uVar8 = FUN_08000BFC(uVar8,_DAT_200002c8);
      uVar8 = FUN_08000BFC(uVar5,uVar8);
      _DAT_20000320 = FUN_08000E5C(uVar6,uVar8);
      FUN_0800A4DC(0x20000f60);
    }
    FUN_0800A558(0x20000f48,&DAT_20000f40);
    if ((((int)((uint)DAT_200002b4 << 0x1f) < 0) && (_DAT_200002b0 != 0)) &&
       ((_DAT_20000f44 != 0 || (10 < _DAT_20000f40)))) {
      uVar9 = FUN_08008E70(_DAT_200002d0,_DAT_200002d4,1);
      uVar10 = FUN_08008E70(_DAT_200002d8,_DAT_200002dc,2);
      if (uVar9 < (_DAT_200002b0 & 0xffff)) {
        cVar3 = FUN_08001208(_DAT_200002f4);
        FUN_0800328C((int)cVar3,1);
      }
      else {
        FUN_0800328C(0,1);
        _DAT_200002b0 = _DAT_200002b0 & 0xffff0000;
      }
      if (uVar10 < _DAT_200002b0 >> 0x10) {
        cVar3 = FUN_08001208(_DAT_20000320);
        FUN_0800328C((int)cVar3,2);
      }
      else {
        FUN_0800328C(0,2);
        _DAT_200002b0 = _DAT_200002b0 & 0xffff;
      }
      uVar8 = FUN_080012F0((_DAT_200002b0 >> 0x10) - uVar10);
      uVar5 = FUN_08000BFC(_DAT_20000320,0x41700000);
      uVar5 = FUN_08000818(uVar5,0x41200000);
      iVar11 = FUN_080002AC(uVar8,uVar5);
      if (iVar11 != 0) {
        uVar8 = FUN_080012F0((_DAT_200002b0 & 0xffff) - uVar9);
        uVar5 = FUN_08000BFC(_DAT_200002f4,0x41700000);
        uVar5 = FUN_08000818(uVar5,0x41200000);
        iVar11 = FUN_080002AC(uVar8,uVar5);
        if ((iVar11 != 0) && (_DAT_20000350 != 0)) {
          FUN_08006828(_DAT_20000350,0);
          _DAT_20000350 = 0;
        }
      }
      if ((_DAT_200002b0 == 0) && (DAT_200002b4 = DAT_200002b4 & 0xfe, _DAT_20000350 != 0)) {
        FUN_08006828(_DAT_20000350,0);
        _DAT_20000350 = 0;
      }
      FUN_0800A4DC(0x20000f48);
    }
    FUN_0800A558(0x20000f58,&DAT_20000f40);
    uVar8 = _DAT_200002f4;
    if (((_DAT_20000f44 == 0) && (_DAT_20000f40 < 0x65)) || (_DAT_200002b8 == 0)) {
      iVar11 = FUN_080002A0(_DAT_200002f4,0);
      if (iVar11 == 0) {
        iVar11 = FUN_080002AC(uVar8,0);
        if (iVar11 == 0) {
          sVar4 = 0x208;
        }
        else {
          sVar4 = -0x208;
        }
        uVar5 = FUN_08001248((int)sVar4);
        uVar8 = FUN_08000BFC(uVar8,0x43f00000);
        uVar8 = FUN_08000818(uVar8,0x42c80000);
        FUN_08000474(uVar5,uVar8);
        sVar4 = FUN_08001208();
        FUN_08008AD0((int)sVar4,1);
      }
      else {
        FUN_08008AD0(0,1);
        FUN_0800A558(0x20000340,&local_28);
        if ((local_24 != 0) || (200 < local_28)) {
          FUN_08008BD4(0,1);
        }
      }
      uVar8 = _DAT_20000320;
      iVar11 = FUN_080002A0(_DAT_20000320,0);
      if (iVar11 == 0) {
        iVar11 = FUN_080002AC(uVar8,0);
        if (iVar11 == 0) {
          sVar4 = 0x208;
        }
        else {
          sVar4 = -0x208;
        }
        uVar5 = FUN_08001248((int)sVar4);
        uVar8 = FUN_08000BFC(uVar8,0x43f00000);
        uVar8 = FUN_08000818(uVar8,0x42c80000);
        FUN_08000474(uVar5,uVar8);
        sVar4 = FUN_08001208();
        FUN_08008AD0((int)sVar4,2);
        uVar1 = CONCAT44(_DAT_20000f54,_DAT_20000f50);
        uVar2 = CONCAT44(_DAT_20000f6c,_DAT_20000f68);
      }
      else {
        FUN_08008AD0(0,2);
        FUN_0800A558(0x20000348,&local_28);
        if ((local_24 != 0) ||
           (uVar1 = CONCAT44(_DAT_20000f54,_DAT_20000f50),
           uVar2 = CONCAT44(_DAT_20000f6c,_DAT_20000f68), 200 < local_28)) {
          FUN_08008BD4(0,2);
          uVar1 = CONCAT44(_DAT_20000f54,_DAT_20000f50);
          uVar2 = CONCAT44(_DAT_20000f6c,_DAT_20000f68);
        }
      }
    }
    else {
      lVar12 = FUN_08008E00(1);
      lVar12 = lVar12 - CONCAT44(_DAT_20000f54,_DAT_20000f50);
      uVar8 = FUN_080003b0((int)lVar12,(int)((ulonglong)lVar12 >> 0x20));
      FUN_080089B0(1);
      uVar5 = FUN_08001248();
      _DAT_20000338 = FUN_08000BFC(uVar8,uVar5);
      lVar12 = FUN_08008E00(2);
      lVar12 = lVar12 - CONCAT44(_DAT_20000f6c,_DAT_20000f68);
      uVar8 = FUN_080003b0((int)lVar12,(int)((ulonglong)lVar12 >> 0x20));
      FUN_080089B0(2);
      uVar5 = FUN_08001248();
      _DAT_2000033c = FUN_08000BFC(uVar8,uVar5);
      _DAT_20000f50 = FUN_08008E00(1);
      _DAT_20000f68 = FUN_08008E00(2);
      FUN_0800A4DC(0x20000f58);
      FUN_08006AB4(&DAT_200002e0,_DAT_20000338);
      FUN_08006AB4(&DAT_2000030c,_DAT_2000033c);
      FUN_08006c40(&DAT_200002e0);
      sVar4 = FUN_08001208();
      FUN_08008AD0((int)sVar4,1);
      FUN_08006c40(&DAT_2000030c);
      sVar4 = FUN_08001208();
      FUN_08008AD0((int)sVar4,2);
      uVar1 = _DAT_20000f50;
      uVar2 = _DAT_20000f68;
    }
  }
  _DAT_20000f6c = (undefined4)((ulonglong)uVar2 >> 0x20);
  _DAT_20000f68 = (undefined4)uVar2;
  _DAT_20000f54 = (undefined4)((ulonglong)uVar1 >> 0x20);
  _DAT_20000f50 = (undefined4)uVar1;
  return;
}


