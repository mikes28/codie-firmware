/* ==== FUN_08006F2C @ 0x8006f2c ==== */

// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_08006F2C(int param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined1 auStack_14 [12];
  
  if (param_1 == 1) {
    iVar1 = FUN_0800b740(param_2,"voltage");
    if (iVar1 == 0) {
      uVar2 = FUN_080068D4(_DAT_200004a4,auStack_14,0x10);
      FUN_0800A16C(2,"drvBattery",param_2,uVar2);
      uVar2 = 1;
    }
    else {
      iVar1 = FUN_0800b740(param_2,&DAT_0800c4b0);
      if (iVar1 == 0) {
        uVar2 = FUN_0800693C((int)_DAT_200004a2,auStack_14,0x10);
        FUN_0800A16C(2,"drvBattery",param_2,uVar2);
        uVar2 = 1;
      }
      else {
        iVar1 = FUN_0800b740(param_2,"current");
        if (iVar1 == 0) {
          uVar2 = FUN_0800693C((int)_DAT_200004a6,auStack_14,0x10);
          FUN_0800A16C(2,"drvBattery",param_2,uVar2);
          uVar2 = 1;
        }
        else {
          iVar1 = FUN_0800b740(param_2,"remainingCapacity");
          if (iVar1 == 0) {
            uVar2 = FUN_0800693C((int)_DAT_200004a8,auStack_14,0x10);
            FUN_0800A16C(2,"drvBattery",param_2,uVar2);
            uVar2 = 1;
          }
          else {
            iVar1 = FUN_0800b740(param_2,"chargeStat");
            uVar2 = 0;
            if (iVar1 == 0) {
              uVar2 = FUN_08007CFC(0);
              uVar2 = FUN_080068D4(uVar2,auStack_14,0x10);
              FUN_0800A16C(2,"drvBattery",param_2,uVar2);
              uVar2 = 1;
            }
          }
        }
      }
    }
  }
  else {
    uVar2 = 0;
    if (param_1 == 3) {
      iVar1 = FUN_0800b740(param_2,"hwInit");
      if (iVar1 == 0) {
        while (iVar1 = FUN_08006C44(), iVar1 != 0) {
          FUN_0800ab10();
        }
        FUN_08006E94(0x73a);
        uVar2 = 1;
      }
      else {
        uVar2 = 0;
      }
    }
  }
  return uVar2;
}


