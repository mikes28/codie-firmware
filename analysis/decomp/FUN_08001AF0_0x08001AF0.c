/* ==== FUN_08001AF0 @ 0x8001af0 ==== */

undefined8 FUN_08001AF0(uint param_1,uint param_2,uint param_3,uint param_4)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  uint local_3c;
  uint local_34;
  int local_30;
  
  uVar3 = param_2 & 0xfffff;
  uVar5 = (param_2 & 0x7fffffff) >> 0x14;
  uVar11 = param_2 >> 0x1f;
  if (uVar5 == 0) {
    if (uVar3 == 0 && param_1 == 0) {
      uVar10 = 4;
      uVar3 = 0;
      param_1 = 0;
      local_34 = 1;
    }
    else {
      if (uVar3 == 0) {
        iVar1 = FUN_0800216c();
        iVar1 = iVar1 + 0x20;
      }
      else {
        iVar1 = FUN_0800216c(uVar3);
      }
      if (iVar1 < 0x28) {
        uVar3 = param_1 >> (0x28U - iVar1 & 0xff) | uVar3 << (iVar1 - 8U & 0xff);
        param_1 = param_1 << (iVar1 - 8U & 0xff);
      }
      else {
        uVar3 = param_1 << (iVar1 - 0x28U & 0xff);
        param_1 = 0;
      }
      uVar5 = -iVar1 - 0x3f3;
      uVar10 = 0;
      local_34 = 0;
    }
  }
  else if (uVar5 == 0x7ff) {
    if (uVar3 == 0 && param_1 == 0) {
      uVar10 = 8;
      uVar3 = 0;
      param_1 = 0;
      local_34 = 2;
    }
    else {
      uVar10 = 0xc;
      local_34 = 3;
    }
  }
  else {
    uVar3 = (uVar3 | 0x100000) << 3 | param_1 >> 0x1d;
    param_1 = param_1 << 3;
    uVar5 = uVar5 - 0x3ff;
    uVar10 = 0;
    local_34 = 0;
  }
  uVar7 = param_4 & 0xfffff;
  uVar4 = (param_4 & 0x7fffffff) >> 0x14;
  if (uVar4 == 0) {
    if (uVar7 == 0 && param_3 == 0) {
      uVar7 = 0;
      param_3 = 0;
      uVar2 = 1;
      goto LAB_08001c12;
    }
    if (uVar7 == 0) {
      iVar1 = FUN_0800216c();
      iVar1 = iVar1 + 0x20;
    }
    else {
      iVar1 = FUN_0800216c(uVar7);
    }
    if (iVar1 < 0x28) {
      uVar7 = uVar7 << (iVar1 - 8U & 0xff) | param_3 >> (0x28U - iVar1 & 0xff);
      param_3 = param_3 << (iVar1 - 8U & 0xff);
    }
    else {
      uVar7 = param_3 << (iVar1 - 0x28U & 0xff);
      param_3 = 0;
    }
    uVar4 = -iVar1 - 0x3f3;
    local_3c = (param_4 ^ param_2) >> 0x1f;
    uVar2 = 0;
    if (uVar10 < 0x10) goto LAB_08001bd2;
switchD_08001bdc_caseD_8001c20:
    local_30 = uVar4 + uVar5;
    uVar5 = param_1 & 0xffff;
    uVar4 = param_3 & 0xffff;
    param_1 = param_1 >> 0x10;
    param_3 = param_3 >> 0x10;
    uVar10 = uVar4 * param_1;
    iVar1 = param_3 * param_1;
    uVar11 = param_3 * uVar5 + (uVar4 * uVar5 >> 0x10) + uVar10;
    if (uVar11 <= uVar10 && uVar10 - uVar11 != 0) {
      iVar1 = iVar1 + 0x10000;
    }
    uVar2 = uVar7 >> 0x10;
    uVar7 = uVar7 & 0xffff;
    iVar9 = uVar2 * param_1;
    param_1 = uVar7 * param_1;
    uVar10 = uVar2 * uVar5 + (uVar7 * uVar5 >> 0x10) + param_1;
    if (uVar10 <= param_1 && param_1 - uVar10 != 0) {
      iVar9 = iVar9 + 0x10000;
    }
    uVar13 = iVar9 + (uVar10 >> 0x10);
    uVar6 = uVar10 * 0x10000 + (uVar7 * uVar5 & 0xffff);
    uVar10 = uVar3 & 0xffff;
    uVar3 = uVar3 >> 0x10;
    uVar12 = uVar3 * uVar4;
    uVar14 = param_3 * uVar10 + (uVar4 * uVar10 >> 0x10) + uVar12;
    iVar9 = uVar3 * param_3;
    if (uVar14 <= uVar12 && uVar12 - uVar14 != 0) {
      iVar9 = iVar9 + 0x10000;
    }
    uVar8 = iVar9 + (uVar14 >> 0x10);
    uVar12 = uVar3 * uVar7;
    iVar9 = uVar3 * uVar2;
    uVar3 = uVar2 * uVar10 + (uVar7 * uVar10 >> 0x10) + uVar12;
    uVar2 = uVar14 * 0x10000 + (uVar4 * uVar10 & 0xffff);
    if (uVar3 <= uVar12 && uVar12 - uVar3 != 0) {
      iVar9 = iVar9 + 0x10000;
    }
    uVar12 = (uVar11 >> 0x10) + uVar6 + iVar1;
    uVar6 = (uint)(uVar12 < uVar6);
    uVar7 = uVar13 + (uVar7 * uVar10 & 0xffff) + uVar3 * 0x10000;
    uVar15 = uVar7 + uVar6;
    uVar12 = uVar12 + uVar2;
    uVar10 = (uint)(uVar12 < uVar2);
    uVar14 = uVar8 + uVar15;
    uVar2 = uVar14 + uVar10;
    uVar11 = (uint)((uVar12 & 0x7fffff) != 0 || uVar11 * 0x10000 + (uVar4 * uVar5 & 0xffff) != 0);
    uVar7 = ((uint)(uVar7 < uVar13 || uVar15 < uVar6) + (uVar3 >> 0x10) +
             (uint)(uVar2 < uVar10 || uVar14 < uVar8) + iVar9) * 0x200 | uVar2 >> 0x17;
    param_1 = uVar11 | uVar12 >> 0x17 | uVar2 * 0x200;
    if ((int)(uVar7 << 7) < 0) {
      local_30 = local_30 + 1;
      param_1 = uVar11 | uVar12 >> 0x17 & 1 | param_1 >> 1 | (uVar2 >> 0x17) << 0x1f;
      uVar7 = uVar7 >> 1;
    }
    goto LAB_08001dda;
  }
  if (uVar4 == 0x7ff) {
    uVar2 = 3;
    if (uVar7 == 0 && param_3 == 0) {
      uVar7 = 0;
      param_3 = 0;
      uVar2 = 2;
    }
  }
  else {
    uVar7 = (uVar7 | 0x100000) << 3 | param_3 >> 0x1d;
    param_3 = param_3 << 3;
    uVar4 = uVar4 - 0x3ff;
    uVar2 = 0;
  }
LAB_08001c12:
  local_3c = (param_4 ^ param_2) >> 0x1f;
  uVar10 = uVar10 | uVar2;
  if (0xf < uVar10) goto switchD_08001bdc_caseD_8001c20;
LAB_08001bd2:
  switch((uint)(&switchD_08001bdc::switchdataD_0800b8c8)[uVar10] & 0xfffffffe) {
  case 0x8001c20:
    goto switchD_08001bdc_caseD_8001c20;
  case 0x8001dc8:
    uVar11 = local_3c;
    uVar2 = local_34;
    uVar7 = uVar3;
    break;
  case 0x8001e50:
    uVar11 = param_4 >> 0x1f;
    param_1 = param_3;
    break;
  case 0x8001e68:
    uVar2 = local_34;
    uVar7 = uVar3;
    break;
  case 0x8001e70:
    uVar11 = 0;
    uVar3 = 0xfffff;
    param_1 = 0xffffffff;
    uVar5 = 0x7ff;
    goto LAB_08001e24;
  case 0x8001e7c:
    uVar11 = local_3c;
    param_1 = param_3;
    break;
  case 0x8001e80:
    if ((uVar3 & 0x80000) == 0) {
      uVar3 = uVar3 & 0xfffff | 0x80000;
      uVar5 = 0x7ff;
    }
    else if ((uVar7 & 0x80000) == 0) {
      uVar3 = uVar7 & 0xfffff | 0x80000;
      uVar5 = 0x7ff;
      param_1 = param_3;
      uVar11 = param_4 >> 0x1f;
    }
    else {
      uVar3 = uVar3 & 0xfffff | 0x80000;
      uVar5 = 0x7ff;
    }
    goto LAB_08001e24;
  }
  local_3c = uVar11;
  if (uVar2 != 2) {
    if (uVar2 == 3) {
      uVar3 = uVar7 & 0xfffff | 0x80000;
      uVar5 = 0x7ff;
      goto LAB_08001e24;
    }
    if (uVar2 == 1) {
      uVar5 = 0;
      uVar3 = 0;
      param_1 = 0;
      goto LAB_08001e24;
    }
LAB_08001dda:
    uVar5 = local_30 + 0x3ff;
    uVar11 = local_3c;
    if ((int)uVar5 < 1) {
      uVar5 = -local_30 - 0x3fe;
      if ((int)uVar5 < 0x39) {
        if ((int)uVar5 < 0x20) {
          uVar3 = uVar7 << (local_30 + 0x41eU & 0xff) | param_1 >> (uVar5 & 0xff) |
                  (uint)(param_1 << (local_30 + 0x41eU & 0xff) != 0);
          uVar7 = uVar7 >> (uVar5 & 0xff);
        }
        else {
          if (uVar5 == 0x20) {
            iVar1 = 0;
          }
          else {
            iVar1 = uVar7 << (local_30 + 0x43eU & 0xff);
          }
          uVar3 = (uint)(param_1 != 0 || iVar1 != 0) | uVar7 >> (-local_30 - 0x41eU & 0xff);
          uVar7 = 0;
        }
        uVar10 = uVar3;
        if ((uVar3 & 0xf) != 4) {
          uVar10 = uVar3 + 4;
          uVar7 = uVar7 + (uVar10 < uVar3);
        }
        if ((int)(uVar7 << 8) < 0) {
          uVar5 = 1;
          uVar3 = 0;
          param_1 = 0;
        }
        else {
          uVar3 = (uVar7 & 0x7fffff) >> 3;
          uVar5 = 0;
          param_1 = uVar10 >> 3 | uVar7 << 0x1d;
        }
      }
      else {
        uVar5 = 0;
        uVar3 = 0;
        param_1 = 0;
      }
      goto LAB_08001e24;
    }
    uVar10 = param_1;
    if ((param_1 & 0xf) != 4) {
      uVar10 = param_1 + 4;
      uVar7 = uVar7 + (uVar10 < param_1);
    }
    if ((int)(uVar7 << 7) < 0) {
      uVar7 = uVar7 & 0xfeffffff;
      uVar5 = local_30 + 0x400;
    }
    if ((int)uVar5 < 0x7ff) {
      uVar3 = (uVar7 & 0x7fffff) >> 3;
      uVar5 = uVar5 & 0x7ff;
      param_1 = uVar10 >> 3 | uVar7 << 0x1d;
      goto LAB_08001e24;
    }
  }
  uVar5 = 0x7ff;
  uVar3 = 0;
  param_1 = 0;
  uVar11 = local_3c;
LAB_08001e24:
  return CONCAT44(uVar3 | uVar5 << 0x14 | uVar11 << 0x1f,param_1);
}


