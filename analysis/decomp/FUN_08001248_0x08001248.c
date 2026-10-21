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


