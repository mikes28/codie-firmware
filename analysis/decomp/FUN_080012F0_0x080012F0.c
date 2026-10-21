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


