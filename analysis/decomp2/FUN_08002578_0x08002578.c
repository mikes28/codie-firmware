/* ==== FUN_08002578 @ 0x8002578 ==== */

undefined8 FUN_08002578(uint param_1,uint param_2,uint param_3,uint param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int extraout_r1;
  int extraout_r1_00;
  int extraout_r1_01;
  int extraout_r1_02;
  int extraout_r1_03;
  int extraout_r1_04;
  int extraout_r1_05;
  int extraout_r1_06;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  
  if (param_4 == 0) {
    if (param_2 < param_3) {
      uVar1 = FUN_0800216c(param_3);
      if (uVar1 != 0) {
        param_2 = param_1 >> (0x20 - uVar1 & 0xff) | param_2 << (uVar1 & 0xff);
        param_3 = param_3 << (uVar1 & 0xff);
        param_1 = param_1 << (uVar1 & 0xff);
      }
      uVar9 = param_3 >> 0x10;
      iVar2 = FUN_08000128(param_2,uVar9);
      uVar10 = iVar2 * (param_3 & 0xffff);
      FUN_080001b0(param_2,uVar9);
      uVar1 = extraout_r1 << 0x10 | param_1 >> 0x10;
      iVar5 = iVar2;
      if (uVar1 <= uVar10 && uVar10 - uVar1 != 0) {
        uVar1 = uVar1 + param_3;
        iVar5 = iVar2 + -1;
        if ((param_3 <= uVar1) && (uVar1 <= uVar10 && uVar10 - uVar1 != 0)) {
          iVar5 = iVar2 + -2;
          uVar1 = uVar1 + param_3;
        }
      }
      uVar3 = FUN_08000128(uVar1 - uVar10,uVar9);
      uVar6 = uVar3 * (param_3 & 0xffff);
      FUN_080001b0(uVar1 - uVar10,uVar9);
      uVar1 = param_1 & 0xffff | extraout_r1_00 << 0x10;
      uVar9 = uVar3;
      if (uVar1 <= uVar6 && uVar6 - uVar1 != 0) {
        uVar1 = uVar1 + param_3;
        if ((uVar1 < param_3) || (uVar9 = uVar3 - 2, uVar6 < uVar1 || uVar6 - uVar1 == 0)) {
          uVar9 = uVar3 - 1;
        }
      }
      uVar9 = iVar5 << 0x10 | uVar9;
      uVar1 = 0;
    }
    else {
      if (param_3 == 0) {
        param_3 = FUN_08000128(1,0);
      }
      uVar1 = FUN_0800216c(param_3);
      if (uVar1 == 0) {
        iVar5 = param_2 - param_3;
        uVar10 = param_3 & 0xffff;
        uVar9 = param_3 >> 0x10;
        uVar1 = 1;
      }
      else {
        param_3 = param_3 << (uVar1 & 0xff);
        uVar6 = param_2 >> (0x20 - uVar1 & 0xff);
        uVar3 = param_1 >> (0x20 - uVar1 & 0xff) | param_2 << (uVar1 & 0xff);
        uVar9 = param_3 >> 0x10;
        param_1 = param_1 << (uVar1 & 0xff);
        iVar5 = FUN_08000128(uVar6,uVar9);
        uVar10 = param_3 & 0xffff;
        uVar1 = uVar10 * iVar5;
        FUN_080001b0(uVar6,uVar9);
        uVar6 = extraout_r1_05 << 0x10 | uVar3 >> 0x10;
        iVar2 = iVar5;
        if (uVar6 <= uVar1 && uVar1 - uVar6 != 0) {
          uVar6 = uVar6 + param_3;
          iVar2 = iVar5 + -1;
          if ((param_3 <= uVar6) && (uVar6 <= uVar1 && uVar1 - uVar6 != 0)) {
            iVar2 = iVar5 + -2;
            uVar6 = uVar6 + param_3;
          }
        }
        uVar4 = FUN_08000128(uVar6 - uVar1,uVar9);
        uVar8 = uVar10 * uVar4;
        FUN_080001b0(uVar6 - uVar1,uVar9);
        uVar3 = extraout_r1_06 << 0x10 | uVar3 & 0xffff;
        uVar1 = uVar4;
        if (uVar3 <= uVar8 && uVar8 - uVar3 != 0) {
          uVar3 = uVar3 + param_3;
          uVar1 = uVar4 - 1;
          if ((param_3 <= uVar3) && (uVar3 <= uVar8 && uVar8 - uVar3 != 0)) {
            uVar1 = uVar4 - 2;
            uVar3 = uVar3 + param_3;
          }
        }
        iVar5 = uVar3 - uVar8;
        uVar1 = uVar1 | iVar2 << 0x10;
      }
      iVar2 = FUN_08000128(iVar5,uVar9);
      uVar3 = uVar10 * iVar2;
      FUN_080001b0(iVar5,uVar9);
      uVar6 = extraout_r1_01 << 0x10 | param_1 >> 0x10;
      iVar5 = iVar2;
      if (uVar6 <= uVar3 && uVar3 - uVar6 != 0) {
        uVar6 = uVar6 + param_3;
        iVar5 = iVar2 + -1;
        if ((param_3 <= uVar6) && (uVar6 <= uVar3 && uVar3 - uVar6 != 0)) {
          iVar5 = iVar2 + -2;
          uVar6 = uVar6 + param_3;
        }
      }
      uVar4 = FUN_08000128(uVar6 - uVar3,uVar9);
      uVar10 = uVar4 * uVar10;
      FUN_080001b0(uVar6 - uVar3,uVar9);
      uVar3 = param_1 & 0xffff | extraout_r1_02 << 0x10;
      uVar9 = uVar4;
      if (uVar3 <= uVar10 && uVar10 - uVar3 != 0) {
        uVar3 = uVar3 + param_3;
        uVar9 = uVar4 - 1;
        if ((param_3 <= uVar3) && (uVar3 <= uVar10 && uVar10 - uVar3 != 0)) {
          uVar9 = uVar4 - 2;
        }
      }
      uVar9 = iVar5 << 0x10 | uVar9;
    }
    goto LAB_08002664;
  }
  if (param_4 <= param_2) {
    uVar1 = FUN_0800216c(param_4);
    if (uVar1 != 0) {
      uVar9 = 0x20 - uVar1;
      uVar6 = param_4 << (uVar1 & 0xff) | param_3 >> (uVar9 & 0xff);
      uVar10 = param_2 >> (uVar9 & 0xff);
      param_3 = param_3 << (uVar1 & 0xff);
      uVar9 = param_1 >> (uVar9 & 0xff) | param_2 << (uVar1 & 0xff);
      uVar4 = uVar6 >> 0x10;
      iVar2 = FUN_08000128(uVar10,uVar4);
      uVar3 = iVar2 * (uVar6 & 0xffff);
      FUN_080001b0(uVar10,uVar4);
      uVar10 = extraout_r1_03 << 0x10 | uVar9 >> 0x10;
      iVar5 = iVar2;
      if (uVar10 <= uVar3 && uVar3 - uVar10 != 0) {
        uVar10 = uVar10 + uVar6;
        iVar5 = iVar2 + -1;
        if ((uVar6 <= uVar10) && (uVar10 <= uVar3 && uVar3 - uVar10 != 0)) {
          iVar5 = iVar2 + -2;
          uVar10 = uVar10 + uVar6;
        }
      }
      uVar8 = FUN_08000128(uVar10 - uVar3,uVar4);
      uVar7 = uVar8 * (uVar6 & 0xffff);
      FUN_080001b0(uVar10 - uVar3,uVar4);
      uVar3 = extraout_r1_04 << 0x10 | uVar9 & 0xffff;
      uVar10 = uVar8;
      if (uVar3 <= uVar7 && uVar7 - uVar3 != 0) {
        uVar3 = uVar3 + uVar6;
        uVar10 = uVar8 - 1;
        if ((uVar6 <= uVar3) && (uVar3 <= uVar7 && uVar7 - uVar3 != 0)) {
          uVar10 = uVar8 - 2;
          uVar3 = uVar3 + uVar6;
        }
      }
      uVar9 = iVar5 << 0x10 | uVar10;
      uVar4 = param_3 & 0xffff;
      param_3 = param_3 >> 0x10;
      uVar6 = (uVar10 & 0xffff) * uVar4;
      uVar4 = (uVar9 >> 0x10) * uVar4;
      iVar5 = param_3 * (uVar9 >> 0x10);
      uVar10 = uVar4 + param_3 * (uVar10 & 0xffff) + (uVar6 >> 0x10);
      if (uVar10 <= uVar4 && uVar4 - uVar10 != 0) {
        iVar5 = iVar5 + 0x10000;
      }
      uVar4 = iVar5 + (uVar10 >> 0x10);
      if ((uVar3 - uVar7 < uVar4) ||
         ((uVar3 - uVar7 == uVar4 &&
          (param_1 << (uVar1 & 0xff) < uVar10 * 0x10000 + (uVar6 & 0xffff))))) {
        uVar9 = uVar9 - 1;
        uVar1 = 0;
      }
      else {
        uVar1 = 0;
      }
      goto LAB_08002664;
    }
    if ((param_4 < param_2) || (param_3 <= param_1)) {
      uVar1 = 0;
      uVar9 = 1;
      goto LAB_08002664;
    }
  }
  uVar1 = 0;
  uVar9 = 0;
LAB_08002664:
  return CONCAT44(uVar1,uVar9);
}


