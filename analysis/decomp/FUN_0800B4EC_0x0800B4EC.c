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


