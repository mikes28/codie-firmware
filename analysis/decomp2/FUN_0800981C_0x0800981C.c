/* ==== FUN_0800981C @ 0x800981c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_0800981C(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined8 uVar2;
  
  uVar2 = CONCAT44(param_2,param_1);
  if (param_1 == 3) {
    iVar1 = FUN_0800b740(param_2,"pwrOff");
    if (iVar1 != 0) {
      return 0;
    }
    uVar2 = FUN_080097E8();
  }
  if (((int)uVar2 == 2) &&
     (iVar1 = FUN_0800b740((int)((ulonglong)uVar2 >> 0x20),"Vsens"), iVar1 == 0)) {
    iVar1 = FUN_0800A084(param_3);
    if (iVar1 == 0) {
      _DAT_48000818 = 0x8000;
    }
    else {
      _DAT_48000828 = 0x8000;
    }
  }
  return 0;
}


