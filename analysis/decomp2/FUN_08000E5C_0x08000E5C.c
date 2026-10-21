/* ==== FUN_08000E5C @ 0x8000e5c ==== */

uint FUN_08000E5C(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  
  uVar5 = -((int)param_1 >> 0x1f);
  uVar8 = param_1 & 0x7fffff;
  uVar1 = uVar8 * 8;
  uVar7 = (param_1 & 0x7fffffff) >> 0x17;
  uVar2 = (param_2 & 0x7fffffff) >> 0x17;
  uVar4 = param_2 >> 0x1f;
  uVar9 = param_2 & 0x7fffff;
  uVar10 = uVar9 * 8;
  if ((uVar2 != 0xff) || (uVar9 == 0)) {
    uVar4 = uVar4 ^ 1;
  }
  if (-uVar4 == (int)param_1 >> 0x1f) {
    uVar4 = uVar7 - uVar2;
    if (0 < (int)uVar4) {
      if (uVar2 == 0) {
        if (uVar9 == 0) goto LAB_08000e90;
        uVar4 = uVar4 - 1;
        if (uVar4 == 0) {
          uVar10 = uVar10 + uVar1;
          goto LAB_08000f12;
        }
        if (uVar7 != 0xff) goto LAB_08000f06;
      }
      else if (uVar7 != 0xff) {
        uVar10 = uVar10 | 0x4000000;
LAB_08000f06:
        uVar8 = 1;
        if ((int)uVar4 < 0x1c) {
          uVar8 = (uint)(uVar10 << (0x20 - uVar4 & 0xff) != 0) | uVar10 >> (uVar4 & 0xff);
        }
        uVar10 = uVar8 + uVar1;
LAB_08000f12:
        if ((int)(uVar10 << 5) < 0) {
          uVar2 = uVar7 + 1;
          if (uVar2 == 0xff) {
            uVar9 = 0x100;
            uVar2 = 0xff;
            uVar10 = 4;
            goto LAB_08000ea6;
          }
          uVar9 = uVar7 + 2;
          uVar10 = (uVar10 & 0xfbffffff) >> 1 | uVar10 & 1;
        }
        else {
          uVar9 = uVar7 + 1;
          uVar2 = uVar7;
        }
        goto LAB_08000e98;
      }
      goto LAB_08000f7a;
    }
    if (uVar4 != 0) {
      uVar4 = -uVar4;
      if (uVar7 == 0) {
        if (uVar8 == 0) {
          uVar9 = uVar2 + 1;
          goto LAB_08000e98;
        }
        uVar4 = uVar4 - 1;
        if (uVar4 == 0) {
          uVar10 = uVar10 + uVar1;
          uVar7 = uVar2;
          goto LAB_08000f12;
        }
        if (uVar2 != 0xff) goto LAB_08001140;
      }
      else if (uVar2 != 0xff) {
        uVar1 = uVar1 | 0x4000000;
LAB_08001140:
        uVar8 = 1;
        if ((int)uVar4 < 0x1c) {
          uVar8 = (uint)(uVar1 << (0x20 - uVar4 & 0xff) != 0) | uVar1 >> (uVar4 & 0xff);
        }
        uVar10 = uVar8 + uVar10;
        uVar7 = uVar2;
        goto LAB_08000f12;
      }
LAB_080011da:
      uVar9 = 0x100;
      uVar2 = 0xff;
      goto LAB_08000e98;
    }
    uVar2 = uVar7 + 1;
    if ((uVar2 & 0xff) < 2) {
      if (uVar7 == 0) {
        if (uVar8 == 0) {
          uVar9 = 1;
          uVar2 = 0;
        }
        else {
          if ((uVar9 == 0) || (uVar1 = uVar10 + uVar1, -1 < (int)(uVar1 * 0x20))) goto LAB_080011a2;
          uVar10 = uVar1 & 0xfbffffff;
          uVar9 = 2;
          uVar2 = 1;
        }
      }
      else {
        if (uVar8 == 0) goto LAB_080011da;
        if (uVar9 == 0) goto LAB_08000f7a;
        uVar10 = param_1 & 0x7fffff;
        if (((param_1 & 0x400000) != 0) && (uVar10 = param_2 & 0x7fffff, (param_2 & 0x400000) != 0))
        {
          uVar10 = param_1 & 0x7fffff;
        }
        uVar10 = uVar10 << 3;
        uVar9 = 0x100;
        uVar2 = 0xff;
      }
      goto LAB_08000e98;
    }
    if (uVar2 != 0xff) {
      uVar9 = uVar7 + 2;
      uVar10 = uVar10 + uVar1 >> 1;
      goto LAB_08000e98;
    }
    uVar9 = 0x100;
    uVar10 = 4;
  }
  else {
    uVar6 = uVar7 - uVar2;
    if ((int)uVar6 < 1) {
      if (uVar6 == 0) {
        if ((uVar7 + 1 & 0xff) < 2) {
          if (uVar7 == 0) {
            if (uVar8 == 0) {
              if (uVar9 == 0) {
LAB_08001060:
                uVar5 = 0;
                uVar9 = 1;
                uVar10 = 4;
                uVar2 = 0;
                goto LAB_08000ea6;
              }
              uVar9 = 1;
              uVar2 = 0;
              uVar5 = uVar4;
            }
            else {
              if (uVar9 != 0) {
                uVar1 = uVar1 + uVar9 * -8;
                if ((int)(uVar1 * 0x20) < 0) {
                  uVar9 = 1;
                  uVar10 = uVar10 + uVar8 * -8;
                  uVar2 = 0;
                  uVar5 = uVar4;
                  goto LAB_08000e98;
                }
                if (uVar1 == 0) goto LAB_08001060;
              }
LAB_080011a2:
              uVar9 = 1;
              uVar10 = uVar1;
              uVar2 = 0;
            }
          }
          else {
            if (uVar8 == 0) {
              if (uVar9 == 0) {
                uVar5 = 0;
                uVar9 = 0x100;
                uVar10 = 0x3fffffc;
                uVar2 = 0xff;
                goto LAB_08000ea6;
              }
              goto LAB_08001110;
            }
            if (uVar9 == 0) goto LAB_08000f7a;
            if (((param_1 & 0x400000) != 0) && ((param_2 & 0x400000) == 0)) {
              uVar5 = uVar4;
              param_1 = param_2;
            }
            uVar9 = 0x100;
            uVar10 = (param_1 & 0x7fffff) << 3;
            uVar2 = 0xff;
          }
        }
        else {
          uVar1 = uVar1 + uVar9 * -8;
          if ((int)(uVar1 * 0x20) < 0) {
            uVar1 = uVar10 + uVar8 * -8;
            uVar5 = uVar4;
          }
          else if (uVar1 == 0) goto LAB_08001060;
LAB_08000f38:
          iVar3 = FUN_0800216c(uVar1);
          uVar8 = iVar3 - 5;
          uVar1 = uVar1 << (uVar8 & 0xff);
          if ((int)uVar8 < (int)uVar7) {
            uVar9 = (uVar7 - uVar8) + 1;
            uVar10 = uVar1 & 0xfbffffff;
            uVar2 = uVar7 - uVar8;
          }
          else {
            uVar9 = 1;
            uVar10 = (uint)(uVar1 << (0x1f - (uVar8 - uVar7) & 0xff) != 0) |
                     uVar1 >> ((uVar8 - uVar7) + 1 & 0xff);
            uVar2 = 0;
          }
        }
      }
      else {
        uVar6 = -uVar6;
        if (uVar7 == 0) {
          if (uVar8 == 0) {
            uVar9 = uVar2 + 1;
            uVar5 = uVar4;
            goto LAB_08000e98;
          }
          uVar6 = uVar6 - 1;
          if (uVar6 == 0) {
            uVar10 = uVar10 + uVar8 * -8;
            uVar7 = uVar2;
            uVar5 = uVar4;
            goto LAB_08000f30;
          }
          if (uVar2 != 0xff) goto LAB_0800103a;
        }
        else if (uVar2 != 0xff) {
          uVar1 = uVar1 | 0x4000000;
LAB_0800103a:
          uVar8 = 1;
          if ((int)uVar6 < 0x1c) {
            uVar8 = (uint)(uVar1 << (0x20 - uVar6 & 0xff) != 0) | uVar1 >> (uVar6 & 0xff);
          }
          uVar10 = uVar10 - uVar8;
          uVar7 = uVar2;
          uVar5 = uVar4;
LAB_08000f30:
          if (-1 < (int)(uVar10 << 5)) goto LAB_08000eee;
LAB_08000f34:
          uVar1 = uVar10 & 0x3ffffff;
          goto LAB_08000f38;
        }
LAB_08001110:
        uVar9 = 0x100;
        uVar2 = 0xff;
        uVar5 = uVar4;
      }
      goto LAB_08000e98;
    }
    if (uVar2 == 0) {
      if (uVar9 == 0) {
LAB_08000e90:
        uVar9 = uVar7 + 1;
        uVar10 = uVar1;
        uVar2 = uVar7;
        goto LAB_08000e98;
      }
      uVar6 = uVar6 - 1;
      if (uVar6 == 0) {
        uVar10 = uVar1 + uVar9 * -8;
        goto LAB_08000f30;
      }
      if (uVar7 != 0xff) goto LAB_08000edc;
    }
    else if (uVar7 != 0xff) {
      uVar10 = uVar10 | 0x4000000;
LAB_08000edc:
      if ((int)uVar6 < 0x1c) {
        iVar3 = -((uint)(uVar10 << (0x20 - uVar6 & 0xff) != 0) | uVar10 >> (uVar6 & 0xff));
      }
      else {
        iVar3 = -1;
      }
      uVar10 = uVar1 + iVar3;
      if ((int)(uVar10 * 0x20) < 0) goto LAB_08000f34;
LAB_08000eee:
      uVar9 = uVar7 + 1;
      uVar2 = uVar7;
      goto LAB_08000e98;
    }
LAB_08000f7a:
    uVar9 = 0x100;
    uVar10 = uVar1;
    uVar2 = 0xff;
LAB_08000e98:
    if ((uVar10 & 0xf) != 4) {
      uVar10 = uVar10 + 4;
    }
  }
LAB_08000ea6:
  uVar8 = uVar9;
  if ((int)(uVar10 << 5) < 0) {
    if (uVar9 == 0xff) {
      uVar2 = 0xff;
      uVar8 = 0;
      goto LAB_08000ec6;
    }
    uVar10 = uVar10 & 0xfbffffff;
    uVar8 = uVar9 + 1;
    uVar2 = uVar9;
  }
  uVar10 = uVar10 >> 3;
  if ((uVar8 & 0xff) < 2) {
    uVar8 = 0;
    if (uVar10 != 0) {
      if (uVar2 == 0) {
        uVar8 = uVar10 & 0x7fffff;
        uVar2 = 0;
        goto LAB_08000ec6;
      }
      uVar10 = uVar10 | 0x400000;
      goto LAB_08000ec0;
    }
  }
  else {
LAB_08000ec0:
    uVar8 = uVar10 & 0x7fffff;
  }
  uVar2 = uVar2 & 0xff;
LAB_08000ec6:
  return uVar8 | uVar2 << 0x17 | uVar5 << 0x1f;
}


