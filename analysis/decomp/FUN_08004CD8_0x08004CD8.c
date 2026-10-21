/* ==== FUN_08004CD8 @ 0x8004cd8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08004CD8(void)

{
  uint uVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 uVar7;
  uint uVar8;
  undefined4 uVar9;
  uint uVar10;
  uint uVar11;
  
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 5;
  }
  uVar5 = FUN_080012F0(_DAT_20000358);
  uVar5 = FUN_08000BFC(uVar5,0x40490fdb);
  uVar5 = FUN_08000818(uVar5,0x42200000);
  iVar6 = FUN_080002A0(uVar5,0);
  if (iVar6 == 0) {
    uVar7 = FUN_08002F48(uVar5);
    uVar5 = FUN_08000818(uVar7,uVar5);
    uVar5 = FUN_08000E5C(0x3f800000,uVar5);
  }
  else {
    uVar5 = 0;
  }
  uVar7 = FUN_08001248(DAT_20000383);
  uVar7 = FUN_08000BFC(uVar7,uVar5);
  FUN_08000474(uVar7,0x3f000000);
  bVar2 = FUN_08000380();
  uVar7 = FUN_08001248(DAT_20000382);
  uVar7 = FUN_08000BFC(uVar7,uVar5);
  FUN_08000474(uVar7,0x3f000000);
  bVar3 = FUN_08000380();
  uVar7 = FUN_08001248(DAT_20000384);
  uVar5 = FUN_08000BFC(uVar7,uVar5);
  FUN_08000474(uVar5,0x3f000000);
  bVar4 = FUN_08000380();
  uVar1 = _DAT_20000358;
  uVar8 = (uint)DAT_20000380;
  uVar10 = (uint)DAT_2000037f;
  uVar11 = (uint)DAT_20000381;
  DAT_20000385 = bVar3;
  DAT_20000386 = bVar2;
  DAT_20000387 = bVar4;
  if (0x96 < _DAT_20000358) {
    uVar5 = FUN_08001248((uint)bVar2);
    uVar7 = FUN_08001248(uVar8 - bVar2);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000386 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar10 - bVar3);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000385 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar11 - bVar4);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000387 = FUN_08000380();
  }
  uVar1 = _DAT_20000358;
  DAT_20000388 = bVar3;
  DAT_20000389 = bVar2;
  DAT_2000038a = bVar4;
  if (0x96 < _DAT_20000358) {
    uVar5 = FUN_08001248((uint)bVar2);
    uVar7 = FUN_08001248(uVar8 - bVar2);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000389 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar10 - bVar3);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000388 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar11 - bVar4);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_2000038a = FUN_08000380();
  }
  uVar1 = _DAT_20000358;
  DAT_2000038b = bVar3;
  DAT_2000038c = bVar2;
  DAT_2000038d = bVar4;
  if (0x96 < _DAT_20000358) {
    uVar5 = FUN_08001248((uint)bVar2);
    uVar7 = FUN_08001248(uVar8 - bVar2);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_2000038c = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar10 - bVar3);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_2000038b = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar11 - bVar4);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_2000038d = FUN_08000380();
  }
  uVar1 = _DAT_20000358;
  DAT_2000038e = bVar3;
  DAT_2000038f = bVar2;
  DAT_20000390 = bVar4;
  if (0x96 < _DAT_20000358) {
    uVar5 = FUN_08001248((uint)bVar2);
    uVar7 = FUN_08001248(uVar8 - bVar2);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_2000038f = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar10 - bVar3);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_2000038e = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar11 - bVar4);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000390 = FUN_08000380();
  }
  uVar1 = _DAT_20000358;
  DAT_20000391 = bVar3;
  DAT_20000392 = bVar2;
  DAT_20000393 = bVar4;
  if (0x96 < _DAT_20000358) {
    uVar5 = FUN_08001248((uint)bVar2);
    uVar7 = FUN_08001248(uVar8 - bVar2);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000392 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar10 - bVar3);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000391 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar11 - bVar4);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000393 = FUN_08000380();
  }
  uVar1 = _DAT_20000358;
  DAT_20000394 = bVar3;
  DAT_20000395 = bVar2;
  DAT_20000396 = bVar4;
  if (0x96 < _DAT_20000358) {
    uVar5 = FUN_08001248((uint)bVar2);
    uVar7 = FUN_08001248(uVar8 - bVar2);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000395 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar10 - bVar3);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000394 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar11 - bVar4);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000396 = FUN_08000380();
  }
  uVar1 = _DAT_20000358;
  DAT_20000397 = bVar3;
  DAT_20000398 = bVar2;
  DAT_20000399 = bVar4;
  if (0x96 < _DAT_20000358) {
    uVar5 = FUN_08001248((uint)bVar2);
    uVar7 = FUN_08001248(uVar8 - bVar2);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000398 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar10 - bVar3);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000397 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar11 - bVar4);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_20000399 = FUN_08000380();
  }
  uVar1 = _DAT_20000358;
  DAT_2000039a = bVar3;
  DAT_2000039b = bVar2;
  DAT_2000039c = bVar4;
  if (0x96 < _DAT_20000358) {
    uVar5 = FUN_08001248((uint)bVar2);
    uVar7 = FUN_08001248(uVar8 - bVar2);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_2000039b = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar10 - bVar3);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_2000039a = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar11 - bVar4);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_2000039c = FUN_08000380();
  }
  uVar1 = _DAT_20000358;
  DAT_2000039d = bVar3;
  DAT_2000039e = bVar2;
  DAT_2000039f = bVar4;
  if (0x96 < _DAT_20000358) {
    uVar5 = FUN_08001248((uint)bVar2);
    uVar7 = FUN_08001248(uVar8 - bVar2);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_2000039e = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar10 - bVar3);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_2000039d = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar11 - bVar4);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_2000039f = FUN_08000380();
  }
  uVar1 = _DAT_20000358;
  DAT_200003a0 = bVar3;
  DAT_200003a1 = bVar2;
  DAT_200003a2 = bVar4;
  if (0x96 < _DAT_20000358) {
    uVar5 = FUN_08001248((uint)bVar2);
    uVar7 = FUN_08001248(uVar8 - bVar2);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_200003a1 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar10 - bVar3);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_200003a0 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar11 - bVar4);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_200003a2 = FUN_08000380();
  }
  uVar1 = _DAT_20000358;
  DAT_200003a3 = bVar3;
  DAT_200003a4 = bVar2;
  DAT_200003a5 = bVar4;
  if (0x96 < _DAT_20000358) {
    uVar5 = FUN_08001248((uint)bVar2);
    uVar7 = FUN_08001248(uVar8 - bVar2);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_200003a4 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar10 - bVar3);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_200003a3 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar5 = FUN_08001248();
    uVar7 = FUN_08001248(uVar11 - bVar4);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_200003a5 = FUN_08000380();
  }
  uVar1 = _DAT_20000358;
  DAT_200003a6 = bVar3;
  DAT_200003a7 = bVar2;
  DAT_200003a8 = bVar4;
  if (0x96 < _DAT_20000358) {
    uVar5 = FUN_08001248((uint)bVar2);
    uVar7 = FUN_08001248(uVar8 - bVar2);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_200003a7 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar8 = (uint)DAT_200003a6;
    uVar5 = FUN_08001248(uVar8);
    uVar7 = FUN_08001248(uVar10 - uVar8);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_200003a6 = FUN_08000380();
    uVar1 = _DAT_20000358;
    uVar8 = (uint)DAT_200003a8;
    uVar5 = FUN_08001248(uVar8);
    uVar7 = FUN_08001248(uVar11 - uVar8);
    uVar9 = FUN_080012F0(uVar1 - 0x96);
    uVar7 = FUN_08000BFC(uVar7,uVar9);
    uVar7 = FUN_08000818(uVar7,0x42c80000);
    FUN_08000474(uVar5,uVar7);
    DAT_200003a8 = FUN_08000380();
  }
  return _DAT_20000358 < 0xfa;
}


