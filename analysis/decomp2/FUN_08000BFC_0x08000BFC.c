/* ==== FUN_08000BFC @ 0x8000bfc ==== */

uint FUN_08000BFC(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  int unaff_r9;
  uint local_2c;
  
  uVar6 = param_1 & 0x7fffff;
  uVar7 = (param_1 & 0x7fffffff) >> 0x17;
  uVar4 = param_1 >> 0x1f;
  if (uVar7 == 0) {
    if (uVar6 == 0) {
      uVar9 = 4;
      uVar3 = 1;
    }
    else {
      iVar5 = FUN_0800216c(uVar6);
      uVar6 = uVar6 << (iVar5 - 5U & 0xff);
      uVar7 = -iVar5 - 0x76;
      uVar9 = 0;
      uVar3 = uVar9;
    }
  }
  else if (uVar7 == 0xff) {
    if (uVar6 == 0) {
      uVar9 = 8;
      uVar3 = 2;
    }
    else {
      uVar9 = 0xc;
      uVar3 = 3;
    }
  }
  else {
    uVar6 = (uVar6 | 0x800000) << 3;
    uVar7 = uVar7 - 0x7f;
    uVar9 = 0;
    uVar3 = 0;
  }
  uVar8 = param_2 & 0x7fffff;
  uVar2 = (param_2 & 0x7fffffff) >> 0x17;
  if (uVar2 != 0) {
    if (uVar2 == 0xff) {
      uVar1 = 3 - (uVar8 == 0);
      goto LAB_08000c52;
    }
    uVar1 = 0;
    local_2c = (param_2 ^ param_1) >> 0x1f;
    uVar8 = (uVar8 | 0x800000) << 3;
    uVar2 = uVar2 - 0x7f;
    if (uVar9 < 0x10) goto LAB_08000ce0;
switchD_08000cea_caseD_8000c60:
    unaff_r9 = uVar7 + uVar2;
    uVar4 = (uVar8 & 0xffff) * (uVar6 & 0xffff);
    uVar7 = (uVar6 >> 0x10) * (uVar8 & 0xffff);
    uVar3 = (uVar8 >> 0x10) * (uVar6 & 0xffff) + (uVar4 >> 0x10) + uVar7;
    iVar5 = (uVar8 >> 0x10) * (uVar6 >> 0x10);
    if (uVar3 <= uVar7 && uVar7 - uVar3 != 0) {
      iVar5 = iVar5 + 0x10000;
    }
    uVar6 = uVar3 * 0x10000 + (uVar4 & 0xffff);
    uVar4 = (uint)((uVar6 & 0x3ffffff) != 0);
    uVar6 = uVar6 >> 0x1a;
    uVar8 = uVar4 | uVar6 | (iVar5 + (uVar3 >> 0x10)) * 0x40;
    if ((int)(uVar8 << 4) < 0) {
      unaff_r9 = unaff_r9 + 1;
      uVar8 = uVar8 >> 1 | uVar4 | uVar6 & 1;
    }
    goto LAB_08000d3e;
  }
  uVar1 = 1;
  if (uVar8 != 0) {
    iVar5 = FUN_0800216c(uVar8);
    uVar8 = uVar8 << (iVar5 - 5U & 0xff);
    uVar1 = 0;
    uVar2 = -iVar5 - 0x76;
  }
LAB_08000c52:
  local_2c = (param_2 ^ param_1) >> 0x1f;
  uVar9 = uVar9 | uVar1;
  if (0xf < uVar9) goto switchD_08000cea_caseD_8000c60;
LAB_08000ce0:
  switch((uint)(&switchD_08000cea::switchdataD_0800b888)[uVar9] & 0xfffffffe) {
  case 0x8000c60:
    goto switchD_08000cea_caseD_8000c60;
  case 0x8000d2c:
    uVar8 = uVar6;
    uVar1 = uVar3;
    break;
  case 0x8000d80:
    uVar4 = 0;
    uVar7 = 0x7fffff;
    uVar6 = 0xff;
    goto LAB_08000d6a;
  case 0x8000d88:
    uVar8 = uVar6;
    uVar1 = uVar3;
    uVar4 = local_2c;
    break;
  case 0x8000d9a:
    uVar4 = param_2 >> 0x1f;
    break;
  case 0x8000da2:
    uVar4 = local_2c;
    break;
  case 0x8000da6:
    if (((uVar6 & 0x400000) == 0) || ((uVar8 & 0x400000) != 0)) {
      uVar7 = uVar6 & 0x7fffff | 0x400000;
      uVar6 = 0xff;
    }
    else {
      uVar7 = uVar8 & 0x7fffff | 0x400000;
      uVar6 = 0xff;
      uVar4 = param_2 >> 0x1f;
    }
    goto LAB_08000d6a;
  }
  if (uVar1 == 2) {
    uVar6 = 0xff;
    uVar7 = 0;
  }
  else if (uVar1 == 3) {
    uVar7 = uVar8 & 0x7fffff | 0x400000;
    uVar6 = 0xff;
  }
  else {
    local_2c = uVar4;
    if (uVar1 == 1) {
      uVar7 = 0;
      uVar6 = 0;
    }
    else {
LAB_08000d3e:
      uVar6 = unaff_r9 + 0x7f;
      uVar4 = local_2c;
      if ((int)uVar6 < 1) {
        if ((int)(-unaff_r9 - 0x7eU) < 0x1c) {
          uVar7 = uVar8 >> (-unaff_r9 - 0x7eU & 0xff);
          uVar6 = (uint)(uVar8 << (unaff_r9 + 0x9eU & 0xff) != 0);
          uVar3 = uVar6 | uVar7;
          if ((uVar6 | uVar7 & 0xf) != 4) {
            uVar3 = uVar3 + 4;
          }
          if ((int)(uVar3 << 5) < 0) {
            uVar6 = 1;
            uVar7 = 0;
          }
          else {
            uVar7 = (uVar3 & 0x3ffffff) >> 3;
            uVar6 = 0;
          }
        }
        else {
          uVar6 = 0;
          uVar7 = 0;
        }
      }
      else {
        if ((uVar8 & 0xf) != 4) {
          uVar8 = uVar8 + 4;
        }
        if ((int)(uVar8 << 4) < 0) {
          uVar8 = uVar8 & 0xf7ffffff;
          uVar6 = unaff_r9 + 0x80;
        }
        if ((int)uVar6 < 0xff) {
          uVar7 = (uVar8 & 0x3ffffff) >> 3;
          uVar6 = uVar6 & 0xff;
        }
        else {
          uVar6 = 0xff;
          uVar7 = 0;
        }
      }
    }
  }
LAB_08000d6a:
  return uVar6 << 0x17 | uVar7 | uVar4 << 0x1f;
}


