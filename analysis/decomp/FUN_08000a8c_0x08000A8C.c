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


