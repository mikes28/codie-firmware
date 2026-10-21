/* ==== FUN_080000C8 @ 0x80000c8 ==== */

// WARNING: Removing unreachable block (ram,0x080000d8)

void FUN_080000C8(void)

{
  if (DAT_20000f08 == '\0') {
    DAT_20000f08 = '\x01';
  }
  return;
}


/* ==== FUN_080000DE @ 0x80000de ==== */

void FUN_080000DE(void)

{
  undefined1 *unaff_r4;
  
  *unaff_r4 = 1;
  return;
}


/* ==== FUN_080000E2 @ 0x80000e2 ==== */

void FUN_080000E2(void)

{
  return;
}


/* ==== FUN_080000F0 @ 0x80000f0 ==== */

// WARNING: Removing unreachable block (ram,0x080000f8)
// WARNING: Removing unreachable block (ram,0x0800010e)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080000F0(void)

{
  return;
}


/* ==== FUN_08000128 @ 0x8000128 ==== */

ulonglong FUN_08000128(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  
  if (param_2 == 0) {
    uVar1 = 0;
    if (param_1 != 0) {
      uVar1 = 0xffffffff;
    }
    return (ulonglong)uVar1;
  }
  uVar2 = 1;
  uVar1 = 0;
  if (param_2 <= param_1) {
    for (; (param_2 < 0x10000000 && (param_2 < param_1)); param_2 = param_2 << 4) {
      uVar2 = uVar2 << 4;
    }
    for (; (param_2 < 0x80000000 && (param_2 < param_1)); param_2 = param_2 << 1) {
      uVar2 = uVar2 << 1;
    }
    while( true ) {
      if (param_2 <= param_1) {
        param_1 = param_1 - param_2;
        uVar1 = uVar1 | uVar2;
      }
      if (param_2 >> 1 <= param_1) {
        param_1 = param_1 - (param_2 >> 1);
        uVar1 = uVar1 | uVar2 >> 1;
      }
      if (param_2 >> 2 <= param_1) {
        param_1 = param_1 - (param_2 >> 2);
        uVar1 = uVar1 | uVar2 >> 2;
      }
      if (param_2 >> 3 <= param_1) {
        param_1 = param_1 - (param_2 >> 3);
        uVar1 = uVar1 | uVar2 >> 3;
      }
      if ((param_1 == 0) || (uVar2 = uVar2 >> 4, uVar2 == 0)) break;
      param_2 = param_2 >> 4;
    }
  }
  return CONCAT44(param_2,uVar1);
}


/* ==== FUN_0800012c @ 0x800012c ==== */

uint FUN_0800012c(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = 1;
  uVar1 = 0;
  if (param_2 <= param_1) {
    for (; (param_2 < 0x10000000 && (param_2 < param_1)); param_2 = param_2 << 4) {
      uVar2 = uVar2 << 4;
    }
    for (; (param_2 < 0x80000000 && (param_2 < param_1)); param_2 = param_2 << 1) {
      uVar2 = uVar2 << 1;
    }
    while( true ) {
      if (param_2 <= param_1) {
        param_1 = param_1 - param_2;
        uVar1 = uVar1 | uVar2;
      }
      if (param_2 >> 1 <= param_1) {
        param_1 = param_1 - (param_2 >> 1);
        uVar1 = uVar1 | uVar2 >> 1;
      }
      if (param_2 >> 2 <= param_1) {
        param_1 = param_1 - (param_2 >> 2);
        uVar1 = uVar1 | uVar2 >> 2;
      }
      if (param_2 >> 3 <= param_1) {
        param_1 = param_1 - (param_2 >> 3);
        uVar1 = uVar1 | uVar2 >> 3;
      }
      if ((param_1 == 0) || (uVar2 = uVar2 >> 4, uVar2 == 0)) break;
      param_2 = param_2 >> 4;
    }
  }
  return uVar1;
}


/* ==== FUN_080001b0 @ 0x80001b0 ==== */

ulonglong FUN_080001b0(int param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  
  if (param_2 == 0) {
    uVar1 = 0;
    if (param_1 != 0) {
      uVar1 = 0xffffffff;
    }
    return (ulonglong)uVar1;
  }
  iVar2 = FUN_0800012c();
  return CONCAT44(param_1 - iVar2 * param_2,iVar2);
}


/* ==== FUN_080001B4 @ 0x80001b4 ==== */

void FUN_080001B4(void)

{
  FUN_0800012c();
  return;
}


/* ==== FUN_080001c4 @ 0x80001c4 ==== */

ulonglong FUN_080001c4(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  if (param_2 == 0) {
    uVar1 = 0;
    if (param_1 != 0) {
      if ((int)param_1 < 0) {
        uVar1 = 0x80000000;
      }
      else {
        uVar1 = 0x7fffffff;
      }
    }
    return (ulonglong)uVar1;
  }
  uVar3 = param_1 ^ param_2;
  uVar2 = 1;
  uVar1 = 0;
  if ((int)param_2 < 0) {
    param_2 = -param_2;
  }
  if ((int)param_1 < 0) {
    param_1 = -param_1;
  }
  if (param_2 <= param_1) {
    for (; (param_2 < 0x10000000 && (param_2 < param_1)); param_2 = param_2 << 4) {
      uVar2 = uVar2 << 4;
    }
    for (; (param_2 < 0x80000000 && (param_2 < param_1)); param_2 = param_2 << 1) {
      uVar2 = uVar2 << 1;
    }
    while( true ) {
      if (param_2 <= param_1) {
        param_1 = param_1 - param_2;
        uVar1 = uVar1 | uVar2;
      }
      if (param_2 >> 1 <= param_1) {
        param_1 = param_1 - (param_2 >> 1);
        uVar1 = uVar1 | uVar2 >> 1;
      }
      if (param_2 >> 2 <= param_1) {
        param_1 = param_1 - (param_2 >> 2);
        uVar1 = uVar1 | uVar2 >> 2;
      }
      if (param_2 >> 3 <= param_1) {
        param_1 = param_1 - (param_2 >> 3);
        uVar1 = uVar1 | uVar2 >> 3;
      }
      if ((param_1 == 0) || (uVar2 = uVar2 >> 4, uVar2 == 0)) break;
      param_2 = param_2 >> 4;
    }
  }
  if ((int)uVar3 < 0) {
    uVar1 = -uVar1;
  }
  return CONCAT44(param_2,uVar1);
}


/* ==== FUN_080001c8 @ 0x80001c8 ==== */

uint FUN_080001c8(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = param_1 ^ param_2;
  uVar2 = 1;
  uVar1 = 0;
  if ((int)param_2 < 0) {
    param_2 = -param_2;
  }
  if ((int)param_1 < 0) {
    param_1 = -param_1;
  }
  if (param_2 <= param_1) {
    for (; (param_2 < 0x10000000 && (param_2 < param_1)); param_2 = param_2 << 4) {
      uVar2 = uVar2 << 4;
    }
    for (; (param_2 < 0x80000000 && (param_2 < param_1)); param_2 = param_2 << 1) {
      uVar2 = uVar2 << 1;
    }
    while( true ) {
      if (param_2 <= param_1) {
        param_1 = param_1 - param_2;
        uVar1 = uVar1 | uVar2;
      }
      if (param_2 >> 1 <= param_1) {
        param_1 = param_1 - (param_2 >> 1);
        uVar1 = uVar1 | uVar2 >> 1;
      }
      if (param_2 >> 2 <= param_1) {
        param_1 = param_1 - (param_2 >> 2);
        uVar1 = uVar1 | uVar2 >> 2;
      }
      if (param_2 >> 3 <= param_1) {
        param_1 = param_1 - (param_2 >> 3);
        uVar1 = uVar1 | uVar2 >> 3;
      }
      if ((param_1 == 0) || (uVar2 = uVar2 >> 4, uVar2 == 0)) break;
      param_2 = param_2 >> 4;
    }
  }
  if ((int)uVar3 < 0) {
    uVar1 = -uVar1;
  }
  return uVar1;
}


/* ==== FUN_08000270 @ 0x8000270 ==== */

ulonglong FUN_08000270(int param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  
  if (param_2 == 0) {
    uVar1 = 0;
    if (param_1 != 0) {
      if (param_1 < 0) {
        uVar1 = 0x80000000;
      }
      else {
        uVar1 = 0x7fffffff;
      }
    }
    return (ulonglong)uVar1;
  }
  iVar2 = FUN_080001c8();
  return CONCAT44(param_1 - iVar2 * param_2,iVar2);
}


/* ==== FUN_08000274 @ 0x8000274 ==== */

void FUN_08000274(void)

{
  FUN_080001c8();
  return;
}


/* ==== FUN_08000290 @ 0x8000290 ==== */

undefined8 FUN_08000290(undefined4 param_1,undefined4 param_2)

{
  FUN_08000B6C();
  return CONCAT44(param_2,param_1);
}


/* ==== FUN_080002A0 @ 0x80002a0 ==== */

int FUN_080002A0(void)

{
  int iVar1;
  
  iVar1 = FUN_08000a8c();
  return 1 - iVar1;
}


/* ==== FUN_080002AC @ 0x80002ac ==== */

undefined4 FUN_080002AC(void)

{
  int iVar1;
  
  iVar1 = FUN_08000B6C();
  if (-1 < iVar1) {
    return 0;
  }
  return 1;
}


/* ==== FUN_080002C0 @ 0x80002c0 ==== */

undefined4 FUN_080002C0(void)

{
  int iVar1;
  
  iVar1 = FUN_08000B6C();
  if (0 < iVar1) {
    return 0;
  }
  return 1;
}


/* ==== FUN_080002D4 @ 0x80002d4 ==== */

undefined4 FUN_080002D4(void)

{
  int iVar1;
  
  iVar1 = FUN_08000ADC();
  if (iVar1 < 1) {
    return 0;
  }
  return 1;
}


/* ==== FUN_080002E8 @ 0x80002e8 ==== */

undefined4 FUN_080002E8(void)

{
  int iVar1;
  
  iVar1 = FUN_08000ADC();
  if (iVar1 < 0) {
    return 0;
  }
  return 1;
}


/* ==== FUN_080002fc @ 0x80002fc ==== */

int FUN_080002fc(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  
  if ((param_4 == 0) && (param_3 == 0)) {
    if (param_2 != 0 || param_1 != 0) {
      param_1 = -1;
    }
    return param_1;
  }
  iVar1 = FUN_0800043C(param_1);
  return iVar1;
}


/* ==== FUN_08000328 @ 0x8000328 ==== */

void FUN_08000328(void)

{
  undefined4 in_stack_00000000;
  
  FUN_0800043C(in_stack_00000000);
  return;
}


/* ==== FUN_0800033c @ 0x800033c ==== */

undefined8 FUN_0800033c(uint param_1,int param_2,uint param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  
  uVar3 = (param_3 & 0xffff) * (param_1 & 0xffff);
  uVar4 = (param_1 >> 0x10) * (param_3 & 0xffff);
  iVar2 = (param_1 >> 0x10) * (param_3 >> 0x10);
  uVar1 = uVar4 + (param_3 >> 0x10) * (param_1 & 0xffff) + (uVar3 >> 0x10);
  if (uVar1 <= uVar4 && uVar4 - uVar1 != 0) {
    iVar2 = iVar2 + 0x10000;
  }
  return CONCAT44(param_4 * param_1 + param_2 * param_3 + iVar2 + (uVar1 >> 0x10),
                  uVar1 * 0x10000 + (uVar3 & 0xffff));
}


/* ==== FUN_08000340 @ 0x8000340 ==== */

undefined8 FUN_08000340(uint param_1,int param_2,uint param_3,uint param_4)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int in_r12;
  
  uVar3 = (param_3 & 0xffff) * (param_4 >> 0x10);
  uVar4 = (param_1 >> 0x10) * (param_3 & 0xffff);
  iVar2 = (param_1 >> 0x10) * (param_3 >> 0x10);
  uVar1 = uVar4 + (param_3 >> 0x10) * (param_4 >> 0x10) + (uVar3 >> 0x10);
  if (uVar1 <= uVar4 && uVar4 - uVar1 != 0) {
    iVar2 = iVar2 + 0x10000;
  }
  return CONCAT44(in_r12 * param_1 + param_2 * param_3 + iVar2 + (uVar1 >> 0x10),
                  uVar1 * 0x10000 + (uVar3 & 0xffff));
}


/* ==== FUN_08000380 @ 0x8000380 ==== */

int FUN_08000380(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = FUN_080002E8(param_1,0x4f000000);
  if (iVar1 == 0) {
    iVar1 = FUN_08001208(param_1);
  }
  else {
    FUN_08000E5C(param_1,0x4f000000);
    iVar1 = FUN_08001208();
    iVar1 = iVar1 + -0x80000000;
  }
  return iVar1;
}


/* ==== FUN_08000382 @ 0x8000382 ==== */

int FUN_08000382(undefined4 param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_080002E8(param_1,param_2 << 0x17);
  if (iVar1 == 0) {
    iVar1 = FUN_08001208(param_1);
  }
  else {
    FUN_08000E5C(param_1,0x4f000000);
    iVar1 = FUN_08001208();
    iVar1 = iVar1 + -0x80000000;
  }
  return iVar1;
}


/* ==== FUN_080003b0 @ 0x80003b0 ==== */

void FUN_080003b0(uint param_1,uint param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  if ((0x1fffff < param_2) && ((param_1 & 0x7ff) != 0)) {
    param_1 = param_1 & 0xfffff800 | 0x800;
  }
  uVar1 = FUN_08001FD4(param_2);
  uVar1 = FUN_08001AF0((int)uVar1,(int)((ulonglong)uVar1 >> 0x20),0,0x41f00000);
  uVar2 = FUN_08001FD4(param_1);
  FUN_08001398((int)uVar2,(int)((ulonglong)uVar2 >> 0x20),(int)uVar1,(int)((ulonglong)uVar1 >> 0x20)
              );
  FUN_08002038();
  return;
}


/* ==== FUN_080003B2 @ 0x80003b2 ==== */

void FUN_080003B2(uint param_1,uint param_2,undefined4 param_3,uint param_4)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  if ((param_4 < param_2) && ((param_1 & 0x7ff) != 0)) {
    param_1 = param_1 & 0xfffff800 | 0x800;
  }
  uVar1 = FUN_08001FD4(param_2);
  uVar1 = FUN_08001AF0((int)uVar1,(int)((ulonglong)uVar1 >> 0x20),0,0x41f00000);
  uVar2 = FUN_08001FD4(param_1);
  FUN_08001398((int)uVar2,(int)((ulonglong)uVar2 >> 0x20),(int)uVar1,(int)((ulonglong)uVar1 >> 0x20)
              );
  FUN_08002038();
  return;
}


/* ==== FUN_08000400 @ 0x8000400 ==== */

undefined8 FUN_08000400(uint param_1,int param_2,undefined4 param_3,undefined4 param_4,int *param_5)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  uVar1 = FUN_080021A8();
  uVar2 = FUN_0800033c(param_3,param_4,(int)uVar1,(int)((ulonglong)uVar1 >> 0x20));
  *param_5 = param_1 - (uint)uVar2;
  param_5[1] = (param_2 - (int)((ulonglong)uVar2 >> 0x20)) - (uint)(param_1 < (uint)uVar2);
  return uVar1;
}


/* ==== FUN_0800043C @ 0x800043c ==== */

undefined8 FUN_0800043C(uint param_1,int param_2,undefined4 param_3,undefined4 param_4,int *param_5)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  uVar1 = FUN_08002578();
  uVar2 = FUN_0800033c((int)uVar1,(int)((ulonglong)uVar1 >> 0x20),param_3,param_4);
  *param_5 = param_1 - (uint)uVar2;
  param_5[1] = (param_2 - (int)((ulonglong)uVar2 >> 0x20)) - (uint)(param_1 < (uint)uVar2);
  return uVar1;
}


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


/* ==== FUN_08000a8c @ 0x8000a8c ==== */

undefined4 FUN_08000a8c(uint param_1,uint param_2)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  uVar4 = param_1 & 0x7fffff;
  uVar3 = (param_1 & 0x7fffffff) >> 0x17;
  uVar2 = (param_2 & 0x7fffffff) >> 0x17;
  if (((((uVar3 != 0xff) || (uVar1 = 1, uVar4 == 0)) &&
       ((uVar2 != 0xff || (uVar1 = 1, (param_2 & 0x7fffff) == 0)))) &&
      ((uVar1 = 1, uVar3 == uVar2 && (uVar1 = 1, uVar4 == (param_2 & 0x7fffff))))) &&
     ((-((int)param_1 >> 0x1f) == -((int)param_2 >> 0x1f) || ((uVar3 == 0 && (uVar4 == 0)))))) {
    uVar1 = 0;
  }
  return uVar1;
}


/* ==== FUN_08000A90 @ 0x8000a90 ==== */

undefined4 FUN_08000A90(int param_1,uint param_2,uint param_3,uint param_4)

{
  undefined4 uVar1;
  uint uVar2;
  
  param_4 = param_4 >> 9;
  param_3 = param_3 >> 0x18;
  uVar2 = (param_2 & 0x7fffffff) >> 0x17;
  if (((((param_3 != 0xff) || (uVar1 = 1, param_4 == 0)) &&
       ((uVar2 != 0xff || (uVar1 = 1, (param_2 & 0x7fffff) == 0)))) &&
      ((uVar1 = 1, param_3 == uVar2 && (uVar1 = 1, param_4 == (param_2 & 0x7fffff))))) &&
     ((-(param_1 >> 0x1f) == -((int)param_2 >> 0x1f) || ((param_3 == 0 && (param_4 == 0)))))) {
    uVar1 = 0;
  }
  return uVar1;
}


/* ==== FUN_08000ADC @ 0x8000adc ==== */

undefined4 FUN_08000ADC(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  bool bVar6;
  
  iVar5 = -((int)param_1 >> 0x1f);
  uVar3 = param_1 & 0x7fffff;
  uVar4 = param_2 & 0x7fffff;
  uVar2 = (param_1 & 0x7fffffff) >> 0x17;
  uVar1 = (param_2 & 0x7fffffff) >> 0x17;
  if (((uVar2 == 0xff) && (uVar3 != 0)) || ((uVar1 == 0xff && (uVar4 != 0)))) {
    return 0xfffffffe;
  }
  if (uVar2 == 0) {
    bVar6 = uVar3 == 0;
    if (uVar1 == 0) goto LAB_08000b1e;
    if (bVar6) goto LAB_08000b3a;
  }
  else if (uVar1 == 0) {
    bVar6 = false;
LAB_08000b1e:
    if (bVar6) {
      if (uVar4 == 0) {
        return 0;
      }
LAB_08000b3a:
      if (-((int)param_2 >> 0x1f) == 0) {
        return 0xffffffff;
      }
      return 1;
    }
    if (uVar4 == 0) {
      if (iVar5 != 0) {
        return 0xffffffff;
      }
      return 1;
    }
  }
  if ((iVar5 == -((int)param_2 >> 0x1f)) && (uVar2 <= uVar1)) {
    if (uVar1 <= uVar2) {
      if (uVar4 < uVar3) goto LAB_08000b10;
      if (uVar4 <= uVar3) {
        return 0;
      }
    }
    if (iVar5 != 0) {
      return 1;
    }
  }
  else {
LAB_08000b10:
    if (iVar5 == 0) {
      return 1;
    }
  }
  return 0xffffffff;
}


/* ==== FUN_08000B6C @ 0x8000b6c ==== */

undefined4 FUN_08000B6C(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  bool bVar6;
  
  iVar5 = -((int)param_1 >> 0x1f);
  uVar3 = param_1 & 0x7fffff;
  uVar4 = param_2 & 0x7fffff;
  uVar2 = (param_1 & 0x7fffffff) >> 0x17;
  uVar1 = (param_2 & 0x7fffffff) >> 0x17;
  if ((uVar2 == 0xff) && (uVar3 != 0)) {
    return 2;
  }
  if ((uVar1 == 0xff) && (uVar4 != 0)) {
    return 2;
  }
  if (uVar2 == 0) {
    bVar6 = uVar3 == 0;
    if (uVar1 == 0) goto LAB_08000b94;
    if (bVar6) goto LAB_08000ba4;
  }
  else if (uVar1 == 0) {
    bVar6 = false;
LAB_08000b94:
    if (bVar6) {
      if (uVar4 == 0) {
        return 0;
      }
LAB_08000ba4:
      if (-((int)param_2 >> 0x1f) == 0) {
        return 0xffffffff;
      }
      return 1;
    }
    if (uVar4 == 0) {
      if (iVar5 != 0) {
        return 0xffffffff;
      }
      return 1;
    }
  }
  if ((iVar5 == -((int)param_2 >> 0x1f)) && (uVar2 <= uVar1)) {
    if (uVar1 <= uVar2) {
      if (uVar4 < uVar3) goto LAB_08000bc0;
      if (uVar4 <= uVar3) {
        return 0;
      }
    }
    if (iVar5 != 0) {
      return 1;
    }
  }
  else {
LAB_08000bc0:
    if (iVar5 == 0) {
      return 1;
    }
  }
  return 0xffffffff;
}


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


/* ==== FUN_08001208 @ 0x8001208 ==== */

uint FUN_08001208(uint param_1)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = (param_1 & 0x7fffffff) >> 0x17;
  uVar1 = 0;
  if (0x7e < uVar3) {
    if (uVar3 < 0x9e) {
      uVar2 = param_1 & 0x7fffff | 0x800000;
      if (uVar3 < 0x96) {
        uVar2 = uVar2 >> (0x96 - uVar3 & 0xff);
      }
      else {
        uVar2 = uVar2 << (uVar3 - 0x96 & 0xff);
      }
      uVar1 = -uVar2;
      if (-1 < (int)param_1) {
        uVar1 = uVar2;
      }
    }
    else {
      uVar1 = 0x7fffffff - ((int)param_1 >> 0x1f);
    }
  }
  return uVar1;
}


/* ==== FUN_0800120E @ 0x800120e ==== */

uint FUN_0800120E(int param_1,uint param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  
  param_3 = param_3 >> 0x18;
  uVar1 = 0;
  if (0x7e < param_3) {
    if (param_3 < 0x9e) {
      if (param_3 < 0x96) {
        uVar2 = (param_2 | 0x800000) >> (0x96 - param_3 & 0xff);
      }
      else {
        uVar2 = (param_2 | 0x800000) << (param_3 - 0x96 & 0xff);
      }
      uVar1 = -uVar2;
      if (-1 < param_1) {
        uVar1 = uVar2;
      }
    }
    else {
      uVar1 = 0x7fffffff - (param_1 >> 0x1f);
    }
  }
  return uVar1;
}


/* ==== FUN_08001248 @ 0x8001248 ==== */

uint FUN_08001248(uint param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  
  if (param_1 == 0) {
    iVar5 = 0;
    uVar2 = 0;
LAB_080012cc:
    uVar3 = 0;
    goto LAB_0800126e;
  }
  iVar5 = -((int)param_1 >> 0x1f);
  if (iVar5 != 0) {
    param_1 = -param_1;
  }
  iVar1 = FUN_0800216c(param_1);
  uVar2 = 0x9e - iVar1;
  if ((int)uVar2 < 0x97) {
    uVar3 = param_1 << (iVar1 - 8U & 0xff) & 0x7fffff;
    uVar2 = uVar2 & 0xff;
    goto LAB_0800126e;
  }
  if (0x99 < (int)uVar2) {
    param_1 = (uint)(param_1 << (iVar1 + 0x1bU & 0xff) != 0) | param_1 >> (5U - iVar1 & 0xff);
  }
  if (5 < iVar1) {
    param_1 = param_1 << (iVar1 - 5U & 0xff);
  }
  uVar4 = param_1 & 0xfbffffff;
  if ((param_1 & 0xf) != 4) {
    uVar4 = uVar4 + 4;
  }
  if ((int)(uVar4 << 5) < 0) {
    uVar2 = 0x9f - iVar1;
    if (uVar2 == 0xff) goto LAB_080012cc;
    uVar4 = uVar4 & 0xfbffffff;
  }
  uVar4 = uVar4 >> 3;
  if ((uVar2 + 1 & 0xff) < 2) {
    uVar3 = 0;
    if (uVar4 != 0) {
      if (uVar2 == 0) {
        uVar3 = uVar4 & 0x7fffff;
        uVar2 = 0;
        goto LAB_0800126e;
      }
      uVar4 = uVar4 | 0x400000;
      goto LAB_080012c0;
    }
  }
  else {
LAB_080012c0:
    uVar3 = uVar4 & 0x7fffff;
  }
  uVar2 = uVar2 & 0xff;
LAB_0800126e:
  return uVar3 | uVar2 << 0x17 | iVar5 << 0x1f;
}


/* ==== FUN_080012F0 @ 0x80012f0 ==== */

uint FUN_080012F0(uint param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  if (param_1 == 0) {
    uVar3 = 0;
    uVar2 = 0;
  }
  else {
    iVar1 = FUN_0800216c();
    uVar3 = 0x9e - iVar1;
    if ((int)uVar3 < 0x97) {
      uVar2 = param_1 << (iVar1 - 8U & 0xff) & 0x7fffff;
      uVar3 = uVar3 & 0xff;
    }
    else {
      if (0x99 < (int)uVar3) {
        param_1 = param_1 >> (5U - iVar1 & 0xff) | (uint)(param_1 << (iVar1 + 0x1bU & 0xff) != 0);
      }
      if (5 < iVar1) {
        param_1 = param_1 << (iVar1 - 5U & 0xff);
      }
      uVar2 = param_1 & 0xfbffffff;
      if ((param_1 & 0xf) != 4) {
        uVar2 = uVar2 + 4;
      }
      if ((int)(uVar2 << 5) < 0) {
        uVar3 = 0x9f - iVar1;
        if (uVar3 == 0xff) {
          uVar3 = 0xff;
          uVar2 = 0;
          goto LAB_0800130c;
        }
        uVar2 = uVar2 & 0xfbffffff;
      }
      uVar2 = uVar2 >> 3;
      if ((uVar3 + 1 & 0xff) < 2) {
        if (uVar2 == 0) {
          uVar3 = uVar3 & 0xff;
          uVar2 = 0;
        }
        else if (uVar3 == 0) {
          uVar2 = uVar2 & 0x7fffff;
          uVar3 = 0;
        }
        else {
          uVar2 = uVar2 & 0x7fffff | 0x400000;
          uVar3 = uVar3 & 0xff;
        }
      }
      else {
        uVar2 = uVar2 & 0x7fffff;
        uVar3 = uVar3 & 0xff;
      }
    }
  }
LAB_0800130c:
  return uVar3 << 0x17 | uVar2;
}


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


/* ==== FUN_08001FD4 @ 0x8001fd4 ==== */

undefined8 FUN_08001FD4(uint param_1)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  
  if (param_1 == 0) {
    uVar2 = 0;
    uVar3 = 0;
    iVar4 = 0;
  }
  else {
    iVar1 = FUN_0800216c();
    if (iVar1 < 0xb) {
      iVar4 = param_1 << (iVar1 + 0x15U & 0xff);
      uVar3 = param_1 >> (0xbU - iVar1 & 0xff) & 0xfffff;
      uVar2 = 0x41eU - iVar1 & 0x7ff;
    }
    else {
      uVar2 = 0x41eU - iVar1 & 0x7ff;
      uVar3 = param_1 << (iVar1 - 0xbU & 0xff) & 0xfffff;
      iVar4 = 0;
    }
  }
  return CONCAT44(uVar3 | uVar2 << 0x14,iVar4);
}


/* ==== FUN_08002038 @ 0x8002038 ==== */

uint FUN_08002038(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  
  uVar1 = (param_2 & 0x7fffffff) >> 0x14;
  uVar3 = (param_2 & 0xfffff) << 3 | param_1 >> 0x1d;
  uVar5 = param_1 * 8;
  if ((uVar1 + 1 & 0x7ff) < 2) {
    if (uVar1 != 0) {
      if (uVar5 != 0 || uVar3 != 0) {
        uVar3 = uVar3 << 3 | 0x2000000;
        uVar1 = 0x100;
        uVar2 = 0xff;
        goto LAB_080020a6;
      }
      goto LAB_0800205e;
    }
    uVar1 = 1;
    if (uVar3 == 0 && uVar5 == 0) {
      uVar3 = 4;
      uVar2 = 0;
    }
    else {
LAB_080020fa:
      uVar1 = 1;
      uVar3 = 5;
      uVar2 = 0;
    }
  }
  else {
    uVar2 = uVar1 - 0x380;
    if ((int)uVar2 < 0xff) {
      if ((int)uVar2 < 1) {
        if ((int)uVar2 < -0x17) goto LAB_080020fa;
        uVar3 = uVar3 | 0x800000;
        uVar2 = 0x39e - uVar1;
        if ((int)uVar2 < 0x20) {
          uVar3 = uVar3 << (uVar1 - 0x37e & 0xff) | (uint)(uVar5 << (uVar1 - 0x37e & 0xff) != 0) |
                  uVar5 >> (uVar2 & 0xff);
          uVar1 = 1;
          uVar2 = 0;
        }
        else {
          if (uVar2 == 0x20) {
            iVar4 = 0;
          }
          else {
            iVar4 = uVar3 << (uVar1 - 0x35e & 0xff);
          }
          uVar3 = (uint)(iVar4 != 0 || uVar5 != 0) | uVar3 >> (0x37e - uVar1 & 0xff);
          uVar1 = 1;
          uVar2 = 0;
        }
      }
      else {
        uVar1 = uVar1 - 0x37f;
        uVar3 = uVar3 << 3 | (uint)((param_1 & 0x3ffffff) != 0) | (param_1 & 0x1fffffff) >> 0x1a;
      }
LAB_080020a6:
      if ((uVar3 & 0xf) != 4) {
        uVar3 = uVar3 + 4;
      }
    }
    else {
LAB_0800205e:
      uVar1 = 0x100;
      uVar2 = 0xff;
      uVar3 = 4;
    }
  }
  uVar5 = uVar1;
  if ((int)(uVar3 << 5) < 0) {
    if (uVar1 == 0xff) {
      uVar2 = 0xff;
      uVar1 = 0;
      goto LAB_08002084;
    }
    uVar3 = uVar3 & 0xfbffffff;
    uVar5 = uVar1 + 1;
    uVar2 = uVar1;
  }
  uVar3 = uVar3 >> 3;
  if ((uVar5 & 0xff) < 2) {
    uVar1 = 0;
    if (uVar3 != 0) {
      if (uVar2 == 0) {
        uVar1 = uVar3 & 0x7fffff;
        uVar2 = 0;
        goto LAB_08002084;
      }
      uVar3 = uVar3 | 0x400000;
      goto LAB_0800207e;
    }
  }
  else {
LAB_0800207e:
    uVar1 = uVar3 & 0x7fffff;
  }
  uVar2 = uVar2 & 0xff;
LAB_08002084:
  return uVar1 | uVar2 << 0x17 | param_2 & 0x80000000;
}


/* ==== FUN_0800216c @ 0x800216c ==== */

int FUN_0800216c(uint param_1)

{
  int iVar1;
  
  iVar1 = 0x1c;
  if (0xffff < param_1) {
    param_1 = param_1 >> 0x10;
    iVar1 = 0xc;
  }
  if (0xff < param_1) {
    param_1 = param_1 >> 8;
    iVar1 = iVar1 + -8;
  }
  if (0xf < param_1) {
    param_1 = param_1 >> 4;
    iVar1 = iVar1 + -4;
  }
  return (uint)(byte)(&DAT_08002198)[param_1] + iVar1;
}


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


/* ==== Reset_Handler @ 0x8002904 ==== */

// WARNING: This function may have set the stack pointer

void Reset_Handler(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  for (iVar1 = 0; (undefined1 *)(iVar1 + 0x20000000) < &DAT_20000f04; iVar1 = iVar1 + 4) {
    *(undefined4 *)(iVar1 + 0x20000000) = *(undefined4 *)((int)&DAT_0800c720 + iVar1);
  }
  for (puVar2 = (undefined4 *)&DAT_20000f08; puVar2 < (undefined4 *)0x20001030; puVar2 = puVar2 + 1)
  {
    *puVar2 = 0;
  }
  FUN_08002958();
  FUN_0800B6B0();
  FUN_08009FA4();
  do {
                    // WARNING: Do nothing block with infinite loop
  } while( true );
}


/* ==== FUN_08002954 @ 0x8002954 ==== */

void FUN_08002954(void)

{
  do {
                    // WARNING: Do nothing block with infinite loop
  } while( true );
}


/* ==== FUN_08002958 @ 0x8002958 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08002958(void)

{
  uint uVar1;
  uint uVar2;
  int local_10;
  
  _DAT_4002102c = _DAT_4002102c & 0xfffffff0;
  _DAT_40021030 = _DAT_40021030 & 0xfffffeac;
  _DAT_40021034 = _DAT_40021034 & 0xfffffffe;
  _DAT_40021008 = 0;
  local_10 = 0;
  uVar2 = _DAT_40021000 & 0xfef2ffff | 0x10001;
  do {
    local_10 = local_10 + 1;
    if ((_DAT_40021000 & 0x20000) != 0) break;
  } while (local_10 != 0x5000);
  uVar1 = _DAT_40021004 & 0xf8c0b80c;
  if ((int)(uVar2 << 0xe) < 0) {
    _DAT_40022000 = 0x11;
    do {
    } while ((_DAT_40021000 & 0x2000000) == 0);
    do {
      uVar2 = _DAT_40021000 & 0xfef2ffff | 0x1010001;
      uVar1 = _DAT_40021004 & 0xf8c0380c | 0x130002;
    } while ((_DAT_40021004 & 0xc) != 8);
  }
  _DAT_40021004 = uVar1;
  _DAT_40021000 = uVar2;
  return;
}


/* ==== FUN_08002A60 @ 0x8002a60 ==== */

undefined4 FUN_08002A60(uint param_1)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar3 = param_1 >> 0x10 & 0xff;
  uVar2 = param_1 >> 0x18;
  if ((param_1 & 0xff) < 0x80) {
    if (uVar3 < 10) {
      if (uVar2 < *(byte *)(uVar3 * 8 + 0x20000198)) {
        uVar1 = *(undefined4 *)(uVar2 * 4 + *(int *)(uVar3 * 8 + 0x20000194));
      }
      else {
        uVar1 = 0;
      }
    }
    else {
      uVar1 = 0;
    }
  }
  else if (uVar3 < 10) {
    if (uVar2 < *(byte *)(uVar3 * 8 + 0x20000118)) {
      uVar1 = *(undefined4 *)(uVar2 * 4 + *(int *)(uVar3 * 8 + 0x20000114));
    }
    else {
      uVar1 = 0;
    }
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}


/* ==== FUN_08002ac0 @ 0x8002ac0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08002ac0(uint param_1,uint param_2,undefined4 param_3,undefined4 param_4)

{
  undefined1 *puVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint *puVar5;
  undefined1 *puVar6;
  uint uVar7;
  undefined4 uStack_44;
  undefined1 auStack_40 [20];
  uint uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  param_1 = param_1 & 0xff;
  puVar5 = &uStack_c;
  uVar2 = *(uint *)(&DAT_0800b93c + param_1 * 0xc);
  uStack_44 = *(undefined4 *)(&DAT_0800b938 + param_1 * 0xc);
  uStack_c = param_2;
  uStack_8 = param_3;
  uStack_4 = param_4;
  if (uVar2 == 0) {
    uVar3 = 0;
    uVar4 = 0;
  }
  else {
    uVar3 = 0;
    uVar4 = 0;
    puVar1 = auStack_40;
    do {
      switch(uVar2 & 0xf) {
      case 2:
      case 3:
        *puVar1 = (char)*puVar5;
        puVar1 = puVar1 + 1;
        puVar5 = puVar5 + 1;
        break;
      case 4:
      case 5:
        uVar7 = *puVar5;
        *puVar1 = (char)uVar7;
        puVar1[1] = (char)(uVar7 >> 8);
        puVar1 = puVar1 + 2;
        puVar5 = puVar5 + 1;
        break;
      case 6:
      case 7:
        uVar7 = *puVar5;
        *puVar1 = (char)uVar7;
        puVar1[1] = (char)(uVar7 >> 8);
        puVar1[2] = (char)(uVar7 >> 0x10);
        puVar1[3] = (char)(uVar7 >> 0x18);
        puVar1 = puVar1 + 4;
        puVar5 = puVar5 + 1;
        break;
      case 8:
      case 9:
        uVar3 = *puVar5 & 0xffff;
        *puVar1 = (char)*puVar5;
        puVar1 = puVar1 + 1;
        uVar4 = uVar3 + uStack_44._1_1_ & 0xffff;
        uStack_44._0_2_ = CONCAT11((char)uVar4,(byte)uStack_44);
        uStack_44 = CONCAT31(uStack_44._1_3_,(byte)uStack_44 | (byte)(uVar4 >> 8));
        uVar4 = puVar5[1];
        puVar5 = puVar5 + 2;
        break;
      case 10:
        puVar6 = (undefined1 *)*puVar5;
        *puVar1 = *puVar6;
        puVar1[1] = puVar6[1];
        puVar1[2] = puVar6[2];
        puVar1[3] = puVar6[3];
        puVar1[4] = puVar6[4];
        puVar1[5] = puVar6[5];
        puVar1 = puVar1 + 6;
        puVar5 = puVar5 + 1;
        break;
      case 0xb:
        uVar3 = *puVar5 & 0xffff;
        *puVar1 = (char)*puVar5;
        puVar1[1] = (char)(uVar3 >> 8);
        puVar1 = puVar1 + 2;
        uVar4 = uVar3 + uStack_44._1_1_ & 0xffff;
        uStack_44._0_2_ = CONCAT11((char)uVar4,(byte)uStack_44);
        uStack_44 = CONCAT31(uStack_44._1_3_,(byte)uStack_44 | (byte)(uVar4 >> 8));
        uVar4 = puVar5[1];
        puVar5 = puVar5 + 2;
      }
      uVar2 = uVar2 >> 4;
    } while (uVar2 != 0);
  }
  if (_DAT_20000f24 != (code *)0x0) {
    (*_DAT_20000f24)((&DAT_0800b939)[param_1 * 0xc] + '\x04',&uStack_44,uVar3,uVar4);
  }
  return;
}


/* ==== FUN_08002AC2 @ 0x8002ac2 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08002AC2(void)

{
  uint uVar1;
  undefined1 *puVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  undefined1 *puVar7;
  uint uVar8;
  byte in_stack_00000000;
  undefined4 local_34;
  undefined1 local_30 [20];
  
  uVar1 = (uint)in_stack_00000000;
  puVar6 = (uint *)&stack0x00000004;
  uVar3 = *(uint *)(&DAT_0800b93c + uVar1 * 0xc);
  local_34 = *(undefined4 *)(&DAT_0800b938 + uVar1 * 0xc);
  if (uVar3 == 0) {
    uVar4 = 0;
    uVar5 = 0;
  }
  else {
    uVar4 = 0;
    uVar5 = 0;
    puVar2 = local_30;
    do {
      switch(uVar3 & 0xf) {
      case 2:
      case 3:
        *puVar2 = (char)*puVar6;
        puVar2 = puVar2 + 1;
        puVar6 = puVar6 + 1;
        break;
      case 4:
      case 5:
        uVar8 = *puVar6;
        *puVar2 = (char)uVar8;
        puVar2[1] = (char)(uVar8 >> 8);
        puVar2 = puVar2 + 2;
        puVar6 = puVar6 + 1;
        break;
      case 6:
      case 7:
        uVar8 = *puVar6;
        *puVar2 = (char)uVar8;
        puVar2[1] = (char)(uVar8 >> 8);
        puVar2[2] = (char)(uVar8 >> 0x10);
        puVar2[3] = (char)(uVar8 >> 0x18);
        puVar2 = puVar2 + 4;
        puVar6 = puVar6 + 1;
        break;
      case 8:
      case 9:
        uVar4 = *puVar6 & 0xffff;
        *puVar2 = (char)*puVar6;
        puVar2 = puVar2 + 1;
        uVar5 = uVar4 + local_34._1_1_ & 0xffff;
        local_34._0_2_ = CONCAT11((char)uVar5,(byte)local_34);
        local_34 = CONCAT31(local_34._1_3_,(byte)local_34 | (byte)(uVar5 >> 8));
        uVar5 = puVar6[1];
        puVar6 = puVar6 + 2;
        break;
      case 10:
        puVar7 = (undefined1 *)*puVar6;
        *puVar2 = *puVar7;
        puVar2[1] = puVar7[1];
        puVar2[2] = puVar7[2];
        puVar2[3] = puVar7[3];
        puVar2[4] = puVar7[4];
        puVar2[5] = puVar7[5];
        puVar2 = puVar2 + 6;
        puVar6 = puVar6 + 1;
        break;
      case 0xb:
        uVar4 = *puVar6 & 0xffff;
        *puVar2 = (char)*puVar6;
        puVar2[1] = (char)(uVar4 >> 8);
        puVar2 = puVar2 + 2;
        uVar5 = uVar4 + local_34._1_1_ & 0xffff;
        local_34._0_2_ = CONCAT11((char)uVar5,(byte)local_34);
        local_34 = CONCAT31(local_34._1_3_,(byte)local_34 | (byte)(uVar5 >> 8));
        uVar5 = puVar6[1];
        puVar6 = puVar6 + 2;
      }
      uVar3 = uVar3 >> 4;
    } while (uVar3 != 0);
  }
  if (_DAT_20000f24 != (code *)0x0) {
    (*_DAT_20000f24)((&DAT_0800b939)[uVar1 * 0xc] + '\x04',&local_34,uVar4,uVar5);
  }
  return;
}


/* ==== FUN_08002DB8 @ 0x8002db8 ==== */

void FUN_08002DB8(void)

{
  FUN_0800976C();
  FUN_08009D30();
  FUN_080086C4();
  return;
}


/* ==== FUN_08002DC8 @ 0x8002dc8 ==== */

void FUN_08002DC8(void)

{
  FUN_08009968();
  FUN_08007EC8();
  return;
}


/* ==== FUN_08002DD4 @ 0x8002dd4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08002DD4(int param_1)

{
  uint uVar1;
  
  uVar1 = FUN_08000128(_DAT_20000000,1000000);
  _DAT_e000e018 = (uVar1 & 0xffffff) - 1;
  _DAT_e000e014 = _DAT_e000e018;
  if (param_1 == 0) {
    _DAT_e000e010 = 4;
    return;
  }
  do {
  } while( true );
}


/* ==== FUN_08002E1C @ 0x8002e1c ==== */

undefined4 FUN_08002E1C(int *param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  
  if (param_1 == (int *)0x0) {
    uVar1 = 0;
  }
  else {
    uVar1 = 0;
    if (param_3 != 0) {
      if (param_2 == 0) {
        uVar1 = 0;
      }
      else {
        *param_1 = param_2;
        param_1[2] = param_2;
        param_1[3] = param_2;
        *(short *)(param_1 + 1) = (short)param_3;
        *(undefined2 *)((int)param_1 + 6) = 0;
        uVar1 = 1;
      }
    }
  }
  return uVar1;
}


/* ==== FUN_08002E44 @ 0x8002e44 ==== */

undefined1 FUN_08002E44(int *param_1)

{
  undefined1 uVar1;
  undefined1 *puVar2;
  
  disableIRQinterrupts();
  uVar1 = 0;
  if (*(short *)((int)param_1 + 6) != 0) {
    enableIRQinterrupts();
    uVar1 = *(undefined1 *)param_1[2];
    puVar2 = (undefined1 *)param_1[2] + 1;
    param_1[2] = (int)puVar2;
    if (puVar2 == (undefined1 *)(*param_1 + (uint)*(ushort *)(param_1 + 1))) {
      param_1[2] = *param_1;
    }
    disableIRQinterrupts();
    *(short *)((int)param_1 + 6) = *(short *)((int)param_1 + 6) + -1;
    enableIRQinterrupts();
  }
  return uVar1;
}


/* ==== FUN_08002E78 @ 0x8002e78 ==== */

int FUN_08002E78(int *param_1,undefined1 *param_2,uint param_3)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  
  disableIRQinterrupts();
  if (*(short *)((int)param_1 + 6) == 0) {
    return 0;
  }
  if (*(ushort *)((int)param_1 + 6) < param_3) {
    param_3 = (uint)*(ushort *)((int)param_1 + 6);
  }
  enableIRQinterrupts();
  disableIRQinterrupts();
  enableIRQinterrupts();
  uVar3 = param_3;
  if ((param_3 != 0) && (*(short *)((int)param_1 + 6) != 0)) {
    uVar1 = param_3;
    if (-1 < (int)(param_3 << 0x1f)) {
      *param_2 = *(undefined1 *)param_1[2];
      param_2 = param_2 + 1;
      iVar2 = param_1[2];
      param_1[2] = iVar2 + 1;
      if (iVar2 + 1 == *param_1 + (uint)*(ushort *)(param_1 + 1)) {
        param_1[2] = *param_1;
      }
      uVar1 = param_3 - 1;
      disableIRQinterrupts();
      enableIRQinterrupts();
      uVar3 = uVar1;
      if (*(short *)((int)param_1 + 6) == 0) goto LAB_08002ed8;
    }
    do {
      *param_2 = *(undefined1 *)param_1[2];
      iVar2 = param_1[2];
      param_1[2] = iVar2 + 1;
      if (iVar2 + 1 == *param_1 + (uint)*(ushort *)(param_1 + 1)) {
        param_1[2] = *param_1;
      }
      uVar3 = uVar1 - 1;
      disableIRQinterrupts();
      enableIRQinterrupts();
      if ((uVar3 == 0) || (*(short *)((int)param_1 + 6) == 0)) break;
      param_2[1] = *(undefined1 *)param_1[2];
      param_2 = param_2 + 2;
      iVar2 = param_1[2];
      param_1[2] = iVar2 + 1;
      if (iVar2 + 1 == *param_1 + (uint)*(ushort *)(param_1 + 1)) {
        param_1[2] = *param_1;
      }
      uVar1 = uVar1 - 2;
      disableIRQinterrupts();
      enableIRQinterrupts();
      uVar3 = uVar1;
    } while (*(short *)((int)param_1 + 6) != 0);
  }
LAB_08002ed8:
  disableIRQinterrupts();
  *(short *)((int)param_1 + 6) = *(short *)((int)param_1 + 6) - (short)(param_3 - uVar3);
  enableIRQinterrupts();
  return param_3 - uVar3;
}


/* ==== FUN_08002f38 @ 0x8002f38 ==== */

void FUN_08002f38(undefined4 *param_1)

{
  disableIRQinterrupts();
  param_1[2] = *param_1;
  param_1[3] = *param_1;
  *(undefined2 *)((int)param_1 + 6) = 0;
  enableIRQinterrupts();
  return;
}


/* ==== FUN_08002F48 @ 0x8002f48 ==== */

void FUN_08002F48(int param_1)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  uint uVar7;
  undefined4 uVar8;
  char cVar9;
  
  iVar2 = FUN_080002AC(param_1,0);
  if (iVar2 == 0) {
    cVar9 = '\x01';
  }
  else {
    param_1 = param_1 + -0x80000000;
    cVar9 = -1;
  }
  iVar2 = FUN_080002E8(param_1,0x40c90fdb);
  if (iVar2 != 0) {
    FUN_08000818(param_1,0x40c90fdb);
    FUN_08001208();
    uVar3 = FUN_08001248();
    uVar3 = FUN_08000BFC(uVar3,0x40c90fdb);
    param_1 = FUN_08000E5C(param_1,uVar3);
  }
  iVar2 = FUN_080002D4(param_1,0x40490fdb);
  if (iVar2 != 0) {
    cVar9 = -cVar9;
    param_1 = FUN_08000E5C(param_1,0x40490fdb);
  }
  iVar2 = FUN_080002D4(param_1,0x3fc90fdb);
  if (iVar2 != 0) {
    param_1 = FUN_08000E5C(0x40490fdb,param_1);
  }
  uVar3 = FUN_08000BFC(param_1,0x41f80000);
  uVar3 = FUN_08000818(uVar3,0x3fc90fdb);
  bVar1 = FUN_08000380();
  uVar7 = (uint)bVar1;
  uVar8 = *(undefined4 *)(&DAT_0800c37c + uVar7 * 4);
  uVar4 = FUN_08001248((int)cVar9);
  uVar5 = FUN_08000E5C(*(undefined4 *)(&DAT_0800c37c + (uVar7 + 1) * 4),uVar8);
  uVar6 = FUN_08001248(uVar7);
  uVar3 = FUN_08000E5C(uVar3,uVar6);
  uVar3 = FUN_08000BFC(uVar5,uVar3);
  uVar3 = FUN_08000474(uVar8,uVar3);
  FUN_08000BFC(uVar4,uVar3);
  return;
}


/* ==== FUN_08003040 @ 0x8003040 ==== */

void FUN_08003040(void)

{
  FUN_0800A4DC(0x20000f30);
  return;
}


/* ==== FUN_08003050 @ 0x8003050 ==== */

undefined8 FUN_08003050(void)

{
  undefined8 local_10;
  
  FUN_0800A558(0x20000f30,&local_10);
  return local_10;
}


/* ==== FUN_0800306C @ 0x800306c ==== */

void FUN_0800306C(void)

{
  if (-1 < (int)((uint)DAT_200002a0 << 0x1e)) {
    FUN_080079A0();
    DAT_200002a0 = 2;
  }
  return;
}


/* ==== FUN_08003088 @ 0x8003088 ==== */

void FUN_08003088(void)

{
  if ((DAT_200002a0 & 6) == 2) {
    FUN_08007B00();
    DAT_200002a0 = DAT_200002a0 | 4;
  }
  return;
}


/* ==== FUN_080030AC @ 0x80030ac ==== */

undefined1 FUN_080030AC(void)

{
  undefined1 uVar1;
  int iVar2;
  undefined4 extraout_r2;
  undefined4 in_r3;
  
  iVar2 = FUN_0800705c();
  uVar1 = FUN_080001c4((uint)*(ushort *)(iVar2 + 2) * 100 + -610000,0x8e8,extraout_r2,
                       (uint)*(ushort *)(iVar2 + 2),in_r3);
  return uVar1;
}


/* ==== FUN_080030D0 @ 0x80030d0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080030D0(void)

{
  int iVar1;
  undefined4 uVar2;
  
  FUN_0800A558(0x200002a8,&DAT_20000f38);
  if ((_DAT_20000f3c != 0) || (500 < _DAT_20000f38)) {
    FUN_0800705c();
    iVar1 = FUN_08006E24();
    if (iVar1 << 0x1b < 0) {
      iVar1 = FUN_08006124();
      if (iVar1 != 0x103) {
        FUN_08005FBC(0x103);
      }
      iVar1 = FUN_08006124();
      if (iVar1 == 0x103) {
        iVar1 = FUN_0800705c();
        uVar2 = FUN_080001c4(*(short *)(iVar1 + 4) * 100,0x76c);
        FUN_08005EC4(0x103,1,uVar2);
        uVar2 = FUN_080030AC();
        FUN_08005EC4(0x103,0,uVar2);
      }
    }
    else {
      iVar1 = FUN_08006124();
      if (iVar1 == 0x103) {
        FUN_080060DC(0x103);
      }
    }
    FUN_08006E5C();
    FUN_0800A4DC(0x200002a8);
  }
  FUN_08006F0C();
  return;
}


/* ==== FUN_08003178 @ 0x8003178 ==== */

undefined4 FUN_08003178(undefined4 param_1)

{
  FUN_080030AC();
  FUN_08006848(param_1,1);
  return 0;
}


/* ==== FUN_08003198 @ 0x8003198 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08003198(void)

{
  if (-1 < (int)((uint)DAT_20000354 << 0x1e)) {
    FUN_08008C48();
    FUN_080087D8();
    _DAT_200002e0 = 0x41700000;
    _DAT_200002e4 = 0x41c80000;
    _DAT_200002e8 = 0x40a00000;
    _DAT_200002ec = 0x41f00000;
    _DAT_200002f8 = 0xc47a0000;
    _DAT_200002fc = 0x447a0000;
    _DAT_20000300 = 0x3f99999a;
    _DAT_20000308 = 1;
    _DAT_2000030c = 0x41700000;
    _DAT_20000310 = 0x41c80000;
    _DAT_20000314 = 0x40a00000;
    _DAT_20000318 = 0x41f00000;
    _DAT_2000031c = _DAT_200002f0;
    _DAT_20000320 = _DAT_200002f4;
    _DAT_20000324 = 0xc47a0000;
    _DAT_20000328 = 0x447a0000;
    _DAT_2000032c = 0x3f99999a;
    _DAT_20000330 = _DAT_20000304;
    _DAT_20000334 = 1;
    FUN_08006A84(&DAT_200002e0,0,&DAT_2000030c,&DAT_20000338);
    FUN_08006A84(&DAT_2000030c,0);
    _DAT_200002c0 = 0x42a00000;
    DAT_200002b4 = 2;
    DAT_20000354 = 2;
  }
  return;
}


/* ==== FUN_08003234 @ 0x8003234 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08003234(void)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  uVar1 = _DAT_200002d0;
  uVar2 = _DAT_200002d8;
  if ((DAT_20000354 & 6) == 2) {
    FUN_08008DBC();
    FUN_08008910();
    _DAT_200002d0 = FUN_08008E00(1);
    uVar2 = FUN_08008E00(2);
    DAT_20000354 = DAT_20000354 | 4;
    uVar1 = _DAT_200002d0;
  }
  _DAT_200002dc = (undefined4)((ulonglong)uVar2 >> 0x20);
  _DAT_200002d8 = (undefined4)uVar2;
  _DAT_200002d4 = (undefined4)((ulonglong)uVar1 >> 0x20);
  _DAT_200002d0 = (undefined4)uVar1;
  return;
}


/* ==== FUN_08003274 @ 0x8003274 ==== */

void FUN_08003274(void)

{
  FUN_08008980();
  DAT_20000354 = DAT_20000354 & 0xfb;
  return;
}


/* ==== FUN_0800328C @ 0x800328c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800328C(char param_1,int param_2)

{
  int iVar1;
  
  if ('d' < param_1) {
    param_1 = 'd';
  }
  if (param_1 < -100) {
    param_1 = -100;
  }
  iVar1 = (int)param_1;
  if (param_2 << 0x1f < 0) {
    _DAT_200002f4 = FUN_08001248(iVar1);
    if (_DAT_200002b8 == 0) {
      if (iVar1 == 0) {
        FUN_0800A4DC(0x20000340);
        FUN_08008BD4(0,1);
      }
      else {
        FUN_08008BD4(0,1);
      }
    }
    else {
      _DAT_200002f4 = FUN_08000818(_DAT_200002f4,0x40a00000);
    }
  }
  if (param_2 << 0x1e < 0) {
    _DAT_20000320 = FUN_08001248(iVar1);
    if (_DAT_200002b8 == 0) {
      if (iVar1 == 0) {
        FUN_0800A4DC(0x20000348);
        FUN_08008BD4(0,2);
      }
      else {
        FUN_08008BD4(0,2);
      }
    }
    else {
      _DAT_20000320 = FUN_08000818(_DAT_20000320,0x40a00000);
    }
  }
  DAT_200002bc = param_1;
  return;
}


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


/* ==== FUN_080036F8 @ 0x80036f8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_080036F8(undefined4 *param_1,ushort *param_2)

{
  FUN_08003040();
  FUN_0800328C((int)(char)param_2[1],1);
  FUN_0800328C((int)*(char *)((int)param_2 + 3),2);
  DAT_200002b4 = 1;
  _DAT_200002b0 = 1;
  if (10 < *param_2) {
    _DAT_200002b0 = *param_2 - 10;
  }
  _DAT_200002b2 = _DAT_200002b0;
  _DAT_200002d0 = FUN_08008E00(1);
  _DAT_200002d8 = FUN_08008E00(2);
  if (_DAT_20000350 != 0) {
    FUN_08006828(_DAT_20000350,1);
  }
  _DAT_20000350 = *param_1;
  *param_1 = 0;
  return 0;
}


/* ==== FUN_0800376C @ 0x800376c ==== */

undefined4 FUN_0800376C(undefined4 param_1,char *param_2)

{
  FUN_08003040();
  FUN_0800328C((int)*param_2,1);
  FUN_0800328C((int)param_2[1],2);
  return 0;
}


/* ==== FUN_0800378C @ 0x800378c ==== */

undefined4 FUN_0800378C(undefined4 param_1,char *param_2)

{
  FUN_08003040();
  FUN_0800328C((int)*param_2,1);
  return 0;
}


/* ==== FUN_080037A4 @ 0x80037a4 ==== */

undefined4 FUN_080037A4(undefined4 param_1,char *param_2)

{
  FUN_08003040();
  FUN_0800328C((int)*param_2,2);
  return 0;
}


/* ==== FUN_080037BC @ 0x80037bc ==== */

void FUN_080037BC(undefined4 param_1,undefined2 *param_2)

{
  char cVar1;
  undefined4 uVar2;
  undefined2 local_14;
  char local_12;
  char local_11;
  
  FUN_08003040();
  cVar1 = *(char *)(param_2 + 1);
  if (cVar1 < 0) {
    local_11 = FUN_080001c4(cVar1 * 0x2d,100);
    local_11 = local_11 + -0x37;
  }
  else {
    local_11 = FUN_080001c4(cVar1 * 0x2d,100);
    local_11 = local_11 + '7';
  }
  local_12 = -local_11;
  uVar2 = FUN_080012F0(*param_2);
  uVar2 = FUN_08000818(uVar2,0x42b40000);
  uVar2 = FUN_08000BFC(uVar2,0x40490fdb);
  uVar2 = FUN_08000BFC(uVar2,0x42140000);
  FUN_08000BFC(uVar2,0x3f8ccccd);
  local_14 = FUN_08000380();
  FUN_080036F8(param_1,&local_14);
  return;
}


/* ==== FUN_08003848 @ 0x8003848 ==== */

undefined4 FUN_08003848(void)

{
  FUN_08003040();
  return 0;
}


/* ==== FUN_08003854 @ 0x8003854 ==== */

undefined4 FUN_08003854(void)

{
  FUN_08003040();
  return 0;
}


/* ==== FUN_08003860 @ 0x8003860 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08003860(void)

{
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 0x50;
  }
  if (_DAT_20000358 == 0xb) {
    DAT_20000386 = 0;
    DAT_20000385 = 0;
    DAT_20000387 = 0;
  }
  if (0xb - _DAT_20000358 < 2) {
    DAT_20000389 = 0;
    DAT_20000388 = 0;
    DAT_2000038a = 0;
  }
  if (0xb - _DAT_20000358 < 3) {
    DAT_2000038c = 0;
    DAT_2000038b = 0;
    DAT_2000038d = 0;
  }
  if (0xb - _DAT_20000358 < 4) {
    DAT_2000038f = 0;
    DAT_2000038e = 0;
    DAT_20000390 = 0;
  }
  if (0xb - _DAT_20000358 < 5) {
    DAT_20000392 = 0;
    DAT_20000391 = 0;
    DAT_20000393 = 0;
  }
  if (0xb - _DAT_20000358 < 6) {
    DAT_20000395 = 0;
    DAT_20000394 = 0;
    DAT_20000396 = 0;
  }
  if (0xb - _DAT_20000358 < 7) {
    DAT_20000398 = 0;
    DAT_20000397 = 0;
    DAT_20000399 = 0;
  }
  if (0xb - _DAT_20000358 < 8) {
    DAT_2000039b = 0;
    DAT_2000039a = 0;
    DAT_2000039c = 0;
  }
  if (0xb - _DAT_20000358 < 9) {
    DAT_2000039e = 0;
    DAT_2000039d = 0;
    DAT_2000039f = 0;
  }
  if (0xb - _DAT_20000358 < 10) {
    DAT_200003a1 = 0;
    DAT_200003a0 = 0;
    DAT_200003a2 = 0;
  }
  if (0xb - _DAT_20000358 < 0xb) {
    DAT_200003a4 = 0;
    DAT_200003a3 = 0;
    DAT_200003a5 = 0;
  }
  if (0xb - _DAT_20000358 < 0xc) {
    DAT_200003a7 = 0;
    DAT_200003a6 = 0;
    DAT_200003a8 = 0;
  }
  return _DAT_20000358 < 0xb;
}


/* ==== FUN_080039D8 @ 0x80039d8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_080039D8(void)

{
  int iVar1;
  byte bVar2;
  char cVar3;
  char cVar4;
  uint uVar5;
  uint extraout_r1;
  uint extraout_r1_00;
  uint uVar6;
  uint extraout_r1_01;
  uint extraout_r1_02;
  uint extraout_r1_03;
  uint extraout_r1_04;
  uint extraout_r1_05;
  uint extraout_r1_06;
  uint extraout_r1_07;
  uint extraout_r1_08;
  uint extraout_r1_09;
  uint extraout_r1_10;
  uint extraout_r1_11;
  
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 100;
    _DAT_20000364 = 0;
    _DAT_20000368 = 0;
  }
  iVar1 = _DAT_20000364;
  bVar2 = FUN_08000128(_DAT_20000364 * 0xc,100);
  _DAT_2000035c = 100 - (short)(_DAT_20000368 >> 1);
  cVar3 = FUN_08000128(iVar1 * -0x16 + 0x898,100);
  cVar4 = FUN_08000128(iVar1 * 0x14,100);
  FUN_080001b0(_DAT_20000358,0xc);
  uVar5 = extraout_r1 & 0xff;
  if (bVar2 == 0) {
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_00) {
      if (uVar5 == 0) {
        iVar1 = (extraout_r1 & 0xff) * 3;
        (&DAT_20000387)[iVar1] = 6;
        (&DAT_20000385)[iVar1] = 6;
        (&DAT_20000386)[iVar1] = 6;
      }
      else {
        uVar6 = extraout_r1 & 0xff;
        if (uVar6 == 0xffffffff) {
          DAT_20000387 = 0xc;
          DAT_20000385 = '\f';
          DAT_20000386 = '\f';
        }
        else if (uVar6 == 0xfffffffe) {
          DAT_20000387 = 7;
          DAT_20000385 = '\a';
          DAT_20000386 = '\a';
        }
        else if (uVar6 == 0xfffffffd) {
          DAT_20000387 = 5;
          DAT_20000385 = '\x05';
          DAT_20000386 = '\x05';
        }
        else {
          DAT_20000387 = 4;
          DAT_20000385 = '\x04';
          DAT_20000386 = '\x04';
        }
      }
      goto LAB_08003b08;
    }
    DAT_20000386 = cVar3 + '\x02';
    DAT_20000385 = cVar4 + '\x02';
    DAT_20000387 = 6;
LAB_08003b0e:
    if (uVar5 == 1) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 0) {
        DAT_2000038a = 0xc;
        DAT_20000388 = '\f';
        DAT_20000389 = '\f';
      }
      else if (uVar6 == 0xffffffff) {
        DAT_2000038a = 7;
        DAT_20000388 = '\a';
        DAT_20000389 = '\a';
      }
      else if (uVar6 == 0xfffffffe) {
        DAT_2000038a = 5;
        DAT_20000388 = '\x05';
        DAT_20000389 = '\x05';
      }
      else {
        DAT_2000038a = 4;
        DAT_20000388 = '\x04';
        DAT_20000389 = '\x04';
      }
    }
LAB_08003bd0:
    if (bVar2 != 2) {
      if (bVar2 < 3) goto LAB_08003bd6;
      DAT_2000038d = 4;
      DAT_2000038b = cVar4;
      DAT_2000038c = cVar3;
      goto LAB_08003c9a;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_02) goto LAB_08003bd6;
    DAT_2000038c = cVar3 + '\x02';
    DAT_2000038b = cVar4 + '\x02';
    DAT_2000038d = 6;
LAB_08003ca0:
    if (uVar5 == 3) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 2) {
        DAT_20000390 = 0xc;
        DAT_2000038e = '\f';
        DAT_2000038f = '\f';
      }
      else if (uVar6 == 1) {
        DAT_20000390 = 7;
        DAT_2000038e = '\a';
        DAT_2000038f = '\a';
      }
      else if (uVar6 == 0) {
        DAT_20000390 = 5;
        DAT_2000038e = '\x05';
        DAT_2000038f = '\x05';
      }
      else {
        DAT_20000390 = 4;
        DAT_2000038e = '\x04';
        DAT_2000038f = '\x04';
      }
    }
LAB_08003d6c:
    if (bVar2 != 4) {
      if (bVar2 < 5) goto LAB_08003d72;
      DAT_20000393 = 4;
      DAT_20000391 = cVar4;
      DAT_20000392 = cVar3;
      goto LAB_08003e36;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_04) goto LAB_08003d72;
    DAT_20000392 = cVar3 + '\x02';
    DAT_20000391 = cVar4 + '\x02';
    DAT_20000393 = 6;
LAB_08003e3c:
    if (uVar5 == 5) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 4) {
        DAT_20000396 = 0xc;
        DAT_20000394 = '\f';
        DAT_20000395 = '\f';
      }
      else if (uVar6 == 3) {
        DAT_20000396 = 7;
        DAT_20000394 = '\a';
        DAT_20000395 = '\a';
      }
      else if (uVar6 == 2) {
        DAT_20000396 = 5;
        DAT_20000394 = '\x05';
        DAT_20000395 = '\x05';
      }
      else {
        DAT_20000396 = 4;
        DAT_20000394 = '\x04';
        DAT_20000395 = '\x04';
      }
    }
LAB_08003f00:
    if (bVar2 != 6) {
      if (bVar2 < 7) goto LAB_08003f06;
      DAT_20000399 = 4;
      DAT_20000397 = cVar4;
      DAT_20000398 = cVar3;
      goto LAB_08003fca;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_06) goto LAB_08003f06;
    DAT_20000398 = cVar3 + '\x02';
    DAT_20000397 = cVar4 + '\x02';
    DAT_20000399 = 6;
LAB_08003fd0:
    if (uVar5 == 7) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 6) {
        DAT_2000039c = 0xc;
        DAT_2000039a = '\f';
        DAT_2000039b = '\f';
      }
      else if (uVar6 == 5) {
        DAT_2000039c = 7;
        DAT_2000039a = '\a';
        DAT_2000039b = '\a';
      }
      else if (uVar6 == 4) {
        DAT_2000039c = 5;
        DAT_2000039a = '\x05';
        DAT_2000039b = '\x05';
      }
      else {
        DAT_2000039c = 4;
        DAT_2000039a = '\x04';
        DAT_2000039b = '\x04';
      }
    }
LAB_0800409a:
    if (bVar2 != 8) {
      if (bVar2 < 9) goto LAB_080040a0;
      DAT_2000039f = 4;
      DAT_2000039d = cVar4;
      DAT_2000039e = cVar3;
      goto LAB_08004164;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_08) goto LAB_080040a0;
    DAT_2000039e = cVar3 + '\x02';
    DAT_2000039d = cVar4 + '\x02';
    DAT_2000039f = 6;
LAB_0800416a:
    if (uVar5 == 9) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 8) {
        DAT_200003a2 = 0xc;
        DAT_200003a0 = '\f';
        DAT_200003a1 = '\f';
      }
      else if (uVar6 == 7) {
        DAT_200003a2 = 7;
        DAT_200003a0 = '\a';
        DAT_200003a1 = '\a';
      }
      else if (uVar6 == 6) {
        DAT_200003a2 = 5;
        DAT_200003a0 = '\x05';
        DAT_200003a1 = '\x05';
      }
      else {
        DAT_200003a2 = 4;
        DAT_200003a0 = '\x04';
        DAT_200003a1 = '\x04';
      }
    }
LAB_0800422e:
    if (bVar2 == 10) {
      FUN_080001b0(_DAT_20000358,10);
      if (extraout_r1_10 < 5) {
        DAT_200003a4 = cVar3 + '\x02';
        DAT_200003a3 = cVar4 + '\x02';
        DAT_200003a5 = 6;
        goto LAB_08004344;
      }
      goto LAB_08004234;
    }
    if (bVar2 < 0xb) goto LAB_08004234;
    DAT_200003a5 = 4;
    DAT_200003a3 = cVar4;
    DAT_200003a4 = cVar3;
  }
  else {
    DAT_20000387 = 4;
    DAT_20000385 = cVar4;
    DAT_20000386 = cVar3;
LAB_08003b08:
    if (bVar2 != 1) {
      if (bVar2 < 2) goto LAB_08003b0e;
      DAT_2000038a = 4;
      DAT_20000388 = cVar4;
      DAT_20000389 = cVar3;
      goto LAB_08003bd0;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_01) goto LAB_08003b0e;
    DAT_20000389 = cVar3 + '\x02';
    DAT_20000388 = cVar4 + '\x02';
    DAT_2000038a = 6;
LAB_08003bd6:
    if (uVar5 == 2) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 1) {
        DAT_2000038d = 0xc;
        DAT_2000038b = '\f';
        DAT_2000038c = '\f';
      }
      else if (uVar6 == 0) {
        DAT_2000038d = 7;
        DAT_2000038b = '\a';
        DAT_2000038c = '\a';
      }
      else if (uVar6 == 0xffffffff) {
        DAT_2000038d = 5;
        DAT_2000038b = '\x05';
        DAT_2000038c = '\x05';
      }
      else {
        DAT_2000038d = 4;
        DAT_2000038b = '\x04';
        DAT_2000038c = '\x04';
      }
    }
LAB_08003c9a:
    if (bVar2 != 3) {
      if (bVar2 < 4) goto LAB_08003ca0;
      DAT_20000390 = 4;
      DAT_2000038e = cVar4;
      DAT_2000038f = cVar3;
      goto LAB_08003d6c;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_03) goto LAB_08003ca0;
    DAT_2000038f = cVar3 + '\x02';
    DAT_2000038e = cVar4 + '\x02';
    DAT_20000390 = 6;
LAB_08003d72:
    if (uVar5 == 4) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 3) {
        DAT_20000393 = 0xc;
        DAT_20000391 = '\f';
        DAT_20000392 = '\f';
      }
      else if (uVar6 == 2) {
        DAT_20000393 = 7;
        DAT_20000391 = '\a';
        DAT_20000392 = '\a';
      }
      else if (uVar6 == 1) {
        DAT_20000393 = 5;
        DAT_20000391 = '\x05';
        DAT_20000392 = '\x05';
      }
      else {
        DAT_20000393 = 4;
        DAT_20000391 = '\x04';
        DAT_20000392 = '\x04';
      }
    }
LAB_08003e36:
    if (bVar2 != 5) {
      if (bVar2 < 6) goto LAB_08003e3c;
      DAT_20000396 = 4;
      DAT_20000394 = cVar4;
      DAT_20000395 = cVar3;
      goto LAB_08003f00;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_05) goto LAB_08003e3c;
    DAT_20000395 = cVar3 + '\x02';
    DAT_20000394 = cVar4 + '\x02';
    DAT_20000396 = 6;
LAB_08003f06:
    if (uVar5 == 6) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 5) {
        DAT_20000399 = 0xc;
        DAT_20000397 = '\f';
        DAT_20000398 = '\f';
      }
      else if (uVar6 == 4) {
        DAT_20000399 = 7;
        DAT_20000397 = '\a';
        DAT_20000398 = '\a';
      }
      else if (uVar6 == 3) {
        DAT_20000399 = 5;
        DAT_20000397 = '\x05';
        DAT_20000398 = '\x05';
      }
      else {
        DAT_20000399 = 4;
        DAT_20000397 = '\x04';
        DAT_20000398 = '\x04';
      }
    }
LAB_08003fca:
    if (bVar2 != 7) {
      if (bVar2 < 8) goto LAB_08003fd0;
      DAT_2000039c = 4;
      DAT_2000039a = cVar4;
      DAT_2000039b = cVar3;
      goto LAB_0800409a;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_07) goto LAB_08003fd0;
    DAT_2000039b = cVar3 + '\x02';
    DAT_2000039a = cVar4 + '\x02';
    DAT_2000039c = 6;
LAB_080040a0:
    if (uVar5 == 8) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 7) {
        DAT_2000039f = 0xc;
        DAT_2000039d = '\f';
        DAT_2000039e = '\f';
      }
      else if (uVar6 == 6) {
        DAT_2000039f = 7;
        DAT_2000039d = '\a';
        DAT_2000039e = '\a';
      }
      else if (uVar6 == 5) {
        DAT_2000039f = 5;
        DAT_2000039d = '\x05';
        DAT_2000039e = '\x05';
      }
      else {
        DAT_2000039f = 4;
        DAT_2000039d = '\x04';
        DAT_2000039e = '\x04';
      }
    }
LAB_08004164:
    if (bVar2 != 9) {
      if (bVar2 < 10) goto LAB_0800416a;
      DAT_200003a2 = 4;
      DAT_200003a0 = cVar4;
      DAT_200003a1 = cVar3;
      goto LAB_0800422e;
    }
    FUN_080001b0(_DAT_20000358,10);
    if (4 < extraout_r1_09) goto LAB_0800416a;
    DAT_200003a1 = cVar3 + '\x02';
    DAT_200003a0 = cVar4 + '\x02';
    DAT_200003a2 = 6;
LAB_08004234:
    if (uVar5 == 10) {
      iVar1 = (extraout_r1 & 0xff) * 3;
      (&DAT_20000387)[iVar1] = 6;
      (&DAT_20000385)[iVar1] = 6;
      (&DAT_20000386)[iVar1] = 6;
    }
    else {
      uVar6 = extraout_r1 & 0xff;
      if (uVar6 == 9) {
        DAT_200003a5 = 0xc;
        DAT_200003a3 = '\f';
        DAT_200003a4 = '\f';
      }
      else if (uVar6 == 8) {
        DAT_200003a5 = 7;
        DAT_200003a3 = '\a';
        DAT_200003a4 = '\a';
      }
      else if (uVar6 == 7) {
        DAT_200003a5 = 5;
        DAT_200003a3 = '\x05';
        DAT_200003a4 = '\x05';
      }
      else {
        DAT_200003a5 = 4;
        DAT_200003a3 = '\x04';
        DAT_200003a4 = '\x04';
      }
    }
  }
  if (bVar2 == 0xb) {
    FUN_080001b0(_DAT_20000358,10);
    if (extraout_r1_11 < 5) {
      DAT_200003a6 = cVar4 + '\x02';
      DAT_200003a7 = cVar3 + '\x02';
      DAT_200003a8 = 6;
      return 1;
    }
  }
  else if (0xb < bVar2) {
    DAT_200003a6 = cVar4;
    DAT_200003a7 = cVar3;
    DAT_200003a8 = 4;
    return 1;
  }
LAB_08004344:
  if (uVar5 == 0xb) {
    iVar1 = (extraout_r1 & 0xff) * 3;
    (&DAT_20000387)[iVar1] = 6;
    (&DAT_20000385)[iVar1] = 6;
    (&DAT_20000386)[iVar1] = 6;
  }
  else {
    uVar5 = extraout_r1 & 0xff;
    if (uVar5 == 10) {
      DAT_200003a8 = 0xc;
      DAT_200003a6 = 0xc;
      DAT_200003a7 = 0xc;
    }
    else if (uVar5 == 9) {
      DAT_200003a8 = 7;
      DAT_200003a6 = 7;
      DAT_200003a7 = 7;
    }
    else if (uVar5 == 8) {
      DAT_200003a8 = 5;
      DAT_200003a6 = 5;
      DAT_200003a7 = 5;
    }
    else {
      DAT_200003a8 = 4;
      DAT_200003a6 = 4;
      DAT_200003a7 = 4;
    }
  }
  return 1;
}


/* ==== FUN_080043D4 @ 0x80043d4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_080043D4(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  
  iVar2 = _DAT_20000364;
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 10;
    _DAT_20000364 = 0;
    cVar1 = '\0';
    FUN_08000128(0,100,param_3,0,param_4);
    iVar2 = -1;
LAB_0800441e:
    DAT_20000387 = '\0';
LAB_0800445e:
    DAT_20000385 = DAT_20000387;
    DAT_20000386 = DAT_20000387;
    if (iVar2 != 1) {
      if (iVar2 < 2) goto LAB_08004464;
      DAT_2000038a = cVar1 + '\b';
      goto LAB_080044a4;
    }
    DAT_20000389 = cVar1 + '\f';
    DAT_20000388 = cVar1 + '\x03';
    DAT_2000038a = cVar1 + '\x02';
LAB_080044aa:
    DAT_2000038d = '\0';
LAB_080044ea:
    DAT_2000038b = DAT_2000038d;
    DAT_2000038c = DAT_2000038d;
    if (iVar2 != 3) {
      if (iVar2 < 4) goto LAB_080044f0;
      DAT_20000390 = cVar1 + '\b';
      goto LAB_08004530;
    }
    DAT_2000038f = cVar1 + '\f';
    DAT_2000038e = cVar1 + '\x03';
    DAT_20000390 = cVar1 + '\x02';
LAB_08004536:
    DAT_20000393 = '\0';
LAB_08004576:
    DAT_20000391 = DAT_20000393;
    DAT_20000392 = DAT_20000393;
    if (iVar2 != 5) {
      if (iVar2 < 6) goto LAB_0800457c;
      DAT_20000396 = cVar1 + '\b';
      goto LAB_080045bc;
    }
    DAT_20000395 = cVar1 + '\f';
    DAT_20000394 = cVar1 + '\x03';
    DAT_20000396 = cVar1 + '\x02';
LAB_080045c2:
    DAT_20000399 = '\0';
LAB_08004602:
    DAT_20000397 = DAT_20000399;
    DAT_20000398 = DAT_20000399;
    if (iVar2 != 7) {
      if (iVar2 < 8) goto LAB_08004608;
      DAT_2000039c = cVar1 + '\b';
      goto LAB_08004648;
    }
    DAT_2000039b = cVar1 + '\f';
    DAT_2000039a = cVar1 + '\x03';
    DAT_2000039c = cVar1 + '\x02';
LAB_0800464e:
    DAT_2000039f = '\0';
LAB_0800468e:
    DAT_2000039d = DAT_2000039f;
    DAT_2000039e = DAT_2000039f;
    if (iVar2 != 9) {
      if (iVar2 < 10) goto LAB_08004694;
      DAT_200003a2 = cVar1 + '\b';
      goto LAB_080046d4;
    }
    DAT_200003a1 = cVar1 + '\f';
    DAT_200003a0 = cVar1 + '\x03';
    DAT_200003a2 = cVar1 + '\x02';
  }
  else {
    cVar1 = FUN_08000128(_DAT_20000364 * 0xf,100,_DAT_20000364 * 0x10,&DAT_20000358,param_4);
    uVar3 = FUN_08000128(iVar2 * 0xc,100);
    iVar2 = (uVar3 & 0xff) - 1;
    if (iVar2 != 0) {
      if (iVar2 < 1) goto LAB_0800441e;
      DAT_20000387 = cVar1 + '\b';
      goto LAB_0800445e;
    }
    DAT_20000386 = cVar1 + '\f';
    DAT_20000385 = cVar1 + '\x03';
    DAT_20000387 = cVar1 + '\x02';
LAB_08004464:
    DAT_2000038a = '\0';
LAB_080044a4:
    DAT_20000388 = DAT_2000038a;
    DAT_20000389 = DAT_2000038a;
    if (iVar2 != 2) {
      if (iVar2 < 3) goto LAB_080044aa;
      DAT_2000038d = cVar1 + '\b';
      goto LAB_080044ea;
    }
    DAT_2000038c = cVar1 + '\f';
    DAT_2000038b = cVar1 + '\x03';
    DAT_2000038d = cVar1 + '\x02';
LAB_080044f0:
    DAT_20000390 = '\0';
LAB_08004530:
    DAT_2000038e = DAT_20000390;
    DAT_2000038f = DAT_20000390;
    if (iVar2 != 4) {
      if (iVar2 < 5) goto LAB_08004536;
      DAT_20000393 = cVar1 + '\b';
      goto LAB_08004576;
    }
    DAT_20000392 = cVar1 + '\f';
    DAT_20000391 = cVar1 + '\x03';
    DAT_20000393 = cVar1 + '\x02';
LAB_0800457c:
    DAT_20000396 = '\0';
LAB_080045bc:
    DAT_20000394 = DAT_20000396;
    DAT_20000395 = DAT_20000396;
    if (iVar2 != 6) {
      if (iVar2 < 7) goto LAB_080045c2;
      DAT_20000399 = cVar1 + '\b';
      goto LAB_08004602;
    }
    DAT_20000398 = cVar1 + '\f';
    DAT_20000397 = cVar1 + '\x03';
    DAT_20000399 = cVar1 + '\x02';
LAB_08004608:
    DAT_2000039c = '\0';
LAB_08004648:
    DAT_2000039a = DAT_2000039c;
    DAT_2000039b = DAT_2000039c;
    if (iVar2 != 8) {
      if (iVar2 < 9) goto LAB_0800464e;
      DAT_2000039f = cVar1 + '\b';
      goto LAB_0800468e;
    }
    DAT_2000039e = cVar1 + '\f';
    DAT_2000039d = cVar1 + '\x03';
    DAT_2000039f = cVar1 + '\x02';
LAB_08004694:
    DAT_200003a2 = '\0';
LAB_080046d4:
    if (iVar2 == 10) {
      DAT_200003a0 = DAT_200003a2;
      DAT_200003a1 = DAT_200003a2;
      DAT_200003a3 = cVar1 + '\x03';
      DAT_200003a4 = cVar1 + '\f';
      DAT_200003a5 = cVar1 + '\x02';
      DAT_200003a6 = 0;
      DAT_200003a7 = 0;
      DAT_200003a8 = 0;
      return 1;
    }
    DAT_200003a0 = DAT_200003a2;
    DAT_200003a1 = DAT_200003a2;
    if (10 < iVar2) {
      DAT_200003a5 = cVar1 + '\b';
      goto LAB_08004720;
    }
  }
  DAT_200003a5 = '\0';
LAB_08004720:
  if (iVar2 == 0xb) {
    DAT_200003a3 = DAT_200003a5;
    DAT_200003a4 = DAT_200003a5;
    DAT_200003a6 = cVar1 + '\x03';
    DAT_200003a7 = cVar1 + '\f';
    DAT_200003a8 = cVar1 + '\x02';
    return 1;
  }
  if (iVar2 < 0xc) {
    DAT_200003a8 = 0;
    DAT_200003a7 = 0;
    DAT_200003a6 = 0;
    DAT_200003a4 = DAT_200003a5;
    DAT_200003a3 = DAT_200003a5;
    return 1;
  }
  DAT_200003a6 = cVar1 + '\b';
  DAT_200003a3 = DAT_200003a5;
  DAT_200003a4 = DAT_200003a5;
  DAT_200003a7 = DAT_200003a6;
  DAT_200003a8 = DAT_200003a6;
  return 1;
}


/* ==== FUN_0800476C @ 0x800476c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_0800476C(void)

{
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 10;
    _DAT_20000364 = 0;
  }
  else if (_DAT_20000364 != 0) {
    if (_DAT_20000364 == 1) {
      DAT_20000385 = 0;
      DAT_20000386 = 0xff;
      DAT_20000387 = 0;
      DAT_20000388 = 0;
      DAT_20000389 = 0xff;
      DAT_2000038a = 0;
      DAT_2000038b = 0;
      DAT_2000038c = 0xff;
      DAT_2000038d = 0;
      DAT_2000038e = 0;
      DAT_2000038f = 0xff;
      DAT_20000390 = 0;
      DAT_20000391 = 0;
      DAT_20000392 = 0xff;
      DAT_20000393 = 0;
      DAT_20000394 = 0;
      DAT_20000395 = 0xff;
      DAT_20000396 = 0;
      DAT_20000397 = 0;
      DAT_20000398 = 0xff;
      DAT_20000399 = 0;
      DAT_2000039a = 0;
      DAT_2000039b = 0xff;
      DAT_2000039c = 0;
      DAT_2000039d = 0;
      DAT_2000039e = 0xff;
      DAT_2000039f = 0;
      DAT_200003a0 = 0;
      DAT_200003a1 = 0xff;
      DAT_200003a2 = 0;
      DAT_200003a3 = 0;
      DAT_200003a4 = 0xff;
      DAT_200003a5 = 0;
      DAT_200003a6 = 0;
      DAT_200003a7 = 0xff;
      DAT_200003a8 = 0;
      return true;
    }
    if (_DAT_20000364 == 2) {
      DAT_20000385 = 0xff;
      DAT_20000386 = 0;
      DAT_20000387 = 0;
      DAT_20000388 = 0xff;
      DAT_20000389 = 0;
      DAT_2000038a = 0;
      DAT_2000038b = 0xff;
      DAT_2000038c = 0;
      DAT_2000038d = 0;
      DAT_2000038e = 0xff;
      DAT_2000038f = 0;
      DAT_20000390 = 0;
      DAT_20000391 = 0xff;
      DAT_20000392 = 0;
      DAT_20000393 = 0;
      DAT_20000394 = 0xff;
      DAT_20000395 = 0;
      DAT_20000396 = 0;
      DAT_20000397 = 0xff;
      DAT_20000398 = 0;
      DAT_20000399 = 0;
      DAT_2000039a = 0xff;
      DAT_2000039b = 0;
      DAT_2000039c = 0;
      DAT_2000039d = 0xff;
      DAT_2000039e = 0;
      DAT_2000039f = 0;
      DAT_200003a0 = 0xff;
      DAT_200003a1 = 0;
      DAT_200003a2 = 0;
      DAT_200003a3 = 0xff;
      DAT_200003a4 = 0;
      DAT_200003a5 = 0;
      DAT_200003a6 = 0xff;
      DAT_200003a7 = 0;
      DAT_200003a8 = 0;
      return true;
    }
    if (_DAT_20000364 == 3) {
      DAT_20000385 = 0;
      DAT_20000386 = 0;
      DAT_20000387 = 0xff;
      DAT_20000388 = 0;
      DAT_20000389 = 0;
      DAT_2000038a = 0xff;
      DAT_2000038b = 0;
      DAT_2000038c = 0;
      DAT_2000038d = 0xff;
      DAT_2000038e = 0;
      DAT_2000038f = 0;
      DAT_20000390 = 0xff;
      DAT_20000391 = 0;
      DAT_20000392 = 0;
      DAT_20000393 = 0xff;
      DAT_20000394 = 0;
      DAT_20000395 = 0;
      DAT_20000396 = 0xff;
      DAT_20000397 = 0;
      DAT_20000398 = 0;
      DAT_20000399 = 0xff;
      DAT_2000039a = 0;
      DAT_2000039b = 0;
      DAT_2000039c = 0xff;
      DAT_2000039d = 0;
      DAT_2000039e = 0;
      DAT_2000039f = 0xff;
      DAT_200003a0 = 0;
      DAT_200003a1 = 0;
      DAT_200003a2 = 0xff;
      DAT_200003a3 = 0;
      DAT_200003a4 = 0;
      DAT_200003a5 = 0xff;
      DAT_200003a6 = 0;
      DAT_200003a7 = 0;
      DAT_200003a8 = 0xff;
      return true;
    }
    return _DAT_20000364 != 4;
  }
  DAT_200003a8 = 0;
  DAT_200003a7 = 0;
  DAT_200003a6 = 0;
  DAT_200003a5 = 0;
  DAT_200003a4 = 0;
  DAT_200003a3 = 0;
  DAT_200003a2 = 0;
  DAT_200003a1 = 0;
  DAT_200003a0 = 0;
  DAT_2000039f = 0;
  DAT_2000039e = 0;
  DAT_2000039d = 0;
  DAT_2000039c = 0;
  DAT_2000039b = 0;
  DAT_2000039a = 0;
  DAT_20000399 = 0;
  DAT_20000398 = 0;
  DAT_20000397 = 0;
  DAT_20000396 = 0;
  DAT_20000395 = 0;
  DAT_20000394 = 0;
  DAT_20000393 = 0;
  DAT_20000392 = 0;
  DAT_20000391 = 0;
  DAT_20000390 = 0;
  DAT_2000038f = 0;
  DAT_2000038e = 0;
  DAT_2000038d = 0;
  DAT_2000038c = 0;
  DAT_2000038b = 0;
  DAT_2000038a = 0;
  DAT_20000389 = 0;
  DAT_20000388 = 0;
  DAT_20000387 = 0;
  DAT_20000386 = 0;
  DAT_20000385 = 0;
  return true;
}


/* ==== FUN_08004A08 @ 0x8004a08 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08004A08(void)

{
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 200;
  }
  DAT_200003a7 = DAT_20000380;
  DAT_200003a6 = DAT_2000037f;
  DAT_200003a8 = DAT_20000381;
  DAT_200003a4 = DAT_20000380;
  DAT_200003a3 = DAT_2000037f;
  DAT_200003a5 = DAT_20000381;
  DAT_200003a1 = DAT_20000380;
  DAT_200003a0 = DAT_2000037f;
  DAT_200003a2 = DAT_20000381;
  DAT_2000039e = DAT_20000380;
  DAT_2000039d = DAT_2000037f;
  DAT_2000039f = DAT_20000381;
  DAT_2000039b = DAT_20000380;
  DAT_2000039a = DAT_2000037f;
  DAT_2000039c = DAT_20000381;
  DAT_20000398 = DAT_20000380;
  DAT_20000397 = DAT_2000037f;
  DAT_20000399 = DAT_20000381;
  DAT_20000395 = DAT_20000380;
  DAT_20000394 = DAT_2000037f;
  DAT_20000396 = DAT_20000381;
  DAT_20000392 = DAT_20000380;
  DAT_20000391 = DAT_2000037f;
  DAT_20000393 = DAT_20000381;
  DAT_2000038f = DAT_20000380;
  DAT_2000038e = DAT_2000037f;
  DAT_20000390 = DAT_20000381;
  DAT_2000038c = DAT_20000380;
  DAT_2000038b = DAT_2000037f;
  DAT_2000038d = DAT_20000381;
  DAT_20000389 = DAT_20000380;
  DAT_20000388 = DAT_2000037f;
  DAT_2000038a = DAT_20000381;
  DAT_20000386 = DAT_20000380;
  DAT_20000385 = DAT_2000037f;
  DAT_20000387 = DAT_20000381;
  return 1;
}


/* ==== FUN_08004AC0 @ 0x8004ac0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08004AC0(void)

{
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 0xfa;
    _DAT_20000364 = 0;
  }
  DAT_200003a7 = DAT_20000380;
  DAT_200003a6 = DAT_2000037f;
  DAT_200003a8 = DAT_20000381;
  DAT_200003a4 = DAT_20000380;
  DAT_200003a3 = DAT_2000037f;
  DAT_200003a5 = DAT_20000381;
  DAT_200003a1 = DAT_20000380;
  DAT_200003a0 = DAT_2000037f;
  DAT_200003a2 = DAT_20000381;
  DAT_2000039e = DAT_20000380;
  DAT_2000039d = DAT_2000037f;
  DAT_2000039f = DAT_20000381;
  DAT_2000039b = DAT_20000380;
  DAT_2000039a = DAT_2000037f;
  DAT_2000039c = DAT_20000381;
  DAT_20000398 = DAT_20000380;
  DAT_20000397 = DAT_2000037f;
  DAT_20000399 = DAT_20000381;
  DAT_20000395 = DAT_20000380;
  DAT_20000394 = DAT_2000037f;
  DAT_20000396 = DAT_20000381;
  DAT_20000392 = DAT_20000380;
  DAT_20000391 = DAT_2000037f;
  DAT_20000393 = DAT_20000381;
  DAT_2000038f = DAT_20000380;
  DAT_2000038e = DAT_2000037f;
  DAT_20000390 = DAT_20000381;
  DAT_2000038c = DAT_20000380;
  DAT_2000038b = DAT_2000037f;
  DAT_2000038d = DAT_20000381;
  DAT_20000389 = DAT_20000380;
  DAT_20000388 = DAT_2000037f;
  DAT_2000038a = DAT_20000381;
  DAT_20000386 = DAT_20000380;
  DAT_20000385 = DAT_2000037f;
  DAT_20000387 = DAT_20000381;
  return 1;
}


/* ==== FUN_08004B7C @ 0x8004b7c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08004B7C(void)

{
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 1000;
  }
  DAT_200003a7 = 0;
  DAT_200003a6 = 0;
  DAT_200003a8 = 0;
  DAT_200003a4 = 0x10;
  if (_DAT_20000364 == 3) {
    DAT_200003a3 = 0;
  }
  else {
    DAT_200003a3 = 0x10;
  }
  DAT_200003a5 = 0;
  DAT_200003a1 = 0;
  DAT_200003a0 = 0;
  DAT_200003a2 = 0;
  DAT_2000039e = 0x10;
  if (_DAT_20000364 == 3) {
    DAT_2000039d = 0;
  }
  else {
    DAT_2000039d = 0x10;
  }
  DAT_2000039f = 0;
  DAT_2000039b = 0;
  DAT_2000039a = 0;
  DAT_2000039c = 0;
  DAT_20000398 = 0x10;
  if (_DAT_20000364 == 3) {
    DAT_20000397 = 0;
  }
  else {
    DAT_20000397 = 0x10;
  }
  DAT_20000399 = 0;
  DAT_20000395 = 0;
  DAT_20000394 = 0;
  DAT_20000396 = 0;
  DAT_20000392 = 0x10;
  if (_DAT_20000364 == 3) {
    DAT_20000391 = 0;
  }
  else {
    DAT_20000391 = 0x10;
  }
  DAT_20000393 = 0;
  DAT_2000038f = 0;
  DAT_2000038e = 0;
  DAT_20000390 = 0;
  DAT_2000038c = 0x10;
  if (_DAT_20000364 == 3) {
    DAT_2000038b = 0;
  }
  else {
    DAT_2000038b = 0x10;
  }
  DAT_2000038d = 0;
  DAT_20000389 = 0;
  DAT_20000388 = 0;
  DAT_2000038a = 0;
  DAT_20000386 = 0x10;
  if (_DAT_20000364 == 3) {
    DAT_20000385 = 0;
  }
  else {
    DAT_20000385 = 0x10;
  }
  DAT_20000387 = 0;
  return 1;
}


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


/* ==== FUN_08005848 @ 0x8005848 ==== */

void FUN_08005848(char *param_1,char *param_2)

{
  char cVar1;
  byte bVar2;
  char cVar3;
  char cVar4;
  char cVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  
  uVar7 = (uint)(byte)param_1[1];
  if (uVar7 == 0) {
    param_2[1] = param_1[2];
    *param_2 = param_1[2];
    param_2[2] = param_1[2];
  }
  else {
    cVar1 = *param_1;
    cVar5 = FUN_08000128(cVar1,0x2b);
    bVar2 = param_1[2];
    uVar6 = (uint)bVar2;
    uVar8 = (uint)(byte)((cVar1 + cVar5 * -0x2b) * '\x06');
    cVar4 = (char)(uVar6 * (0xff - ((int)((0xff - uVar8) * uVar7) >> 8)) >> 8);
    cVar1 = (char)(uVar6 * (0xff - uVar7) >> 8);
    cVar3 = (char)(uVar6 * (0xff - ((int)(uVar7 * uVar8) >> 8)) >> 8);
    switch(cVar5) {
    case '\0':
      param_2[1] = bVar2;
      *param_2 = cVar4;
      param_2[2] = cVar1;
      break;
    case '\x01':
      param_2[1] = cVar3;
      *param_2 = param_1[2];
      param_2[2] = cVar1;
      break;
    case '\x02':
      param_2[1] = cVar1;
      *param_2 = param_1[2];
      param_2[2] = cVar4;
      break;
    case '\x03':
      param_2[1] = cVar1;
      *param_2 = cVar3;
      param_2[2] = param_1[2];
      break;
    case '\x04':
      param_2[1] = cVar4;
      *param_2 = cVar1;
      param_2[2] = param_1[2];
      break;
    default:
      param_2[1] = bVar2;
      *param_2 = cVar1;
      param_2[2] = cVar3;
    }
  }
  return;
}


/* ==== FUN_08005910 @ 0x8005910 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08005910(void)

{
  byte bVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined1 local_20;
  undefined1 local_1f;
  undefined1 local_1e;
  undefined1 local_1c;
  undefined1 local_1b;
  byte local_1a;
  
  bVar1 = DAT_20000366;
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 0x19;
  }
  local_1c = DAT_20000364;
  local_1b = DAT_20000365;
  local_1a = DAT_20000366;
  uVar2 = FUN_080012F0(_DAT_20000358 << 1);
  uVar2 = FUN_08000BFC(uVar2,0x40490fdb);
  uVar2 = FUN_08000818(uVar2,0x42200000);
  FUN_08000E5C(uVar2,0x3fc90fdb);
  uVar2 = FUN_08002F48();
  uVar3 = FUN_08001248(bVar1 >> 1);
  uVar2 = FUN_08000BFC(uVar3,uVar2);
  uVar3 = FUN_08001248(DAT_20000366 >> 1);
  FUN_08000474(uVar2,uVar3);
  local_1a = FUN_08000380();
  FUN_08005848(&local_1c,&local_20);
  DAT_20000386 = local_1f;
  DAT_20000385 = local_20;
  DAT_20000387 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_20000389 = local_1f;
  DAT_20000388 = local_20;
  DAT_2000038a = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_2000038c = local_1f;
  DAT_2000038b = local_20;
  DAT_2000038d = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_2000038f = local_1f;
  DAT_2000038e = local_20;
  DAT_20000390 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_20000392 = local_1f;
  DAT_20000391 = local_20;
  DAT_20000393 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_20000395 = local_1f;
  DAT_20000394 = local_20;
  DAT_20000396 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_20000398 = local_1f;
  DAT_20000397 = local_20;
  DAT_20000399 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_2000039b = local_1f;
  DAT_2000039a = local_20;
  DAT_2000039c = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_2000039e = local_1f;
  DAT_2000039d = local_20;
  DAT_2000039f = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_200003a1 = local_1f;
  DAT_200003a0 = local_20;
  DAT_200003a2 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_200003a4 = local_1f;
  DAT_200003a3 = local_20;
  DAT_200003a5 = local_1e;
  FUN_08005848(&local_1c,&local_20);
  DAT_200003a7 = local_1f;
  DAT_200003a6 = local_20;
  DAT_200003a8 = local_1e;
  return _DAT_20000358 < (uint)(_DAT_20000368 * 0x28);
}


/* ==== FUN_08005B0C @ 0x8005b0c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08005B0C(void)

{
  byte bVar1;
  int iVar2;
  byte bVar3;
  uint extraout_r1;
  uint extraout_r1_00;
  uint extraout_r1_01;
  uint extraout_r1_02;
  uint extraout_r1_03;
  uint extraout_r1_04;
  uint extraout_r1_05;
  uint extraout_r1_06;
  uint extraout_r1_07;
  uint extraout_r1_08;
  uint extraout_r1_09;
  uint extraout_r1_10;
  uint extraout_r1_11;
  uint uVar4;
  undefined1 local_20;
  undefined1 local_1f;
  undefined1 local_1e;
  undefined1 local_1c;
  undefined1 local_1b;
  byte local_1a;
  
  bVar3 = DAT_20000366;
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 0x50;
  }
  local_1c = DAT_20000364;
  local_1b = DAT_20000365;
  local_1a = DAT_20000366;
  bVar1 = DAT_20000366 >> 2;
  FUN_080001b0(_DAT_20000358,0xc);
  uVar4 = extraout_r1 & 0xff;
  FUN_08000270(uVar4,0xc);
  if (bVar3 < (byte)(bVar1 * '\x04')) {
    local_1a = '\0';
  }
  else {
    local_1a = bVar3 + bVar1 * -4;
  }
  FUN_08005848(&local_1c,&local_20);
  iVar2 = (extraout_r1_00 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = local_1f;
  (&DAT_20000385)[iVar2] = local_20;
  (&DAT_20000387)[iVar2] = local_1e;
  FUN_08000270(uVar4 + 1,0xc);
  if (DAT_20000366 < (byte)(bVar1 * '\x03')) {
    local_1a = '\0';
  }
  else {
    local_1a = DAT_20000366 + bVar1 * -3;
  }
  FUN_08005848(&local_1c,&local_20);
  iVar2 = (extraout_r1_01 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = local_1f;
  (&DAT_20000385)[iVar2] = local_20;
  (&DAT_20000387)[iVar2] = local_1e;
  FUN_08000270(uVar4 + 2,0xc);
  if (DAT_20000366 < (byte)(bVar1 * '\x02')) {
    local_1a = '\0';
  }
  else {
    local_1a = DAT_20000366 + bVar1 * -2;
  }
  FUN_08005848(&local_1c,&local_20);
  iVar2 = (extraout_r1_02 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = local_1f;
  (&DAT_20000385)[iVar2] = local_20;
  (&DAT_20000387)[iVar2] = local_1e;
  FUN_08000270(uVar4 + 3,0xc);
  if (DAT_20000366 < bVar1) {
    local_1a = '\0';
  }
  else {
    local_1a = DAT_20000366 - bVar1;
  }
  FUN_08005848(&local_1c,&local_20);
  iVar2 = (extraout_r1_03 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = local_1f;
  (&DAT_20000385)[iVar2] = local_20;
  (&DAT_20000387)[iVar2] = local_1e;
  local_1a = DAT_20000366;
  FUN_08005848(&local_1c,&local_20);
  FUN_08000270(uVar4 + 4,0xc);
  iVar2 = (extraout_r1_04 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = local_1f;
  (&DAT_20000385)[iVar2] = local_20;
  (&DAT_20000387)[iVar2] = local_1e;
  FUN_08000270(uVar4 + 5,0xc);
  iVar2 = (extraout_r1_05 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  FUN_08000270(uVar4 + 6,0xc);
  iVar2 = (extraout_r1_06 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  FUN_08000270(uVar4 + 7,0xc);
  iVar2 = (extraout_r1_07 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  FUN_08000270(uVar4 + 8,0xc);
  iVar2 = (extraout_r1_08 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  FUN_08000270(uVar4 + 9,0xc);
  iVar2 = (extraout_r1_09 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  FUN_08000270(uVar4 + 10,0xc);
  iVar2 = (extraout_r1_10 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  FUN_08000270(uVar4 + 0xb,0xc);
  iVar2 = (extraout_r1_11 & 0xff) * 3;
  (&DAT_20000386)[iVar2] = 0;
  (&DAT_20000385)[iVar2] = 0;
  (&DAT_20000387)[iVar2] = 0;
  uVar4 = FUN_08000128(_DAT_20000358,0xc);
  return uVar4 < _DAT_20000368;
}


/* ==== FUN_08005DC0 @ 0x8005dc0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08005DC0(void)

{
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  
  if (_DAT_20000358 == 0) {
    _DAT_2000035c = 1000;
  }
  FUN_08005848(&DAT_20000364,&local_14);
  DAT_200003a7 = local_13;
  DAT_200003a6 = local_14;
  DAT_200003a8 = local_12;
  DAT_200003a4 = local_13;
  DAT_200003a3 = local_14;
  DAT_200003a5 = local_12;
  DAT_200003a1 = local_13;
  DAT_200003a0 = local_14;
  DAT_200003a2 = local_12;
  DAT_2000039e = local_13;
  DAT_2000039d = local_14;
  DAT_2000039f = local_12;
  DAT_2000039b = local_13;
  DAT_2000039a = local_14;
  DAT_2000039c = local_12;
  DAT_20000398 = local_13;
  DAT_20000397 = local_14;
  DAT_20000399 = local_12;
  DAT_20000395 = local_13;
  DAT_20000394 = local_14;
  DAT_20000396 = local_12;
  DAT_20000392 = local_13;
  DAT_20000391 = local_14;
  DAT_20000393 = local_12;
  DAT_2000038f = local_13;
  DAT_2000038e = local_14;
  DAT_20000390 = local_12;
  DAT_2000038c = local_13;
  DAT_2000038b = local_14;
  DAT_2000038d = local_12;
  DAT_20000389 = local_13;
  DAT_20000388 = local_14;
  DAT_2000038a = local_12;
  DAT_20000386 = local_13;
  DAT_20000385 = local_14;
  DAT_20000387 = local_12;
  return 1;
}


/* ==== FUN_08005E84 @ 0x8005e84 ==== */

void FUN_08005E84(void)

{
  DAT_20000380 = 0;
  DAT_2000037f = 0;
  DAT_20000381 = 0x11;
  DAT_20000383 = 0x14;
  DAT_20000382 = 0;
  DAT_20000384 = 0;
  FUN_08008F14();
  return;
}


/* ==== FUN_08005EBC @ 0x8005ebc ==== */

void FUN_08005EBC(void)

{
  FUN_08009060();
  return;
}


/* ==== FUN_08005EC4 @ 0x8005ec4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08005EC4(uint param_1,int param_2,undefined4 param_3)

{
  uint uVar1;
  
  uVar1 = (uint)_DAT_2000035e;
  if (uVar1 == param_1) {
    *(undefined4 *)(&DAT_2000035c + (param_2 + 2) * 4) = param_3;
  }
  return uVar1 == param_1;
}


/* ==== FUN_08005EE4 @ 0x8005ee4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08005EE4(void)

{
  return _DAT_20000360 != 0 && _DAT_2000035e != 0;
}


/* ==== FUN_08005F00 @ 0x8005f00 ==== */

undefined4 FUN_08005F00(void)

{
  return 0;
}


/* ==== FUN_08005F04 @ 0x8005f04 ==== */

undefined4 FUN_08005F04(int param_1)

{
  int iVar1;
  
  if (param_1 == 4) {
    iVar1 = 10;
  }
  else if (param_1 == 0x108) {
    iVar1 = 9;
  }
  else if (param_1 == 0x107) {
    iVar1 = 8;
  }
  else if (param_1 == 0x106) {
    iVar1 = 7;
  }
  else if (param_1 == 0x1ff) {
    iVar1 = 6;
  }
  else if (param_1 == 0x105) {
    iVar1 = 5;
  }
  else if (param_1 == 3) {
    iVar1 = 4;
  }
  else if (param_1 == 0x104) {
    iVar1 = 3;
  }
  else if (param_1 == 0x103) {
    iVar1 = 2;
  }
  else if (param_1 == 2) {
    iVar1 = 1;
  }
  else {
    if (param_1 != 1) {
      return 0;
    }
    iVar1 = 0;
  }
  return (&DAT_0800c414)[iVar1 * 2];
}


/* ==== FUN_08005F94 @ 0x8005f94 ==== */

bool FUN_08005F94(undefined2 param_1)

{
  uint uVar1;
  
  uVar1 = (uint)DAT_2000037e;
  if (uVar1 < 3) {
    *(undefined2 *)(&DAT_20000358 + (uVar1 + 0x10) * 2) = param_1;
    DAT_2000037e = DAT_2000037e + 1;
  }
  return uVar1 < 3;
}


/* ==== FUN_08005FBC @ 0x8005fbc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08005FBC(undefined4 param_1)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = (uint)_DAT_2000035e;
  if (uVar2 != 0) {
    if (-1 < (int)(uVar2 << 0x17)) {
      return false;
    }
    iVar1 = FUN_08005F94(uVar2);
    if (iVar1 == 0) {
      return false;
    }
  }
  _DAT_20000360 = FUN_08005F04(param_1);
  if (_DAT_20000360 != 0) {
    _DAT_20000358 = 0;
    _DAT_2000035e = (ushort)param_1;
    _DAT_2000035c = 0;
  }
  return _DAT_20000360 != 0;
}


/* ==== FUN_08006000 @ 0x8006000 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08006000(void)

{
  int iVar1;
  uint local_18;
  int local_14;
  
  FUN_0800A558(0x20000f70,&local_18);
  if ((((local_14 == 0) && (local_18 <= _DAT_2000035c)) || (_DAT_2000035e == 0)) &&
     ((local_14 == 0 && (local_18 < 0x321)))) {
    if (_DAT_20000374 == 0) {
      return;
    }
  }
  else if (_DAT_20000374 == 0) goto LAB_0800604c;
  _DAT_20000374 = 0;
LAB_0800604c:
  if ((_DAT_2000035e != 0) && (_DAT_20000360 != (code *)0x0)) {
    if ((_DAT_20000358 == 0) && (_DAT_2000035c == 0)) {
      _DAT_2000035c = 0xa0;
    }
    iVar1 = (*_DAT_20000360)(_DAT_20000358);
    if (iVar1 == 0) {
      _DAT_20000360 = (code *)0x0;
      _DAT_2000035e = 0;
      FUN_08006828(_DAT_200003ac,0);
      _DAT_200003ac = 0;
      if (DAT_2000037e != 0) {
        DAT_2000037e = DAT_2000037e - 1;
        FUN_08005FBC(*(undefined2 *)(&DAT_20000358 + (DAT_2000037e + 0x10) * 2));
      }
    }
    else {
      _DAT_20000358 = _DAT_20000358 + 1;
    }
    FUN_080090A8(&DAT_20000385);
  }
  iVar1 = FUN_08009268();
  if (iVar1 == 0) {
    FUN_080092D4();
  }
  FUN_0800A4DC(0x20000f70);
  return;
}


/* ==== FUN_080060DC @ 0x80060dc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080060DC(uint param_1)

{
  if ((_DAT_2000035e == param_1) || (param_1 == 0)) {
    _DAT_20000360 = 0;
    _DAT_2000035e = 0;
  }
  if (DAT_2000037e != 0) {
    DAT_2000037e = DAT_2000037e - 1;
    FUN_08005FBC(*(undefined2 *)(&DAT_20000358 + (DAT_2000037e + 0x10) * 2));
  }
  return;
}


/* ==== FUN_08006114 @ 0x8006114 ==== */

void FUN_08006114(void)

{
  DAT_2000037e = 0;
  return;
}


/* ==== FUN_08006124 @ 0x8006124 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined2 FUN_08006124(void)

{
  return _DAT_2000035e;
}


/* ==== FUN_08006130 @ 0x8006130 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08006130(void)

{
  _DAT_20000374 = 1;
  return;
}


/* ==== FUN_0800613c @ 0x800613c ==== */

void FUN_0800613c(undefined1 *param_1)

{
  DAT_20000380 = *param_1;
  DAT_2000037f = param_1[1];
  DAT_20000381 = param_1[2];
  return;
}


/* ==== FUN_08006158 @ 0x8006158 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006158(undefined4 param_1,undefined3 *param_2)

{
  undefined3 uVar1;
  int iVar2;
  
  FUN_08003040();
  if (_DAT_200003ac != 0) {
    FUN_08006828(_DAT_200003ac,1);
    iVar2 = FUN_08006124();
    if (iVar2 != 0x106) {
      FUN_080060DC(0);
    }
  }
  uVar1 = *param_2;
  iVar2 = FUN_08006124();
  if (iVar2 != 0x106) {
    FUN_08005FBC(0x106);
  }
  FUN_08005EC4(0x106,0,uVar1);
  FUN_08006130();
  _DAT_200003ac = 0;
  return 0;
}


/* ==== FUN_080061C4 @ 0x80061c4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_080061C4(undefined4 *param_1,char *param_2)

{
  undefined1 local_14 [4];
  
  FUN_08003040();
  if (_DAT_200003ac != 0) {
    FUN_08006828(_DAT_200003ac,1);
    FUN_080060DC(0);
    _DAT_200003ac = 0;
  }
  local_14[2] = 0x3c;
  local_14._0_2_ = *(undefined2 *)(param_2 + 1);
  if (*param_2 == '\0') {
    FUN_08005FBC(0x107);
    FUN_08005EC4(0x107,0,local_14._0_3_);
    FUN_08005EC4(0x107,1,param_2[3]);
    FUN_08005EC4(0x107,2,param_2[4]);
  }
  else if (*param_2 == '\x01') {
    FUN_08005FBC(0x108);
    FUN_08005EC4(0x108,0,local_14._0_3_);
    FUN_08005EC4(0x108,1,param_2[3]);
    FUN_08005EC4(0x108,2,param_2[4]);
  }
  _DAT_200003ac = *param_1;
  *param_1 = 0;
  return 0;
}


/* ==== FUN_0800626C @ 0x800626c ==== */

void FUN_0800626C(void)

{
  undefined4 in_r3;
  
  if (DAT_20000410 == '\x02') {
    DAT_20000410 = '\x03';
    FUN_08005EC4(4,0,3,0x60,in_r3);
    FUN_08006130();
  }
  return;
}


/* ==== FUN_08006294 @ 0x8006294 ==== */

void FUN_08006294(int param_1)

{
  int iVar1;
  
  if (param_1 == 2) {
    DAT_20000410 = 2;
    iVar1 = FUN_08005FBC(4);
    if (iVar1 == 0) {
      FUN_08005F94(4);
    }
    FUN_08005EC4(4,0,2);
  }
  else {
    FUN_080060DC(4);
  }
  return;
}


/* ==== FUN_080062CC @ 0x80062cc ==== */

void FUN_080062CC(void)

{
  FUN_0800613c(&LAB_0800c700);
  return;
}


/* ==== FUN_080062DC @ 0x80062dc ==== */

void FUN_080062DC(void)

{
  FUN_0800613c(&DAT_0800c6fc);
  FUN_08003040();
  return;
}


/* ==== FUN_080062F0 @ 0x80062f0 ==== */

void FUN_080062F0(void)

{
  if (DAT_20000400 == '\0') {
    DAT_20000400 = 1;
    FUN_0800A4DC(0x20000408);
    FUN_080065F4();
    FUN_0800942c();
    FUN_08009AE4();
    FUN_08003274();
    FUN_08006114();
    FUN_080060DC(0);
    FUN_08005FBC(2);
    DAT_20000400 = '\x02';
  }
  return;
}


/* ==== FUN_08006338 @ 0x8006338 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08006338(void)

{
  FUN_08009654();
  FUN_080098C4();
  FUN_08009874();
  FUN_08009930(0x80062f1);
  FUN_0800993c(0x800626d);
  FUN_08009958();
  FUN_08009948();
  FUN_08007068();
  FUN_08007168();
  FUN_08005E84();
  FUN_08005EBC();
  FUN_08009A04();
  FUN_08009AAC();
  FUN_08003198();
  FUN_08003234();
  FUN_08007F7C();
  FUN_0800806C();
  FUN_0800306C();
  FUN_08003088();
  FUN_08008774(0x8006295);
  FUN_08008700();
  FUN_08009D88();
  FUN_08009E0C();
  FUN_08008380();
  FUN_08008440();
  FUN_080094C4();
  FUN_080094DC();
  FUN_080093C8();
  FUN_0800940C();
  FUN_0800930C();
  FUN_08009324();
  _DAT_20001028 = 0x20000420;
  DAT_2000102c = 0x10;
  FUN_08005FBC(1);
  FUN_08005F94(0x105);
  return;
}


/* ==== FUN_080063EC @ 0x80063ec ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080063EC(void)

{
  int iVar1;
  undefined8 uVar2;
  undefined1 auStack_18 [8];
  uint local_10;
  int local_c;
  
  FUN_080096EC();
  FUN_08008784();
  FUN_080071C0();
  FUN_08009EA0();
  FUN_08008130();
  FUN_08009340();
  FUN_08006000();
  FUN_080030D0();
  FUN_080094F8();
  FUN_08008574();
  if ((DAT_20000410 == '\x03') && (iVar1 = FUN_08006124(), iVar1 == 4)) {
    local_10 = 0;
    local_c = 0;
    FUN_0800A4DC(auStack_18);
    while ((((iVar1 = FUN_08009094(), iVar1 != 0 &&
             (iVar1 = FUN_0800A558(auStack_18,&local_10), iVar1 != 0)) && (local_c == 0)) &&
           (local_10 < 0xb))) {
      FUN_0800ab10();
    }
    DataSynchronizationBarrier(0xf);
    DataSynchronizationBarrier(0xf);
    do {
                    // WARNING: Do nothing block with infinite loop
    } while( true );
  }
  FUN_08003330();
  uVar2 = FUN_08003050();
  if (((int)((ulonglong)uVar2 >> 0x20) != 0) || (900000 < (uint)uVar2)) {
    FUN_080062F0();
  }
  if (DAT_20000400 == '\x02') {
    FUN_0800A558(0x20000408,&local_10);
    iVar1 = FUN_08005EE4();
    if (((iVar1 == 0) || (local_c != 0)) || (3000 < local_10)) {
      FUN_080097E8(1);
    }
  }
  return;
}


/* ==== FUN_080064F0 @ 0x80064f0 ==== */

undefined4 FUN_080064F0(void)

{
  return 0;
}


/* ==== FUN_080064f4 @ 0x80064f4 ==== */

void FUN_080064f4(undefined1 param_1)

{
  DAT_20000f78 = param_1;
  return;
}


/* ==== FUN_08006500 @ 0x8006500 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006500(uint param_1,undefined4 param_2)

{
  undefined4 uVar1;
  byte *pbVar2;
  uint uVar3;
  
  uVar3 = (uint)(byte)(DAT_2000102c - 1U);
  pbVar2 = (byte *)(_DAT_20001028 + uVar3 * 8);
  if (*pbVar2 == param_1) {
LAB_0800652c:
    uVar1 = (**(code **)(pbVar2 + 4))(param_2);
  }
  else {
    if (uVar3 != 0) {
      if ((DAT_2000102c - 1U & 1) != 0) {
        uVar3 = uVar3 - 1 & 0xff;
        pbVar2 = (byte *)(_DAT_20001028 + uVar3 * 8);
        if (*pbVar2 == param_1) goto LAB_0800652c;
        if (uVar3 == 0) goto LAB_08006534;
      }
      do {
        uVar3 = uVar3 - 1 & 0xff;
        pbVar2 = (byte *)(_DAT_20001028 + uVar3 * 8);
        if (*pbVar2 == param_1) goto LAB_0800652c;
        uVar3 = uVar3 - 1 & 0xff;
        pbVar2 = (byte *)(_DAT_20001028 + uVar3 * 8);
        if (*pbVar2 == param_1) goto LAB_0800652c;
      } while (uVar3 != 0);
    }
LAB_08006534:
    uVar1 = 0x7f;
  }
  return uVar1;
}


/* ==== FUN_0800656C @ 0x800656c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800656C(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  uint uVar2;
  byte *pbVar3;
  
  uVar2 = (uint)(byte)(DAT_2000102c - 1U);
  pbVar3 = (byte *)(_DAT_20001028 + uVar2 * 8);
  if (*pbVar3 == param_1) {
LAB_08006598:
    uVar1 = (**(code **)(pbVar3 + 4))(param_2,param_3,param_3,uVar2,param_4);
  }
  else {
    if (uVar2 != 0) {
      if ((DAT_2000102c - 1U & 1) != 0) {
        uVar2 = uVar2 - 1 & 0xff;
        pbVar3 = (byte *)(_DAT_20001028 + uVar2 * 8);
        if (*pbVar3 == param_1) goto LAB_08006598;
        if (uVar2 == 0) goto LAB_080065a2;
      }
      do {
        uVar2 = uVar2 - 1 & 0xff;
        pbVar3 = (byte *)(_DAT_20001028 + uVar2 * 8);
        if (*pbVar3 == param_1) goto LAB_08006598;
        uVar2 = uVar2 - 1 & 0xff;
        pbVar3 = (byte *)(_DAT_20001028 + uVar2 * 8);
        if (*pbVar3 == param_1) goto LAB_08006598;
      } while (uVar2 != 0);
    }
LAB_080065a2:
    uVar1 = 0x7f;
  }
  return uVar1;
}


/* ==== FUN_080065DC @ 0x80065dc ==== */

void FUN_080065DC(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  FUN_08002ac0(0x17,param_1,0,param_2,param_3);
  return;
}


/* ==== FUN_080065F4 @ 0x80065f4 ==== */

void FUN_080065F4(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  FUN_08002ac0(0x1c,DAT_20000f78,param_3,&DAT_20000f78,param_4);
  return;
}


/* ==== FUN_08006608 @ 0x8006608 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08006608(void)

{
  (*_DAT_20000a10)("BLE-boot\n");
  return;
}


/* ==== FUN_0800661C @ 0x800661c ==== */

void FUN_0800661C(undefined4 param_1)

{
  FUN_08002ac0(0x39,2,2);
  FUN_08002ac0(0x32,1);
  FUN_08006500(2,param_1);
  return;
}


/* ==== FUN_0800663C @ 0x800663c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800663C(void)

{
  (*_DAT_20000a10)("BLE-RSP HELLO\n");
  return;
}


/* ==== FUN_08006650 @ 0x8006650 ==== */

void FUN_08006650(undefined1 *param_1)

{
  FUN_08002ac0(0x1a,*param_1,1,0,0);
  return;
}


/* ==== FUN_08006668 @ 0x8006668 ==== */

void FUN_08006668(undefined1 *param_1)

{
  short sVar1;
  undefined4 uVar2;
  uint local_c;
  
  if (*(short *)(param_1 + 2) == 0x1e) {
    if (param_1[1] == '\x02') {
      FUN_08002ac0(0x1b,*param_1,0);
    }
    sVar1 = *(short *)(param_1 + 9);
    local_c = (uint)*(ushort *)(param_1 + 7);
    if (sVar1 == 1) {
      uVar2 = FUN_0800656C(3,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 2) {
      uVar2 = FUN_0800656C(4,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 3) {
      uVar2 = FUN_0800656C(5,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 4) {
      uVar2 = FUN_0800656C(6,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 5) {
      uVar2 = FUN_0800656C(7,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 6) {
      uVar2 = FUN_0800656C(8,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 7) {
      uVar2 = FUN_0800656C(9,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 8) {
      uVar2 = FUN_0800656C(10,&local_c,param_1 + 0xb);
    }
    else {
      uVar2 = 0;
      if (sVar1 == 9) {
        uVar2 = FUN_0800656C(0xb,&local_c,param_1 + 0xb);
      }
    }
    if (local_c != 0) {
      FUN_08006828(local_c,uVar2);
    }
  }
  else if (*(short *)(param_1 + 2) == 0x28) {
    if (param_1[1] == '\x02') {
      FUN_08002ac0(0x1b,*param_1,0);
    }
    sVar1 = *(short *)(param_1 + 9);
    local_c = (uint)*(ushort *)(param_1 + 7);
    if (sVar1 == 1) {
      FUN_0800656C(0xc,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 2) {
      FUN_0800656C(0xd,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 3) {
      FUN_0800656C(0xe,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 4) {
      FUN_0800656C(0xf,&local_c,param_1 + 0xb);
    }
    else if (sVar1 == 5) {
      FUN_0800656C(0x10,&local_c,param_1 + 0xb);
    }
  }
  else if (param_1[1] == '\x02') {
    FUN_08002ac0(0x1b,*param_1,1);
  }
  return;
}


/* ==== FUN_080067F4 @ 0x80067f4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080067F4(void)

{
  (*_DAT_20000a10)("BLE-RSP GAP SET MODE: ");
  return;
}


/* ==== FUN_08006808 @ 0x8006808 ==== */

void FUN_08006808(undefined1 *param_1)

{
  if ((int)((uint)(byte)param_1[1] << 0x1d) < 0) {
    FUN_080064f4(*param_1);
    FUN_08006500(1,param_1);
  }
  return;
}


/* ==== FUN_08006828 @ 0x8006828 ==== */

void FUN_08006828(int param_1,undefined1 param_2)

{
  undefined1 local_c;
  undefined1 local_b;
  undefined1 local_a;
  
  if (param_1 != 0) {
    local_c = (undefined1)param_1;
    local_b = (undefined1)((uint)param_1 >> 8);
    local_a = param_2;
    FUN_080065DC(0x21,3,&local_c,0);
  }
  return;
}


/* ==== FUN_08006848 @ 0x8006848 ==== */

void FUN_08006848(undefined4 *param_1,int param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  undefined1 *puVar3;
  
  if (param_1 == (undefined4 *)0x0) {
    return;
  }
  iVar1 = -(param_2 + 9U & 0xfffffff8);
  puVar3 = &stack0xffffffe8 + iVar1;
  *puVar3 = (char)*param_1;
  (&stack0xffffffe9)[iVar1] = (char)((uint)*param_1 >> 8);
  uVar2 = param_2 - 1U & 0xff;
  if (param_2 != 0) {
    puVar3[uVar2 + 2] = *(undefined1 *)(param_3 + uVar2);
    uVar2 = uVar2 - 1 & 0xff;
    if (uVar2 != 0xff) {
      if ((param_2 - 1U & 1) != 0) {
        puVar3[uVar2 + 2] = *(undefined1 *)(param_3 + uVar2);
        uVar2 = uVar2 - 1 & 0xff;
        if (uVar2 == 0xff) goto LAB_080068a4;
      }
      do {
        puVar3[uVar2 + 2] = *(undefined1 *)(param_3 + uVar2);
        uVar2 = uVar2 - 1 & 0xff;
        puVar3[uVar2 + 2] = *(undefined1 *)(param_3 + uVar2);
        uVar2 = uVar2 - 1 & 0xff;
      } while (uVar2 != 0xff);
    }
  }
LAB_080068a4:
  FUN_080065DC(0x2b,param_2 + 2U & 0xff,puVar3,0);
  return;
}


/* ==== FUN_080068D4 @ 0x80068d4 ==== */

char * FUN_080068D4(int param_1,char *param_2,int param_3)

{
  uint extraout_r1;
  char *pcVar1;
  char *pcVar2;
  char cVar3;
  
  *param_2 = '\0';
  pcVar2 = param_2;
  if (param_3 - 2U < 0x23) {
    do {
      pcVar1 = pcVar2;
      FUN_080001b0(param_1,param_3);
      if (extraout_r1 < 10) {
        cVar3 = (char)extraout_r1 + '0';
      }
      else {
        cVar3 = (char)extraout_r1 + 'W';
      }
      *pcVar1 = cVar3;
      param_1 = FUN_08000128(param_1,param_3);
      pcVar2 = pcVar1 + 1;
    } while (param_1 != 0);
    pcVar1[1] = '\0';
    pcVar2 = param_2;
    if (param_2 < pcVar1) {
      do {
        cVar3 = *pcVar2;
        *pcVar2 = *pcVar1;
        *pcVar1 = cVar3;
        pcVar2 = pcVar2 + 1;
        pcVar1 = pcVar1 + -1;
      } while (pcVar2 < pcVar1);
    }
  }
  return param_2;
}


/* ==== FUN_0800693C @ 0x800693c ==== */

char * FUN_0800693C(int param_1,char *param_2,int param_3)

{
  char *pcVar1;
  char cVar2;
  uint extraout_r1;
  char *pcVar3;
  char *pcVar4;
  
  *param_2 = '\0';
  if (param_3 - 2U < 0x23) {
    pcVar1 = param_2;
    pcVar4 = param_2;
    if (param_1 < 0) {
      *param_2 = '-';
      pcVar4 = param_2 + 1;
      param_1 = -param_1;
      pcVar1 = pcVar4;
    }
    do {
      pcVar3 = pcVar1;
      FUN_080001b0(param_1,param_3);
      if (extraout_r1 < 10) {
        cVar2 = (char)extraout_r1 + '0';
      }
      else {
        cVar2 = (char)extraout_r1 + 'W';
      }
      *pcVar3 = cVar2;
      param_1 = FUN_08000128(param_1,param_3);
      pcVar1 = pcVar3 + 1;
    } while (param_1 != 0);
    pcVar3[1] = '\0';
    if (pcVar4 < pcVar3) {
      do {
        cVar2 = *pcVar4;
        *pcVar4 = *pcVar3;
        *pcVar3 = cVar2;
        pcVar4 = pcVar4 + 1;
        pcVar3 = pcVar3 + -1;
      } while (pcVar4 < pcVar3);
    }
  }
  return param_2;
}


/* ==== FUN_080069BC @ 0x80069bc ==== */

int FUN_080069BC(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_080002AC(param_1,0);
  if (iVar1 != 0) {
    param_1 = param_1 + -0x80000000;
  }
  return param_1;
}


/* ==== FUN_080069D4 @ 0x80069d4 ==== */

void FUN_080069D4(char *param_1,uint param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  char cVar6;
  undefined8 uVar7;
  
  if (*param_1 == '-') {
    cVar6 = -1;
  }
  else {
    cVar6 = '\x01';
  }
  uVar5 = (uint)(*param_1 == '-');
  uVar4 = (uint)(byte)param_1[uVar5];
  if (uVar4 == 0) {
    iVar1 = 0;
    iVar3 = 0;
  }
  else {
    iVar1 = 0;
    iVar3 = 0;
    do {
      if ((param_2 < 0xb) || (uVar4 < 0x3a)) {
        uVar7 = FUN_0800033c(param_2,0,iVar1,iVar3);
        uVar2 = (uint)uVar7 + uVar4;
        iVar1 = uVar2 - 0x30;
        iVar3 = (int)((ulonglong)uVar7 >> 0x20) + (uint)CARRY4((uint)uVar7,uVar4) + -1 +
                (uint)(0x2f < uVar2);
      }
      else {
        uVar7 = FUN_0800033c(param_2,0,iVar1,iVar3);
        uVar2 = uVar4 + (uint)uVar7;
        iVar1 = uVar2 - 0x57;
        iVar3 = (int)((ulonglong)uVar7 >> 0x20) + (uint)CARRY4(uVar4,(uint)uVar7) + -1 +
                (uint)(0x56 < uVar2);
      }
      uVar5 = uVar5 + 1 & 0xff;
      uVar4 = (uint)(byte)param_1[uVar5];
    } while (uVar4 != 0);
  }
  FUN_0800033c((int)cVar6,(int)cVar6 >> 0x1f,iVar1,iVar3);
  return;
}


/* ==== FUN_08006A84 @ 0x8006a84 ==== */

void FUN_08006A84(undefined4 *param_1,undefined4 *param_2)

{
  if (param_2 != (undefined4 *)0x0) {
    *param_1 = *param_2;
    param_1[1] = param_2[1];
    param_1[2] = param_2[2];
    param_1[3] = param_2[3];
    param_1[6] = param_2[4];
    param_1[7] = param_2[5];
    param_1[8] = param_2[6];
    param_1[10] = param_2[7];
  }
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[9] = 0;
  return;
}


/* ==== FUN_08006AB4 @ 0x8006ab4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006AB4(undefined4 *param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  
  uVar6 = param_1[5];
  iVar1 = FUN_080002AC(uVar6,0);
  uVar2 = FUN_08000BFC(uVar6,param_1[3]);
  uVar6 = FUN_08000E5C(uVar6,param_2);
  uVar3 = FUN_080069BC();
  uVar7 = param_1[8];
  iVar4 = FUN_080002AC(uVar3,uVar7);
  if (iVar4 == 0) {
    uVar3 = FUN_08000BFC(uVar6,param_1[1]);
    uVar5 = FUN_08000BFC(uVar6,*param_1);
    uVar2 = FUN_08000474(uVar5,uVar2);
    uVar2 = FUN_08000474(uVar2,param_1[4]);
    uVar2 = FUN_08000474(uVar2,uVar3);
    uVar5 = FUN_08000E5C(uVar6,_DAT_20000f7c);
    uVar5 = FUN_08000BFC(uVar5,param_1[2]);
    uVar2 = FUN_08000474(uVar2,uVar5);
    iVar4 = FUN_080002D4(uVar7,0);
    if ((iVar4 == 0) || (iVar4 = FUN_080002A0(uVar6,0), iVar4 == 0)) goto LAB_08006b6c;
  }
  else {
    uVar3 = FUN_08000BFC(param_1[1],0);
    uVar6 = FUN_08000BFC(*param_1,0);
    uVar2 = FUN_08000474(uVar6,uVar2);
    uVar2 = FUN_08000474(uVar2,param_1[4]);
    uVar2 = FUN_08000474(uVar2,uVar3);
    uVar6 = FUN_08000E5C(0,_DAT_20000f7c);
    uVar6 = FUN_08000BFC(uVar6,param_1[2]);
    uVar2 = FUN_08000474(uVar2,uVar6);
    iVar4 = FUN_080002D4(uVar7,0);
    uVar6 = 0;
    if (iVar4 == 0) goto LAB_08006b6c;
    uVar6 = 0;
  }
  iVar4 = FUN_080002A0(param_1[5],0);
  if (iVar4 != 0) {
    param_1[4] = 0;
    uVar2 = 0;
    uVar3 = 0;
  }
LAB_08006b6c:
  uVar7 = param_1[7];
  iVar4 = FUN_080002D4(uVar2,uVar7);
  if ((iVar4 == 0) && (iVar4 = FUN_080002AC(uVar2,param_1[6]), iVar4 == 0)) {
    uVar3 = FUN_08000474(param_1[4],uVar3);
    param_1[4] = uVar3;
  }
  else {
    uVar2 = FUN_08000E5C(uVar2,uVar3);
  }
  uVar3 = FUN_08001248(-(uint)(iVar1 != 0) | 1);
  uVar3 = FUN_08000BFC(uVar3,uVar2);
  iVar1 = FUN_080002AC(uVar3,0);
  if ((iVar1 != 0) && (param_1[10] == 0)) {
    uVar2 = 0;
  }
  iVar1 = FUN_080002D4(uVar2,uVar7);
  if (iVar1 == 0) {
    uVar7 = uVar2;
  }
  uVar2 = param_1[6];
  iVar1 = FUN_080002AC(uVar7,uVar2);
  if (iVar1 == 0) {
    uVar2 = uVar7;
  }
  param_1[9] = uVar2;
  _DAT_20000f7c = uVar6;
  return uVar2;
}


/* ==== FUN_08006c40 @ 0x8006c40 ==== */

undefined4 FUN_08006c40(int param_1)

{
  return *(undefined4 *)(param_1 + 0x24);
}


/* ==== FUN_08006C44 @ 0x8006c44 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08006C44(void)

{
  int iVar1;
  
  iVar1 = FUN_08007980();
  return iVar1 != 0 || _DAT_200004ac != 0;
}


/* ==== FUN_08006C60 @ 0x8006c60 ==== */

void FUN_08006C60(undefined1 param_1)

{
  undefined1 local_c;
  undefined1 local_b;
  undefined1 local_a;
  
  local_c = 0xcc;
  local_b = 0xb8;
  local_a = param_1;
  FUN_080078E4(&local_c,3,0);
  return;
}


/* ==== FUN_08006C80 @ 0x8006c80 ==== */

void FUN_08006C80(undefined1 param_1)

{
  undefined1 local_c;
  undefined1 local_b;
  undefined1 local_a;
  
  local_c = 0xcc;
  local_b = 0x48;
  local_a = param_1;
  FUN_080078E4(&local_c,3,0xffffffff);
  return;
}


/* ==== FUN_08006CA0 @ 0x8006ca0 ==== */

undefined4 FUN_08006CA0(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_08007980();
  uVar2 = 1;
  if (iVar1 == 0) {
    if (DAT_200004ab == '\x01') {
      FUN_08006C80(1);
      DAT_200004ab = '\x02';
      uVar2 = 1;
    }
    else {
      uVar2 = 1;
      if (DAT_200004ab == '\x02') {
        DAT_200004ab = '\0';
        uVar2 = 0;
      }
    }
  }
  return uVar2;
}


/* ==== FUN_08006CDC @ 0x8006cdc ==== */

void FUN_08006CDC(undefined1 param_1)

{
  undefined1 local_c;
  undefined1 local_b;
  undefined1 local_a;
  
  local_c = 0xcc;
  local_b = 0xbe;
  local_a = param_1;
  FUN_080078E4(&local_c,3,9);
  return;
}


/* ==== FUN_08006CFC @ 0x8006cfc ==== */

void FUN_08006CFC(void)

{
  undefined1 local_c;
  undefined1 local_b;
  
  local_c = 0xcc;
  local_b = 0x44;
  FUN_080078E4(&local_c,2,0xffffffff);
  return;
}


/* ==== FUN_08006D18 @ 0x8006d18 ==== */

void FUN_08006D18(void)

{
  undefined1 local_c;
  undefined1 local_b;
  
  local_c = 0xcc;
  local_b = 0xb4;
  FUN_080078E4(&local_c,2,0xffffffff);
  return;
}


/* ==== FUN_08006D34 @ 0x8006d34 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006D34(void)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar2 = FUN_08007980();
  uVar3 = 1;
  if (iVar2 == 0) {
    if (DAT_200004ab == '\x01') {
      FUN_08006D18();
      DAT_200004ab = '\x02';
      uVar3 = 1;
    }
    else if (DAT_200004ab == '\x02') {
      FUN_08006C60(0);
      DAT_200004ab = '\x03';
      uVar3 = 1;
    }
    else if (DAT_200004ab == '\x03') {
      FUN_08006CDC(0);
      DAT_200004ab = '\x04';
      uVar3 = 1;
    }
    else if (DAT_200004ab == '\x04') {
      DAT_200004ab = 5;
      iVar2 = FUN_08007d7c();
      _DAT_200004a2 = (short)*(char *)(iVar2 + 2);
      iVar2 = FUN_08007d7c();
      bVar1 = *(byte *)(iVar2 + 4);
      iVar2 = FUN_08007d7c();
      _DAT_200004a4 = ((bVar1 & 3) * 0x100 + (ushort)*(byte *)(iVar2 + 3)) * 10;
      iVar2 = FUN_08007d7c();
      bVar1 = *(byte *)(iVar2 + 6);
      iVar2 = FUN_08007d7c();
      iVar2 = (short)((ushort)*(byte *)(iVar2 + 5) + (ushort)bVar1 * 0x100) * 0x271;
      _DAT_200004a6 = (undefined2)((int)(((uint)(iVar2 >> 0x1f) >> 0x1a) + iVar2) >> 6);
      FUN_08006C60(1);
      DAT_200004ab = '\x06';
      uVar3 = 1;
    }
    else if (DAT_200004ab == '\x06') {
      FUN_08006CDC(1);
      DAT_200004ab = '\a';
      uVar3 = 1;
    }
    else {
      uVar3 = 1;
      if (DAT_200004ab == '\a') {
        DAT_200004ab = 8;
        iVar2 = FUN_08007d7c();
        _DAT_200004a8 = (short)((int)((uint)*(byte *)(iVar2 + 4) * 0x271) >> 5) + -1000;
        DAT_200004ab = '\0';
        uVar3 = 0;
      }
    }
  }
  return uVar3;
}


/* ==== FUN_08006E24 @ 0x8006e24 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined1 FUN_08006E24(void)

{
  int iVar1;
  
  iVar1 = FUN_08007CE4();
  if (iVar1 == 0) {
    DAT_200004aa = 4;
  }
  else if (_DAT_200004a4 < 0x2081) {
    DAT_200004aa = 0x11;
  }
  else {
    DAT_200004aa = 0x12;
  }
  return DAT_200004aa;
}


/* ==== FUN_08006E5C @ 0x8006e5c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006E5C(void)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_08007980();
  if ((iVar1 == 0) && (_DAT_200004ac == 0)) {
    FUN_08006CFC();
    DAT_200004ab = 1;
    _DAT_200004ac = 0x8006d35;
    uVar2 = 1;
  }
  else {
    DAT_200004a1 = 2;
    uVar2 = 0;
  }
  return uVar2;
}


/* ==== FUN_08006E94 @ 0x8006e94 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006E94(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  undefined1 local_18;
  undefined1 local_17;
  undefined1 local_16;
  undefined1 local_15;
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  undefined1 local_11;
  
  uVar3 = param_1 + 1000U & 0xffff;
  uVar1 = 0;
  if (uVar3 < 0x1375) {
    iVar2 = FUN_08007980(0);
    if ((iVar2 == 0) && (_DAT_200004ac == 0)) {
      local_18 = 0xcc;
      local_17 = 0x4e;
      local_16 = 1;
      local_15 = 0;
      local_14 = 0;
      local_13 = 0;
      local_12 = 0;
      local_11 = FUN_080001c4(uVar3 << 5,0x271);
      DAT_200004ab = 1;
      _DAT_200004ac = 0x8006ca1;
      uVar1 = FUN_080078E4(&local_18,8,0);
    }
    else {
      DAT_200004a1 = 2;
      uVar1 = 0;
    }
  }
  return uVar1;
}


/* ==== FUN_08006F0C @ 0x8006f0c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08006F0C(void)

{
  int iVar1;
  
  if ((_DAT_200004ac != (code *)0x0) && (iVar1 = (*_DAT_200004ac)(), iVar1 == 0)) {
    _DAT_200004ac = (code *)0x0;
  }
  FUN_08007B34();
  return;
}


/* ==== FUN_08006F2C @ 0x8006f2c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006F2C(int param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined1 auStack_14 [12];
  
  if (param_1 == 1) {
    iVar1 = FUN_0800b740(param_2,"voltage");
    if (iVar1 == 0) {
      uVar2 = FUN_080068D4(_DAT_200004a4,auStack_14,0x10);
      FUN_0800A16C(2,"drvBattery",param_2,uVar2);
      uVar2 = 1;
    }
    else {
      iVar1 = FUN_0800b740(param_2,&DAT_0800c4b0);
      if (iVar1 == 0) {
        uVar2 = FUN_0800693C((int)_DAT_200004a2,auStack_14,0x10);
        FUN_0800A16C(2,"drvBattery",param_2,uVar2);
        uVar2 = 1;
      }
      else {
        iVar1 = FUN_0800b740(param_2,"current");
        if (iVar1 == 0) {
          uVar2 = FUN_0800693C((int)_DAT_200004a6,auStack_14,0x10);
          FUN_0800A16C(2,"drvBattery",param_2,uVar2);
          uVar2 = 1;
        }
        else {
          iVar1 = FUN_0800b740(param_2,"remainingCapacity");
          if (iVar1 == 0) {
            uVar2 = FUN_0800693C((int)_DAT_200004a8,auStack_14,0x10);
            FUN_0800A16C(2,"drvBattery",param_2,uVar2);
            uVar2 = 1;
          }
          else {
            iVar1 = FUN_0800b740(param_2,"chargeStat");
            uVar2 = 0;
            if (iVar1 == 0) {
              uVar2 = FUN_08007CFC(0);
              uVar2 = FUN_080068D4(uVar2,auStack_14,0x10);
              FUN_0800A16C(2,"drvBattery",param_2,uVar2);
              uVar2 = 1;
            }
          }
        }
      }
    }
  }
  else {
    uVar2 = 0;
    if (param_1 == 3) {
      iVar1 = FUN_0800b740(param_2,"hwInit");
      if (iVar1 == 0) {
        while (iVar1 = FUN_08006C44(), iVar1 != 0) {
          FUN_0800ab10();
        }
        FUN_08006E94(0x73a);
        uVar2 = 1;
      }
      else {
        uVar2 = 0;
      }
    }
  }
  return uVar2;
}


/* ==== FUN_0800705c @ 0x800705c ==== */

undefined1 * FUN_0800705c(void)

{
  return &DAT_200004a2;
}


/* ==== FUN_08007068 @ 0x8007068 ==== */

void FUN_08007068(void)

{
  undefined4 local_64;
  undefined1 local_60;
  undefined4 local_5c;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined1 local_1c;
  undefined1 local_19;
  
  if (-1 < (int)((uint)DAT_200004b0 << 0x1e)) {
    FUN_0800AD5C(0x20000,1);
    FUN_0800aaa4(&local_20);
    local_1c = 3;
    local_19 = 0;
    local_20 = 0x66;
    FUN_0800A964(0x48000000,&local_20);
    FUN_0800A67C(0x40012400);
    FUN_0800AD7C(0x200,1);
    FUN_0800ac0c(0x1004000);
    FUN_0800AD5C(1,1);
    local_4c = 0x40012440;
    local_48 = 0x200004b6;
    local_44 = 0;
    local_40 = 0x20;
    local_3c = 0;
    local_38 = 0x80;
    local_34 = 0x100;
    local_30 = 0x400;
    local_2c = 0x20;
    local_28 = 0x1000;
    local_24 = 0;
    FUN_0800A840(0x4002001c,&local_4c);
    FUN_0800AD7C(1,1);
    FUN_0800AE14(0x100,1);
    FUN_0800a6cc(&local_64);
    local_60 = 1;
    local_54 = 0;
    local_5c = 0;
    local_64 = 0;
    local_50 = 0;
    FUN_0800A6A0(0x40012400,&local_64);
    FUN_0800a6f8(0x40012400,DAT_200004b1,4);
    FUN_0800a6f8(0x40012400,DAT_200004b2,6);
    FUN_0800a6f8(0x40012400,DAT_200004b3,6);
    FUN_0800a6f8(0x40012400,DAT_200004b4,6);
    FUN_0800a77c(0x40012400,2);
    DAT_200004b0 = 2;
  }
  return;
}


/* ==== FUN_08007168 @ 0x8007168 ==== */

void FUN_08007168(void)

{
  int iVar1;
  
  if (-1 < (int)((uint)DAT_200004b0 << 0x1d)) {
    FUN_0800A704(0x40012400);
    FUN_0800A6DC(0x40012400,1);
    do {
      iVar1 = FUN_0800A78C(0x40012400,1);
    } while (iVar1 == 0);
    FUN_0800A87C(0x4002001c,1);
    FUN_0800A760(0x40012400,1);
    FUN_0800a754(0x40012400);
    DAT_200004b0 = DAT_200004b0 | 4;
  }
  return;
}


/* ==== FUN_080071C0 @ 0x80071c0 ==== */

void FUN_080071C0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  iVar1 = FUN_0800A78C(0x40012400,0x10,param_3,param_4,param_4);
  if (iVar1 != 0) {
    FUN_0800a7ac(0x40012400,0x10);
  }
  return;
}


/* ==== FUN_080071DC @ 0x80071dc ==== */

undefined8 FUN_080071DC(uint param_1,uint param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  uint uVar12;
  char cVar13;
  int iVar14;
  int iVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  uint uVar19;
  uint uVar20;
  uint uVar21;
  
  if (DAT_200004b1 == param_1) {
    cVar13 = '\0';
  }
  else if (DAT_200004b2 == param_1) {
    cVar13 = '\x01';
  }
  else if (DAT_200004b3 == param_1) {
    cVar13 = '\x02';
  }
  else {
    uVar5 = 0;
    cVar13 = '\x03';
    if (DAT_200004b4 != param_1) goto LAB_080073d4;
  }
  if (param_2 == 0) {
    iVar14 = (int)cVar13;
    uVar12 = (uint)*(ushort *)(iVar14 * 2 + 0x200004b6);
    uVar16 = (uint)*(ushort *)((iVar14 + 4) * 2 + 0x200004b6);
    uVar5 = uVar12 + uVar16;
    uVar17 = (uint)*(ushort *)((iVar14 + 8) * 2 + 0x200004b6);
    uVar1 = uVar5 + uVar17;
    uVar18 = (uint)*(ushort *)((iVar14 + 0xc) * 2 + 0x200004b6);
    uVar2 = uVar1 + uVar18;
    uVar19 = (uint)*(ushort *)((iVar14 + 0x10) * 2 + 0x200004b6);
    uVar3 = uVar2 + uVar19;
    uVar20 = (uint)*(ushort *)((iVar14 + 0x14) * 2 + 0x200004b6);
    uVar4 = uVar3 + uVar20;
    uVar21 = (uint)*(ushort *)((iVar14 + 0x18) * 2 + 0x200004b6);
    param_2 = (uint)CARRY4(uVar12,uVar16) + (uint)CARRY4(uVar5,uVar17) + (uint)CARRY4(uVar1,uVar18)
              + (uint)CARRY4(uVar2,uVar19) + (uint)CARRY4(uVar3,uVar20) + (uint)CARRY4(uVar4,uVar21)
    ;
    uVar5 = (uint)*(ushort *)((iVar14 + 0x1c) * 2 + 0x200004b6) + uVar4 + uVar21 >> 3 & 0xffff;
  }
  else {
    uVar5 = 0;
    if (param_2 == 1) {
      iVar14 = (int)cVar13;
      uVar5 = (uint)*(ushort *)(iVar14 * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar15 = uVar5 - param_3;
      }
      else {
        iVar15 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 4) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar6 = uVar5 - param_3;
      }
      else {
        iVar6 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 8) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar7 = uVar5 - param_3;
      }
      else {
        iVar7 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 0xc) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar8 = uVar5 - param_3;
      }
      else {
        iVar8 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 0x10) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar9 = uVar5 - param_3;
      }
      else {
        iVar9 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 0x14) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar10 = uVar5 - param_3;
      }
      else {
        iVar10 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 0x18) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar11 = uVar5 - param_3;
      }
      else {
        iVar11 = param_3 - uVar5;
      }
      uVar5 = (uint)*(ushort *)((iVar14 + 0x1c) * 2 + 0x200004b6);
      if (param_3 < uVar5) {
        iVar14 = uVar5 - param_3;
      }
      else {
        iVar14 = param_3 - uVar5;
      }
      param_2 = (uint)(iVar14 + iVar15 + iVar6 + iVar7 + iVar8 + iVar9 + iVar10 + iVar11) >> 3;
      uVar5 = param_2 & 0xffff;
    }
  }
LAB_080073d4:
  return CONCAT44(param_2,uVar5);
}


/* ==== FUN_080073DC @ 0x80073dc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_080073DC(int param_1,undefined4 param_2)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  byte bVar4;
  undefined1 auStack_2c [16];
  
  FUN_08003040();
  if ((param_1 != 1) || (iVar2 = FUN_0800b740(&DAT_0800c4e8,param_2), iVar2 != 0)) {
    return 0;
  }
  bVar4 = 1;
  bVar3 = 0;
  do {
    pcVar1 = _DAT_20000a10;
    FUN_080068D4(*(undefined2 *)((uint)bVar3 * 2 + 0x200004b6),auStack_2c,10);
    (*pcVar1)();
    FUN_0800A140();
    if (bVar3 != 0) {
      if ((bVar3 & 7) == 0) {
        FUN_0800A140();
      }
      if (0x1f < bVar4) {
        return 0;
      }
    }
    bVar3 = bVar3 + 1;
    bVar4 = bVar4 + 1;
  } while( true );
}


/* ==== FUN_0800745C @ 0x800745c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800745C(void)

{
  int iVar1;
  
  iVar1 = FUN_0800B3A8(0x40014000,1);
  if (iVar1 != 0) {
    if (DAT_2000051a == '\x02') {
      FUN_0800AFBC(0x40014000,0);
      DAT_2000051a = 3;
      FUN_0800B38C(0x40014000,1,0);
      FUN_0800af90(0x40014000,0);
      FUN_0800b3c0(0x40014000,1);
      FUN_0800b3c0(0x40014000,2);
      FUN_0800AFA0(0x40014000,0);
      FUN_0800B2C0(0x40014000,&DAT_20000510);
      FUN_0800af54(0x40014000,0xffff);
      FUN_0800B38C(0x40014000,2,1);
      FUN_0800AFBC(0x40014000,0);
      FUN_0800AFA0(0x40014000,1);
      FUN_0800A4DC(&DAT_20000528);
      DAT_2000051a = '\x04';
    }
    FUN_0800b3c0(0x40014000,1);
  }
  iVar1 = FUN_0800B3A8(0x40014000,2);
  if (iVar1 != 0) {
    if (DAT_2000051a == '\x04') {
      FUN_0800AFA0(0x40014000,0);
      _DAT_40014020 = _DAT_40014020 & 0xfffd;
      FUN_0800AFA0(0x40014000,1);
      DAT_2000051a = '\x05';
    }
    else if (DAT_2000051a == '\x05') {
      FUN_0800AFA0(0x40014000,0);
      FUN_0800B38C(0x40014000,2,0);
      FUN_0800B38C(0x40014000,1,0);
      if (_DAT_2000051c == 0) {
        DAT_2000051a = '\x06';
      }
      else {
        DAT_2000051a = '\0';
      }
    }
    else if (DAT_2000051a == '\a') {
      FUN_0800AFBC(0x40014000,0);
      FUN_0800B38C(0x40014000,1,0);
      FUN_0800AFA0(0x40014000,0);
      DAT_2000051a = '\f';
    }
    else if (DAT_2000051a == '\r') {
      FUN_0800AFBC(0x40014000,0);
      FUN_0800b1ec(0x40014000,0x18);
      DAT_2000051a = '\x0e';
    }
    else if (DAT_2000051a == '\x0e') {
      if (DAT_20000589 == 0) {
        DAT_2000051a = '\x10';
        FUN_0800AFBC(0x40014000,0);
        FUN_0800AFA0(0x40014000,0);
      }
      if (DAT_20000589 < 0x80) {
        iVar1 = FUN_0800aabc(0x48000400,0x4000);
        if (iVar1 == 0) {
          (&DAT_2000058a)[_DAT_20000594] =
               (&DAT_2000058a)[_DAT_20000594] & ~(byte)(1 << (_DAT_20000596 & 0xff));
        }
        else {
          (&DAT_2000058a)[_DAT_20000594] =
               (byte)(1 << (_DAT_20000596 & 0xff)) | (&DAT_2000058a)[_DAT_20000594];
        }
        _DAT_20000596 = _DAT_20000596 + 1;
        if (_DAT_20000596 == 8) {
          if ((int)(char)DAT_20000589 == _DAT_20000594 + 1) {
            FUN_0800AFBC(0x40014000,0);
            FUN_0800AFA0(0x40014000,0);
            FUN_0800B38C(0x40014000,2,0);
            DAT_2000051a = '\x10';
          }
          else {
            _DAT_20000594 = _DAT_20000594 + 1;
            _DAT_20000596 = 0;
          }
        }
        if (DAT_2000051a != '\x10') {
          FUN_0800b1ec(0x40014000,6);
          FUN_0800AFBC(0x40014000,1);
          DAT_2000051a = '\r';
        }
      }
      else {
        iVar1 = FUN_0800aabc(0x48000400,0x4000);
        if (iVar1 == 0) {
          DAT_2000058a = 0;
          FUN_0800b1ec(0x40014000,6);
          FUN_0800AFBC(0x40014000,1);
          DAT_2000051a = '\r';
        }
        else {
          DAT_2000058a = 1;
          DAT_2000051a = '\x10';
          FUN_0800AFBC(0x40014000,0);
          FUN_0800AFA0(0x40014000,0);
          FUN_0800B38C(0x40014000,2,0);
        }
      }
    }
    FUN_0800b3c0(0x40014000,2);
  }
  return;
}


/* ==== FUN_08007734 @ 0x8007734 ==== */

void FUN_08007734(void)

{
  int iVar1;
  
  iVar1 = FUN_0800a8b8(0x20000);
  if (iVar1 == 1) {
    if (DAT_2000051a == '\a') {
      FUN_0800A87C(0x40020058,0);
      FUN_0800B3C8(0x40014000,0x100,0);
      FUN_0800b3c0(0x40014000,2);
      FUN_0800B38C(0x40014000,2,1);
    }
    FUN_0800a8c8(0x20000);
  }
  iVar1 = FUN_0800a8b8(0x40000);
  if (iVar1 == 1) {
    FUN_0800a8c8(0x40000);
  }
  return;
}


/* ==== FUN_080077A0 @ 0x80077a0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080077A0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  _DAT_40014024 = 0;
  _DAT_2000051c = param_1;
  FUN_0800b3c0(0x40014000,1,param_3,param_4,param_4);
  FUN_0800b3c0(0x40014000,2);
  FUN_0800B38C(0x40014000,2,0);
  FUN_0800af90(0x40014000,8);
  _DAT_200004fc = 0x70;
  _DAT_20000508 = 2;
  FUN_0800AFDC(0x40014000,&DAT_200004fc);
  FUN_0800af54(0x40014000,0x308);
  FUN_0800b1ec(0x40014000,0);
  FUN_0800B38C(0x40014000,1,1);
  FUN_0800AFBC(0x40014000,1);
  FUN_0800AFA0(0x40014000,1);
  DAT_2000051a = 2;
  FUN_0800b260(&DAT_20000510);
  _DAT_20000510 = 0;
  _DAT_20000518 = 0xf;
  _DAT_20000512 = 2;
  _DAT_20000516 = 0;
  _DAT_20000514 = 1;
  return;
}


/* ==== FUN_08007830 @ 0x8007830 ==== */

void FUN_08007830(byte *param_1,undefined1 *param_2,int param_3)

{
  byte *pbVar1;
  
  if (param_3 != 0) {
    pbVar1 = param_1 + (param_3 - 1U & 0xff) + 1;
    do {
      if ((*param_1 & 1) == 0) {
        *param_2 = 0x60;
      }
      else {
        *param_2 = 4;
      }
      if ((*param_1 & 2) == 0) {
        param_2[1] = 0x60;
      }
      else {
        param_2[1] = 4;
      }
      if ((*param_1 & 4) == 0) {
        param_2[2] = 0x60;
      }
      else {
        param_2[2] = 4;
      }
      if ((*param_1 & 8) == 0) {
        param_2[3] = 0x60;
      }
      else {
        param_2[3] = 4;
      }
      if ((*param_1 & 0x10) == 0) {
        param_2[4] = 0x60;
      }
      else {
        param_2[4] = 4;
      }
      if ((*param_1 & 0x20) == 0) {
        param_2[5] = 0x60;
      }
      else {
        param_2[5] = 4;
      }
      if ((*param_1 & 0x40) == 0) {
        param_2[6] = 0x60;
      }
      else {
        param_2[6] = 4;
      }
      if ((char)*param_1 < '\0') {
        param_2[7] = 4;
      }
      else {
        param_2[7] = 0x60;
      }
      param_1 = param_1 + 1;
      param_2 = param_2 + 8;
    } while (param_1 != pbVar1);
  }
  return;
}


/* ==== FUN_080078E4 @ 0x80078e4 ==== */

bool FUN_080078E4(undefined4 param_1,undefined4 param_2,undefined1 param_3)

{
  bool bVar1;
  
  bVar1 = DAT_2000051a == '\0';
  if (bVar1) {
    FUN_08007830(param_1,0x20000530,param_2);
    DAT_20000588 = (undefined1)param_2;
    DAT_20000589 = param_3;
    FUN_080077A0(0);
  }
  return bVar1;
}


/* ==== FUN_0800791C @ 0x800791c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800791C(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  _DAT_200004fc = 0x60;
  _DAT_20000508 = 2;
  FUN_0800AFDC(0x40014000,&DAT_200004fc,param_3,0x60,param_4);
  FUN_0800af90(0x40014000,0);
  FUN_0800af54(0x40014000,0x66);
  FUN_0800b1ec(0x40014000,6);
  FUN_0800B38C(0x40014000,2,1);
  FUN_0800B38C(0x40014000,1,0);
  DAT_2000051a = 0xd;
  FUN_0800AFBC(0x40014000,1);
  FUN_0800AFA0(0x40014000,1);
  return;
}


/* ==== FUN_08007980 @ 0x8007980 ==== */

bool FUN_08007980(void)

{
  return 1 < DAT_2000051a;
}


/* ==== FUN_080079A0 @ 0x80079a0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080079A0(void)

{
  undefined1 *local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined1 local_40;
  undefined1 local_3f;
  undefined1 local_3e;
  undefined2 local_3c;
  undefined2 local_3a;
  undefined4 local_38;
  undefined2 local_34;
  undefined1 local_32;
  undefined4 local_30;
  undefined1 local_2c;
  undefined1 local_2b;
  undefined1 local_2a;
  undefined1 local_29;
  
  if (-1 < (int)((uint)DAT_200004f8 << 0x1e)) {
    FUN_0800AD5C(0x40000,1);
    local_2c = 2;
    local_2a = 1;
    local_29 = 0;
    local_2b = 3;
    local_30 = 0x4000;
    FUN_0800A964(0x48000400,&local_30);
    local_2c = 0;
    local_29 = 0;
    local_30 = 0x1000;
    FUN_0800A964(0x48000400,&local_30);
    local_29 = 1;
    local_30 = 0x8000;
    FUN_0800A964(0x48000400,&local_30);
    FUN_0800AD7C(0x10000,1);
    FUN_0800AAC8(0x48000400,0xe,1);
    local_3a = 0;
    local_3c = 0x1d;
    local_38 = 0;
    local_32 = 0;
    local_34 = 0;
    FUN_0800AEC4();
    local_40 = 0x14;
    local_3e = 1;
    local_3f = 3;
    FUN_0800AB30(&local_40);
    FUN_0800b1fc(0x40014000,0);
    FUN_0800AF74(0x40014000,0);
    FUN_0800b1d8(&DAT_200004fc);
    _DAT_200004fc = 0x70;
    _DAT_2000050c = 0x100;
    _DAT_200004fe = 1;
    _DAT_20000508 = 2;
    _DAT_20000504 = 0;
    FUN_0800AFDC(0x40014000,&DAT_200004fc);
    FUN_0800AD5C(1,1);
    FUN_0800A7B0();
    local_6c = &DAT_40014034;
    local_68 = 0x20000530;
    local_64 = 0x10;
    local_60 = 1;
    local_5c = 0;
    local_58 = 0x80;
    local_54 = 0x100;
    local_50 = 0;
    local_4c = 0;
    local_48 = 0x3000;
    local_44 = 0;
    FUN_0800A840(0x40020058,&local_6c);
    local_40 = 0xb;
    local_3e = 1;
    local_3f = 4;
    FUN_0800AB30(&local_40);
    FUN_0800A8A0(0x40020058,2,1);
    DAT_200004f8 = 2;
  }
  return;
}


/* ==== FUN_08007B00 @ 0x8007b00 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08007B00(void)

{
  undefined4 in_r3;
  
  if ((DAT_200004f8 & 6) == 2) {
    FUN_0800B244(0x40014000,0,1,&DAT_200004f8,in_r3);
    _DAT_20000528 = 0;
    _DAT_2000052c = 0;
    DAT_200004f8 = DAT_200004f8 | 4;
  }
  return;
}


/* ==== FUN_08007B34 @ 0x8007b34 ==== */

// WARNING: Removing unreachable block (ram,0x08007b54)
// WARNING: Removing unreachable block (ram,0x08007ba8)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08007B34(void)

{
  undefined4 in_r3;
  
  if (DAT_2000051a == '\x04') {
    FUN_0800A500(&DAT_20000528,&DAT_20000f80);
    if ((_DAT_20000f84 != 0) || (0x46 < _DAT_20000f80)) {
      FUN_0800B38C(0x40014000,2,0);
      FUN_0800AFA0(0x40014000,0);
      DAT_20000520 = 0;
      DAT_2000051a = '\x01';
      (*_DAT_20000a10)("PPRT\n");
    }
  }
  else if (DAT_2000051a == '\x05') {
    FUN_0800A500(&DAT_20000528,&DAT_20000f80);
    if ((_DAT_20000f84 != 0) || (0x13b < _DAT_20000f80)) {
      FUN_0800B38C(0x40014000,2,0);
      FUN_0800AFA0(0x40014000,0);
      DAT_20000520 = 1;
      DAT_2000051a = '\x01';
      (*_DAT_20000a10)("PPRET\n");
    }
  }
  else if (DAT_2000051a == '\x06') {
    if ((_DAT_40014034 < 0x5a) || (0x187 < _DAT_40014034)) {
      DAT_20000520 = 2;
      DAT_2000051a = '\x01';
    }
    else {
      FUN_0800af54(0x40014000,0x66,&DAT_200004f8,0x187,in_r3);
      _DAT_200004fc = 0x60;
      _DAT_2000050c = 0x100;
      _DAT_200004fe = 1;
      _DAT_20000508 = 2;
      _DAT_20000504 = 0;
      FUN_0800AFDC(0x40014000,&DAT_200004fc);
      FUN_0800B3C8(0x40014000,0x100,1);
      FUN_0800a89c(0x40020058,(uint)DAT_20000588 << 3);
      FUN_0800b1ec(0x40014000,0xffff);
      FUN_0800A87C(0x40020058,1);
      _DAT_40014024 = 0;
      FUN_0800AFA0(0x40014000,1);
      FUN_0800b3a4(0x40014000,1);
      FUN_0800AFBC(0x40014000,1);
      DAT_2000051a = '\a';
    }
  }
  else if (DAT_2000051a == '\f') {
    if (DAT_20000589 == '\0') {
      FUN_080077A0(1);
    }
    else {
      _DAT_20000594 = 0;
      _DAT_20000596 = 0;
      FUN_0800791C();
    }
  }
  else if (DAT_2000051a == '\x10') {
    FUN_080077A0(1);
  }
  return;
}


/* ==== FUN_08007CE4 @ 0x8007ce4 ==== */

bool FUN_08007CE4(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  iVar1 = FUN_0800aabc(0x48000400,0x8000,param_3,param_4,param_4);
  return iVar1 == 0;
}


/* ==== FUN_08007CFC @ 0x8007cfc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08007CFC(void)

{
  int iVar1;
  undefined4 uVar2;
  
  _DAT_4800140c = _DAT_4800140c & 0xffffcfff | 0x1000;
  FUN_08002DD4(10);
  iVar1 = FUN_0800aabc(0x48001400,0x40);
  if (iVar1 == 0) {
    _DAT_4800140c = _DAT_4800140c & 0xffffcfff;
    uVar2 = 0;
  }
  else {
    _DAT_4800140c = _DAT_4800140c & 0xffffcfff | 0x2000;
    FUN_08002DD4(10);
    iVar1 = FUN_0800aabc(0x48001400,0x40);
    if (iVar1 == 0) {
      _DAT_4800140c = _DAT_4800140c & 0xffffcfff;
      uVar2 = 1;
    }
    else {
      _DAT_4800140c = _DAT_4800140c & 0xffffcfff;
      uVar2 = 2;
    }
  }
  return uVar2;
}


/* ==== FUN_08007d7c @ 0x8007d7c ==== */

undefined1 * FUN_08007d7c(void)

{
  return &DAT_2000058a;
}


/* ==== FUN_08007D88 @ 0x8007d88 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08007D88(int param_1,byte *param_2,int param_3,byte *param_4)

{
  int iVar1;
  byte *pbVar2;
  
  _DAT_20000f88 = (param_1 + param_3) * 0x528;
  FUN_0800A4DC(0x20000f98);
  _DAT_20000f90 = 0;
  _DAT_20000f94 = 0;
  if ((param_1 != 0) && (pbVar2 = param_2 + (param_1 - 1U & 0xff) + 1, _DAT_20000f88 != 0)) {
LAB_08007dfe:
    do {
      iVar1 = FUN_0800b53c(0x40004800,0x80);
      if (iVar1 == 0) {
        FUN_0800A500(0x20000f98,&DAT_20000f90);
        if ((_DAT_20000f94 == 0) && (_DAT_20000f90 <= _DAT_20000f88)) goto LAB_08007dfe;
      }
      _DAT_40004828 = (ushort)*param_2;
      param_2 = param_2 + 1;
      FUN_0800A500(0x20000f98,&DAT_20000f90);
      if ((param_2 == pbVar2) || ((_DAT_20000f94 != 0 || (_DAT_20000f88 <= _DAT_20000f90)))) break;
    } while( true );
  }
  pbVar2 = param_4 + (param_3 - 1U & 0xffff) + 1;
  if (param_3 != 0) {
LAB_08007e6a:
    do {
      iVar1 = FUN_0800b53c(0x40004800,0x80);
      if ((iVar1 == 0) && (FUN_0800A500(0x20000f98,&DAT_20000f90), _DAT_20000f94 == 0)) {
        if (_DAT_20000f90 <= _DAT_20000f88) goto LAB_08007e6a;
      }
      _DAT_40004828 = (ushort)*param_4;
      param_4 = param_4 + 1;
      FUN_0800A500(0x20000f98,&DAT_20000f90);
    } while (param_4 != pbVar2);
  }
  if ((_DAT_20000f94 != 0) || (_DAT_20000f88 < _DAT_20000f90)) {
    (*_DAT_20000a10)("ERR:BLE_serout_TO\n");
  }
  return;
}


/* ==== FUN_08007EC8 @ 0x8007ec8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08007EC8(void)

{
  undefined1 uVar1;
  int iVar2;
  
  iVar2 = FUN_0800B54C(0x40004800,0x50105);
  if (iVar2 == 1) {
    _DAT_2000062c = _DAT_4000481c & 0x1f | _DAT_2000062c & 0xffffffe0;
    if (((_DAT_4000481c & 0x1f) == 0) || ((int)(_DAT_4000481c << 0x1b) < 0)) {
      if (_DAT_20000622 < _DAT_20000620) {
        uVar1 = FUN_0800b4e4(0x40004800);
        disableIRQinterrupts();
        if (_DAT_20000622 < _DAT_20000620) {
          enableIRQinterrupts();
          *_DAT_20000628 = uVar1;
          _DAT_20000628 = _DAT_20000628 + 1;
          if (_DAT_20000628 == _DAT_2000061c + _DAT_20000620) {
            _DAT_20000628 = _DAT_2000061c;
          }
          disableIRQinterrupts();
          _DAT_20000622 = _DAT_20000622 + 1;
          enableIRQinterrupts();
        }
      }
      else {
        _DAT_2000062c = _DAT_2000062c | 0x20;
        FUN_0800B4EC(0x40004800,0x50105,0);
      }
    }
    else {
      FUN_0800B524(0x40004800,8,1);
    }
  }
  return;
}


/* ==== FUN_08007F7C @ 0x8007f7c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08007F7C(void)

{
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  undefined1 local_19;
  
  if (-1 < (int)((uint)DAT_20000598 << 0x1e)) {
    FUN_0800AD5C(0xe0000,1);
    FUN_0800AD9C(0x40000,1);
    local_1b = 3;
    local_1c = 2;
    local_1a = 0;
    local_19 = 0;
    local_20 = 0x2002;
    FUN_0800A964(0x48000400,&local_20);
    local_20 = 0x30;
    FUN_0800A964(0x48000800,&local_20);
    FUN_0800AAC8(0x48000800,4,1);
    FUN_0800AAC8(0x48000800,5,1);
    FUN_0800AAC8(0x48000400,0xd,4);
    FUN_0800AAC8(0x48000400,1,4);
    local_20 = 0x400;
    local_1c = 0;
    local_1a = 0;
    local_19 = 0;
    FUN_0800A964(0x48000000,&local_20);
    local_38 = 0x3840;
    local_34 = 0;
    local_30 = 0x2000;
    local_2c = 0;
    local_24 = 0x300;
    local_28 = 0xc;
    FUN_0800B3FC(0x40004800,&local_38);
    _DAT_20000f24 = 0x8007d89;
    FUN_08002E1C(&DAT_2000061c,0x20000599,0x80);
    DAT_20000598 = 2;
  }
  return;
}


/* ==== FUN_0800806C @ 0x800806c ==== */

void FUN_0800806C(void)

{
  if (-1 < (int)((uint)DAT_20000598 << 0x1d)) {
    FUN_0800B4EC(0x40004800,0x50105,1);
    FUN_0800B4C0(0x40004800,1);
    FUN_08002f38(&DAT_2000061c);
    DAT_20000598 = DAT_20000598 | 4;
  }
  return;
}


/* ==== FUN_080080AC @ 0x80080ac ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_080080AC(undefined2 *param_1)

{
  byte bVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  int iVar4;
  uint local_20;
  int local_1c;
  undefined1 auStack_18 [8];
  
  FUN_0800A4DC(auStack_18);
  while( true ) {
    disableIRQinterrupts();
    enableIRQinterrupts();
    if (_DAT_20000622 == 0) {
      return 0;
    }
    FUN_0800A500(auStack_18,&local_20);
    if ((local_1c != 0) || (10000 < local_20)) break;
    uVar3 = FUN_08002E44(&DAT_2000061c);
    *(undefined1 *)param_1 = *(undefined1 *)((int)param_1 + 1);
    bVar1 = *(byte *)(param_1 + 1);
    *(byte *)((int)param_1 + 1) = bVar1;
    uVar2 = *(undefined1 *)((int)param_1 + 3);
    *(undefined1 *)(param_1 + 1) = uVar2;
    *(undefined1 *)((int)param_1 + 3) = uVar3;
    if ((bVar1 < 0x81) &&
       (iVar4 = FUN_08002A60(CONCAT13(*(undefined1 *)((int)param_1 + 3),CONCAT12(uVar2,*param_1))),
       iVar4 != 0)) {
      return 1;
    }
  }
  return 0;
}


/* ==== FUN_08008130 @ 0x8008130 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008130(void)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint local_28;
  int local_24;
  uint local_20;
  int local_1c;
  
  FUN_0800A558(0,&local_20);
  if (((local_1c != 0) || (2000 < local_20)) && (_DAT_20000630 == 0)) {
    FUN_08002ac0(0x39,2,2);
    FUN_08002ac0(0x32,1);
    _DAT_20000630 = 1;
  }
  if (-1 < (int)(_DAT_2000062c << 0x19)) {
    disableIRQinterrupts();
    enableIRQinterrupts();
    if ((3 < _DAT_20000622) && (iVar1 = FUN_08002E78(&DAT_2000061c,&DAT_20000fa0,4), iVar1 == 4)) {
      if (DAT_20000fa1 < 0x81) {
        _DAT_2000062c = _DAT_2000062c | 0x40;
        FUN_0800A4DC(0x20000638);
      }
      else {
        _DAT_20000640 = _DAT_20000640 + 1;
      }
    }
    if (-1 < (int)(_DAT_2000062c << 0x19)) goto LAB_0800827c;
  }
  FUN_0800A500(0x20000638,&local_28);
  if ((local_24 == 0) && (local_28 < 0x3a99)) {
    iVar1 = FUN_08002A60(_DAT_20000fa0);
    if (iVar1 == 0) {
      iVar1 = FUN_080080AC(&DAT_20000fa0);
      if (iVar1 == 0) {
        _DAT_2000062c = _DAT_2000062c & 0xffffffbf;
        _DAT_20000640 = 0;
      }
      else {
        FUN_0800A4DC(0x20000638);
        _DAT_20000640 = 0;
      }
    }
    else {
      disableIRQinterrupts();
      enableIRQinterrupts();
      uVar3 = _DAT_20000fa0 >> 8 & 0xff;
      if (uVar3 <= _DAT_20000622) {
        uVar2 = FUN_08002E78(&DAT_2000061c);
        if (uVar2 == (_DAT_20000fa0 >> 8 & 0xff)) {
          (**(code **)(iVar1 + 8))((int)&local_28 - (uVar3 + 7 & 0xfffffff8));
          _DAT_20000640 = 0;
        }
        _DAT_2000062c = _DAT_2000062c & 0xffffffbf;
      }
    }
  }
  else {
    _DAT_2000062c = _DAT_2000062c & 0xffffffbf;
  }
LAB_0800827c:
  if ((int)(_DAT_2000062c << 0x1e) < 0) {
    FUN_0800A2D0(4,"BLE framing error",0);
    FUN_0800b548(0x40004800,2);
    _DAT_2000062c = _DAT_2000062c & 0xfffffffd;
  }
  if ((int)(_DAT_2000062c << 0x1b) < 0) {
    FUN_0800b548(0x40004800,0x10);
    _DAT_2000062c = _DAT_2000062c & 0xffffffef;
  }
  if ((int)(_DAT_2000062c << 0x1d) < 0) {
    FUN_0800A2D0(4,"BLE USART noise",0);
    FUN_0800b548(0x40004800,4);
    _DAT_2000062c = _DAT_2000062c & 0xfffffffb;
  }
  if ((int)(_DAT_2000062c << 0x1c) < 0) {
    FUN_0800A2D0(4,"BLE USART overrun",0);
    FUN_0800b548(0x40004800,8);
    _DAT_2000062c = _DAT_2000062c & 0xfffffff7;
  }
  if (5 < _DAT_20000640) {
    FUN_08002f38(&DAT_2000061c);
    _DAT_20000640 = 0;
  }
  if (((int)(_DAT_2000062c << 0x1a) < 0) && (_DAT_20000622 < _DAT_20000620)) {
    _DAT_2000062c = _DAT_2000062c & 0xffffffdf;
    FUN_0800B4EC(0x40004800,0x50105,1);
  }
  return;
}


/* ==== FUN_0800837C @ 0x800837c ==== */

undefined4 FUN_0800837C(void)

{
  return 0;
}


/* ==== FUN_08008380 @ 0x8008380 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008380(void)

{
  undefined2 local_40;
  undefined2 local_3e;
  undefined2 local_3c;
  undefined4 local_38;
  undefined2 local_34;
  undefined2 local_32;
  undefined2 local_30;
  undefined2 local_2e;
  undefined2 local_2c;
  undefined2 local_2a;
  undefined4 local_28;
  undefined2 local_24;
  undefined1 local_22;
  undefined4 local_20;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  undefined1 local_19;
  
  if (-1 < (int)((uint)DAT_20000648 << 0x1e)) {
    _DAT_200006e0 = 0;
    _DAT_200006e2 = 0;
    FUN_0800AD5C(0x40000,1);
    local_1c = 2;
    local_1a = 0;
    local_19 = 0;
    local_1b = 3;
    local_20 = 0x100;
    FUN_0800A964(0x48000400);
    FUN_0800AD7C(0x20000,1);
    FUN_0800AAC8(0x48000400,8,2);
    local_2a = 0;
    local_2c = 2;
    local_28 = 24000;
    local_22 = 0;
    local_24 = 0;
    FUN_0800AEC4(0x40014400);
    FUN_0800b1fc(0x40014400,8);
    FUN_0800AF74(0x40014400,1);
    local_40 = 0x60;
    local_30 = 0;
    local_2e = 0;
    local_34 = 0;
    local_32 = 0;
    local_3e = 1;
    local_3c = 0;
    local_38 = 0;
    FUN_0800AFDC(0x40014400,&local_40);
    _DAT_200006ec = 0;
    DAT_20000648 = 2;
  }
  return;
}


/* ==== FUN_08008440 @ 0x8008440 ==== */

void FUN_08008440(void)

{
  if ((DAT_20000648 & 6) == 2) {
    FUN_0800B244(0x40014400,0,1);
    FUN_0800b1ec(0x40014400,0);
    FUN_0800b3a4(0x40014400,1);
    FUN_0800AFA0(0x40014400,1);
    FUN_0800AFBC(0x40014400,1);
    DAT_20000648 = DAT_20000648 | 4;
  }
  return;
}


/* ==== FUN_08008490 @ 0x8008490 ==== */

undefined4 FUN_08008490(void)

{
  FUN_08003040();
  return 0;
}


/* ==== FUN_0800849C @ 0x800849c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800849C(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  _DAT_4001442c = FUN_08000128(_DAT_20000000 >> 1,param_1,param_3,0x20000000,param_4);
  _DAT_4001442c = _DAT_4001442c + -1;
  FUN_0800b3a4(0x40014400,1);
  return;
}


/* ==== FUN_080084C0 @ 0x80084c0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080084C0(undefined2 param_1,undefined2 param_2,undefined1 param_3,int param_4)

{
  int iVar1;
  uint local_20;
  int local_1c;
  
  if (_DAT_200006e2 != 10) {
    iVar1 = (uint)_DAT_200006e2 * 6;
    *(undefined2 *)(iVar1 + 0x20000650) = param_1;
    *(undefined2 *)(iVar1 + 0x20000652) = param_2;
    *(undefined1 *)(iVar1 + 0x20000654) = param_3;
    FUN_0800A4DC(&local_20);
    iVar1 = (_DAT_200006e2 + 9) * 8;
    *(uint *)(&DAT_20000648 + iVar1) = param_4 * 1000 + local_20;
    *(uint *)(iVar1 + 0x2000064c) = local_1c + (uint)CARRY4(param_4 * 1000,local_20);
    _DAT_200006e2 = _DAT_200006e2 + 1;
  }
  return;
}


/* ==== FUN_08008514 @ 0x8008514 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008514(int param_1)

{
  undefined4 uVar1;
  
  uVar1 = FUN_08000128(_DAT_4001442c * param_1,100);
  FUN_0800b1ec(0x40014400,uVar1);
  FUN_0800b3a4(0x40014400,1);
  return;
}


/* ==== FUN_08008538 @ 0x8008538 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008538(int param_1)

{
  FUN_08008514(0);
  _DAT_200006ec = 0;
  _DAT_200006e0 = _DAT_200006e0 + 1;
  if ((param_1 != 0) || (_DAT_200006e2 == _DAT_200006e0)) {
    _DAT_200006e0 = 0;
    _DAT_200006e2 = 0;
  }
  return;
}


/* ==== FUN_08008574 @ 0x8008574 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008574(void)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint local_10;
  int local_c;
  
  FUN_0800A4DC(&DAT_20000fa8);
  if (_DAT_200006ec != 0) {
    FUN_0800A558(0x20000fb0,&local_10);
    if ((local_c != 0) || (*(ushort *)((uint)_DAT_200006e0 * 6 + 0x20000652) < local_10)) {
      FUN_08008514(0);
      _DAT_200006e0 = _DAT_200006e0 + 1;
      if (_DAT_200006e2 == _DAT_200006e0) {
        _DAT_200006e0 = 0;
        _DAT_200006e2 = 0;
      }
      _DAT_200006ec = 0;
      if (_DAT_200006e8 != 0) {
        FUN_08006828();
        _DAT_200006e8 = 0;
      }
    }
    if (_DAT_200006ec != 0) {
      return;
    }
  }
  uVar3 = (uint)_DAT_200006e0;
  if (uVar3 < _DAT_200006e2) {
    iVar2 = (uVar3 + 9) * 8;
    uVar1 = *(uint *)(iVar2 + 0x2000064c);
    if ((uVar1 <= _DAT_20000fac) &&
       ((uVar1 != _DAT_20000fac || (*(uint *)(&DAT_20000648 + iVar2) <= _DAT_20000fa8)))) {
      FUN_0800849C(*(undefined2 *)(uVar3 * 6 + 0x20000650));
      FUN_08008514(*(byte *)((uint)_DAT_200006e0 * 6 + 0x20000654) >> 1);
      FUN_0800A4DC(0x20000fb0);
      _DAT_200006ec = 1;
    }
  }
  return;
}


/* ==== FUN_08008668 @ 0x8008668 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4
FUN_08008668(undefined4 *param_1,undefined1 *param_2,undefined4 param_3,undefined4 param_4)

{
  FUN_08003040();
  if (_DAT_200006e8 != 0) {
    FUN_08008538(1);
    FUN_08006828(_DAT_200006e8,1);
    _DAT_200006e8 = 0;
  }
  FUN_080084C0(*(undefined2 *)(param_2 + 1),*(undefined2 *)(param_2 + 3),*param_2,0,param_4);
  _DAT_200006e8 = *param_1;
  *param_1 = 0;
  return 0;
}


/* ==== FUN_080086C4 @ 0x80086c4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080086C4(void)

{
  int iVar1;
  
  iVar1 = FUN_0800a948(0x1000);
  if (iVar1 != 0) {
    iVar1 = FUN_0800aabc(0x48000000,0x1000);
    _DAT_200006f4 = (uint)(iVar1 == 0);
    FUN_0800a958(0x1000);
  }
  return;
}


/* ==== FUN_08008700 @ 0x8008700 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008700(void)

{
  int iVar1;
  undefined4 local_20;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_19;
  
  if (-1 < (int)((uint)DAT_200006f0 << 0x1e)) {
    FUN_0800AD5C(0x20000,1);
    local_1c = 0;
    local_19 = 1;
    local_1b = 0;
    local_20 = 0x1000;
    FUN_0800A964(0x48000000,&local_20);
    FUN_0800AD7C(1,1);
    FUN_0800AE34(0,0xc);
    _DAT_20000fb8 = 0x1000;
    DAT_20000fbc = 0;
    DAT_20000fbd = 0x10;
    DAT_20000fbe = 1;
    FUN_0800A8D4();
    iVar1 = FUN_0800aabc(0x48000000,0x1000);
    _DAT_200006f4 = (uint)(iVar1 == 0);
    DAT_200006f0 = 2;
  }
  return;
}


/* ==== FUN_08008774 @ 0x8008774 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008774(int param_1)

{
  if (param_1 != 0) {
    _DAT_200006f8 = param_1;
  }
  return;
}


/* ==== FUN_08008784 @ 0x8008784 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008784(void)

{
  int iVar1;
  undefined4 in_r3;
  
  if (_DAT_200006f4 != _DAT_20000fc0) {
    iVar1 = FUN_0800aabc(0x48000000,0x1000,0x80,0x90,in_r3);
    _DAT_200006f4 = (uint)(iVar1 == 0);
    if (_DAT_200006f4 != _DAT_20000fc0) {
      if (_DAT_200006f4 == 0) {
        (*_DAT_200006f8)(1);
      }
      else {
        (*_DAT_200006f8)(2);
      }
      _DAT_20000fc0 = _DAT_200006f4;
    }
  }
  return;
}


/* ==== FUN_080087D8 @ 0x80087d8 ==== */

void FUN_080087D8(void)

{
  undefined2 local_40;
  undefined2 local_3e;
  undefined2 local_3c;
  undefined4 local_38;
  undefined2 local_34;
  undefined2 local_32;
  undefined2 local_30;
  undefined2 local_2e;
  undefined2 local_2c;
  undefined2 local_2a;
  undefined4 local_28;
  undefined2 local_24;
  undefined1 local_22;
  undefined4 local_20;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  undefined1 local_19;
  
  if (-1 < (int)((uint)DAT_200006fc << 0x1e)) {
    FUN_0800AD5C(0x80000,1);
    FUN_0800aaa4(&local_20);
    local_1c = 2;
    local_1a = 0;
    local_19 = 0;
    local_1b = 3;
    local_20 = 0x3c0;
    FUN_0800A964(0x48000800,&local_20);
    local_1c = 1;
    local_19 = 0;
    local_1b = 0;
    local_20 = 4;
    FUN_0800A964(0x48000800,&local_20);
    local_1c = 0;
    local_19 = 1;
    local_20 = 2;
    FUN_0800A964(0x48000800,&local_20);
    FUN_0800AAC8(0x48000800,6,0);
    FUN_0800AAC8(0x48000800,7,0);
    FUN_0800AAC8(0x48000800,8,0);
    FUN_0800AAC8(0x48000800,9,0);
    FUN_0800AD9C(2,1);
    FUN_0800b1fc(0x40000400,8);
    FUN_0800b208(0x40000400,8);
    FUN_0800b220(0x40000400,8);
    FUN_0800b22c(0x40000400,8);
    FUN_0800AF74(0x40000400,0);
    local_2a = 0x20;
    local_2c = 0;
    local_28 = 1000;
    local_22 = 0;
    local_24 = 0;
    FUN_0800AEC4(0x40000400);
    local_40 = 0x60;
    local_30 = 0x100;
    local_2e = 0x200;
    local_34 = 0;
    local_32 = 8;
    local_3e = 1;
    local_3c = 0;
    local_38 = 0;
    FUN_0800AFDC(0x40000400,&local_40);
    FUN_0800B054(0x40000400,&local_40);
    FUN_0800B0F4(0x40000400,&local_40);
    FUN_0800B174(0x40000400,&local_40);
    DAT_200006fc = 2;
  }
  return;
}


/* ==== FUN_08008910 @ 0x8008910 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008910(void)

{
  if ((DAT_200006fc & 6) == 2) {
    _DAT_48000818 = 4;
    FUN_0800B244(0x40000400,0,1);
    FUN_0800B244(0x40000400,4,1);
    FUN_0800B244(0x40000400,8,1);
    FUN_0800B244(0x40000400,0xc,1);
    FUN_0800b3a4(0x40000400,1);
    FUN_0800AFA0(0x40000400,1);
    FUN_0800AFBC(0x40000400,1);
    DAT_200006fc = DAT_200006fc | 4;
  }
  return;
}


/* ==== FUN_08008980 @ 0x8008980 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008980(void)

{
  FUN_0800AFA0(0x40000400,0);
  FUN_0800AFBC(0x40000400,0);
  _DAT_48000828 = 4;
  DAT_200006fc = DAT_200006fc & 0xfb;
  return;
}


/* ==== FUN_080089B0 @ 0x80089b0 ==== */

int FUN_080089B0(int param_1)

{
  char cVar1;
  
  cVar1 = DAT_200006fe;
  if ((-1 < param_1 << 0x1f) && (cVar1 = '\0', param_1 << 0x1e < 0)) {
    cVar1 = DAT_200006fd;
  }
  return (int)cVar1;
}


/* ==== FUN_080089D0 @ 0x80089d0 ==== */

void FUN_080089D0(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = param_4;
  if (1000 < (int)param_1) {
    uVar1 = 0xfa;
    param_1 = 1000;
  }
  if ((short)param_1 < -1000) {
    param_1 = 0xfffffc18;
  }
  iVar2 = (int)(short)param_1;
  if (iVar2 < 1) {
    if (iVar2 < 0) {
      FUN_0800b1ec(0x40000400,0,param_1,uVar1,param_4);
      FUN_0800b1f0(0x40000400,-iVar2 & 0xffff);
    }
    else {
      FUN_0800b1ec(0x40000400,(uint)DAT_20000700 * 10,param_1,(uint)DAT_20000700,param_4);
      FUN_0800b1f0(0x40000400,(uint)DAT_20000700 * 10);
    }
  }
  else {
    FUN_0800b1ec(0x40000400,param_1 & 0xffff,param_1,uVar1,param_4);
    FUN_0800b1f0(0x40000400,0);
  }
  return;
}


/* ==== FUN_08008A50 @ 0x8008a50 ==== */

void FUN_08008A50(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = param_4;
  if (1000 < (int)param_1) {
    uVar1 = 0xfa;
    param_1 = 1000;
  }
  if ((short)param_1 < -1000) {
    param_1 = 0xfffffc18;
  }
  iVar2 = (int)(short)param_1;
  if (iVar2 < 1) {
    if (iVar2 < 0) {
      FUN_0800b1f4(0x40000400,0,param_1,uVar1,param_4);
      FUN_0800b1f8(0x40000400,-iVar2 & 0xffff);
    }
    else {
      FUN_0800b1f4(0x40000400,(uint)DAT_200006ff * 10,param_1,(uint)DAT_200006ff,param_4);
      FUN_0800b1f8(0x40000400,(uint)DAT_200006ff * 10);
    }
  }
  else {
    FUN_0800b1f4(0x40000400,param_1 & 0xffff,param_1,uVar1,param_4);
    FUN_0800b1f8(0x40000400,0);
  }
  return;
}


/* ==== FUN_08008AD0 @ 0x8008ad0 ==== */

void FUN_08008AD0(undefined4 param_1,int param_2)

{
  if ((short)-(short)param_1 < 1) {
    if ((short)-(short)param_1 < 0) {
      if (param_2 << 0x1f < 0) {
        DAT_200006fe = 0xff;
      }
      if (param_2 << 0x1e < 0) {
        DAT_200006fd = 0xff;
      }
    }
    else {
      if (param_2 << 0x1f < 0) {
        DAT_200006fe = 1;
      }
      if (param_2 << 0x1e < 0) {
        DAT_200006fd = 1;
      }
    }
  }
  else {
    if (param_2 << 0x1f < 0) {
      DAT_200006fe = 1;
    }
    if (param_2 << 0x1e < 0) {
      DAT_200006fd = 1;
    }
  }
  if (param_2 << 0x1f < 0) {
    FUN_08008A50();
  }
  if (param_2 << 0x1e < 0) {
    FUN_080089D0(param_1);
  }
  return;
}


/* ==== FUN_08008B3C @ 0x8008b3c ==== */

undefined4 FUN_08008B3C(int param_1,undefined4 param_2,undefined4 param_3)

{
  short sVar1;
  int iVar2;
  undefined4 uVar3;
  
  if (param_1 == 2) {
    iVar2 = FUN_0800b740(param_2,"dutyA");
    if (iVar2 == 0) {
      sVar1 = FUN_080069D4(param_3,0x10);
      FUN_080089D0((int)sVar1);
      uVar3 = 1;
    }
    else {
      iVar2 = FUN_0800b740(param_2,"dutyB");
      uVar3 = 0;
      if (iVar2 == 0) {
        sVar1 = FUN_080069D4(param_3,0x10);
        FUN_08008A50((int)sVar1);
        uVar3 = 1;
      }
    }
  }
  else {
    uVar3 = 0;
    if ((param_1 == 1) && (iVar2 = FUN_0800b740(param_2,"nFault"), iVar2 == 0)) {
      FUN_0800aabc(0x48000800,0x4000);
      FUN_0800A16C(2,"hwiDrive",param_2);
      uVar3 = 1;
    }
  }
  return uVar3;
}


/* ==== FUN_08008BD4 @ 0x8008bd4 ==== */

void FUN_08008BD4(byte param_1,int param_2)

{
  if (100 < param_1) {
    param_1 = 100;
  }
  if (param_2 << 0x1f < 0) {
    DAT_200006ff = param_1;
  }
  if (param_2 << 0x1e < 0) {
    DAT_20000700 = param_1;
  }
  return;
}


/* ==== FUN_08008BF8 @ 0x8008bf8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008BF8(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  iVar1 = FUN_0800B3A8(0x40012c00,1,param_3,param_4,param_4);
  if (iVar1 != 0) {
    _DAT_2000070c = _DAT_2000070c + 1;
    FUN_0800b3c0(0x40012c00,1);
  }
  return;
}


/* ==== FUN_08008C20 @ 0x8008c20 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008C20(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  iVar1 = FUN_0800B3A8(0x40000000,1,param_3,param_4,param_4);
  if (iVar1 != 0) {
    _DAT_20000708 = _DAT_20000708 + 1;
    FUN_0800b3c0(0x40000000,1);
  }
  return;
}


/* ==== FUN_08008C48 @ 0x8008c48 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008C48(void)

{
  undefined2 local_38;
  undefined2 local_36;
  undefined4 local_34;
  undefined2 local_30;
  undefined1 local_2c;
  undefined1 local_2b;
  undefined1 local_2a;
  undefined4 local_28;
  undefined1 local_24;
  undefined1 local_23;
  undefined1 local_22;
  undefined1 local_21;
  
  if (-1 < (int)((uint)DAT_20000704 << 0x1e)) {
    FUN_0800AD7C(0x800,1);
    FUN_0800AD9C(1,1);
    FUN_0800AD5C(0x20000,1);
    local_28 = 0x101;
    local_24 = 2;
    local_21 = 0;
    local_23 = 3;
    FUN_0800A964(0x48000000,&local_28);
    FUN_0800AAC8(0x48000000,0,2);
    FUN_0800AAC8(0x48000000,8,2);
    local_28 = 0x8000;
    local_24 = 1;
    local_22 = 0;
    local_21 = 0;
    local_23 = 0;
    FUN_0800A964(0x48000000,&local_28);
    _DAT_48000028 = 0x8000;
    local_2c = 0xd;
    local_2b = 2;
    local_2a = 1;
    FUN_0800AB30(&local_2c);
    local_34 = 0xffff;
    local_38 = 0;
    local_30 = 0;
    local_36 = 0;
    FUN_0800AEC4(0x40012c00,&local_38);
    FUN_0800af4c(0x40012c00,0,1);
    _DAT_40012c18 = _DAT_40012c18 & 0xff0c | 0x21;
    _DAT_40012c20 = _DAT_40012c20 & 0xfffe | 0xb;
    FUN_0800b3ec(0x40012c00,7);
    FUN_0800b3e0(0x40012c00,0x50);
    FUN_0800B38C(0x40012c00,1,1);
    local_2c = 0xf;
    local_2b = 2;
    local_2a = 1;
    FUN_0800AB30(&local_2c);
    FUN_0800AEC4(0x40000000,&local_38);
    FUN_0800af4c(0x40000000,0,1);
    _DAT_40000018 = _DAT_40000018 & 0xff0c | 0x21;
    _DAT_40000020 = _DAT_40000020 & 0xfffe | 0xb;
    FUN_0800b3ec(0x40000000,7);
    FUN_0800b3e0(0x40000000,0x50);
    FUN_0800B38C(0x40000000,1,1);
    DAT_20000704 = 2;
  }
  return;
}


/* ==== FUN_08008DBC @ 0x8008dbc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008DBC(void)

{
  undefined4 in_r3;
  
  if ((DAT_20000704 & 6) == 2) {
    _DAT_48000018 = 0x8000;
    FUN_0800AFA0(0x40012c00,1,0x48000000,0x90,in_r3);
    FUN_0800AFA0(0x40000000,1);
    DAT_20000704 = DAT_20000704 | 4;
  }
  return;
}


/* ==== FUN_08008DFC @ 0x8008dfc ==== */

undefined4 FUN_08008DFC(void)

{
  return 0;
}


/* ==== FUN_08008E00 @ 0x8008e00 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

uint FUN_08008E00(int param_1)

{
  uint uVar1;
  
  if (param_1 == 1) {
    uVar1 = (_DAT_40000024 & 0xffff) + _DAT_20000708 * 0x10000;
  }
  else if (param_1 == 2) {
    uVar1 = (_DAT_40012c24 & 0xffff) + _DAT_2000070c * 0x10000;
  }
  else {
    uVar1 = 0;
    if (param_1 == 0x20) {
      uVar1 = (_DAT_40000024 & 0xffff) + _DAT_20000708 * 0x10000 + _DAT_2000070c * 0x10000 +
              (_DAT_40012c24 & 0xffff) >> 1;
    }
  }
  return uVar1;
}


/* ==== FUN_08008E70 @ 0x8008e70 ==== */

// WARNING: Removing unreachable block (ram,0x08008e82)

undefined4 FUN_08008E70(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  ulonglong uVar2;
  
  uVar2 = FUN_08008E00(param_3);
  if (CONCAT44(param_2,param_1) < uVar2) {
    uVar1 = FUN_08000128(((int)uVar2 - param_1) * 0x30,10);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}


/* ==== FUN_08008E9C @ 0x8008e9c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08008E9C(void)

{
  int iVar1;
  
  iVar1 = FUN_0800B3A8(0x40014800,1);
  if (iVar1 != 0) {
    FUN_0800AFA0(0x40014800,0);
    FUN_0800A87C(0x40020008,0);
    FUN_0800B38C(0x40014800,1,0);
    _DAT_2000085c = 0;
    FUN_0800b3c0(0x40014800,1);
  }
  return;
}


/* ==== FUN_08008EE8 @ 0x8008ee8 ==== */

void FUN_08008EE8(void)

{
  int iVar1;
  
  iVar1 = FUN_0800a8b8(2);
  if (iVar1 == 1) {
    FUN_0800b3c0(0x40014800,1);
    FUN_0800B38C(0x40014800,1,1);
    FUN_0800a8c8(2);
  }
  return;
}


/* ==== FUN_08008F14 @ 0x8008f14 ==== */

void FUN_08008F14(void)

{
  undefined4 local_78;
  undefined1 *local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined2 local_4c;
  undefined2 local_4a;
  undefined4 local_44;
  undefined2 local_40;
  undefined2 local_3c;
  undefined1 local_38;
  undefined1 local_37;
  undefined1 local_36;
  undefined2 local_34;
  undefined2 local_32;
  undefined4 local_30;
  undefined2 local_2c;
  undefined1 local_2a;
  undefined4 local_28;
  undefined1 local_24;
  undefined1 local_23;
  undefined1 local_22;
  
  if (-1 < (int)((uint)DAT_20000710 << 0x1e)) {
    FUN_0800AD5C(0x40000,1);
    FUN_0800aaa4(&local_28);
    local_24 = 2;
    local_22 = 0;
    local_23 = 3;
    local_28 = 0x200;
    FUN_0800A964(0x48000400,&local_28);
    FUN_0800AAC8(0x48000400,9,2);
    FUN_0800AD7C(0x40000,1);
    local_32 = 0;
    local_34 = 0;
    local_30 = 0x3b;
    local_2a = 0;
    local_2c = 0;
    FUN_0800AEC4(0x40014800);
    local_38 = 0x16;
    local_36 = 1;
    local_37 = 3;
    FUN_0800AB30(&local_38);
    FUN_0800b1fc(0x40014800,8);
    FUN_0800AF74(0x40014800,0);
    FUN_0800b1d8(&local_4c);
    local_4c = 0x60;
    local_3c = 0x100;
    local_4a = 1;
    local_40 = 0;
    local_44 = 0;
    FUN_0800AFDC(0x40014800,&local_4c);
    FUN_0800AD5C(1,1);
    FUN_0800A7B0(0x40020008);
    local_78 = 0x40014834;
    local_74 = &DAT_20000711;
    local_70 = 0x10;
    local_6c = 0;
    local_68 = 0;
    local_64 = 0x80;
    local_60 = 0x100;
    local_5c = 0;
    local_58 = 0;
    local_54 = 0x2000;
    local_50 = 0;
    FUN_0800A840(0x40020008,&local_78);
    FUN_0800B3C8(0x40014800,0x100,1);
    local_38 = 9;
    local_36 = 1;
    local_37 = 2;
    FUN_0800AB30(&local_38);
    FUN_0800A8A0(0x40020008,2,1);
    DAT_20000710 = 2;
  }
  return;
}


/* ==== FUN_08009060 @ 0x8009060 ==== */

void FUN_08009060(void)

{
  if (-1 < (int)((uint)DAT_20000710 << 0x1d)) {
    FUN_0800B244(0x40014800,0,1);
    FUN_0800AFBC(0x40014800,1);
    DAT_20000710 = DAT_20000710 | 4;
  }
  return;
}


/* ==== FUN_08009094 @ 0x8009094 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08009094(void)

{
  return _DAT_2000085c;
}


/* ==== FUN_080090A4 @ 0x80090a4 ==== */

undefined4 FUN_080090A4(void)

{
  return 0;
}


/* ==== FUN_080090A8 @ 0x80090a8 ==== */

void FUN_080090A8(int param_1)

{
  int extraout_r1;
  byte *pbVar1;
  undefined1 *puVar2;
  int iVar3;
  
  puVar2 = &DAT_20000710;
  iVar3 = 0;
  do {
    FUN_08000270(iVar3 + 0xb,0xc);
    pbVar1 = (byte *)(extraout_r1 * 3 + param_1);
    if ((*pbVar1 & 0x80) == 0) {
      puVar2[1] = 0xe;
    }
    else {
      puVar2[1] = 0x2e;
    }
    if ((*pbVar1 & 0x40) == 0) {
      puVar2[2] = 0xe;
    }
    else {
      puVar2[2] = 0x2e;
    }
    if ((*pbVar1 & 0x20) == 0) {
      puVar2[3] = 0xe;
    }
    else {
      puVar2[3] = 0x2e;
    }
    if ((*pbVar1 & 0x10) == 0) {
      puVar2[4] = 0xe;
    }
    else {
      puVar2[4] = 0x2e;
    }
    if ((*pbVar1 & 8) == 0) {
      puVar2[5] = 0xe;
    }
    else {
      puVar2[5] = 0x2e;
    }
    if ((*pbVar1 & 4) == 0) {
      puVar2[6] = 0xe;
    }
    else {
      puVar2[6] = 0x2e;
    }
    if ((*pbVar1 & 2) == 0) {
      puVar2[7] = 0xe;
    }
    else {
      puVar2[7] = 0x2e;
    }
    if ((*pbVar1 & 1) == 0) {
      puVar2[8] = 0xe;
    }
    else {
      puVar2[8] = 0x2e;
    }
    if ((pbVar1[1] & 0x80) == 0) {
      puVar2[9] = 0xe;
    }
    else {
      puVar2[9] = 0x2e;
    }
    if ((pbVar1[1] & 0x40) == 0) {
      puVar2[10] = 0xe;
    }
    else {
      puVar2[10] = 0x2e;
    }
    if ((pbVar1[1] & 0x20) == 0) {
      puVar2[0xb] = 0xe;
    }
    else {
      puVar2[0xb] = 0x2e;
    }
    if ((pbVar1[1] & 0x10) == 0) {
      puVar2[0xc] = 0xe;
    }
    else {
      puVar2[0xc] = 0x2e;
    }
    if ((pbVar1[1] & 8) == 0) {
      puVar2[0xd] = 0xe;
    }
    else {
      puVar2[0xd] = 0x2e;
    }
    if ((pbVar1[1] & 4) == 0) {
      puVar2[0xe] = 0xe;
    }
    else {
      puVar2[0xe] = 0x2e;
    }
    if ((pbVar1[1] & 2) == 0) {
      puVar2[0xf] = 0xe;
    }
    else {
      puVar2[0xf] = 0x2e;
    }
    if ((pbVar1[1] & 1) == 0) {
      puVar2[0x10] = 0xe;
    }
    else {
      puVar2[0x10] = 0x2e;
    }
    if ((pbVar1[2] & 0x80) == 0) {
      puVar2[0x11] = 0xe;
    }
    else {
      puVar2[0x11] = 0x2e;
    }
    if ((pbVar1[2] & 0x40) == 0) {
      puVar2[0x12] = 0xe;
    }
    else {
      puVar2[0x12] = 0x2e;
    }
    if ((pbVar1[2] & 0x20) == 0) {
      puVar2[0x13] = 0xe;
    }
    else {
      puVar2[0x13] = 0x2e;
    }
    if ((pbVar1[2] & 0x10) == 0) {
      puVar2[0x14] = 0xe;
    }
    else {
      puVar2[0x14] = 0x2e;
    }
    if ((pbVar1[2] & 8) == 0) {
      puVar2[0x15] = 0xe;
    }
    else {
      puVar2[0x15] = 0x2e;
    }
    if ((pbVar1[2] & 4) == 0) {
      puVar2[0x16] = 0xe;
    }
    else {
      puVar2[0x16] = 0x2e;
    }
    if ((pbVar1[2] & 2) == 0) {
      puVar2[0x17] = 0xe;
    }
    else {
      puVar2[0x17] = 0x2e;
    }
    if ((pbVar1[2] & 1) == 0) {
      puVar2[0x18] = 0xe;
    }
    else {
      puVar2[0x18] = 0x2e;
    }
    iVar3 = iVar3 + 1;
    puVar2 = puVar2 + 0x18;
  } while (iVar3 != 0xc);
  puVar2 = &DAT_20000831;
  do {
    *puVar2 = 0;
    puVar2[1] = 0;
    puVar2 = puVar2 + 2;
  } while (puVar2 != (undefined1 *)0x2000084f);
  return;
}


/* ==== FUN_08009268 @ 0x8009268 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_08009268(void)

{
  undefined4 in_r3;
  bool bVar1;
  
  bVar1 = _DAT_2000085c == 0;
  if (bVar1) {
    FUN_0800b3c0(0x40014800,2,&DAT_20000710,0xa6,in_r3);
    FUN_0800b3c0(0x40014800,1);
    FUN_0800a89c(0x40020008,0x13e);
    FUN_0800b1ec(0x40014800,0);
    FUN_0800A87C(0x40020008,1);
    _DAT_40014824 = 0;
    FUN_0800AFA0(0x40014800,1);
    FUN_0800b3a4(0x40014800,1);
    _DAT_2000085c = 1;
  }
  return bVar1;
}


/* ==== FUN_080092D4 @ 0x80092d4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080092D4(void)

{
  FUN_0800AFA0(0x40014800,0);
  FUN_0800A87C(0x40020008,0);
  FUN_0800B38C(0x40014800,1,0);
  _DAT_2000085c = 0;
  return;
}


/* ==== FUN_0800930C @ 0x800930c ==== */

void FUN_0800930C(void)

{
  if (-1 < (int)((uint)DAT_20000860 << 0x1e)) {
    DAT_20000860 = 2;
  }
  return;
}


/* ==== FUN_08009324 @ 0x8009324 ==== */

void FUN_08009324(void)

{
  if ((DAT_20000860 & 6) == 2) {
    DAT_20000860 = DAT_20000860 | 4;
  }
  return;
}


/* ==== FUN_08009340 @ 0x8009340 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009340(void)

{
  int iVar1;
  char cVar2;
  char cVar3;
  uint local_18;
  int local_14;
  
  local_18 = 0;
  local_14 = 0;
  FUN_0800A558(0x20000fc8,&local_18);
  iVar1 = _DAT_20000864;
  if ((local_14 != 0) || (_DAT_2000086a < local_18)) {
    _DAT_20000864 = FUN_080071DC(4,0,0);
    _DAT_20000864 = _DAT_20000864 + iVar1;
    cVar3 = DAT_2000086c + '\x01';
    cVar2 = DAT_2000086c + '\x01';
    DAT_2000086c = cVar3;
    if (DAT_20000868 == cVar2) {
      _DAT_2000086e = FUN_08000128();
      _DAT_20000864 = 0;
      DAT_2000086c = '\0';
    }
    FUN_0800A4DC(0x20000fc8);
  }
  return;
}


/* ==== FUN_080093A8 @ 0x80093a8 ==== */

undefined4 FUN_080093A8(void)

{
  return 0;
}


/* ==== FUN_080093AC @ 0x80093ac ==== */

undefined4 FUN_080093AC(undefined4 param_1)

{
  FUN_08003040();
  FUN_08006848(param_1,2,&DAT_2000086e);
  return 0;
}


/* ==== FUN_080093C8 @ 0x80093c8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080093C8(void)

{
  undefined4 local_18;
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  undefined1 local_11;
  
  if (-1 < (int)((uint)DAT_20000870 << 0x1e)) {
    FUN_0800AD5C(0x20000,1);
    local_14 = 1;
    local_12 = 0;
    local_11 = 0;
    local_13 = 0;
    local_18 = 0x10;
    FUN_0800A964(0x48000000,&local_18);
    _DAT_48000028 = 0x10;
    DAT_20000870 = 2;
  }
  return;
}


/* ==== FUN_0800940C @ 0x800940c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800940C(void)

{
  if (-1 < (int)((uint)DAT_20000870 << 0x1d)) {
    _DAT_48000018 = 0x10;
    DAT_20000870 = DAT_20000870 | 4;
  }
  return;
}


/* ==== FUN_0800942c @ 0x800942c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800942c(void)

{
  _DAT_48000028 = 0x10;
  DAT_20000870 = DAT_20000870 & 0xfb;
  return;
}


/* ==== FUN_08009444 @ 0x8009444 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

int FUN_08009444(void)

{
  int iVar1;
  int iVar2;
  undefined4 in_r3;
  int iVar3;
  
  iVar3 = (int)_DAT_20000872;
  iVar1 = FUN_080071DC(0x20,0,0,in_r3,in_r3);
  iVar2 = FUN_080071DC(0x40,0,0);
  iVar1 = (iVar3 - iVar1) + (iVar2 - _DAT_20000874);
  return (int)(short)((uint)(iVar1 - (iVar1 >> 0x1f)) >> 1);
}


/* ==== FUN_08009478 @ 0x8009478 ==== */

undefined4 FUN_08009478(void)

{
  FUN_08003040();
  return 0;
}


/* ==== FUN_08009484 @ 0x8009484 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08009484(undefined4 param_1)

{
  short local_14;
  short local_12;
  
  local_14 = FUN_080071DC(0x20,0,0);
  local_14 = local_14 - _DAT_20000872;
  local_12 = FUN_080071DC(0x40,0,0);
  local_12 = local_12 - _DAT_20000874;
  FUN_08006848(param_1,4,&local_14);
  return 0;
}


/* ==== FUN_080094C4 @ 0x80094c4 ==== */

void FUN_080094C4(void)

{
  if (-1 < (int)((uint)DAT_20000880 << 0x1e)) {
    DAT_20000880 = 2;
  }
  return;
}


/* ==== FUN_080094DC @ 0x80094dc ==== */

void FUN_080094DC(void)

{
  if ((DAT_20000880 & 6) == 2) {
    DAT_20000880 = DAT_20000880 | 4;
  }
  return;
}


/* ==== FUN_080094F8 @ 0x80094f8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080094F8(void)

{
  undefined8 uVar1;
  byte bVar2;
  undefined2 uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  bool bVar13;
  uint local_28;
  int local_24;
  
  FUN_0800A558(0x20000fd0,&local_28);
  if ((local_24 == 0) && (uVar1 = CONCAT44(_DAT_200008ac,_DAT_200008a8), local_28 <= _DAT_200008c2))
  goto LAB_080095cc;
  uVar10 = (uint)DAT_200008a4;
  uVar3 = FUN_080071DC(2,1,_DAT_20000882);
  *(undefined2 *)(uVar10 * 2 + 0x20000884) = uVar3;
  bVar2 = DAT_200008a4;
  uVar10 = (uint)DAT_200008a4;
  uVar12 = (uint)DAT_200008c0;
  if (uVar12 == 0) {
    uVar7 = 0;
    uVar4 = 0;
  }
  else {
    uVar5 = uVar12 - 1 & 0xff;
    uVar7 = (uint)*(ushort *)(uVar10 * 2 + 0x20000884);
    uVar4 = 0;
    uVar9 = uVar5;
    if (uVar10 != 0) {
      uVar9 = uVar10 - 1 & 0xff;
    }
    uVar11 = 1;
    if (uVar12 != 1) {
      if ((uVar12 - 1 & 1) != 0) {
        uVar4 = (uint)*(ushort *)(uVar9 * 2 + 0x20000884);
        bVar13 = CARRY4(uVar7,uVar4);
        uVar7 = uVar7 + uVar4;
        uVar4 = (uint)bVar13;
        uVar11 = uVar5;
        if (uVar9 != 0) {
          uVar11 = uVar9 - 1 & 0xff;
        }
        uVar9 = uVar11;
        uVar11 = 2;
        if (uVar12 == 2) goto LAB_080095a0;
      }
      do {
        uVar8 = (uint)*(ushort *)(uVar9 * 2 + 0x20000884);
        bVar13 = CARRY4(uVar7,uVar8);
        uVar8 = uVar7 + uVar8;
        uVar6 = uVar5;
        if (uVar9 != 0) {
          uVar6 = uVar9 - 1 & 0xff;
        }
        uVar9 = (uint)*(ushort *)(uVar6 * 2 + 0x20000884);
        uVar7 = uVar8 + uVar9;
        uVar4 = uVar4 + bVar13 + (uint)CARRY4(uVar8,uVar9);
        uVar9 = uVar5;
        if (uVar6 != 0) {
          uVar9 = uVar6 - 1 & 0xff;
        }
        uVar11 = (uVar11 + 1 & 0xff) + 1 & 0xff;
      } while (uVar11 != uVar12);
    }
  }
LAB_080095a0:
  _DAT_200008a8 = FUN_080002fc(uVar7,uVar4,uVar12,0);
  if (uVar10 == uVar12) {
    DAT_200008a4 = 0;
  }
  else {
    DAT_200008a4 = bVar2 + 1;
  }
  FUN_0800A4DC(0x20000fd0);
  uVar1 = _DAT_200008a8;
LAB_080095cc:
  _DAT_200008ac = (undefined4)((ulonglong)uVar1 >> 0x20);
  _DAT_200008a8 = (undefined4)uVar1;
  return;
}


/* ==== FUN_0800962C @ 0x800962c ==== */

undefined4 FUN_0800962C(void)

{
  FUN_08003040();
  return 0;
}


/* ==== FUN_08009638 @ 0x8009638 ==== */

undefined4 FUN_08009638(undefined4 param_1)

{
  FUN_08003040();
  FUN_08006848(param_1,2,&DAT_200008a8);
  return 0;
}


/* ==== FUN_08009654 @ 0x8009654 ==== */

void FUN_08009654(void)

{
  undefined4 local_28;
  undefined1 local_24;
  undefined1 local_23;
  undefined1 local_22;
  undefined1 local_21;
  
  if (-1 < (int)((uint)DAT_200008c8 << 0x1e)) {
    FUN_0800AD5C(0x80000,1);
    local_24 = 0;
    local_21 = 2;
    local_23 = 1;
    local_28 = 0x2000;
    FUN_0800A964(0x48000800,&local_28);
    local_24 = 1;
    local_22 = 0;
    local_21 = 0;
    local_23 = 0;
    local_28 = 0xd000;
    FUN_0800A964(0x48000800,&local_28);
    FUN_0800AD7C(1,1);
    FUN_0800AE34(2,0xd);
    FUN_0800A8D4();
    FUN_0800AB30();
    DAT_200008c8 = 2;
  }
  return;
}


/* ==== FUN_080096EC @ 0x80096ec ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080096EC(void)

{
  if (DAT_200008c9 == '\x01') {
    FUN_0800A558(0x200008d0,&DAT_20000fd8);
    if ((DAT_200008ca == '\0') && ((_DAT_20000fdc != 0 || (800 < _DAT_20000fd8)))) {
      DAT_200008ca = '\0';
      DAT_200008c9 = '\0';
      (*_DAT_200008ec)();
    }
  }
  else {
    DAT_200008c9 = '\0';
  }
  if (DAT_200008ca == '\x01') {
    if (DAT_200008c9 == '\x01') {
      (*_DAT_200008f0)();
    }
    DAT_200008c9 = '\0';
    DAT_200008ca = '\0';
  }
  return;
}


/* ==== FUN_08009758 @ 0x8009758 ==== */

void FUN_08009758(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  FUN_0800aabc(0x48000800,0x2000,param_3,param_4,param_4);
  return;
}


/* ==== FUN_0800976C @ 0x800976c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800976C(void)

{
  int iVar1;
  
  iVar1 = FUN_0800a948(0x2000);
  if (iVar1 != 0) {
    iVar1 = FUN_08009758();
    if (iVar1 == 0) {
      FUN_0800A4DC(0x200008d8);
      DAT_200008ca = DAT_200008ca + '\x01';
      _DAT_4001040c = _DAT_4001040c & 0xffffdfff;
      _DAT_40010408 = _DAT_40010408 | 0x2000;
    }
    else {
      FUN_0800A4DC(0x200008d0);
      DAT_200008c9 = DAT_200008c9 + '\x01';
      _DAT_4001040c = _DAT_4001040c | 0x2000;
      _DAT_40010408 = _DAT_40010408 & 0xffffdfff;
    }
    FUN_0800a958(0x2000);
  }
  return;
}


/* ==== FUN_080097E8 @ 0x80097e8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080097E8(int param_1)

{
  int iVar1;
  
  if (param_1 != 0) {
    while (iVar1 = FUN_0800aabc(0x48000800,0x2000), iVar1 != 0) {
      FUN_0800ab10();
    }
  }
  _DAT_48000828 = 0x1000;
  do {
    FUN_0800ab10();
  } while( true );
}


/* ==== FUN_0800981C @ 0x800981c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800981C(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined8 uVar2;
  
  uVar2 = CONCAT44(param_2,param_1);
  if (param_1 == 3) {
    iVar1 = FUN_0800b740(param_2,"pwrOff");
    if (iVar1 != 0) {
      return 0;
    }
    uVar2 = FUN_080097E8();
  }
  if (((int)uVar2 == 2) &&
     (iVar1 = FUN_0800b740((int)((ulonglong)uVar2 >> 0x20),"Vsens"), iVar1 == 0)) {
    iVar1 = FUN_0800A084(param_3);
    if (iVar1 == 0) {
      _DAT_48000818 = 0x8000;
    }
    else {
      _DAT_48000828 = 0x8000;
    }
  }
  return 0;
}


/* ==== FUN_08009874 @ 0x8009874 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009874(void)

{
  do {
    FUN_0800A558(0x200008e0,&DAT_20000fe0);
    if (_DAT_20000fe4 != 0) break;
  } while (_DAT_20000fe0 < 0x191);
  if (DAT_200008ca != '\0') {
    _DAT_200008e8 = 0;
    DAT_200008ca = 0;
    DAT_200008c9 = 0;
    FUN_080097E8(0);
  }
  _DAT_200008e8 = 1;
  DAT_200008ca = 0;
  DAT_200008c9 = 0;
  return;
}


/* ==== FUN_080098C4 @ 0x80098c4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_080098C4(void)

{
  int iVar1;
  
  if (-1 < (int)((uint)DAT_200008c8 << 0x1d)) {
    _DAT_48000818 = 0x1000;
    iVar1 = FUN_0800aabc(0x48000800,0x2000);
    if (iVar1 == 0) {
      _DAT_200008e8 = 0;
      DAT_200008ca = 0;
      DAT_200008c9 = 0;
      FUN_080097E8();
    }
    else {
      DAT_200008ca = 0;
      DAT_200008c9 = 0;
      _DAT_4001040c = _DAT_4001040c | 0x2000;
      _DAT_40010408 = _DAT_40010408 & 0xffffdfff;
      FUN_0800A4DC(0x200008e0);
      DAT_200008c8 = DAT_200008c8 | 4;
    }
  }
  return;
}


/* ==== FUN_08009930 @ 0x8009930 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009930(undefined4 param_1)

{
  _DAT_200008ec = param_1;
  return;
}


/* ==== FUN_0800993c @ 0x800993c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800993c(undefined4 param_1)

{
  _DAT_200008f0 = param_1;
  return;
}


/* ==== FUN_08009948 @ 0x8009948 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009948(void)

{
  _DAT_48000818 = 0x4000;
  return;
}


/* ==== FUN_08009958 @ 0x8009958 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009958(void)

{
  _DAT_48000828 = 0x8000;
  return;
}


/* ==== FUN_08009968 @ 0x8009968 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009968(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  int iVar2;
  
  iVar2 = FUN_0800B54C(0x40004c00,0x50105,param_3,param_4,param_4);
  if (iVar2 == 1) {
    _DAT_200009d4 = _DAT_40004c1c & 0x1f | _DAT_200009d4 & 0xffffffe0;
    cVar1 = FUN_0800b4e4();
    disableIRQinterrupts();
    if (_DAT_200009ca < _DAT_200009c8) {
      enableIRQinterrupts();
      *_DAT_200009d0 = cVar1;
      _DAT_200009d0 = _DAT_200009d0 + 1;
      if (_DAT_200009d0 == _DAT_200009c4 + _DAT_200009c8) {
        _DAT_200009d0 = _DAT_200009c4;
      }
      disableIRQinterrupts();
      _DAT_200009ca = _DAT_200009ca + 1;
      enableIRQinterrupts();
    }
    else {
      (*_DAT_20000a10)("BUFOVF!!!!!\n");
    }
    if (cVar1 == '\n') {
      DAT_200009d8 = DAT_200009d8 + '\x01';
    }
  }
  return;
}


/* ==== FUN_08009A04 @ 0x8009a04 ==== */

void FUN_08009A04(void)

{
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  undefined1 local_19;
  
  if (-1 < (int)((uint)DAT_200008f8 << 0x1e)) {
    FUN_0800AD5C(0x80000,1);
    FUN_0800AD9C(0x80000,1);
    local_1b = 3;
    local_1c = 2;
    local_1a = 0;
    local_19 = 1;
    local_20 = 0xc00;
    FUN_0800A964(0x48000800);
    FUN_0800AAC8(0x48000800,10,0);
    FUN_0800AAC8(0x48000800,0xb,0);
    local_38 = 0x70800;
    local_34 = 0;
    local_30 = 0;
    local_2c = 0;
    local_24 = 0;
    local_28 = 0xc;
    FUN_0800B3FC(0x40004c00,&local_38);
    FUN_0800AB30();
    FUN_08002E1C(&DAT_200009c4,0x200008f9,200);
    DAT_200008f8 = 2;
  }
  return;
}


/* ==== FUN_08009AAC @ 0x8009aac ==== */

void FUN_08009AAC(void)

{
  if (-1 < (int)((uint)DAT_200008f8 << 0x1d)) {
    FUN_0800B4EC(0x40004c00,0x50105,1);
    FUN_0800B4C0(0x40004c00,1);
    DAT_200008f8 = DAT_200008f8 | 4;
  }
  return;
}


/* ==== FUN_08009AE4 @ 0x8009ae4 ==== */

void FUN_08009AE4(void)

{
  FUN_0800B4EC(0x40004c00,0x50105,0);
  FUN_0800B4C0(0x40004c00,0);
  DAT_200008f8 = DAT_200008f8 & 0xfb;
  return;
}


/* ==== FUN_08009B14 @ 0x8009b14 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

int FUN_08009B14(undefined1 *param_1,int param_2)

{
  int iVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  
  if (DAT_200009d8 == '\0') {
    if (_DAT_200009c8 <= _DAT_200009ca) {
      FUN_08002f38(&DAT_200009c4);
    }
    goto LAB_08009bbc;
  }
  disableIRQinterrupts();
  enableIRQinterrupts();
  if (_DAT_200009ca == 0) {
LAB_08009b9a:
    DAT_200009d8 = DAT_200009d8 + -1;
LAB_08009bbc:
    if (_DAT_200009d4 << 0x1e < 0) {
      FUN_0800A2D0(4,"USART framing error",0);
      FUN_0800b548(0x40004c00,2);
    }
    if (_DAT_200009d4 << 0x1b < 0) {
      FUN_0800b548(0x40004c00,0x10);
    }
    if (_DAT_200009d4 << 0x1d < 0) {
      FUN_0800A2D0(4,"USART noise",0);
      FUN_0800b548(0x40004c00,4);
    }
    if (_DAT_200009d4 << 0x1c < 0) {
      FUN_0800A2D0(4,"USART overrun",0);
      FUN_0800b548(0x40004c00,8);
    }
    return 0;
  }
  iVar1 = FUN_08002E44(&DAT_200009c4);
  *param_1 = (char)iVar1;
  puVar3 = param_1;
  if (iVar1 == 10) {
LAB_08009b38:
    *puVar3 = 0;
    DAT_200009d8 = DAT_200009d8 + -1;
    return (int)puVar3 - (int)param_1;
  }
  puVar2 = param_1 + 1;
  if ((int)puVar2 - (int)param_1 == param_2) goto LAB_08009b60;
  if (((uint)(param_1 + param_2 + ~(uint)param_1) & 1) != 0) {
    disableIRQinterrupts();
    enableIRQinterrupts();
    if (_DAT_200009ca == 0) goto LAB_08009b9a;
    iVar1 = FUN_08002E44(&DAT_200009c4);
    *puVar2 = (char)iVar1;
    puVar3 = puVar2;
    if (iVar1 == 10) goto LAB_08009b38;
    puVar2 = param_1 + 2;
    if ((int)puVar2 - (int)param_1 == param_2) goto LAB_08009b60;
  }
  do {
    disableIRQinterrupts();
    enableIRQinterrupts();
    if (_DAT_200009ca == 0) goto LAB_08009b9a;
    iVar1 = FUN_08002E44(&DAT_200009c4);
    *puVar2 = (char)iVar1;
    puVar3 = puVar2;
    if (iVar1 == 10) goto LAB_08009b38;
    puVar3 = puVar2 + 1;
    disableIRQinterrupts();
    enableIRQinterrupts();
    if (_DAT_200009ca == 0) goto LAB_08009b9a;
    iVar1 = FUN_08002E44(&DAT_200009c4);
    *puVar3 = (char)iVar1;
    if (iVar1 == 10) goto LAB_08009b38;
    puVar2 = puVar2 + 2;
  } while ((int)puVar2 - (int)param_1 != param_2);
LAB_08009b60:
  *param_1 = 0;
  return param_2;
}


/* ==== FUN_08009CA0 @ 0x8009ca0 ==== */

void FUN_08009CA0(undefined4 param_1)

{
  int iVar1;
  
  if ((int)((uint)DAT_200008f8 << 0x1d) < 0) {
    do {
      iVar1 = FUN_0800b53c(0x40004c00,0x80);
    } while (iVar1 == 0);
    FUN_0800b4dc(0x40004c00,param_1);
  }
  return;
}


/* ==== FUN_08009CCC @ 0x8009ccc ==== */

int FUN_08009CCC(char *param_1)

{
  int iVar1;
  
  iVar1 = 0;
  if ((int)((uint)DAT_200008f8 << 0x1d) < 0) {
    if (*param_1 == '\0') {
      iVar1 = 0;
    }
    else {
      do {
        FUN_08009CA0();
        iVar1 = iVar1 + 1;
      } while (param_1[iVar1] != '\0');
    }
  }
  return iVar1;
}


/* ==== FUN_08009CF8 @ 0x8009cf8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009CF8(int param_1)

{
  if (param_1 == 0) {
    FUN_08006848(&DAT_20000a0c,2,&DAT_200009e4);
  }
  else {
    FUN_08006848(&DAT_20000a0c,2);
  }
  _DAT_20000a0c = 0;
  return;
}


/* ==== FUN_08009D30 @ 0x8009d30 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009D30(void)

{
  int iVar1;
  
  iVar1 = FUN_0800a948(0x20);
  if (iVar1 != 0) {
    iVar1 = FUN_0800aabc(0x48000400,0x20);
    if (iVar1 == 0) {
      if ((int)(_DAT_20000a00 << 0x1f) < 0) {
        if (_DAT_200009ec != 0 || _DAT_200009e8 != 0) {
          FUN_0800A4DC(&DAT_200009f0);
          _DAT_20000a00 = _DAT_20000a00 | 2;
        }
      }
    }
    else {
      FUN_0800A4DC(&DAT_200009e8);
    }
    FUN_0800a958(0x20);
  }
  return;
}


/* ==== FUN_08009D88 @ 0x8009d88 ==== */

void FUN_08009D88(void)

{
  undefined4 local_28;
  undefined1 local_24;
  undefined1 local_23;
  undefined1 local_22;
  undefined4 local_20;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  undefined1 local_19;
  
  if (-1 < (int)((uint)DAT_200009e0 << 0x1e)) {
    FUN_0800AD5C(0x60000,1);
    local_1c = 0;
    local_19 = 2;
    local_1b = 3;
    local_20 = 0x20;
    FUN_0800A964(0x48000400,&local_20);
    local_1c = 1;
    local_1a = 0;
    local_19 = 0;
    local_20 = 0x80;
    FUN_0800A964(0x48000000,&local_20);
    FUN_0800AD7C(1,1);
    FUN_0800AE34(1,5);
    local_28 = 0x20;
    local_24 = 0;
    local_23 = 0x10;
    local_22 = 1;
    FUN_0800A8D4(&local_28);
    DAT_200009e0 = 2;
  }
  return;
}


/* ==== FUN_08009E0C @ 0x8009e0c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009E0C(void)

{
  if ((DAT_200009e0 & 6) == 2) {
    _DAT_48000028 = 0x80;
    DAT_200009e0 = DAT_200009e0 | 4;
  }
  return;
}


/* ==== FUN_08009E30 @ 0x8009e30 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08009E30(undefined4 param_1)

{
  undefined4 uVar1;
  
  if ((int)(_DAT_20000a00 << 0x1f) < 0) {
    DAT_20000a08 = 2;
    uVar1 = 2;
  }
  else {
    if (_DAT_20000a0c != 0) {
      FUN_08006848(&DAT_20000a0c,2);
    }
    _DAT_48000018 = 0x80;
    FUN_08002DD4(8);
    _DAT_48000028 = 0x80;
    _DAT_20000a00 = _DAT_20000a00 | 1;
    FUN_0800A4DC(0x200009f8);
    _DAT_200009e8 = 0;
    _DAT_200009ec = 0;
    _DAT_200009f0 = 0;
    _DAT_200009f4 = 0;
    uVar1 = 0;
    _DAT_20000a04 = param_1;
  }
  return uVar1;
}


/* ==== FUN_08009EA0 @ 0x8009ea0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009EA0(void)

{
  int iVar1;
  uint local_18;
  int local_14;
  
  if (((int)(_DAT_20000a00 << 0x1f) < 0) &&
     ((FUN_0800A558(0x200009f8,&local_18), local_14 != 0 || (0x37 < local_18)))) {
    DAT_20000a08 = 3;
    _DAT_20000a00 = _DAT_20000a00 & 0xfffffffe;
    if (_DAT_20000a04 != (code *)0x0) {
      (*_DAT_20000a04)(3);
    }
    _DAT_48000018 = 0x80;
    FUN_08002DD4(8);
    _DAT_48000028 = 0x80;
  }
  if ((int)(_DAT_20000a00 << 0x1e) < 0) {
    iVar1 = FUN_0800A5C8(&DAT_200009f0,&DAT_200009e8,&local_18);
    if (iVar1 != 0) {
      _DAT_200009e4 =
           FUN_080002fc(local_18 * 10,
                        -((int)(local_18 * 5) >> 0x1f) |
                        (local_14 + (local_14 << 2 | local_18 >> 0x1e) +
                        (uint)CARRY4(local_18,local_18 * 4)) * 2,0x3a,0);
    }
    _DAT_20000a00 = _DAT_20000a00 & 0xfffffffc;
    if (_DAT_20000a04 != (code *)0x0) {
      (*_DAT_20000a04)(0);
    }
  }
  return;
}


/* ==== FUN_08009F4C @ 0x8009f4c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08009F4C(undefined4 *param_1)

{
  int iVar1;
  
  FUN_08003040();
  iVar1 = FUN_08009E30(0x8009cf9);
  if (iVar1 == 0) {
    _DAT_20000a0c = *param_1;
  }
  else {
    FUN_08006848(param_1,2);
  }
  return 0;
}


/* ==== FUN_08009F88 @ 0x8009f88 ==== */

undefined4 FUN_08009F88(void)

{
  return 0;
}


/* ==== FUN_08009F8C @ 0x8009f8c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009F8C(void)

{
  bool bVar1;
  
  bVar1 = (bool)isCurrentModePrivileged();
  if (bVar1) {
    setMainStackPointer(0x200014a8);
  }
  (*_DAT_1fffc804)();
  return;
}


/* ==== FUN_08009FA4 @ 0x8009fa4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_08009FA4(void)

{
  int iVar1;
  undefined4 local_18;
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  undefined1 local_11;
  
  if ((_DAT_200025a8 == -0x281a0858) && (_DAT_200017a8 == 0x19c3fe40)) {
    _DAT_200017a8 = 0;
    _DAT_200025a8 = 0;
    FUN_0800ab88();
    _DAT_e000e010 = 0;
    _DAT_e000e014 = 0;
    _DAT_e000e018 = 0;
    FUN_0800AD5C(0x80000,1);
    local_14 = 1;
    local_12 = 0;
    local_11 = 0;
    local_13 = 0;
    local_18 = 0x5000;
    FUN_0800A964(0x48000800,&local_18);
    _DAT_48000818 = 0x4000;
    FUN_08009F8C();
  }
  FUN_0800A640();
  FUN_0800A464();
  FUN_0800A4C0();
  FUN_08006338();
  FUN_0800a150(0x20000a44,0x10);
  FUN_0800A1C8(0x20000a14,0);
  do {
    iVar1 = FUN_08009B14(&DAT_20000fe8,0x28);
    if (iVar1 != 0) {
      if (DAT_20000fe8 == '\0') {
        FUN_0800A2D0(4,"possible cmdBuf overflow",0);
      }
      FUN_0800A350(&DAT_20000fe8);
    }
    FUN_080063EC();
    FUN_0800ab10();
  } while( true );
}


/* ==== FUN_0800A084 @ 0x800a084 ==== */

undefined4 FUN_0800A084(undefined4 param_1)

{
  int iVar1;
  undefined *puVar2;
  
  puVar2 = &DAT_0800c6ec;
  do {
    iVar1 = FUN_0800b740(param_1,puVar2);
    if (iVar1 == 0) {
      return 1;
    }
    puVar2 = puVar2 + 5;
  } while (puVar2 < &UNK_0800c6fb);
  return 0;
}


/* ==== FUN_0800A0AC @ 0x800a0ac ==== */

undefined4
FUN_0800A0AC(undefined4 param_1,undefined1 *param_2,undefined4 *param_3,int *param_4,int *param_5)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined1 uVar4;
  
  *param_2 = 0;
  uVar1 = FUN_0800B764(param_1,&DAT_0800c6a4,param_3,param_4,param_4);
  iVar2 = FUN_0800b740(uVar1,&DAT_0800b82c);
  if (iVar2 == 0) {
    uVar4 = 3;
  }
  else {
    iVar2 = FUN_0800b740(uVar1,&DAT_0800b828);
    if (iVar2 == 0) {
      uVar4 = 2;
    }
    else {
      iVar2 = FUN_0800b740(uVar1,&DAT_0800b824);
      if (iVar2 != 0) {
        return 0;
      }
      uVar4 = 1;
    }
  }
  *param_2 = uVar4;
  uVar1 = FUN_0800B764(0,&DAT_0800c6a4);
  *param_3 = uVar1;
  iVar2 = FUN_0800B764(0,&DAT_0800c6a4);
  *param_4 = iVar2;
  if (param_3 == (undefined4 *)0x0) {
    uVar1 = 0;
  }
  else if (param_4 == (int *)0x0) {
    uVar1 = 0;
  }
  else {
    iVar3 = FUN_0800b754(iVar2);
    *param_5 = iVar2 + iVar3 + 1;
    uVar1 = 1;
  }
  return uVar1;
}


/* ==== FUN_0800A140 @ 0x800a140 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800A140(void)

{
  (*_DAT_20000a40)(10);
  return;
}


/* ==== FUN_0800a150 @ 0x800a150 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800a150(undefined4 param_1,undefined4 param_2)

{
  _DAT_20001010 = param_1;
  _DAT_20001014 = param_2;
  return 1;
}


/* ==== FUN_0800a160 @ 0x800a160 ==== */

undefined * FUN_0800a160(int param_1)

{
  return &UNK_0800b820 + param_1 * 4;
}


/* ==== FUN_0800A16C @ 0x800a16c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800A16C(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  code *pcVar1;
  
  pcVar1 = _DAT_20000a10;
  FUN_0800a160();
  (*pcVar1)();
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)(param_2);
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)(param_3);
  if (param_4 != 0) {
    (*_DAT_20000a40)(0x20);
    (*_DAT_20000a10)(param_4);
  }
  (*_DAT_20000a40)(10);
  return 1;
}


/* ==== FUN_0800A1C8 @ 0x800a1c8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800A1C8(int *param_1,int param_2)

{
  code *pcVar1;
  uint uVar2;
  int *piVar3;
  
  pcVar1 = _DAT_20000a10;
  FUN_0800a160(3);
  (*pcVar1)();
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)(":proxy");
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)("greeting");
  if (param_1 != (int *)0x0) {
    (*_DAT_20000a40)(0x20);
    uVar2 = 0;
    piVar3 = param_1;
    if (*param_1 != 0) {
      while( true ) {
        (*_DAT_20000a40)(0x22);
        (*_DAT_20000a10)(*piVar3);
        (*_DAT_20000a40)(0x3a);
        if (param_1[uVar2 + 1] == 0) break;
        uVar2 = uVar2 + 2 & 0xff;
        (*_DAT_20000a10)();
        (*_DAT_20000a40)(0x22);
        piVar3 = param_1 + uVar2;
        if (*piVar3 == 0) goto LAB_0800a270;
        if (uVar2 != 0) {
          (*_DAT_20000a40)(0x20);
        }
      }
      (*_DAT_20000a40)(0x22);
    }
  }
LAB_0800a270:
  if (param_2 != 0) {
    (*_DAT_20000a40)(0x20);
    (*_DAT_20000a40)(0x22);
    (*_DAT_20000a10)(&DAT_0800c6b8);
    (*_DAT_20000a10)(param_2);
    (*_DAT_20000a40)(0x22);
  }
  (*_DAT_20000a40)(10);
  return;
}


/* ==== FUN_0800a2c0 @ 0x800a2c0 ==== */

undefined * FUN_0800a2c0(int param_1)

{
  return &UNK_0800b834 + param_1 * 4;
}


/* ==== FUN_0800A2D0 @ 0x800a2d0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800A2D0(undefined4 param_1,undefined4 param_2,int param_3)

{
  code *pcVar1;
  
  pcVar1 = _DAT_20000a10;
  FUN_0800a160(3);
  (*pcVar1)();
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)(":proxy");
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)("message");
  (*_DAT_20000a40)(0x20);
  pcVar1 = _DAT_20000a10;
  FUN_0800a2c0(param_1);
  (*pcVar1)();
  (*_DAT_20000a40)(0x20);
  (*_DAT_20000a10)(param_2);
  if (param_3 != 0) {
    (*_DAT_20000a10)(param_3);
  }
  (*_DAT_20000a40)(10);
  return;
}


/* ==== FUN_0800A350 @ 0x800a350 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800A350(undefined4 param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined4 *puVar7;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined1 local_19;
  
  if (_DAT_20001014 == 0) {
    return 0;
  }
  local_20 = 0;
  local_24 = 0;
  local_28 = 0;
  iVar1 = FUN_0800A0AC(param_1,&local_19,&local_20,&local_24,&local_28);
  uVar2 = local_20;
  iVar3 = _DAT_20001014;
  if (iVar1 == 0) {
    FUN_0800A2D0(4,"Invalid command!",0);
    return 0;
  }
  puVar5 = (undefined4 *)(_DAT_20001010 + (_DAT_20001014 + 0x1fffffff) * 8);
  uVar4 = _DAT_20001014 - 1;
  iVar1 = FUN_0800b740(local_20,*puVar5);
  puVar7 = puVar5;
  if (iVar1 == 0) {
LAB_0800a3c4:
    uVar2 = (*(code *)puVar7[1])(local_19,local_24,local_28);
  }
  else {
    puVar6 = puVar5 + -2;
    if (uVar4 != 0) {
      if ((uVar4 & 1) != 0) {
        uVar4 = iVar3 - 2;
        iVar3 = FUN_0800b740(uVar2,*puVar6);
        puVar7 = puVar6;
        if (iVar3 == 0) goto LAB_0800a3c4;
        puVar6 = puVar5 + -4;
        if (uVar4 == 0) goto LAB_0800a3e8;
      }
      do {
        iVar3 = FUN_0800b740(uVar2,*puVar6);
        puVar7 = puVar6;
        if (iVar3 == 0) goto LAB_0800a3c4;
        uVar4 = uVar4 - 2;
        iVar3 = FUN_0800b740(uVar2,puVar6[-2]);
        puVar7 = puVar6 + -2;
        if (iVar3 == 0) goto LAB_0800a3c4;
        puVar6 = puVar6 + -4;
      } while (uVar4 != 0);
    }
LAB_0800a3e8:
    FUN_0800A2D0(4,"No interface:",uVar2);
    uVar2 = 0;
  }
  return uVar2;
}


/* ==== FUN_0800A43C @ 0x800a43c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800A43C(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  iVar1 = FUN_0800B3A8(&DAT_40001000,1,param_3,param_4,param_4);
  if (iVar1 == 1) {
    _DAT_20001018 = _DAT_20001018 + 1;
    FUN_0800b3c0(&DAT_40001000,1);
  }
  return;
}


/* ==== FUN_0800A464 @ 0x800a464 ==== */

void FUN_0800A464(void)

{
  undefined1 local_18;
  undefined1 local_17;
  undefined1 local_16;
  undefined2 local_14;
  undefined2 local_12;
  undefined4 local_10;
  undefined2 local_c;
  
  FUN_0800AD9C(0x10,1);
  FUN_0800AF74(&DAT_40001000,0);
  local_12 = 0;
  local_10 = 0xffff;
  local_14 = 0x30;
  local_c = 0;
  FUN_0800AEC4(&DAT_40001000);
  local_18 = 0x11;
  local_16 = 1;
  local_17 = 2;
  FUN_0800AB30(&local_18);
  FUN_0800AF58(&DAT_40001000,1);
  FUN_0800b3c0(&DAT_40001000,1);
  return;
}


/* ==== FUN_0800A4C0 @ 0x800a4c0 ==== */

void FUN_0800A4C0(void)

{
  FUN_0800B38C(&DAT_40001000,1,1);
  FUN_0800AFA0(&DAT_40001000,1);
  return;
}


/* ==== FUN_0800A4DC @ 0x800a4dc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800A4DC(int *param_1)

{
  uint uVar1;
  bool bVar2;
  
  uVar1 = _DAT_20001018 >> 0x10;
  bVar2 = CARRY4(_DAT_40001024,_DAT_20001018 * 0x10000);
  *param_1 = _DAT_40001024 + _DAT_20001018 * 0x10000;
  param_1[1] = uVar1 + bVar2;
  return;
}


/* ==== FUN_0800A500 @ 0x800a500 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800A500(uint *param_1,int *param_2)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  
  uVar4 = _DAT_40001024 + _DAT_20001018 * 0x10000;
  uVar5 = (_DAT_20001018 >> 0x10) + (uint)CARRY4(_DAT_40001024,_DAT_20001018 * 0x10000);
  if (param_1 == (uint *)0x0) {
    param_1 = (uint *)&DAT_20001020;
  }
  if ((uVar5 < param_1[1]) || ((param_1[1] == uVar5 && (uVar4 < *param_1)))) {
    *param_2 = 0;
    param_2[1] = 0;
    uVar1 = 0;
  }
  else {
    uVar3 = param_1[1];
    uVar2 = *param_1;
    *param_2 = uVar4 - uVar2;
    param_2[1] = (uVar5 - uVar3) - (uint)(uVar4 < uVar2);
    uVar1 = 1;
  }
  return uVar1;
}


/* ==== FUN_0800A558 @ 0x800a558 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800A558(uint *param_1,undefined8 *param_2)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  undefined8 uVar4;
  
  uVar2 = _DAT_40001024 + _DAT_20001018 * 0x10000;
  uVar3 = (_DAT_20001018 >> 0x10) + (uint)CARRY4(_DAT_40001024,_DAT_20001018 * 0x10000);
  if (param_1 == (uint *)0x0) {
    param_1 = (uint *)&DAT_20001020;
  }
  if ((uVar3 < param_1[1]) || ((param_1[1] == uVar3 && (uVar2 < *param_1)))) {
    *(undefined4 *)param_2 = 0;
    *(undefined4 *)((int)param_2 + 4) = 0;
    uVar1 = 0;
  }
  else {
    uVar4 = FUN_080002fc(uVar2 - *param_1,(uVar3 - param_1[1]) - (uint)(uVar2 < *param_1),1000,0);
    *param_2 = uVar4;
    uVar1 = 1;
  }
  return uVar1;
}


/* ==== FUN_0800A5C8 @ 0x800a5c8 ==== */

bool FUN_0800A5C8(uint *param_1,uint *param_2,int *param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  
  if (param_2 <= param_1) {
    uVar2 = param_1[1];
    uVar1 = *param_1;
    uVar3 = *param_2;
    uVar4 = param_2[1];
    *param_3 = uVar1 - uVar3;
    param_3[1] = (uVar2 - uVar4) - (uint)(uVar1 < uVar3);
  }
  else {
    *param_3 = 0;
    param_3[1] = 0;
  }
  return param_2 <= param_1;
}


/* ==== FUN_0800A5F0 @ 0x800a5f0 ==== */

void FUN_0800A5F0(void)

{
  undefined4 local_10;
  undefined1 local_c;
  undefined1 local_b;
  undefined1 local_9;
  
  FUN_0800AD5C(0xe0000,1);
  local_c = 0;
  local_9 = 0;
  local_b = 0;
  local_10 = 0xffff;
  FUN_0800A964(0x48000400,&local_10);
  FUN_0800A964(0x48000800,&local_10);
  local_10 = 0x9fff;
  FUN_0800A964(0x48000000,&local_10);
  return;
}


/* ==== FUN_0800A640 @ 0x800a640 ==== */

void FUN_0800A640(void)

{
  int iVar1;
  
  FUN_0800A5F0();
  FUN_0800ABE8(1);
  do {
    iVar1 = FUN_0800ADDC(0x41);
  } while (iVar1 == 0);
  FUN_0800aaec(0x5555);
  FUN_0800aaf8(2);
  FUN_0800ab04(0xffe);
  FUN_0800ab10();
  FUN_0800ab20();
  return;
}


/* ==== FUN_0800A67C @ 0x800a67c ==== */

void FUN_0800A67C(int param_1)

{
  if (param_1 == 0x40012400) {
    FUN_0800ADBC(0x200,1);
    FUN_0800ADBC(0x200,0);
  }
  return;
}


/* ==== FUN_0800A6A0 @ 0x800a6a0 ==== */

void FUN_0800A6A0(int param_1,uint *param_2)

{
  *(uint *)(param_1 + 0xc) =
       *(uint *)(param_1 + 0xc) & 0xffffd203 |
       param_2[2] | *param_2 | param_2[3] | param_2[4] | param_2[5] | (uint)(byte)param_2[1] << 0xd;
  return;
}


/* ==== FUN_0800a6cc @ 0x800a6cc ==== */

void FUN_0800a6cc(undefined4 *param_1)

{
  *param_1 = 0;
  *(undefined1 *)(param_1 + 1) = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  return;
}


/* ==== FUN_0800A6DC @ 0x800a6dc ==== */

void FUN_0800A6DC(int param_1,int param_2)

{
  if (param_2 == 0) {
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 2;
  }
  else {
    *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 1;
  }
  return;
}


/* ==== FUN_0800a6f8 @ 0x800a6f8 ==== */

void FUN_0800a6f8(int param_1,uint param_2,undefined4 param_3)

{
  *(uint *)(param_1 + 0x28) = param_2 | *(uint *)(param_1 + 0x28);
  *(undefined4 *)(param_1 + 0x14) = param_3;
  return;
}


/* ==== FUN_0800A704 @ 0x800a704 ==== */

undefined4 FUN_0800A704(int param_1)

{
  undefined4 uVar1;
  uint uVar2;
  int iVar3;
  
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 0x80000000;
  if (*(int *)(param_1 + 8) < 0) {
    uVar2 = *(uint *)(param_1 + 8);
    iVar3 = 0xeffe;
    do {
      if (((uVar2 & 0x80000000) == 0) || (-1 < *(int *)(param_1 + 8))) break;
      uVar2 = *(uint *)(param_1 + 8);
      iVar3 = iVar3 + -2;
    } while (iVar3 != 0);
  }
  if (*(int *)(param_1 + 8) < 0) {
    uVar1 = 0;
  }
  else {
    uVar1 = *(undefined4 *)(param_1 + 0x40);
  }
  return uVar1;
}


/* ==== FUN_0800a754 @ 0x800a754 ==== */

void FUN_0800a754(int param_1)

{
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 4;
  return;
}


/* ==== FUN_0800A760 @ 0x800a760 ==== */

void FUN_0800A760(int param_1,int param_2)

{
  if (param_2 == 0) {
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xfffffffe;
  }
  else {
    *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) | 1;
  }
  return;
}


/* ==== FUN_0800a77c @ 0x800a77c ==== */

void FUN_0800a77c(int param_1,uint param_2)

{
  *(uint *)(param_1 + 0xc) = *(uint *)(param_1 + 0xc) & 0xfffffffd;
  *(uint *)(param_1 + 0xc) = param_2 | *(uint *)(param_1 + 0xc);
  return;
}


/* ==== FUN_0800A78C @ 0x800a78c ==== */

bool FUN_0800A78C(uint *param_1,uint param_2)

{
  uint uVar1;
  
  if ((int)(param_2 << 7) < 0) {
    uVar1 = param_1[2] & 0xfeffffff;
  }
  else {
    uVar1 = *param_1;
  }
  return (param_2 & uVar1) != 0;
}


/* ==== FUN_0800a7ac @ 0x800a7ac ==== */

void FUN_0800a7ac(undefined4 *param_1,undefined4 param_2)

{
  *param_1 = param_2;
  return;
}


/* ==== FUN_0800A7B0 @ 0x800a7b0 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800A7B0(uint *param_1)

{
  *param_1 = *param_1 & 0xfffe;
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  if (param_1 == (uint *)0x40020008) {
    _DAT_40020004 = _DAT_40020004 | 0xf;
  }
  else if (param_1 == (uint *)0x4002001c) {
    _DAT_40020004 = _DAT_40020004 | 0xf0;
  }
  else if (param_1 == (uint *)0x40020030) {
    _DAT_40020004 = _DAT_40020004 | 0xf00;
  }
  else if (param_1 == (uint *)0x40020044) {
    _DAT_40020004 = _DAT_40020004 | 0xf000;
  }
  else if (param_1 == (uint *)0x40020058) {
    _DAT_40020004 = _DAT_40020004 | 0xf0000;
  }
  return;
}


/* ==== FUN_0800A840 @ 0x800a840 ==== */

void FUN_0800A840(uint *param_1,uint *param_2)

{
  *param_1 = param_2[8] | param_2[2] | param_2[4] | param_2[5] | param_2[6] | param_2[7] |
             param_2[9] | param_2[10] | *param_1 & 0xffff800f;
  param_1[1] = param_2[3];
  param_1[2] = *param_2;
  param_1[3] = param_2[1];
  return;
}


/* ==== FUN_0800A87C @ 0x800a87c ==== */

void FUN_0800A87C(uint *param_1,int param_2)

{
  if (param_2 == 0) {
    *param_1 = *param_1 & 0xfffe;
  }
  else {
    *param_1 = *param_1 | 1;
  }
  return;
}


/* ==== FUN_0800a89c @ 0x800a89c ==== */

void FUN_0800a89c(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 4) = param_2;
  return;
}


/* ==== FUN_0800A8A0 @ 0x800a8a0 ==== */

void FUN_0800A8A0(uint *param_1,uint param_2,int param_3)

{
  if (param_3 == 0) {
    *param_1 = *param_1 & ~param_2;
  }
  else {
    *param_1 = param_2 | *param_1;
  }
  return;
}


/* ==== FUN_0800a8b8 @ 0x800a8b8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_0800a8b8(uint param_1)

{
  return (param_1 & _DAT_40020000) != 0;
}


/* ==== FUN_0800a8c8 @ 0x800a8c8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800a8c8(undefined4 param_1)

{
  _DAT_40020004 = param_1;
  return;
}


/* ==== FUN_0800A8D4 @ 0x800a8d4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800A8D4(uint *param_1)

{
  if (*(char *)((int)param_1 + 6) == '\0') {
    *(uint *)(&DAT_40010400 + (byte)param_1[1]) =
         *(uint *)(&DAT_40010400 + (byte)param_1[1]) & ~*param_1;
  }
  else {
    _DAT_40010400 = _DAT_40010400 & ~*param_1;
    _DAT_40010404 = _DAT_40010404 & ~*param_1;
    *(uint *)(&DAT_40010400 + (byte)param_1[1]) =
         *(uint *)(&DAT_40010400 + (byte)param_1[1]) | *param_1;
    _DAT_40010408 = _DAT_40010408 & ~*param_1;
    _DAT_4001040c = _DAT_4001040c & ~*param_1;
    if (*(byte *)((int)param_1 + 5) == 0x10) {
      _DAT_40010408 = _DAT_40010408 | *param_1;
      _DAT_4001040c = *param_1 | _DAT_4001040c;
    }
    else {
      *(uint *)(&DAT_40010400 + *(byte *)((int)param_1 + 5)) =
           *param_1 | *(uint *)(&DAT_40010400 + *(byte *)((int)param_1 + 5));
    }
  }
  return;
}


/* ==== FUN_0800a948 @ 0x800a948 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

bool FUN_0800a948(uint param_1)

{
  return (param_1 & _DAT_40010414) != 0;
}


/* ==== FUN_0800a958 @ 0x800a958 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800a958(undefined4 param_1)

{
  _DAT_40010414 = param_1;
  return;
}


/* ==== FUN_0800A964 @ 0x800a964 ==== */

void FUN_0800A964(uint *param_1,uint *param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  
  uVar1 = 0;
  uVar2 = 0;
  do {
    uVar5 = 1 << (uVar2 & 0xff);
    if ((*param_2 & uVar5) == uVar5) {
      if ((byte)param_2[1] - 1 < 2) {
        param_1[2] = param_1[2] & ~(3 << (uVar1 & 0xff));
        param_1[2] = (uint)*(byte *)((int)param_2 + 5) << (uVar1 & 0xff) | param_1[2];
        *(ushort *)(param_1 + 1) = (ushort)param_1[1] & ~(ushort)(1 << (uVar2 & 0xff));
        *(ushort *)(param_1 + 1) =
             (ushort)*(byte *)((int)param_2 + 6) << (uVar2 & 0xff) | (ushort)param_1[1];
      }
      *param_1 = *param_1 & ~(3 << (uVar1 & 0xff));
      *param_1 = (uint)(byte)param_2[1] << (uVar1 & 0xff) | *param_1;
      param_1[3] = param_1[3] & ~(3 << ((uVar2 & 0x7f) << 1));
      param_1[3] = (uint)*(byte *)((int)param_2 + 7) << (uVar1 & 0xff) | param_1[3];
    }
    uVar3 = uVar2 + 1;
    uVar5 = uVar1 + 2;
    uVar4 = 1 << (uVar3 & 0xff);
    if ((*param_2 & uVar4) == uVar4) {
      if ((byte)param_2[1] - 1 < 2) {
        param_1[2] = param_1[2] & ~(3 << (uVar5 & 0xff));
        param_1[2] = (uint)*(byte *)((int)param_2 + 5) << (uVar5 & 0xff) | param_1[2];
        *(ushort *)(param_1 + 1) = (ushort)param_1[1] & ~(ushort)(1 << (uVar3 & 0xff));
        *(ushort *)(param_1 + 1) =
             (ushort)*(byte *)((int)param_2 + 6) << (uVar3 & 0xff) | (ushort)param_1[1];
      }
      *param_1 = *param_1 & ~(3 << (uVar5 & 0xff));
      *param_1 = (uint)(byte)param_2[1] << (uVar5 & 0xff) | *param_1;
      param_1[3] = param_1[3] & ~(3 << ((uVar3 & 0x7f) << 1));
      param_1[3] = (uint)*(byte *)((int)param_2 + 7) << (uVar5 & 0xff) | param_1[3];
    }
    uVar2 = uVar2 + 2;
    uVar1 = uVar1 + 4;
  } while (uVar2 != 0x10);
  return;
}


/* ==== FUN_0800aaa4 @ 0x800aaa4 ==== */

void FUN_0800aaa4(undefined4 *param_1)

{
  *param_1 = 0xffff;
  *(undefined1 *)(param_1 + 1) = 0;
  *(undefined1 *)((int)param_1 + 5) = 1;
  *(undefined1 *)((int)param_1 + 6) = 0;
  *(undefined1 *)((int)param_1 + 7) = 0;
  return;
}


/* ==== FUN_0800aabc @ 0x800aabc ==== */

bool FUN_0800aabc(int param_1,ushort param_2)

{
  return (*(ushort *)(param_1 + 0x10) & param_2) != 0;
}


/* ==== FUN_0800AAC8 @ 0x800aac8 ==== */

void FUN_0800AAC8(int param_1,uint param_2,int param_3)

{
  int iVar1;
  int iVar2;
  
  iVar2 = (param_2 & 7) << 2;
  iVar1 = ((param_2 >> 3) + 8) * 4;
  *(uint *)(iVar1 + param_1) = *(uint *)(iVar1 + param_1) & ~(0xf << iVar2);
  *(uint *)(iVar1 + param_1) = param_3 << iVar2 | *(uint *)(iVar1 + param_1);
  return;
}


/* ==== FUN_0800aaec @ 0x800aaec ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800aaec(undefined4 param_1)

{
  _DAT_40003000 = param_1;
  return;
}


/* ==== FUN_0800aaf8 @ 0x800aaf8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800aaf8(undefined4 param_1)

{
  _DAT_40003004 = param_1;
  return;
}


/* ==== FUN_0800ab04 @ 0x800ab04 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800ab04(undefined4 param_1)

{
  _DAT_40003008 = param_1;
  return;
}


/* ==== FUN_0800ab10 @ 0x800ab10 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800ab10(void)

{
  _DAT_40003000 = 0xaaaa;
  return;
}


/* ==== FUN_0800ab20 @ 0x800ab20 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800ab20(void)

{
  _DAT_40003000 = 0xcccc;
  return;
}


/* ==== FUN_0800AB30 @ 0x800ab30 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800AB30(byte *param_1)

{
  int iVar1;
  int iVar2;
  
  if (param_1[2] == 0) {
    _DAT_e000e180 = 1 << (*param_1 & 0x1f);
  }
  else {
    iVar2 = ((*param_1 >> 2) + 0xc0) * 4;
    iVar1 = (*param_1 & 3) << 3;
    *(uint *)(&DAT_e000e100 + iVar2) =
         *(uint *)(&DAT_e000e100 + iVar2) & ~(0xff << iVar1) | ((param_1[1] & 3) << 6) << iVar1;
    _DAT_e000e100 = 1 << (*param_1 & 0x1f);
  }
  return;
}


/* ==== FUN_0800ab88 @ 0x800ab88 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800ab88(void)

{
  _DAT_40021000 = _DAT_40021000 & 0xfef2ffff | 1;
  _DAT_40021004 = _DAT_40021004 & 0x8c0b80c;
  _DAT_4002102c = _DAT_4002102c & 0xfffffff0;
  _DAT_40021030 = _DAT_40021030 & 0xfffffeac;
  _DAT_40021034 = _DAT_40021034 & 0xfffffffe;
  _DAT_40021008 = 0;
  return;
}


/* ==== FUN_0800ABE8 @ 0x800abe8 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800ABE8(int param_1)

{
  if (param_1 == 0) {
    _DAT_40021024 = _DAT_40021024 & 0xfffffffe;
  }
  else {
    _DAT_40021024 = _DAT_40021024 | 1;
  }
  return;
}


/* ==== FUN_0800ac0c @ 0x800ac0c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800ac0c(uint param_1)

{
  _DAT_40021004 = param_1 & 0xffff | _DAT_40021004 & 0xffffbfff;
  _DAT_40021030 = param_1 >> 0x10 | _DAT_40021030 & 0xfffffeff;
  return;
}


/* ==== FUN_0800AC40 @ 0x800ac40 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800AC40(uint *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  
  uVar1 = _DAT_40021004 & 0xc;
  if (uVar1 == 4) {
    *param_1 = 16000000;
  }
  else if (uVar1 == 8) {
    iVar3 = ((_DAT_40021004 & 0x3fffff) >> 0x12) + 2;
    if ((_DAT_40021004 & 0x18000) == 0) {
      *param_1 = iVar3 * 4000000;
    }
    else {
      iVar2 = FUN_08000128(16000000,(_DAT_4002102c & 0xf) + 1,&DAT_40021000,_DAT_4002102c,param_4);
      *param_1 = iVar2 * iVar3;
    }
  }
  else if (uVar1 == 0) {
    *param_1 = 8000000;
  }
  else {
    *param_1 = 8000000;
  }
  uVar1 = *param_1;
  uVar4 = uVar1 >> *(sbyte *)(((_DAT_40021004 & 0xff) >> 4) + 0x20000ac4);
  param_1[1] = uVar4;
  uVar4 = uVar4 >> *(sbyte *)(((_DAT_40021004 & 0x7ff) >> 8) + 0x20000ac4);
  param_1[2] = uVar4;
  if ((int)(_DAT_40021030 << 0x17) < 0) {
    if ((int)(_DAT_40021004 << 0x11) < 0) {
      param_1[3] = uVar4 >> 2;
    }
    else {
      param_1[3] = uVar4 >> 1;
    }
  }
  else {
    param_1[3] = 14000000;
  }
  if ((int)(_DAT_40021030 << 0x19) < 0) {
    param_1[4] = 0x8000;
  }
  else {
    param_1[4] = 0x8012;
  }
  if ((int)(_DAT_40021030 << 0x1b) < 0) {
    param_1[5] = uVar1;
  }
  else {
    param_1[5] = 8000000;
  }
  if ((_DAT_40021030 & 3) == 0) {
    param_1[6] = uVar4;
  }
  else if ((_DAT_40021030 & 3) == 1) {
    param_1[6] = uVar1;
  }
  else if ((_DAT_40021030 & 3) == 2) {
    param_1[6] = 0x8000;
  }
  else if ((_DAT_40021030 & 3) == 3) {
    param_1[6] = 8000000;
  }
  return;
}


/* ==== FUN_0800AD5C @ 0x800ad5c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800AD5C(uint param_1,int param_2)

{
  if (param_2 == 0) {
    _DAT_40021014 = _DAT_40021014 & ~param_1;
  }
  else {
    _DAT_40021014 = param_1 | _DAT_40021014;
  }
  return;
}


/* ==== FUN_0800AD7C @ 0x800ad7c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800AD7C(uint param_1,int param_2)

{
  if (param_2 == 0) {
    _DAT_40021018 = _DAT_40021018 & ~param_1;
  }
  else {
    _DAT_40021018 = param_1 | _DAT_40021018;
  }
  return;
}


/* ==== FUN_0800AD9C @ 0x800ad9c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800AD9C(uint param_1,int param_2)

{
  if (param_2 == 0) {
    _DAT_4002101c = _DAT_4002101c & ~param_1;
  }
  else {
    _DAT_4002101c = param_1 | _DAT_4002101c;
  }
  return;
}


/* ==== FUN_0800ADBC @ 0x800adbc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800ADBC(uint param_1,int param_2)

{
  if (param_2 == 0) {
    _DAT_4002100c = _DAT_4002100c & ~param_1;
  }
  else {
    _DAT_4002100c = param_1 | _DAT_4002100c;
  }
  return;
}


/* ==== FUN_0800ADDC @ 0x800addc ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

uint FUN_0800ADDC(uint param_1)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = param_1 >> 5;
  uVar1 = _DAT_40021000;
  if (((uVar2 != 0) && (uVar1 = _DAT_40021020, uVar2 != 1)) && (uVar1 = _DAT_40021034, uVar2 == 2))
  {
    uVar1 = _DAT_40021024;
  }
  return uVar1 >> (param_1 & 0x1f) & 1;
}


/* ==== FUN_0800AE14 @ 0x800ae14 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800AE14(uint param_1,int param_2)

{
  if (param_2 == 0) {
    _DAT_40010000 = _DAT_40010000 & ~param_1;
  }
  else {
    _DAT_40010000 = param_1 | _DAT_40010000;
  }
  return;
}


/* ==== FUN_0800AE34 @ 0x800ae34 ==== */

void FUN_0800AE34(int param_1,uint param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = (param_2 & 3) << 2;
  iVar1 = ((param_2 >> 2) + 2) * 4;
  *(uint *)(&DAT_40010000 + iVar1) = *(uint *)(&DAT_40010000 + iVar1) & ~(0xf << iVar2);
  *(uint *)(&DAT_40010000 + iVar1) = param_1 << iVar2 | *(uint *)(&DAT_40010000 + iVar1);
  return;
}


/* ==== FUN_0800AE5C @ 0x800ae5c ==== */

void FUN_0800AE5C(int param_1,ushort param_2,ushort param_3,short param_4)

{
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xfffe;
  *(ushort *)(param_1 + 0x18) = param_3 | *(ushort *)(param_1 + 0x18) & 0xff0c | param_4 << 4;
  *(ushort *)(param_1 + 0x20) = param_2 | *(ushort *)(param_1 + 0x20) & 0xfff5 | 1;
  return;
}


/* ==== FUN_0800AE88 @ 0x800ae88 ==== */

void FUN_0800AE88(int param_1,short param_2,short param_3,short param_4)

{
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xffef;
  *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & 0xcff | param_3 << 8 | param_4 << 0xc;
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xff5f | 0x10 | param_2 << 4;
  return;
}


/* ==== FUN_0800AEC4 @ 0x800aec4 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800AEC4(ushort *param_1,ushort *param_2)

{
  ushort uVar1;
  
  uVar1 = *param_1;
  if (((param_1 == (ushort *)0x40012c00) || (param_1 == (ushort *)0x40000000)) ||
     (param_1 == (ushort *)0x40000400)) {
    uVar1 = uVar1 & 0xff8f | param_2[1];
  }
  if (param_1 == (ushort *)&DAT_40001000) {
    _DAT_4000102c = *(undefined4 *)(param_2 + 2);
    _DAT_40001028 = *param_2;
    _DAT_40001000 = uVar1;
  }
  else {
    *param_1 = uVar1 & 0xfcff | param_2[4];
    *(undefined4 *)(param_1 + 0x16) = *(undefined4 *)(param_2 + 2);
    param_1[0x14] = *param_2;
    if (((param_1 == (ushort *)0x40012c00) || (param_1 == (ushort *)0x40014000)) ||
       ((param_1 == (ushort *)0x40014400 || (param_1 == (ushort *)0x40014800)))) {
      param_1[0x18] = (ushort)(byte)param_2[5];
    }
  }
  param_1[10] = 1;
  return;
}


/* ==== FUN_0800af4c @ 0x800af4c ==== */

void FUN_0800af4c(int param_1,undefined2 param_2,undefined2 param_3)

{
  *(undefined2 *)(param_1 + 0x28) = param_2;
  *(undefined2 *)(param_1 + 0x14) = param_3;
  return;
}


/* ==== FUN_0800af54 @ 0x800af54 ==== */

void FUN_0800af54(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x2c) = param_2;
  return;
}


/* ==== FUN_0800AF58 @ 0x800af58 ==== */

void FUN_0800AF58(ushort *param_1,int param_2)

{
  if (param_2 == 0) {
    *param_1 = *param_1 & 0xfffb;
  }
  else {
    *param_1 = *param_1 | 4;
  }
  return;
}


/* ==== FUN_0800AF74 @ 0x800af74 ==== */

void FUN_0800AF74(ushort *param_1,int param_2)

{
  if (param_2 == 0) {
    *param_1 = *param_1 & 0xff7f;
  }
  else {
    *param_1 = *param_1 | 0x80;
  }
  return;
}


/* ==== FUN_0800af90 @ 0x800af90 ==== */

void FUN_0800af90(ushort *param_1,ushort param_2)

{
  *param_1 = *param_1 & 0xfff7;
  *param_1 = param_2 | *param_1;
  return;
}


/* ==== FUN_0800AFA0 @ 0x800afa0 ==== */

void FUN_0800AFA0(ushort *param_1,int param_2)

{
  if (param_2 == 0) {
    *param_1 = *param_1 & 0xfffe;
  }
  else {
    *param_1 = *param_1 | 1;
  }
  return;
}


/* ==== FUN_0800AFBC @ 0x800afbc ==== */

void FUN_0800AFBC(int param_1,int param_2)

{
  if (param_2 == 0) {
    *(ushort *)(param_1 + 0x44) = *(ushort *)(param_1 + 0x44) & 0x7fff;
  }
  else {
    *(ushort *)(param_1 + 0x44) = *(ushort *)(param_1 + 0x44) | 0x8000;
  }
  return;
}


/* ==== FUN_0800AFDC @ 0x800afdc ==== */

void FUN_0800AFDC(int param_1,ushort *param_2)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xfffe;
  uVar2 = *(ushort *)(param_1 + 4);
  uVar1 = *param_2;
  uVar3 = param_2[6] | param_2[1] | *(ushort *)(param_1 + 0x20) & 0xfffd;
  if ((((param_1 == 0x40012c00) || (param_1 == 0x40014000)) || (param_1 == 0x40014400)) ||
     (param_1 == 0x40014800)) {
    uVar3 = (uVar3 & 0xfff7 | param_2[7]) & 0xfffb | param_2[2];
    uVar2 = uVar2 & 0xfcff | param_2[8] | param_2[9];
  }
  *(ushort *)(param_1 + 4) = uVar2;
  *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & 0xff8c | uVar1;
  *(undefined4 *)(param_1 + 0x34) = *(undefined4 *)(param_2 + 4);
  *(ushort *)(param_1 + 0x20) = uVar3;
  return;
}


/* ==== FUN_0800B054 @ 0x800b054 ==== */

void FUN_0800B054(int param_1,short *param_2)

{
  short sVar1;
  ushort uVar2;
  ushort uVar3;
  
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xffef;
  uVar3 = *(ushort *)(param_1 + 4);
  sVar1 = *param_2;
  uVar2 = *(ushort *)(param_1 + 0x20) & 0xffdf | param_2[6] << 4 | param_2[1] << 4;
  if (param_1 == 0x40012c00) {
    uVar2 = (uVar2 & 0xff7f | param_2[7] << 4) & 0xffbf | param_2[2] << 4;
    uVar3 = (uVar3 & 0xfbff | param_2[8] << 2) & 0xf7ff | param_2[9] << 2;
  }
  else if (param_1 == 0x40014000) {
    uVar3 = uVar3 & 0xfbff | param_2[8] << 2;
  }
  *(ushort *)(param_1 + 4) = uVar3;
  *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & 0x8cff | sVar1 << 8;
  *(undefined4 *)(param_1 + 0x38) = *(undefined4 *)(param_2 + 4);
  *(ushort *)(param_1 + 0x20) = uVar2;
  return;
}


/* ==== FUN_0800B0F4 @ 0x800b0f4 ==== */

void FUN_0800B0F4(int param_1,ushort *param_2)

{
  ushort uVar1;
  ushort uVar2;
  ushort uVar3;
  
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xfeff;
  uVar2 = *(ushort *)(param_1 + 4);
  uVar1 = *param_2;
  uVar3 = *(ushort *)(param_1 + 0x20) & 0xfdff | param_2[6] << 8 | param_2[1] << 8;
  if (param_1 == 0x40012c00) {
    uVar3 = (uVar3 & 0xf7ff | param_2[7] << 8) & 0xfbff | param_2[2] << 8;
    uVar2 = uVar2 & 0xcfff | param_2[8] << 4 | param_2[9] << 4;
  }
  *(ushort *)(param_1 + 4) = uVar2;
  *(ushort *)(param_1 + 0x1c) = *(ushort *)(param_1 + 0x1c) & 0xff8c | uVar1;
  *(undefined4 *)(param_1 + 0x3c) = *(undefined4 *)(param_2 + 4);
  *(ushort *)(param_1 + 0x20) = uVar3;
  return;
}


/* ==== FUN_0800B174 @ 0x800b174 ==== */

void FUN_0800B174(int param_1,short *param_2)

{
  short sVar1;
  short sVar2;
  short sVar3;
  ushort uVar4;
  
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xefff;
  uVar4 = *(ushort *)(param_1 + 4);
  sVar1 = *param_2;
  sVar2 = param_2[6];
  sVar3 = param_2[1];
  if (param_1 == 0x40012c00) {
    uVar4 = uVar4 & 0xbfff | param_2[8] << 6;
  }
  *(ushort *)(param_1 + 4) = uVar4;
  *(ushort *)(param_1 + 0x1c) = *(ushort *)(param_1 + 0x1c) & 0x8cff | sVar1 << 8;
  *(undefined4 *)(param_1 + 0x40) = *(undefined4 *)(param_2 + 4);
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xdfff | sVar2 << 0xc | sVar3 << 0xc;
  return;
}


/* ==== FUN_0800b1d8 @ 0x800b1d8 ==== */

void FUN_0800b1d8(undefined2 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  *(undefined4 *)(param_1 + 4) = 0;
  param_1[6] = 0;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  return;
}


/* ==== FUN_0800b1ec @ 0x800b1ec ==== */

void FUN_0800b1ec(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x34) = param_2;
  return;
}


/* ==== FUN_0800b1f0 @ 0x800b1f0 ==== */

void FUN_0800b1f0(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x38) = param_2;
  return;
}


/* ==== FUN_0800b1f4 @ 0x800b1f4 ==== */

void FUN_0800b1f4(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x3c) = param_2;
  return;
}


/* ==== FUN_0800b1f8 @ 0x800b1f8 ==== */

void FUN_0800b1f8(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x40) = param_2;
  return;
}


/* ==== FUN_0800b1fc @ 0x800b1fc ==== */

void FUN_0800b1fc(int param_1,ushort param_2)

{
  *(ushort *)(param_1 + 0x18) = param_2 | *(ushort *)(param_1 + 0x18) & 0xfff7;
  return;
}


/* ==== FUN_0800b208 @ 0x800b208 ==== */

void FUN_0800b208(int param_1,short param_2)

{
  *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & 0xf7ff | param_2 << 8;
  return;
}


/* ==== FUN_0800b220 @ 0x800b220 ==== */

void FUN_0800b220(int param_1,ushort param_2)

{
  *(ushort *)(param_1 + 0x1c) = param_2 | *(ushort *)(param_1 + 0x1c) & 0xfff7;
  return;
}


/* ==== FUN_0800b22c @ 0x800b22c ==== */

void FUN_0800b22c(int param_1,short param_2)

{
  *(ushort *)(param_1 + 0x1c) = *(ushort *)(param_1 + 0x1c) & 0xf7ff | param_2 << 8;
  return;
}


/* ==== FUN_0800B244 @ 0x800b244 ==== */

void FUN_0800B244(int param_1,uint param_2,int param_3)

{
  *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & ~(ushort)(1 << (param_2 & 0xff));
  *(ushort *)(param_1 + 0x20) = (ushort)(param_3 << (param_2 & 0xff)) | *(ushort *)(param_1 + 0x20);
  return;
}


/* ==== FUN_0800b260 @ 0x800b260 ==== */

void FUN_0800b260(undefined2 *param_1)

{
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 1;
  param_1[3] = 0;
  param_1[4] = 0;
  return;
}


/* ==== FUN_0800b270 @ 0x800b270 ==== */

void FUN_0800b270(int param_1,ushort param_2)

{
  *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & 0xfff3;
  *(ushort *)(param_1 + 0x18) = param_2 | *(ushort *)(param_1 + 0x18);
  return;
}


/* ==== FUN_0800b280 @ 0x800b280 ==== */

void FUN_0800b280(int param_1,short param_2)

{
  *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & 0xf3ff;
  *(ushort *)(param_1 + 0x18) = param_2 << 8 | *(ushort *)(param_1 + 0x18);
  return;
}


/* ==== FUN_0800b298 @ 0x800b298 ==== */

void FUN_0800b298(int param_1,ushort param_2)

{
  *(ushort *)(param_1 + 0x1c) = *(ushort *)(param_1 + 0x1c) & 0xfff3;
  *(ushort *)(param_1 + 0x1c) = param_2 | *(ushort *)(param_1 + 0x1c);
  return;
}


/* ==== FUN_0800b2a8 @ 0x800b2a8 ==== */

void FUN_0800b2a8(int param_1,short param_2)

{
  *(ushort *)(param_1 + 0x1c) = *(ushort *)(param_1 + 0x1c) & 0xf3ff;
  *(ushort *)(param_1 + 0x1c) = param_2 << 8 | *(ushort *)(param_1 + 0x1c);
  return;
}


/* ==== FUN_0800B2C0 @ 0x800b2c0 ==== */

void FUN_0800B2C0(int param_1,short *param_2,undefined4 param_3,undefined4 param_4)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  ushort uVar4;
  
  sVar1 = *param_2;
  if (sVar1 == 0) {
    FUN_0800AE5C(param_1,param_2[1],param_2[2],param_2[4]);
    FUN_0800b270(param_1,param_2[3]);
  }
  else if (sVar1 == 4) {
    FUN_0800AE88(param_1,param_2[1],param_2[2],param_2[4]);
    FUN_0800b280(param_1,param_2[3]);
  }
  else if (sVar1 == 8) {
    sVar1 = param_2[1];
    uVar2 = param_2[2];
    sVar3 = param_2[4];
    *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xfeff;
    *(ushort *)(param_1 + 0x1c) = *(ushort *)(param_1 + 0x1c) & 0xff0c | sVar3 << 4 | uVar2;
    *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xf5ff | sVar1 << 8 | 0x100;
    FUN_0800b298(param_1,param_2[3]);
  }
  else {
    uVar2 = param_2[1];
    sVar1 = param_2[2];
    sVar3 = param_2[4];
    *(ushort *)(param_1 + 0x20) = *(ushort *)(param_1 + 0x20) & 0xefff;
    uVar4 = *(ushort *)(param_1 + 0x1c) & 0xcff | sVar1 << 8 | sVar3 << 0xc;
    *(ushort *)(param_1 + 0x1c) = uVar4;
    *(ushort *)(param_1 + 0x20) =
         *(ushort *)(param_1 + 0x20) & 0x5fff | (ushort)((uint)uVar2 << 0xc) | 0x1000;
    FUN_0800b2a8(param_1,param_2[3],uVar4,(uint)uVar2 << 0xc,param_4);
  }
  return;
}


/* ==== FUN_0800B38C @ 0x800b38c ==== */

void FUN_0800B38C(int param_1,ushort param_2,int param_3)

{
  if (param_3 == 0) {
    *(ushort *)(param_1 + 0xc) = *(ushort *)(param_1 + 0xc) & ~param_2;
  }
  else {
    *(ushort *)(param_1 + 0xc) = param_2 | *(ushort *)(param_1 + 0xc);
  }
  return;
}


/* ==== FUN_0800b3a4 @ 0x800b3a4 ==== */

void FUN_0800b3a4(int param_1,undefined2 param_2)

{
  *(undefined2 *)(param_1 + 0x14) = param_2;
  return;
}


/* ==== FUN_0800B3A8 @ 0x800b3a8 ==== */

bool FUN_0800B3A8(int param_1,ushort param_2)

{
  return (*(ushort *)(param_1 + 0x10) & param_2) != 0 && (*(ushort *)(param_1 + 0xc) & param_2) != 0
  ;
}


/* ==== FUN_0800b3c0 @ 0x800b3c0 ==== */

void FUN_0800b3c0(int param_1,ushort param_2)

{
  *(ushort *)(param_1 + 0x10) = ~param_2;
  return;
}


/* ==== FUN_0800B3C8 @ 0x800b3c8 ==== */

void FUN_0800B3C8(int param_1,ushort param_2,int param_3)

{
  if (param_3 == 0) {
    *(ushort *)(param_1 + 0xc) = *(ushort *)(param_1 + 0xc) & ~param_2;
  }
  else {
    *(ushort *)(param_1 + 0xc) = param_2 | *(ushort *)(param_1 + 0xc);
  }
  return;
}


/* ==== FUN_0800b3e0 @ 0x800b3e0 ==== */

void FUN_0800b3e0(int param_1,ushort param_2)

{
  *(ushort *)(param_1 + 8) = param_2 | *(ushort *)(param_1 + 8) & 0xff8f;
  return;
}


/* ==== FUN_0800b3ec @ 0x800b3ec ==== */

void FUN_0800b3ec(int param_1,ushort param_2)

{
  *(ushort *)(param_1 + 8) = *(ushort *)(param_1 + 8) & 0xfff8;
  *(ushort *)(param_1 + 8) = param_2 | *(ushort *)(param_1 + 8);
  return;
}


/* ==== FUN_0800B3FC @ 0x800b3fc ==== */

void FUN_0800B3FC(uint *param_1,uint *param_2)

{
  int iVar1;
  uint extraout_r1;
  uint extraout_r1_00;
  int iVar2;
  uint uVar3;
  ushort uVar4;
  undefined1 auStack_40 [8];
  int local_38;
  int local_28;
  
  *param_1 = *param_1 & 0xfffffffe;
  param_1[1] = param_1[1] & 0xffffcfff | param_2[2];
  *param_1 = param_2[3] | param_2[1] | param_2[4] | *param_1 & 0xffffe9f3;
  param_1[2] = param_1[2] & 0xfffffcff | param_2[5];
  FUN_0800AC40(auStack_40);
  iVar2 = local_38;
  if (param_1 == (uint *)0x40013800) {
    iVar2 = local_28;
  }
  if ((int)(*param_1 << 0x10) < 0) {
    uVar3 = *param_2;
    iVar1 = FUN_08000128(iVar2 * 2,uVar3);
    FUN_080001b0(iVar2 * 2,uVar3);
    uVar3 = extraout_r1;
  }
  else {
    uVar3 = *param_2;
    iVar1 = FUN_08000128(iVar2);
    FUN_080001b0(iVar2,uVar3);
    uVar3 = extraout_r1_00;
  }
  uVar3 = iVar1 + (uint)(*param_2 >> 1 <= uVar3);
  uVar4 = (ushort)uVar3;
  if ((int)(*param_1 << 0x10) < 0) {
    uVar4 = uVar4 & 0xfff0 | (ushort)((uVar3 & 0xf) >> 1);
  }
  *(ushort *)(param_1 + 3) = uVar4;
  return;
}


/* ==== FUN_0800B4C0 @ 0x800b4c0 ==== */

void FUN_0800B4C0(uint *param_1,int param_2)

{
  if (param_2 == 0) {
    *param_1 = *param_1 & 0xfffffffe;
  }
  else {
    *param_1 = *param_1 | 1;
  }
  return;
}


/* ==== FUN_0800b4dc @ 0x800b4dc ==== */

void FUN_0800b4dc(int param_1,ushort param_2)

{
  *(ushort *)(param_1 + 0x28) = param_2 & 0x1ff;
  return;
}


/* ==== FUN_0800b4e4 @ 0x800b4e4 ==== */

ushort FUN_0800b4e4(int param_1)

{
  return *(ushort *)(param_1 + 0x24) & 0x1ff;
}


/* ==== FUN_0800B4EC @ 0x800b4ec ==== */

void FUN_0800B4EC(uint *param_1,uint param_2,int param_3)

{
  uint uVar1;
  uint uVar2;
  
  uVar1 = (param_2 & 0xffff) >> 8;
  uVar2 = 1 << (param_2 & 0xff);
  if (uVar1 == 2) {
    param_1 = param_1 + 1;
  }
  else if (uVar1 == 3) {
    param_1 = param_1 + 2;
  }
  if (param_3 == 0) {
    *param_1 = *param_1 & ~uVar2;
  }
  else {
    *param_1 = uVar2 | *param_1;
  }
  return;
}


/* ==== FUN_0800B524 @ 0x800b524 ==== */

void FUN_0800B524(int param_1,ushort param_2,int param_3)

{
  if (param_3 == 0) {
    *(ushort *)(param_1 + 0x18) = *(ushort *)(param_1 + 0x18) & ~param_2;
  }
  else {
    *(ushort *)(param_1 + 0x18) = param_2 | *(ushort *)(param_1 + 0x18);
  }
  return;
}


/* ==== FUN_0800b53c @ 0x800b53c ==== */

bool FUN_0800b53c(int param_1,uint param_2)

{
  return (*(uint *)(param_1 + 0x1c) & param_2) != 0;
}


/* ==== FUN_0800b548 @ 0x800b548 ==== */

void FUN_0800b548(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x20) = param_2;
  return;
}


/* ==== FUN_0800B54C @ 0x800b54c ==== */

bool FUN_0800B54C(uint *param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  bool bVar3;
  
  uVar1 = (param_2 & 0xffff) >> 8;
  uVar2 = 1 << (param_2 & 0xff);
  if (uVar1 == 1) {
    uVar2 = uVar2 & *param_1;
  }
  else if (uVar1 == 2) {
    uVar2 = uVar2 & param_1[1];
  }
  else {
    uVar2 = uVar2 & param_1[2];
  }
  bVar3 = false;
  if (uVar2 != 0) {
    bVar3 = (1 << (param_2 >> 0x10 & 0xff) & param_1[7]) != 0;
  }
  return bVar3;
}


/* ==== FUN_0800B58C @ 0x800b58c ==== */

// WARNING: Removing unreachable block (ram,0x0800b5c0)
// WARNING: Removing unreachable block (ram,0x0800b5cc)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800B58C(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  
  if (_DAT_20000c20 == 0) {
    _DAT_20000c20 = 0x20000c24;
  }
  iVar1 = _DAT_20000c20;
  if (*(int *)(_DAT_20000c20 + 4) < 0x20) {
    uVar4 = *(uint *)(_DAT_20000c20 + 4);
    if (param_1 != 0) {
      *(undefined4 *)(_DAT_20000c20 + uVar4 * 4 + 0x88) = param_3;
      uVar3 = 1 << (uVar4 & 0xff);
      *(uint *)(iVar1 + 0x188) = *(uint *)(iVar1 + 0x188) | uVar3;
      *(undefined4 *)(iVar1 + (uVar4 + 0x20) * 4 + 0x88) = param_4;
      if (param_1 == 2) {
        *(uint *)(iVar1 + 0x18c) = uVar3 | *(uint *)(iVar1 + 0x18c);
      }
    }
    *(undefined4 *)((uVar4 + 2) * 4 + iVar1) = param_2;
    *(uint *)(iVar1 + 4) = uVar4 + 1;
    uVar2 = 0;
  }
  else {
    uVar2 = 0xffffffff;
  }
  return uVar2;
}


/* ==== FUN_0800B650 @ 0x800b650 ==== */

// WARNING: Removing unreachable block (ram,0x0800b676)
// WARNING: Removing unreachable block (ram,0x0800b67c)
// WARNING: Removing unreachable block (ram,0x0800b68a)

void FUN_0800B650(void)

{
  FUN_080000C8();
  FUN_0800B814();
  return;
}


/* ==== FUN_0800B6B0 @ 0x800b6b0 ==== */

// WARNING: Removing unreachable block (ram,0x0800b71c)
// WARNING: Removing unreachable block (ram,0x0800b6bc)
// WARNING: Removing unreachable block (ram,0x0800b6ce)
// WARNING: Removing unreachable block (ram,0x0800b6d2)
// WARNING: Removing unreachable block (ram,0x0800b6dc)

void FUN_0800B6B0(void)

{
  FUN_0800B808();
  FUN_0800B7E0();
  FUN_080000F0();
  return;
}


/* ==== FUN_0800b740 @ 0x800b740 ==== */

int FUN_0800b740(byte *param_1,byte *param_2)

{
  byte bVar1;
  uint uVar2;
  
  do {
    uVar2 = (uint)*param_1;
    bVar1 = *param_2;
    param_1 = param_1 + 1;
    param_2 = param_2 + 1;
    if (uVar2 == 0) break;
  } while (uVar2 == bVar1);
  return uVar2 - bVar1;
}


/* ==== FUN_0800b754 @ 0x800b754 ==== */

int FUN_0800b754(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = 0;
  do {
    iVar2 = iVar1;
    iVar1 = iVar2 + 1;
  } while (*(char *)(param_1 + iVar2) != '\0');
  return iVar2;
}


/* ==== FUN_0800B764 @ 0x800b764 ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_0800B764(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  FUN_0800B778(param_1,param_2,_DAT_20000f00 + 0x5c,1,param_4);
  return;
}


/* ==== FUN_0800B778 @ 0x800b778 ==== */

char * FUN_0800B778(char *param_1,char *param_2,undefined4 *param_3,int param_4)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;
  char *pcVar4;
  
  if ((param_1 == (char *)0x0) && (param_1 = (char *)*param_3, (char *)*param_3 == (char *)0x0)) {
    pcVar2 = (char *)0x0;
  }
  else {
    do {
      pcVar2 = param_1;
      param_1 = pcVar2 + 1;
      pcVar3 = param_2;
      do {
        cVar1 = *pcVar3;
        pcVar3 = pcVar3 + 1;
        if (cVar1 == '\0') {
          if (*pcVar2 == '\0') {
            *param_3 = 0;
            return (char *)0x0;
          }
          do {
            pcVar4 = param_1;
            param_1 = pcVar4 + 1;
            pcVar3 = param_2;
            do {
              cVar1 = *pcVar3;
              pcVar3 = pcVar3 + 1;
              if (*pcVar4 == cVar1) {
                if (*pcVar4 == '\0') {
                  param_1 = (char *)0x0;
                }
                else {
                  *pcVar4 = '\0';
                }
                *param_3 = param_1;
                return pcVar2;
              }
            } while (cVar1 != '\0');
          } while( true );
        }
      } while (*pcVar2 != cVar1);
    } while (param_4 != 0);
    *param_3 = param_1;
    *pcVar2 = '\0';
  }
  return pcVar2;
}


/* ==== FUN_0800B7E0 @ 0x800b7e0 ==== */

// WARNING: Removing unreachable block (ram,0x0800b7e8)

void FUN_0800B7E0(void)

{
  return;
}


/* ==== FUN_0800B7F8 @ 0x800b7f8 ==== */

void FUN_0800B7F8(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  FUN_0800B58C(0,param_1,0,0,param_4);
  return;
}


/* ==== FUN_0800B808 @ 0x800b808 ==== */

void FUN_0800B808(void)

{
  return;
}


/* ==== FUN_0800B814 @ 0x800b814 ==== */

void FUN_0800B814(void)

{
  return;
}


/* ==== FUN_0800bfd4 @ 0x800bfd4 ==== */

// WARNING: Control flow encountered bad instruction data

void FUN_0800bfd4(int param_1,undefined4 param_2,int param_3)

{
  uint uVar1;
  uint unaff_r5;
  undefined4 unaff_r7;
  undefined4 unaff_r10;
  undefined4 unaff_r11;
  bool in_ZR;
  
  if (in_ZR) {
    uVar1 = unaff_r5 & param_1 << 6 & param_3 >> 0x20;
    *(BADSPACEBASE **)uVar1 = register0x00000054;
    *(undefined4 *)(uVar1 - 4) = unaff_r11;
    *(undefined4 *)(uVar1 - 8) = unaff_r10;
    *(undefined4 *)(uVar1 - 0xc) = unaff_r7;
    *(uint *)(uVar1 - 0x10) = unaff_r5;
    *(int *)(uVar1 - 0x14) = param_3;
    *(uint *)(uVar1 - 0x18) = uVar1;
  }
                    // WARNING: Bad instruction - Truncating control flow here
  halt_baddata();
}


