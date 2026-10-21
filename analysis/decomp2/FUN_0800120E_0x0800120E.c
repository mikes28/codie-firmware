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


