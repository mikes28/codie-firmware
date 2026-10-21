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


