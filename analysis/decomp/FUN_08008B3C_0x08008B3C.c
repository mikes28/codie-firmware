/* ==== FUN_08008B3C @ 0x8008b3c ==== */

undefined4 FUN_08008B3C(int param_1,undefined4 param_2,undefined4 param_3)

{
  short sVar1;
  int iVar2;
  undefined4 uVar3;
  
  if (param_1 == 2) {
    iVar2 = FUN_0800b740(param_2,"dutyA");
    if (iVar2 == 0) {
      sVar1 = FUN_080069D4(param_3,0x10);
      FUN_080089D0((int)sVar1);
      uVar3 = 1;
    }
    else {
      iVar2 = FUN_0800b740(param_2,"dutyB");
      uVar3 = 0;
      if (iVar2 == 0) {
        sVar1 = FUN_080069D4(param_3,0x10);
        FUN_08008A50((int)sVar1);
        uVar3 = 1;
      }
    }
  }
  else {
    uVar3 = 0;
    if ((param_1 == 1) && (iVar2 = FUN_0800b740(param_2,"nFault"), iVar2 == 0)) {
      FUN_0800aabc(0x48000800,0x4000);
      FUN_0800A16C(2,"hwiDrive",param_2);
      uVar3 = 1;
    }
  }
  return uVar3;
}


