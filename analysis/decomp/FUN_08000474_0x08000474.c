/* ==== FUN_08000474 @ 0x8000474 ==== */

uint FUN_08000474(uint param_1,uint param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  
  iVar1 = -((int)param_1 >> 0x1f);
  uVar7 = param_1 & 0x7fffff;
  uVar4 = uVar7 * 8;
  uVar3 = (param_1 & 0x7fffffff) >> 0x17;
  uVar5 = (param_2 & 0x7fffffff) >> 0x17;
  iVar2 = -((int)param_2 >> 0x1f);
  uVar8 = param_2 & 0x7fffff;
  uVar9 = uVar8 * 8;
  if ((int)param_2 >> 0x1f == (int)param_1 >> 0x1f) {
    uVar6 = uVar3 - uVar5;
    if (0 < (int)uVar6) {
      if (uVar5 == 0) {
        if (uVar8 != 0) {
          uVar6 = uVar6 - 1;
          if (uVar6 == 0) {
            uVar9 = uVar9 + uVar4;
            uVar5 = uVar3;
            goto LAB_0800055c;
          }
          if (uVar3 != 0xff) goto LAB_08000550;
          goto LAB_08000600;
        }
        uVar8 = uVar3 + 1;
        uVar9 = uVar4;
        uVar5 = uVar3;
      }
      else if (uVar3 == 0xff) {
LAB_08000600:
        uVar8 = 0x100;
        uVar9 = uVar4;
        uVar5 = 0xff;
      }
      else {
        uVar9 = uVar9 | 0x4000000;
LAB_08000550:
        uVar7 = 1;
        if ((int)uVar6 < 0x1c) {
          uVar7 = (uint)(uVar9 << (0x20 - uVar6 & 0xff) != 0) | uVar9 >> (uVar6 & 0xff);
        }
        uVar9 = uVar7 + uVar4;
        uVar5 = uVar3;
LAB_0800055c:
        if ((int)(uVar9 << 5) < 0) {
          if (uVar5 + 1 == 0xff) {
            uVar8 = 0x100;
            uVar5 = 0xff;
            uVar9 = 4;
            goto LAB_080004f2;
          }
          uVar8 = uVar5 + 2;
          uVar9 = (uVar9 & 0xfbffffff) >> 1 | uVar9 & 1;
          uVar5 = uVar5 + 1;
        }
        else {
          uVar8 = uVar5 + 1;
        }
      }
      goto LAB_080004e4;
    }
    if (uVar6 != 0) {
      uVar6 = -uVar6;
      if (uVar3 == 0) {
        if (uVar7 == 0) {
          uVar8 = uVar5 + 1;
          goto LAB_080004e4;
        }
        uVar6 = uVar6 - 1;
        if (uVar6 == 0) {
          uVar9 = uVar9 + uVar4;
          goto LAB_0800055c;
        }
        if (uVar5 != 0xff) goto LAB_08000740;
      }
      else if (uVar5 != 0xff) {
        uVar4 = uVar4 | 0x4000000;
LAB_08000740:
        uVar7 = 1;
        if ((int)uVar6 < 0x1c) {
          uVar7 = (uint)(uVar4 << (0x20 - uVar6 & 0xff) != 0) | uVar4 >> (uVar6 & 0xff);
        }
        uVar9 = uVar7 + uVar9;
        goto LAB_0800055c;
      }
      uVar8 = 0x100;
      uVar5 = 0xff;
      goto LAB_080004e4;
    }
    uVar5 = uVar3 + 1;
    if ((uVar5 & 0xff) < 2) {
      if (uVar3 == 0) {
        if (uVar7 == 0) {
          uVar8 = 1;
          uVar5 = 0;
        }
        else if (uVar8 == 0) {
          uVar8 = 1;
          uVar5 = 0;
          uVar9 = uVar4;
        }
        else {
          uVar9 = uVar9 + uVar4;
          if ((int)(uVar9 * 0x20) < 0) {
            uVar8 = 2;
            uVar9 = uVar9 & 0xfbffffff;
            uVar5 = 1;
          }
          else {
            uVar8 = 1;
            uVar5 = 0;
          }
        }
      }
      else if (uVar7 == 0) {
        uVar8 = 0x100;
        uVar5 = 0xff;
      }
      else {
        if (uVar8 == 0) goto LAB_08000578;
        uVar9 = param_1 & 0x7fffff;
        if (((param_1 & 0x400000) != 0) && (uVar9 = param_2 & 0x7fffff, (param_2 & 0x400000) != 0))
        {
          uVar9 = param_1 & 0x7fffff;
        }
        uVar9 = uVar9 << 3;
        uVar8 = 0x100;
        uVar5 = 0xff;
      }
      goto LAB_080004e4;
    }
    if (uVar5 != 0xff) {
      uVar8 = uVar3 + 2;
      uVar9 = uVar9 + uVar4 >> 1;
      goto LAB_080004e4;
    }
    uVar8 = 0x100;
    uVar9 = 4;
  }
  else {
    uVar6 = uVar3 - uVar5;
    if ((int)uVar6 < 1) {
      if (uVar6 == 0) {
        if ((uVar3 + 1 & 0xff) < 2) {
          if (uVar3 == 0) {
            if (uVar7 == 0) {
              if (uVar8 == 0) {
LAB_08000662:
                iVar1 = 0;
                uVar8 = 1;
                uVar9 = 4;
                uVar5 = 0;
                goto LAB_080004f2;
              }
              uVar8 = 1;
              uVar5 = 0;
              iVar1 = iVar2;
            }
            else if (uVar8 == 0) {
              uVar8 = 1;
              uVar9 = uVar4;
              uVar5 = 0;
            }
            else {
              uVar4 = uVar4 + uVar8 * -8;
              if (-1 < (int)(uVar4 * 0x20)) {
                if (uVar4 == 0) goto LAB_08000662;
                goto LAB_080004e0;
              }
              uVar8 = 1;
              uVar9 = uVar9 + uVar7 * -8;
              uVar5 = 0;
              iVar1 = iVar2;
            }
          }
          else {
            if (uVar7 == 0) {
              if (uVar8 == 0) {
                iVar1 = 0;
                uVar8 = 0x100;
                uVar9 = 0x3fffffc;
                uVar5 = 0xff;
                goto LAB_080004f2;
              }
              goto LAB_08000712;
            }
            if (uVar8 == 0) goto LAB_08000578;
            uVar7 = param_1 & 0x7fffff;
            if (((param_1 & 0x400000) != 0) && ((param_2 & 0x400000) == 0)) {
              uVar7 = param_2 & 0x7fffff;
              iVar1 = iVar2;
            }
            uVar8 = 0x100;
            uVar9 = uVar7 << 3;
            uVar5 = 0xff;
          }
        }
        else {
          uVar4 = uVar4 + uVar8 * -8;
          if ((int)(uVar4 * 0x20) < 0) {
            uVar4 = uVar9 + uVar7 * -8;
            iVar1 = iVar2;
          }
          else if (uVar4 == 0) goto LAB_08000662;
LAB_080004b8:
          iVar2 = FUN_0800216c(uVar4);
          uVar7 = iVar2 - 5;
          uVar4 = uVar4 << (uVar7 & 0xff);
          if ((int)uVar7 < (int)uVar3) {
            uVar8 = (uVar3 - uVar7) + 1;
            uVar9 = uVar4 & 0xfbffffff;
            uVar5 = uVar3 - uVar7;
          }
          else {
            uVar4 = (uint)(uVar4 << (0x1f - (uVar7 - uVar3) & 0xff) != 0) |
                    uVar4 >> ((uVar7 - uVar3) + 1 & 0xff);
LAB_080004e0:
            uVar8 = 1;
            uVar9 = uVar4;
            uVar5 = 0;
          }
        }
      }
      else {
        uVar6 = -uVar6;
        if (uVar3 == 0) {
          if (uVar7 == 0) {
            uVar8 = uVar5 + 1;
            iVar1 = iVar2;
            goto LAB_080004e4;
          }
          uVar6 = uVar6 - 1;
          if (uVar6 == 0) {
            uVar9 = uVar9 + uVar7 * -8;
            uVar3 = uVar5;
            iVar1 = iVar2;
            goto LAB_080004b0;
          }
          if (uVar5 != 0xff) goto LAB_0800063a;
        }
        else if (uVar5 != 0xff) {
          uVar4 = uVar4 | 0x4000000;
LAB_0800063a:
          uVar7 = 1;
          if ((int)uVar6 < 0x1c) {
            uVar7 = (uint)(uVar4 << (0x20 - uVar6 & 0xff) != 0) | uVar4 >> (uVar6 & 0xff);
          }
          uVar9 = uVar9 - uVar7;
          uVar3 = uVar5;
          iVar1 = iVar2;
LAB_080004b0:
          if (-1 < (int)(uVar9 << 5)) goto LAB_08000538;
LAB_080004b4:
          uVar4 = uVar9 & 0x3ffffff;
          goto LAB_080004b8;
        }
LAB_08000712:
        uVar8 = 0x100;
        uVar5 = 0xff;
        iVar1 = iVar2;
      }
    }
    else if (uVar5 == 0) {
      if (uVar8 == 0) {
        uVar8 = uVar3 + 1;
        uVar9 = uVar4;
        uVar5 = uVar3;
      }
      else {
        uVar6 = uVar6 - 1;
        if (uVar6 == 0) {
          uVar9 = uVar4 + uVar8 * -8;
          goto LAB_080004b0;
        }
        if (uVar3 == 0xff) goto LAB_08000600;
LAB_08000526:
        if ((int)uVar6 < 0x1c) {
          iVar2 = -((uint)(uVar9 << (0x20 - uVar6 & 0xff) != 0) | uVar9 >> (uVar6 & 0xff));
        }
        else {
          iVar2 = -1;
        }
        uVar9 = uVar4 + iVar2;
        if ((int)(uVar9 * 0x20) < 0) goto LAB_080004b4;
LAB_08000538:
        uVar8 = uVar3 + 1;
        uVar5 = uVar3;
      }
    }
    else {
      if (uVar3 != 0xff) {
        uVar9 = uVar9 | 0x4000000;
        goto LAB_08000526;
      }
LAB_08000578:
      uVar8 = 0x100;
      uVar9 = uVar4;
      uVar5 = 0xff;
    }
LAB_080004e4:
    if ((uVar9 & 0xf) != 4) {
      uVar9 = uVar9 + 4;
    }
  }
LAB_080004f2:
  uVar7 = uVar8;
  if ((int)(uVar9 << 5) < 0) {
    if (uVar8 == 0xff) {
      uVar5 = 0xff;
      uVar7 = 0;
      goto LAB_08000510;
    }
    uVar9 = uVar9 & 0xfbffffff;
    uVar7 = uVar8 + 1;
    uVar5 = uVar8;
  }
  uVar9 = uVar9 >> 3;
  if ((uVar7 & 0xff) < 2) {
    uVar7 = 0;
    if (uVar9 != 0) {
      if (uVar5 == 0) {
        uVar7 = uVar9 & 0x7fffff;
        uVar5 = 0;
        goto LAB_08000510;
      }
      uVar9 = uVar9 | 0x400000;
      goto LAB_0800050a;
    }
  }
  else {
LAB_0800050a:
    uVar7 = uVar9 & 0x7fffff;
  }
  uVar5 = uVar5 & 0xff;
LAB_08000510:
  return uVar7 | uVar5 << 0x17 | iVar1 << 0x1f;
}


