/* ==== FUN_080089D0 @ 0x80089d0 ==== */

void FUN_080089D0(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = param_4;
  if (1000 < (int)param_1) {
    uVar1 = 0xfa;
    param_1 = 1000;
  }
  if ((short)param_1 < -1000) {
    param_1 = 0xfffffc18;
  }
  iVar2 = (int)(short)param_1;
  if (iVar2 < 1) {
    if (iVar2 < 0) {
      FUN_0800b1ec(0x40000400,0,param_1,uVar1,param_4);
      FUN_0800b1f0(0x40000400,-iVar2 & 0xffff);
    }
    else {
      FUN_0800b1ec(0x40000400,(uint)DAT_20000700 * 10,param_1,(uint)DAT_20000700,param_4);
      FUN_0800b1f0(0x40000400,(uint)DAT_20000700 * 10);
    }
  }
  else {
    FUN_0800b1ec(0x40000400,param_1 & 0xffff,param_1,uVar1,param_4);
    FUN_0800b1f0(0x40000400,0);
  }
  return;
}


