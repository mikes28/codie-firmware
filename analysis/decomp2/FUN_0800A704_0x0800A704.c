/* ==== FUN_0800A704 @ 0x800a704 ==== */

undefined4 FUN_0800A704(int param_1)

{
  undefined4 uVar1;
  uint uVar2;
  int iVar3;
  
  *(uint *)(param_1 + 8) = *(uint *)(param_1 + 8) | 0x80000000;
  if (*(int *)(param_1 + 8) < 0) {
    uVar2 = *(uint *)(param_1 + 8);
    iVar3 = 0xeffe;
    do {
      if (((uVar2 & 0x80000000) == 0) || (-1 < *(int *)(param_1 + 8))) break;
      uVar2 = *(uint *)(param_1 + 8);
      iVar3 = iVar3 + -2;
    } while (iVar3 != 0);
  }
  if (*(int *)(param_1 + 8) < 0) {
    uVar1 = 0;
  }
  else {
    uVar1 = *(undefined4 *)(param_1 + 0x40);
  }
  return uVar1;
}


