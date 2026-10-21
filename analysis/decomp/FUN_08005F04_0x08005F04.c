/* ==== FUN_08005F04 @ 0x8005f04 ==== */

undefined4 FUN_08005F04(int param_1)

{
  int iVar1;
  
  if (param_1 == 4) {
    iVar1 = 10;
  }
  else if (param_1 == 0x108) {
    iVar1 = 9;
  }
  else if (param_1 == 0x107) {
    iVar1 = 8;
  }
  else if (param_1 == 0x106) {
    iVar1 = 7;
  }
  else if (param_1 == 0x1ff) {
    iVar1 = 6;
  }
  else if (param_1 == 0x105) {
    iVar1 = 5;
  }
  else if (param_1 == 3) {
    iVar1 = 4;
  }
  else if (param_1 == 0x104) {
    iVar1 = 3;
  }
  else if (param_1 == 0x103) {
    iVar1 = 2;
  }
  else if (param_1 == 2) {
    iVar1 = 1;
  }
  else {
    if (param_1 != 1) {
      return 0;
    }
    iVar1 = 0;
  }
  return (&DAT_0800c414)[iVar1 * 2];
}


