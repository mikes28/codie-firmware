/* ==== FUN_08001398 @ 0x8001398 ==== */

undefined8 FUN_08001398(uint param_1,uint param_2,uint param_3,uint param_4)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  
  uVar13 = param_1 >> 0x1d;
  uVar9 = (param_2 & 0xfffff) << 3;
  uVar10 = uVar9 | uVar13;
  iVar5 = -((int)param_4 >> 0x1f);
  uVar6 = (param_4 & 0xfffff) << 3;
  uVar7 = param_3 >> 0x1d;
  uVar11 = uVar6 | uVar7;
  uVar12 = (param_2 & 0x7fffffff) >> 0x14;
  iVar3 = -((int)param_2 >> 0x1f);
  uVar14 = (param_4 & 0x7fffffff) >> 0x14;
  uVar4 = param_3 * 8;
  uVar1 = param_1 * 8;
  uVar8 = uVar12 - uVar14;
  if ((int)param_4 >> 0x1f == (int)param_2 >> 0x1f) {
    if ((int)uVar8 < 1) {
      if (uVar8 != 0) {
        uVar8 = -uVar8;
        if (uVar12 == 0) {
          if (uVar10 == 0 && uVar1 == 0) {
            uVar8 = uVar14 + 1;
            uVar10 = uVar11;
            uVar1 = uVar4;
            uVar12 = uVar14;
          }
          else {
            uVar8 = uVar8 - 1;
            if (uVar8 != 0) {
              if (uVar14 == 0x7ff) goto LAB_08001a7c;
              goto LAB_080018de;
            }
            uVar6 = uVar1 + uVar4;
            uVar11 = uVar10 + uVar11 + (uint)(uVar6 < uVar4);
            uVar12 = uVar14;
LAB_08001766:
            if ((int)(uVar11 << 8) < 0) {
              uVar4 = uVar12 + 1;
              if (uVar4 == 0x7ff) {
                uVar8 = 0x800;
                uVar10 = 0;
                uVar1 = 0;
                uVar12 = uVar4;
                goto LAB_080014c8;
              }
              uVar8 = uVar12 + 2;
              uVar10 = (uVar11 & 0xff7fffff) >> 1;
              uVar1 = uVar6 & 1 | uVar6 >> 1 | uVar11 << 0x1f;
              uVar12 = uVar4;
            }
            else {
              uVar8 = uVar12 + 1;
              uVar10 = uVar11;
              uVar1 = uVar6;
            }
          }
        }
        else {
          if (uVar14 != 0x7ff) {
            uVar10 = uVar10 | 0x800000;
LAB_080018de:
            if ((int)uVar8 < 0x39) {
              if ((int)uVar8 < 0x20) {
                uVar6 = uVar1 >> (uVar8 & 0xff) | uVar10 << (0x20 - uVar8 & 0xff) |
                        (uint)(uVar1 << (0x20 - uVar8 & 0xff) != 0);
                uVar8 = uVar10 >> (uVar8 & 0xff);
              }
              else {
                if (uVar8 == 0x20) {
                  iVar5 = 0;
                }
                else {
                  iVar5 = uVar10 << (0x40 - uVar8 & 0xff);
                }
                uVar6 = (uint)(iVar5 != 0 || uVar1 != 0) | uVar10 >> (uVar8 - 0x20 & 0xff);
                uVar8 = 0;
              }
            }
            else {
              uVar8 = 0;
              uVar6 = 0;
              if (uVar10 != 0 || uVar1 != 0) {
                uVar6 = 1;
              }
            }
            uVar6 = uVar6 + uVar4;
            uVar11 = uVar8 + uVar11 + (uint)(uVar6 < uVar4);
            uVar12 = uVar14;
            goto LAB_08001766;
          }
LAB_08001a7c:
          uVar8 = 0x800;
          uVar10 = uVar11;
          uVar1 = uVar4;
          uVar12 = uVar14;
        }
        goto LAB_080014b2;
      }
      uVar14 = uVar12 + 1;
      if ((uVar14 & 0x7ff) < 2) {
        if (uVar12 == 0) {
          if (uVar10 == 0 && uVar1 == 0) {
            uVar8 = 1;
            uVar10 = uVar11;
            uVar1 = uVar4;
            uVar12 = 0;
          }
          else if (uVar11 == 0 && uVar4 == 0) {
            uVar8 = 1;
            uVar12 = 0;
          }
          else {
            uVar4 = uVar4 + uVar1;
            uVar10 = uVar11 + uVar10 + (uint)(uVar4 < uVar1);
            uVar1 = uVar4;
            if ((int)(uVar10 * 0x100) < 0) {
              uVar8 = 2;
              uVar10 = uVar10 & 0xff7fffff;
              uVar12 = 1;
            }
            else {
              uVar8 = 1;
              uVar12 = 0;
            }
          }
        }
        else if (uVar10 == 0 && uVar1 == 0) {
          uVar8 = 0x800;
          uVar10 = uVar11;
          uVar1 = uVar4;
          uVar12 = 0x7ff;
        }
        else if (uVar11 == 0 && uVar4 == 0) {
LAB_080019e2:
          uVar8 = 0x800;
          uVar12 = 0x7ff;
        }
        else {
          uVar1 = param_1 & 0x1fffffff | uVar13 << 0x1d;
          uVar11 = param_2 & 0xfffff;
          if (((uVar9 & 0x400000) != 0) && ((uVar6 & 0x400000) == 0)) {
            uVar1 = uVar7 << 0x1d | param_3 & 0x1fffffff;
            uVar11 = param_4 & 0xfffff;
          }
          uVar8 = 0x800;
          uVar10 = uVar11 << 3 | uVar1 >> 0x1d;
          uVar1 = uVar1 << 3;
          uVar12 = 0x7ff;
        }
        goto LAB_080014b2;
      }
      if (uVar14 != 0x7ff) {
        uVar11 = uVar11 + uVar10 + (uint)(uVar4 + uVar1 < uVar1);
        uVar8 = uVar12 + 2;
        uVar10 = uVar11 >> 1;
        uVar1 = uVar11 * -0x80000000 | uVar4 + uVar1 >> 1;
        uVar12 = uVar14;
        goto LAB_080014b2;
      }
      uVar8 = 0x800;
      uVar10 = 0;
      uVar1 = 0;
      uVar12 = uVar14;
    }
    else {
      if (uVar14 == 0) {
        if (uVar11 == 0 && uVar4 == 0) {
          uVar8 = uVar12 + 1;
          goto LAB_080014b2;
        }
        uVar8 = uVar8 - 1;
        if (uVar8 == 0) {
          uVar6 = uVar4 + uVar1;
          goto LAB_08001758;
        }
        uVar7 = uVar11;
        if (uVar12 != 0x7ff) goto LAB_0800153c;
      }
      else if (uVar12 != 0x7ff) {
        uVar7 = uVar11 | 0x800000;
LAB_0800153c:
        if ((int)uVar8 < 0x39) {
          if ((int)uVar8 < 0x20) {
            uVar6 = uVar4 >> (uVar8 & 0xff) | uVar7 << (0x20 - uVar8 & 0xff) |
                    (uint)(uVar4 << (0x20 - uVar8 & 0xff) != 0);
            uVar11 = uVar7 >> (uVar8 & 0xff);
            goto LAB_0800156a;
          }
          if (uVar8 == 0x20) {
            iVar5 = 0;
          }
          else {
            iVar5 = uVar7 << (0x40 - uVar8 & 0xff);
          }
          uVar11 = 0;
          uVar6 = ((uint)(iVar5 != 0 || uVar4 != 0) | uVar7 >> (uVar8 - 0x20 & 0xff)) + uVar1;
        }
        else {
          uVar11 = 0;
          if (uVar7 == 0 && uVar4 == 0) {
            uVar6 = 0;
LAB_0800156a:
            uVar6 = uVar6 + uVar1;
          }
          else {
            uVar6 = uVar1 + 1;
          }
        }
LAB_08001758:
        uVar11 = uVar11 + uVar10 + (uint)(uVar6 < uVar1);
        goto LAB_08001766;
      }
LAB_08001680:
      uVar8 = 0x800;
LAB_080014b2:
      uVar11 = uVar1;
      if ((uVar1 & 0xf) != 4) goto LAB_080014ba;
    }
  }
  else {
    if ((int)uVar8 < 1) {
      if (uVar8 != 0) {
        uVar8 = -uVar8;
        if (uVar12 == 0) {
          if (uVar10 != 0 || uVar1 != 0) {
            uVar8 = uVar8 - 1;
            if (uVar8 != 0) {
              if (uVar14 == 0x7ff) goto LAB_08001996;
              goto LAB_080016f2;
            }
            uVar6 = uVar4 + param_1 * -8;
            uVar10 = (uVar11 - uVar10) - (uint)(uVar4 < uVar6);
            uVar12 = uVar14;
            iVar3 = iVar5;
            goto LAB_08001450;
          }
          uVar8 = uVar14 + 1;
          uVar10 = uVar11;
          uVar1 = uVar4;
          uVar12 = uVar14;
          iVar3 = iVar5;
          goto LAB_080014b2;
        }
        if (uVar14 == 0x7ff) {
LAB_08001996:
          uVar8 = 0x800;
          uVar10 = uVar11;
          uVar1 = uVar4;
          uVar12 = uVar14;
          iVar3 = iVar5;
          goto LAB_080014b2;
        }
        uVar10 = uVar10 | 0x800000;
LAB_080016f2:
        if ((int)uVar8 < 0x39) {
          if ((int)uVar8 < 0x20) {
            uVar6 = (uint)(uVar1 << (0x20 - uVar8 & 0xff) != 0) |
                    uVar1 >> (uVar8 & 0xff) | uVar10 << (0x20 - uVar8 & 0xff);
            uVar8 = uVar10 >> (uVar8 & 0xff);
          }
          else {
            if (uVar8 == 0x20) {
              iVar3 = 0;
            }
            else {
              iVar3 = uVar10 << (0x40 - uVar8 & 0xff);
            }
            uVar6 = (uint)(uVar1 != 0 || iVar3 != 0) | uVar10 >> (uVar8 - 0x20 & 0xff);
            uVar8 = 0;
          }
        }
        else {
          uVar8 = 0;
          uVar6 = 0;
          if (uVar10 != 0 || uVar1 != 0) {
            uVar6 = 1;
          }
        }
        uVar6 = uVar4 - uVar6;
        uVar10 = (uVar11 - uVar8) - (uint)(uVar4 < uVar6);
        uVar12 = uVar14;
        iVar3 = iVar5;
        goto LAB_08001450;
      }
      if ((uVar12 + 1 & 0x7ff) < 2) {
        if (uVar12 == 0) {
          if (uVar10 == 0 && uVar1 == 0) {
            if (uVar11 == 0 && uVar4 == 0) {
LAB_08001732:
              iVar3 = 0;
              uVar8 = 1;
              uVar10 = 0;
              uVar1 = 0;
              uVar12 = 0;
              goto LAB_080014c8;
            }
            uVar8 = 1;
            uVar12 = 0;
            uVar10 = uVar11;
            uVar1 = uVar4;
            iVar3 = iVar5;
          }
          else if (uVar11 == 0 && uVar4 == 0) {
            uVar8 = 1;
            uVar12 = 0;
          }
          else {
            uVar6 = uVar1 + param_3 * -8;
            uVar1 = (uVar10 - uVar11) - (uint)(uVar1 < uVar6);
            if ((int)(uVar1 * 0x100) < 0) {
              uVar1 = uVar4 + param_1 * -8;
              uVar8 = 1;
              uVar12 = 0;
              uVar10 = (uVar11 - uVar10) - (uint)(uVar4 < uVar1);
              iVar3 = iVar5;
            }
            else {
              if (uVar1 == 0 && uVar6 == 0) goto LAB_08001732;
              uVar8 = 1;
              uVar12 = 0;
              uVar10 = uVar1;
              uVar1 = uVar6;
            }
          }
        }
        else if (uVar10 == 0 && uVar1 == 0) {
          if (uVar11 == 0 && uVar4 == 0) {
            iVar3 = 0;
            uVar8 = 0x800;
            uVar10 = 0x7fffff;
            uVar1 = 0xfffffffc;
            uVar12 = 0x7ff;
            goto LAB_080014c8;
          }
          uVar8 = 0x800;
          uVar12 = 0x7ff;
          uVar10 = uVar11;
          uVar1 = uVar4;
          iVar3 = iVar5;
        }
        else {
          if (uVar11 == 0 && uVar4 == 0) goto LAB_080019e2;
          uVar11 = param_1 & 0x1fffffff | uVar13 << 0x1d;
          if (((uVar9 & 0x400000) != 0) && ((uVar6 & 0x400000) == 0)) {
            uVar11 = uVar7 << 0x1d | param_3 & 0x1fffffff;
            iVar3 = iVar5;
            param_2 = param_4;
          }
          uVar1 = uVar11 << 3;
          uVar10 = (param_2 & 0xfffff) << 3 | uVar11 >> 0x1d;
          uVar8 = 0x800;
          uVar12 = 0x7ff;
        }
        goto LAB_080014b2;
      }
      uVar6 = uVar1 + param_3 * -8;
      uVar8 = (uVar10 - uVar11) - (uint)(uVar1 < uVar6);
      if ((int)(uVar8 * 0x100) < 0) {
        uVar6 = uVar4 + param_1 * -8;
        uVar8 = (uVar11 - uVar10) - (uint)(uVar4 < uVar6);
        iVar3 = iVar5;
      }
      else if (uVar8 == 0 && uVar6 == 0) goto LAB_08001732;
LAB_0800145c:
      if (uVar8 == 0) {
        iVar2 = FUN_0800216c(uVar6);
        iVar5 = iVar2 + 0x20;
        uVar11 = iVar2 + 0x18;
        if (0x1f < (int)uVar11) goto LAB_08001620;
LAB_08001474:
        uVar1 = uVar6 >> (0x28U - iVar5 & 0xff) | uVar8 << (uVar11 & 0xff);
        uVar6 = uVar6 << (uVar11 & 0xff);
      }
      else {
        iVar5 = FUN_0800216c(uVar8);
        uVar11 = iVar5 - 8;
        if ((int)uVar11 < 0x20) goto LAB_08001474;
LAB_08001620:
        uVar1 = uVar6 << (iVar5 - 0x28U & 0xff);
        uVar6 = 0;
      }
      if ((int)uVar12 <= (int)uVar11) {
        iVar5 = uVar11 - uVar12;
        uVar11 = iVar5 + 1;
        if ((int)uVar11 < 0x20) {
          uVar8 = 1;
          uVar10 = uVar1 >> (uVar11 & 0xff);
          uVar1 = (uint)(uVar6 << (0x1fU - iVar5 & 0xff) != 0) |
                  uVar6 >> (uVar11 & 0xff) | uVar1 << (0x1fU - iVar5 & 0xff);
          uVar12 = 0;
        }
        else {
          if (uVar11 == 0x20) {
            iVar2 = 0;
          }
          else {
            iVar2 = uVar1 << (0x3fU - iVar5 & 0xff);
          }
          uVar8 = 1;
          uVar10 = 0;
          uVar1 = (uint)(uVar6 != 0 || iVar2 != 0) | uVar1 >> (iVar5 - 0x1fU & 0xff);
          uVar12 = 0;
        }
        goto LAB_080014b2;
      }
      uVar12 = uVar12 - uVar11;
      uVar10 = uVar1 & 0xff7fffff;
LAB_080015aa:
      uVar8 = uVar12 + 1;
      uVar1 = uVar6;
      goto LAB_080014b2;
    }
    if (uVar14 != 0) {
      if (uVar12 == 0x7ff) {
        uVar8 = 0x800;
        goto LAB_080014b2;
      }
      uVar11 = uVar11 | 0x800000;
LAB_08001410:
      if ((int)uVar8 < 0x39) {
        if ((int)uVar8 < 0x20) {
          uVar6 = uVar11 >> (uVar8 & 0xff);
          uVar11 = uVar4 >> (uVar8 & 0xff) | uVar11 << (0x20 - uVar8 & 0xff) |
                   (uint)(uVar4 << (0x20 - uVar8 & 0xff) != 0);
        }
        else {
          if (uVar8 == 0x20) {
            iVar5 = 0;
          }
          else {
            iVar5 = uVar11 << (0x40 - uVar8 & 0xff);
          }
          uVar11 = (uint)(iVar5 != 0 || uVar4 != 0) | uVar11 >> (uVar8 - 0x20 & 0xff);
          uVar6 = 0;
        }
      }
      else {
        uVar6 = 0;
        if (uVar11 == 0 && uVar4 == 0) {
          uVar11 = 0;
        }
        else {
          uVar11 = 1;
        }
      }
      iVar5 = -uVar11;
      uVar11 = uVar6;
LAB_08001442:
      uVar6 = uVar1 + iVar5;
      uVar10 = (uVar10 - uVar11) - (uint)(uVar1 < uVar6);
LAB_08001450:
      if ((int)(uVar10 << 8) < 0) {
        uVar8 = uVar10 & 0x7fffff;
        goto LAB_0800145c;
      }
      goto LAB_080015aa;
    }
    if (uVar11 != 0 || uVar4 != 0) {
      uVar8 = uVar8 - 1;
      if (uVar8 != 0) {
        if (uVar12 == 0x7ff) goto LAB_08001680;
        goto LAB_08001410;
      }
      iVar5 = param_3 * -8;
      goto LAB_08001442;
    }
    uVar8 = uVar12 + 1;
    uVar11 = uVar1;
    if ((uVar1 & 0xf) == 4) goto LAB_080014c8;
LAB_080014ba:
    uVar1 = uVar11 + 4;
    uVar10 = uVar10 + (uVar1 < uVar11);
  }
LAB_080014c8:
  uVar11 = uVar8;
  if ((int)(uVar10 << 8) < 0) {
    if (uVar8 == 0x7ff) {
      uVar10 = 0;
      uVar8 = 0;
      uVar12 = 0x7ff;
      goto LAB_080014f4;
    }
    uVar10 = uVar10 & 0xff7fffff;
    uVar11 = uVar8 + 1;
    uVar12 = uVar8;
  }
  uVar8 = uVar10 << 0x1d | uVar1 >> 3;
  uVar10 = uVar10 >> 3;
  if ((uVar11 & 0x7ff) < 2) {
    if (uVar8 == 0 && uVar10 == 0) {
      uVar12 = uVar12 & 0x7ff;
      uVar10 = 0;
      uVar8 = 0;
      goto LAB_080014f4;
    }
    if (uVar12 == 0) {
      uVar10 = uVar10 & 0xfffff;
      uVar12 = 0;
      goto LAB_080014f4;
    }
    uVar10 = uVar10 | 0x80000;
  }
  uVar10 = uVar10 & 0xfffff;
  uVar12 = uVar12 & 0x7ff;
LAB_080014f4:
  return CONCAT44(uVar10 | uVar12 << 0x14 | iVar3 << 0x1f,uVar8);
}


