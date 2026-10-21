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


