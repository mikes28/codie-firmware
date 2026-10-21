/* ==== FUN_08000400 @ 0x8000400 ==== */

undefined8 FUN_08000400(uint param_1,int param_2,undefined4 param_3,undefined4 param_4,int *param_5)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  uVar1 = FUN_080021A8();
  uVar2 = FUN_0800033c(param_3,param_4,(int)uVar1,(int)((ulonglong)uVar1 >> 0x20));
  *param_5 = param_1 - (uint)uVar2;
  param_5[1] = (param_2 - (int)((ulonglong)uVar2 >> 0x20)) - (uint)(param_1 < (uint)uVar2);
  return uVar1;
}


