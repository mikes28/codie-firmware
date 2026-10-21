/* ==== FUN_0800043C @ 0x800043c ==== */

undefined8 FUN_0800043C(uint param_1,int param_2,undefined4 param_3,undefined4 param_4,int *param_5)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  uVar1 = FUN_08002578();
  uVar2 = FUN_0800033c((int)uVar1,(int)((ulonglong)uVar1 >> 0x20),param_3,param_4);
  *param_5 = param_1 - (uint)uVar2;
  param_5[1] = (param_2 - (int)((ulonglong)uVar2 >> 0x20)) - (uint)(param_1 < (uint)uVar2);
  return uVar1;
}


