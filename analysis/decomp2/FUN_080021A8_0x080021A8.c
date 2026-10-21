/* ==== FUN_080021A8 @ 0x80021a8 ==== */

undefined8 FUN_080021A8(uint param_1,uint param_2,uint param_3,uint param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  int extraout_r1;
  int iVar7;
  uint uVar8;
  int extraout_r1_00;
  int extraout_r1_01;
  int extraout_r1_02;
  int extraout_r1_03;
  int extraout_r1_04;
  int extraout_r1_05;
  int extraout_r1_06;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  bool bVar13;
  uint local_38;
  
  uVar10 = 0;
  if ((int)param_2 < 0) {
    bVar13 = param_1 != 0;
    param_1 = -param_1;
    param_2 = -(uint)bVar13 - param_2;
    uVar10 = 0xffffffff;
  }
  if ((int)param_4 < 0) {
    uVar10 = ~uVar10;
    bVar13 = param_3 != 0;
    param_3 = -param_3;
    param_4 = -(uint)bVar13 - param_4;
  }
  if (param_4 == 0) {
    if (param_2 < param_3) {
      uVar1 = FUN_0800216c(param_3);
      if (uVar1 != 0) {
        param_2 = param_1 >> (0x20 - uVar1 & 0xff) | param_2 << (uVar1 & 0xff);
        param_1 = param_1 << (uVar1 & 0xff);
        param_3 = param_3 << (uVar1 & 0xff);
      }
      uVar11 = param_3 >> 0x10;
      iVar2 = FUN_08000128(param_2,uVar11);
      uVar6 = iVar2 * (param_3 & 0xffff);
      FUN_080001b0(param_2,uVar11);
      uVar1 = extraout_r1 << 0x10 | param_1 >> 0x10;
      iVar7 = iVar2;
      if (uVar1 <= uVar6 && uVar6 - uVar1 != 0) {
        uVar1 = uVar1 + param_3;
        iVar7 = iVar2 + -1;
        if ((param_3 <= uVar1) && (uVar1 <= uVar6 && uVar6 - uVar1 != 0)) {
          iVar7 = iVar2 + -2;
          uVar1 = uVar1 + param_3;
        }
      }
      uVar3 = FUN_08000128(uVar1 - uVar6,uVar11);
      uVar8 = uVar3 * (param_3 & 0xffff);
      FUN_080001b0(uVar1 - uVar6,uVar11);
      uVar1 = extraout_r1_00 << 0x10 | param_1 & 0xffff;
      uVar12 = uVar3;
      if (uVar1 <= uVar8 && uVar8 - uVar1 != 0) {
        uVar1 = uVar1 + param_3;
        uVar12 = uVar3 - 1;
        if ((param_3 <= uVar1) && (uVar1 <= uVar8 && uVar8 - uVar1 != 0)) {
          uVar12 = uVar3 - 2;
        }
      }
      uVar12 = iVar7 << 0x10 | uVar12;
      uVar1 = 0;
    }
    else {
      if (param_3 == 0) {
        param_3 = FUN_08000128(1,0);
      }
      uVar1 = FUN_0800216c(param_3);
      if (uVar1 == 0) {
        iVar2 = param_2 - param_3;
        local_38 = param_3 & 0xffff;
        uVar12 = param_3 >> 0x10;
        uVar1 = 1;
      }
      else {
        uVar6 = param_1 >> (0x20 - uVar1 & 0xff);
        uVar11 = param_2 >> (0x20 - uVar1 & 0xff);
        param_3 = param_3 << (uVar1 & 0xff);
        param_1 = param_1 << (uVar1 & 0xff);
        uVar6 = uVar6 | param_2 << (uVar1 & 0xff);
        uVar12 = param_3 >> 0x10;
        local_38 = param_3 & 0xffff;
        iVar2 = FUN_08000128(uVar11,uVar12);
        uVar1 = iVar2 * local_38;
        FUN_080001b0(uVar11,uVar12);
        uVar11 = extraout_r1_03 << 0x10 | uVar6 >> 0x10;
        iVar7 = iVar2;
        if (uVar11 <= uVar1 && uVar1 - uVar11 != 0) {
          uVar11 = uVar11 + param_3;
          iVar7 = iVar2 + -1;
          if ((param_3 <= uVar11) && (uVar11 <= uVar1 && uVar1 - uVar11 != 0)) {
            iVar7 = iVar2 + -2;
            uVar11 = uVar11 + param_3;
          }
        }
        uVar3 = FUN_08000128(uVar11 - uVar1,uVar12);
        uVar8 = uVar3 * local_38;
        FUN_080001b0(uVar11 - uVar1,uVar12);
        uVar6 = extraout_r1_04 << 0x10 | uVar6 & 0xffff;
        uVar1 = uVar3;
        if (uVar6 <= uVar8 && uVar8 - uVar6 != 0) {
          uVar6 = uVar6 + param_3;
          uVar1 = uVar3 - 1;
          if ((param_3 <= uVar6) && (uVar6 <= uVar8 && uVar8 - uVar6 != 0)) {
            uVar1 = uVar3 - 2;
            uVar6 = uVar6 + param_3;
          }
        }
        iVar2 = uVar6 - uVar8;
        uVar1 = uVar1 | iVar7 << 0x10;
      }
      iVar4 = FUN_08000128(iVar2,uVar12);
      uVar11 = iVar4 * local_38;
      FUN_080001b0(iVar2,uVar12);
      uVar6 = extraout_r1_01 << 0x10 | param_1 >> 0x10;
      iVar7 = iVar4;
      if (uVar6 <= uVar11 && uVar11 - uVar6 != 0) {
        uVar6 = uVar6 + param_3;
        iVar7 = iVar4 + -1;
        if ((param_3 <= uVar6) && (uVar6 <= uVar11 && uVar11 - uVar6 != 0)) {
          iVar7 = iVar4 + -2;
          uVar6 = uVar6 + param_3;
        }
      }
      uVar3 = FUN_08000128(uVar6 - uVar11,uVar12);
      FUN_080001b0(uVar6 - uVar11,uVar12);
      uVar6 = extraout_r1_02 << 0x10 | param_1 & 0xffff;
      uVar12 = uVar3;
      if (uVar6 < uVar3 * local_38) {
        uVar6 = uVar6 + param_3;
        uVar12 = uVar3 - 1;
        if ((param_3 <= uVar6) && (uVar6 < uVar3 * local_38)) {
          uVar12 = uVar3 - 2;
        }
      }
      uVar12 = iVar7 << 0x10 | uVar12;
    }
    goto LAB_08002296;
  }
  if (param_4 <= param_2) {
    uVar1 = FUN_0800216c(param_4);
    if (uVar1 != 0) {
      uVar11 = 0x20 - uVar1;
      uVar3 = param_4 << (uVar1 & 0xff) | param_3 >> (uVar11 & 0xff);
      param_3 = param_3 << (uVar1 & 0xff);
      uVar6 = param_2 >> (uVar11 & 0xff);
      uVar12 = param_1 >> (uVar11 & 0xff) | param_2 << (uVar1 & 0xff);
      uVar8 = uVar3 >> 0x10;
      iVar2 = FUN_08000128(uVar6,uVar8);
      uVar11 = iVar2 * (uVar3 & 0xffff);
      FUN_080001b0(uVar6,uVar8);
      uVar6 = extraout_r1_05 << 0x10 | uVar12 >> 0x10;
      iVar7 = iVar2;
      if (uVar6 <= uVar11 && uVar11 - uVar6 != 0) {
        uVar6 = uVar6 + uVar3;
        iVar7 = iVar2 + -1;
        if ((uVar3 <= uVar6) && (uVar6 <= uVar11 && uVar11 - uVar6 != 0)) {
          iVar7 = iVar2 + -2;
          uVar6 = uVar6 + uVar3;
        }
      }
      uVar5 = FUN_08000128(uVar6 - uVar11,uVar8);
      uVar9 = uVar5 * (uVar3 & 0xffff);
      FUN_080001b0(uVar6 - uVar11,uVar8);
      uVar11 = extraout_r1_06 << 0x10 | uVar12 & 0xffff;
      uVar6 = uVar5;
      if (uVar11 <= uVar9 && uVar9 - uVar11 != 0) {
        uVar11 = uVar11 + uVar3;
        uVar6 = uVar5 - 1;
        if ((uVar3 <= uVar11) && (uVar11 <= uVar9 && uVar9 - uVar11 != 0)) {
          uVar6 = uVar5 - 2;
          uVar11 = uVar11 + uVar3;
        }
      }
      uVar12 = iVar7 << 0x10 | uVar6;
      uVar8 = param_3 & 0xffff;
      param_3 = param_3 >> 0x10;
      uVar3 = (uVar6 & 0xffff) * uVar8;
      uVar8 = (uVar12 >> 0x10) * uVar8;
      iVar7 = param_3 * (uVar12 >> 0x10);
      uVar6 = uVar8 + param_3 * (uVar6 & 0xffff) + (uVar3 >> 0x10);
      if (uVar6 <= uVar8 && uVar8 - uVar6 != 0) {
        iVar7 = iVar7 + 0x10000;
      }
      uVar8 = iVar7 + (uVar6 >> 0x10);
      if ((uVar11 - uVar9 < uVar8) ||
         ((uVar11 - uVar9 == uVar8 &&
          (param_1 << (uVar1 & 0xff) < uVar6 * 0x10000 + (uVar3 & 0xffff))))) {
        uVar12 = uVar12 - 1;
        uVar1 = 0;
      }
      else {
        uVar1 = 0;
      }
      goto LAB_08002296;
    }
    if ((param_4 < param_2) || (param_3 <= param_1)) {
      uVar1 = 0;
      uVar12 = 1;
      goto LAB_08002296;
    }
  }
  uVar1 = 0;
  uVar12 = 0;
LAB_08002296:
  if (uVar10 != 0) {
    bVar13 = uVar12 != 0;
    uVar12 = -uVar12;
    uVar1 = -(uint)bVar13 - uVar1;
  }
  return CONCAT44(uVar1,uVar12);
}


