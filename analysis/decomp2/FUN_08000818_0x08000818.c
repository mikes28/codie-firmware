/* ==== FUN_08000818 @ 0x8000818 ==== */

uint FUN_08000818(uint param_1,uint param_2)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  int unaff_r9;
  uint uVar11;
  bool bVar12;
  uint local_2c;
  
  uVar8 = param_1 & 0x7fffff;
  uVar6 = (param_1 & 0x7fffffff) >> 0x17;
  uVar10 = -((int)param_1 >> 0x1f);
  if (uVar6 == 0) {
    if (uVar8 == 0) {
      uVar3 = 4;
      uVar5 = 1;
    }
    else {
      iVar2 = FUN_0800216c(uVar8);
      uVar8 = uVar8 << (iVar2 - 5U & 0xff);
      uVar6 = -iVar2 - 0x76;
      uVar3 = 0;
      uVar5 = uVar3;
    }
  }
  else if (uVar6 == 0xff) {
    if (uVar8 == 0) {
      uVar3 = 8;
      uVar5 = 2;
    }
    else {
      uVar3 = 0xc;
      uVar5 = 3;
    }
  }
  else {
    uVar8 = (uVar8 | 0x800000) << 3;
    uVar6 = uVar6 - 0x7f;
    uVar3 = 0;
    uVar5 = 0;
  }
  uVar4 = param_2 >> 0x1f;
  uVar9 = param_2 & 0x7fffff;
  uVar1 = (param_2 & 0x7fffffff) >> 0x17;
  if (uVar1 != 0) {
    if (uVar1 == 0xff) {
      uVar11 = 3 - (uVar9 == 0);
      goto LAB_08000874;
    }
    uVar11 = 0;
    uVar9 = (uVar9 | 0x800000) << 3;
    uVar1 = uVar1 - 0x7f;
    if (uVar3 < 0x10) goto LAB_08000926;
switchD_0800092c_caseD_8000884:
    local_2c = uVar4 ^ uVar10;
    unaff_r9 = uVar6 - uVar1;
    uVar8 = uVar8 * 0x20;
    uVar6 = uVar9 * 0x20;
    bVar12 = uVar8 < uVar6;
    if (bVar12) {
      unaff_r9 = unaff_r9 + -1;
      iVar2 = 0x1b;
    }
    else {
      uVar8 = uVar8 + uVar9 * -0x20;
      iVar2 = 0x1a;
    }
    uVar5 = uVar8 * 2;
    uVar10 = (uint)!bVar12 << 1;
    if (((int)uVar8 < 0) || (uVar6 < uVar5 || uVar6 + uVar8 * -2 == 0)) {
      uVar5 = uVar5 + uVar9 * -0x20;
      uVar10 = uVar10 | 1;
    }
    iVar7 = iVar2 + -1;
    if (0 < iVar7) {
      uVar8 = uVar5;
      if ((iVar2 - 1U & 1) == 0) goto LAB_080008da;
      while( true ) {
        uVar5 = uVar8 * 2;
        uVar10 = uVar10 << 1;
        if (((int)uVar8 < 0) || (uVar6 < uVar5 || uVar6 + uVar8 * -2 == 0)) {
          uVar5 = uVar5 + uVar9 * -0x20;
          uVar10 = uVar10 | 1;
        }
        iVar7 = iVar7 + -1;
        if (iVar7 < 1) break;
LAB_080008da:
        uVar8 = uVar5 * 2;
        uVar10 = uVar10 << 1;
        if (((int)uVar5 < 0) || (uVar6 < uVar8 || uVar6 + uVar5 * -2 == 0)) {
          uVar8 = uVar8 + uVar9 * -0x20;
          uVar10 = uVar10 | 1;
        }
        iVar7 = iVar7 + -1;
      }
    }
    uVar9 = uVar5 != 0 | uVar10;
    goto LAB_080009aa;
  }
  uVar11 = 1;
  if (uVar9 != 0) {
    iVar2 = FUN_0800216c(uVar9);
    uVar9 = uVar9 << (iVar2 - 5U & 0xff);
    uVar11 = 0;
    uVar1 = -iVar2 - 0x76;
  }
LAB_08000874:
  uVar3 = uVar11 | uVar3;
  if (0xf < uVar3) goto switchD_0800092c_caseD_8000884;
LAB_08000926:
  local_2c = uVar4 ^ uVar10;
  switch((uint)(&switchD_0800092c::switchdataD_0800b848)[uVar3] & 0xfffffffe) {
  case 0x8000884:
    goto switchD_0800092c_caseD_8000884;
  case 0x800092e:
    goto switchD_0800092c_caseD_800092e;
  case 0x800098e:
    local_2c = 0;
    uVar6 = 0x7fffff;
    uVar8 = 0xff;
    break;
  case 0x8000996:
    local_2c = uVar4;
    goto LAB_0800099e;
  case 0x80009d6:
    uVar8 = 0;
    uVar6 = 0;
    break;
  case 0x80009dc:
    if (((uVar8 & 0x400000) == 0) || ((uVar9 & 0x400000) != 0)) {
      uVar6 = uVar8 & 0x7fffff | 0x400000;
      uVar8 = 0xff;
      local_2c = uVar10;
    }
    else {
      uVar6 = uVar9 & 0x7fffff | 0x400000;
      uVar8 = 0xff;
      local_2c = uVar4;
    }
    break;
  case 0x80009f4:
    uVar11 = uVar5;
    uVar9 = uVar8;
    local_2c = uVar10;
LAB_0800099e:
    if (uVar11 != 2) {
      if (uVar11 == 3) {
        uVar6 = uVar9 & 0x7fffff | 0x400000;
        uVar8 = 0xff;
        break;
      }
      if (uVar11 == 1) {
        uVar8 = 0;
        local_2c = local_2c & 1;
        uVar6 = 0;
        break;
      }
LAB_080009aa:
      uVar8 = unaff_r9 + 0x7f;
      if ((int)uVar8 < 1) {
        if ((int)(-unaff_r9 - 0x7eU) < 0x1c) {
          uVar8 = (uint)(uVar9 << (unaff_r9 + 0x9eU & 0xff) != 0);
          uVar9 = uVar9 >> (-unaff_r9 - 0x7eU & 0xff);
          uVar6 = uVar8 | uVar9;
          if ((uVar8 | uVar9 & 0xf) != 4) {
            uVar6 = uVar6 + 4;
          }
          if ((int)(uVar6 << 5) < 0) {
            uVar8 = 1;
            uVar6 = 0;
          }
          else {
            uVar6 = (uVar6 & 0x3ffffff) >> 3;
            uVar8 = 0;
          }
        }
        else {
          uVar8 = 0;
          uVar6 = 0;
        }
        break;
      }
      if ((uVar9 & 0xf) != 4) {
        uVar9 = uVar9 + 4;
      }
      if ((int)(uVar9 << 4) < 0) {
        uVar9 = uVar9 & 0xf7ffffff;
        uVar8 = unaff_r9 + 0x80;
      }
      if ((int)uVar8 < 0xff) {
        uVar6 = (uVar9 & 0x3ffffff) >> 3;
        uVar8 = uVar8 & 0xff;
        break;
      }
    }
switchD_0800092c_caseD_800092e:
    uVar8 = 0xff;
    uVar6 = 0;
  }
  return uVar8 << 0x17 | uVar6 | local_2c << 0x1f;
}


