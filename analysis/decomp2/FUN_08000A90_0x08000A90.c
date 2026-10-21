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


