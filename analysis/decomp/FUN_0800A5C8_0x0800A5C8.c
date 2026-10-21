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


