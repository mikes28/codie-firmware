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


