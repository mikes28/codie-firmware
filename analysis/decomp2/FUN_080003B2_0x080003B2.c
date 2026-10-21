/* ==== FUN_080003B2 @ 0x80003b2 ==== */

void FUN_080003B2(uint param_1,uint param_2,undefined4 param_3,uint param_4)

{
  undefined8 uVar1;
  undefined8 uVar2;
  
  if ((param_4 < param_2) && ((param_1 & 0x7ff) != 0)) {
    param_1 = param_1 & 0xfffff800 | 0x800;
  }
  uVar1 = FUN_08001FD4(param_2);
  uVar1 = FUN_08001AF0((int)uVar1,(int)((ulonglong)uVar1 >> 0x20),0,0x41f00000);
  uVar2 = FUN_08001FD4(param_1);
  FUN_08001398((int)uVar2,(int)((ulonglong)uVar2 >> 0x20),(int)uVar1,(int)((ulonglong)uVar1 >> 0x20)
              );
  FUN_08002038();
  return;
}


